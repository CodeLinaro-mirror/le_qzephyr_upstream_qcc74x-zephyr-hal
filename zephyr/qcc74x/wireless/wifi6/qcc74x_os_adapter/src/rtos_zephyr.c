#include "rtos_def.h"
#include "rtos_al.h"

#include <zephyr/kernel.h>  // Zephyr核心头文件
#include <zephyr/irq.h>     // 中断相关

#include <stdbool.h>

#include "soc_mem.h"

#define TST_SHRAM_PTR(...)
#define ASSERT_ERR(...) //__ASSERT(0, "")
#define ASSERT(...)
#define user_debug(...) //printf
#define user_err        printf

/* 定义任务句柄结构体 */
struct rtos_task_handle_internal {
    struct k_thread *thread;
    k_thread_stack_t *stack;
    // enum rtos_task_id task_id;
    size_t stack_size;
} rtos_task_handle_internal_t;

/* 回收队列和初始化标志 */
static K_FIFO_DEFINE(recycle_fifo);
static volatile int rtos_initialized = 0;
rtos_queue g_idle_queue;

/* 空闲任务栈大小 */
#define IDLE_STACK_SIZE 4096

#define RTOS_IDLE_TASK_PRI       (15)

// Zephyr优先级转换（根据实际需求调整数值）
#undef RTOS_TASK_PRIORITY
#define RTOS_TASK_PRIORITY(prio) K_PRIO_PREEMPT(32 - prio)

const int fhost_tcpip_priority = RTOS_TASK_PRIORITY(28);
const int fhost_wifi_priority = RTOS_TASK_PRIORITY(27);
const int fhost_wifi_priority_high = RTOS_TASK_PRIORITY(30);
const int fhost_cntrl_priority = RTOS_TASK_PRIORITY(27);
#if defined(CONFIG_HIGH_PERFORMANCE) && (CONFIG_HIGH_PERFORMANCE == 1)  && CFG_LINK_ALL_IN_FLASH
const int fhost_rx_priority = RTOS_TASK_PRIORITY(30);
#else
const int fhost_rx_priority = RTOS_TASK_PRIORITY(27);
#endif
const int fhost_tx_priority = RTOS_TASK_PRIORITY(29);
const int fhost_wpa_priority = RTOS_TASK_PRIORITY(26);
const int fhost_ipc_priority = RTOS_TASK_PRIORITY(29);
const int fhost_iperf_priority = RTOS_TASK_PRIORITY(27);
const int fhost_connect_priority = RTOS_TASK_PRIORITY(2);
const int fhost_tg_priority = RTOS_TASK_PRIORITY(5);
const int fhost_ping_priority = RTOS_TASK_PRIORITY(27);

/*
 * FUNCTIONS
 ****************************************************************************************
 */
uint32_t rtos_now(bool isr)
{
    return k_uptime_ticks(); // 获取系统tick数
}

/* 内存回收任务 */
static void recycle_task(void *p1, void *p2, void *p3)
{
    struct rtos_task_handle_internal *handle = NULL;
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    user_debug("recycle_task entry\r\n");

    while (1) {
        rtos_queue_read(g_idle_queue, &handle, -1, 0);

        user_debug("fifo get handler:%p, thread:%p\r\n", handle, handle->thread);
        user_debug("===========\r\n");
        // k_msleep(100);

        /* 等待线程完全退出 */
        // user_debug("-------------abort handle->thread :%p\r\n", handle->thread);
        // k_thread_abort(handle->thread);
        if (k_thread_join(handle->thread, K_FOREVER) == 0) {
            /* 释放资源 */
            if (handle->stack) {
                user_debug("free stack:%p\r\n", handle->stack);
                soc_multi_free(handle->stack);
            }
            if (handle->thread) {
                user_debug("free thread:%p\r\n", handle->thread);
                soc_multi_free(handle->thread);
            }
            user_debug("free handle:%p\r\n", handle);
            soc_multi_free(handle);

            k_object_release(handle);
        }
    }
}

