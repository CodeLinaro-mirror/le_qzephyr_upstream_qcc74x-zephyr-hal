# Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_WIFI)

  # mfg_api
  if(NOT CONFIG_SOC_SERIES_QCC75X)
  zephyr_sources(
    ../../../drivers/soc/qcc74x/std/src/qcc74x_mfg_efuse.c
    ../../../drivers/soc/qcc74x/std/src/qcc74x_mfg_flash.c
    ../../../drivers/soc/qcc74x/std/src/qcc74x_mfg_media.c
  )
  endif()

  # mbedtls
  zephyr_include_directories(
    ../../../../../crypto/mbedtls/include
  )

  zephyr_sources(
    ../../../drivers/rfparam/Inc
  )

  # wireless
  zephyr_include_directories(include)

endif()

if(CONFIG_WIFI)
  add_subdirectory(macsw_lib)
  add_subdirectory(wifi6)
  add_subdirectory(socport_zephyr)
endif()

if(CONFIG_BT)
  if(CONFIG_SOC_SERIES_QCC75X)
    message(FATAL_ERROR "QCC75x Bluetooth is not provided by qcc74x wireless")
  endif()

  zephyr_sources_ifdef(CONFIG_BT
    bluetooth/btblecontroller/zephyr_port/qcc74x_btblecontroller.c
  )

  zephyr_include_directories(bluetooth/btblecontroller)
  zephyr_link_libraries(
    btblecontroller
    -L${QCC74X_BLOB_BASE}/lib/
  )
endif()
