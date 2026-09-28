/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 *****************************************************************************************
 * @addtogroup MACHW
 * @{
 *****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include "qcc74x_lp.h"
#include "arch.h"
#include "hal_machw.h"
#include "crm.h"
#include "phy.h"

#include "reg_mac_core.h"
#include "reg_mac_pl.h"

#include "mac_frame.h"
#include "mac_types.h"

/// Mask used for the MAC address compatibility checking
#define MM_MAC_ADDR_MSK  ((NX_VIRT_DEV_MAX - 1) << 8)

/// Mask used for the AP MAC address compatibility checking
#define MM_AP_MAC_ADDR_MSK 0x00000002

/// Error interrupts
#define HW_ERROR_IRQ (NXMAC_RX_FIFO_OVER_FLOW_BIT | NXMAC_MAC_PHYIF_OVERFLOW_BIT |      \
                      NXMAC_PT_ERROR_BIT | NXMAC_HW_ERR_BIT | NXMAC_PHY_ERR_BIT |       \
                      NXMAC_AC_0_TX_DMA_DEAD_BIT | NXMAC_AC_1_TX_DMA_DEAD_BIT |         \
                      NXMAC_AC_2_TX_DMA_DEAD_BIT | NXMAC_AC_3_TX_DMA_DEAD_BIT |         \
                      NXMAC_BCN_TX_DMA_DEAD_BIT | NXMAC_MAC_PHYIF_UNDER_RUN_BIT |       \
                      NXMAC_RX_HEADER_DMA_DEAD_BIT | NXMAC_RX_PAYLOAD_DMA_DEAD_BIT)

/// TX interrupt bits
#define TX_IRQ 0

/// Fake definition for compilation
#define NXMAC_ENCR_RX_FIFO_RESET_BIT 0
/// RX interrupt bits
#define RX_IRQ (NXMAC_RX_BUFFER_1_TRIGGER_BIT)

#define BW_DROP_IRQ  0

/**
 ****************************************************************************************
 * @brief Updates the MAC HW timing registers according to the frequency actually used.
 *
 * @param[in] newfreq Frequency of the MAC HW core, in MHz
 ****************************************************************************************
 */
#define TIMING_UPDATE(param, old, new)                                         \
                     (nxmac_ ## param ## _in_mac_clk_setf((uint32_t)           \
                                        nxmac_ ## param ## _in_mac_clk_getf()  \
                                          * (uint32_t)(new) / (uint32_t)(old)))
static void hal_machw_setfreq(uint8_t newfreq)
{
    uint8_t oldfreq;

    oldfreq = nxmac_mac_core_clk_freq_getf();

    // TIMINGS_1
    nxmac_mac_core_clk_freq_setf(newfreq);
    TIMING_UPDATE(tx_rf_delay, oldfreq, newfreq);
    TIMING_UPDATE(tx_chain_delay, oldfreq, newfreq);
    // Temporary - Will have to be removed once MAC HW has the correct default value
    // nxmac_tx_chain_delay_in_mac_clk_setf(0x15F);
    nxmac_tx_chain_delay_in_mac_clk_setf(4*60);
    TIMING_UPDATE(tx_chain_delay, 60, newfreq);

    // TIMINGS_2
    TIMING_UPDATE(slot_time, oldfreq, newfreq);

    // TIMINGS_3
    TIMING_UPDATE(rx_rf_delay, oldfreq, newfreq);
    TIMING_UPDATE(mac_proc_delay, oldfreq, newfreq);

    // TIMINGS_4
    if (newfreq<30)
        // WTClk running at 3x macCoreClk freq
        nxmac_wt_2_crypt_clk_ratio_setf(3);
    else if (newfreq<60)
        // WTClk running at 2x macCoreClk freq
        nxmac_wt_2_crypt_clk_ratio_setf(2);
    else
        // WTClk running at macCoreClk freq
        nxmac_wt_2_crypt_clk_ratio_setf(1);

    // TIMINGS_5
    TIMING_UPDATE(sifs_b, oldfreq, newfreq);

    // TIMINGS_6
    TIMING_UPDATE(sifs_a, oldfreq, newfreq);

    // TIMINGS_7
    // Nothing to do

    // TIMINGS_8
    // Nothing to do

    // TIMINGS_9
    TIMING_UPDATE(rifs_to, oldfreq, newfreq);
    TIMING_UPDATE(tx_dma_proc_dly, oldfreq, newfreq);
}

