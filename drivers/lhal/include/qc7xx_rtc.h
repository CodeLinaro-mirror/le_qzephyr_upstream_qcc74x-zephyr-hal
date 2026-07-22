/*
 * Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _QC7XX_RTC_H
#define _QC7XX_RTC_H

#include "qc7xx_core.h"

/** @addtogroup LHAL
  * @{
  */

/** @addtogroup RTC
  * @{
  */

#define QC7XX_RTC_SEC2TIME(s)    (s * qc7xx_clk_get_peripheral_clock(QC7XX_DEVICE_TYPE_RTC, 0))
#define QC7XX_RTC_TIME2SEC(time) (time / qc7xx_clk_get_peripheral_clock(QC7XX_DEVICE_TYPE_RTC, 0))

/* This struct is the same with struct tm */
struct qc7xx_tm {
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_mday;
    int tm_mon;
    int tm_year;
    int tm_wday;
    int tm_yday;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief clear and disable rtc.
 *
 * @param [in] dev device handle
 */
void qc7xx_rtc_disable(struct qc7xx_device_s *dev);

/**
 * @brief Set rtc alarming time.
 *
 * @param [in] dev device handle
 * @param [in] time alarming time, unit is (1/32768 s)
 */
void qc7xx_rtc_set_time(struct qc7xx_device_s *dev, uint64_t time);

/**
 * @brief Get rtc current time.
 *
 * @param [in] dev device handle
 * @return current rtc running time
 */
uint64_t qc7xx_rtc_get_time(struct qc7xx_device_s *dev);

/**
 * @brief Set current utc time.
 *
 * @param [in] time tm handle
 */
void qc7xx_rtc_set_utc_time(const struct qc7xx_tm *time);

/**
 * @brief Get current utc time.
 *
 * @param [out] time tm handle
 */
void qc7xx_rtc_get_utc_time(struct qc7xx_tm *time);

/**
 * @brief Get current utc timestamp(s).
 *
 */
uint64_t qc7xx_rtc_get_utc_timestamp(void);

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