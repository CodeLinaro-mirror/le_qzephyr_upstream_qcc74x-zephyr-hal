/*
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include "hbn_reg.h"
#include "pds_reg.h"
#include "glb_reg.h"
#include "qc7xx_irq.h"
#include "qcc74x_lp.h"
#include "lpfw_printf.h"
#undef __INLINE

#include "wl_api.h"
#include "rxl_hwdesc.h"
#include "qcc74x_lp.h"

/// Indicate if the received packet is a Data packet by reading the Frame Control Field
#define RXL_CNTRL_IS_DATA(frame_cntrl)                                                   \
           ((frame_cntrl & MAC_FCTRL_TYPE_MASK) == MAC_FCTRL_DATA_T)
/// Indicate if the received packet is a QOS Data packet by reading the Frame Control Field
#define RXL_CNTRL_IS_QOS_DATA(frame_cntrl)                                               \
           ((frame_cntrl & MAC_FCTRL_TYPESUBTYPE_MASK) == MAC_FCTRL_QOS_DATA)
/// Indicate if the received packet is a Management packet by reading the Frame Control Field
#define RXL_CNTRL_IS_MGT(frame_cntrl)                                                    \
           ((frame_cntrl & MAC_FCTRL_TYPE_MASK) == MAC_FCTRL_MGT_T)
/// Indicate if the received packet is a Control packet by reading the Frame Control Field
#define RXL_CNTRL_IS_CTRL(frame_cntrl)                                                   \
           ((frame_cntrl & MAC_FCTRL_TYPE_MASK) == MAC_FCTRL_CTRL_T)

/// Indicate if the received frame was encrypted (Protected Bit set to 1)
#define RXL_CNTRL_IS_PROTECTED(frame_cntrl)                                              \
           ((frame_cntrl & MAC_FCTRL_PROTECTEDFRAME) == MAC_FCTRL_PROTECTEDFRAME)

#define LLC_ETHERTYPE_IPX            0x8137

/// LLC/SNAP structure
struct llc_snap_short
{
    /// DSAP + SSAP fieldsr
    uint16_t dsap_ssap;
    /// LLC control + OUI byte 0
    uint16_t control_oui0;
    /// OUI bytes 1 and 2
    uint16_t oui1_2;
};

/// LLC/SNAP structure
struct llc_snap
{
    /// DSAP + SSAP fieldsr
    uint16_t dsap_ssap;
    /// LLC control + OUI byte 0
    uint16_t control_oui0;
    /// OUI bytes 1 and 2
    uint16_t oui1_2;
    /// Protocol
    uint16_t proto_id;
};

const struct llc_snap_short llc_rfc1042_hdr = {
                                                  0xAAAA, // DSAP LSAP
                                                  0x0003, // Control/Prot0
                                                  0x0000, // Prot1 and 2
                                              };

/// Bridge-Tunnel LLC/SNAP Header
const struct llc_snap_short llc_bridge_tunnel_hdr = {
                                                        0xAAAA, // DSAP LSAP
                                                        0x0003, // Control/Prot0
                                                        0xF800, // Prot1 and 2
                                                    };

/// Bit field positions for frame_info value (struct rxu_cntrl_rx_status)
enum rxu_cntrl_frame_info_pos
{
    /// Received frame was encrypted using TKIP method, so a MIC check is required
    RXU_CNTRL_MIC_CHECK_NEEDED = CO_BIT(0),
    /// Received frame has to pass the PN replay check
    RXU_CNTRL_PN_CHECK_NEEDED = CO_BIT(1),
    /// Received frame is a Mesh Beacon received from an unknown Mesh STA
    RXU_CNTRL_NEW_MESH_PEER   = CO_BIT(2),
    /// Received frame was group addressed
    RXU_CNTRL_GROUP_ADDRESSED = CO_BIT(3),
};

struct rxl_cntrl_rx_status
{
    /// Frame Control field
    uint16_t frame_cntl;
    /// Sequence control field
    uint16_t seq_cntl;
    /// qos
    uint16_t qos;
    /// Sequence Number
    uint16_t sn;
    /// Fragment Number
    uint8_t fn;
    /// Traffic Index
    uint8_t tid;
    /// MAC Header Length
    uint8_t machdr_len;
    /// Index of the sending STA, if known
    uint8_t sta_idx;
    /// Index of the receiving VIF, if known
    uint8_t vif_idx;
    /// Index of the destination STA, if known (AP mode only)
    uint8_t dst_idx;
    /// Packet number (used for replay check in TKIP, CCMP and WAPI)
    uint64_t pn;
    /// Status information field indicated by the HW
    uint32_t statinfo;
    /// Address of the host buffer
    uint32_t host_buf_addr;
    /// Pointer to the key structure that will be used for PN replay and MIC check
    struct key_info_tag *key;
    /// Destination Address and Source Address
    struct mac_addr da;
    /// Source Address
    struct mac_addr sa;
    /// Internal information about the received frame
    uint8_t frame_info;
    /// Indicate if Ethernet Header contains the length field
    bool eth_len_present;
    /// Payload offset in the frame after MAC header conversion
    uint8_t payl_offset;
};

#define LINEAR_ALLOCATOR_MAX_PTRS 12

typedef struct {
    uint8_t *buffer;
    size_t size;
    size_t offset;

    void *ptrs[LINEAR_ALLOCATOR_MAX_PTRS];
    size_t bsize[LINEAR_ALLOCATOR_MAX_PTRS];
    size_t ptr_count;
    size_t iter_index;
} linear_allocator;

static size_t align_up(size_t ptr, size_t align) {
    if (align == 0) return ptr;
    return (ptr + (align - 1)) & ~(align - 1);
}

void *linear_allocator_alloc(linear_allocator *alloc, size_t size, size_t align) {
    size_t base = (size_t)(alloc->buffer);
    size_t current = base + alloc->offset;
    size_t aligned = align_up(current, align);
    size_t rel_offset = aligned - base;
    size_t new_offset = rel_offset + size;

    if (new_offset > alloc->size || alloc->ptr_count >= LINEAR_ALLOCATOR_MAX_PTRS)
        return NULL;

    size_t idx = alloc->ptr_count;
    void *ptr = (void *)(alloc->buffer + rel_offset);
    alloc->offset = new_offset;
    alloc->ptrs[idx] = ptr;
    alloc->bsize[idx] = size;
    alloc->ptr_count++;

    return ptr;
}

struct rxl_cntrl_rx_status rxl_status;

volatile uint8_t g_wifi_int_flag = 0;

#define WL_API_RMEM_ADDR (0x20010600)
int ps_rxl_payload_transfer(struct rx_payloaddesc **curr_pd,
                                                   uint16_t upload_len, uint32_t hostbuf,
                                                   uint16_t *payl_offset, int dma_idx,
                                                   int irq_en);

extern void lpfw_wifi_addr_init(uint32_t buff_start_addr,
                                uint32_t beacon_start_addr,
                                uint32_t beacon_bssid_addr,
                                uint32_t beacon_fixed_part_addr,
                                uint32_t beacon_interval_part_addr,
                                uint32_t beacon_variable_part_addr);

void wl_drv_init(uint8_t *wl_rmem_addr)
{
    if (iot2lp_para->ap_channel == 14) {
        wl_lp_init(wl_rmem_addr, 2484);
    } else {
        wl_lp_init(wl_rmem_addr, 2412 + (iot2lp_para->ap_channel - 1) * 5);
    }
}

void lpfw_wifi_rx_start(void)
{
    /* wifi rx buff init */
    lpfw_wifi_addr_init((LPFW_WIFI_RX_BUFF + 0x18),         /* rx_buff start addr */
                        (LPFW_WIFI_RX_BUFF + 0x18 + 0xA0),  /* beacon start addr*/
                        (LPFW_WIFI_RX_BUFF + 0x18 + 0xB0),  /* beacon bssid addr */
                        (LPFW_WIFI_RX_BUFF + 0x18 + 0xB8),  /* beacon fixed part addr*/
                        (LPFW_WIFI_RX_BUFF + 0x18 + 0xC0),  /* beacon interval part addr*/
                        (LPFW_WIFI_RX_BUFF + 0x18 + 0xC4)); /* beacon variable part addr*/

    // extern void lp_set_rxl_hwdesc_env_para_addr(uint32_t addr);
    // lp_set_rxl_hwdesc_env_para_addr(LPFW_WIFI_RX_BUFF);
    *((volatile uint32_t *)0x200005C8) = LPFW_WIFI_RX_BUFF;
