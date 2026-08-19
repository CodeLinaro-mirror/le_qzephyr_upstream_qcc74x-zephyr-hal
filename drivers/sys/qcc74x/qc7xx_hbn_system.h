/*
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __QC7XX_HBN_SYSTEM_H__
#define __QC7XX_HBN_SYSTEM_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define QC7XX_HBN_ACOMP_COUNT                2U
#define QC7XX_HBN_ACOMP_TRIGGER_FALLING      1U
#define QC7XX_HBN_ACOMP_TRIGGER_RISING       2U
#define QC7XX_HBN_ACOMP_TRIGGER_BOTH         3U

struct qc7xx_hbn_acomp_config {
	uint8_t id;
	uint8_t input_pin;
	uint8_t trigger;
};

void qc7xx_hbn_boot2_init(uint8_t wdt_en, uint8_t feed_wdt_pin,
			  uint8_t feed_wdt_type,
			  uint32_t feed_wdt_max_continue_times);
void qc7xx_hbn_entry_begin(void);
int qc7xx_hbn_acomp_select_id(uint8_t input_pin, uint8_t *acomp_id);
int qc7xx_hbn_acomp_validate(const struct qc7xx_hbn_acomp_config *config);
int qc7xx_hbn_acomp_apply(const struct qc7xx_hbn_acomp_config *config);
void qc7xx_hbn_acomp_disable_all(void);
void qc7xx_hbn_system_enter(uint8_t hbn_level,
			    uint64_t sleep_cycles) __attribute__((noreturn));

#ifdef __cplusplus
}
#endif

#endif /* __QC7XX_HBN_SYSTEM_H__ */
