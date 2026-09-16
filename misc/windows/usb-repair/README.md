# Windows USB / VCP repair

Tooling for the case where a rusEFI ECU stops presenting its Virtual COM Port
(VCP) on Windows - most often right after a firmware change to the USB composite
descriptor.

## The root cause

Windows caches USB descriptor and driver-binding state per device, and the cache
key is `idVendor + idProduct + bcdDevice`. rusEFI ships:

| Field       | Value    | Where                                                          |
|-------------|----------|----------------------------------------------------------------|
| `idVendor`  | `0x0483` | `firmware/hw_layer/ports/stm32/serial_over_usb/usbcfg.cpp`     |
| `idProduct` | `0x5740` | same                                                            |
| `bcdDevice` | `0x0200` | same                                                            |

-> cache key `048357400200`, under
`HKLM\SYSTEM\CurrentControlSet\Control\usbflags\`.

`NUM_INTERFACES` and `DESCRIPTOR_SIZE` in `usbcfg.cpp` are computed from
`HAL_USE_USB_MSD` / `HAL_USE_USB_CDC_2` (and 3/4), so enabling a second VCP
genuinely changes the composite layout - the interface count, the endpoint
allocation, and `wTotalLength`. But `bcdDevice` does not change, so the cache key
does not change either. Windows therefore keeps applying the layout it learned
from the *old* firmware, and the new interfaces never bind. The symptom is a VCP
that silently fails to appear on hosts that had previously seen the board, while
a never-before-used machine works fine.

Three separate stores accumulate this stale state:

1. **Ghost device instances** - every board ever plugged in stays in the driver
   database with `Present = False`.
2. **`usbflags` descriptor cache** - the entry described above.
3. **COM Name Arbiter (`ComDB`)** - a 32-byte bitmap reserving COM numbers.
   Ghost boards hoard the low range, so each new board lands on a higher COM
   number until tools that scan a limited range stop finding it.

A fourth, unrelated-but-easy-to-confuse cause: if the board is bound to
**usbipd-win** (for WSL passthrough) it is attached to the `VBoxUSB` stub driver
and shows in Device Manager as *USBIP Shared Device*. In that state Windows
never builds a CDC stack and **no COM port is created at all**. The script
detects and reports this explicitly.

## The proper fix (firmware side)

**Bump `bcdDevice` in `usbcfg.cpp` whenever the composite layout changes.**

```c
0x0200,        /* bcdDevice.                       */
```

A new value yields a new cache key, so every Windows host in the field re-reads
the descriptors instead of needing manual cleanup. Treat it like the OpenBLT
`BLxx` marker: a deliberate declaration that the USB-visible shape changed.

The script is the remedy for hosts that already cached the stale layout.

## The workaround (host side)

`fix-vcp.ps1` clears all three stores and optionally releases a usbipd binding.
It only ever removes devices with `Present = False`, and it exports every
registry key it touches to `backup/*.reg` before modifying it. No driver package
is uninstalled - replugging the board recreates everything.

```powershell
# read-only: show exactly what would change
powershell -ExecutionPolicy Bypass -File fix-vcp.ps1 -All

# apply (requires an elevated PowerShell)
powershell -ExecutionPolicy Bypass -File fix-vcp.ps1 -Execute -All
```

To start an elevated shell: `Start-Process powershell -Verb RunAs`

### Switches

| Switch           | Effect                                                                 |
|------------------|------------------------------------------------------------------------|
| *(none)*         | Dry run. Reports state, changes nothing.                               |
| `-Execute`       | Actually apply. Removes ghost device instances.                        |
| `-ClearUsbFlags` | Delete cached `usbflags` entries for `0483:5740`. The descriptor fix.  |
| `-ResetComDb`    | Rewrite the COM arbiter bitmap, keeping only present devices' ports.   |
| `-UnbindUsbip`   | `usbipd unbind` any bound rusEFI board, and drop stale persisted binds.|
| `-All`           | All three of the above.                                                |

Ordering is handled internally: caches are cleared before the usbipd unbind, so
the board enumerates fresh into a low COM number.

After running: unplug the ECU, wait ~5 s, replug, then confirm with

```powershell
[System.IO.Ports.SerialPort]::GetPortNames()
```

### Restoring a backup

```powershell
reg import backup\ComDB-<timestamp>.reg
```

## Notes

- usbipd warns that the `EUsbHubFilter` (Eltima) and `USBPcap` filter drivers are
  incompatible with it, which is why `bind --force` ends up being used on
  machines that have either installed. `USBPcap` in particular is commonly
  present on rusEFI dev boxes for MSD/CDC sniffing.
- Clearing `ComDB` also drops reservations held by virtual-serial-port software
  (e.g. Eltima VSPD). Those tools re-request a number when they next create a
  port, so this is normally harmless - but drop `-ResetComDb` if you have
  virtual pairs pinned to specific COM numbers.
