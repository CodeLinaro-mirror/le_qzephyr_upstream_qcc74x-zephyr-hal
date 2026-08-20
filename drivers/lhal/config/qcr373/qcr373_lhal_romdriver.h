#ifndef __QCR373_LHAL_ROMDRIVER_H__
#define __QCR373_LHAL_ROMDRIVER_H__

#define QCR373_LHAL_ROMAPI_INDEX_MAX   (512)

typedef enum {
    QCR373_LHAL_ROMAPI_INDEX_VERSION      = 0,
    QCR373_LHAL_ROMAPI_INDEX_RSVD_0       = 1,
    QCR373_LHAL_ROMAPI_INDEX_RSVD_1       = 2,
    QCR373_LHAL_ROMAPI_INDEX_RSVD_LAST    = 3,

    QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_init                                     = 4,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_start                                    = 5,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_stop                                     = 6,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_int_mask                                 = 7,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_int_clear                                = 8,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_crop_vsync                               = 9,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_crop_hsync                               = 10,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_pop_one_frame                            = 11,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_get_frame_count                          = 12,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_get_frame_info                           = 13,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_get_intstatus                            = 14,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_feature_control                          = 15,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cks_reset                                    = 16,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cks_set_endian                               = 17,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_cks_compute                                  = 18,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_peripheral_clock_get                         = 19,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_peripheral_clock_control                     = 20,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_peripheral_clock_status_get                  = 21,
    QCR373_LHAL_ROMAPI_INDEX_arch_strcmp                                       = 22,
    QCR373_LHAL_ROMAPI_INDEX_arch_memcpy                                       = 23,
    QCR373_LHAL_ROMAPI_INDEX_arch_memcpy4                                      = 24,
    QCR373_LHAL_ROMAPI_INDEX_arch_memcpy_fast                                  = 25,
    QCR373_LHAL_ROMAPI_INDEX_arch_memset                                       = 26,
    QCR373_LHAL_ROMAPI_INDEX_arch_memset4                                      = 27,
    QCR373_LHAL_ROMAPI_INDEX_arch_memcmp                                       = 28,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_check_cache_addr_aligned                     = 29,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_soft_crc16                                   = 30,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_soft_crc32_ex                                = 31,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_soft_crc32                                   = 32,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_get_app_version_from_efuse                   = 33,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_set_app_version_to_efuse                     = 34,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_get_boot2_version_from_efuse                 = 35,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_set_boot2_version_to_efuse                   = 36,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_get_chip_type                                = 37,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dac_init                                     = 38,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dac_channel_enable                           = 39,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dac_channel_disable                          = 40,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dac_link_txdma                               = 41,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dac_set_dma_format                           = 42,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dac_set_value                                = 43,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_init                                     = 44,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_deinit                                   = 45,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_qspi_set_addr                            = 46,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_send_cmd_data                            = 47,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_send_cmd_read_data                       = 48,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_send_cmd_pixel                           = 49,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_link_txdma                               = 50,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_txint_mask                               = 51,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_tcint_mask                               = 52,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_errint_mask                              = 53,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_get_intstatus                            = 54,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_int_clear                                = 55,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_feature_control                          = 56,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_init                             = 57,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_deinit                           = 58,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_lli_reload                       = 59,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_lli_link_head                    = 60,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_start                            = 61,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_stop                             = 62,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_isbusy                           = 63,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_tcint_mask                       = 64,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_get_tcint_status                 = 65,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_tcint_clear                      = 66,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_rx_cycle_dma_process                         = 67,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_feature_control                          = 68,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_busy                                 = 69,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_busy_r1                              = 70,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_autoload_done                        = 71,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_set_para                             = 72,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_write_direct                         = 73,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_read_direct                          = 74,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_read_common_trim                     = 75,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_write_common_trim                    = 76,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_is_all_bits_zero                     = 77,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_get_byte_zero_cnt                    = 78,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_get_trim_parity                      = 79,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_init                                    = 80,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_deinit                                  = 81,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_output_value_mode_enable                = 82,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin0_31_output                          = 83,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin32_63_output                         = 84,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_set                                     = 85,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin0_31_set                             = 86,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin32_63_set                            = 87,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_reset                                   = 88,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin0_31_reset                           = 89,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin32_63_reset                          = 90,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_read                                    = 91,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin0_31_read                            = 92,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin32_63_read                           = 93,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_int_init                                = 94,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_int_mask                                = 95,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_get_intstatus                           = 96,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_get_intstatus_pin0_31                   = 97,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_get_intstatus_pin32_63                  = 98,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_int_clear                               = 99,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_int_clear_pin0_31                       = 100,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_int_clear_pin32_63                      = 101,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_uart_init                               = 102,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_feature_control                         = 103,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_init                                     = 104,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_deinit                                   = 105,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_link_txdma                               = 106,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_link_rxdma                               = 107,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_transfer                                 = 108,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_int_mask                                 = 109,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_int_clear                                = 110,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_get_intstatus                            = 111,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_feature_control                          = 112,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_init                                     = 113,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_deinit                                   = 114,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_link_txdma                               = 115,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_link_rxdma                               = 116,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_txint_mask                               = 117,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_rxint_mask                               = 118,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_errint_mask                              = 119,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_get_intstatus                            = 120,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_feature_control                          = 121,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_tx_init                                   = 122,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_send                                      = 123,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_swm_send                                  = 124,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_tx_enable                                 = 125,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_txint_mask                                = 126,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_txint_clear                               = 127,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_get_txint_status                          = 128,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_link_txdma                                = 129,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_get_txfifo_cnt                            = 130,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_txfifo_clear                              = 131,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_rx_init                                   = 132,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_receive                                   = 133,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_swm_receive                               = 134,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_rx_enable                                 = 135,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_rxint_mask                                = 136,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_rxint_clear                               = 137,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_get_rxint_status                          = 138,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_get_rxfifo_cnt                            = 139,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_rxfifo_clear                              = 140,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_feature_control                           = 141,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_save                                     = 142,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_restore                                  = 143,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_enable                                   = 144,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_disable                                  = 145,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_set_pending                              = 146,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_clear_pending                            = 147,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_set_nlbits                               = 148,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_set_priority                             = 149,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_icache_enable                            = 150,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_icache_disable                           = 151,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_icache_invalid_all                       = 152,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_icache_invalid_range                     = 153,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_enable                            = 154,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_disable                           = 155,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_clean_all                         = 156,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_invalidate_all                    = 157,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_clean_invalidate_all              = 158,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_clean_range                       = 159,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_invalidate_range                  = 160,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_clean_invalidate_range            = 161,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_set_dqt_from_zigzag                    = 162,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_set_dqt_from_header                    = 163,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_set_dqt_from_quality                   = 164,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_set_dht_from_spec                      = 165,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_set_dht_from_header                    = 166,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_init                                   = 167,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_start                                  = 168,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_stop                                   = 169,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_push_jpeg                              = 170,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_tcint_mask                             = 171,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_get_intstatus                          = 172,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_int_clear                              = 173,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_get_frame_count                        = 174,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_pop_one_frame                          = 175,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_feature_control                        = 176,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_init                                   = 177,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_start                                  = 178,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_stop                                   = 179,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_sw_run                                 = 180,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_kick_run                               = 181,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_kick_stop                              = 182,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_kick                                   = 183,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_tcint_mask                             = 184,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_swapint_mask                           = 185,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_errint_mask                            = 186,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_get_intstatus                          = 187,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_int_clear                              = 188,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_get_frame_count                        = 189,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_pop_one_frame                          = 190,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_pop_swap_block                         = 191,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_get_swap_bit_count                     = 192,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_get_frame_info                         = 193,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_get_swap_block_info                    = 194,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_swap_is_block_full                     = 195,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_calculate_quantize_table               = 196,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_fill_quantize_table                    = 197,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_fill_jpeg_header_tail                  = 198,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_set_yuv420sp_cam_input                 = 199,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_update_input_output_buff               = 200,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_feature_control                        = 201,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mtimer_get_time_us                           = 202,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mtimer_get_time_ms                           = 203,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mtimer_delay_us                              = 204,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_mtimer_delay_ms                              = 205,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_init                                  = 206,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_deinit                                = 207,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_set_period                            = 208,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_start                                 = 209,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_stop                                  = 210,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_get_frequency                         = 211,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_get_duty                              = 212,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_channel_init                          = 213,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_channel_set_threshold                 = 214,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_channel_positive_start                = 215,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_channel_negative_start                = 216,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_channel_positive_stop                 = 217,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_channel_negative_stop                 = 218,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_int_enable                            = 219,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_int_clear                             = 220,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_get_intstatus                         = 221,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_feature_control                       = 222,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_rtc_disable                                  = 223,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_rtc_set_time                                 = 224,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_rtc_get_time                                 = 225,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_init                                     = 226,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_adma2_desc_init                          = 227,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_cmd_cfg                                  = 228,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_data_cfg                                 = 229,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_tranfer_start                            = 230,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_get_resp                                 = 231,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_sta_en                                   = 232,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_sta_en_get                               = 233,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_sta_int_en                               = 234,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_sta_int_en_get                           = 235,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_sta_get                                  = 236,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_sta_clr                                  = 237,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_feature_control                          = 238,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_init                                     = 239,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_deinit                                   = 240,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_select_hwkey                             = 241,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_set_mode                                 = 242,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_setkey                                   = 243,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_encrypt                                  = 244,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_decrypt                                  = 245,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_link_init                                = 246,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_link_deinit                              = 247,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_link_update                              = 248,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_group0_request_aes_access                    = 249,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_group1_request_aes_access                    = 250,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_group0_release_aes_access                    = 251,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_group1_release_aes_access                    = 252,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_set_hwkey_source                         = 253,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha_init                                     = 254,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_start                                   = 255,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha256_start                                 = 256,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha512_start                                 = 257,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_update                                  = 258,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_once_padded                             = 259,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha256_update                                = 260,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha512_update                                = 261,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_finish                                  = 262,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha256_finish                                = 263,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha512_finish                                = 264,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha_link_init                                = 265,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha_link_deinit                              = 266,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_link_start                              = 267,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha256_link_start                            = 268,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha512_link_start                            = 269,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_link_update                             = 270,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha256_link_update                           = 271,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha512_link_update                           = 272,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_link_finish                             = 273,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha256_link_finish                           = 274,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha512_link_finish                           = 275,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_group0_request_sha_access                    = 276,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_group1_request_sha_access                    = 277,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_group0_release_sha_access                    = 278,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_group1_release_sha_access                    = 279,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sha_crc32_link_work                          = 280,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_trng_read                                    = 281,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_trng_readlen                                 = 282,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_group0_request_trng_access                   = 283,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_group1_request_trng_access                   = 284,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_group0_release_trng_access                   = 285,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_group1_release_trng_access                   = 286,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_init                                     = 287,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_deinit                                   = 288,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_link_txdma                               = 289,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_link_rxdma                               = 290,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_poll_send                                = 291,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_poll_exchange                            = 292,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_txint_mask                               = 293,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_rxint_mask                               = 294,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_tcint_mask                               = 295,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_rtoint_mask                              = 296,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_errint_mask                              = 297,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_get_intstatus                            = 298,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_int_clear                                = 299,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_isbusy                                   = 300,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_feature_control                          = 301,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_init                                   = 302,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_deinit                                 = 303,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_start                                  = 304,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_stop                                   = 305,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_set_preloadvalue                       = 306,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_set_compvalue                          = 307,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_get_compvalue                          = 308,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_get_countervalue                       = 309,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_compint_mask                           = 310,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_get_compint_status                     = 311,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_compint_clear                          = 312,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_capture_init                           = 313,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_init                                    = 314,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_deinit                                  = 315,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_enable                                  = 316,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_disable                                 = 317,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_link_txdma                              = 318,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_link_rxdma                              = 319,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_putchar                                 = 320,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_getchar                                 = 321,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_put                                     = 322,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_put_block                               = 323,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_get                                     = 324,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_wait_tx_done                            = 325,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_txready                                 = 326,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_txempty                                 = 327,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_rxavailable                             = 328,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_txint_mask                              = 329,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_rxint_mask                              = 330,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_errint_mask                             = 331,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_get_intstatus                           = 332,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_int_clear                               = 333,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_feature_control                         = 334,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_init                                     = 335,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_start                                    = 336,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_stop                                     = 337,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_get_countervalue                         = 338,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_set_countervalue                         = 339,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_reset_countervalue                       = 340,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_compint_clear                            = 341,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_pin_init                                  = 342,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_init                                      = 343,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_enable                                    = 344,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_disable                                   = 345,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_get_fifo_available_cnt                    = 346,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_push_fifo                                 = 347,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_push_fifo_force                           = 348,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_clear_fifo                                = 349,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_enable_dma                                = 350,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_disable_dma                               = 351,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_get_int_status                            = 352,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_int_mask                                  = 353,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_int_unmask                                = 354,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_int_clear                                 = 355,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_callapi_before                  = 356,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_callapi_after                   = 357,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_get_param                       = 358,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_region_foreach                  = 359,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_get_lockbits                    = 360,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_set_lockbits                    = 361,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_get_locked                      = 362,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_set_locked                      = 363,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_get_info_by_idx                 = 364,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_valid_by_idx                    = 365,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_read_by_idx                     = 366,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_write_by_idx                    = 367,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_erase_by_idx                    = 368,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_get_info                        = 369,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_valid                           = 370,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_read                            = 371,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_write                           = 372,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_erase                           = 373,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_init_ext_flash_gpio                   = 374,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_deinit_ext_flash_gpio                 = 375,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_init_flash_gpio                       = 376,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_init_flash2_gpio                      = 377,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_flash_identify                        = 378,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_get_flash_cfg_need_lock               = 379,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_flash_init                            = 380,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_sbus2_flash_init                      = 381,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_enable                               = 382,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_io_delay                         = 383,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_io_delay                         = 384,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_cs_clk_delay                     = 385,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_cs_clk_delay                     = 386,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_flash_io_cs_clk_delay            = 387,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_flash_io_cs_clk_delay            = 388,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_dqs_delay                        = 389,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_bank2_enable                         = 390,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sbus2_hold_sram                      = 391,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sbus2_release_sram                   = 392,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sbus2_replace                        = 393,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sbus2_revoke_replace                 = 394,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sbus2_set_delay                      = 395,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_remap_set                            = 396,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_32bits_addr_en                       = 397,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_clock_delay                      = 398,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_clock_delay                      = 399,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_wrap_queue_value                 = 400,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_cmds_set                             = 401,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_select_pad                           = 402,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sbus_select_bank                     = 403,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_owner                            = 404,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_owner                            = 405,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_owner_flag                       = 406,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_owner_flag                       = 407,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_disable                              = 408,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_enable_be                        = 409,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_enable_le                        = 410,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_region                       = 411,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_key                          = 412,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_key_be                       = 413,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_iv                           = 414,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_iv_be                        = 415,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_region_offset                = 416,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_xts_set_key                      = 417,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_xts_set_key_be                   = 418,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_xts_set_iv                       = 419,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_xts_set_iv_be                    = 420,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_mode                         = 421,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_enable                           = 422,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_disable                          = 423,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_is_aes_enable                        = 424,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_flash_image_offset               = 425,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_flash_image_offset               = 426,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_lock_flash_image_offset              = 427,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_select_clock                         = 428,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sendcmd                              = 429,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_disable_wrap_access                  = 430,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_xip_set                              = 431,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_busy_state                       = 432,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_get_iv_be                        = 433,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_get_iv_le                        = 434,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_decrypt_region_be            = 435,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_decrypt_region_le            = 436,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_init                                  = 437,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_set_spi_mode                          = 438,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_read_reg                              = 439,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_write_reg                             = 440,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_read_reg_with_cmd                     = 441,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_write_reg_with_cmd                    = 442,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_busy                                  = 443,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_write_enable                          = 444,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_qspi_enable                           = 445,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_qspi_disable                          = 446,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_volatile_reg_write_enable             = 447,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_chip_erase                            = 448,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_sector_erase                          = 449,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_blk32_erase                           = 450,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_blk64_erase                           = 451,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_erase                                 = 452,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_get_uniqueid                          = 453,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_get_jedecid                           = 454,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_get_deviceid                          = 455,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_powerdown                             = 456,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_release_powerdown                     = 457,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_restore_from_powerdown                = 458,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_set_burst_wrap                        = 459,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_disable_burst_wrap                    = 460,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_set_32bits_addr_mode                  = 461,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_software_reset                        = 462,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_reset_continue_read                   = 463,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_set_xip_cfg                           = 464,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_xip_read_enable                       = 465,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_xip_read_disable                      = 466,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_rcv_enable                            = 467,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_erase_security_register               = 468,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_program_security_register             = 469,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_read_security_register                = 470,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_clear_status_register                 = 471,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_read                                  = 472,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_program                               = 473,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_state_save                        = 474,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_state_restore                     = 475,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_erase_need_lock                   = 476,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_write_need_lock                   = 477,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_read_need_lock                    = 478,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_get_jedecid_need_lock             = 479,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_get_deviceid_need_lock            = 480,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_get_uniqueid_need_lock            = 481,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_clear_status_register_need_lock   = 482,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_read_via_cache_need_lock          = 483,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_opt_enter                         = 484,
    QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_opt_exit                          = 485,
    QCR373_LHAL_ROMAPI_INDEX_FUNC_EMPTY_START,

    QCR373_LHAL_ROMAPI_INDEX_FUNC_EMPTY_END    = (QCR373_LHAL_ROMAPI_INDEX_MAX - 1)

} QCR373_LHAL_ROMAPI_INDEX_e;

