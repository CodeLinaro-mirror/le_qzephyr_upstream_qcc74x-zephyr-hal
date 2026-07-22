#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/net/ethernet.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_pkt.h>

#include "qc7xx_wifi_drv_priv.h"

#include "eloop.h"
#include "eloop_rtos.h"
#include "fhost.h"
#include "mac_frame.h"
#include "net_al.h"
#include "os.h"
#include "rtos_al.h"

#if defined(CONFIG_BOARD_QCC74X_DEVKITP) || defined(CONFIG_BOARD_QCC743_DEVKITP) || \
	defined(CFG_QCC74X_WIFI_CONSTRAINED_RAM)
#define TX_BUF_CNT (11)
#else
#define TX_BUF_CNT (24)
#endif

#define TX_BUF_ALLOC_TIMEOUT (0)

static rtos_semaphore dnld_txbuf_semaphore;

void cmd_net_printf()
{
	struct k_mem_slab *rx, *tx;
	struct net_buf_pool *rx_data, *tx_data;

	net_pkt_get_info(&rx, &tx, &rx_data, &tx_data);

    printf("pkt:%u/%u, tx:%u/%u\r\n",
	       rx->info.num_blocks, k_mem_slab_num_free_get(rx),
	       tx->info.num_blocks, k_mem_slab_num_free_get(tx));

	printf("Fragment length %d bytes\n", CONFIG_NET_BUF_DATA_SIZE);
	printf("Network buffer pools:\n");

	printf("Address\t\tTotal\tAvail\tMaxUsed\tName\n");
	printf("%p\t%d\t%u\t-\tRX\n",
	       rx, rx->info.num_blocks, k_mem_slab_num_free_get(rx));

	printf("%p\t%d\t%u\t-\tTX\n",
	       tx, tx->info.num_blocks, k_mem_slab_num_free_get(tx));
	printf("%p\t%d\t%ld\t%d\tRX DATA (%s)\n", rx_data, rx_data->buf_count,
	   atomic_get(&rx_data->avail_count), rx_data->max_used, rx_data->name);

	printf("%p\t%d\t%ld\t%d\tTX DATA (%s)\n", tx_data, tx_data->buf_count,
	   atomic_get(&tx_data->avail_count), tx_data->max_used, tx_data->name);

}

int net_pkt_avil_cnt(void)
{
	struct k_mem_slab *rx, *tx;
	struct net_buf_pool *rx_data, *tx_data;
	net_pkt_get_info(&rx, &tx, &rx_data, &tx_data);
    return k_mem_slab_num_free_get(rx);
}

static int net_pkt_rx_avil(void)
{
	struct k_mem_slab *rx, *tx;
	struct net_buf_pool *rx_data, *tx_data;
	net_pkt_get_info(&rx, &tx, &rx_data, &tx_data);

    //if ((k_mem_slab_num_free_get(rx) > 2) && (k_mem_slab_num_free_get(tx) > 2)) {
    if (k_mem_slab_num_free_get(rx) > 2) {
        return 1;
    }

    return 0;
}

volatile uint32_t g_net_recv_data = 0;

#if defined(CONFIG_NET_STATISTICS_WIFI)
static void qc7xx_wifi_update_rx_stats(struct net_stats_wifi *stats,
                                       struct net_eth_hdr *hdr, uint16_t len)
{
    stats->bytes.received += len;
    stats->pkts.rx++;

    if (net_eth_is_addr_broadcast(&hdr->dst)) {
        stats->broadcast.rx++;
    } else if (net_eth_is_addr_multicast(&hdr->dst)) {
        stats->multicast.rx++;
    } else {
        stats->unicast.rx++;
    }
}
#endif