#if 0
    lpfw_printf_debug("wifi_rx buff 0x%08X\r\n", LPFW_WIFI_RX_BUFF);
#endif

    wl_drv_init((uint8_t *)WL_API_RMEM_ADDR);

    rwnx_lpfw_init((struct mac_addr const *)iot2lp_para->local_mac, (struct mac_addr const *)iot2lp_para->bssid, iot2lp_para->pack_env);

    return;
}

static uint32_t get_time_us(void)
{
#ifdef SHARED_FUNC_EN
    return shared_cpu_get_mtimer_counter();
#else
    return CPU_Get_MTimer_Counter();
#endif
}

void rx_data_processing(struct rxdesc *rxdesc)
{
    static uint32_t rx_count = 0;

    rx_count++;
    //lpfw_printf_debug("RX frame %d, addr:%p\r\n", rx_count, rxdesc);
}

#if 0
void dump802_11(uint8_t *packet, uint16_t length) {
    lpfw_printf_debug2("802.11 Raw Packet (%d bytes):\n", length);
    for (int i = 0; i < length; i++) {
        lpfw_printf_debug2("%02x ", packet[i]);
        if ((i + 1) % 16 == 0) lpfw_printf_debug2("\r\n");
    }
    lpfw_printf_debug2("\r\n");
}
#endif

