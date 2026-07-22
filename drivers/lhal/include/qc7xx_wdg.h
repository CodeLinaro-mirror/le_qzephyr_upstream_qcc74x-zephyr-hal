/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_WDG_H
#define _QC7XX_WDG_H

#include "qc7xx_core.h"

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup WDG
  * @{
  */

#define WDG_CLKSRC_BCLK 0
#define WDG_CLKSRC_32K  1
#define WDG_CLKSRC_1K   2
#define WDG_CLKSRC_XTAL 3
#define WDG_CLKSRC_GPIO 4
#define WDG_CLKSRC_NO   5

/** @defgroup WDG_MODE Watch-dog reset/interrupt mode definition
  * @{
  */
#define WDG_MODE_INTERRUPT 0
#define WDG_MODE_RESET     1
/**
  * @}
  */

// clang-format off
#define IS_WDG_CLKSRC(type) ((type) <= 5)

#define IS_WDG_MODE(type)   (((type) == WDG_MODE_INTERRUPT) || \
                            ((type) == WDG_MODE_RESET))

#define IS_WDG_CLOCK_DIV(type) ((type) <= 255)

// clang-format on

/**
 * @brief WDG configuration structure
 *
 * @param clock_source      Wdg clock source, use QC7XX_SYSTEM_* definition
 * @param clock_div         Wdg clock divison value, from 0 to 255
 * @param comp_val          Wdg compare value
 * @param mode              Wdg reset/interrupt mode
 */
struct qc7xx_wdg_config_s {
    uint8_t clock_source;
    uint8_t clock_div;
    uint16_t comp_val;
    uint8_t mode;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize watchdog.
 *
 * @param [in] dev device handle
 * @param [in] config pointer to save watchdog config
 */
void qc7xx_wdg_init(struct qc7xx_device_s *dev, const struct qc7xx_wdg_config_s *config);

/**
 * @brief Start watchdog.
 *
 * @param [in] dev device handle
 */
void qc7xx_wdg_start(struct qc7xx_device_s *dev);

/**
 * @brief Stop watchdog.
 *
 * @param [in] dev device handle
 */
void qc7xx_wdg_stop(struct qc7xx_device_s *dev);

/**
 * @brief Get watchdog counter value.
 *
 * @param [in] dev device handle
 * @return counter value
 */
uint16_t qc7xx_wdg_get_countervalue(struct qc7xx_device_s *dev);

/**
 * @brief Set watchdog counter value.
 *
 * @param [in] dev device handle
 * @param [in] counter value
 */
void qc7xx_wdg_set_countervalue(struct qc7xx_device_s *dev, uint16_t value);

/**
 * @brief Reset watchdog counter value.
 *
 * @param [in] dev device handle
 */
void qc7xx_wdg_reset_countervalue(struct qc7xx_device_s *dev);

/**
 * @brief Clear watchdog compare interrupt status.
 *
 * @param [in] dev device handle
 */
void qc7xx_wdg_compint_clear(struct qc7xx_device_s *dev);

#ifdef __cplusplus
}
#endif

/**
  * @}
  */

/**
  * @}
  */

#endif
