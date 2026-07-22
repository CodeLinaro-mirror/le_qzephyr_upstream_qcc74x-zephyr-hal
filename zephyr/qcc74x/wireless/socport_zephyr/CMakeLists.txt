zephyr_library()

zephyr_library_sources(src/utils_getopt.c)
zephyr_library_sources(src/wifi_drv_lower.c)
zephyr_library_sources(src/soc_wifi_mgmt.c)
zephyr_library_sources_ifdef(CONFIG_QCC74X_WIFI_LOW_POWER src/qcc74x_wifi_lowpower.c)
zephyr_library_sources_ifdef(CONFIG_QCC74X_PM_PDS15_BLE src/qcc74x_ble_lowpower.c)

zephyr_include_directories(include)
zephyr_include_directories(src)
# The qc7xx Wi-Fi driver (providing qc7xx_wifi_drv_priv.h) lives in the
# qcc74x_zephyr module, not the Zephyr tree, so the tree stays clean. The driver
# also exposes its src/ globally, but reference it here explicitly for clarity.
if(DEFINED ZEPHYR_QCC74X_ZEPHYR_MODULE_DIR)
  zephyr_include_directories(${ZEPHYR_QCC74X_ZEPHYR_MODULE_DIR}/drivers/wifi/qc7xx/src)
else()
  zephyr_include_directories(${ZEPHYR_BASE}/drivers/wifi/qc7xx/src)
endif()