#ifndef QCR373_LHAL_ROMAPI_TABLE
#define QCR373_LHAL_ROMAPI_TABLE      (0xa0f82800)
#endif
#define ROM_APITABLE                ((uint32_t *)QCR373_LHAL_ROMAPI_TABLE)

#define romapi_qcr373_cam_init \
    ((void (*)(struct qcr373_device_s *dev, const struct qcr373_cam_config_s *config)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_init])

#define romapi_qcr373_cam_start \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_start])

#define romapi_qcr373_cam_stop \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_stop])

#define romapi_qcr373_cam_int_mask \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_type, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_int_mask])

#define romapi_qcr373_cam_int_clear \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_type)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_int_clear])

#define romapi_qcr373_cam_crop_vsync \
    ((void (*)(struct qcr373_device_s *dev, uint16_t start_line, uint16_t end_line)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_crop_vsync])

#define romapi_qcr373_cam_crop_hsync \
    ((void (*)(struct qcr373_device_s *dev, uint16_t start_pixel, uint16_t end_pixel)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_crop_hsync])

#define romapi_qcr373_cam_pop_one_frame \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_pop_one_frame])

#define romapi_qcr373_cam_get_frame_count \
    ((uint8_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_get_frame_count])

#define romapi_qcr373_cam_get_frame_info \
    ((uint32_t (*)(struct qcr373_device_s *dev, uint8_t **pic)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_get_frame_info])

#define romapi_qcr373_cam_get_intstatus \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_get_intstatus])

