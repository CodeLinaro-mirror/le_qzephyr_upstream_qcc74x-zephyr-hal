/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MM_H_
#define _MM_H_

/**
 *****************************************************************************************
 * @defgroup LMAC LMAC
 * @ingroup MACSW
 * @brief Lower MAC SW module.
 *****************************************************************************************
 */

/**
 *****************************************************************************************
 * @defgroup MM MM
 * @ingroup LMAC
 * @brief LMAC MAC Management module.
 * @{
 *****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
// standard includes
#include "export/common/co_int.h"
#include "export/common/co_bool.h"

#include "export/common/co_utils.h"
#include "modules/ke/src/ke_timer.h"
#include "ip/lmac/src/mm/mm_timer.h"
#include "ip/lmac/src/mm/mm_task.h"
#include "ip/lmac/src/hal/hal_desc.h"
#include "ip/lmac/src/hal/hal_machw.h"
#if NX_GP_DMA
#include "hal_dma.h"
#endif
#include "ip/lmac/src/sta/sta_mgmt.h"

/*
 * DEFINES
 ****************************************************************************************
 */
/// Default TX power to apply when no regulatory info is available
#define MM_DEFAULT_TX_POWER 15

/// Maximum duration of an A-MPDU (in 32us units)
#define MM_DEFAULT_MAX_AMPDU_DURATION 150

/// Default peer device accuracy (in ppm)
#define MM_AP_CLK_ACCURACY 20

/// RX filter for monitoring mode (all packets allowed)
#define MM_RX_FILTER_MONITOR  (0xFFFFFFFF & ~(NXMAC_ACCEPT_ERROR_FRAMES_BIT | NXMAC_EXC_UNENCRYPTED_BIT |                      \
                                              NXMAC_EN_DUPLICATE_DETECTION_BIT))

/// RX filter for active mode (i.e. not monitoring)
#if NX_UMAC_PRESENT
#define MM_RX_FILTER_ACTIVE (NXMAC_ACCEPT_MULTICAST_BIT | NXMAC_ACCEPT_BROADCAST_BIT | \
                             NXMAC_ACCEPT_MY_UNICAST_BIT | NXMAC_ACCEPT_PROBE_REQ_BIT | \
                             NXMAC_ACCEPT_BEACON_BIT | NXMAC_ACCEPT_OTHER_MGMT_FRAMES_BIT | \
                             NXMAC_ACCEPT_BAR_BIT | NXMAC_ACCEPT_BA_BIT |                \
                             NXMAC_ACCEPT_DATA_BIT | NXMAC_ACCEPT_Q_DATA_BIT |           \
                             NXMAC_ACCEPT_QO_S_NULL_BIT | NXMAC_ACCEPT_OTHER_DATA_FRAMES_BIT)
#else
#define MM_RX_FILTER_ACTIVE (NXMAC_ACCEPT_MULTICAST_BIT | NXMAC_ACCEPT_BROADCAST_BIT | \
                             NXMAC_ACCEPT_MY_UNICAST_BIT | NXMAC_ACCEPT_PROBE_REQ_BIT | \
                             NXMAC_ACCEPT_BEACON_BIT | NXMAC_ACCEPT_OTHER_MGMT_FRAMES_BIT | \
                             NXMAC_ACCEPT_BAR_BIT | NXMAC_ACCEPT_BA_BIT |                \
                             NXMAC_ACCEPT_DATA_BIT | NXMAC_ACCEPT_Q_DATA_BIT |           \
                             NXMAC_ACCEPT_QO_S_NULL_BIT | NXMAC_ACCEPT_OTHER_DATA_FRAMES_BIT | \
                             NXMAC_ACCEPT_OTHER_BSSID_BIT)
#endif


/// Number of supported Default+Pairwise keys
#if NX_KEY_RAM_CONFIG
#define MM_SEC_MAX_KEY_NBR      (nxmac_sta_key_max_index_getf() + 1)
#else
#define MM_SEC_MAX_KEY_NBR      64
#endif

/// Number of VLAN Id
#if NX_KEY_RAM_CONFIG
#define MM_SEC_VLAN_COUNT (NX_VIRT_DEV_MAX + RW_MESH_LINK_NB)
#else
#define MM_SEC_VLAN_COUNT 6
#endif

/// Number of default keys
#define MM_SEC_DEFAULT_KEY_COUNT (MAC_DEFAULT_KEY_COUNT * MM_SEC_VLAN_COUNT)
#if (RW_MESH_EN)
/// Number of default keys for number of supported VIFs
#define MM_SEC_DEFAULT_VIF_KEY_COUNT    (MAC_DEFAULT_KEY_COUNT * NX_VIRT_DEV_MAX)
#endif //(RW_MESH_EN)

/// NULL cipher
#define MM_SEC_CTYPE_NULL        0
/// WEP (RC4) cipher
#define MM_SEC_CTYPE_WEP         1
/// TKIP cipher
#define MM_SEC_CTYPE_TKIP        2
/// CCMP cipher (128 and 256 bits)
#define MM_SEC_CTYPE_CCMP        3
/// WAPI (SMS4) cipher
#define MM_SEC_CTYPE_WPI_SMS4    4
/// GCMP cipher (128 and 256 bits)
#define MM_SEC_CTYPE_GCMP        5