int rtos_task_init(void)
{
    if (rtos_initialized) {
        return -1;
    }

    user_debug("rtos_task_init\r\n");

    /* 创建内存回收线程 */
    static k_thread_stack_t *idle_stack;
    static struct k_thread idle_thread;

    idle_stack = soc_multi_malloc(IDLE_STACK_SIZE);
    if (!idle_stack) {
        user_err("soc_multi_malloc NULL\r\n");
        return -ENOMEM;
    }

    k_thread_create(&idle_thread, idle_stack, IDLE_STACK_SIZE,
                   recycle_task, NULL, NULL, NULL,
                   RTOS_IDLE_TASK_PRI, 0, K_NO_WAIT);
	if (IS_ENABLED(CONFIG_THREAD_NAME)) {
		k_thread_name_set(&idle_thread, "RecycleTask");
	}
    rtos_queue_create(sizeof(rtos_task_handle_internal_t), 40, &g_idle_queue);

    rtos_initialized = 1;
    
    return 0;
}

int rtos_task_create(rtos_task_fct func,
    const char * const name,
    enum rtos_task_id task_id,
    const uint16_t stack_depth,
    void * const params,
    rtos_prio prio,
    rtos_task_handle * const task_handle)
{
    ARG_UNUSED(task_id);
    /* 初始化检查 */
    rtos_task_init();

    /* 分配栈空间 */
    k_thread_stack_t *stack = soc_multi_malloc(stack_depth * 4);
    user_debug("alloc stack:%p\r\n", stack);
    if (!stack) {
        return -ENOMEM;
    }

    /* 分配线程控制块 */
    struct k_thread *thread = soc_multi_malloc(sizeof(struct k_thread));
    user_debug("alloc thread:%p\r\n", thread);
    if (!thread) {
        soc_multi_free(stack);
        return -ENOMEM;
    }

    /* 分配句柄结构体 */
    struct rtos_task_handle_internal *handle = soc_multi_malloc(sizeof(*handle));
    user_debug("alloc handle:%p\r\n", handle);
    if (!handle) {
        soc_multi_free(stack);
        soc_multi_free(thread);
        return -ENOMEM;
    }

    /* 初始化线程 */
    *handle = (struct rtos_task_handle_internal){
        .thread = thread,
        .stack = stack,
        // .task_id = task_id,
        .stack_size = stack_depth * 4
    };

    /* 创建线程 */
    k_tid_t tid = k_thread_create(thread, stack, stack_depth * 4,
                                 (k_thread_entry_t)func, params, handle, NULL,// p1, p2_handle, p3
                                 prio, 0, K_NO_WAIT);
    if (!tid) {
        soc_multi_free(stack);
        soc_multi_free(thread);
        soc_multi_free(handle);
        return -EINVAL;
    }

	if (IS_ENABLED(CONFIG_THREAD_NAME)) {
		k_thread_name_set(tid, name);
	}

    /* 设置任务ID到自定义数据 */
    k_thread_custom_data_set(handle);

    user_debug("-------------create handle->thread :%p\r\n", handle->thread);

    if (task_handle) {
        *task_handle = handle;
    }

    return 0;
}

void rtos_task_delete(void *task_handle)
{
    user_debug("fifo put handler:%p start\r\n", task_handle);
    if (!task_handle) {
        return;
    }

    /* 终止线程并加入回收队列 */
    // k_fifo_put(&recycle_fifo, task_handle);
    rtos_queue_write(g_idle_queue, &task_handle, -1, 0);
    user_debug("fifo put handler:%p end\r\n", task_handle);

    struct rtos_task_handle_internal *handle = task_handle;
    k_thread_abort(handle->thread);
}

void rtos_task_suspend(int duration)
{
    k_sleep(K_MSEC(duration)); // 任务休眠
}

int g_wifitask_sem_valid = 0;
static struct k_sem g_wifitask_sem;
int rtos_task_init_notification(rtos_task_handle task)
{
    if (!g_wifitask_sem_valid) {
        k_sem_init(&g_wifitask_sem, 0, 1);
        g_wifitask_sem_valid = 1;
    }

    return 0;
}

int rtos_task_wait_notification(int timeout)
{
    k_timeout_t t;

    if (timeout == -1) {
        t = K_FOREVER;
    } else if (timeout == 0) {
        t = K_NO_WAIT;
    } else {
        t = K_MSEC(timeout);
    }
    if (!g_wifitask_sem_valid) {
        while (1) {
            printf("g_wifitask_sem no init\r\n");
            k_msleep(1000);
        }
    }
    return (!k_sem_take(&g_wifitask_sem, t));
}

void rtos_task_notify(rtos_task_handle task, bool isr)
{
    if (!g_wifitask_sem_valid) {
        while (1) {
            printf("g_wifitask_sem no init\r\n");
            k_msleep(1000);
        }
    }
    k_sem_give(&g_wifitask_sem);
}