static int qc7xx_wifi_recv(void *payload, uint16_t length, uint8_t offset, uint8_t skip_after_eth_hdr, net_buf_free_fn free_fn)
{
    struct net_pkt *pkt;
    uint8_t *data = (uint8_t *)payload + offset + skip_after_eth_hdr;
    uint16_t pkt_len;
    ARG_UNUSED(free_fn);

    if (qc7xx_wifi_iface == NULL) {
        printf("net interface unavailable\r\n");
        return -EIO;
    }

    if (skip_after_eth_hdr >= length || (length - skip_after_eth_hdr) < sizeof(struct net_eth_hdr)) {
        printf("Invalid length/skip\r\n");
        goto out;
    }
    pkt_len = length - skip_after_eth_hdr;


#if 1
    if (1 != net_pkt_rx_avil()) {
        // workaround
        goto out;
    }

    pkt = net_pkt_rx_alloc_with_buffer(qc7xx_wifi_iface, pkt_len, AF_UNSPEC, 0, K_MSEC(100));
    if (!pkt) {
        printf("Failed alloc pkt buf, flag:%d\r\n", g_net_recv_data);
        goto out;
    }
#else
    pkt = net_pkt_rx_alloc_on_iface(qc7xx_wifi_iface, K_MSEC(100));
    if (!pkt) {
        printf("Failed to alloc pkt, flag:%d\r\n", g_net_recv_data);
        k_sleep(K_MSEC(2));
        goto out;
    }
	net_pkt_set_family(pkt, AF_UNSPEC);
	if (net_pkt_alloc_buffer(pkt, pkt_len, 0, K_MSEC(100))) {
        printf("Failed to alloc buffer, flag:%d\r\n", g_net_recv_data);
        k_sleep(K_MSEC(1));
        goto pkt_unref;
    }
#endif

    if (net_pkt_write(pkt, data, pkt_len) < 0) {
        printf("Failed to write to net buf\r\n");
        goto pkt_unref;
    }

    if (net_recv_data(qc7xx_wifi_iface, pkt) < 0) {
        printf("Failed to push received data\r\n");
        goto pkt_unref;
    }
    g_net_recv_data++;

#if defined(CONFIG_NET_STATISTICS_WIFI)
    qc7xx_wifi_update_rx_stats(&qc7xx_data.stats, (struct net_eth_hdr *)data, pkt_len);
#endif

    return 0;

pkt_unref:
    net_pkt_unref(pkt);

out:
#if defined(CONFIG_NET_STATISTICS_WIFI)
    qc7xx_data.stats.errors.rx++;
#endif

    return -EIO;
}

#if defined(CONFIG_QC7XX_WIFI_AP_STA_MODE)
static int qc7xx_wifi_ap_recv(void *payload, uint16_t length, uint8_t offset, uint8_t skip_after_eth_hdr, net_buf_free_fn free_fn)
{
    struct net_pkt *pkt;
    uint8_t *data = (uint8_t *)payload + offset + skip_after_eth_hdr;
    uint16_t pkt_len;
    ARG_UNUSED(free_fn);

    if (qc7xx_wifi_iface_ap == NULL) {
        printf("net interface unavailable\r\n");
        return -EIO;
    }

    if (skip_after_eth_hdr >= length || (length - skip_after_eth_hdr) < sizeof(struct net_eth_hdr)) {
        printf("Invalid length/skip\r\n");
        goto out;
    }
    pkt_len = length - skip_after_eth_hdr;

    pkt = net_pkt_rx_alloc_with_buffer(qc7xx_wifi_iface_ap, pkt_len, AF_UNSPEC, 0, K_MSEC(100));
    if (!pkt)
    {
        k_sleep(K_MSEC(1));
        printf("Failed to alloc net buf\r\n");
        goto out;
    }

    if (net_pkt_write(pkt, data, pkt_len) < 0) {
        printf("Failed to write to net buf\r\n");
        goto pkt_unref;
    }

    if (net_recv_data(qc7xx_wifi_iface_ap, pkt) < 0) {
        printf("Failed to push received data\r\n");
        goto pkt_unref;
    }

#if defined(CONFIG_NET_STATISTICS_WIFI)
    qc7xx_wifi_update_rx_stats(&qc7xx_ap_sta_data.stats, (struct net_eth_hdr *)data, pkt_len);
#endif

    return 0;

pkt_unref:
    net_pkt_unref(pkt);

out:
#if defined(CONFIG_NET_STATISTICS_WIFI)
    qc7xx_ap_sta_data.stats.errors.rx++;
#endif

    return -EIO;
}
#endif

/*
 * replace net_al.c
 */