/**
 * | hw idx | Desc        | TYPE     | usage         | macro               |
 * |--------+-------------+----------+---------------+---------------------|
 * |      0 | VIF-0       |          | RX if STA     |                     |\
 * |      1 |             | group    | TX if AP/MESH | MM_VIF_TO_KEY       | \
 * |      2 |             |          |               | (key_idx, vif_idx)  |  \
 * |      3 |             |          |               |                     |   \
 * |--------+-------------+----------+---------------+---------------------|    \  One set of
 * |      4 | VIF-1       |          | RX if STA     |                     |     | MAC_DEFAULT_KEY_COUNT
 * |      5 |             | group    | TX if AP/MESH | MM_VIF_TO_KEY       |     | group keys
 * |      6 |             |          |               | (key_idx, vif_idx)  |    /  per vif
 * |      7 |             |          |               |                     |   /
 * |--------+-------------+----------+---------------+---------------------|  /
 * |  [...] |             |          |               |                     | /
 * |--------+-------------+----------+---------------+---------------------|/
 * |     16 | MESH_LINK-0 |          | RX            | MM_MLINK_TO_KEY     |\
 * |     17 |             | group    |               | (key_idx, link_idx) | \   One set of
 * |     18 |             |          |               |                     |  \  MAC_DEFAULT_KEY_COUNT
 * |     19 |             |          |               |                     |   | group keys
 * |--------+-------------+----------+---------------+---------------------|  /  per MESH link
 * |  [...] |             |          |               |                     | /
 * |--------+-------------+----------+---------------+---------------------|/
 * |     24 | STA-0       | pairwise | RX/TX         | MM_STA_TO_KEY       |\   The rest of
 * |     25 | STA-1       | pairwise |               | (sta_idx)           | \  the MM_SEC_MAX_KEY_NBR
 * |     26 |             |          |               |                     |  | key are used
 * |  [...] |             |          |               |                     | /  for pairwise keys
 * |--------+-------------+----------+---------------+---------------------|/
 *
 * |--------+-------------+----------+---------------+---------------------|\
 * |     64 | MFP VIF-0   | MFP      | RX if STA     | MM_VIF_TO_MFP_KEY   | \
 * |     65 |             | group    | TX if AP/MESH | (key_idx, vif_idx)  |  \
 * |--------+-------------+----------+---------------+---------------------|   | MFP keys are not
 * |     66 | MFP VIF-1   | MFP      | RX if STA     | MM_VIF_TO_MFP_KEY   |   | stored in HW key ram.
 * |     67 |             | group    | TX if AP/MESH | (key_idx, vif_idx)  |   |
 * |--------+-------------+----------+---------------+---------------------|   | There are 2 MFP keys
 * |  [...] |             |          |               |                     |   | (idx 4 and 5)
 * |--------+-------------+----------+---------------+---------------------|   | per VIF/MESH link
 * |     72 | MFP         | MFP      | RX            | MM_MLINK_TO_MFP_KEY |   |
 * |     73 | MESH_LINK 0 | group    |               | (key_idx, vif_idx)  |   |
 * |--------+-------------+----------+---------------+---------------------|  /
 * |   [..] |             |          |               |                     | /
 * |--------+-------------+----------+---------------+---------------------|/
 *
 * Hw index are given as example for the config:
 * NX_VIRT_DEV_MAX = 4, RW_MESH_LINK_NB = 2 and MM_SEC_MAX_KEY_NBR = 64
 */

/// Convert a vif default/group key id to its HW index in key memory
#define MM_VIF_TO_KEY(key_idx, vif_idx)     ((key_idx) + ((vif_idx) * MAC_DEFAULT_KEY_COUNT))
/// Convert a STA index to the its HW pairwise key index in key memory
#define MM_STA_TO_KEY(sta_idx)              ((sta_idx) + MM_SEC_DEFAULT_KEY_COUNT)
#if (RW_MESH_EN)
/// Convert a MESH link default/group key index to its HW index in key memory
#define MM_MLINK_TO_KEY(key_idx, mlink_idx) MM_VIF_TO_KEY(key_idx, (NX_VIRT_DEV_MAX + mlink_idx))
#endif //(RW_MESH_EN)

/// Convert HW pairwise key index to corresponding STA index (hw_key_idx >= @ref MAC_SEC_DEFAULT_KEY_COUNT)
#define MM_KEY_TO_STA(hw_key_idx)    ((hw_key_idx) - MM_SEC_DEFAULT_KEY_COUNT)
/// Convert HW group key index to corresponding VIF index (hw_key_idx < @ref MAC_SEC_DEFAULT_KEY_COUNT)
#define MM_KEY_TO_VIF(hw_key_idx)    (((hw_key_idx) & ~(MAC_DEFAULT_KEY_COUNT - 1)) / MAC_DEFAULT_KEY_COUNT)
/// Convert HW group key index to corresponding Key id (hw_key_idx < @ref MAC_SEC_DEFAULT_KEY_COUNT)
#define MM_KEY_TO_KEYID(hw_key_idx)  ((hw_key_idx) & (MAC_DEFAULT_KEY_COUNT - 1))
#if (RW_MESH_EN)
/// Convert HW group key index to corresponding MESH link index (hw_key_idx < @ref MAC_SEC_DEFAULT_KEY_COUNT)
#define MM_KEY_TO_MLINK(hw_key_idx)  (MM_KEY_TO_VIF(hw_key_idx) - NX_VIRT_DEV_MAX)
#endif //(RW_MESH_EN)

#if NX_MFP
/// Number of MFP keys per VIF
#define MM_SEC_MFP_KEY_COUNT (MAC_DEFAULT_MFP_KEY_COUNT - MAC_DEFAULT_KEY_COUNT)
/// Maximum number of hardware key that can be defined when MFP is enabled
#define MM_SEC_MAX_MFP_KEY_NBR MM_SEC_MAX_KEY_NBR + (MM_SEC_MFP_KEY_COUNT * MM_SEC_VLAN_COUNT)

