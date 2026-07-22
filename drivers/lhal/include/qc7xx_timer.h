/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_TIMER_H
#define _QC7XX_TIMER_H

#include "qc7xx_core.h"

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup TIMER
  * @{
  */

/** @defgroup TIMER_CLK_SOURCE timer clock source definition
  * @{
  */
#define TIMER_CLKSRC_BCLK 0
#define TIMER_CLKSRC_32K  1
#define TIMER_CLKSRC_1K   2
#define TIMER_CLKSRC_XTAL 3
#define TIMER_CLKSRC_GPIO 4
#define TIMER_CLKSRC_NO   5
/**
  * @}
  */

/** @defgroup TIMER_COUNTER_MODE timer counter mode definition
  * @{
  */
#define TIMER_COUNTER_MODE_PROLOAD 0
#define TIMER_COUNTER_MODE_UP      1
/**
  * @}
  */

/** @defgroup TIMER_COMP_ID timer compare id definition
  * @{
  */
#define TIMER_COMP_ID_0 0
#define TIMER_COMP_ID_1 1
#define TIMER_COMP_ID_2 2
#define TIMER_COMP_NONE 3
/**
  * @}
  */

/** @defgroup TIMER_GPIO_PULSE_POLARITY timer gpio pulse polarity definition
  * @{
  */
#define TIMER_GPIO_PULSE_POLARITY_POSITIVE 0
#define TIMER_GPIO_PULSE_POLARITY_NEGATIVE 1
/**
  * @}
  */

// clang-format off

#define IS_TIMER_COUNTER_MODE(type)   (((type) == TIMER_COUNTER_MODE_PROLOAD) || \
                                      ((type) == TIMER_COUNTER_MODE_UP))

#define IS_TIMER_CLK_SOURCE(type)   ((type) <= 3)

#define IS_TIMER_COMP_ID(type)   (((type) == TIMER_COMP_ID_0) || \
                                  ((type) == TIMER_COMP_ID_1) || \
                                  ((type) == TIMER_COMP_ID_2) || \
                                  ((type) == TIMER_COMP_NONE))

#define IS_TIMER_COMP_VAL(value) ((value) >= 2)

#define IS_TIMER_CLOCK_DIV(type) ((type) <= 255)

// clang-format on

/**
 * @brief TIMER configuration structure
 *
 * @param counter_mode      Timer counter mode, use @ref TIMER_COUNTER_MODE
 * @param clock_source      Timer clock source, use @ref TIMER_CLK_SOURCE
 * @param clock_div         Timer clock divison value, from 0 to 255
 * @param trigger_comp_id   Timer count register preload trigger source slelect, use @ref TIMER_COMP_ID
 * @param comp0_val         Timer compare 0 value
 * @param comp1_val         Timer compare 1 value
 * @param comp2_val         Timer compare 2 value
 * @param preload_val       Timer preload value
 */
struct qc7xx_timer_config_s {
    uint8_t counter_mode;
    uint8_t clock_source;
    uint8_t clock_div;
    uint8_t trigger_comp_id;
    uint32_t comp0_val;
    uint32_t comp1_val;
    uint32_t comp2_val;
    uint32_t preload_val;
};

#if defined(QCC74X) || defined(QCC75X)
/**
 * @brief TIMER capture configuration structure
 *
 * @param pin      Timer capture pin
 * @param polarity Timer capture polarity, use @ref TIMER_GPIO_PULSE_POLARITY
 */
struct qc7xx_timer_capture_config_s {
    uint8_t pin;
    uint8_t polarity;
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize timer.
 *
 * @param [in] dev device handle
 * @param [in] config pointer to save timer config
 */
void qc7xx_timer_init(struct qc7xx_device_s *dev, const struct qc7xx_timer_config_s *config);

/**
 * @brief Deinitialize timer.
 *
 * @param [in] dev device handle
 */
void qc7xx_timer_deinit(struct qc7xx_device_s *dev);

/**
 * @brief Start timer.
 *
 * @param [in] dev device handle
 */
void qc7xx_timer_start(struct qc7xx_device_s *dev);

/**
 * @brief Stop timer.
 *
 * @param [in] dev device handle
 */
void qc7xx_timer_stop(struct qc7xx_device_s *dev);

/**
 * @brief Set timer preload value.
 *
 * @param [in] dev device handle
 * @param [in] val preload value
 */
void qc7xx_timer_set_preloadvalue(struct qc7xx_device_s *dev, uint32_t val);

/**
 * @brief Set compare value of corresponding compare id.
 *
 * @param [in] dev device handle
 * @param [in] cmp_no compare id, use @ref TIMER_COMP_ID
 * @param [in] val compare value
 */
void qc7xx_timer_set_compvalue(struct qc7xx_device_s *dev, uint8_t cmp_no, uint32_t val);

/**
 * @brief Get compare value of corresponding compare id.
 *
 * @param [in] dev device handle
 * @param [in] cmp_no compare id, use @ref TIMER_COMP_ID
 * @return uint32_t
 */
uint32_t qc7xx_timer_get_compvalue(struct qc7xx_device_s *dev, uint8_t cmp_no);

/**
 * @brief Get timer counter value.
 *
 * @param [in] dev device handle
 * @return counter value
 */
uint32_t qc7xx_timer_get_countervalue(struct qc7xx_device_s *dev);

/**
 * @brief Enable or disable timer interrupt of corresponding compare id.
 *
 * @param [in] dev device handle
 * @param [in] cmp_no compare id, use @ref TIMER_COMP_ID
 * @param [in] mask true means disable, false means enable
 */
void qc7xx_timer_compint_mask(struct qc7xx_device_s *dev, uint8_t cmp_no, bool mask);

/**
 * @brief Get timer interrupt status of corresponding compare id.
 *
 * @param [in] dev device handle
 * @param [in] cmp_no compare id, use @ref TIMER_COMP_ID
 * @return true mean yes, otherwise no.
 */
bool qc7xx_timer_get_compint_status(struct qc7xx_device_s *dev, uint8_t cmp_no);

/**
 * @brief Clear timer interrupt status of corresponding compare id.
 *
 * @param [in] dev device handle
 * @param [in] cmp_no compare id, use @ref TIMER_COMP_ID
 */
void qc7xx_timer_compint_clear(struct qc7xx_device_s *dev, uint8_t cmp_no);

/**
 * @brief Control timer feature.
 *
 * @param [in] dev device handle
 * @param [in] cmd feature command, use @ref TIMER_CMD
 * @param [in] arg user data
 * @return A negated errno value on failure.
 */
int qc7xx_timer_feature_control(struct qc7xx_device_s *dev, int cmd, size_t arg);

#if defined(QCC74X) || defined(QCC75X)
/**
 * @brief Initialize the timer capture feature.
 *
 * @param [in] dev device handle
 * @param [in] config pointer to the timer capture configuration structure
 */
void qc7xx_timer_capture_init(struct qc7xx_device_s *dev, const struct qc7xx_timer_capture_config_s *config);
#endif

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