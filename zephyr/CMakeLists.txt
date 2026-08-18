# Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
# SPDX-License-Identifier: BSD-3-Clause

# check for RF libraries in case BT or WIFI are enabled

# Define blob base path using CMAKE_CURRENT_SOURCE_DIR (reliable, no dependency on
# ZEPHYR_HAL_QCOM_MODULE_DIR whose value depends on west manifest naming convention)
set(QCC74X_BLOB_BASE "${CMAKE_CURRENT_SOURCE_DIR}/blobs/qcc74x" CACHE PATH "" FORCE)

# Copy platform host tool binaries from blobs to tools/ directory.
# Runs at cmake configure time so tool checks in soc CMakeLists can find executables.
# Blobs must be fetched first: west blobs fetch hal_qcom
set(QCC74X_TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../tools/qc7xx_tools")

macro(qcc74x_copy_tool SRC DST)
  if(EXISTS ${SRC})
    configure_file(${SRC} ${DST} COPYONLY)
    if(NOT CMAKE_HOST_WIN32)
      file(CHMOD ${DST}
           PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                       GROUP_READ GROUP_EXECUTE
                       WORLD_READ WORLD_EXECUTE)
    endif()
  else()
    message(WARNING "Blob not found: ${SRC}\nRun: west blobs fetch hal_qcom")
  endif()
endmacro()

# Populate blobs from the proprietary binary drop (prop/qcc74x-bin) if present,
# so builds can pick up binaries without requiring "west blobs fetch".
set(QCC74X_BIN_BASE "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../prop/qcc74x-bin")
if(EXISTS ${QCC74X_BIN_BASE})
  if(NOT EXISTS ${QCC74X_BLOB_BASE})
    file(MAKE_DIRECTORY ${QCC74X_BLOB_BASE})
    file(COPY ${QCC74X_BIN_BASE}/bin ${QCC74X_BIN_BASE}/lib ${QCC74X_BIN_BASE}/tool
         DESTINATION ${QCC74X_BLOB_BASE})
  else()
    message(STATUS "qcc74x binaries existed, skip the copy")
    message(STATUS "If require to replace: please run prop_blobs_fetch.py manually")
  endif()
endif()

if(CMAKE_HOST_WIN32)
  qcc74x_copy_tool(${QCC74X_BLOB_BASE}/tool/QConn_Flash.exe     ${QCC74X_TOOLS_DIR}/QConn_Flash/QConn_Flash.exe)
  qcc74x_copy_tool(${QCC74X_BLOB_BASE}/tool/QConn_Flash_Cmd.exe ${QCC74X_TOOLS_DIR}/QConn_Flash/QConn_Flash_Cmd.exe)
  qcc74x_copy_tool(${QCC74X_BLOB_BASE}/tool/QConn_Secure.exe    ${QCC74X_TOOLS_DIR}/QConn_Secure/QConn_Secure.exe)
elseif(CMAKE_HOST_APPLE)
  qcc74x_copy_tool(${QCC74X_BLOB_BASE}/tool/QConn_Flash-macos      ${QCC74X_TOOLS_DIR}/QConn_Flash/QConn_Flash-macos)
  qcc74x_copy_tool(${QCC74X_BLOB_BASE}/tool/QConn_Flash_Cmd-macos  ${QCC74X_TOOLS_DIR}/QConn_Flash/QConn_Flash_Cmd-macos)
  qcc74x_copy_tool(${QCC74X_BLOB_BASE}/tool/QConn_Secure-macos     ${QCC74X_TOOLS_DIR}/QConn_Secure/QConn_Secure-macos)
else()
  qcc74x_copy_tool(${QCC74X_BLOB_BASE}/tool/QConn_Flash-ubuntu     ${QCC74X_TOOLS_DIR}/QConn_Flash/QConn_Flash-ubuntu)
  qcc74x_copy_tool(${QCC74X_BLOB_BASE}/tool/QConn_Flash_Cmd-ubuntu ${QCC74X_TOOLS_DIR}/QConn_Flash/QConn_Flash_Cmd-ubuntu)
  qcc74x_copy_tool(${QCC74X_BLOB_BASE}/tool/QConn_Secure-ubuntu    ${QCC74X_TOOLS_DIR}/QConn_Secure/QConn_Secure-ubuntu)
endif()
qcc74x_copy_tool(${QCC74X_BLOB_BASE}/bin/flash_para.bin
                 ${QCC74X_TOOLS_DIR}/QConn_Flash/chips/qcc74x/efuse_bootheader/flash_para.bin)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/zephyr)
qcc74x_copy_tool(${QCC74X_BLOB_BASE}/bin/boot2_qcc743_isp_release_v8.1.9.bin
                 ${CMAKE_BINARY_DIR}/zephyr/boot2_qcc743_isp_release_v8.1.9.bin)


if(CONFIG_SOC_SERIES_QCC74X)
set(CONFIG_ROMAPI 1)
else()
set(CONFIG_ROMAPI 0)
endif()

add_subdirectory_ifdef(CONFIG_SOC_SERIES_QCC74X qcc74x)
add_subdirectory_ifdef(CONFIG_SOC_SERIES_QCC75X qcc75x)

# check VERSION file
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../VERSION")
    # read info from VERSION file
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/../VERSION" VERSION_FILE_CONTENT)

    message(STATUS "Reading version from VERSION file")
    string(REGEX MATCH "LHAL_VERSION[ ]+\"([^\"]+)\"" _ ${VERSION_FILE_CONTENT})
    set(LHAL_VERSION "${CMAKE_MATCH_1}")
else()
    # get git tag
    execute_process(
        COMMAND git describe --abbrev=8 --tags --dirty --always
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if(GIT_TAG)
        set(LHAL_VERSION ${GIT_TAG})
    else()
        message(WARNING "No Valid version info found for LHAL!")
        set(LHAL_VERSION "version-unknown-panic")
    endif()
endif()
message(STATUS "LHAL Version: ${LHAL_VERSION}")

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/../drivers/lhal/config/lhal_version.h.in"
  "${CMAKE_BINARY_DIR}/generated/lhal_version.h"
)
zephyr_include_directories("${CMAKE_BINARY_DIR}/generated")
zephyr_sources(../drivers/lhal/config/lhal_version.c)