/// Convert a vif MFP group key id (4,5) to its HW idx (although not store in KEY RAM)
#define MM_VIF_TO_MFP_KEY(key_idx, vif_idx) ((((key_idx) - MAC_DEFAULT_KEY_COUNT) + ((vif_idx) * MM_SEC_MFP_KEY_COUNT)) + MM_SEC_MAX_KEY_NBR)
/// Convert HW MFP group key to corresponding VIF index (hw_key_idx in [@ref MM_SEC_MAX_KEY_NBR @ref MM_SEC_MAX_MFP_KEY_NBR[)
#define MM_MFP_KEY_TO_VIF(hw_key_idx) (((hw_key_idx) - MM_SEC_MAX_KEY_NBR) / MM_SEC_MFP_KEY_COUNT)
/// Convert HW MFP group key to corresponding Key id - 4 (hw_key_idx in [@ref MM_SEC_MAX_KEY_NBR @ref MM_SEC_MAX_MFP_KEY_NBR[)
#define MM_MFP_KEY_TO_KEYID_BASE(hw_key_idx) (((hw_key_idx) - MM_SEC_MAX_KEY_NBR) & (MM_SEC_MFP_KEY_COUNT - 1))
/// Convert HW MFP group key to corresponding Key id (hw_key_idx in [@ref MM_SEC_MAX_KEY_NBR @ref MM_SEC_MAX_MFP_KEY_NBR[)
#define MM_MFP_KEY_TO_KEYID(hw_key_idx) (MM_MFP_KEY_TO_KEYID_BASE(hw_key_idx) + MAC_DEFAULT_KEY_COUNT)

#if (RW_UMESH_EN)
/// Convert a MESH link MFP group key id (4,5) to its HW idx (although not store in KEY RAM)
#define MM_MLINK_TO_MFP_KEY(key_idx, mlink_idx)  MM_VIF_TO_MFP_KEY(key_idx, mlink_idx + NX_VIRT_DEV_MAX)
/// Convert HW MFP group key to corresponding MLINK index (hw_key_idx in [@ref MM_SEC_MAX_KEY_NBR @ref MM_SEC_MAX_MFP_KEY_NBR[)
#define MM_MFP_KEY_TO_MLINK(hw_key_idx) (MM_MFP_KEY_TO_VIF(hw_key_idx) - NX_VIRT_DEV_MAX)
#endif //(RW_UMESH_EN)
#endif //(NX_MFP)

/// Delay between call to tbtt_timer callback for AP VIFs and AP_TBTT
#define MM_PRE_AP_TBTT_DELAY_US    (1000)

#if (NX_ANT_DIV)
/// Antenna Diversity RSSI threshold
#define ANT_DIV_RSSI_THD                (20)
/// Antenna Diversity RSSI golden range min
#define ANT_DIV_RSSI_GOLDEN_MIN         (-55)
/// Antenna Diversity RSSI golden range max
#define ANT_DIV_RSSI_GOLDEN_MAX         (-20)
/// Antenna Diversity RSSI step change
#define ANT_DIV_RSSI_STEP_CHANGE        (5)
/// Antenna Diversity beacon loss threshold
#define ANT_DIV_BEACON_LOSS_THD         (3)
/// Antenna Diversity RSSI reset value
#define ANT_DIV_RSSI_RESET              (-128)
/// Antenna Diversity update counter threshold
#define ANT_DIV_UPDATE_CNT_THD          (100)
#endif //(NX_ANT_DIV)

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

///BA agreement types
enum
{
    ///BlockAck agreement for TX
    BA_AGMT_TX,
    ///BlockAck agreement for RX
    BA_AGMT_RX,
};


///BA agreement related status
enum
{
    /// Correct BA agreement establishment
    BA_AGMT_ESTABLISHED,
    /// BA agreement already exists for STA+TID requested, cannot override it (should have been deleted first)
    BA_AGMT_ALREADY_EXISTS,
    /// Correct BA agreement deletion
    BA_AGMT_DELETED,
    /// BA agreement for the (STA, TID) doesn't exist so nothing to delete
    BA_AGMT_DOESNT_EXIST,
    /// No more BA agreement can be added for the specified type
    BA_AGMT_NO_MORE_BA_AGMT,
    /// BA agreement type not supported
    BA_AGMT_NOT_SUPPORTED
};

/// Features supported by LMAC - Positions
enum mm_features
{
    /// Beaconing
    MM_FEAT_BCN_BIT         = 0,
    /// Radar Detection
    MM_FEAT_RADAR_BIT,
    /// Power Save
    MM_FEAT_PS_BIT,
    /// UAPSD
    MM_FEAT_UAPSD_BIT,
    /// A-MPDU
    MM_FEAT_AMPDU_BIT,
    /// A-MSDU
    MM_FEAT_AMSDU_BIT,
    /// P2P
    MM_FEAT_P2P_BIT,
    /// P2P Go
    MM_FEAT_P2P_GO_BIT,
    /// UMAC Present
    MM_FEAT_UMAC_BIT,
    /// VHT support
    MM_FEAT_VHT_BIT,
    /// Beamformee
    MM_FEAT_BFMEE_BIT,
    /// Beamformer
    MM_FEAT_BFMER_BIT,
    /// WAPI
    MM_FEAT_WAPI_BIT,
    /// MFP
    MM_FEAT_MFP_BIT,
    /// Mu-MIMO RX support
    MM_FEAT_MU_MIMO_RX_BIT,
    /// Mu-MIMO TX support
    MM_FEAT_MU_MIMO_TX_BIT,
    /// Wireless Mesh Networking
    MM_FEAT_MESH_BIT,
    /// TDLS support
    MM_FEAT_TDLS_BIT,
    /// Antenna Diversity support
    MM_FEAT_ANT_DIV_BIT,
    /// UF support
    MM_FEAT_UF_BIT,
    /// A-MSDU maximum size (bit0)
    MM_AMSDU_MAX_SIZE_BIT0,
    /// A-MSDU maximum size (bit1)
    MM_AMSDU_MAX_SIZE_BIT1,
    /// MON_DATA support
    MM_FEAT_MON_DATA_BIT,
    /// HE (802.11ax) support
    MM_FEAT_HE_BIT,
    /// TWT support
    MM_FEAT_TWT_BIT,
    /// FTM initiator support
    MM_FEAT_FTM_INIT_BIT,
    /// Fake FTM responder support
    MM_FEAT_FAKE_FTM_RSP_BIT
};


