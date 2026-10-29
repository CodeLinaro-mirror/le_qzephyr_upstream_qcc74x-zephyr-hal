/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 ****************************************************************************************
 * @addtogroup PLATFORM_DRIVERS
 * @{
 ****************************************************************************************
 */

#include <stdint.h>
#include "reg_sysctrl.h"
#include "reg_mac_core.h"
#include "reg_mac_pl.h"
#include "mac_types.h"
#include "qcc74x_lp.h"
#include "hal_machw.h"
#include "rxl_hwdesc.h"

void crm_init(void); // phyrf

#define SYSCTRL_DIAG_SEL_EN_BIT      ((uint32_t)0x80000000)
#define DIAG_MAC             12
/// Clock gating mask
#define CLOCK_GATING_BITS (SYSCTRL_MAC_PI_CLK_GATING_EN_BIT | SYSCTRL_MAC_PI_TX_CLK_GATING_EN_BIT |\
                           SYSCTRL_MAC_PI_RX_CLK_GATING_EN_BIT | SYSCTRL_MAC_CORE_CLK_GATING_EN_BIT |\
                           SYSCTRL_MAC_CRYPT_CLK_GATING_EN_BIT | SYSCTRL_MAC_CORE_TX_CLK_GATING_EN_BIT |\
                           SYSCTRL_MAC_CORE_RX_CLK_GATING_EN_BIT | SYSCTRL_MAC_WT_CLK_GATING_EN_BIT |\
                           SYSCTRL_MPIF_CLK_GATING_EN_BIT)

void sysctrl_init(void)
{
    // Set DIAG MUX
    sysctrl_diag_conf1_set(SYSCTRL_DIAG_SEL_EN_BIT | DIAG_MAC);

    // Enable Clock gating
    sysctrl_misc_cntl_set(sysctrl_misc_cntl_get() | CLOCK_GATING_BITS);
}

typedef struct __ps_keyram_slot {
    uint32_t bssid[2];
    uint8_t  keylen;
    uint8_t  ctype;
    uint8_t  vlanid;
    uint8_t  spp;
    uint8_t  usedefkey;
    uint32_t key[4];
} ps_keyram_slot_t;

typedef struct __ps_macaddr {
    uint32_t mac[2];
    uint32_t mac_mask[2];
    uint32_t bssid[2];
    uint32_t bssid_mask[2];
} ps_macaddr_t;

typedef struct __ps_edca_ac {
    uint32_t ac0;
    uint32_t ac1;
    uint32_t ac2;
    uint32_t ac3;
} ps_edca_ac_t;

typedef struct __ps_pwr_mgt {
    uint8_t pwrbit;
} ps_pwr_mgt_t;

typedef struct __test_wifi_save_arg {
    uint8_t flag;//default
    ps_macaddr_t macaddr;
    ps_edca_ac_t edca;
    ps_pwr_mgt_t pwrmgt;
    uint8_t slot_max;
    ps_keyram_slot_t slot[64];
} ps_save_arg_t;

ps_save_arg_t *keyram_arg;


#define WLAN_MAC_TZC_DEBUG_REGISTER (*(volatile uint32_t *)(0x20005198))

#define KEY_RAM_WR(key_idx, ctype, vif_idx, spp, usedefkey, clen)                        \
{                                                                                        \
    nxmac_encr_cntrl_pack(0, 1, 0, 0, key_idx, clen, ctype, vif_idx, spp, usedefkey);    \
    while(nxmac_new_write_getf());                                                       \
}


