# Release v2.1.16
## Features
- feat: decouple QCC748 PDS1 PSRAM retention from the Wi-Fi performance profile
- feat: add QCC748 PDS1 Wi-Fi library variants
## Bug Fixes
- fix: retain PSRAM controller access and Wi-Fi task stacks safely across PDS1

# Release v2.1.15
## Features
- feat: restructure to release-style west workspace layout
- feat: support QCC74x PDS1 PSRAM retention in LPFW
- zperf: Optimize 748 performance (TCP TX: 28Mbps TCP RX: 29Mbps UDP TX: 52Mbps UDP RX: 53Mbps)
## Bug Fixes
- fix: preserve WPA and crypto attribution.

# Release v2.1.14
## Features
- feat: support PDS15 GPIO wake
- feat: add flash application jump support
- feat: accelerate MCUboot image authentication
- feat: update support max connection from 2 to 4
## Bug Fixes
- fix: follow STA channel when starting AP

# Release v2.1.13
## Features
- feat: CSI DMA capture and antenna diversity config by user
## Bug Fixes
- None

# Release v2.1.12
## Features
- feat: add CSI DMA capture and host tools
- feat: add antenna diversity support
## Bug Fixes
- fix: reject invalid AP channels instead of fallback

# Release v2.1.11
## Features
- feat: add lmac154 15.4 MAC library and Zephyr build wiring
## Bug Fixes
- None

# Release v2.1.10
## Features
- feat: add anti-rollback enable getter
## Bug Fixes
- fix: Fix the WiFi control task stack overflow issue

# Release v2.1.9
## Features
- feat: add QCC74x HBN system helper
## Bug Fixes
- None


# Release v2.1.8
## Features
- feat: support wram reset for qcc74x
## Bug Fixes
- None

# Release v2.1.7
## Features
- optimize keyram dump and fix 256-bit key display.
- optimise ble lowpower
## Bug Fixes
- None

# Release v2.1.6
## Features
- feat: tx buffer alloc support timeout config
## Bug Fixes
- None

# Release v2.1.5
## Features
- feat: support reset from interface
- fix: reset from interface with por reset
## Bug Fixes
- None

# Release v2.1.3
## Features
- fix: fix bt_disable failed
## Bug Fixes
- None

# Release v2.1.2
## Features
- add coex support
- add pds15 and wifi lowpower support
## Bug Fixes
- None

# Release v2.1.1
## Features
- None
## Bug Fixes
- Fix build err for wifi

# Release v2.1.0
## Features
- None
## Bug Fixes
- None