int lpfw_mac_reset_for_app(void)
{
    nxmac_gen_int_enable_set(0);
    nxmac_tx_rx_int_enable_set(0);
    nxmac_soft_reset_setf(1);
    uint64_t start = shared_cpu_get_mtimer_counter();

    while (nxmac_soft_reset_getf()) {
        if (shared_cpu_get_mtimer_counter() - start >= 1000U) {
            return -1;
        }
    }
    return 0;
}

void hal_machw_init(void)
{
    int8_t min, max;

    // Soft Reset
    nxmac_soft_reset_setf(1);
    while (nxmac_soft_reset_getf());

    // Update the timing registers of the MAC according to the MAC core clock
    hal_machw_setfreq(crm_get_mac_freq());

    // enable MAC HW general interrupt events
    nxmac_gen_int_enable_set(NXMAC_MASTER_GEN_INT_EN_BIT | NXMAC_IDLE_INTERRUPT_BIT |
                             NXMAC_ABS_GEN_TIMERS_BIT | HW_ERROR_IRQ);

    // delegate MPIF interface flow control to PHY
    nxmac_rate_controller_mpif_setf(0);

    //ASSERT_ERR(MM_STA_TO_KEY(NX_REMOTE_STA_MAX - 1) <= nxmac_sta_key_max_index_getf());
    //nxmac_encr_ram_config_pack(NX_VIRT_DEV_MAX, MM_STA_TO_KEY(NX_REMOTE_STA_MAX - 1),
    //                           MM_STA_TO_KEY(0));
    nxmac_encr_ram_config_pack(2, ((4 - 1) + (4 * (2 + (0)))),
                               ((0) + (4 * (2 + (0)))));

    // enable MAC HW TX and RX interrupt events
    nxmac_tx_rx_int_enable_set(TX_IRQ | BW_DROP_IRQ | RX_IRQ | NXMAC_MASTER_TX_RX_INT_EN_BIT);

    // At initialization we are not supposed to reply to any frame (in case monitor mode
    // would be used), so disable the auto-reply capabilities of the HW
    nxmac_mac_cntrl_1_set(nxmac_mac_cntrl_1_get() | NXMAC_DISABLE_ACK_RESP_BIT |
                                                    NXMAC_DISABLE_CTS_RESP_BIT |
                                                    NXMAC_DISABLE_BA_RESP_BIT |
                                                    NXMAC_ACTIVE_CLK_GATING_BIT |
                                                    NXMAC_ENABLE_LP_CLK_SWITCH_BIT |
                                                    NXMAC_RX_RIFS_EN_BIT);
    // RIFS not supported in HE
    nxmac_rx_rifs_en_setf(0);
    if (phy_get_nss())
        nxmac_default_resp_smm_index_setf(2);

    // enable the RX flow control
    nxmac_rx_flow_cntrl_en_setf(1);

    // enable reception of all frames (i.e. monitor mode)
    //nxmac_rx_cntrl_set(MM_RX_FILTER_MONITOR);

    // put default values of the beacon timings
    #define HAL_MACHW_BCN_TX_DELAY_US       (2048)
    nxmac_bcn_cntrl_1_pack(255, 1, HAL_MACHW_BCN_TX_DELAY_US / 128, 100);

    // limit the maximum RX size to the maximum A-MSDU size we support
    #define RWNX_MAX_AMSDU_RX 4096
    nxmac_max_rx_length_set(RWNX_MAX_AMSDU_RX);

    // configure the EDCA control register
    nxmac_edca_cntrl_pack(0, 0, 0, 0);

    // Default power level
    phy_get_rf_gain_capab(&max, &min);
    nxmac_max_power_level_pack(min, max, max);

    // reset MIB Table
    nxmac_mib_table_reset_setf(1);

    // reset Key storage RAM
    nxmac_key_sto_ram_reset_setf(1);

    // Initialize diagnostic ports
    nxmac_debug_port_sel_pack(0x1C, 0x25);

    // Enable the dynamic bandwidth feature
    nxmac_dyn_bw_en_setf(1);

    // Set the number of TX chains the HW shall use for immediate responses
    uint8_t phy_get_ntx(void);
    nxmac_max_phy_ntx_setf(phy_get_ntx() + 1);

    // The SW manages by itself the STA TSF. Let's disable the corresponding HW feature.
    nxmac_tsf_mgt_disable_setf(1);

    nxmac_rx_start_delay_ofdm_setf(33+20);
    nxmac_rx_start_delay_short_setf(102+50);
    nxmac_rx_start_delay_long_setf(198+50);
}