// Forward declaration
struct vif_info_tag;

#if (NX_ANT_DIV)
/// Antenna Diversity structure
struct antenna_diversity
{
    /// Boolean indicating if the algorithm is enabled
    bool enable;
    /// Boolean indicating if the antenna can be switched
    bool sw_enable;
    /// Current RSSI
    int8_t rssi[2];
    /// Initial RSSI
    int8_t init_rssi[2];
    /// Current antenna
    uint8_t cur_ant;
    /// Boolean indicating if the RSSI should be updated
    bool init_rssi_upd_req;
    /// Counter of beacon periods spent without any antenna switch outside of the golden range
    uint32_t update_cnt;
    /// Counter of active VIFs
    uint8_t nvif_active;
};
#endif //(NX_ANT_DIV)

#if (QC7XX_RA_EN)
#define MM_LINK_VER                 (2)
#define MM_LINK_SAMPLE_MAX          (8)
#define MM_LINK_SAMPLE_PERIOD       (2 * TU_DURATION)
#define MM_LINK_TIMEOUT             (1000 * TU_DURATION)
#define MM_LINK_TX_THR              (200)
#define MM_LINK_RX_THR              (200)
#ifdef CFG_LPM
#define MM_LINK_NOISE_HIST_NUM      (10)
#define MM_LINK_NOISE_SAMPLE_PERIOD (32)
#endif

enum
{
    MM_LINK_CLEAN = 0,
    MM_LINK_CONGESTION,
    MM_LINK_OPEN
};

#ifdef CFG_LPM
enum
{
    MM_LINK_NOISE_LOW = 0,
    MM_LINK_NOISE_MID,
    MM_LINK_NOISE_HIGH
};
#endif

struct mm_link_env_tag
{
    #if MM_LINK_VER == 1
    uint8_t  ccad_idx;
    uint32_t ccad_dur[MM_LINK_SAMPLE_MAX];
    uint32_t ccad_sample_time;
    uint32_t ccad_sample_dur;
    uint32_t ccad_sample_tot;
    uint32_t ccad_acc_sample_dur;
    uint32_t ccad_acc_sample_tot;
    uint32_t ccad_free_sample_time;
    uint32_t ccad_free_sample_dur;
    uint32_t ccad_free_sample_tot;
    #endif

    #if MM_LINK_VER == 2
    uint32_t tx_cnt;
    uint32_t rx_cnt;
    uint32_t ccad_sample_time;
    uint32_t ccad_sample_dur;
    uint32_t ccad_sample_tot;
    uint32_t ccad_last_update_time;
    uint32_t ccad_last_dur;
    uint32_t ccad_last_tot;
    uint32_t ccad_update_cnt;
    #endif

    #ifdef CFG_LPM
    uint8_t rx_noise_hist_cnt;
    uint16_t rx_noise_hist[MM_LINK_NOISE_HIST_NUM]; // record sample below -95+i*5
    int8_t rx_noise_avg;
    int8_t rx_noise_min;
    int8_t rx_noise_max;
    uint8_t rx_noise_cnt;
    uint8_t rx_noise_state;
    uint8_t rx_noise_class;
    int8_t rx_noise_mu;
    int8_t rx_noise_hi;
    uint32_t rx_noise_time;

    uint32_t rssi_sample_cnt;
    #if LINK_STAT_LOG
    uint32_t logtime_rssi;
    #endif
    #else
    uint16_t noise_cnt_total;
    uint16_t noise_cnt_100;
    uint16_t noise_cnt_80;
    uint16_t noise_cnt_65;
    uint16_t noise_cnt_50;
    #endif // CFG_LPM
};
#endif // (QC7XX_RA_EN)

#ifdef CFG_LPM
struct bbp_ctrl
{
    bool enable;
};

struct ack_ctrl
{
    bool boost;
    uint32_t boost_time;
};
#endif

