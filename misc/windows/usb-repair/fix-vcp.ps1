<#
.SYNOPSIS
    Repair Windows USB state for rusEFI ECUs whose Virtual COM Port (VCP) stops
    appearing - typically after a firmware change to the USB composite layout.

.DESCRIPTION
    Windows caches USB descriptor and driver-binding state per device. When ECU
    firmware changes its composite descriptor (endpoint allocation, adding a
    second CDC/VCP interface) WITHOUT changing idVendor/idProduct/bcdDevice,
    Windows keeps reusing the stale cached layout and the VCP never binds.

    This script clears the three places that stale state accumulates:

      1. Ghost device instances  - every past board still remembered by Windows
                                   (Present = False). Forces a fresh descriptor
                                   read on next plug-in.
      2. usbflags descriptor cache - HKLM\...\Control\usbflags\<VID><PID><bcdDevice>.
                                   This is the entry keyed by bcdDevice, and is
                                   the one that bites after a layout change.
      3. COM Name Arbiter (ComDB) - the bitmap reserving COM numbers. Ghost
                                   boards hoard the low range, pushing each new
                                   board to an ever-higher COM number.

    It can also unbind the ECU from usbipd-win, which replaces the normal CDC
    stack with a VBoxUSB stub (device shows as "USBIP Shared Device" and no COM
    port is created at all).

    Only NON-PRESENT devices are ever removed, and the registry keys touched are
    backed up to .reg files first. Nothing here uninstalls a driver package -
    replugging the board recreates everything from scratch.

.PARAMETER Execute
    Actually apply changes. Without it the script is a read-only dry run.

.PARAMETER ResetComDb
    Also rewrite the COM Name Arbiter bitmap, freeing hoarded COM numbers.

.PARAMETER ClearUsbFlags
    Also delete the cached usbflags entries for the ECU VID/PID. This is the fix
    for "VCP disappeared after a firmware descriptor change".

.PARAMETER UnbindUsbip
    Also run 'usbipd unbind' for any rusEFI board currently bound/shared, and
    drop stale persisted bindings.

.PARAMETER All
    Shorthand for -ResetComDb -ClearUsbFlags -UnbindUsbip.

.EXAMPLE
    # read-only: show everything that WOULD be changed
    powershell -ExecutionPolicy Bypass -File fix-vcp.ps1 -All

.EXAMPLE
    # full repair (elevated PowerShell required)
    powershell -ExecutionPolicy Bypass -File fix-vcp.ps1 -Execute -All

.NOTES
    Recommended order of operations is handled automatically: clean ghosts and
    caches FIRST, then unbind usbipd last, so the board enumerates fresh into a
    low COM number.

    Firmware-side companion fix: bump bcdDevice in
    firmware/hw_layer/ports/stm32/serial_over_usb/usbcfg.cpp whenever the
    composite layout changes. That changes the usbflags cache key, so every
    Windows host in the field re-reads descriptors instead of needing this
    script.
#>
[CmdletBinding()]
param(
    [switch]$Execute,
    [switch]$ResetComDb,
    [switch]$ClearUsbFlags,
    [switch]$UnbindUsbip,
    [switch]$All
)

$ErrorActionPreference = 'Stop'

if ($All) {
    $ResetComDb    = $true
    $ClearUsbFlags = $true
    $UnbindUsbip   = $true
}

# rusEFI / STM32 CDC identity.
# NB: deliberately NOT named $PID - that is a read-only PowerShell automatic
# variable (the current process id) and assigning to it is a hard error.
$DevVid = '0483'
$DevPid = '5740'
$devicePrefix  = "USB\VID_${DevVid}&PID_${DevPid}"
$usbFlagsRoot  = 'HKLM:\SYSTEM\CurrentControlSet\Control\usbflags'
$comDbPath     = 'HKLM:\SYSTEM\CurrentControlSet\Control\COM Name Arbiter'
$backupDir     = Join-Path $PSScriptRoot 'backup'

function Write-Section {
    param([string]$Text)
    Write-Host ''
    Write-Host ('=' * 72) -ForegroundColor DarkGray
    Write-Host "  $Text" -ForegroundColor Cyan
    Write-Host ('=' * 72) -ForegroundColor DarkGray
}

function Test-Elevated {
    $id = [Security.Principal.WindowsIdentity]::GetCurrent()
    (New-Object Security.Principal.WindowsPrincipal($id)).IsInRole(
        [Security.Principal.WindowsBuiltInRole]::Administrator)
}

