# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.20)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
if(NOT DEFINED CROSS_COMPILE)
  message(FATAL_ERROR "Set CROSS_COMPILE to a T-Head E907 toolchain prefix")
endif()
set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_ASM_COMPILER ${CROSS_COMPILE}gcc)
project(qcc74x_lpfw C ASM)

get_filename_component(HAL ${CMAKE_CURRENT_LIST_DIR}/../../.. ABSOLUTE)
set(LP ${HAL}/tools/lpfw/qcc743_lp_fw)
set(STD ${HAL}/drivers/soc/qcc74x/std)
set(LHAL ${HAL}/drivers/lhal)
set(LOG_LEVEL 0 CACHE STRING "LPFW logging level")
set(LPFW_VERSION "qcc74x-pds1-abi2")
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/generated)
configure_file(${LP}/qcc743_lpfw_board/lpfw_version.S.in
  ${CMAKE_BINARY_DIR}/generated/lpfw_version.S @ONLY)

add_executable(lpfw.elf
  ${LP}/main.c ${LP}/lp_board.c ${LP}/lp_fw.c ${LP}/pds15.c ${LP}/pds1.c
  ${LP}/lpfw_wifi.c
  ${LP}/lpfw_printf.c ${LP}/vsnprintf_nano.c
  ${LP}/startup/startup.S ${LP}/startup/vectors.S ${LP}/startup/interrupt.c
  ${LP}/lp_drv/lp_uart.c ${LP}/lp_drv/lp_wdt.c ${LP}/lp_drv/lp_gpio.c
  ${LP}/lp_drv/qcc743_timer.c ${LP}/lp_drv/qcc743_uart.c
  ${LP}/macsw_port/wifi_rx.c ${LP}/macsw_port/source/hal_machw.c
  ${LP}/macsw_port/source/rwnx_platform.c ${LP}/macsw_port/source/rxl_hwdesc.c
  ${STD}/src/qcc74x_romapi_e907.c ${STD}/src/qcc74x_romapi_patch.c
  ${STD}/src/qcc74x_common.c ${STD}/src/qcc74x_clock.c ${STD}/src/qcc74x_pm.c
  ${STD}/src/qcc74x_tzc_sec.c
  ${HAL}/drivers/sys/qcc74x/qcc74x_lp.c
  ${LHAL}/src/qc7xx_irq.c ${LHAL}/src/qc7xx_l1c.c
  ${LHAL}/src/qc7xx_acomp.c ${LHAL}/src/qc7xx_mtimer.c
  ${LHAL}/src/flash/qc7xx_sf_ctrl.c ${LHAL}/src/flash/qc7xx_sflash.c
)
target_include_directories(lpfw.elf PRIVATE
  ${LP} ${LP}/startup ${LP}/lp_drv ${LP}/macsw_port/include
  ${STD}/include ${STD}/include/hardware
  ${LHAL}/include ${LHAL}/config/qcc74x ${LHAL}/src/flash
  ${LHAL}/include/arch ${LHAL}/include/arch/risc-v/t-head
  ${LHAL}/include/arch/risc-v/t-head/Core/Include
  ${HAL}/drivers/sys ${HAL}/drivers/sys/qcc74x ${CMAKE_BINARY_DIR}/generated
  ${HAL}/zephyr/qcc74x/include
  ${HAL}/zephyr/port/include ${HAL}/zephyr/common/include
  ${HAL}/drivers/soc/qcc74x/phyrf/include
  ${HAL}/zephyr/qcc74x/wireless/macsw/include/export/common
  ${HAL}/zephyr/qcc74x/wireless/macsw/include/export
)
target_link_libraries(lpfw.elf PRIVATE
  ${HAL}/drivers/soc/qcc74x/phyrf/lib-gcc_10.2.0-toolchain_V2.6.1/libqcc743_lpphyrf.a)
set_source_files_properties(${LHAL}/src/flash/qc7xx_sf_ctrl.c PROPERTIES
  INCLUDE_DIRECTORIES "${LHAL}/include")
# HAL uses QC7XX_WIFI_LP_FW; the retained LPFW assembly uses QCC74x_WIFI_LP_FW.
target_compile_definitions(lpfw.elf PRIVATE
  QCC74X ARCH_RISCV QC7XX_USE_ROM_DRIVER QC7XX_USE_HAL_DRIVER
  QC7XX_WIFI_LP_FW=1 QCC74x_WIFI_LP_FW=1 CONFIG_IRQ_NUM=80
  PM_PDS_FLASH_POWER_OFF=0 PM_PDS_PLL_POWER_OFF=0
  WL_API_RMEM_EN=1 LPFW_LOG_EN=${LOG_LEVEL} LPFW_WIFI_RF_DEBUG=0
  LPFW_SHELL_EN=0 LPFW_MFG_EN=0 LPFW_GPIO_DEBUG=0 LPFW_JTAG_EN=0
  SHARED_FUNC_EN=1 WL_NIC=0 UART_CLOCK_FREQ=40000000 TICKLESS_DEBUG=3
)
set(ARCH_FLAGS -march=rv32imafcpzpsfoperand_xtheade -mabi=ilp32f -mtune=e907)
target_compile_options(lpfw.elf PRIVATE ${ARCH_FLAGS} -Os -g3
  -ffunction-sections -fdata-sections -fno-common -fstrict-volatile-bitfields)
target_link_options(lpfw.elf PRIVATE ${ARCH_FLAGS} -nostartfiles
  -T${LP}/qcc743_lp_fw.ld -Wl,--gc-sections,-Map=${CMAKE_BINARY_DIR}/lpfw.map
  --specs=nano.specs --specs=nosys.specs)
set_property(TARGET lpfw.elf APPEND PROPERTY LINK_DEPENDS ${LP}/qcc743_lp_fw.ld)
add_custom_command(TARGET lpfw.elf POST_BUILD
  COMMAND ${CROSS_COMPILE}objcopy -O binary $<TARGET_FILE:lpfw.elf> ${CMAKE_BINARY_DIR}/lpfw.bin
  COMMAND ${CROSS_COMPILE}size $<TARGET_FILE:lpfw.elf>)
