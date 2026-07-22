zephyr_library()

zephyr_library_sources(net_al_ext.c)


if (CONFIG_WIFI_MAT)
    zephyr_compile_definitions(-DCONFIG_MAT)
    zephyr_library_sources(mat.c)
    if (CONFIG_WIFI_MAT_FULL_COPY)
        zephyr_compile_definitions(-DCONFIG_MAT_FULL_COPY)
    endif()
endif()
zephyr_include_directories(include)
zephyr_include_directories(.)

zephyr_compile_definitions(-DPLATFORM_PRIVDE_ERRNO)