static int ps_rxl_frame_handle(struct rxdesc* rxdesc)
{
    struct rx_dmadesc *dma_hdrdesc = rxl_dmadesc_get(rxdesc);
    struct rx_hd *rhd = &dma_hdrdesc->hd;
    struct rx_pbd *pd;
    uint8_t *frame;
    uint16_t framectrl;
    uint8_t  status = 0;

    // Ensure the packet has a length
    if (!rhd->frmlen)
        return PM_FRAME_DELETE;

    // Sanity check: frames with length not NULL have at least 1 buffer descriptor
    ASSERT_REC_VAL(rhd->first_pbd_ptr != 0, false);

    // Get value of the buffer descriptor pointer
    pd = HW2CPU(rhd->first_pbd_ptr);

    // Get the pointer to the frame data
    frame = HW2CPU(pd->datastartptr);

    // Get the frame control
    framectrl = *(uint16_t*)frame;

    //dump802_11(frame, rhd->frmlen);

    struct mac_hdr *mac_header = (struct mac_hdr *)frame;
    uint16_t seq_num = mac_header->seq >> MAC_SEQCTRL_NUM_OFT;

    if((framectrl & MAC_FCTRL_TYPE_MASK)==MAC_FCTRL_DATA_T){
        status |= PM_FRAME_UPLOAD;
        if (!(framectrl & MAC_FCTRL_MOREDATA)) {
            status |= PM_FRAME_END;
            //lpfw_printf_debug2("<ps_rxl_frame_handle> FRAME END seq=%d, type=%d \r\n", seq_num, framectrl);
        } else {
            status |= PM_FRAME_START;
            //lpfw_printf_debug2("<ps_rxl_frame_handle> FRAME MORE seq=%d, type=%d \r\n", seq_num, framectrl);
        }
    } else {
        status |= PM_FRAME_DELETE;
    }

    return status;
}

static uint16_t co_bswap16(uint16_t val16)
{
    return ((val16<<8)&0xFF00) | ((val16>>8)&0xFF);
}

static uint16_t co_htons(uint16_t hostshort)
{
    return co_bswap16(hostshort);
}