char __attribute__((section("SHAREDRAM"))) __attribute__((aligned(4))) fhost_tx_buf[TX_BUF_CNT][400 + 1600];
volatile int fhost_tx_buf_length[TX_BUF_CNT];

extern int net_eth_receive(net_al_if_t *netif, void *addr, uint16_t len);
extern void fhost_rx_buf_push(void *net_buf);
extern int fhost_tx_req_do(net_al_if_t net_if, net_al_tx_t net_buf, int type,
    void *cfm_cb, void *cfm_cb_arg, void* buf_rx, int no_cck);
extern int fhost_tx_start(net_al_if_t net_if, net_al_tx_t net_buf,
        cb_fhost_tx cfm_cb, void *cfm_cb_arg);

void *net_al_tx_buf_get_header(void *buf)
{
    uint32_t val = (uint32_t)buf;

    for (int i = 0; i < TX_BUF_CNT; i++) {
        if (val - (uint32_t)(fhost_tx_buf[i]) == 400) {
            return fhost_tx_buf[i];
        }
    }

    return NULL;
}

void net_al_tx_buf_set_len(void *buf, int length)
{
    uint32_t val = (uint32_t)buf;

    for (int i = 0; i < TX_BUF_CNT; i++) {
        if(val == (uint32_t)(fhost_tx_buf[i]) || val - (uint32_t)(fhost_tx_buf[i]) == 400) {
            fhost_tx_buf_length[i] = length;
        }
    }
}

int net_al_tx_buf_get_len(void *buf)
{
    uint32_t val = (uint32_t)buf;

    for (int i = 0; i < TX_BUF_CNT; i++) {
        if(val == (uint32_t)(fhost_tx_buf[i]) || val - (uint32_t)(fhost_tx_buf[i]) == 400) {
            return fhost_tx_buf_length[i];
        }
    }

    return -1;
}


void net_al_tx_buf_init()
{
    if (rtos_semaphore_create(&dnld_txbuf_semaphore, TX_BUF_CNT, 0))
    {
        while(1) {
        printf("assert. =============\r\n");
        }
        //assert(0);
    }
}

// timeout:
//        -1 : forever,
//         0 : no block
//      other: other ms(When using multiple threads,
//                      the timing may be inaccurate,
//                      which could potentially
//                      result in return 0.)
void *net_al_tx_buf_alloc(int32_t timeout)
{
#if 1
    for (int i = 0; i < TX_BUF_CNT; i++) {
        if(fhost_tx_buf_length[i] == 0) {
            fhost_tx_buf_length[i] = 1;
            return &fhost_tx_buf[i][400];
        }
    }
#else
    volatile int wait_flag = 0;
    uint64_t start, cur;

    start = qc7xx_mtimer_get_time_us();
    while (1) {
        for (int i = 0; i < TX_BUF_CNT; i++) {
            if(fhost_tx_buf_length[i] == 0) {
                fhost_tx_buf_length[i] = 1;
                return &fhost_tx_buf[i][400];
            }
        }

        if (wait_flag) {
            return 0;
        }

        if (timeout > 0) {
            rtos_semaphore_wait(dnld_txbuf_semaphore, timeout);
            cur = qc7xx_mtimer_get_time_us();
            if ((cur - start) > ((timeout - 1) * 1000)) {
                wait_flag = 1;
            } else {
                timeout -= (cur - start)/1000;
                if (timeout <= 0) {
                    printf("it's a error!!!\r\n");
                }
            }
        } if (timeout == 0) {
            return 0;
        } else {
            rtos_semaphore_wait(dnld_txbuf_semaphore, -1);
        }
    }
#endif
    return 0;
}

void net_al_tx_buf_free(void *buf)
{
    for (int i = 0; i < TX_BUF_CNT; i++) {
        if(buf == &fhost_tx_buf[i][400]) {
            fhost_tx_buf_length[i] = 0;
        }
    }
    rtos_semaphore_signal(dnld_txbuf_semaphore, false);
}

void net_al_tx_buf_get(uint32_t *tot, uint32_t *used)
{
    uint32_t tmp_used = 0;

    for (int i = 0; i < TX_BUF_CNT; i++) {
        if(fhost_tx_buf_length[i] != 0) {
            tmp_used++;
        }
    }
    if (tot) {
        *tot = TX_BUF_CNT;
    }
    if (used) {
        *used = tmp_used;
    }
}

