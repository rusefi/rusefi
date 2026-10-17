#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Locate an STM32 USB device (VID 0483) and bind/attach it to WSL via usbipd-win.

.DESCRIPTION
    Uses `usbipd state` (JSON) to find connected devices whose InstanceId matches
    VID_0483. If found, it binds the device (needs admin, one-time) and attaches it
    to the running WSL distro. Re-running after a re-plug just re-attaches.

    See readme-usb.md for the manual steps this automates.

.PARAMETER Vid
    USB Vendor ID to search for. Defaults to 0483 (STMicroelectronics).

.PARAMETER BusId
    Force a specific bus id instead of auto-detecting (e.g. "2-1").

.EXAMPLE
    ./attach-stm32-wsl.ps1
    Auto-detect the STM32 and attach it to WSL.

.EXAMPLE
    ./attach-stm32-wsl.ps1 -Vid 0483 -BusId 2-4
#>
[CmdletBinding()]
param(
    [string]$Vid = '0483',
    [string]$BusId
)

$ErrorActionPreference = 'Stop'
. "$PSScriptRoot/usbipd-common.ps1"

try {
    Invoke-UsbipdDeviceScript -Action Attach -Name 'STM32' -Pattern "VID_$Vid" -BusId $BusId
} catch {
    Fail $_
}

Write-Host "Inside WSL verify with:  lsusb | grep -i 0483" -ForegroundColor DarkGray
Write-Host "(if lsusb is missing in WSL:  sudo apt install usbutils)" -ForegroundColor DarkGray