int keyram_restore(uint32_t addr)
{
    int idx;

    keyram_arg = (ps_save_arg_t *)(addr);

    WLAN_MAC_TZC_DEBUG_REGISTER &= ~0x01;

    /* ==========================  Keyram ========================= */
    for (idx = 0; idx <= keyram_arg->slot_max; idx++) {
        nxmac_bss_id_low_set      (keyram_arg->slot[idx].bssid[0]);
        nxmac_bss_id_hi_set       (keyram_arg->slot[idx].bssid[1]);
        nxmac_encr_mac_addr_low_set(keyram_arg->slot[idx].bssid[0]);
        nxmac_encr_mac_addr_high_set(keyram_arg->slot[idx].bssid[1]);
        nxmac_c_len_ram_setf      (keyram_arg->slot[idx].keylen);
        nxmac_c_type_ram_setf     (keyram_arg->slot[idx].ctype);
        nxmac_vlan_idram_setf     (keyram_arg->slot[idx].vlanid);
        nxmac_spp_ram_setf        (keyram_arg->slot[idx].spp);
        nxmac_use_def_key_ram_setf(keyram_arg->slot[idx].usedefkey);
        nxmac_encr_key_0_set      (keyram_arg->slot[idx].key[0]);
        nxmac_encr_key_1_set      (keyram_arg->slot[idx].key[1]);
        nxmac_encr_key_2_set      (keyram_arg->slot[idx].key[2]);
        nxmac_encr_key_3_set      (keyram_arg->slot[idx].key[3]);

        nxmac_encr_cntrl_pack(0, 1, 0, 0, idx, keyram_arg->slot[idx].keylen, keyram_arg->slot[idx].ctype, keyram_arg->slot[idx].vlanid, keyram_arg->slot[idx].spp, 0);

        while(nxmac_new_write_getf());
    }

    /* ==========================  mac address ========================= */
    nxmac_mac_addr_low_set(keyram_arg->macaddr.mac[0]);
    nxmac_mac_addr_hi_set(keyram_arg->macaddr.mac[1]);
    nxmac_mac_addr_low_mask_set(keyram_arg->macaddr.mac_mask[0]);
    nxmac_mac_addr_hi_mask_set(keyram_arg->macaddr.mac_mask[1]);
    nxmac_bss_id_low_set(keyram_arg->macaddr.bssid[0]);
    nxmac_bss_id_hi_set(keyram_arg->macaddr.bssid[1]);
    nxmac_bss_id_low_mask_set(keyram_arg->macaddr.bssid_mask[0]);
    nxmac_bss_id_hi_mask_set(keyram_arg->macaddr.bssid_mask[1]);

    /* ==========================  edca config  ========================= */
    nxmac_edca_ac_0_set(keyram_arg->edca.ac0);
    nxmac_edca_ac_1_set(keyram_arg->edca.ac1);
    nxmac_edca_ac_2_set(keyram_arg->edca.ac2);
    nxmac_edca_ac_3_set(keyram_arg->edca.ac3);
    //mm_env_max_ampdu_duration_set();

    /* ==========================  pwr mgt config  ========================= */
    nxmac_pwr_mgt_setf(keyram_arg->pwrmgt.pwrbit);

    return 0;
}


void rwnx_lpfw_init(struct mac_addr const *mac, struct mac_addr const *bssid, uint32_t addr)
{
    // MAC/MODEM Clock initialization
    crm_init();

    // system controller initialization
    sysctrl_init();

    // Initialize the MAC HW
    hal_machw_init();

    // Initialize rxl_hwdesc
    rxl_hwdesc_init();

    nxmac_ap_setf(0);
    nxmac_bss_type_setf(1);

    // Reset TSF
    nxmac_tsf_lo_set(0);
    nxmac_tsf_hi_set(0);

    // Set the MAC address of the interface to the MAC HW
    nxmac_mac_addr_low_set(mac->array[0] | (((uint32_t)mac->array[1]) << 16));
    nxmac_mac_addr_hi_set(mac->array[2]);

    // Now that we have a MAC Address we are supposed to reply
    nxmac_mac_cntrl_1_set(nxmac_mac_cntrl_1_get() & ~(NXMAC_DISABLE_ACK_RESP_BIT
                            | NXMAC_DISABLE_CTS_RESP_BIT | NXMAC_DISABLE_BA_RESP_BIT));

    // Enable reception of useful frames only
    // mm_rx_filter_umac_set(MM_RX_FILTER_ACTIVE);

    // Allow/disallow reception of all HE trigger frames
    nxmac_accept_all_trigger_frames_setf(false);

    // Enable/disable the RX buffer 2
    nxmac_disable_rx_buffer_2_setf(false);

    // Re-initialize the buffers
    rxl_hwdesc_init();

    nxmac_rx_cntrl_set(NXMAC_ACCEPT_DATA_BIT | NXMAC_ACCEPT_BROADCAST_BIT | NXMAC_ACCEPT_MULTICAST_BIT
		       | NXMAC_ACCEPT_DATA_BIT | NXMAC_ACCEPT_BROADCAST_BIT | NXMAC_ACCEPT_MULTICAST_BIT);

    nxmac_bss_id_low_set(bssid->array[0] | (((uint32_t)bssid->array[1]) << 16));
    nxmac_bss_id_hi_set(bssid->array[2]);

    keyram_restore(addr);

    #define HW_ACTIVE 3
    nxmac_next_state_setf(HW_ACTIVE);
}

/// @}