net_al_tx_t net_buf_tx_alloc_fill(const uint8_t *frame, uint32_t length)
{
    uint8_t *pkg;

    pkg = net_al_tx_buf_alloc(TX_BUF_ALLOC_TIMEOUT);
    if (!pkg) {
        return NULL;
    }

    memcpy(pkg, frame, length);
    net_al_tx_buf_set_len(pkg, length);

    return (net_al_tx_t)pkg;
}

/* init, tx/rx */
void net_al_input(net_al_rx_t net_buf, void *payload,
                  net_al_if_t net_if,
                  uint16_t length,
                  uint8_t offset,
                  uint8_t skip_after_eth_hdr,
          net_buf_free_fn free_fn)
{
    if (skip_after_eth_hdr != 0) {
        memcpy((char*)payload + skip_after_eth_hdr, payload, sizeof(struct mac_eth_hdr));
    }

    /* Drop my broadcast message forwarded by AP. */
    uint8_t from_us = 1;
    uint8_t *mac_eth_hdr = (uint8_t *)payload + offset;
    const uint8_t *src_addr = net_if_get_mac_addr(net_if);
    for (int i = 0; i < 6; i++) {
        if (mac_eth_hdr[6 + i] != src_addr[i]) {
            from_us = 0;
            break;
        }
    }

    if (from_us) {
        fhost_rx_buf_push(net_buf);
        return;
    }

    struct net_eth_hdr *eth_hdr = payload;
    if (htons(eth_hdr->type) != NET_ETH_PTYPE_EAPOL) {
        if ((uint32_t)net_if == (uint32_t)qc7xx_wifi_iface) {
            qc7xx_wifi_recv(payload, length, offset, skip_after_eth_hdr, free_fn);
#if defined(CONFIG_QC7XX_WIFI_AP_STA_MODE)
        } else if ((uint32_t)net_if == (uint32_t)qc7xx_wifi_iface_ap) {
            qc7xx_wifi_ap_recv(payload, length, offset, skip_after_eth_hdr, free_fn);
#endif
        } else {
            printf("net_if not match \n");
        }
    } else {
        net_eth_receive(net_if, (uint8_t *)payload + offset + skip_after_eth_hdr, length - skip_after_eth_hdr);
    }

    free_fn(net_buf);
}

void *net_buf_tx_info(net_al_tx_t *net_buf, uint16_t *tot_len, int *seg_cnt,
                      uint32_t seg_addr[], uint16_t seg_len[], uint32_t *headroom_len)
{

    char *buf;
    int length;
    void *headroom;

    buf = (char *)net_buf;
    length = net_al_tx_buf_get_len(buf);
    headroom = net_al_tx_buf_get_header(buf);

    *headroom_len = 400;

    *tot_len = length;

    seg_addr[0] = (uint32_t)buf;
    seg_len[0] = *tot_len;
    *seg_cnt = 1;

    return headroom;
}

void net_buf_tx_free(net_al_tx_t net_buf)
{
    void *buf = (void *)net_buf;
    // Free the buffer
    net_al_tx_buf_free(buf);
}

/*
void net_buf_tx_cat(net_al_tx_t net_buf1, net_al_tx_t net_buf2)
{
    inet_buf_tx_t *buf1 = (inet_buf_tx_t *)net_buf1;
    inet_buf_tx_t *buf2 = (inet_buf_tx_t *)net_buf2;
    int len1 = net_al_tx_buf_get_len(buf1);
    int len2 = net_al_tx_buf_get_len(buf2);
    if(len1 + len2 < 1500) {
    memcpy(buf1+len1, buf2, len2);
    } else {
    printf("canot concatenate buf1 and buf2");
    while(1);
    }
}
*/

/*
void net_buf_rx_free(net_al_rx_t net_buf)
{
    inet_buf_rx_t *buf = (inet_buf_rx_t *)net_buf;
    // first, free from ip stack
    // second, free from fhost rx buffer
}
*/

