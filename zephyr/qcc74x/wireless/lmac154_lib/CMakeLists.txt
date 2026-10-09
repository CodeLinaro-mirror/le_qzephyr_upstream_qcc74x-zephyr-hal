set(QCC74X_LMAC154_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})

zephyr_include_directories(include)

set(QCC74X_LMAC154_PREBUILT_LIB
    ${QCC74X_BLOB_BASE}/lib/liblmac154_qcc743.a)

if(NOT EXISTS ${QCC74X_LMAC154_PREBUILT_LIB})
  message(FATAL_ERROR
    "Missing QCC74x lmac154 prebuilt library: ${QCC74X_LMAC154_PREBUILT_LIB}")
endif()

zephyr_blobs_verify(FILES ${QCC74X_LMAC154_PREBUILT_LIB} REQUIRED)
zephyr_library_import(lmac154_qcc743_prebuilt ${QCC74X_LMAC154_PREBUILT_LIB})
