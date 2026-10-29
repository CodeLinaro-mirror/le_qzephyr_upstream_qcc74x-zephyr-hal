# Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
# SPDX-License-Identifier: BSD-3-Clause

# check for RF libraries in case BT or WIFI are enabled

# Define blob base path using CMAKE_CURRENT_SOURCE_DIR (reliable, no dependency on
# ZEPHYR_HAL_QCOM_MODULE_DIR whose value depends on west manifest naming convention)
set(QCC7XX_BLOB_BASE "${CMAKE_CURRENT_SOURCE_DIR}/blobs" CACHE PATH "" FORCE)
set(QCC7XX_BLOB_COMMON "${QCC7XX_BLOB_BASE}/common")
if(CONFIG_SOC_SERIES_QCC74X)
  set(QCC74X_BLOB_BASE "${QCC7XX_BLOB_BASE}/qcc74x")
  set(QCC7XX_BLOB_SOC "${QCC74X_BLOB_BASE}")
endif()
if(CONFIG_SOC_SERIES_QCR3XX)
  set(QCR373_BLOB_BASE "${QCC7XX_BLOB_BASE}/qcr373")
  set(QCC7XX_BLOB_SOC "${QCR373_BLOB_BASE}")
endif()

# Copy platform host tool binaries from blobs to tools/ directory.
# Runs at cmake configure time so tool checks in soc CMakeLists can find executables.
# Blobs must be fetched first: west blobs fetch hal_qcom
set(QCC7XX_TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../tools/qc7xx_tools")

macro(qcc7xx_copy_tool SRC DST)
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
set(QCC7XX_BIN_BASE "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../prop/qcc74x-bin")
set(QCC7XX_BIN_COMMON "${QCC7XX_BIN_BASE}/common")
if(CONFIG_SOC_SERIES_QCC74X)
  set(QCC7XX_BIN_SOC "${QCC7XX_BIN_BASE}")
endif()
if(CONFIG_SOC_SERIES_QCR3XX)
  set(QCC7XX_BIN_SOC "${QCC7XX_BIN_BASE}/qcr373")
endif()

if(EXISTS ${QCC7XX_BIN_SOC})
  if(NOT EXISTS ${QCC7XX_BLOB_SOC})
    file(MAKE_DIRECTORY ${QCC7XX_BLOB_SOC})
    file(COPY ${QCC7XX_BIN_SOC}/bin ${QCC7XX_BIN_SOC}/lib ${QCC7XX_BIN_SOC}/tool
         DESTINATION ${QCC7XX_BLOB_SOC})
  else()
    message(STATUS "binaries existed, skip the copy")
    message(STATUS "If require to replace: please run prop_blobs_fetch.py manually")
  endif()
endif()



if(CMAKE_HOST_WIN32)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/tool/QConn_Flash.exe             ${QCC7XX_TOOLS_DIR}/QConn_Flash/QConn_Flash.exe)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/tool/QConn_Flash_Cmd.exe         ${QCC7XX_TOOLS_DIR}/QConn_Flash/QConn_Flash_Cmd.exe)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/tool/QConn_Secure.exe            ${QCC7XX_TOOLS_DIR}/QConn_Secure/QConn_Secure.exe)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/tool/QConn_Production_Tool.exe   ${QCC7XX_TOOLS_DIR}/QConn_Production_Tool/QConn_Production_Tool.exe)
elseif(CMAKE_HOST_APPLE)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/tool/QConn_Flash-macos                ${QCC7XX_TOOLS_DIR}/QConn_Flash/QConn_Flash-macos)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/tool/QConn_Flash_Cmd-macos            ${QCC7XX_TOOLS_DIR}/QConn_Flash/QConn_Flash_Cmd-macos)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/tool/QConn_Secure-macos               ${QCC7XX_TOOLS_DIR}/QConn_Secure/QConn_Secure-macos)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/tool/QConn_Production_Tool-macos      ${QCC7XX_TOOLS_DIR}/QConn_Production_Tool/QConn_Production_Tool-macos)
else()
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/tool/QConn_Flash-ubuntu              ${QCC7XX_TOOLS_DIR}/QConn_Flash/QConn_Flash-ubuntu)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/tool/QConn_Flash_Cmd-ubuntu          ${QCC7XX_TOOLS_DIR}/QConn_Flash/QConn_Flash_Cmd-ubuntu)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/tool/QConn_Secure-ubuntu             ${QCC7XX_TOOLS_DIR}/QConn_Secure/QConn_Secure-ubuntu)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/tool/QConn_Production_Tool-ubuntu    ${QCC7XX_TOOLS_DIR}/QConn_Production_Tool/QConn_Production_Tool-ubuntu)
endif()
if(CONFIG_SOC_SERIES_QCC74X)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/bin/flash_para.bin
                 ${QCC7XX_TOOLS_DIR}/QConn_Flash/chips/qcc74x/efuse_bootheader/flash_para.bin)
endif()
if(CONFIG_SOC_SERIES_QCR3XX)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/bin/flash_para.bin
                   ${QCC7XX_TOOLS_DIR}/QConn_Flash/chips/qcr373/efuse_bootheader/flash_para.bin)
endif()
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/zephyr)
if(CONFIG_SOC_SERIES_QCC74X)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/bin/boot2_qcc743_isp_release_v8.1.9.bin
                   ${CMAKE_BINARY_DIR}/zephyr/boot2_qcc743_isp_release_v8.1.9.bin)
endif()
if(CONFIG_SOC_SERIES_QCR3XX)
  qcc7xx_copy_tool(${QCC7XX_BLOB_SOC}/bin/boot2_qcr373_isp_release_v8.2.4.bin
                   ${CMAKE_BINARY_DIR}/zephyr/boot2_qcr373_isp_release_v8.2.4.bin)
endif()


if(CONFIG_SOC_SERIES_QCC74X)
set(CONFIG_ROMAPI 1)
else()
set(CONFIG_ROMAPI 0)
endif()

add_subdirectory_ifdef(CONFIG_SOC_SERIES_QCC74X qcc74x)
add_subdirectory_ifdef(CONFIG_SOC_SERIES_QCC75X qcc75x)
add_subdirectory_ifdef(CONFIG_SOC_SERIES_QCR3XX qcr373)

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
