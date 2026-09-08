# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_SOC_SERIES_QCR373)
  zephyr_compile_options(-fstrict-volatile-bitfields)
  zephyr_compile_definitions_ifndef(asm asm=__asm__)
  zephyr_compile_definitions(CONFIG_IRQ_NUM=117)
  zephyr_compile_definitions(ARCH_RISCV)
  zephyr_compile_definitions(QCR373)
  zephyr_compile_definitions(QC7XX_SPI_IDEL_DATA=0)
  zephyr_compile_definitions(QC7XX_USE_ROM_DRIVER)
  zephyr_compile_definitions(NOT_USE_QC7XX_LHAL_IRQ_ATTACH)
  zephyr_compile_definitions(RFPARAM_QCR373)
  zephyr_compile_definitions(CONFIG_RFPARAM_TLV_FIXED_ADDRESS)

  zephyr_include_directories(
    ../port/include
    ../common/include

    ../../drivers/lhal/include
    ../../drivers/lhal/include/arch
    ../../drivers/lhal/include/arch/risc-v/t-head
    ../../drivers/lhal/include/arch/risc-v/t-head/Core/Include
    ../../drivers/lhal/include/arch/risc-v/n900/Core/Include
    ../../drivers/lhal/include/arch/risc-v/t-head/csi_dsp/include
    ../../drivers/lhal/config/qcr373
    ../../drivers/lhal/src/flash
    ../../drivers/soc/qcr373/std/include
    ../../drivers/soc/qcr373/std/include/hardware
    ../../drivers/sys
    ../../drivers/rfparam/Inc
  )

  zephyr_sources(

    ../../drivers/lhal/config/qcr373/device_table.c
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
    ../../drivers/lhal/src/qc7xx_rv_privilege.c
    ../../drivers/lhal/src/qc7xx_emac.c
    ../../drivers/lhal/src/qc7xx_wdg.c
    ../../drivers/lhal/src/qc7xx_adc.c
    ../../drivers/lhal/src/qc7xx_rtc.c
    ../../drivers/lhal/src/qc7xx_sec_sha.c
    ../../drivers/lhal/src/qc7xx_sec_aes.c
    ../../drivers/lhal/src/qc7xx_sec_pka.c
    ../../drivers/lhal/src/qc7xx_clock.c
  )

  zephyr_sources_ifdef(CONFIG_AUDIO_ADC_QCC74X ../../drivers/lhal/src/qc7xx_auadc.c)
  zephyr_sources_ifdef(CONFIG_AUDIO_DAC_QCC74X ../../drivers/lhal/src/qc7xx_audac.c)
  zephyr_sources_ifdef(CONFIG_VIDEO_QCC74X_JPEG ../../drivers/lhal/src/qc7xx_mjpeg.c)

  # QCR373 SOC: follow bl's CONFIG_ROMAPI pattern
  if(CONFIG_ROMAPI)
    zephyr_sources(
      ../../drivers/soc/qcr373/std/src/qcr373_romapi_e907.c
      ../../drivers/soc/qcr373/std/src/qcr373_romapi_patch.c
    )
  else()
    zephyr_sources(
      ../../drivers/soc/qcr373/std/src/qcr373_glb.c
      ../../drivers/soc/qcr373/std/src/qcr373_hbn.c
      ../../drivers/soc/qcr373/std/src/qcr373_aon.c
      ../../drivers/soc/qcr373/std/src/qcr373_clock.c
      ../../drivers/soc/qcr373/std/src/qcr373_sec_dbg.c
      ../../drivers/soc/qcr373/std/src/qcr373_psram.c
      ../../drivers/soc/qcr373/std/src/qcr373_tzc_sec.c
      ../../drivers/soc/qcr373/std/src/qcr373_ef_cfg.c
      ../../drivers/soc/qcr373/std/src/qcr373_common.c
      ../../drivers/soc/qcr373/std/src/qcr373_pm.c
      ../../drivers/soc/qcr373/std/src/qcr373_pds.c
    )
  endif()

  zephyr_sources(
    ../../drivers/soc/qcr373/std/port/qcr373_clock.c
    ../../drivers/soc/qcr373/std/port/qcr373_reset.c
  )

  zephyr_sources_ifdef(CONFIG_WIFI
    ../../drivers/soc/qcr373/std/src/qcr373_mfg_media.c
    ../../drivers/soc/qcr373/std/src/qcr373_mfg_efuse.c
    ../../drivers/soc/qcr373/std/src/qcr373_mfg_flash.c
  )

  zephyr_sources_ifdef(CONFIG_BT
    ../../drivers/soc/qcr373/std/src/qcr373_mfg_media.c
    ../../drivers/soc/qcr373/std/src/qcr373_mfg_efuse.c
    ../../drivers/soc/qcr373/std/src/qcr373_mfg_flash.c
  )

  if(CONFIG_WIFI OR CONFIG_BT)
    add_subdirectory(wireless)
  endif()

  set(LLEXT_APPEND_FLAGS
    -mabi=ilp32d
    -march=rv32imafdc_zicsr_zifencei
  )

  # B0: Nuclei N900 core - standard RISC-V march (no T-Head extensions)
  # FR reference: CPU_MODEL=b0 uses MCPU=nuclei-900-series, MARCH=rv32imafdc_zicsr_zifencei
  # ABI kept as ilp32f for compatibility with Zephyr kernel (which uses ilp32f)
  list(APPEND TOOLCHAIN_C_FLAGS -mabi=ilp32d -march=rv32imafdc_zicsr_zifencei)
  list(APPEND TOOLCHAIN_LD_FLAGS NO_SPLIT -mabi=ilp32d -march=rv32imafdc_zicsr_zifencei)
endif()
