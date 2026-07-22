# QCC743 LPFW

This directory contains the QCC743 low-power firmware source used by the
Zephyr QCC74x PDS15 flow.

- `qcc743_lp_fw/` is imported from
  `bouffalo_sdk_qc/examples/pmu/lp_fw`.
- `build_qcc743_lpfw.sh` builds the imported source with the Bouffalo SDK
  build system and emits `qcc743_lp_fw.bin`.
- When `CONFIG_QCC74X_PM_PDS15=y`, Zephyr builds LPFW on every image build,
  writes the generated image to `<build-dir>/lpfw/qcc743_lp_fw.bin`, and
  patches that generated image into `zephyr.bin`.
- `bin/qcc743_lp_fw.bin` is a checked-in reference binary only. It is not the
  patch input for current Zephyr PDS15 builds.

The Zephyr build auto-detects the Bouffalo SDK base for local bench builds.
It checks `QCC74X_LPFW_SDK_BASE`, a `bouffalo_sdk_qc` directory next to the
Zephyr workspace, and the current bench fallback under
`$HOME/zephyr_update_0401/bouffalo_sdk_qc`. The LPFW compiler prefix is also
auto-detected from `QCC74X_LPFW_CROSS_COMPILE`, `PATH`, or the current bench
toolchain path.

Direct build example:

```bash
cd /home/zhang/zephyr_update_0401/zephyr
modules/hal/qualcomm/tools/lpfw/build_qcc743_lpfw.sh \
  --source-dir modules/hal/qualcomm/tools/lpfw/qcc743_lp_fw \
  --build-dir build_lpfw_manual/qcc743_build \
  --output build_lpfw_manual/qcc743_lp_fw.bin \
  --jobs 8
```

Zephyr CMake options:

- `QCC74X_LPFW_SDK_BASE`: optional Bouffalo SDK base override.
- `QCC74X_LPFW_CROSS_COMPILE`: optional LPFW toolchain prefix override.
- `QCC74X_LPFW_JOBS`: LPFW parallel build jobs, default `8`.
- `QCC74X_LPFW_LOG_LEVEL`: LPFW log level, default `0`.
  - `0`: no LPFW printf output.
  - `1`: enable `lpfw_printf` and `lpfw_printf_debug`.
  - `2`: also enable verbose `lpfw_printf_debug2`.