// 中断管理
uint32_t rtos_protect(void)
{
    return irq_lock();
}

void rtos_unprotect(uint32_t protect)
{
    irq_unlock(protect);
}

int rtos_init(void)
{
    rtos_task_init();
    return 0;
}

rtos_task_handle rtos_get_task_handle()
{
    return k_current_get();
}

void rtos_trace_task(int id, void *task)
{
}

void rtos_trace_mem(int id, void *ptr, int size, int free_size)
{
}

void rtos_priority_set(rtos_task_handle handle, rtos_prio priority)
{
}

#ifdef CFG_QCC74x_WIFI_PS_ENABLE
struct rtos_zephyr_timer {
    struct k_work_delayable work;
    rtos_timer_callback_t callback;
    void *data;
};

static void rtos_timer_work_handler(struct k_work *work)
{
    struct k_work_delayable *dwork = k_work_delayable_from_work(work);
    struct rtos_zephyr_timer *timer =
        CONTAINER_OF(dwork, struct rtos_zephyr_timer, work);

    if (timer->callback != NULL) {
        timer->callback(timer->data);
    }
}

rtos_timer_t rtos_timer_create(const char *name, uint32_t period_ms,
                               rtos_timer_callback_t callback, void *user_data)
{
    struct rtos_zephyr_timer *timer;

    ARG_UNUSED(name);
    ARG_UNUSED(period_ms);

    timer = soc_multi_malloc(sizeof(*timer));
    if (timer == NULL) {
        user_err("Failed to create timer.\r\n");
        return NULL;
    }

    timer->callback = callback;
    timer->data = user_data;
    k_work_init_delayable(&timer->work, rtos_timer_work_handler);

    return timer;
}

int rtos_timer_update_and_start(rtos_timer_t timer, uint32_t period_ms)
{
    if (timer == NULL) {
        user_err("Failed to update timer.\r\n");
        return -1;
    }

    (void)k_work_cancel_delayable(&timer->work);

    return k_work_reschedule(&timer->work, K_MSEC(period_ms)) < 0 ? -1 : 0;
}

int rtos_delete_timer(rtos_timer_t timer)
{
    if (timer == NULL) {
        user_err("Error: Timer handle is NULL\r\n");
        return -1;
    }

    if (!k_is_in_isr()) {
        struct k_work_sync sync;

        (void)k_work_cancel_delayable_sync(&timer->work, &sync);
    } else {
        (void)k_work_cancel_delayable(&timer->work);
    }

    soc_multi_free(timer);

    return 0;
}
#endif


/*--------------------------------------------------
 *                    互斥量
 *--------------------------------------------------*/
int rtos_mutex_create(rtos_mutex *mutex)
{
    *mutex = soc_multi_malloc(sizeof(struct k_mutex));
    if (*mutex) {
        k_mutex_init(*mutex);
        return 0;
    }
    return -1;
}

void rtos_mutex_delete(rtos_mutex mutex)
{
    soc_multi_free(mutex);
}

void rtos_mutex_lock(rtos_mutex mutex)
{
    k_mutex_lock(mutex, K_FOREVER);
}

void rtos_mutex_unlock(rtos_mutex mutex)
{
    k_mutex_unlock(mutex);
}

/*--------------------------------------------------
 *                    信号量
 *--------------------------------------------------*/
int rtos_semaphore_create(rtos_semaphore *semaphore, int max_count, int init_count)
{
    /* Zephyr 信号量统一使用 k_sem */
    *semaphore = soc_multi_malloc(sizeof(struct k_sem));
    if (*semaphore) {
        k_sem_init(*semaphore, init_count, max_count);
        return 0;
    }
    return -1;
}

void rtos_semaphore_delete(rtos_semaphore semaphore)
{
    soc_multi_free(semaphore);
}

int rtos_semaphore_get_count(rtos_semaphore semaphore)
{
    return k_sem_count_get(semaphore);
}