function New-BackupDir {
    if (-not (Test-Path $backupDir)) {
        New-Item -ItemType Directory -Force -Path $backupDir | Out-Null
    }
}

function Backup-RegKey {
    param([string]$RegPath, [string]$Label)
    New-BackupDir
    $file = Join-Path $backupDir ("{0}-{1}.reg" -f $Label, (Get-Date -Format 'yyyyMMdd-HHmmss'))
    & reg.exe export $RegPath $file /y | Out-Null
    if (Test-Path $file) {
        Write-Host "  backup -> $file" -ForegroundColor DarkGreen
    } else {
        Write-Host "  WARNING: backup of $RegPath failed" -ForegroundColor Yellow
    }
}

# Windows PowerShell 5.1 wraps a native executable's stderr lines in ErrorRecords
# ("NativeCommandError"), which $ErrorActionPreference='Stop' escalates to a
# terminating error even when the exe exited 0. usbipd in particular writes
# filter-driver warnings to stderr on every invocation. Run native commands
# through here so their stderr stays informational.
function Invoke-Native {
    param([string]$FilePath, [string[]]$Arguments = @())
    $old = $ErrorActionPreference
    $ErrorActionPreference = 'Continue'
    try {
        # 2>&1 merges stderr into the output stream as ErrorRecord objects, which
        # PowerShell would then render as multi-line error blocks. Flatten them to
        # their plain text so the transcript stays readable.
        $out = & $FilePath @Arguments 2>&1 | ForEach-Object {
            if ($_ -is [System.Management.Automation.ErrorRecord]) { $_.Exception.Message } else { $_ }
        } | Out-String
        [PSCustomObject]@{ ExitCode = $LASTEXITCODE; Output = $out }
    } finally {
        $ErrorActionPreference = $old
    }
}

function Get-ComDbPorts {
    $db = Get-ItemProperty -Path $comDbPath -Name ComDB -ErrorAction SilentlyContinue
    if (-not $db) { return $null }
    $bytes = $db.ComDB
    $used = @()
    for ($i = 0; $i -lt $bytes.Length; $i++) {
        for ($b = 0; $b -lt 8; $b++) {
            if ($bytes[$i] -band (1 -shl $b)) { $used += ($i * 8 + $b + 1) }
        }
    }
    , $used
}

if ($Execute -and -not (Test-Elevated)) {
    Write-Host 'ERROR: -Execute requires an elevated (Administrator) PowerShell.' -ForegroundColor Red
    Write-Host 'Start one with:  Start-Process powershell -Verb RunAs' -ForegroundColor Yellow
    exit 1
}

if (-not $Execute) {
    Write-Host ''
    Write-Host 'DRY RUN - no changes will be made. Add -Execute to apply.' -ForegroundColor Yellow
}

# ----------------------------------------------------------- current state --
Write-Section 'Current state'

$presentPorts = @(Get-PnpDevice -Class Ports -ErrorAction SilentlyContinue |
    Where-Object { $_.Present })
if ($presentPorts) {
    Write-Host 'Present COM ports:' -ForegroundColor Green
    $presentPorts | Select-Object Status, FriendlyName, InstanceId |
        Format-Table -AutoSize -Wrap | Out-String -Width 200 | Write-Host
} else {
    Write-Host 'Present COM ports: NONE' -ForegroundColor Yellow
}

$presentEcu = @(Get-PnpDevice -ErrorAction SilentlyContinue |
    Where-Object { $_.Present -and $_.InstanceId -like "$devicePrefix*" })
