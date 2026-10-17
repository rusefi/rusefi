# Shared usbipd operations. Dot-source from scripts using $PSScriptRoot.
function Fail($msg) {
    Write-Host "ERROR: $msg" -ForegroundColor Red
    exit 1
}

function Assert-Usbipd {
    if (-not (Get-Command usbipd -ErrorAction SilentlyContinue)) {
        Fail 'usbipd-win is not installed. Install with: winget install --interactive --exact dorssel.usbipd-win'
    }
}

function Test-UsbipdAdministrator {
    return ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole(
        [Security.Principal.WindowsBuiltinRole]::Administrator)
}

function Get-UsbipdDevices {
    # Windows PowerShell treats native stderr as errors even for ordinary info.
    $ErrorActionPreference = 'Continue'
    $json = usbipd state | Out-String
    if ($LASTEXITCODE -ne 0) { throw "'usbipd state' failed (exit $LASTEXITCODE)." }
    try { return @(($json | ConvertFrom-Json -ErrorAction Stop).Devices) }
    catch { throw "Could not parse 'usbipd state' output: $_" }
}

function Invoke-Usbipd {
    param([string[]]$Arguments, [string]$Indent = '  ')
    $ErrorActionPreference = 'Continue'
    $output = & usbipd @Arguments 2>&1
    $code = $LASTEXITCODE
    foreach ($line in $output) {
        $message = [string]$line
        if (-not $message.Trim()) { continue }
        $color = if ($message -match ':\s*error:') { 'Red' }
                 elseif ($message -match ':\s*warning:') { 'Yellow' }
                 else { 'DarkGray' }
        Write-Host "$Indent$message" -ForegroundColor $color
    }
    return $code
}

function Invoke-UsbipdBind {
    param([string]$BusId)
    if (Test-UsbipdAdministrator) {
        return (Invoke-Usbipd -Arguments @('bind', '--busid', $BusId))
    }
    Write-Host "Requesting elevation for 'usbipd bind' (accept the UAC prompt)..." -ForegroundColor Yellow
    $p = Start-Process -FilePath 'usbipd.exe' -ArgumentList @('bind', '--busid', $BusId) `
        -WindowStyle Hidden -Verb RunAs -Wait -PassThru -ErrorAction Stop
    return $p.ExitCode
}

function Invoke-UsbipdAdminBatch {
    param([ValidateSet('bind', 'unbind')][string]$Operation, [string[]]$Ids, [switch]$Force)
    $option = if ($Operation -eq 'bind') { '--busid' } else { '--guid' }
    $arguments = @($Operation)
    if ($Force) { $arguments += '--force' }
    $arguments += $option
    # These IDs enter an elevated command. Accept only bus IDs or GUIDs.
    foreach ($id in $Ids) {
        if (($Operation -eq 'bind' -and $id -notmatch '^\d+-\d+$') -or
            ($Operation -eq 'unbind' -and $id -notmatch '^[0-9a-fA-F]{8}(?:-[0-9a-fA-F]{4}){3}-[0-9a-fA-F]{12}$')) {
            throw "Invalid $option value: $id"
        }
    }
    if (Test-UsbipdAdministrator) {
        foreach ($id in $Ids) {
            $code = Invoke-Usbipd -Arguments ($arguments + $id)
            if ($code -ne 0) { Write-Host "'usbipd $Operation' failed for $id (exit $code)." -ForegroundColor Red }
        }
        return
    }
    Write-Host "Requesting elevation for 'usbipd $Operation' (accept the UAC prompt)..." -ForegroundColor Yellow
    $inner = ($Ids | ForEach-Object { "usbipd $($arguments -join ' ') $_" }) -join '; '
    $encoded = [Convert]::ToBase64String([Text.Encoding]::Unicode.GetBytes($inner))
    try {
        $p = Start-Process -FilePath 'powershell.exe' -ArgumentList @('-NoProfile', '-EncodedCommand', $encoded) `
            -WindowStyle Hidden -Verb RunAs -Wait -PassThru -ErrorAction Stop
        if ($p.ExitCode -ne 0) { Write-Host "Elevated helper exited $($p.ExitCode); checking actual state." -ForegroundColor Yellow }
    } catch {
        Write-Host "Elevation was declined or could not be shown: $($_.Exception.Message)" -ForegroundColor Red
        Write-Host "Run from an elevated prompt: usbipd $($arguments -join ' ') <id>" -ForegroundColor Yellow
    }
    # Callers re-query the state to detect partial success.
}