/// LMAC MAC Management Context
struct mm_env_tag
{
    /// RX filtering requested by the Upper MAC
    uint32_t rx_filter_umac;
    /// RX filtering forced to be enabled by the Lower MAC
    uint32_t rx_filter_lmac_enable;
    /// Per-AC maximum A-MPDU duration value (in 32us unit)
    uint16_t ampdu_max_dur[NX_TXQ_CNT];
    /// Previous state of the MM task, prior to go to IDLE
    uint8_t prev_mm_state;
    /// Previous state of the HW, prior to go to IDLE
    uint8_t prev_hw_state;
    /// Basic rates per band
    uint32_t basic_rates[PHY_BAND_MAX];
    //sta keep alive time
    //unit:seconds
    uint8_t time_keepalive;
    #if NX_UAPSD
    /// UAPSD Timer timeout value, in microseconds
    uint32_t uapsd_timeout;
    #endif
    #if NX_POWERSAVE
    /// Local LP clock accuracy (in ppm)
    uint16_t lp_clk_accuracy;
    #endif
    /// IDLE state requested by the host
    uint8_t host_idle;
    #if (NX_ANT_DIV)
    /// Antenna Diversity structure
    struct antenna_diversity ant_div;
    #endif //(NX_ANT_DIV)
    #if NX_BEACONING
    /// Timer structure used to schedule TBTT moves
    struct mm_timer_tag tbtt_move_tmr;
    /// Flag indicating that a TBTT move is ongoing
    bool tbtt_move_ongoing;
    #endif
    /// Last key cookie
    uint8_t key_cookie;
    #if QC7XX_RA_EN
    /// Link statistics
    struct mm_link_env_tag link;
    #endif
    #ifdef CFG_LPM
    struct bbp_ctrl bbp;
    struct ack_ctrl ack;
    #endif
};

/*
 * GLOBAL VARIABLES
 ****************************************************************************************
 */
extern struct mm_env_tag mm_env;

/*
 * FUNCTION PROTOTYPES
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief This function applies the RX filter requested by the UMAC and LMAC.
 *
 ****************************************************************************************
 */
__INLINE void mm_rx_filter_set(void)
{
    // Configure the filtering of the HW
    ASSERT_WARN(nxmac_current_state_getf() == HW_IDLE);
    nxmac_rx_cntrl_set(mm_env.rx_filter_umac | mm_env.rx_filter_lmac_enable);
}

/**
 ****************************************************************************************
 * @brief This function sets the RX filter for the UMAC.
 *
 * @param[in] filter  Filter bitfield requested by the UMAC
 ****************************************************************************************
 */
__INLINE void mm_rx_filter_umac_set(uint32_t filter)
{
    mm_env.rx_filter_umac = filter;

    // Configure the filtering of the HW
    mm_rx_filter_set();
}

/**
 ****************************************************************************************
 * @brief This function enables some RX filter bits on the LMAC filter
 *
 * @param[in] filter  Bits to be set
 ****************************************************************************************
 */
__INLINE void mm_rx_filter_lmac_enable_set(uint32_t filter)
{
    mm_env.rx_filter_lmac_enable |= filter;

    // Configure the filtering of the HW
    mm_rx_filter_set();
}

/**
 ****************************************************************************************
 * @brief This function disables some RX filter bits on the LMAC filter
 *
 * @param[in] filter  Bits to be cleared
 ****************************************************************************************
 */
__INLINE void mm_rx_filter_lmac_enable_clear(uint32_t filter)
{
    mm_env.rx_filter_lmac_enable &= ~filter;

    // Configure the filtering of the HW
    mm_rx_filter_set();
}

/**
 ****************************************************************************************
 * @brief Sets the basic rate for the MM module.
 *
 * @param[in] rate The basic rate to set.
 ****************************************************************************************
 */
__INLINE void mm_basic_rate_set(uint32_t rate)
{
    rate = rate & 0xf & ~(1 << HW_RATE_2MBPS) ? rate & ~(1 << HW_RATE_2MBPS) : rate;
    nxmac_rates_set(rate);
}

/**
 ****************************************************************************************
 * @brief This function indicates a PS state change of a peer device to the upper layers.
 *
 * @param[in] sta_idx  Index of the peer device in the station table
 * @param[in] ps_state New PS state of the peer device
 ****************************************************************************************
 */
__INLINE void mm_ps_change_ind(uint8_t sta_idx, uint8_t ps_state)
{
#ifdef CFG_ENABLE_AP_PS
    struct sta_info_tag *sta = &sta_info_tab[sta_idx];

    #if (NX_P2P)
    if (sta->ps_state != ps_state)
    #endif //(NX_P2P)
    {
        struct mm_ps_change_ind *ind = KE_MSG_ALLOC(MM_PS_CHANGE_IND, TASK_API,
                                                        TASK_MM, mm_ps_change_ind);

        // Change the local value of the PS state
        sta->ps_state = ps_state;

        // Fill-in the parameters
        ind->sta_idx = sta_idx;
        ind->ps_state = ps_state;

        // Send the message
        ke_msg_send(ind);
    }
#endif
}

/**
 ****************************************************************************************
 * @brief This function requests some buffered packets to be transmitted
 *
 * @param[in] sta_idx  Index of the peer device we have to transmit to
 * @param[in] pkt_cnt  Number of packets that need to be transmitted
 * @param[in] uapsd    Flag indicating if the request is for U-APSD queues or not
 ****************************************************************************************
 */
__INLINE void mm_traffic_req_ind(uint8_t sta_idx, uint8_t pkt_cnt, bool uapsd)
{
#ifdef CFG_ENABLE_AP_PS
    struct mm_traffic_req_ind *ind = KE_MSG_ALLOC(MM_TRAFFIC_REQ_IND, TASK_API,
                                                    TASK_MM, mm_traffic_req_ind);

    PROF_PS_TRAFFIC_REQ_SET();

    // Fill-in the parameters
    ind->sta_idx = sta_idx;
    ind->pkt_cnt = pkt_cnt;
    ind->uapsd = uapsd;

    // Send the message
    ke_msg_send(ind);

    PROF_PS_TRAFFIC_REQ_CLR();
#endif
}

#if (NX_ANT_DIV)
/**
 ****************************************************************************************
 * @brief This function calculates the distance of the RSSI from the golden range
 *
 * @param[in] rssi     RSSI value
 *
 * @return Distance of RSSI from the golden range
 ****************************************************************************************
 */
