# Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
# SPDX-License-Identifier: BSD-3-Clause
#
# Download CH347-patched OpenOCD binaries from WCHSoftGroup/ch347.
#
# Usage:
#   .\fetch_ch347.ps1              # use default pinned commit
#   .\fetch_ch347.ps1 -Commit <sha> # use specific commit
#
# Files downloaded:
#   openocd\openocd.exe
#   openocd\libhidapi-0.dll
#   openocd\libusb-1.0.dll
#   openocd\interface\ch347.cfg

param(
    [string]$Commit = "9e4dd43e72dd3e16e46f0d6e2041b84e56cfb0a9"
)

$ErrorActionPreference = "Stop"
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$BaseUrl = "https://raw.githubusercontent.com/WCHSoftGroup/ch347/$Commit"

function Download-File {
    param([string]$Url, [string]$Dst)
    $dir = Split-Path -Parent $Dst
    if (-not (Test-Path $dir)) { New-Item -ItemType Directory -Path $dir | Out-Null }
    Write-Host "  Downloading $(Split-Path -Leaf $Dst) ..."
    Invoke-WebRequest -Uri $Url -OutFile $Dst -UseBasicParsing
}

Write-Host "Fetching CH347-patched OpenOCD from WCHSoftGroup/ch347 @ $Commit"

Download-File "$BaseUrl/OpenOCD/bin/openocd.exe"     "$ScriptDir\openocd.exe"
Download-File "$BaseUrl/OpenOCD/bin/libhidapi-0.dll" "$ScriptDir\libhidapi-0.dll"
Download-File "$BaseUrl/OpenOCD/bin/libusb-1.0.dll"  "$ScriptDir\libusb-1.0.dll"
Download-File "$BaseUrl/OpenOCD/bin/ch347.cfg" `     "$ScriptDir\ch347.cfg"

Write-Host "Done. Files saved to $ScriptDir"
