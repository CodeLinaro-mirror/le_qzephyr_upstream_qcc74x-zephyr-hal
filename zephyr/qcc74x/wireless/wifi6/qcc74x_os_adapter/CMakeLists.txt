zephyr_library()

# sdk_library_add_sources(src/rtos_al.c)
# sdk_library_add_sources(src/platform_qcc74x_sdk.c)

zephyr_library_sources(src/rtos_zephyr.c)
zephyr_library_sources(src/platform_zephyrsdk.c)

zephyr_compile_options(-Dqcc74x_SDK)

zephyr_include_directories(include)