__INLINE uint8_t mm_get_golden_range_distance(int8_t rssi)
{
    uint8_t dist = 0;
    if (rssi < ANT_DIV_RSSI_GOLDEN_MIN)
    {
        dist = ANT_DIV_RSSI_GOLDEN_MIN - rssi;
    }
    else if (rssi > ANT_DIV_RSSI_GOLDEN_MAX)
    {
        dist = rssi - ANT_DIV_RSSI_GOLDEN_MAX;
    }

    return dist;
}
#endif //(NX_ANT_DIV)

/**
 ****************************************************************************************
 * @brief MM Module main initialization function.
 * This function is called after reset and initializes all MM related env and data.
 ****************************************************************************************
 */
void mm_init(void);

#ifdef CFG_QCC74x_WIFI_PS_ENABLE
/**
 ****************************************************************************************
 * @brief MM Module main in ps module initialization function.
 * This function is called after ps resume and initializes MM related env and data.
 ****************************************************************************************
 */
void mm_ps_init(void);
#endif

/**
 ****************************************************************************************
 * @brief MM Module reset function.
 * This function is part of the recovery mechanism invoked upon an error detection in the
 * LMAC. It resets the MM state machine.
 ****************************************************************************************
 */
void mm_reset(void);

/**
 ****************************************************************************************
 * @brief Put the HW state to active.
 ****************************************************************************************
 */
void mm_active(void);

/**
 ****************************************************************************************
 * @brief MM Module mm_env_init routine.
 ****************************************************************************************
 */
void mm_env_init(void);


/**
 ****************************************************************************************
 * @brief Set the maximum A-MPDU duration based on TXOP registers.
 ****************************************************************************************
 */
void mm_env_max_ampdu_duration_set(void);


/**
 ****************************************************************************************
 * @brief Function to add a MAC address in the MAC HW key storage.
 * This function is called from MM_STA_ADD_REQ handler.
 *
 * @param[in] sta_idx  Index of the station
 * @param[in] inst_nbr Index of the VIF
 *
 * @return The HW STA index for the new STA
 *
 ****************************************************************************************
 */
uint8_t mm_sec_machwaddr_wr(uint8_t sta_idx, uint8_t inst_nbr);

/**
 ****************************************************************************************
 * @brief Function to delete a MAC address from the MAC HW key storage.
 * This function is called from MM_STA_DEL_REQ handler.
 *
 * @param[in] sta_idx Index of the station
 *
 ****************************************************************************************
 */
void mm_sec_machwaddr_del(uint8_t sta_idx);

/**
 ****************************************************************************************
 * @brief Function to get new key cookie.
 *
 * @return Key cookie
 *
 ****************************************************************************************
 */
uint8_t mm_sec_key_new_cookie();

/**
 ****************************************************************************************
 * @brief Function to write key in MAC HW.
 * This function is called from MM_KEY_ADD_REQ handler to store a key into the
 * KeyStoragRAM of the MAC HW.
 *
 * @param[in] param     Pointer to the parameters of the message.
 *
 * @return The HW key index for the new key
 *
 ****************************************************************************************
 */
uint8_t mm_sec_machwkey_wr(struct mm_key_add_req const *param);

/**
 ****************************************************************************************
 * @brief Function to delete a key from the MAC HW.
 * This function is called from MM_KEY_DEL_REQ handler to remove a key from the
 * KeyStoragRAM of the MAC HW.
 *
 * @param[in] hw_key_idx  Index of the key to be removed
 *
 ****************************************************************************************
 */
void mm_sec_machwkey_del(uint8_t hw_key_idx);

#if NX_BEACONING
/**
 ****************************************************************************************
 * @brief Kernel event handler for the TBTT event.
 *
 * This function flushes the beacon queue and signals the event to the upper
 * MAC.
 *
 * @param[in] dummy Parameter not used but required to follow the kernel event callback
 * format
 ****************************************************************************************
 */
void mm_tbtt_evt(int dummy);
#endif

/**
 ****************************************************************************************
 * @brief Function responsible for all the connection monitoring stuff. It resets the
 * beacon loss counter, checks if the beacon has changed compared to the previous one
 * (in order to know if it has to be forwarded to the upper layers or not), and sends the
 * keep-alive NULL frame if it is time to do so.
 *
 * @param[in] rhd         Pointer to the beacon RX descriptor
 * @param[in] vif         Pointer to the VIF entry
 * @param[in] sta         Pointer to the STA entry that sent the beacon
 * @param[out] tim        Pointer to the TIM IE found in the beacon
 *
 * @return true if the beacon has to be uploaded (i.e because it changed since the last
 * one), false otherwise.
 *
 ****************************************************************************************
 */
bool mm_check_beacon(struct rx_hd *rhd, struct vif_info_tag *vif,
                     struct sta_info_tag *sta, uint32_t *tim);

/**
 ****************************************************************************************
 * @brief Kernel event handler for the HW IDLE event.
 *
 * This function puts the MM task in IDLE state.
 *
 * @param[in] dummy Parameter not used but required to follow the kernel event callback
 * format
 ****************************************************************************************
 */
void mm_hw_idle_evt(int dummy);

/**
 ****************************************************************************************
 * @brief Setting of general HW info upon the first interface creation
 *
 * @param[in] mac_addr Primary MAC address of the interface
 ****************************************************************************************
 */
void mm_hw_info_set(struct mac_addr const *mac_addr);

/**
 ****************************************************************************************
 * @brief Set the correct HW information for an AP
 ****************************************************************************************
 */
void mm_hw_ap_info_set(void);

