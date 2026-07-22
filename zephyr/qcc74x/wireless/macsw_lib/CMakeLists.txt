set(QCC74X_MACSW_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})

zephyr_include_directories(include)
zephyr_include_directories(include/export)
zephyr_include_directories(include/export/mac)

if(CONFIG_QCC74X_PM_PDS15 OR CONFIG_QCC74X_WIFI_LOW_POWER)
  set(QCC74X_MACSW_PREBUILT_DIR ${QCC74X_BLOB_BASE}/lib/lp)
else()
  set(QCC74X_MACSW_PREBUILT_DIR ${QCC74X_BLOB_BASE}/lib/default)
endif()

set(QCC74X_MACSW_PREBUILT_LIB ${QCC74X_MACSW_PREBUILT_DIR}/libqcc743_wifi_macsw.a)

if(NOT EXISTS ${QCC74X_MACSW_PREBUILT_LIB})
  message(FATAL_ERROR "Missing QCC74x macsw prebuilt library: ${QCC74X_MACSW_PREBUILT_LIB}\n"
    "Run: west blobs fetch hal_qcom")
endif()

zephyr_library_import(qcc743_wifi_macsw_prebuilt ${QCC74X_MACSW_PREBUILT_LIB})