int net_al_tx_req(struct net_al_tx_req req)
{
    net_al_tx_t *ibuf = req.net_buf;
    net_al_if_t net_if = req.net_if;
    int type = req.type;
    void *cfm_cb = req.cfm_cb;
    void *cfm_cb_arg = req.cfm_cb_arg;
    void *buf_rx = req.buf_rx;
    int no_cck = req.no_cck;

    return fhost_tx_req_do(net_if, ibuf, type, cfm_cb, cfm_cb_arg, buf_rx, no_cck);
}

void net_al_rx_resend(bool rx_buf_forward,
                    void *buf,
                    void *payload,
                    int length,
                    int offset,
                    int mac_hdr_len)
{
    uint8_t *pkg;

    pkg = net_al_tx_buf_alloc(TX_BUF_ALLOC_TIMEOUT);
    if (!pkg)
    {
        if (!rx_buf_forward)
        {
            void fhost_rx_buf_push(void *);
            fhost_rx_buf_push(buf);
        }
    }
    else
    {
        if (offset != 0) {
            printf("=============== offset:%d\r\n", offset);
        }
        memcpy(pkg,  (uint8_t*)payload + offset, length);
        net_al_tx_buf_set_len(pkg, length);

        void fhost_rx_do_resend_req(void *buf, net_al_tx_t net_buf_tx, bool forward);
        fhost_rx_do_resend_req(buf, (net_al_tx_t)pkg, rx_buf_forward);
    }
}

/* l2 adapter */

static rtos_semaphore l2_semaphore;
static rtos_mutex     l2_mutex;


int net_init(void)
{
    //int i;

    if (rtos_semaphore_create(&l2_semaphore, 1, 0))
    {
        //assert(0);
        return -1;
    }

    if (rtos_mutex_create(&l2_mutex))
    {
        //assert(0);
        return -1;
    }

    // Initialize the TCP/IP stack
    // tcpip_init(net_init_done, NULL);
    fhost_task_ready(IP_TASK);

    return 0;
}


static void net_l2_send_cfm(uint32_t frame_id, bool acknowledged, void *arg)
{
    if (arg)
        *((bool *)arg) = acknowledged;
    rtos_semaphore_signal(l2_semaphore, false);
}

int net_l2_send_do(net_al_if_t net_if, const uint8_t *data, int data_len, uint16_t ethertype,
                   const uint8_t *dst_addr, bool *ack)
{
    int res = 0;
    uint8_t *pkg;

    pkg = net_al_tx_buf_alloc(TX_BUF_ALLOC_TIMEOUT);
    if (!pkg) {
        return -1;
    }
    memcpy(pkg + 14, data, data_len);
    net_al_tx_buf_set_len(pkg, data_len + 14);

    if (dst_addr) {
        struct net_eth_hdr *eth_hdr = (struct net_eth_hdr *)pkg;
        memcpy(eth_hdr->dst.addr, dst_addr, WIFI_MAC_ADDR_LEN);
        memcpy(eth_hdr->src.addr, net_if_get_mac_addr(net_if), WIFI_MAC_ADDR_LEN);
        eth_hdr->type = htons(ethertype);
    }

    // Ensure no other thread will program a L2 transmission while this one is waiting
    // for its confirmation
    rtos_mutex_lock(l2_mutex);

    // In order to implement this function as blocking until the completion of the frame
    // transmission, directly call fhost_tx_start with a confirmation callback.
    res = fhost_tx_start((net_al_if_t)net_if, pkg, net_l2_send_cfm, ack);

    // Wait for the transmission completion
    rtos_semaphore_wait(l2_semaphore, -1);

    // Now new L2 transmissions are possible
    rtos_mutex_unlock(l2_mutex);
    return res;
}

int fh_net_l2_send(net_al_if_t net_if, const uint8_t *data, int data_len, uint16_t ethertype,
                const uint8_t *dst_addr, bool *ack)
{
    size_t tries = 7 + 1;

    while (tries-- > 0) {
        bool ack_;

        if (net_l2_send_do(net_if, data, data_len, ethertype, dst_addr, &ack_)) {
            return -1;
        }
        if (ack_) {
            *ack = true;
            return 0;
        }
    }

    *ack = false;
    return 0;
}

int net_l2_socket_create(net_al_if_t net_if, uint16_t ethertype)
{
    return 0;
}