static bool rxl_cntrl_protected_handle(uint8_t *frame, uint32_t statinfo)
{
    uint32_t frame_addr = CPU2HW(frame);
    uint16_t *iv = HW2CPU(frame_addr + rxl_status.machdr_len);
    //bool ga = (statinfo & RX_HD_GA_FRAME) != 0;
    struct key_info_tag *key;
    bool upload = true;

    switch (statinfo & RX_HD_DECRSTATUS)
    {
        case RX_HD_DECR_CCMP128_SUCCESS:
        #if  NX_RX_RING
        case RX_HD_DECR_CCMP256_SUCCESS:
        case RX_HD_DECR_GCMP128_SUCCESS:
        case RX_HD_DECR_GCMP256_SUCCESS:
        #endif
#if 0
            if (ga)
            {
                struct vif_info_tag *vif = &vif_info_tab[rxu_cntrl_env.rx_status.vif_idx];
                key = &vif->key_info[iv[1] >> 14];
            }
            else
            {
                struct sta_info_tag *sta = &sta_info_tab[rxu_cntrl_env.rx_status.sta_idx];
                key = &sta->sta_sec_info.key_info;
            }
            rxu_cntrl_env.rx_status.key = key;
            #if RW_WAPI_EN && !NX_RX_RING
            // keep this for old MACHW (<v20) that don't distinguish CCMP and WAPI success
            if (key->cipher == MAC_CIPHER_WPI_SMS4) {
                rxu_cntrl_env.rx_status.machdr_len += WPI_HDR_LEN;
                rxu_cntrl_env.rx_status.pn = (((uint64_t)iv[4]) << 48) |
                    (((uint64_t)iv[3]) << 32) | (((uint64_t)iv[2]) << 16) | iv[1];
                rxu_cntrl_env.rx_status.frame_info |= RXU_CNTRL_PN_CHECK_NEEDED;
            } else
            #endif
#endif
            {
                // all CCMP and GCMP variant use the same header
                rxl_status.machdr_len += CCMP_HDR_LEN;
                rxl_status.pn = (((uint64_t)iv[3]) << 32) |
                    (((uint64_t)iv[2]) << 16) | iv[0];
                rxl_status.frame_info |= RXU_CNTRL_PN_CHECK_NEEDED;
            }
            break;
#if 0
        #if RW_WAPI_EN && NX_RX_RING
        case RX_HD_DECR_WAPI_SUCCESS:
            if (ga)
            {
                struct vif_info_tag *vif = &vif_info_tab[rxu_cntrl_env.rx_status.vif_idx];
                key = &vif->key_info[iv[1] >> 14];
            }
            else
            {
                struct sta_info_tag *sta = &sta_info_tab[rxu_cntrl_env.rx_status.sta_idx];
                key = &sta->sta_sec_info.key_info;
            }
            rxu_cntrl_env.rx_status.key = key;
            rxu_cntrl_env.rx_status.machdr_len += WPI_HDR_LEN;
            rxu_cntrl_env.rx_status.pn = (((uint64_t)iv[4]) << 48) |
                (((uint64_t)iv[3]) << 32) | (((uint64_t)iv[2]) << 16) | iv[1];
            rxu_cntrl_env.rx_status.frame_info |= RXU_CNTRL_PN_CHECK_NEEDED;
            break;
        #endif // RW_WAPI_EN && NX_RX_RING
#endif
        case RX_HD_DECR_WEP_SUCCESS:
            rxl_status.machdr_len += WEP_HDR_LEN;
            break;
        case RX_HD_DECR_TKIP_SUCCESS:
            rxl_status.machdr_len += TKIP_HDR_LEN;
            rxl_status.pn = (((uint64_t)iv[3]) << 32) |
                                         (((uint64_t)iv[2]) << 16) |
                                          ((iv[0] & 0xFF) << 8) | (iv[1] & 0xFF);
            rxl_status.frame_info |= RXU_CNTRL_PN_CHECK_NEEDED |
                                                  RXU_CNTRL_MIC_CHECK_NEEDED;
#if 0
            if (ga)
            {
                struct vif_info_tag *vif = &vif_info_tab[rxu_cntrl_env.rx_status.vif_idx];
                key = &vif->key_info[iv[1] >> 14];
            }
            else
            {
                struct sta_info_tag *sta = &sta_info_tab[rxu_cntrl_env.rx_status.sta_idx];
                key = &sta->sta_sec_info.key_info;
            }
            rxu_cntrl_env.rx_status.key = key;
#endif
            break;
        default:
            upload = false;
            break;
    }

    return upload;
}

