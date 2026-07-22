#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/sys/util.h>

#include "platform_al.h"

#if defined(CONFIG_QCC74X_PM_PDS15) || defined(CFG_WIFI_PDS_RESUME)
#include <qcc74x_lp.h>

#include "export/common/co_utils.h"
#include "export/mac/mac_frame.h"

extern int lpfw_recal_rc32k(uint64_t beacon_timestamp_now_us,
			    uint64_t rtc_timestamp_now_us, uint32_t mode);
extern int32_t lpfw_calculate_beacon_delay(uint64_t beacon_timestamp_us,
					   uint64_t rtc_timestamp_us, uint32_t mode);
struct qc7xx_device_s;
extern uint64_t qc7xx_rtc_get_time(struct qc7xx_device_s *dev);

#define NXMAC_TSF_TIMER_LO_ADDR 0x24B080A4
#define REG_PL_RD(addr) (*(volatile uint32_t *)(HW2CPU(addr)))
#define MAC_TSF_TIMER_LOW REG_PL_RD(NXMAC_TSF_TIMER_LO_ADDR)
#endif

#include <qc7xx_efuse.h>
#include <qc7xx_sec_trng.h>

#ifdef CONFIG_SOC_SERIES_QCC74X
#include <qcc74x_mfg_media.h>
#endif

#include "soc_mem.h"

#include  <rtos_al.h>

#define ASSERT(...)

/**
 ****************************************************************************************
 * @brief Get MAC Address
 ****************************************************************************************
 */
int platform_get_mac(uint8_t *mac)
{
#ifdef CONFIG_SOC_SERIES_QCC74X
    if (0 == mfg_media_read_macaddr_with_lock(mac, 1)) {
        return 0;
    }
#else
    return -1;
#endif
    return -1;
}

/**
 ****************************************************************************************
 * @brief 获取真随机数（使用硬件TRNG）
 ****************************************************************************************
 */
int platform_get_random(unsigned char *buf, size_t len)
{
    int ret;
    unsigned int key = irq_lock();  // 替换临界区保护

    ret = qc7xx_trng_readlen(buf, len);

    irq_unlock(key);
    return ret;
}

/**
 ****************************************************************************************
 * @brief 内存管理适配层
 ****************************************************************************************
 */
void *rtos_malloc(uint32_t size)
{
    return (void *)soc_multi_malloc(size);
}

void *rtos_calloc(uint32_t nb_elt, uint32_t size)
{
    void *res = (void *)soc_multi_malloc(nb_elt * size);
    if (res) {
        memset(res, 0, nb_elt * size);
    }
    return res;
}

void rtos_free(void *ptr)
{
    soc_multi_free(ptr);
}

void *platform_malloc(uint32_t size)
{
    return (void *)soc_multi_malloc(size);
}

void platform_free(void *mem_ptr)
{
    soc_multi_free(mem_ptr);
}

/**
 ****************************************************************************************
 * @brief 异步事件处理（Zephyr工作队列实现）
 ****************************************************************************************
 */

typedef struct async_work {
    struct k_work work;
    uint32_t code1;
    uint32_t code2;
} async_work_t;

/* 预分配线程栈 */
K_THREAD_STACK_DEFINE(wifi_work_q_stack, 2048);

/* 定义工作队列和工作项 */
static struct k_work_q wifi_work_queue;

extern void qc7xx_wifi_event_handler(uint32_t code, uint32_t code1);
static void async_work_handler(struct k_work *work)
{
    struct async_work *aw = CONTAINER_OF(work, struct async_work, work);

    /* 执行事件处理 */
    qc7xx_wifi_event_handler(aw->code1, aw->code2);

    soc_multi_free(aw);
}

/**
 ****************************************************************************************
 * @brief 事件投递接口（线程安全）
 ****************************************************************************************
 */
void platform_post_event(int catalogue, int code1, int code2)
{
    async_work_t *aw;

    aw = (async_work_t *)soc_multi_malloc(sizeof(async_work_t));
    if (aw) {
        aw->code1 = code1;
        aw->code2 = code2;
        k_work_init(&aw->work, async_work_handler);
        k_work_submit_to_queue(&wifi_work_queue, &aw->work);
    }
}

static int platform_workqueue_init(void)
{
    struct k_work_queue_config cfg = {
        .name = "wifiworkq",
    };

    /* 1. 初始化工作队列 */
    k_work_queue_init(&wifi_work_queue);

    /* 2. 启动工作队列线程 */
    k_work_queue_start(&wifi_work_queue,
                      wifi_work_q_stack,
                      K_THREAD_STACK_SIZEOF(wifi_work_q_stack),
                      -1,  // 优先级
                      &cfg);

    return 0;
}
SYS_INIT(platform_workqueue_init, POST_KERNEL, 0);

#if defined(CONFIG_QCC74X_PM_PDS15) || defined(CFG_WIFI_PDS_RESUME)
static uint8_t platform_get_leg_rate(uint8_t rate)
{
	switch (rate) {
	case 0:
		return 2;
	case 1:
		return 4;
	case 2:
		return 11;
	case 3:
		return 22;
	case 11:
		return 12;
	case 15:
		return 18;
	case 10:
		return 24;
	case 14:
		return 36;
	case 9:
		return 48;
	case 13:
		return 72;
	case 8:
		return 96;
	case 12:
		return 108;
	default:
		return 2;
	}
}

void platform_hook_beacon(uint32_t rhd, uint32_t tim, bcn_param_t *param)
{
	uint64_t rtc_cnt;
	uint64_t rtc_stamp_us;
	uint64_t beacon_stamp_us;
	uint32_t mac_time_now;
	uint32_t rx_delay_us;
	uint32_t proce_delay_us;
	uint8_t *tim_ie = NULL;
	uint8_t bcn_rate;
	unsigned int key;
	struct bcn_frame *bcn = (void *)rhd;

	if ((bcn == NULL) || (param == NULL)) {
		return;
	}

	key = irq_lock();
	rtc_cnt = qc7xx_rtc_get_time(NULL);
	mac_time_now = MAC_TSF_TIMER_LOW;
	irq_unlock(key);

	bcn_rate = platform_get_leg_rate(param->beacon_rate);
	BEACON_DATA_RATE = bcn_rate;

	rtc_stamp_us = QC7XX_PDS_CNT_TO_US(rtc_cnt);
	beacon_stamp_us = bcn->tsf;
	rx_delay_us = (param->beacon_len - 32U) * 8U * 2U / bcn_rate;
	proce_delay_us = mac_time_now - param->mac_local_tsf_l;
	rtc_stamp_us = rtc_stamp_us - proce_delay_us - rx_delay_us;

	lpfw_recal_rc32k(beacon_stamp_us, rtc_stamp_us, BEACON_STAMP_APP);
	(void)lpfw_calculate_beacon_delay(beacon_stamp_us, rtc_stamp_us, BEACON_STAMP_APP);

	(void)qc7xx_lp_beacon_interval_update(bcn->bcnint);
	if (tim != 0U) {
		tim_ie = (uint8_t *)(uintptr_t)HW2CPU(tim);
		(void)qc7xx_lp_beacon_tim_update(tim_ie, BEACON_STAMP_APP);
	}
}
#endif