/**
* Attempt to take (acquire) a semaphore.
*
* @param sem     Pointer to the semaphore to take.
* @param timeout Timeout duration (e.g., K_NO_WAIT, K_FOREVER, K_MSEC(x)).
*
* @return 0      – Semaphore successfully taken (count decremented).
* @return -EBUSY – Semaphore unavailable and no waiting allowed (timeout == K_NO_WAIT).
* @return -EAGAIN– Semaphore not taken: either wait timed out or the semaphore was reset while waiting.
*/
int rtos_semaphore_wait(rtos_semaphore semaphore, int timeout)
{
    k_timeout_t t;

    if (timeout == -1) {
        t = K_FOREVER;
    } else if (timeout == 0) {
        t = K_NO_WAIT;
    } else {
        t = K_MSEC(timeout);
    }

    return k_sem_take(semaphore, t);
}

int rtos_semaphore_signal(rtos_semaphore semaphore, bool isr)
{
    /* Zephyr 允许在 ISR 中直接释放信号量 */
    k_sem_give(semaphore);
    return 0;
}

/*--------------------------------------------------
 *                    队列操作
 *--------------------------------------------------*/
/* 队列结构体 */
typedef struct {
    struct k_msgq msgq;
    void *buffer;
    int elt_size;
    int nb_elt;
} zephyr_queue_t;

/* 创建队列 */
int rtos_queue_create(int elt_size, int nb_elt, rtos_queue *queue)
{
    zephyr_queue_t *q = soc_multi_malloc(sizeof(zephyr_queue_t));
    if (!q) return -1;

    q->buffer = soc_multi_malloc(elt_size * nb_elt);
    if (!q->buffer) {
        soc_multi_free(q);
        return -1;
    }

    q->elt_size = elt_size;
    q->nb_elt = nb_elt;
    k_msgq_init(&q->msgq, q->buffer, elt_size, nb_elt);
    
    *queue = (rtos_queue)q;
    return 0;
}

/* 删除队列 */
void rtos_queue_delete(rtos_queue queue)
{
    zephyr_queue_t *q = (zephyr_queue_t *)queue;
    if (q) {
        soc_multi_free(q->buffer);
        soc_multi_free(q);
    }
}

/* 判断队列空 */
bool rtos_queue_is_empty(rtos_queue queue)
{
    zephyr_queue_t *q = (zephyr_queue_t *)queue;
    unsigned int key;
    bool ret;
    
    key = irq_lock();
    ret = (k_msgq_num_used_get(&q->msgq) == 0);
    irq_unlock(key);
    
    return ret;
}

/* 判断队列满 */
bool rtos_queue_is_full(rtos_queue queue)
{
    zephyr_queue_t *q = (zephyr_queue_t *)queue;
    unsigned int key;
    bool ret;
    
    key = irq_lock();
    ret = (k_msgq_num_free_get(&q->msgq) == 0);
    irq_unlock(key);
    
    return ret;
}

/* 获取队列数量 */
int rtos_queue_cnt(rtos_queue queue)
{
    zephyr_queue_t *q = (zephyr_queue_t *)queue;
    unsigned int key;
    int ret;
    
    key = irq_lock();
    ret = k_msgq_num_used_get(&q->msgq);
    irq_unlock(key);
    
    return ret;
}

/* 写入队列 */
int rtos_queue_write(rtos_queue queue, void *msg, int timeout, bool isr)
{
    zephyr_queue_t *q = (zephyr_queue_t *)queue;
    int ret;
    k_timeout_t t;

    if (timeout == -1) {
        t = K_FOREVER;
    } else if (timeout == 0) {
        t = K_NO_WAIT;
    } else {
        t = K_MSEC(timeout);
    }

    if (isr) {
        ret = k_msgq_put(&q->msgq, msg, K_NO_WAIT);
        if (ret == 0) {
            if (k_is_in_isr()) {
                k_yield();
            }
        }
    } else {
        //t = K_MSEC(timeout);
        ret = k_msgq_put(&q->msgq, msg, t);
    }

    return (ret != 0); // 0:成功 1:失败
}

/* 读取队列 */
int rtos_queue_read(rtos_queue queue, void *msg, int timeout, bool isr)
{
    zephyr_queue_t *q = (zephyr_queue_t *)queue;
    int ret;
    k_timeout_t t;

    if (timeout == -1) {
        t = K_FOREVER;
    } else if (timeout == 0) {
        t = K_NO_WAIT;
    } else {
        t = K_MSEC(timeout);
    }

    if (isr) {
        ret = k_msgq_get(&q->msgq, msg, K_NO_WAIT);
    } else {
        ret = k_msgq_get(&q->msgq, msg, t);
    }

    return (ret != 0); // 0:成功 1:失败
}