if ($presentEcu) {
    Write-Host "Present ${DevVid}:${DevPid} devices:" -ForegroundColor Green
    $presentEcu | Select-Object Class, FriendlyName, InstanceId |
        Format-Table -AutoSize -Wrap | Out-String -Width 200 | Write-Host

    # A board captured by usbipd shows up bound to the VBoxUSB stub driver and
    # will never produce a COM port until it is unbound.
    foreach ($d in $presentEcu) {
        $svc = (Get-PnpDeviceProperty -InstanceId $d.InstanceId -KeyName 'DEVPKEY_Device_Service' `
                -ErrorAction SilentlyContinue).Data
        if ($svc -eq 'VBoxUSB') {
            Write-Host "  !! $($d.InstanceId)" -ForegroundColor Red
            Write-Host '     bound to VBoxUSB (usbipd stub) - no COM port will appear.' -ForegroundColor Red
            Write-Host '     Use -UnbindUsbip to release it back to Windows.' -ForegroundColor Red
        }
    }
}

$flagKeys = @(Get-ChildItem $usbFlagsRoot -ErrorAction SilentlyContinue |
    Where-Object { $_.PSChildName -like "$DevVid$DevPid*" })
Write-Host ("usbflags cache entries for ${DevVid}:${DevPid}: {0}" -f $(
    if ($flagKeys) { ($flagKeys.PSChildName -join ', ') } else { '(none)' }))
Write-Host '  (key = VID + PID + bcdDevice; unchanged bcdDevice across a'
Write-Host '   descriptor change is exactly what strands the cache)'

$before = Get-ComDbPorts
if ($null -ne $before) {
    Write-Host ("ComDB reserved COM numbers ({0}): {1}" -f $before.Count, ($before -join ', '))
}

# ------------------------------------------------------------ ghost sweep ---
Write-Section 'Ghost device instances'

$ghosts = @(Get-PnpDevice -ErrorAction SilentlyContinue |
    Where-Object {
        $_.Present -eq $false -and (
            $_.InstanceId -like "$devicePrefix*" -or
            $_.Class -eq 'Ports'
        )
    })

if (-not $ghosts) {
    Write-Host 'No ghost COM/VCP devices found.' -ForegroundColor Green
} else {
    Write-Host "Ghost (not-present) devices: $($ghosts.Count)" -ForegroundColor Yellow
    $ghosts | Select-Object Class, FriendlyName, InstanceId |
        Sort-Object Class, FriendlyName |
        Format-Table -AutoSize -Wrap | Out-String -Width 200 | Write-Host

    if ($Execute) {
        $ok = 0; $failed = @()
        foreach ($d in $ghosts) {
            Write-Host ("  removing {0} ..." -f $d.InstanceId) -NoNewline
            $r = Invoke-Native -FilePath 'pnputil.exe' -Arguments @('/remove-device', $d.InstanceId)
            if ($r.ExitCode -eq 0) {
                Write-Host ' OK' -ForegroundColor Green
                $ok++
            } else {
                Write-Host ' FAILED' -ForegroundColor DarkYellow
                $failed += [PSCustomObject]@{
                    InstanceId = $d.InstanceId
                    Output     = ($r.Output -replace '\s+', ' ').Trim()
                }
            }
        }
        Write-Host "Removed $ok of $($ghosts.Count)." -ForegroundColor Cyan
        if ($failed) {
            Write-Host 'Failures (usually harmless - removing a parent already took the child):' -ForegroundColor DarkYellow
            $failed | Format-Table -AutoSize -Wrap | Out-String -Width 200 | Write-Host
        }
    }
}

# ------------------------------------------------------ usbflags cache ------
if ($ClearUsbFlags) {
    Write-Section 'usbflags descriptor cache'
    if (-not $flagKeys) {
        Write-Host 'Nothing cached - nothing to clear.' -ForegroundColor Green
    } elseif (-not $Execute) {
        Write-Host ('Would delete: {0}' -f ($flagKeys.PSChildName -join ', ')) -ForegroundColor Yellow
    } else {
        Backup-RegKey -RegPath 'HKLM\SYSTEM\CurrentControlSet\Control\usbflags' -Label 'usbflags'
        foreach ($k in $flagKeys) {
            Write-Host ("  deleting usbflags\{0} ..." -f $k.PSChildName) -NoNewline
            try {
                Remove-Item -Path $k.PSPath -Recurse -Force
                Write-Host ' OK' -ForegroundColor Green
            } catch {
                Write-Host " FAILED: $($_.Exception.Message)" -ForegroundColor Red
            }
        }
    }
}

# ------------------------------------------------------- COM name arbiter ---
if ($ResetComDb) {
    Write-Section 'COM Name Arbiter (ComDB)'
    if (-not $Execute) {
        Write-Host 'Would rewrite ComDB, keeping only ports of currently-present devices.' -ForegroundColor Yellow
    } else {
        Backup-RegKey -RegPath 'HKLM\SYSTEM\CurrentControlSet\Control\COM Name Arbiter' -Label 'ComDB'

        # Keep reservations belonging to devices that are actually present.
        $keep = @()
        foreach ($p in $presentPorts) {
            $reg = "HKLM:\SYSTEM\CurrentControlSet\Enum\$($p.InstanceId)\Device Parameters"
            $pn = (Get-ItemProperty -Path $reg -Name PortName -ErrorAction SilentlyContinue).PortName
            if (-not $pn -and $p.FriendlyName -match 'COM(\d+)') { $pn = "COM$($Matches[1])" }
            if ($pn -match 'COM(\d+)') { $keep += [int]$Matches[1] }
        }
        $keep = $keep | Sort-Object -Unique
        Write-Host ('Keeping: {0}' -f $(if ($keep) { $keep -join ', ' } else { '(none present)' }))

        $new = New-Object byte[] 32
        foreach ($n in $keep) {
            if ($n -ge 1 -and $n -le 256) {
                $idx = [int][math]::Floor(($n - 1) / 8)
                $new[$idx] = $new[$idx] -bor (1 -shl (($n - 1) % 8))
            }
        }
        Set-ItemProperty -Path $comDbPath -Name ComDB -Value $new
        $after = Get-ComDbPorts
        Write-Host ('ComDB rewritten. Now reserved ({0}): {1}' -f $after.Count,
            $(if ($after.Count) { $after -join ', ' } else { '(empty)' })) -ForegroundColor Green
    }
}

# ------------------------------------------------------------- usbipd -------
if ($UnbindUsbip) {
    Write-Section 'usbipd bindings'
    $usbipd = Get-Command usbipd.exe -ErrorAction SilentlyContinue
    if (-not $usbipd) {
        Write-Host 'usbipd.exe not installed - skipping.' -ForegroundColor DarkGray
    } else {
        $raw = (Invoke-Native -FilePath $usbipd.Source -Arguments @('list')).Output
        Write-Host $raw

        # Connected rusEFI boards that are bound/shared.
        $busIds = [regex]::Matches($raw, '(?m)^\s*(\d+-\d+)\s+' + $DevVid + ':' + $DevPid + '\s+.*?(Shared|Attached)') |
            ForEach-Object { $_.Groups[1].Value }

        if (-not $busIds) {
            Write-Host 'No connected rusEFI board is currently bound to usbipd.' -ForegroundColor Green
        } elseif (-not $Execute) {
            Write-Host ('Would unbind busid(s): {0}' -f ($busIds -join ', ')) -ForegroundColor Yellow
        } else {
            foreach ($b in $busIds) {
                Write-Host "  usbipd unbind --busid $b ..." -NoNewline
                $r = Invoke-Native -FilePath $usbipd.Source -Arguments @('unbind', '--busid', $b)
                if ($r.ExitCode -eq 0) {
                    Write-Host ' OK' -ForegroundColor Green
                } else {
                    Write-Host (" FAILED: {0}" -f ($r.Output -replace '\s+', ' ').Trim()) -ForegroundColor Red
                }
            }
        }

        # Stale persisted bindings for boards that are long gone.
        $persisted = [regex]::Matches($raw,
            '(?m)^([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12})\s+(.*)$')
        $staleEcu = $persisted | Where-Object { $_.Groups[2].Value -match 'USB Serial Device' }
        if ($staleEcu) {
            Write-Host ''
            Write-Host "Stale persisted rusEFI bindings: $($staleEcu.Count)" -ForegroundColor Yellow
            foreach ($m in $staleEcu) {
                $guid = $m.Groups[1].Value
                if (-not $Execute) {
                    Write-Host ("  would unbind --guid {0}  ({1})" -f $guid, $m.Groups[2].Value.Trim())
                } else {
                    Write-Host "  usbipd unbind --guid $guid ..." -NoNewline
                    $r = Invoke-Native -FilePath $usbipd.Source -Arguments @('unbind', '--guid', $guid)
                    if ($r.ExitCode -eq 0) {
                        Write-Host ' OK' -ForegroundColor Green
                    } else {
                        Write-Host (" FAILED: {0}" -f ($r.Output -replace '\s+', ' ').Trim()) -ForegroundColor Red
                    }
                }
            }
        }
    }
}

# --------------------------------------------------------------- wrap up ----
Write-Section 'Next steps'
if ($Execute) {
    Write-Host '1. Unplug the ECU, wait ~5 seconds, plug it back in.'
    Write-Host '2. Windows re-reads the descriptors and rebuilds the CDC stack.'
    Write-Host '3. Confirm with:  [System.IO.Ports.SerialPort]::GetPortNames()'
    Write-Host ''
    Write-Host 'If the VCP still does not appear, the cache is being rebuilt from an'
    Write-Host 'unchanged bcdDevice. Bump bcdDevice in usbcfg.cpp and reflash - see'
    Write-Host 'README.md in this folder.' -ForegroundColor Yellow
} else {
    Write-Host 'Re-run with -Execute (from an elevated PowerShell) to apply.' -ForegroundColor Yellow
}
Write-Host ''