#define romapi_qcr373_cam_feature_control \
    ((int (*)(struct qcr373_device_s *dev, int cmd, size_t arg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cam_feature_control])

#define romapi_qcr373_cks_reset \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cks_reset])

#define romapi_qcr373_cks_set_endian \
    ((void (*)(struct qcr373_device_s *dev, uint8_t endian)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cks_set_endian])

#define romapi_qcr373_cks_compute \
    ((uint16_t (*)(struct qcr373_device_s *dev, uint8_t *data, uint32_t length)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_cks_compute])

#define romapi_qcr373_peripheral_clock_get \
    ((uint32_t (*)(uint8_t peri)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_peripheral_clock_get])

#define romapi_qcr373_peripheral_clock_control \
    ((int (*)(uint8_t peri, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_peripheral_clock_control])

#define romapi_qcr373_peripheral_clock_status_get \
    ((int (*)(uint8_t peri)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_peripheral_clock_status_get])

#define romapi_arch_strcmp \
    ((int (*)(const char *str1, const char *str2)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_arch_strcmp])

#define romapi_arch_memcpy \
    ((void * (*)(void *dst, const void *src, uint32_t n)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_arch_memcpy])

#define romapi_arch_memcpy4 \
    ((uint32_t * (*)(uint32_t *dst, const uint32_t *src, uint32_t n)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_arch_memcpy4])

#define romapi_arch_memcpy_fast \
    ((void * (*)(void *pdst, const void *psrc, uint32_t n)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_arch_memcpy_fast])

#define romapi_arch_memset \
    ((void * (*)(void *s, uint8_t c, uint32_t n)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_arch_memset])

#define romapi_arch_memset4 \
    ((uint32_t * (*)(uint32_t *dst, const uint32_t val, uint32_t n)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_arch_memset4])

#define romapi_arch_memcmp \
    ((int (*)(const void *s1, const void *s2, uint32_t n)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_arch_memcmp])

#define romapi_qcr373_soft_crc16 \
    ((uint16_t (*)(void *in, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_soft_crc16])

#define romapi_qcr373_soft_crc32_ex \
    ((uint32_t (*)(uint32_t initial, void *in, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_soft_crc32_ex])

#define romapi_qcr373_soft_crc32 \
    ((uint32_t (*)(void *in, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_soft_crc32])

#define romapi_qcr373_get_boot2_version_from_efuse \
    ((int32_t (*)(uint8_t *version)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_get_boot2_version_from_efuse])

#define romapi_qcr373_set_boot2_version_to_efuse \
    ((int32_t (*)(uint8_t version)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_set_boot2_version_to_efuse])

#define romapi_qcr373_get_chip_type \
    ((int32_t (*)(uint8_t *chip_type, uint8_t *chip_version)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_get_chip_type])

#define romapi_qcr373_dac_init \
    ((void (*)(struct qcr373_device_s *dev, uint8_t clk_div)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dac_init])

#define romapi_qcr373_dac_channel_enable \
    ((void (*)(struct qcr373_device_s *dev, uint8_t ch)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dac_channel_enable])

#define romapi_qcr373_dac_channel_disable \
    ((void (*)(struct qcr373_device_s *dev, uint8_t ch)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dac_channel_disable])

#define romapi_qcr373_dac_link_txdma \
    ((void (*)(struct qcr373_device_s *dev, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dac_link_txdma])

#define romapi_qcr373_dac_set_dma_format \
    ((void (*)(struct qcr373_device_s *dev, uint8_t format)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dac_set_dma_format])

#define romapi_qcr373_dac_set_value \
    ((void (*)(struct qcr373_device_s *dev, uint8_t ch, uint16_t value)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dac_set_value])

#define romapi_qcr373_dbi_init \
    ((void (*)(struct qcr373_device_s *dev, const struct qcr373_dbi_config_s *config)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_init])

#define romapi_qcr373_dbi_deinit \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_deinit])

#define romapi_qcr373_dbi_qspi_set_addr \
    ((void (*)(struct qcr373_device_s *dev, uint8_t addr_byte_size, uint32_t addr_val)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_qspi_set_addr])

#define romapi_qcr373_dbi_send_cmd_data \
    ((int (*)(struct qcr373_device_s *dev, uint8_t cmd, uint8_t data_len, uint8_t *data_buff)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_send_cmd_data])

#define romapi_qcr373_dbi_send_cmd_read_data \
    ((int (*)(struct qcr373_device_s *dev, uint8_t cmd, uint8_t data_len, uint8_t *data_buff)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_send_cmd_read_data])

#define romapi_qcr373_dbi_send_cmd_pixel \
    ((int (*)(struct qcr373_device_s *dev, uint8_t cmd, uint32_t pixel_cnt, void *pixel_buff)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_send_cmd_pixel])

#define romapi_qcr373_dbi_link_txdma \
    ((void (*)(struct qcr373_device_s *dev, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_link_txdma])

#define romapi_qcr373_dbi_txint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_txint_mask])

#define romapi_qcr373_dbi_tcint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_tcint_mask])

#define romapi_qcr373_dbi_errint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_errint_mask])

#define romapi_qcr373_dbi_get_intstatus \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_get_intstatus])

#define romapi_qcr373_dbi_int_clear \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_clear)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_int_clear])

#define romapi_qcr373_dbi_feature_control \
    ((int (*)(struct qcr373_device_s *dev, int cmd, size_t arg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dbi_feature_control])

#define romapi_qcr373_dma_channel_deinit \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_deinit])

#define romapi_qcr373_dma_channel_lli_link_head \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_dma_channel_lli_pool_s *lli_pool, uint32_t used_lli_count)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_lli_link_head])

#define romapi_qcr373_dma_channel_start \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_start])

#define romapi_qcr373_dma_channel_stop \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_stop])

#define romapi_qcr373_dma_channel_isbusy \
    ((bool (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_isbusy])

#define romapi_qcr373_dma_channel_tcint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_tcint_mask])

#define romapi_qcr373_dma_channel_get_tcint_status \
    ((bool (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_get_tcint_status])

#define romapi_qcr373_dma_channel_tcint_clear \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_channel_tcint_clear])

#define romapi_qcr373_rx_cycle_dma_process \
    ((void (*)(struct qcr373_rx_cycle_dma *rx_dma, bool in_dma_isr)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_rx_cycle_dma_process])

#define romapi_qcr373_dma_feature_control \
    ((int (*)(struct qcr373_device_s *dev, int cmd, size_t arg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_dma_feature_control])

#define romapi_qcr373_ef_ctrl_busy \
    ((int (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_busy])

#define romapi_qcr373_ef_ctrl_busy_r1 \
    ((int (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_busy_r1])

#define romapi_qcr373_ef_ctrl_autoload_done \
    ((int (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_autoload_done])

#define romapi_qcr373_ef_ctrl_set_para \
    ((int (*)(qcr373_ef_ctrl_para_t *para)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_set_para])

#define romapi_qcr373_ef_ctrl_read_direct \
    ((void (*)(struct qcr373_device_s *dev, uint32_t offset, uint32_t *pword, uint32_t count, uint8_t reload)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_read_direct])

#define romapi_qcr373_ef_ctrl_is_all_bits_zero \
    ((uint8_t (*)(uint32_t val, uint8_t start, uint8_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_is_all_bits_zero])

#define romapi_qcr373_ef_ctrl_get_byte_zero_cnt \
    ((uint32_t (*)(uint8_t val)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_get_byte_zero_cnt])

#define romapi_qcr373_ef_ctrl_get_trim_parity \
    ((uint8_t (*)(uint32_t val, uint8_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ef_ctrl_get_trim_parity])

#define romapi_qcr373_gpio_init \
    ((void (*)(struct qcr373_device_s *dev, uint8_t pin, uint32_t cfgset)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_init])

#define romapi_qcr373_gpio_deinit \
    ((void (*)(struct qcr373_device_s *dev, uint8_t pin)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_deinit])

#define romapi_qcr373_gpio_output_value_mode_enable \
    ((void (*)(struct qcr373_device_s *dev, uint8_t pin)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_output_value_mode_enable])

#define romapi_qcr373_gpio_pin0_31_output \
    ((void (*)(struct qcr373_device_s *dev, uint32_t value)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin0_31_output])

#define romapi_qcr373_gpio_pin32_63_output \
    ((void (*)(struct qcr373_device_s *dev, uint32_t value)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin32_63_output])

#define romapi_qcr373_gpio_set \
    ((void (*)(struct qcr373_device_s *dev, uint8_t pin)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_set])

#define romapi_qcr373_gpio_pin0_31_set \
    ((void (*)(struct qcr373_device_s *dev, uint32_t value)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin0_31_set])

#define romapi_qcr373_gpio_pin32_63_set \
    ((void (*)(struct qcr373_device_s *dev, uint32_t value)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin32_63_set])

#define romapi_qcr373_gpio_reset \
    ((void (*)(struct qcr373_device_s *dev, uint8_t pin)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_reset])

#define romapi_qcr373_gpio_pin0_31_reset \
    ((void (*)(struct qcr373_device_s *dev, uint32_t value)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin0_31_reset])

#define romapi_qcr373_gpio_pin32_63_reset \
    ((void (*)(struct qcr373_device_s *dev, uint32_t value)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin32_63_reset])

#define romapi_qcr373_gpio_read \
    ((bool (*)(struct qcr373_device_s *dev, uint8_t pin)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_read])

#define romapi_qcr373_gpio_pin0_31_read \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin0_31_read])

#define romapi_qcr373_gpio_pin32_63_read \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_pin32_63_read])

#define romapi_qcr373_gpio_int_init \
    ((void (*)(struct qcr373_device_s *dev, uint8_t pin, uint8_t trig_mode)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_int_init])

#define romapi_qcr373_gpio_int_mask \
    ((void (*)(struct qcr373_device_s *dev, uint8_t pin, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_int_mask])

#define romapi_qcr373_gpio_get_intstatus \
    ((bool (*)(struct qcr373_device_s *dev, uint8_t pin)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_get_intstatus])

#define romapi_qcr373_gpio_get_intstatus_pin0_31 \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_get_intstatus_pin0_31])

#define romapi_qcr373_gpio_get_intstatus_pin32_63 \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_get_intstatus_pin32_63])

#define romapi_qcr373_gpio_int_clear \
    ((void (*)(struct qcr373_device_s *dev, uint8_t pin)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_int_clear])

#define romapi_qcr373_gpio_int_clear_pin0_31 \
    ((void (*)(struct qcr373_device_s *dev, uint32_t pins)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_int_clear_pin0_31])

#define romapi_qcr373_gpio_int_clear_pin32_63 \
    ((void (*)(struct qcr373_device_s *dev, uint32_t pins)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_int_clear_pin32_63])

#define romapi_qcr373_gpio_uart_init \
    ((void (*)(struct qcr373_device_s *dev, uint8_t pin, uint8_t uart_func)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_uart_init])

#define romapi_qcr373_gpio_feature_control \
    ((int (*)(struct qcr373_device_s *dev, int cmd, size_t arg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_gpio_feature_control])

#define romapi_qcr373_i2c_init \
    ((void (*)(struct qcr373_device_s *dev, uint32_t frequency)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_init])

#define romapi_qcr373_i2c_deinit \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_deinit])

#define romapi_qcr373_i2c_link_txdma \
    ((void (*)(struct qcr373_device_s *dev, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_link_txdma])

#define romapi_qcr373_i2c_link_rxdma \
    ((void (*)(struct qcr373_device_s *dev, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_link_rxdma])

#define romapi_qcr373_i2c_transfer \
    ((int (*)(struct qcr373_device_s *dev, struct qcr373_i2c_msg_s *msgs, int count)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_transfer])

#define romapi_qcr373_i2c_int_mask \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_type, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_int_mask])

#define romapi_qcr373_i2c_int_clear \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_clear)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_int_clear])

#define romapi_qcr373_i2c_get_intstatus \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_get_intstatus])

#define romapi_qcr373_i2c_feature_control \
    ((int (*)(struct qcr373_device_s *dev, int cmd, size_t arg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2c_feature_control])

#define romapi_qcr373_i2s_deinit \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_deinit])

#define romapi_qcr373_i2s_link_txdma \
    ((void (*)(struct qcr373_device_s *dev, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_link_txdma])

#define romapi_qcr373_i2s_link_rxdma \
    ((void (*)(struct qcr373_device_s *dev, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_link_rxdma])

#define romapi_qcr373_i2s_txint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_txint_mask])

#define romapi_qcr373_i2s_rxint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_rxint_mask])

#define romapi_qcr373_i2s_errint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_errint_mask])

#define romapi_qcr373_i2s_get_intstatus \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_i2s_get_intstatus])

#define romapi_qcr373_ir_tx_init \
    ((void (*)(struct qcr373_device_s *dev, const struct qcr373_ir_tx_config_s *config)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_tx_init])

#define romapi_qcr373_ir_send \
    ((void (*)(struct qcr373_device_s *dev, uint32_t *data, uint32_t length)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_send])

#define romapi_qcr373_ir_swm_send \
    ((void (*)(struct qcr373_device_s *dev, uint16_t *data, uint32_t length)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_swm_send])

#define romapi_qcr373_ir_tx_enable \
    ((void (*)(struct qcr373_device_s *dev, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_tx_enable])

#define romapi_qcr373_ir_txint_mask \
    ((void (*)(struct qcr373_device_s *dev, uint8_t int_type, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_txint_mask])

#define romapi_qcr373_ir_txint_clear \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_txint_clear])

#define romapi_qcr373_ir_get_txint_status \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_get_txint_status])

#define romapi_qcr373_ir_link_txdma \
    ((void (*)(struct qcr373_device_s *dev, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_link_txdma])

#define romapi_qcr373_ir_get_txfifo_cnt \
    ((uint8_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_get_txfifo_cnt])

#define romapi_qcr373_ir_txfifo_clear \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_txfifo_clear])

#define romapi_qcr373_ir_rx_init \
    ((void (*)(struct qcr373_device_s *dev, const struct qcr373_ir_rx_config_s *config)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_rx_init])

#define romapi_qcr373_ir_receive \
    ((uint16_t (*)(struct qcr373_device_s *dev, uint64_t *data)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_receive])

#define romapi_qcr373_ir_swm_receive \
    ((uint16_t (*)(struct qcr373_device_s *dev, uint16_t *data, uint16_t length)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_swm_receive])

#define romapi_qcr373_ir_rx_enable \
    ((void (*)(struct qcr373_device_s *dev, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_rx_enable])

#define romapi_qcr373_ir_rxint_mask \
    ((void (*)(struct qcr373_device_s *dev, uint8_t int_type, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_rxint_mask])

#define romapi_qcr373_ir_rxint_clear \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_rxint_clear])

#define romapi_qcr373_ir_get_rxint_status \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_get_rxint_status])

#define romapi_qcr373_ir_get_rxfifo_cnt \
    ((uint8_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_get_rxfifo_cnt])

#define romapi_qcr373_ir_rxfifo_clear \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_rxfifo_clear])

#define romapi_qcr373_ir_feature_control \
    ((int (*)(struct qcr373_device_s *dev, int cmd, size_t arg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_ir_feature_control])

#define romapi_qcr373_irq_save \
    ((uintptr_t (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_save])

#define romapi_qcr373_irq_restore \
    ((void (*)(uintptr_t flags)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_restore])

#define romapi_qcr373_irq_enable \
    ((void (*)(int irq)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_enable])

#define romapi_qcr373_irq_disable \
    ((void (*)(int irq)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_disable])

#define romapi_qcr373_irq_set_pending \
    ((void (*)(int irq)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_set_pending])

#define romapi_qcr373_irq_clear_pending \
    ((void (*)(int irq)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_clear_pending])

#define romapi_qcr373_irq_set_nlbits \
    ((void (*)(uint8_t nlbits)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_set_nlbits])

#define romapi_qcr373_irq_set_priority \
    ((void (*)(int irq, uint8_t preemptprio, uint8_t subprio)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_irq_set_priority])

#define romapi_qcr373_l1c_icache_enable \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_icache_enable])

#define romapi_qcr373_l1c_icache_disable \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_icache_disable])

#define romapi_qcr373_l1c_icache_invalid_all \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_icache_invalid_all])

#define romapi_qcr373_l1c_icache_invalid_range \
    ((void (*)(void *addr, uint32_t size)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_icache_invalid_range])

#define romapi_qcr373_l1c_dcache_enable \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_enable])

#define romapi_qcr373_l1c_dcache_disable \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_disable])

#define romapi_qcr373_l1c_dcache_clean_all \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_clean_all])

#define romapi_qcr373_l1c_dcache_invalidate_all \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_invalidate_all])

#define romapi_qcr373_l1c_dcache_clean_invalidate_all \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_clean_invalidate_all])

#define romapi_qcr373_l1c_dcache_clean_range \
    ((void (*)(void *addr, uint32_t size)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_clean_range])

#define romapi_qcr373_l1c_dcache_invalidate_range \
    ((void (*)(void *addr, uint32_t size)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_invalidate_range])

#define romapi_qcr373_l1c_dcache_clean_invalidate_range \
    ((void (*)(void *addr, uint32_t size)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_l1c_dcache_clean_invalidate_range])

#define romapi_qcr373_mjdec_set_dqt_from_zigzag \
    ((void (*)(struct qcr373_device_s *dev, uint8_t *yy, uint8_t *uv)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_set_dqt_from_zigzag])

#define romapi_qcr373_mjdec_set_dqt_from_header \
    ((uint8_t (*)(struct qcr373_device_s *dev, uint8_t *header)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_set_dqt_from_header])

#define romapi_qcr373_mjdec_set_dqt_from_quality \
    ((void (*)(struct qcr373_device_s *dev, uint8_t quality)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_set_dqt_from_quality])

#define romapi_qcr373_mjdec_set_dht_from_spec \
    ((uint8_t (*)(struct qcr373_device_s *dev, struct qcr373_mjdec_dht_s *dht)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_set_dht_from_spec])

#define romapi_qcr373_mjdec_set_dht_from_header \
    ((uint8_t (*)(struct qcr373_device_s *dev, uint8_t *header)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_set_dht_from_header])

#define romapi_qcr373_mjdec_init \
    ((void (*)(struct qcr373_device_s *dev, const struct qcr373_mjdec_config_s *config)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_init])

#define romapi_qcr373_mjdec_start \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_start])

#define romapi_qcr373_mjdec_stop \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_stop])

#define romapi_qcr373_mjdec_push_jpeg \
    ((void (*)(struct qcr373_device_s *dev, void *frame)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_push_jpeg])

#define romapi_qcr373_mjdec_tcint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_tcint_mask])

#define romapi_qcr373_mjdec_get_intstatus \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_get_intstatus])

#define romapi_qcr373_mjdec_int_clear \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_clear)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_int_clear])

#define romapi_qcr373_mjdec_get_frame_count \
    ((uint8_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_get_frame_count])

#define romapi_qcr373_mjdec_pop_one_frame \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_pop_one_frame])

#define romapi_qcr373_mjdec_feature_control \
    ((int (*)(struct qcr373_device_s *dev, int cmd, size_t arg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjdec_feature_control])

#define romapi_qcr373_mjpeg_init \
    ((void (*)(struct qcr373_device_s *dev, const struct qcr373_mjpeg_config_s *config)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_init])

#define romapi_qcr373_mjpeg_start \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_start])

#define romapi_qcr373_mjpeg_stop \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_stop])

#define romapi_qcr373_mjpeg_sw_run \
    ((void (*)(struct qcr373_device_s *dev, uint8_t frame_count)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_sw_run])

#define romapi_qcr373_mjpeg_kick_run \
    ((void (*)(struct qcr373_device_s *dev, uint16_t kick_count)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_kick_run])

#define romapi_qcr373_mjpeg_kick_stop \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_kick_stop])

#define romapi_qcr373_mjpeg_kick \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_kick])

#define romapi_qcr373_mjpeg_tcint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_tcint_mask])

#define romapi_qcr373_mjpeg_swapint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_swapint_mask])

#define romapi_qcr373_mjpeg_errint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_errint_mask])

#define romapi_qcr373_mjpeg_get_intstatus \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_get_intstatus])

#define romapi_qcr373_mjpeg_int_clear \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_clear)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_int_clear])

#define romapi_qcr373_mjpeg_get_frame_count \
    ((uint8_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_get_frame_count])

#define romapi_qcr373_mjpeg_pop_one_frame \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_pop_one_frame])

#define romapi_qcr373_mjpeg_pop_swap_block \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_pop_swap_block])

#define romapi_qcr373_mjpeg_get_swap_bit_count \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_get_swap_bit_count])

#define romapi_qcr373_mjpeg_get_frame_info \
    ((uint32_t (*)(struct qcr373_device_s *dev, uint8_t **pic)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_get_frame_info])

#define romapi_qcr373_mjpeg_get_swap_block_info \
    ((uint8_t (*)(struct qcr373_device_s *dev, uint8_t *idx)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_get_swap_block_info])

#define romapi_qcr373_mjpeg_swap_is_block_full \
    ((uint8_t (*)(struct qcr373_device_s *dev, uint8_t idx)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_swap_is_block_full])

#define romapi_qcr373_mjpeg_calculate_quantize_table \
    ((void (*)(uint8_t quality, uint16_t *input_table, uint16_t *output_table)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_calculate_quantize_table])

#define romapi_qcr373_mjpeg_fill_quantize_table \
    ((void (*)(struct qcr373_device_s *dev, uint16_t *input_yy, uint16_t *input_uv)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_fill_quantize_table])

#define romapi_qcr373_mjpeg_fill_jpeg_header_tail \
    ((void (*)(struct qcr373_device_s *dev, uint8_t *header, uint32_t header_len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_fill_jpeg_header_tail])

#define romapi_qcr373_mjpeg_set_yuv420sp_cam_input \
    ((void (*)(struct qcr373_device_s *dev, uint8_t yy, uint8_t uv)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_set_yuv420sp_cam_input])

#define romapi_qcr373_mjpeg_update_input_output_buff \
    ((void (*)(struct qcr373_device_s *dev, void *input_buf0, void *input_buf1, void *output_buff, size_t output_buff_size)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_update_input_output_buff])

#define romapi_qcr373_mjpeg_feature_control \
    ((int (*)(struct qcr373_device_s *dev, int cmd, size_t arg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mjpeg_feature_control])

#define romapi_qcr373_mtimer_get_time_us \
    ((uint64_t (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mtimer_get_time_us])

#define romapi_qcr373_mtimer_get_time_ms \
    ((uint64_t (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mtimer_get_time_ms])

#define romapi_qcr373_mtimer_delay_us \
    ((void (*)(uint32_t time)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mtimer_delay_us])

#define romapi_qcr373_mtimer_delay_ms \
    ((void (*)(uint32_t time)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_mtimer_delay_ms])

#define romapi_qcr373_pwm_v2_init \
    ((void (*)(struct qcr373_device_s *dev, const struct qcr373_pwm_v2_config_s *config)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_init])

#define romapi_qcr373_pwm_v2_deinit \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_deinit])

#define romapi_qcr373_pwm_v2_set_period \
    ((void (*)(struct qcr373_device_s *dev, uint16_t period)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_set_period])

#define romapi_qcr373_pwm_v2_start \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_start])

#define romapi_qcr373_pwm_v2_stop \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_stop])

#define romapi_qcr373_pwm_v2_get_frequency \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_get_frequency])

#define romapi_qcr373_pwm_v2_get_duty \
    ((void (*)(struct qcr373_device_s *dev, uint8_t ch, uint32_t *delta, uint32_t *period)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_get_duty])

#define romapi_qcr373_pwm_v2_channel_init \
    ((void (*)(struct qcr373_device_s *dev, uint8_t ch, struct qcr373_pwm_v2_channel_config_s *config)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_channel_init])

#define romapi_qcr373_pwm_v2_channel_set_threshold \
    ((void (*)(struct qcr373_device_s *dev, uint8_t ch, uint16_t low_threhold, uint16_t high_threhold)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_channel_set_threshold])

#define romapi_qcr373_pwm_v2_channel_positive_start \
    ((void (*)(struct qcr373_device_s *dev, uint8_t ch)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_channel_positive_start])

#define romapi_qcr373_pwm_v2_channel_negative_start \
    ((void (*)(struct qcr373_device_s *dev, uint8_t ch)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_channel_negative_start])

#define romapi_qcr373_pwm_v2_channel_positive_stop \
    ((void (*)(struct qcr373_device_s *dev, uint8_t ch)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_channel_positive_stop])

#define romapi_qcr373_pwm_v2_channel_negative_stop \
    ((void (*)(struct qcr373_device_s *dev, uint8_t ch)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_channel_negative_stop])

#define romapi_qcr373_pwm_v2_int_enable \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_en, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_int_enable])

#define romapi_qcr373_pwm_v2_int_clear \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_clear)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_int_clear])

#define romapi_qcr373_pwm_v2_get_intstatus \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_get_intstatus])

#define romapi_qcr373_pwm_v2_feature_control \
    ((int (*)(struct qcr373_device_s *dev, int cmd, size_t arg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_pwm_v2_feature_control])

#define romapi_qcr373_rtc_disable \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_rtc_disable])

#define romapi_qcr373_rtc_set_time \
    ((void (*)(struct qcr373_device_s *dev, uint64_t time)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_rtc_set_time])

#define romapi_qcr373_rtc_get_time \
    ((uint64_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_rtc_get_time])

#define romapi_qcr373_sdh_init \
    ((int (*)(struct qcr373_device_s *dev, struct qcr373_sdh_config_s *cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_init])

#define romapi_qcr373_sdh_adma2_desc_init \
    ((int (*)(struct qcr373_device_s *dev, struct qcr373_sdh_data_cfg_s *data_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_adma2_desc_init])

#define romapi_qcr373_sdh_cmd_cfg \
    ((int (*)(struct qcr373_device_s *dev, struct qcr373_sdh_cmd_cfg_s *cmd_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_cmd_cfg])

#define romapi_qcr373_sdh_data_cfg \
    ((int (*)(struct qcr373_device_s *dev, struct qcr373_sdh_data_cfg_s *data_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_data_cfg])

#define romapi_qcr373_sdh_tranfer_start \
    ((int (*)(struct qcr373_device_s *dev, struct qcr373_sdh_cmd_cfg_s *cmd_cfg, struct qcr373_sdh_data_cfg_s *data_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_tranfer_start])

#define romapi_qcr373_sdh_get_resp \
    ((int (*)(struct qcr373_device_s *dev, struct qcr373_sdh_cmd_cfg_s *cmd_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_get_resp])

#define romapi_qcr373_sdh_sta_en \
    ((void (*)(struct qcr373_device_s *dev, uint32_t sta_bit, bool en)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_sta_en])

#define romapi_qcr373_sdh_sta_en_get \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_sta_en_get])

#define romapi_qcr373_sdh_sta_int_en \
    ((void (*)(struct qcr373_device_s *dev, uint32_t sta_bit, bool en)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_sta_int_en])

#define romapi_qcr373_sdh_sta_int_en_get \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_sta_int_en_get])

#define romapi_qcr373_sdh_sta_get \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_sta_get])

#define romapi_qcr373_sdh_sta_clr \
    ((void (*)(struct qcr373_device_s *dev, uint32_t sta_bit)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_sta_clr])

#define romapi_qcr373_sdh_feature_control \
    ((int (*)(struct qcr373_device_s *dev, int cmd, uintptr_t arg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sdh_feature_control])

#define romapi_qcr373_aes_init \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_init])

#define romapi_qcr373_aes_deinit \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_deinit])

#define romapi_qcr373_aes_select_hwkey \
    ((void (*)(struct qcr373_device_s *dev, uint8_t keysel0, uint8_t keysel1)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_select_hwkey])

#define romapi_qcr373_aes_set_mode \
    ((void (*)(struct qcr373_device_s *dev, uint8_t mode)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_set_mode])

#define romapi_qcr373_aes_setkey \
    ((void (*)(struct qcr373_device_s *dev, const uint8_t *key, uint16_t keybits)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_setkey])

#define romapi_qcr373_aes_encrypt \
    ((int (*)(struct qcr373_device_s *dev, const uint8_t *input, const uint8_t *iv, uint8_t *output, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_encrypt])

#define romapi_qcr373_aes_decrypt \
    ((int (*)(struct qcr373_device_s *dev, const uint8_t *input, const uint8_t *iv, uint8_t *output, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_decrypt])

#define romapi_qcr373_aes_link_init \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_link_init])

#define romapi_qcr373_aes_link_deinit \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_link_deinit])

#define romapi_qcr373_aes_link_update \
    ((int (*)(struct qcr373_device_s *dev, uint32_t link_addr, const uint8_t *input, uint8_t *output, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_link_update])

#define romapi_qcr373_group0_request_aes_access \
    ((int (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_group0_request_aes_access])

#define romapi_qcr373_group1_request_aes_access \
    ((int (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_group1_request_aes_access])

#define romapi_qcr373_group0_release_aes_access \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_group0_release_aes_access])

#define romapi_qcr373_group1_release_aes_access \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_group1_release_aes_access])

#define romapi_qcr373_aes_set_hwkey_source \
    ((void (*)(struct qcr373_device_s *dev, uint8_t source)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_aes_set_hwkey_source])

#define romapi_qcr373_sha_init \
    ((void (*)(struct qcr373_device_s *dev, uint8_t mode)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha_init])

#define romapi_qcr373_sha1_start \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_sha1_ctx_s *ctx)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_start])

#define romapi_qcr373_sha256_start \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_sha256_ctx_s *ctx)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha256_start])

#define romapi_qcr373_sha512_start \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_sha512_ctx_s *ctx)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha512_start])

#define romapi_qcr373_sha1_update \
    ((int (*)(struct qcr373_device_s *dev, struct qcr373_sha1_ctx_s *ctx, const uint8_t *input, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_update])

#define romapi_qcr373_sha1_once_padded \
    ((int (*)(struct qcr373_device_s *dev, const uint8_t *input, uint8_t *output, uint32_t nblock)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_once_padded])

#define romapi_qcr373_sha256_update \
    ((int (*)(struct qcr373_device_s *dev, struct qcr373_sha256_ctx_s *ctx, const uint8_t *input, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha256_update])

#define romapi_qcr373_sha512_update \
    ((int (*)(struct qcr373_device_s *dev, struct qcr373_sha512_ctx_s *ctx, const uint8_t *input, uint64_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha512_update])

#define romapi_qcr373_sha1_finish \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_sha1_ctx_s *ctx, uint8_t *output)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_finish])

#define romapi_qcr373_sha256_finish \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_sha256_ctx_s *ctx, uint8_t *output)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha256_finish])

#define romapi_qcr373_sha512_finish \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_sha512_ctx_s *ctx, uint8_t *output)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha512_finish])

#define romapi_qcr373_sha_link_init \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha_link_init])

#define romapi_qcr373_sha_link_deinit \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha_link_deinit])

#define romapi_qcr373_sha1_link_start \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_sha1_link_ctx_s *ctx)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_link_start])

#define romapi_qcr373_sha256_link_start \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_sha256_link_ctx_s *ctx, int is224)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha256_link_start])

#define romapi_qcr373_sha512_link_start \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_sha512_link_ctx_s *ctx, int is384)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha512_link_start])

#define romapi_qcr373_sha1_link_update \
    ((int (*)(struct qcr373_device_s *dev, struct qcr373_sha1_link_ctx_s *ctx, const uint8_t *input, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_link_update])

#define romapi_qcr373_sha256_link_update \
    ((int (*)(struct qcr373_device_s *dev, struct qcr373_sha256_link_ctx_s *ctx, const uint8_t *input, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha256_link_update])

#define romapi_qcr373_sha512_link_update \
    ((int (*)(struct qcr373_device_s *dev, struct qcr373_sha512_link_ctx_s *ctx, const uint8_t *input, uint64_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha512_link_update])

#define romapi_qcr373_sha1_link_finish \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_sha1_link_ctx_s *ctx, uint8_t *output)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha1_link_finish])

#define romapi_qcr373_sha256_link_finish \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_sha256_link_ctx_s *ctx, uint8_t *output)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha256_link_finish])

#define romapi_qcr373_sha512_link_finish \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_sha512_link_ctx_s *ctx, uint8_t *output)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha512_link_finish])

#define romapi_qcr373_group0_request_sha_access \
    ((int (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_group0_request_sha_access])

#define romapi_qcr373_group1_request_sha_access \
    ((int (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_group1_request_sha_access])

#define romapi_qcr373_group0_release_sha_access \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_group0_release_sha_access])

#define romapi_qcr373_group1_release_sha_access \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_group1_release_sha_access])

#define romapi_qcr373_sha_crc32_link_work \
    ((int (*)(struct qcr373_device_s *dev, uint32_t addr, const uint8_t *in, uint32_t len, uint8_t *out)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sha_crc32_link_work])

#define romapi_qcr373_trng_read \
    ((int (*)(struct qcr373_device_s *dev, uint8_t data[32])) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_trng_read])

#define romapi_qcr373_trng_readlen \
    ((int (*)(uint8_t *data, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_trng_readlen])

#define romapi_qcr373_group0_request_trng_access \
    ((int (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_group0_request_trng_access])

#define romapi_qcr373_group1_request_trng_access \
    ((int (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_group1_request_trng_access])

#define romapi_qcr373_group0_release_trng_access \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_group0_release_trng_access])

#define romapi_qcr373_group1_release_trng_access \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_group1_release_trng_access])

#define romapi_qcr373_spi_init \
    ((void (*)(struct qcr373_device_s *dev, const struct qcr373_spi_config_s *config)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_init])

#define romapi_qcr373_spi_deinit \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_deinit])

#define romapi_qcr373_spi_link_txdma \
    ((void (*)(struct qcr373_device_s *dev, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_link_txdma])

#define romapi_qcr373_spi_link_rxdma \
    ((void (*)(struct qcr373_device_s *dev, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_link_rxdma])

#define romapi_qcr373_spi_poll_send \
    ((uint32_t (*)(struct qcr373_device_s *dev, uint32_t data)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_poll_send])

#define romapi_qcr373_spi_poll_exchange \
    ((int (*)(struct qcr373_device_s *dev, const void *txbuffer, void *rxbuffer, size_t nbytes)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_poll_exchange])

#define romapi_qcr373_spi_txint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_txint_mask])

#define romapi_qcr373_spi_rxint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_rxint_mask])

#define romapi_qcr373_spi_tcint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_tcint_mask])

#define romapi_qcr373_spi_rtoint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_rtoint_mask])

#define romapi_qcr373_spi_errint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_errint_mask])

#define romapi_qcr373_spi_get_intstatus \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_get_intstatus])

#define romapi_qcr373_spi_int_clear \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_clear)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_int_clear])

#define romapi_qcr373_spi_isbusy \
    ((bool (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_isbusy])

#define romapi_qcr373_spi_feature_control \
    ((int (*)(struct qcr373_device_s *dev, int cmd, size_t arg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_spi_feature_control])

#define romapi_qcr373_timer_init \
    ((void (*)(struct qcr373_device_s *dev, const struct qcr373_timer_config_s *config)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_init])

#define romapi_qcr373_timer_deinit \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_deinit])

#define romapi_qcr373_timer_start \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_start])

#define romapi_qcr373_timer_stop \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_stop])

#define romapi_qcr373_timer_set_preloadvalue \
    ((void (*)(struct qcr373_device_s *dev, uint32_t val)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_set_preloadvalue])

#define romapi_qcr373_timer_set_compvalue \
    ((void (*)(struct qcr373_device_s *dev, uint8_t cmp_no, uint32_t val)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_set_compvalue])

#define romapi_qcr373_timer_get_compvalue \
    ((uint32_t (*)(struct qcr373_device_s *dev, uint8_t cmp_no)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_get_compvalue])

#define romapi_qcr373_timer_get_countervalue \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_get_countervalue])

#define romapi_qcr373_timer_compint_mask \
    ((void (*)(struct qcr373_device_s *dev, uint8_t cmp_no, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_compint_mask])

#define romapi_qcr373_timer_get_compint_status \
    ((bool (*)(struct qcr373_device_s *dev, uint8_t cmp_no)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_get_compint_status])

#define romapi_qcr373_timer_compint_clear \
    ((void (*)(struct qcr373_device_s *dev, uint8_t cmp_no)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_compint_clear])

#define romapi_qcr373_timer_capture_init \
    ((void (*)(struct qcr373_device_s *dev, const struct qcr373_timer_capture_config_s *config)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_timer_capture_init])

#define romapi_qcr373_uart_init \
    ((void (*)(struct qcr373_device_s *dev, const struct qcr373_uart_config_s *config)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_init])

#define romapi_qcr373_uart_deinit \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_deinit])

#define romapi_qcr373_uart_enable \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_enable])

#define romapi_qcr373_uart_disable \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_disable])

#define romapi_qcr373_uart_link_txdma \
    ((void (*)(struct qcr373_device_s *dev, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_link_txdma])

#define romapi_qcr373_uart_link_rxdma \
    ((void (*)(struct qcr373_device_s *dev, bool enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_link_rxdma])

#define romapi_qcr373_uart_putchar \
    ((int (*)(struct qcr373_device_s *dev, int ch)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_putchar])

#define romapi_qcr373_uart_getchar \
    ((int (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_getchar])

#define romapi_qcr373_uart_put \
    ((int (*)(struct qcr373_device_s *dev, uint8_t *data, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_put])

#define romapi_qcr373_uart_put_block \
    ((int (*)(struct qcr373_device_s *dev, uint8_t *data, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_put_block])

#define romapi_qcr373_uart_get \
    ((int (*)(struct qcr373_device_s *dev, uint8_t *data, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_get])

#define romapi_qcr373_uart_wait_tx_done \
    ((int (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_wait_tx_done])

#define romapi_qcr373_uart_txready \
    ((bool (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_txready])

#define romapi_qcr373_uart_txempty \
    ((bool (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_txempty])

#define romapi_qcr373_uart_rxavailable \
    ((bool (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_rxavailable])

#define romapi_qcr373_uart_txint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_txint_mask])

#define romapi_qcr373_uart_rxint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_rxint_mask])

#define romapi_qcr373_uart_errint_mask \
    ((void (*)(struct qcr373_device_s *dev, bool mask)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_errint_mask])

#define romapi_qcr373_uart_get_intstatus \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_get_intstatus])

#define romapi_qcr373_uart_int_clear \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_clear)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_int_clear])

#define romapi_qcr373_uart_feature_control \
    ((int (*)(struct qcr373_device_s *dev, int cmd, size_t arg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_uart_feature_control])

#define romapi_qcr373_wdg_init \
    ((void (*)(struct qcr373_device_s *dev, const struct qcr373_wdg_config_s *config)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_init])

#define romapi_qcr373_wdg_start \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_start])

#define romapi_qcr373_wdg_stop \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_stop])

#define romapi_qcr373_wdg_get_countervalue \
    ((uint16_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_get_countervalue])

#define romapi_qcr373_wdg_set_countervalue \
    ((void (*)(struct qcr373_device_s *dev, uint16_t value)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_set_countervalue])

#define romapi_qcr373_wdg_reset_countervalue \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_reset_countervalue])

#define romapi_qcr373_wdg_compint_clear \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wdg_compint_clear])

#define romapi_qcr373_wo_pin_init \
    ((void (*)(struct qcr373_device_s *dev, uint8_t pin, uint8_t mode)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_pin_init])

#define romapi_qcr373_wo_init \
    ((void (*)(struct qcr373_device_s *dev, struct qcr373_wo_cfg_s *cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_init])

#define romapi_qcr373_wo_enable \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_enable])

#define romapi_qcr373_wo_disable \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_disable])

#define romapi_qcr373_wo_get_fifo_available_cnt \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_get_fifo_available_cnt])

#define romapi_qcr373_wo_push_fifo \
    ((uint32_t (*)(struct qcr373_device_s *dev, uint16_t *data, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_push_fifo])

#define romapi_qcr373_wo_push_fifo_force \
    ((void (*)(struct qcr373_device_s *dev, uint16_t *data, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_push_fifo_force])

#define romapi_qcr373_wo_clear_fifo \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_clear_fifo])

#define romapi_qcr373_wo_enable_dma \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_enable_dma])

#define romapi_qcr373_wo_disable_dma \
    ((void (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_disable_dma])

#define romapi_qcr373_wo_get_int_status \
    ((uint32_t (*)(struct qcr373_device_s *dev)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_get_int_status])

#define romapi_qcr373_wo_int_mask \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_type)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_int_mask])

#define romapi_qcr373_wo_int_unmask \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_type)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_int_unmask])

#define romapi_qcr373_wo_int_clear \
    ((void (*)(struct qcr373_device_s *dev, uint32_t int_type)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_wo_int_clear])

#define romapi_qcr373_flash_secreg_callapi_before \
    ((int (*)(const spi_flash_cfg_type *flash_cfg, struct callapi_content *content)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_callapi_before])

#define romapi_qcr373_flash_secreg_callapi_after \
    ((void (*)(const spi_flash_cfg_type *flash_cfg, struct callapi_content *content)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_callapi_after])

#define romapi_qcr373_flash_secreg_get_param \
    ((int (*)(uint32_t jid, const qcr373_flash_secreg_param_t **param)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_get_param])

#define romapi_qcr373_flash_secreg_region_foreach \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, region_cb cb, void *data)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_region_foreach])

#define romapi_qcr373_flash_secreg_get_lockbits \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint8_t *lockbits)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_get_lockbits])

#define romapi_qcr373_flash_secreg_set_lockbits \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint8_t lockbits)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_set_lockbits])

#define romapi_qcr373_flash_secreg_get_locked \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint8_t index)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_get_locked])

#define romapi_qcr373_flash_secreg_set_locked \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint8_t index)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_set_locked])

#define romapi_qcr373_flash_secreg_get_info_by_idx \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint8_t index, qcr373_flash_secreg_region_info_t *info)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_get_info_by_idx])

#define romapi_qcr373_flash_secreg_valid_by_idx \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint8_t index, uint32_t offset, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_valid_by_idx])

#define romapi_qcr373_flash_secreg_read_by_idx \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint8_t index, uint32_t offset, void *data, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_read_by_idx])

#define romapi_qcr373_flash_secreg_write_by_idx \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint8_t index, uint32_t offset, const void *data, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_write_by_idx])

#define romapi_qcr373_flash_secreg_erase_by_idx \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint8_t index)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_erase_by_idx])

#define romapi_qcr373_flash_secreg_get_info \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint32_t addr, qcr373_flash_secreg_region_info_t *info)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_get_info])

#define romapi_qcr373_flash_secreg_valid \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint32_t addr, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_valid])

#define romapi_qcr373_flash_secreg_read \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint32_t addr, void *data, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_read])

#define romapi_qcr373_flash_secreg_write \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint32_t addr, const void *data, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_write])

#define romapi_qcr373_flash_secreg_erase \
    ((int (*)(qcr373_flash_otp_config_t *otp_cfg, uint32_t addr, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_flash_secreg_erase])

#define romapi_qcr373_sf_cfg_init_ext_flash_gpio \
    ((int (*)(uint8_t ext_flash_pin)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_init_ext_flash_gpio])

#define romapi_qcr373_sf_cfg_deinit_ext_flash_gpio \
    ((int (*)(uint8_t ext_flash_pin)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_deinit_ext_flash_gpio])

#define romapi_qcr373_sf_cfg_init_flash_gpio \
    ((int (*)(uint8_t flash_pin_cfg, uint8_t restore_default)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_init_flash_gpio])

#define romapi_qcr373_sf_cfg_init_flash2_gpio \
    ((int (*)(uint8_t swap)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_init_flash2_gpio])

#define romapi_qcr373_sf_cfg_flash_identify \
    ((uint32_t (*)(uint8_t call_from_flash, uint8_t flash_pin_cfg, uint8_t restore_default, spi_flash_cfg_type *p_flash_cfg, uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_flash_identify])

#define romapi_qcr373_sf_cfg_get_flash_cfg_need_lock \
    ((int (*)(uint32_t flash_id, spi_flash_cfg_type *p_flash_cfg, uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_get_flash_cfg_need_lock])

#define romapi_qcr373_sf_cfg_flash_init \
    ((int (*)(uint8_t sel, const struct sf_ctrl_cfg_type *p_sf_ctrl_cfg, const struct sf_ctrl_bank2_cfg *p_bank2_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_flash_init])

#define romapi_qcr373_sf_cfg_sbus2_flash_init \
    ((int (*)(uint8_t sel, const struct sf_ctrl_bank2_cfg *p_bank2_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_cfg_sbus2_flash_init])

#define romapi_qcr373_sf_ctrl_enable \
    ((void (*)(const struct sf_ctrl_cfg_type *cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_enable])

#define romapi_qcr373_sf_ctrl_set_io_delay \
    ((void (*)(uint8_t pad, uint8_t do_delay, uint8_t di_delay, uint8_t oe_delay)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_io_delay])

#define romapi_qcr373_sf_ctrl_get_io_delay \
    ((void (*)(uint8_t pad, uint8_t *do_delay, uint8_t *di_delay, uint8_t *oe_delay)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_io_delay])

#define romapi_qcr373_sf_ctrl_set_cs_clk_delay \
    ((void (*)(uint8_t pad, uint8_t cs_delay, uint8_t clk_delay)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_cs_clk_delay])

#define romapi_qcr373_sf_ctrl_get_cs_clk_delay \
    ((void (*)(uint8_t pad, uint8_t *cs_delay, uint8_t *clk_delay)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_cs_clk_delay])

#define romapi_qcr373_sf_ctrl_set_flash_io_cs_clk_delay \
    ((void (*)(struct qcr373_sf_ctrl_io_cs_clk_delay_cfg cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_flash_io_cs_clk_delay])

#define romapi_qcr373_sf_ctrl_get_flash_io_cs_clk_delay \
    ((void (*)(struct qcr373_sf_ctrl_io_cs_clk_delay_cfg *cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_flash_io_cs_clk_delay])

#define romapi_qcr373_sf_ctrl_set_dqs_delay \
    ((void (*)(uint8_t pad, uint8_t dodelay, uint8_t didelay, uint8_t oedelay)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_dqs_delay])

#define romapi_qcr373_sf_ctrl_bank2_enable \
    ((void (*)(const struct sf_ctrl_bank2_cfg *bank2_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_bank2_enable])

#define romapi_qcr373_sf_ctrl_sbus2_hold_sram \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sbus2_hold_sram])

#define romapi_qcr373_sf_ctrl_sbus2_release_sram \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sbus2_release_sram])

#define romapi_qcr373_sf_ctrl_sbus2_replace \
    ((void (*)(uint8_t pad)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sbus2_replace])

#define romapi_qcr373_sf_ctrl_sbus2_revoke_replace \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sbus2_revoke_replace])

#define romapi_qcr373_sf_ctrl_sbus2_set_delay \
    ((void (*)(uint8_t clk_delay, uint8_t rx_clk_invert)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sbus2_set_delay])

#define romapi_qcr373_sf_ctrl_remap_set \
    ((void (*)(uint8_t remap, uint8_t lock)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_remap_set])

#define romapi_qcr373_sf_ctrl_32bits_addr_en \
    ((void (*)(uint8_t en32_bits_addr)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_32bits_addr_en])

#define romapi_qcr373_sf_ctrl_get_clock_delay \
    ((uint8_t (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_clock_delay])

#define romapi_qcr373_sf_ctrl_set_clock_delay \
    ((void (*)(uint8_t delay)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_clock_delay])

#define romapi_qcr373_sf_ctrl_get_wrap_queue_value \
    ((uint8_t (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_wrap_queue_value])

#define romapi_qcr373_sf_ctrl_cmds_set \
    ((void (*)(struct sf_ctrl_cmds_cfg *cmds_cfg, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_cmds_set])

#define romapi_qcr373_sf_ctrl_select_pad \
    ((void (*)(uint8_t sel)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_select_pad])

#define romapi_qcr373_sf_ctrl_sbus_select_bank \
    ((void (*)(uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sbus_select_bank])

#define romapi_qcr373_sf_ctrl_set_owner \
    ((void (*)(uint8_t owner)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_owner])

#define romapi_qcr373_sf_ctrl_get_owner \
    ((uint8_t (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_owner])

#define romapi_qcr373_sf_ctrl_set_owner_flag \
    ((void (*)(uint8_t owner)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_owner_flag])

#define romapi_qcr373_sf_ctrl_get_owner_flag \
    ((uint8_t (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_owner_flag])

#define romapi_qcr373_sf_ctrl_disable \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_disable])

#define romapi_qcr373_sf_ctrl_aes_enable_be \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_enable_be])

#define romapi_qcr373_sf_ctrl_aes_enable_le \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_enable_le])

#define romapi_qcr373_sf_ctrl_aes_set_region \
    ((void (*)(uint8_t region, uint8_t enable, uint8_t hw_key, uint32_t start_addr, uint32_t end_addr, uint8_t locked)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_region])

#define romapi_qcr373_sf_ctrl_aes_set_key \
    ((void (*)(uint8_t region, uint8_t *key, uint8_t key_type)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_key])

#define romapi_qcr373_sf_ctrl_aes_set_key_be \
    ((void (*)(uint8_t region, uint8_t *key, uint8_t key_type)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_key_be])

#define romapi_qcr373_sf_ctrl_aes_set_iv \
    ((void (*)(uint8_t region, uint8_t *iv, uint32_t addr_offset)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_iv])

#define romapi_qcr373_sf_ctrl_aes_set_iv_be \
    ((void (*)(uint8_t region, uint8_t *iv, uint32_t addr_offset)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_iv_be])

#define romapi_qcr373_sf_ctrl_aes_set_region_offset \
    ((void (*)(uint8_t region, uint32_t addr_offset)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_region_offset])

#define romapi_qcr373_sf_ctrl_aes_xts_set_key \
    ((void (*)(uint8_t region, uint8_t *key, uint8_t key_type)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_xts_set_key])

#define romapi_qcr373_sf_ctrl_aes_xts_set_key_be \
    ((void (*)(uint8_t region, uint8_t *key, uint8_t key_type)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_xts_set_key_be])

#define romapi_qcr373_sf_ctrl_aes_xts_set_iv \
    ((void (*)(uint8_t region, uint8_t *iv, uint32_t addr_offset)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_xts_set_iv])

#define romapi_qcr373_sf_ctrl_aes_xts_set_iv_be \
    ((void (*)(uint8_t region, uint8_t *iv, uint32_t addr_offset)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_xts_set_iv_be])

#define romapi_qcr373_sf_ctrl_aes_set_mode \
    ((void (*)(uint8_t mode)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_mode])

#define romapi_qcr373_sf_ctrl_aes_enable \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_enable])

#define romapi_qcr373_sf_ctrl_aes_disable \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_disable])

#define romapi_qcr373_sf_ctrl_is_aes_enable \
    ((uint8_t (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_is_aes_enable])

#define romapi_qcr373_sf_ctrl_set_flash_image_offset \
    ((void (*)(uint32_t addr_offset, uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_set_flash_image_offset])

#define romapi_qcr373_sf_ctrl_get_flash_image_offset \
    ((uint32_t (*)(uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_flash_image_offset])

#define romapi_qcr373_sf_ctrl_lock_flash_image_offset \
    ((void (*)(uint8_t lock)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_lock_flash_image_offset])

#define romapi_qcr373_sf_ctrl_select_clock \
    ((void (*)(uint8_t sahb_sram_sel)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_select_clock])

#define romapi_qcr373_sf_ctrl_sendcmd \
    ((void (*)(struct sf_ctrl_cmd_cfg_type *cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_sendcmd])

#define romapi_qcr373_sf_ctrl_disable_wrap_access \
    ((void (*)(uint8_t disable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_disable_wrap_access])

#define romapi_qcr373_sf_ctrl_xip_set \
    ((void (*)(struct sf_ctrl_cmd_cfg_type *cfg, uint8_t cmd_valid)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_xip_set])

#define romapi_qcr373_sf_ctrl_get_busy_state \
    ((uint8_t (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_get_busy_state])

#define romapi_qcr373_sf_ctrl_aes_get_iv_be \
    ((void (*)(uint8_t region, uint8_t *iv)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_get_iv_be])

#define romapi_qcr373_sf_ctrl_aes_get_iv_le \
    ((void (*)(uint8_t region, uint8_t *iv)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_get_iv_le])

#define romapi_qcr373_sf_ctrl_aes_set_decrypt_region_be \
    ((int32_t (*)(struct qcr373_sf_ctrl_decrypt_type *parm)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_decrypt_region_be])

#define romapi_qcr373_sf_ctrl_aes_set_decrypt_region_le \
    ((int32_t (*)(struct qcr373_sf_ctrl_decrypt_type *parm)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sf_ctrl_aes_set_decrypt_region_le])

#define romapi_qcr373_sflash_init \
    ((void (*)(const struct sf_ctrl_cfg_type *p_sf_ctrl_cfg, const struct sf_ctrl_bank2_cfg *p_bank2_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_init])

#define romapi_qcr373_sflash_set_spi_mode \
    ((int (*)(uint8_t mode)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_set_spi_mode])

#define romapi_qcr373_sflash_read_reg \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint8_t reg_index, uint8_t *reg_value, uint8_t reg_len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_read_reg])

#define romapi_qcr373_sflash_write_reg \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint8_t reg_index, uint8_t *reg_value, uint8_t reg_len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_write_reg])

#define romapi_qcr373_sflash_read_reg_with_cmd \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint8_t read_reg_cmd, uint8_t *reg_value, uint8_t reg_len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_read_reg_with_cmd])

#define romapi_qcr373_sflash_write_reg_with_cmd \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint8_t write_reg_cmd, uint8_t *reg_value, uint8_t reg_len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_write_reg_with_cmd])

#define romapi_qcr373_sflash_busy \
    ((int (*)(spi_flash_cfg_type *flash_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_busy])

#define romapi_qcr373_sflash_write_enable \
    ((int (*)(spi_flash_cfg_type *flash_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_write_enable])

#define romapi_qcr373_sflash_qspi_enable \
    ((int (*)(spi_flash_cfg_type *flash_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_qspi_enable])

#define romapi_qcr373_sflash_qspi_disable \
    ((int (*)(spi_flash_cfg_type *flash_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_qspi_disable])

#define romapi_qcr373_sflash_volatile_reg_write_enable \
    ((void (*)(spi_flash_cfg_type *flash_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_volatile_reg_write_enable])

#define romapi_qcr373_sflash_chip_erase \
    ((int (*)(spi_flash_cfg_type *flash_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_chip_erase])

#define romapi_qcr373_sflash_sector_erase \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint32_t sector_num)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_sector_erase])

#define romapi_qcr373_sflash_blk32_erase \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint32_t blk_num)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_blk32_erase])

#define romapi_qcr373_sflash_blk64_erase \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint32_t blk_num)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_blk64_erase])

#define romapi_qcr373_sflash_erase \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint32_t start_addr, uint32_t end_addr)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_erase])

#define romapi_qcr373_sflash_get_uniqueid \
    ((void (*)(uint8_t *data, uint8_t id_len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_get_uniqueid])

#define romapi_qcr373_sflash_get_jedecid \
    ((void (*)(spi_flash_cfg_type *flash_cfg, uint8_t *data)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_get_jedecid])

#define romapi_qcr373_sflash_get_deviceid \
    ((void (*)(uint8_t *data, uint8_t is_32bits_addr)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_get_deviceid])

#define romapi_qcr373_sflash_powerdown \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_powerdown])

#define romapi_qcr373_sflash_release_powerdown \
    ((void (*)(spi_flash_cfg_type *flash_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_release_powerdown])

#define romapi_qcr373_sflash_restore_from_powerdown \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint8_t flash_cont_read, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_restore_from_powerdown])

#define romapi_qcr373_sflash_set_burst_wrap \
    ((void (*)(spi_flash_cfg_type *flash_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_set_burst_wrap])

#define romapi_qcr373_sflash_disable_burst_wrap \
    ((void (*)(spi_flash_cfg_type *flash_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_disable_burst_wrap])

#define romapi_qcr373_sflash_set_32bits_addr_mode \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint8_t en_32bits_addr)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_set_32bits_addr_mode])

#define romapi_qcr373_sflash_software_reset \
    ((int (*)(spi_flash_cfg_type *flash_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_software_reset])

#define romapi_qcr373_sflash_reset_continue_read \
    ((void (*)(spi_flash_cfg_type *flash_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_reset_continue_read])

#define romapi_qcr373_sflash_set_xip_cfg \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint8_t io_mode, uint8_t cont_read, uint32_t addr, uint32_t len, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_set_xip_cfg])

#define romapi_qcr373_sflash_xip_read_enable \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint8_t io_mode, uint8_t cont_read, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_xip_read_enable])

#define romapi_qcr373_sflash_xip_read_disable \
    ((void (*)(void)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_xip_read_disable])

#define romapi_qcr373_sflash_rcv_enable \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint8_t r_cmd, uint8_t w_cmd, uint8_t bit_pos)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_rcv_enable])

#define romapi_qcr373_sflash_erase_security_register \
    ((int (*)(spi_flash_cfg_type *flash_cfg, struct sflash_sec_reg_cfg *p_sec_reg_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_erase_security_register])

#define romapi_qcr373_sflash_program_security_register \
    ((int (*)(spi_flash_cfg_type *flash_cfg, struct sflash_sec_reg_cfg *p_sec_reg_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_program_security_register])

#define romapi_qcr373_sflash_read_security_register \
    ((int (*)(struct sflash_sec_reg_cfg *p_sec_reg_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_read_security_register])

#define romapi_qcr373_sflash_clear_status_register \
    ((int (*)(spi_flash_cfg_type *flash_cfg)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_clear_status_register])

#define romapi_qcr373_sflash_read \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint8_t io_mode, uint8_t cont_read, uint32_t addr, uint8_t *data, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_read])

#define romapi_qcr373_sflash_program \
    ((int (*)(spi_flash_cfg_type *flash_cfg, uint8_t io_mode, uint32_t addr, uint8_t *data, uint32_t len)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_sflash_program])

#define romapi_qcr373_xip_sflash_state_save \
    ((int (*)(spi_flash_cfg_type *p_flash_cfg, uint32_t *offset, uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_state_save])

#define romapi_qcr373_xip_sflash_state_restore \
    ((int (*)(spi_flash_cfg_type *p_flash_cfg, uint32_t offset, uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_state_restore])

#define romapi_qcr373_xip_sflash_erase_need_lock \
    ((int (*)(spi_flash_cfg_type *p_flash_cfg, uint32_t start_addr, int len, uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_erase_need_lock])

#define romapi_qcr373_xip_sflash_write_need_lock \
    ((int (*)(spi_flash_cfg_type *p_flash_cfg, uint32_t addr, uint8_t *data, uint32_t len, uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_write_need_lock])

#define romapi_qcr373_xip_sflash_read_need_lock \
    ((int (*)(spi_flash_cfg_type *p_flash_cfg, uint32_t addr, uint8_t *data, uint32_t len, uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_read_need_lock])

#define romapi_qcr373_xip_sflash_get_jedecid_need_lock \
    ((int (*)(spi_flash_cfg_type *p_flash_cfg, uint8_t *data, uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_get_jedecid_need_lock])

#define romapi_qcr373_xip_sflash_get_deviceid_need_lock \
    ((int (*)(spi_flash_cfg_type *p_flash_cfg, uint8_t is_32bits_addr, uint8_t *data, uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_get_deviceid_need_lock])

#define romapi_qcr373_xip_sflash_get_uniqueid_need_lock \
    ((int (*)(spi_flash_cfg_type *p_flash_cfg, uint8_t *data, uint8_t id_len, uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_get_uniqueid_need_lock])

#define romapi_qcr373_xip_sflash_clear_status_register_need_lock \
    ((int (*)(spi_flash_cfg_type *p_flash_cfg, uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_clear_status_register_need_lock])

#define romapi_qcr373_xip_sflash_read_via_cache_need_lock \
    ((int (*)(uint32_t addr, uint8_t *data, uint32_t len, uint8_t group, uint8_t bank)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_read_via_cache_need_lock])

#define romapi_qcr373_xip_sflash_opt_enter \
    ((void (*)(uint8_t *aes_enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_opt_enter])

#define romapi_qcr373_xip_sflash_opt_exit \
    ((void (*)(uint8_t aes_enable)) \
    ROM_APITABLE[QCR373_LHAL_ROMAPI_INDEX_qcr373_xip_sflash_opt_exit])

#endif
