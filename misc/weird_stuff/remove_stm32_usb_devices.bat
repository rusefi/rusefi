@echo off
setlocal DisableDelayedExpansion
rem Work around stale USB device caching: https://github.com/rusefi/rusefi/issues/10286
rem Requires Windows 10 version 2004 or later, with Windows PowerShell.
rem Close rusEFI Console/TunerStudio and unplug affected boards before running.
rem Accept the administrator prompt when launched, then reconnect boards when done.
rem Removes all matching device instances (including disconnected ones) and their
rem child interfaces. Driver packages remain installed.

rem Pass the path through the environment to preserve spaces and apostrophes.
set "RUSEFI_USB_CLEANUP_BATCH=%~f0"
set "RUSEFI_USB_CLEANUP_ELEVATED=%~1"

powershell.exe -NoLogo -NoProfile -Command ^
  "$ErrorActionPreference = 'Stop'; try {" ^
  "  $identity = [Security.Principal.WindowsIdentity]::GetCurrent();" ^
  "  $principal = New-Object Security.Principal.WindowsPrincipal($identity);" ^
  "  if (-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {" ^
  "    if ($env:RUSEFI_USB_CLEANUP_ELEVATED -eq '--elevated') { throw 'Administrator rights were not granted.' };" ^
  "    Write-Host 'Requesting administrator rights. Accept the Windows UAC prompt to continue.';" ^
  "    $process = Start-Process -FilePath $env:RUSEFI_USB_CLEANUP_BATCH -ArgumentList '--elevated' -Verb RunAs -Wait -PassThru;" ^
  "    exit $process.ExitCode;" ^
  "  };" ^
  "  if ([Environment]::OSVersion.Version.Build -lt 19041) { throw 'Windows 10 version 2004 or later is required.' };" ^
  "  $devices = @(Get-PnpDevice | Where-Object { $_.InstanceId -like 'USB\VID_0483&PID_5740\*' });" ^
  "  if ($devices.Count -eq 0) { Write-Host 'No matching USB devices found.'; exit 0 };" ^
  "  $failed = $false; $reboot = $false;" ^
  "  foreach ($device in $devices) {" ^
  "    Write-Host ('Removing ' + $device.InstanceId);" ^
  "    & $env:SystemRoot\System32\pnputil.exe /remove-device $device.InstanceId /subtree;" ^
  "    $result = $LASTEXITCODE;" ^
  "    if ($result -eq 3010) { $reboot = $true }" ^
  "    elseif ($result -ne 0) { Write-Warning ('Removal failed with exit code ' + $result + ': ' + $device.InstanceId); $failed = $true };" ^
  "  };" ^
  "  if ($reboot) { Write-Host 'Windows requires a restart to complete device removal.' };" ^
  "  if ($failed) { exit 1 };" ^
  "  Write-Host 'Removal complete. Reconnect your boards so Windows detects them again.';" ^
  "  if ($reboot) { exit 3010 };" ^
  "  exit 0;" ^
  "} catch { Write-Host ('ERROR: ' + $_.Exception.Message); exit 1 }"

set "result=%errorlevel%"
echo.
pause
exit /b %result%
