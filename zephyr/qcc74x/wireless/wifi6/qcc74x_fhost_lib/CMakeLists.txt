set(QCC74X_FHOST_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})

zephyr_include_directories(include)

if((CONFIG_BOARD_QCC748M_EVK OR CONFIG_BOARD_QCC748M3_EVK)
   AND CONFIG_QCC74X_PM_PDS1)
  set(QCC74X_FHOST_PREBUILT_LIB
      ${QCC74X_BLOB_BASE}/lib/qcc748_lowpower/libqcc748_wifi_fhost.a)
elseif((CONFIG_BOARD_QCC748M_EVK OR CONFIG_BOARD_QCC748M3_EVK)
   AND CONFIG_QCC74X_WIFI_HIGH_PERFORMANCE)
  set(QCC74X_FHOST_PREBUILT_LIB
      ${QCC74X_BLOB_BASE}/lib/qcc748/libqcc748_wifi_fhost.a)
elseif(CONFIG_QCC74X_PM_LPFW OR CONFIG_QCC74X_WIFI_LOW_POWER)
  set(QCC74X_FHOST_PREBUILT_LIB
      ${QCC74X_BLOB_BASE}/lib/lp/libqcc743_wifi_fhost.a)
else()
  set(QCC74X_FHOST_PREBUILT_LIB
      ${QCC74X_BLOB_BASE}/lib/default/libqcc743_wifi_fhost.a)
endif()

if(NOT EXISTS ${QCC74X_FHOST_PREBUILT_LIB})
  message(FATAL_ERROR "Missing QCC74x fhost prebuilt library: ${QCC74X_FHOST_PREBUILT_LIB}")
endif()

zephyr_library_import(qcc74x_wifi_fhost_prebuilt ${QCC74X_FHOST_PREBUILT_LIB})
