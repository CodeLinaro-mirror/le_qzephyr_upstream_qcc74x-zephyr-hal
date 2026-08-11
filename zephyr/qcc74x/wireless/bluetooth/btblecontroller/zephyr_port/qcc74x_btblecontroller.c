#include <zephyr/kernel.h>

#include "common/assert.h"
#include "common/bt_str.h"

#include "btble_inc/btble_lib_api.h"
#include "btble_inc/hci_onchip.h"
//#include "rfparam_adapter.h"

#include <stdlib.h>

#define LOG_LEVEL CONFIG_BT_HCI_DRIVER_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ctlr_port);

typedef struct k_msgq* btblecontroller_QueueHandle_t;
//typedef struct k_queue* btblecontroller_QueueHandle_t;
typedef void (*btblecontroller_TaskFunction_t)(void *);
static struct k_thread controller_tread;
static btblecontroller_TaskFunction_t g_func;
static bool dm_irq_connected = false;
static bool ble_irq_connected = false;
static bool bt_irq_connected = false;
static void controller_thread_func(void *dummy1, void *dummy2, void *dummy3)
{
    g_func(dummy1);
}
int btblecontroller_task_new(btblecontroller_TaskFunction_t taskFunction,const char *name, int stack_size, void *arg, int prio,void **taskHandler)
{
    LOG_DBG("");
    g_func = taskFunction;
    static K_KERNEL_STACK_DEFINE(ctrl_thread_stack, 2048);
    k_tid_t kid = k_thread_create(&controller_tread, ctrl_thread_stack,
    K_KERNEL_STACK_SIZEOF(ctrl_thread_stack),
    controller_thread_func, (void *)arg, NULL, NULL,
    K_PRIO_COOP(CONFIG_BT_DRIVER_RX_HIGH_PRIO), 0, K_NO_WAIT);
    k_thread_name_set(kid, name);
    *taskHandler = kid;
    return 0;
}

void btblecontroller_task_delete(uint32_t taskHandler)
{
    LOG_DBG("");
    k_thread_abort((k_tid_t)taskHandler);
}

int btblecontroller_queue_new(uint32_t size, uint32_t max_msg,btblecontroller_QueueHandle_t *queue)
{
    *queue = k_malloc(sizeof(struct k_msgq));
    if(*queue == NULL){
        return 1;
    }
    int err = k_msgq_alloc_init(*queue, size, max_msg);
    LOG_DBG("err:%d",err);

    return err ? 1 : 0;
}

void btblecontroller_queue_free(btblecontroller_QueueHandle_t q)
{
    LOG_DBG("");
    k_msgq_cleanup(q);
    k_free(q);
}

int btblecontroller_queue_send(btblecontroller_QueueHandle_t q, void *msg, uint32_t size, uint32_t timeout)
{
    k_timeout_t a = {
        .ticks = timeout,
    };
    int err = k_msgq_put(q, msg, a);
    LOG_DBG("err:%d",err);

    return err ? 0 : 1;
}

int btblecontroller_queue_recv(btblecontroller_QueueHandle_t q, void *msg, uint32_t timeout)
{
    k_timeout_t a = {
        .ticks = timeout,
    };
    
    int err = k_msgq_get(q, msg, a);
    LOG_DBG("err:%d",err);
    return err ? 0 : 1;
}

int btblecontroller_queue_send_from_isr(btblecontroller_QueueHandle_t q, void *msg, uint32_t size)
{
    k_timeout_t a = {
        .ticks = 0,
    };
    int err = k_msgq_put(q, msg, a);
    LOG_DBG("err:%d", err);

    return err ? 0 : 1;
}

int btblecontroller_xport_is_inside_interrupt(void)
{
    LOG_DBG("");
    return k_is_in_isr();
}

void btblecontroller_task_delay(uint32_t ms)
{
    LOG_DBG("");
    k_sleep(K_MSEC(ms));
}

void * btblecontroller_task_get_current_task_handle(void)
{
    LOG_DBG("");
    return k_current_get();
}

void *btblecontroller_malloc(size_t xWantedSize)
{
    return k_malloc(xWantedSize);
}

void btblecontroller_free(void *buf)
{
    k_free(buf);
}

#include "qcc74x.h"
#include "qcc74x_glb.h"
#include "qc7xx_efuse.h"
void btblecontroller_ble_irq_init(void *handler)
{
    qc7xx_irq_clear_pending(BLE_IRQn);
    if (!ble_irq_connected) {
        irq_connect_dynamic(BLE_IRQn, 0, handler, NULL, 0);
        ble_irq_connected = true;
    }
    irq_enable(BLE_IRQn);
}

void btblecontroller_bt_irq_init(void *handler)
{
    qc7xx_irq_clear_pending(BT_IRQn);
    if (!bt_irq_connected) {
        irq_connect_dynamic(BT_IRQn, 0, handler, NULL, 0);
        bt_irq_connected = true;
    }
    irq_enable(BT_IRQn);
}

void btblecontroller_dm_irq_init(void *handler)
{
    qc7xx_irq_clear_pending(DM_IRQn);
    if (!dm_irq_connected) {
        irq_connect_dynamic(DM_IRQn, 0, handler, NULL, 0);
        dm_irq_connected = true;
    }
    irq_enable(DM_IRQn);
}

void btblecontroller_ble_irq_enable(uint8_t enable)
{

    if(enable){
        irq_enable(BLE_IRQn);
    }
    else {
        irq_disable(BLE_IRQn);
    }
}

void btblecontroller_bt_irq_enable(uint8_t enable)
{
    if(enable){
        irq_enable(BT_IRQn);
    }
    else{
        irq_disable(BT_IRQn);
    }
}

void btblecontroller_dm_irq_enable(uint8_t enable)
{
    if(enable){
        irq_enable(DM_IRQn);
    }
    else{
        irq_disable(DM_IRQn);
    }
}

__attribute__((weak)) uint8_t btblecontrolller_get_chip_version()
{
    extern void qc7xx_efuse_get_device_info(qc7xx_efuse_device_info_type *device_info);
    qc7xx_efuse_device_info_type device_info;
    qc7xx_efuse_get_device_info(&device_info);
    return device_info.version;
}