/**
 ****************************************************************************************
 * @brief Reset the HW information for an AP
 ****************************************************************************************
 */
void mm_hw_ap_info_reset(void);

/**
 ****************************************************************************************
 * @brief Configure tbtt_timer for next beacon
 *
 * Configure tbtt_timer of the vif to be triggered before the next expected beacon
 *
 * @param[in] bcn         Pointer to the beacon
 * @param[in] len         Length of the beacon
 * @param[in] rhd         Pointer to the beacon RX descriptor
 * @param[in] vif         Pointer to the VIF entry
 * @param[in] sta         Pointer to the STA entry that send the beacon
 * @param[in] tim         Pointer to the TIM IE found in the beacon
 ****************************************************************************************
 */
void mm_tbtt_compute(struct bcn_frame *bcn, uint16_t len, struct rx_hd *rhd,
                     struct vif_info_tag *vif,
                     struct sta_info_tag *sta, uint32_t tim);

/**
 ****************************************************************************************
 * @brief MM timer callback for the station TBTT handling
 *
 * @param[in] env  Pointer to the environment to be passed to the callback upon timer
 *                 expiry
 ****************************************************************************************
 */
void mm_sta_tbtt(void *env);

/**
 ****************************************************************************************
 * @brief MM timer callback for the AP pre-TBTT handling
 *
 * @param[in] env  Pointer to the environment to be passed to the callback upon timer
 *                 expiry
 ****************************************************************************************
 */
void mm_ap_pre_tbtt(void *env);

/**
 ****************************************************************************************
 * @brief Request to apply an offset (positive or negative) to the next TBTT.
 * Depending on the current position within the beacon period the function will chose to
 * apply immediately the offset or program a timer to apply later.
 *
 * @param[in] offset Offset to apply to the TBTT (can be positive to delay the TBTT or
 * negative to advance it)
 *
 * @return The offset that will be applied (0 if no offset applied).
 *
 ****************************************************************************************
 */
int32_t mm_ap_tbtt_move(int32_t offset);

extern struct mm_env_tag mm_env;

#if (NX_AMPDU_TX)
/**
 ****************************************************************************************
 * @brief Simple check for existence of a BA agreement for a certain (STA, TID) pair (TX)
 * @param[in] sta_idx     STA index in STA info table for which BA agreement existence must be checked
 * @param[in] tid         TID corresponding to STA index in STA info table for which BA agreement existence must be checked
 * @return 1 if existent, 0 if not.
 ****************************************************************************************
 */
bool mm_ba_agmt_tx_exists(uint8_t sta_idx, uint8_t tid);
#endif //(NX_AMPDU_TX)

#if (NX_AMPDU_RX)
/**
 ****************************************************************************************
 * @brief Simple check for existence of a BA agreement for a certain (STA, TID) pair (RX)
 * @param[in] sta_idx     STA index in STA info table for which BA agreement existence must be checked
 * @param[in] tid         TID corresponding to STA index in STA info table for which BA agreement existence must be checked
 * @return 1 if existent, 0 if not.
 ****************************************************************************************
 */
bool mm_ba_agmt_rx_exists(uint8_t sta_idx, uint8_t tid);
#endif //(NX_AMPDU_RX)

/**
 ****************************************************************************************
 * @brief Force the HW to go to IDLE immediately
 ****************************************************************************************
 */
void mm_force_idle_req(void);


/**
 ****************************************************************************************
 * @brief Add a station to the list of known stations
 *
 * @param[in] param         MM_STA_ADD_REQ message parameters
 * @param[out] sta_idx      Pointer to which the allocated STA index should be written
 * @param[out] hw_sta_idx   Pointer to which the allocated STA HW index should be written
 *
 * @return The status of the allocation (@ref CO_OK if successful)
 ****************************************************************************************
 */
uint8_t mm_sta_add(struct mm_sta_add_req const *param, uint8_t  *sta_idx,
                   uint8_t *hw_sta_idx);

/**
 ****************************************************************************************
 * @brief Delete a station from the list of known stations
 *
 * @param[in] sta_idx Index of the station to delete
 ****************************************************************************************
 */
void mm_sta_del(uint8_t sta_idx);

/**
 ****************************************************************************************
 * @brief Ask MM to switch back to IDLE state requested by the host (quit BYPASSED mode)
 ****************************************************************************************
 */
void mm_back_to_host_idle(void);

/**
 ****************************************************************************************
 * @brief Ask MM to disallow the switch to IDLE state.
 * This function is called upon beacon programming to ensure that the MAC HW proceeds to
 * the packet transmission.
 ****************************************************************************************
 */
void mm_no_idle_start(void);

/**
 ****************************************************************************************
 * @brief Ask MM to allow again the switch to IDLE state.
 * This function is called upon beacon transmission confirmation from the MAC HW.
 ****************************************************************************************
 */
void mm_no_idle_stop(void);

/**
 ****************************************************************************************
 * @brief Inform the host that connection has been lost with an AP.
 *
 * @param[in] vif         VIF Entry used for the connection.
 ****************************************************************************************
 */
void mm_send_connection_loss_ind(struct vif_info_tag *vif, uint16_t status_code);

/**
 ****************************************************************************************
 * @brief Check RSSI level and inform the host if RSSI is decreased below the threshold
 * or if it is increased above the threshold.
 *
 * @param[in] vif        Pointer to the VIF entry
 * @param[in] rssi       RSSI received
 ****************************************************************************************
 */
void mm_check_rssi(struct vif_info_tag *vif, int8_t rssi);

