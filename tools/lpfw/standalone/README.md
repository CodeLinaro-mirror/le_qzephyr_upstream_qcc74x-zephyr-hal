<!--
Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
SPDX-License-Identifier: BSD-3-Clause
-->

# LPFW source package from BL v2.1.15

The SDK firmware builds use the released, ABI-checked `prop/qcc74x-bin/bin/qcc74x_lp_fw.bin`, staged by `qapp/qcc74x/prop_blobs_fetch.py`. The host and this binary implement `qcc74x-pds1-abi2`.

The source files and CMake entry point in this directory are imported from BL release `3c92d0b`. The upstream release is **not a self-contained LPFW rebuild kit**: its CMake file references missing `lpfw_printf.c`, `vsnprintf_nano.c`, `startup/startup.S`, `macsw_port/source/rxl_hwdesc.c`, and `qcc743_lpfw_board/lpfw_version.S.in`. It also requires a T-Head E907 compiler and vendor LP PHY inputs. Those missing files are absent from the BL release itself, not deferred host-side integration.

Do not replace the released LPFW with an unrelated or older image. Obtain a complete matching source/toolchain package from BL before rebuilding LPFW. Application builds do not invoke this optional standalone build; they validate the released LPFW header, ABI and SHA-256 before packaging it.