static void ps_rxl_cntrl_mac2eth_update(struct rxdesc *rxdesc)
{

    struct rx_dmadesc *dma_hdrdesc = rxl_dmadesc_get(rxdesc);
    // First payload descriptor -> Buffer will contain the MAC Header (and IV, EIV if present)
    struct rx_payloaddesc *first_pbd
            = (struct rx_payloaddesc *)HW2CPU(dma_hdrdesc->hd.first_pbd_ptr);
    // Map a MAC Header structure on the frame
    struct mac_hdr *machdr = (struct mac_hdr *)HW2CPU(first_pbd->pbd.datastartptr);
    // LLC/SNAP part of the PDU
    struct llc_snap *llc_snap;
    // Ethernet Header
    struct mac_eth_hdr *eth_hdr;
    // Compute MAC Header Length (will IV length + EIV length if present)
    uint16_t machdr_len = rxl_status.machdr_len;
    uint8_t payl_offset = machdr_len - sizeof_b(struct mac_eth_hdr);

    do
    {
        {
            // Pointer to the payload - Skip MAC Header
            llc_snap = (struct llc_snap *)((uint16_t *)machdr + (machdr_len >> 1));

            /*
             * Ethernet encapsulated packet structure
             ******************************************
             *  MAC HEADER  |  LLC  |  SNAP  |  DATA  |
             ******************************************
             * or
             *************************
             *  MAC HEADER  |  DATA  |
             *************************
             * The type of structure depends on the LLC fields.
             */
            if ((!memcmp(llc_snap, &llc_rfc1042_hdr, sizeof(llc_rfc1042_hdr))
                 //&& (llc_snap->ether_type != LLC_ETHERTYPE_AARP) - Appletalk depracated ?
                 && (llc_snap->proto_id != LLC_ETHERTYPE_IPX))
                || (!memcmp(llc_snap, &llc_bridge_tunnel_hdr, sizeof(llc_bridge_tunnel_hdr))))
            {
                // Case 1 -> Packet becomes
                /********************************************
                 *  DA  |  SA  |  SNAP->ETHERTYPE  |  DATA  |
                 ********************************************/
                /*
                 * Ethernet Header will start 6 half-words (MAC Address length is 6 bytes) before Ethertag
                 * type
                 */
                eth_hdr = (struct mac_eth_hdr *)((uint16_t *)&(llc_snap->proto_id) - 6);

                // Remove length of LLC/SNAP
                payl_offset += 8;

                rxl_status.eth_len_present = false;
            }
            else
            {
                // Case 2 -> Packet becomes
                /********************************
                 *  DA  |  SA  |  LEN  |  DATA  |
                 ********************************/
                /*
                 * Ethernet Header will start 7 half-words (MAC Address length is 6 bytes and Length
                 * field is 2 bytes) before LLC Snap
                 */
                eth_hdr = (struct mac_eth_hdr *)((uint16_t *)llc_snap - 7);

                // Set length (Initial length - MAC Header Length)
                eth_hdr->len = co_htons(dma_hdrdesc->hd.frmlen - machdr_len);

                rxl_status.eth_len_present = true;
            }
        }

        // Set DA and SA in the Ethernet Header
        MAC_ADDR_CPY(&eth_hdr->da, &rxl_status.da);
        MAC_ADDR_CPY(&eth_hdr->sa, &rxl_status.sa);

    } while(0);

    // Update Frame Length
    dma_hdrdesc->hd.frmlen -= payl_offset;
    rxl_status.payl_offset = payl_offset;
}

uint8_t rxu_cntrl_machdr_len_get(uint16_t frame_cntl)
{
    // MAC Header length
    uint8_t mac_hdr_len = MAC_SHORT_MAC_HDR_LEN;

    // Check if Address 4 field is present (FDS and TDS set to 1)
    if ((frame_cntl & (MAC_FCTRL_TODS | MAC_FCTRL_FROMDS))
                                    == (MAC_FCTRL_TODS | MAC_FCTRL_FROMDS))
    {
        mac_hdr_len += (MAC_LONG_MAC_HDR_LEN - MAC_SHORT_MAC_HDR_LEN);
    }

    // Check if QoS Control Field is present
    if (RXL_CNTRL_IS_QOS_DATA(frame_cntl))
    {
        mac_hdr_len += (MAC_LONG_QOS_MAC_HDR_LEN - MAC_LONG_MAC_HDR_LEN);
        //lpfw_printf_debug2("QOS data, mac len:%d\r\n", mac_hdr_len);
    }

    // Check if HT Control Field is present (Order bit set to 1)
    if (frame_cntl & MAC_FCTRL_ORDER)
    {
        mac_hdr_len += (MAC_LONG_QOS_HTC_MAC_HDR_LEN - MAC_LONG_QOS_MAC_HDR_LEN);
    }

    //lpfw_printf_debug2("mac len:%d\r\n", mac_hdr_len);
    return (mac_hdr_len);
}