function Get-SlcanPattern {
    param([string]$Vid, [string]$PidRegex)
    # No override: match the known pairs, never the cross-product of IDs.
    if (-not $Vid -and -not $PidRegex) {
        return 'VID_(?:0483&PID_5740|16D0&PID_117E)(?=[\\&]|$)'
    }
    # Partial overrides retain the current WeAct defaults.
    if (-not $Vid) { $Vid = '0483' }
    if (-not $PidRegex) { $PidRegex = '5740' }
    return "VID_(?:$Vid)&PID_(?:$PidRegex)(?=[\\&]|$)"
}

function Select-UsbipdDevices {
    param([object[]]$Devices, [string]$Pattern, [string]$BusId, [string]$ComPort,
        [switch]$All, [switch]$IncludePersisted)
    if ($BusId) {
        $targets = @($Devices | Where-Object { $_.BusId -eq $BusId })
        if (-not $targets) { throw "No device on busid $BusId." }
        return $targets
    }
    # Unbinding must also find saved bindings for unplugged devices.
    $targets = @($Devices | Where-Object {
        ($_.BusId -or ($IncludePersisted -and $_.PersistedGuid)) -and $_.InstanceId -match $Pattern
    })
    if ($ComPort) {
        $preferred = @($targets | Where-Object { $_.Description -match ('\(' + [regex]::Escape($ComPort) + '\)') })
        if ($preferred) { $targets = $preferred }
    }
    if ($All) { return $targets }
    return ($targets | Select-Object -First 1)
}

function Connect-UsbipdDevice {
    param($Device)
    $busId = $Device.BusId
    if ($Device.ClientIPAddress) {
        Write-Host "busid $busId already attached to WSL ($($Device.ClientIPAddress)); nothing to do." -ForegroundColor Green
        return $true
    }
    if (-not $Device.PersistedGuid) {
        Write-Host "Binding busid $busId (requires administrator)..." -ForegroundColor Cyan
        $code = Invoke-UsbipdBind -BusId $busId
        if ($code -ne 0) {
            Write-Host "'usbipd bind' failed for busid $busId (exit $code)." -ForegroundColor Red
            return $false
        }
    }
    Write-Host "Attaching busid $busId to WSL..." -ForegroundColor Cyan
    $code = Invoke-Usbipd -Arguments @('attach', '--wsl', '--busid', $busId)
    if ($code -ne 0) {
        Write-Host "'usbipd attach' failed for busid $busId (exit $code). Is WSL running?" -ForegroundColor Red
        return $false
    }
    Write-Host "Done. busid $busId is attached to WSL." -ForegroundColor Green
    return $true
}

function Disconnect-UsbipdDevice {
    param($Device)
    if (-not $Device.ClientIPAddress) {
        Write-Host "busid $($Device.BusId) is not currently attached to WSL; nothing to do." -ForegroundColor Green
        return $true
    }
    Write-Host "Detaching busid $($Device.BusId) from WSL..." -ForegroundColor Cyan
    $code = Invoke-Usbipd -Arguments @('detach', '--busid', $Device.BusId)
    if ($code -ne 0) {
        Write-Host "'usbipd detach' failed (exit $code)." -ForegroundColor Red
        return $false
    }
    Write-Host "Done. busid $($Device.BusId) detached from WSL." -ForegroundColor Green
    return $true
}

function Invoke-UsbipdDeviceScript {
    param([ValidateSet('Attach', 'Detach')][string]$Action, [string]$Name,
        [string]$Pattern, [string]$BusId, [string]$ComPort, [switch]$All)
    Assert-Usbipd
    Write-Host 'Querying usbipd for connected devices...' -ForegroundColor Cyan
    $devices = @(Get-UsbipdDevices)
    $targets = @(Select-UsbipdDevices -Devices $devices -Pattern $Pattern -BusId $BusId -ComPort $ComPort -All:$All)
    if (-not $targets) {
        Write-Host "No connected $Name matching $Pattern found." -ForegroundColor Yellow
        if ($Action -eq 'Detach') { return }
        $devices | Where-Object { $_.BusId } | ForEach-Object { Write-Host ("  {0,-6} {1}" -f $_.BusId, $_.Description) }
        Fail "$Name not detected. Is it plugged in?"
    }
    $failed = 0
    foreach ($dev in $targets) {
        Write-Host ("Found {0}: busid={1}  {2}" -f $Name, $dev.BusId, $dev.Description) -ForegroundColor Green
        $ok = if ($Action -eq 'Attach') { Connect-UsbipdDevice $dev } else { Disconnect-UsbipdDevice $dev }
        if (-not $ok) { $failed++ }
    }
    if ($failed) { Fail "$failed of $($targets.Count) device(s) failed to $($Action.ToLower())." }
}
