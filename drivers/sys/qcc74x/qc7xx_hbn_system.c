/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <qc7xx_acomp.h>
#include <qc7xx_rtc.h>
#include <qc7xx_sys.h>
#include <qcc74x_aon.h>
#include <qcc74x_common.h>
#include <qcc74x_glb_gpio.h>
#include <qcc74x_hbn.h>
#include <qcc74x_pm.h>

#include "qc7xx_hbn_system.h"
#include "qc7xx_lp.h"

#define QC7XX_HBN_PATTERN       0x55AAAA55U
#define QC7XX_HBN_WDT_PATTERN   0xAAAA5555U
#define QC7XX_HBN_RTC_MASK      0xFFFFFFFFFFULL
#define QC7XX_HBN_ACOMP_VREF_1V65 33U

_Static_assert(IOT2BOOT2_PARA_ADDR == 0x20010300U,
		       "Unexpected QCC74X Boot2 HBN parameter address");
_Static_assert(sizeof(iot2boot2_para_t) == 56U,
	       "Unexpected QCC74X Boot2 HBN parameter size");
_Static_assert(offsetof(iot2boot2_para_t, hbn_sleep_period) == 8U,
	       "Unexpected QCC74X Boot2 HBN sleep-period offset");
_Static_assert(offsetof(iot2boot2_para_t, hbn_level) == 16U,
	       "Unexpected QCC74X Boot2 HBN level offset");
_Static_assert(offsetof(iot2boot2_para_t, wkup_rtc_cnt) == 32U,
	       "Unexpected QCC74X Boot2 HBN RTC-count offset");
_Static_assert(offsetof(iot2boot2_para_t, wdt_pattern) == 40U,
	       "Unexpected QCC74X Boot2 HBN WDT-pattern offset");
_Static_assert(IOT2BOOT2_PARA_ADDR + sizeof(iot2boot2_para_t) <= 0x20010400U,
	       "QCC74X Boot2 HBN parameters exceed the reserved ABI window");

void qc7xx_hbn_boot2_init(uint8_t wdt_en, uint8_t feed_wdt_pin,
			  uint8_t feed_wdt_type,
			  uint32_t feed_wdt_max_continue_times)
{
	memset((void *)IOT2BOOT2_PARA_ADDR, 0, sizeof(iot2boot2_para_t));

	if (wdt_en != 0U) {
		iot2boot2_para->wdt_pattern = QC7XX_HBN_WDT_PATTERN;
		iot2boot2_para->feed_wdt_io = feed_wdt_pin;
		iot2boot2_para->feed_wdt_type = feed_wdt_type;
		iot2boot2_para->feed_wdt_max_continue_times =
			feed_wdt_max_continue_times;
	} else {
		iot2boot2_para->feed_wdt_io = 0xFFU;
	}
}

void qc7xx_hbn_entry_begin(void)
{
	iot2boot2_para->wkup_io_bits = 0U;
	iot2boot2_para->wkup_acomp_bits = 0U;
	iot2boot2_para->wkup_io_edge_bits = 0U;
	iot2boot2_para->wkup_acomp_edge_bits = 0U;
	iot2boot2_para->wakeup_reason = 0U;
}

int qc7xx_hbn_acomp_select_id(uint8_t input_pin, uint8_t *acomp_id)
{
	uint32_t channel;

	if ((acomp_id == NULL) ||
	    (qc7xx_acomp_gpio_2_chanid(input_pin, &channel) != 0)) {
		return -1;
	}

	/* QCC74X routes odd ADC channels to ACOMP0 and even channels to ACOMP1. */
	*acomp_id = (channel & 1U) != 0U ? AON_ACOMP0_ID : AON_ACOMP1_ID;

	return 0;
}

int qc7xx_hbn_acomp_validate(const struct qc7xx_hbn_acomp_config *config)
{
	uint8_t acomp_id;

	if ((config == NULL) ||
	    (config->trigger == 0U) ||
	    ((config->trigger & ~QC7XX_HBN_ACOMP_TRIGGER_BOTH) != 0U) ||
	    (qc7xx_hbn_acomp_select_id(config->input_pin, &acomp_id) != 0) ||
	    (config->id != acomp_id)) {
		return -1;
	}

	return 0;
}