static void rxu_cntrl_get_da_sa(struct mac_hdr_long *machdr_ptr)
{
    // Get DA
    if (machdr_ptr->fctl & MAC_FCTRL_TODS)
    {
        MAC_ADDR_CPY(&rxl_status.da, &machdr_ptr->addr3);
    }
    else
    {
        MAC_ADDR_CPY(&rxl_status.da, &machdr_ptr->addr1);
    }

    // Get SA
    if (machdr_ptr->fctl & MAC_FCTRL_FROMDS)
    {
        if (machdr_ptr->fctl & MAC_FCTRL_TODS)
        {
            MAC_ADDR_CPY(&rxl_status.sa, &machdr_ptr->addr4);
        }
        else
        {
            MAC_ADDR_CPY(&rxl_status.sa, &machdr_ptr->addr3);
        }
    }
    else
    {
        MAC_ADDR_CPY(&rxl_status.sa, &machdr_ptr->addr2);
    }
}

static void rxl_cntrl_machdr_read(uint8_t *frame)
{
    // Map a MAC Header structure on the frame
    struct mac_hdr *machdr = (struct mac_hdr *)frame;

    rxl_status.frame_cntl = machdr->fctl;
    rxl_status.seq_cntl = machdr->seq;
    rxl_status.frame_info = 0;

    rxl_status.sn = machdr->seq >> MAC_SEQCTRL_NUM_OFT;
    rxl_status.fn = machdr->seq & MAC_SEQCTRL_FRAG_MSK;

    if ((machdr->fctl & MAC_FCTRL_QOS_DATA) == MAC_FCTRL_QOS_DATA)
    {
        uint16_t qos;

        // Check if Address4 field is present
        if ((machdr->fctl & MAC_FCTRL_TODS_FROMDS) == MAC_FCTRL_TODS_FROMDS)
            qos = ((struct mac_hdr_long_qos *)machdr)->qos;
        else
            qos = ((struct mac_hdr_qos *)machdr)->qos;

        // Extract TID from QoS Control Field
        rxl_status.tid = (qos & MAC_QOSCTRL_UP_MSK);
        rxl_status.qos = qos;
    }
    else
    {
        rxl_status.tid = 0;
        rxl_status.qos = 0;
    }

    rxl_status.machdr_len = rxu_cntrl_machdr_len_get(machdr->fctl);

    if (MAC_ADDR_GROUP(&machdr->addr1))
        rxl_status.frame_info |= RXU_CNTRL_GROUP_ADDRESSED;

    // Extract Destination Address and Source Address
    rxu_cntrl_get_da_sa((struct mac_hdr_long *)machdr);
}

int ps_rxl_mpdu_transfer(struct rxdesc *rxdesc, linear_allocator *mempool)
{
    uint16_t payl_offset = rxl_status.payl_offset;

    uint32_t hostbuf;
    struct rx_dmadesc *dma_hdrdesc = rxl_dmadesc_get(rxdesc);
    uint16_t mpdu_len;
    struct rx_payloaddesc *pd;
    uint8_t ret = 0;
    void *alloc_buf;

    // Cache the pointers for faster access
    pd = (struct rx_payloaddesc *)HW2CPU(dma_hdrdesc->hd.first_pbd_ptr);

    alloc_buf = linear_allocator_alloc(mempool, dma_hdrdesc->hd.frmlen, 4);
    lpfw_printf_debug("alloc frame p:0x%x  len:%d\r\n", alloc_buf, dma_hdrdesc->hd.frmlen);

    if (alloc_buf) {
        hostbuf = (uint32_t)(alloc_buf);
        lpfw_printf_debug("<ps_rxl_mpdu_transfer> hostbuf:%p size:%d\r\n", hostbuf, dma_hdrdesc->hd.frmlen);
    } else {
        lpfw_printf_debug("!!!! No memory.\r\n");
        return PM_FRAME_DELETE;
    }

    // Get the MPDU body length
    mpdu_len = dma_hdrdesc->hd.frmlen;

    // Sanity check - The frame length cannot be NULL...
    ASSERT_REC(mpdu_len != 0);
    // as well as not greater than the max allowed length
    ASSERT_REC(mpdu_len <= RWNX_MAX_AMSDU_RX);

    // If the MPDU is not a A-MSDU, or if the A-MSDU deaggregator is not compiled,
    // then we will upload the complete packet in a single host buffer
    ret = ps_rxl_payload_transfer(&pd, mpdu_len, hostbuf, &payl_offset, 0, 0);

    return PM_FRAME_UPLOAD;
}