#if NX_UMAC_PRESENT
/**
 ****************************************************************************************
 * @brief Send the @ref MM_PKTLOSS_IND message to the host.
 *
 * @param[in] vif         VIF Entry used for the connection.
 * @param[in] sta_idx     STA index in STA info table for which the packet loss msg is sent.
 * @param[in] num_pkts    Number of packets lost.
 ****************************************************************************************
 */
void mm_send_pktloss_ind(struct vif_info_tag *vif, uint8_t sta_idx, uint32_t num_pkts);

/**
 ****************************************************************************************
 * @brief Send host message to stop (resp restarted) tx traffic because CSA is in
 * progress (resp. finished)
 *
 * @param[in] vif_index  Vif index
 * @param[in] enable     Traffic status
 ****************************************************************************************
 */
void mm_send_csa_traffic_ind(uint8_t vif_index, bool enable);
#endif

#if (NX_ANT_DIV)
/**
 ****************************************************************************************
 * @brief Initialize the antenna diversity parameters.
 *
 * @param[in] enable Whether the algorithm is enabled or not
 ****************************************************************************************
 */
void mm_ant_div_init(bool enable);

/**
 ****************************************************************************************
 * @brief Switch antenna, if no other active connections are set up and if the algorithm
 * is enabled. Then disable the antenna switch.
 ****************************************************************************************
 */
void mm_ant_div_switch_and_stop(void);

/**
 ****************************************************************************************
 * @brief Update the number of active VIF and update accordingly the status of the antenna
 * switch flag.
 *
 * @param[in] active VIF status
 ****************************************************************************************
 */
void mm_ant_div_restore(bool active);

/**
 ****************************************************************************************
 * @brief Update the status of the antenna switch flag.
 ****************************************************************************************
 */
void mm_ant_div_update(void);

/**
 ****************************************************************************************
 * @brief Change the antenna connected to path_0, reset the update counter and request an
 * update of the initial RSSI (case 1x1 station)
 ****************************************************************************************
 */
void mm_ant_div_switch_antenna(void);

/**
 ****************************************************************************************
 * @brief Reset the current and the initial RSSI values in the antenna diversity structure
 * for the requested antenna.
 *
 * @param[in] ant Antenna for which the RSSI reset is requested
 ****************************************************************************************
 */
void mm_ant_div_reset_rssi(uint8_t ant);

/**
 ****************************************************************************************
 * @brief Update the RSSI values for both antennas.
 *
 * @param[in] rssi Pointer to the RSSI values for both antennas.
 ****************************************************************************************
 */
void mm_ant_div_update_rssi(int8_t *rssi);

/**
 ****************************************************************************************
 * @brief This function is called at each tbtt, it updates the counter of beacon periods
 * without any antenna switch (2x2 station only) and it checks if the antenna connected
 * to path_0 should be switched.
 * In case of 1x1 station, an antenna switch is requested if:
 * - more than @ref ANT_DIV_BEACON_LOSS_THD consecutive beacons are lost
 * - the RSSI of the current antenna is outside of the golden range and:
 *    - the distance of the other antenna RSSI from the golden range is less than the
 *      distance of the current antenna RSSI from the golden range
 *    - or the current antenna RSSI is changed of more than @ref ANT_DIV_RSSI_STEP_CHANGE
 *      compared to its initial RSSI
 *    - or it has not been updated for at least @ref ANT_DIV_UPDATE_CNT_THD beacon periods
 * In case of 2x2 station, an antenna switch is requested if the RSSI of the antenna
 * connected to path_1 is greater of at least @ref ANT_DIV_RSSI_THD than the RSSI of the
 * antenna connected to path_0.
 * If the switch is requested, the @ref MM_SWITCH_ANTENNA_REQ message is sent.
 *
 * @param[in] beacon_loss_cnt Number of consecutive beacons lost
 ****************************************************************************************
 */
void mm_ant_div_check(uint8_t beacon_loss_cnt);

#endif //(NX_ANT_DIV)

#ifdef CFG_LPM
void mm_ack_init();
void mm_ack_check(int8_t rssi);
#endif

#if QC7XX_RA_EN
void mm_link_start();
void mm_link_tx_cb();
void mm_link_rx_cb();
void mm_link_timer_cb();
uint8_t mm_link_get_status();
#ifdef CFG_LPM
uint8_t mm_link_get_cca_dur();
int32_t mm_link_ewma_filter(int32_t prev, int32_t in, uint8_t ewma_level_in_256);
int8_t mm_link_get_snr();
int8_t mm_link_get_noise();
uint8_t mm_link_get_noise_class();
uint32_t mm_link_get_update_time();
void mm_link_update_rx_statistic(struct sta_info_tag *sta, struct rx_hd *rhd, uint16_t framectrl);
#endif // CFG_LPM
#endif // (QC7XX_RA_EN)

#ifdef CFG_LPM
void mm_bbp_start();
void mm_bbp_stop();
void mm_start_ac(uint8_t access_category);
void mm_halt_ac(uint8_t access_category);
#endif

/**
 ****************************************************************************************
 * @brief get keepalive period(unit:second) when sta mode
 *
 ****************************************************************************************
 */
uint16_t mm_get_keepalive_period(void);

#ifdef CFG_LPM
void mm_bbp_start();
void mm_bbp_stop();
#endif

/**
 ****************************************************************************************
 * @brief Store keyram config before enter pds.
 *
 ****************************************************************************************
 */
int keyram_pds_enter(int nap_time);

/**
 ****************************************************************************************
 * @brief Resume keyram config after exit pds.
 *
 ****************************************************************************************
 */
int keyram_pds_exit(int usepds);

/// @} end of group

#endif // _MM_H_
