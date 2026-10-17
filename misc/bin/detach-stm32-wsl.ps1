#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Detach an STM32 USB device (VID 0483) from WSL and return it to Windows.

.DESCRIPTION
    Uses `usbipd state` (JSON) to find connected devices whose InstanceId matches
    VID_0483. If the device is currently attached to WSL, it executes `usbipd detach`.

    A plain detach leaves the usbipd *bind* in place. After a forced bind (which
    this machine always needs, see readme-usb.md) that means Windows still has no
    COM port, even after a replug. Use -Unbind to also drop the bind for every
    matching device, connected or only persisted, and hand it back to Windows.

.PARAMETER Vid
    USB Vendor ID to search for. Defaults to 0483 (STMicroelectronics).

.PARAMETER BusId
    Force a specific bus id instead of auto-detecting (e.g. "2-1").

.PARAMETER Unbind
    Also run `usbipd unbind` (requires administrator; elevates via one UAC prompt)
    so Windows gets the CDC / mass-storage interfaces back.

.EXAMPLE
    ./detach-stm32-wsl.ps1
    Auto-detect the STM32 and detach it from WSL.

.EXAMPLE
    ./detach-stm32-wsl.ps1 -Unbind
    Detach and unbind every STM32, including persisted binds of unplugged boards.
#>
[CmdletBinding()]
param(
    [string]$Vid = '0483',
    [string]$BusId,
    [switch]$Unbind
)

$ErrorActionPreference = 'Stop'
. "$PSScriptRoot/usbipd-common.ps1"

try {
    Assert-Usbipd
    Write-Host "Querying usbipd for devices..." -ForegroundColor Cyan
    $devices = @(Get-UsbipdDevices)
    # -Unbind also selects persisted bindings of unplugged boards.
    $targets = @(Select-UsbipdDevices -Devices $devices -Pattern "VID_$Vid" -BusId $BusId `
        -All:$Unbind -IncludePersisted:$Unbind)
    if ($targets.Count -eq 0) {
        Write-Host "No device with VID $Vid found." -ForegroundColor Yellow
        exit 0
    }

    foreach ($dev in $targets) {
        Write-Host ("Found: busid={0}  guid={1}  {2}" -f
            $(if ($dev.BusId) { $dev.BusId } else { '-' }),
            $(if ($dev.PersistedGuid) { $dev.PersistedGuid } else { '-' }),
            $dev.Description) -ForegroundColor Green
    }

    foreach ($dev in $targets) {
        if (-not (Disconnect-UsbipdDevice $dev)) {
            Fail "Could not detach busid $($dev.BusId)."
        }
    }

    if (-not $Unbind) {
        if ($targets | Where-Object { $_.IsForced }) {
            Write-Host "Device is still force-bound, so Windows has no COM port. Re-run with -Unbind to give it back." -ForegroundColor Yellow
        } else {
            Write-Host "Done." -ForegroundColor Green
        }
        exit 0
    }

    # Unbinding by GUID works whether or not the board is plugged in right now.
    $guids = @($targets | Where-Object { $_.PersistedGuid } | ForEach-Object { $_.PersistedGuid })
    if ($guids.Count -eq 0) {
        Write-Host "Nothing is bound; Windows already owns the device." -ForegroundColor Green
        exit 0
    }

    Invoke-UsbipdAdminBatch -Operation unbind -Ids $guids

    # Trust the re-queried state rather than exit codes.
    $left = @(Get-UsbipdDevices | Where-Object { $_.PersistedGuid -and $guids -contains $_.PersistedGuid })
    if ($left.Count -gt 0) {
        foreach ($dev in $left) {
            Write-Host ("  still bound: guid {0}  {1}" -f $dev.PersistedGuid, $dev.Description) -ForegroundColor Red
        }
        Fail "Unbind incomplete. If pnputil reports 'pending system reboot', reboot (see readme-usb.md)."
    }
    Write-Host "Done. Unbound; Windows should re-enumerate the COM port (plug the board in if it is not)." -ForegroundColor Green
} catch {
    Fail $_
}