int ps_rxu_cntrl_frame_handle(struct rxdesc* rxdesc, linear_allocator *mempool)
{
    struct rx_dmadesc *dma_hdrdesc = rxl_dmadesc_get(rxdesc);
    struct rx_hd *rhd = &dma_hdrdesc->hd;
    struct rx_pbd *pd = HW2CPU(rhd->first_pbd_ptr);
    uint8_t *frame = HW2CPU(pd->datastartptr);
    bool upload = false;
    uint16_t frame_cntl;
    uint32_t statinfo = rhd->statinfo;
    uint16_t key_idx_hw;
    bool qos;
    uint16_t frame_subtype;
    static int conn_flag;
    uint8_t ret = 0;

    if (!(statinfo & RX_HD_SUCCESS)) {
        return PM_FRAME_DELETE;
    }

    frame_cntl = *(uint16_t*)frame;
    rxl_cntrl_machdr_read(frame);
    rxl_cntrl_protected_handle(frame, statinfo);

    if ((frame_cntl & MAC_FCTRL_TYPE_MASK) == MAC_FCTRL_DATA_T)
    {
        ps_rxl_cntrl_mac2eth_update(rxdesc);
        ret = ps_rxl_mpdu_transfer(rxdesc, mempool);
    }

    return ret;
}

int lpfw_wifi_rx_broadcast(uint32_t buf_addr)
{
    uint32_t rx_start_ms;
    uint8_t status;
    uint8_t ret = 0;
    int has_pack = 0;
    linear_allocator *pm_mempool;

    pm_mempool = (linear_allocator *)buf_addr;

    /* get time  */
    rx_start_ms = get_time_us();

    /* enable wifi IRQ */
    qc7xx_irq_enable(WIFI_IRQn);
    /*  */

    while (1) {
        if (get_time_us() - rx_start_ms > 30 * 1000) {
            qc7xx_irq_disable(WIFI_IRQn);
            break;
        }

	void lp_wdt_feed(void);
        lp_wdt_feed();

        struct rxdesc *rxdesc = rxl_rxdesc_get();

        // clear the interrupt
        nxmac_tx_rx_int_ack_clear(NXMAC_RX_BUFFER_1_TRIGGER_BIT);

        // Check if we still have ready descriptors
        if (rxdesc == NULL) {
            // No more RX descriptors ready for processing, exit the loop
            // lpfw_printf_debug("No more RX descriptors ready for processing.\r\n");
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            continue;
        }

        // Upload is possible, indicate that the RX descriptor will be processed
        rxl_rxdesc_ready_for_processing(rxdesc);

        rx_data_processing(rxdesc);

        status = ps_rxl_frame_handle(rxdesc);

        if (status & PM_FRAME_UPLOAD) {
            ret = ps_rxu_cntrl_frame_handle(rxdesc, pm_mempool);
            has_pack |= ret;
        }

        rxl_mpdu_free(rxdesc);

        if ((ret & PM_FRAME_DELETE) || (status & PM_FRAME_END)) {
            break;
        }

        nxmac_enable_rx_buffer_1_trigger_setf(1);
    }

    if (has_pack & PM_FRAME_UPLOAD) {
        has_pack = 1;
    }

    lpfw_printf_debug("has_pack :%d\r\n", has_pack);

    return has_pack;
}