int net_l2_socket_delete(int sock)
{
    return 0;
}


int net_eth_receive(net_al_if_t *netif, void *addr, uint16_t len)
{
    //struct net_eth_hdr * eth_hdr = (struct net_eth_hdr *)addr;
    //uint16_t ethertype = ntohs(eth_hdr->type);

    //if(ethertype != NET_ETH_PTYPE_EAPOL) {
    //    return -1;
    //}

#if 0
    printf("\x1b[32m %s \x1b[0m eloop_event_commit len:[%d]: \r\n", __FUNCTION__, len);
    for (int i = 0; i < len; i++) {
        uint8_t *tmp = (uint8_t *)addr;
        printf(" %02X", tmp[i]);
    }
    printf("\r\n");
#endif

    if (eloop_event_commit(ELOOP_EVT_WPA_L2_DATA, addr, len)) {
        printf("failed to commit L2 data\r\n");
    }
    return 0;
}

int net_if_add(net_al_if_t *netif,
               const uint8_t *mac_addr,
               const uint32_t *ipaddr,
               const uint32_t *netmask,
               const uint32_t *gw,
               void *fhostvif)
{
    static int init = 0;

    if (init == 0) {
        init = 1;
        qc7xx_data.fhostvif = fhostvif;
        memcpy(qc7xx_data.mac_addr, mac_addr, WIFI_MAC_ADDR_LEN);
        *netif = qc7xx_wifi_iface;
#if defined(CONFIG_QC7XX_WIFI_AP_STA_MODE)
    } else if (init == 1) {
        init = 2;
        qc7xx_ap_sta_data.fhostvif = fhostvif;
        memcpy(qc7xx_ap_sta_data.mac_addr, mac_addr, WIFI_MAC_ADDR_LEN);
        *netif = qc7xx_wifi_iface_ap;
#endif
    } else {
        printf("%s : reinit.\r\n", __FUNCTION__);
    }

    return 0;
}

void *net_if_vif_info(net_al_if_t net_if)
{
    if ((uint32_t)net_if == (uint32_t)qc7xx_wifi_iface) {
        return qc7xx_data.fhostvif;
#if defined(CONFIG_QC7XX_WIFI_AP_STA_MODE)
    } else if ((uint32_t)net_if == (uint32_t)qc7xx_wifi_iface_ap) {
        return qc7xx_ap_sta_data.fhostvif;
#endif
    }
    return NULL;
}

const uint8_t *net_if_get_mac_addr(net_al_if_t net_if)
{
    const struct device *dev = net_if_get_device(net_if);
    struct qc7xx_wifi_runtime *dev_data = dev->data;

    return dev_data->mac_addr;
}

net_al_if_t net_if_find_from_name(const char *name)
{
    if (strcmp(name, "wl0") == 0) {
        return qc7xx_wifi_iface;
    }
#if defined(CONFIG_QC7XX_WIFI_AP_STA_MODE)
    else if (strcmp(name, "wl1") == 0) {
        return qc7xx_wifi_iface_ap;
    }
#endif

    return NULL;
}

int fh_net_if_get_name(net_al_if_t net_if, char *buf, int len)
{
    if ((uint32_t)net_if == (uint32_t)qc7xx_wifi_iface) {
        buf[0] = 'w';
        buf[1] = 'l';
        buf[2] = '0';
        buf[3] = '\0';
#if defined(CONFIG_QC7XX_WIFI_AP_STA_MODE)
    } else if ((uint32_t)net_if == (uint32_t)qc7xx_wifi_iface_ap) {
        buf[0] = 'w';
        buf[1] = 'l';
        buf[2] = '1';
        buf[3] = '\0';
#endif
    } else {
        printf("never here.\r\n");
    }

    return 3;
}

void net_if_up_cb(net_al_if_t net_if)
{
    net_eth_carrier_on((struct net_if *)net_if);
}

void net_if_down_cb(net_al_if_t net_if)
{
    net_eth_carrier_off((struct net_if *)net_if);
}

void net_al_tx_init()
{

}

void net_al_tx_do_sta_del(uint8_t sta_id, release_buf_cb release_buf)
{

}

void net_al_tx_cfm()
{

}
