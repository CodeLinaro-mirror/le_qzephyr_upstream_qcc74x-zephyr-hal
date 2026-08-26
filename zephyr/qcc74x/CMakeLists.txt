# Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_SOC_SERIES_QCC74X)
  zephyr_compile_options(-fstrict-volatile-bitfields)
  zephyr_compile_definitions_ifndef(asm asm=__asm__)
  zephyr_compile_definitions(CONFIG_IRQ_NUM=80)
  zephyr_compile_definitions(ARCH_RISCV)
  zephyr_compile_definitions(QCC74X)
  zephyr_compile_definitions(QC7XX_SPI_IDEL_DATA=0)
  zephyr_compile_definitions(QC7XX_USE_ROM_DRIVER)
  zephyr_compile_definitions(NOT_USE_QC7XX_LHAL_IRQ_ATTACH)
  zephyr_compile_definitions(RFPARAM_QCC74X)
  zephyr_compile_definitions(CONFIG_RFPARAM_TLV_FIXED_ADDRESS)

  zephyr_include_directories(
    include
    ../port/include
    ../common/include

    ../../drivers/lhal/include
    ../../drivers/lhal/include/arch
    ../../drivers/lhal/include/arch/risc-v/t-head
    ../../drivers/lhal/include/arch/risc-v/t-head/Core/Include
    ../../drivers/lhal/include/arch/risc-v/t-head/csi_dsp/include
    ../../drivers/lhal/config/qcc74x
    ../../drivers/lhal/src/flash
    ../../drivers/soc/qcc74x/std/include
    ../../drivers/soc/qcc74x/std/include/hardware
    ../../drivers/sys
    ../../drivers/sys/qcc74x
    ../../drivers/rfparam/Inc
    ../../drivers/soc/qcc74x/phyrf/include
  )

  zephyr_sources(
    ../../drivers/lhal/include/arch/risc-v/t-head/rv_hart.c
    ../../drivers/lhal/include/arch/risc-v/t-head/rv_pmp.c
    ../../drivers/lhal/config/qcc74x/device_table.c
    ../../drivers/lhal/src/qc7xx_common.c
    ../../drivers/lhal/src/qc7xx_gpio.c
    ../../drivers/lhal/src/qc7xx_uart.c
    ../../drivers/lhal/src/qc7xx_i2c.c
    ../../drivers/lhal/src/qc7xx_spi.c
    ../../drivers/lhal/src/qc7xx_i2s.c
    ../../drivers/lhal/src/qc7xx_mtimer.c
    ../../drivers/lhal/src/qc7xx_timer.c
    ../../drivers/lhal/src/qc7xx_dma.c
    ../../drivers/lhal/src/qc7xx_cam.c
    ../../drivers/lhal/src/qc7xx_dbi.c
    ../../drivers/lhal/src/qc7xx_l1c.c
    ../../drivers/lhal/src/qc7xx_pwm_v2.c
    ../../drivers/lhal/src/qc7xx_flash.c
    ../../drivers/lhal/src/flash/qc7xx_xip_sflash.c
    ../../drivers/lhal/src/flash/qc7xx_sflash.c
    ../../drivers/lhal/src/flash/qc7xx_sf_ctrl.c
    ../../drivers/lhal/src/flash/qc7xx_sf_cfg.c
    ../../drivers/lhal/src/qc7xx_ef_ctrl.c
    ../../drivers/lhal/src/qc7xx_sdh.c
    ../../drivers/lhal/src/qc7xx_sec_trng.c
    ../../drivers/lhal/src/qc7xx_irq.c
    ../../drivers/lhal/src/qc7xx_emac.c
    ../../drivers/lhal/src/qc7xx_wdg.c
    ../../drivers/lhal/src/qc7xx_adc.c
    ../../drivers/lhal/src/qc7xx_rtc.c
    ../../drivers/lhal/src/qc7xx_sec_sha.c
    ../../drivers/lhal/src/qc7xx_sec_aes.c
    ../../drivers/lhal/src/qc7xx_sec_pka.c
    ../../drivers/lhal/src/qc7xx_clock.c
  )
  # Audio ADC/DAC lhal sources: only needed when the audio drivers are built.
  zephyr_sources_ifdef(CONFIG_AUDIO_ADC_QCC74X ../../drivers/lhal/src/qc7xx_auadc.c)
  zephyr_sources_ifdef(CONFIG_AUDIO_DAC_QCC74X ../../drivers/lhal/src/qc7xx_audac.c)
  zephyr_sources_ifdef(CONFIG_VIDEO_QCC74X_JPEG ../../drivers/lhal/src/qc7xx_mjpeg.c)
if(CONFIG_ROMAPI)
  zephyr_sources(
    ../../drivers/soc/qcc74x/std/src/qcc74x_romapi_e907.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_romapi_patch.c
  )
else()
  zephyr_sources(
    ../../drivers/soc/qcc74x/std/src/qcc74x_glb.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_glb_gpio.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_hbn.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_aon.c
  )
endif()
  zephyr_sources(
    ../../drivers/soc/qcc74x/std/src/qcc74x_clock.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_common.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_ef_cfg.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_tzc_sec.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_psram.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_pm.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_sec_dbg.c
    ../../drivers/soc/qcc74x/std/port/qcc74x_gpio.c
    ../../drivers/soc/qcc74x/std/port/qcc74x_clock.c
    ../../drivers/rfparam/Src/rfparam_adapter.c
    ../../drivers/rfparam/Src/rfparam_rftlv.c

    )
  if(CONFIG_WIFI OR CONFIG_BT OR CONFIG_POWEROFF)
    zephyr_sources(
      ../../drivers/sys/qcc74x/qcc74x_sys.c
    )
  endif()
  zephyr_sources_ifdef( CONFIG_WIFI
    ../../drivers/soc/qcc74x/std/src/qcc74x_mfg_media.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_mfg_efuse.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_mfg_flash.c
    ../../bsp/board/qcc74xdk/wireless_regdb.c
  )
  zephyr_sources_ifdef( CONFIG_BT
    ../../drivers/soc/qcc74x/std/src/qcc74x_mfg_media.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_mfg_efuse.c
    ../../drivers/soc/qcc74x/std/src/qcc74x_mfg_flash.c
  )

if(CONFIG_WIFI OR CONFIG_BT)
  add_subdirectory(wireless)

  # phyrf
  zephyr_include_directories(../../drivers/soc/qcc74x/phyrf/include)
  zephyr_link_libraries(
    qcc743_phyrf
    -L${QCC74X_BLOB_BASE}/lib/
  )
endif()

set(LLEXT_APPEND_FLAGS
  -mabi=ilp32f
  -march=rv32imafcpzpsfoperand_xtheade
)

list(APPEND TOOLCHAIN_C_FLAGS -mabi=ilp32f -march=rv32imafcpzpsfoperand_xtheade)
list(APPEND TOOLCHAIN_LD_FLAGS NO_SPLIT -mabi=ilp32f -march=rv32imafcpzpsfoperand_xtheade)
endif()