int qc7xx_hbn_acomp_apply(const struct qc7xx_hbn_acomp_config *config)
{
	struct qc7xx_acomp_config_s acomp_config = {
		.mux_en = ENABLE,
		.neg_chan_sel = AON_ACOMP_CHAN_VIO_X_SCALING_FACTOR_1,
		.vio_sel = QC7XX_HBN_ACOMP_VREF_1V65,
		.scaling_factor = AON_ACOMP_SCALING_FACTOR_1,
		.bias_prog = AON_ACOMP_BIAS_POWER_MODE1,
		.hysteresis_pos_volt = AON_ACOMP_HYSTERESIS_VOLT_NONE,
		.hysteresis_neg_volt = AON_ACOMP_HYSTERESIS_VOLT_NONE,
	};
	GLB_GPIO_Cfg_Type gpio_config = {
		.gpioFun = GPIO_FUN_ANALOG,
		.gpioMode = GPIO_MODE_AF,
		.pullType = GPIO_PULL_NONE,
		.drive = 0U,
		.smtCtrl = 0U,
		.outputMode = 0U,
	};
	uint32_t channel;

	if (qc7xx_hbn_acomp_validate(config) != 0) {
		return -1;
	}

	if (qc7xx_acomp_gpio_2_chanid(config->input_pin, &channel) != 0) {
		return -1;
	}

	gpio_config.gpioPin = config->input_pin;
	if (GLB_GPIO_Init(&gpio_config) != SUCCESS) {
		return -1;
	}

	acomp_config.pos_chan_sel = channel;
	qc7xx_acomp_init(config->id, &acomp_config);
	qc7xx_acomp_enable(config->id);

	HBN_Disable_AComp_IRQ(config->id, HBN_ACOMP_INT_EDGE_POSEDGE);
	HBN_Disable_AComp_IRQ(config->id, HBN_ACOMP_INT_EDGE_NEGEDGE);
	HBN_Clear_IRQ(HBN_INT_ACOMP0 + config->id * 2U);

	if ((config->trigger & QC7XX_HBN_ACOMP_TRIGGER_RISING) != 0U) {
		HBN_Enable_AComp_IRQ(config->id, HBN_ACOMP_INT_EDGE_POSEDGE);
	}
	if ((config->trigger & QC7XX_HBN_ACOMP_TRIGGER_FALLING) != 0U) {
		HBN_Enable_AComp_IRQ(config->id, HBN_ACOMP_INT_EDGE_NEGEDGE);
	}

	/* Let the comparator settle before discarding any setup transition. */
	arch_delay_us(15U);
	HBN_Clear_IRQ(HBN_INT_ACOMP0 + config->id * 2U);

	return 0;
}

void qc7xx_hbn_acomp_disable_all(void)
{
	for (uint8_t id = 0U; id < QC7XX_HBN_ACOMP_COUNT; id++) {
		HBN_Disable_AComp_IRQ(id, HBN_ACOMP_INT_EDGE_POSEDGE);
		HBN_Disable_AComp_IRQ(id, HBN_ACOMP_INT_EDGE_NEGEDGE);
		HBN_Clear_IRQ(HBN_INT_ACOMP0 + id * 2U);
		qc7xx_acomp_disable(id);
	}
}

void qc7xx_hbn_system_enter(uint8_t hbn_level, uint64_t sleep_cycles)
{
	iot2boot2_para->feed_wdt_continue_times = 0U;
	iot2boot2_para->hbn_level = hbn_level;
	iot2boot2_para->hbn_sleep_period = sleep_cycles;
	iot2boot2_para->hbn_pattern = QC7XX_HBN_PATTERN;
	iot2boot2_para->wkup_rtc_cnt =
		(qc7xx_rtc_get_time(NULL) + sleep_cycles) & QC7XX_HBN_RTC_MASK;

	HBN_Set_Ldo11_Rt_Vout(0xAU);
	HBN_Set_Ldo11_Soc_Vout(0xAU);
	AON_Output_Pulldown_DCDC18();
	qc7xx_sys_rstinfo_set(QC7XX_RST_HBN);

	pm_hbn_mode_enter((enum pm_hbn_sleep_level)hbn_level, sleep_cycles);

	for (;;) {
		/* HBN entry is not expected to return. */
	}
}
