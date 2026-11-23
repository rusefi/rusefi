# OpenBLT Validation of the Main Firmware

How the OpenBLT bootloader decides that a main-firmware ("user program") image
is valid: what is checked while an update is being flashed, what is checked
before the bootloader jumps to the application, and which callbacks take part.

Applies to boards built with `USE_OPENBLT=yes`. The bootloader is a separate
ChibiOS binary built from `firmware/bootloader/`; the vendored OpenBLT core is
`firmware/ext/openblt/Target/Source/`, and rusEFI replaces the OpenBLT target
port with its own files in `firmware/bootloader/openblt_chibios/`.

## Summary

- The only integrity check is a **CRC32 embedded in the image at build time**.
  The bootloader never computes or writes a checksum itself.
- The CRC is verified **only before starting the application**
  (`FlashVerifyChecksum()`), never while data is arriving.
- During flashing the bootloader validates the *operation* (address range,
  flash controller status, supply voltage), not the *content*. There is no
  read-back compare and no per-packet checksum above the transport.
- A partially written image is caught at the next start attempt: the CRC does
  not match and the bootloader stays active, waiting for a new update.
- There is no signature, seed/key, or board-compatibility check in the
  bootloader (`BOOT_XCP_SEED_KEY_ENABLE` is 0). Board matching is done on the
  host side (station ID / bundle target).

## Image header: CRC and size

The application's Cortex-M vector table has reserved entries. Two of them
carry the validation metadata, relative to the application base address
(`.vectors` section):

| Offset | Content |
| ------ | ------- |
| `0x1C` | CRC32 of the image, little-endian |
| `0x20` | image size in bytes, little-endian |

The CRC covers bytes `[0, 0x1C)` and `[0x20, size)`: the whole image except
the CRC word itself. The size field is included in the CRC. The algorithm is
the standard reflected CRC32 (`crc32()` / `crc32inc()` from
`firmware/libfirmware/util/include/rusefi/crc.h`, same result as zlib).

The fields are stamped after linking by `hex2dfu -c <address>`
(`misc/encedo_hex2dfu/`), driven from `firmware/bundle.mk`:

- `HEX_BASE_ADDRESS` is read from the ELF `.vectors` section with objdump and
  `CHECKSUM_ADDRESS = HEX_BASE_ADDRESS + 0x1C`.
- `$(BUILDDIR)/rusefi.srec` (the OpenBLT update image) is produced from
  `$(DBIN_CRC)`, the CRC-stamped binary, re-addressed to `HEX_BASE_ADDRESS`.
- `.h2d-sentinel` stamps the composite `rusefi.bin`/`rusefi.dfu`
  (bootloader + application) with the same CRC address.
- Images that skip this path carry no valid CRC and are refused by the
  bootloader: the plain `build/rusefi.bin`/`.hex`/`.elf` straight from the
  linker, non-Cortex-M targets, and boards with `BOARD_IMAGE_SCRIPT` (which
  produce their own addressed images).

The base address the bootloader uses comes from `FlashGetUserProgOffset()` in
`openblt_flash.cpp`: `BOARD_BOOTLOADER_SIZE` if defined, otherwise 128 KiB on
H7 and 32 KiB elsewhere. The application's linker script must place
`.vectors` at the same offset (`bootloader_size` in `STM32F4.ld` and
`STM32F7.ld` has the same 32 KiB default and `BOARD_BOOTLOADER_SIZE`
override), or the bootloader looks for the header in the wrong place.

## Start-up validation

### Boot flow

`main()` in `firmware/bootloader/bootloader_main.cpp`:

1. `halInit()`, `chSysInit()`, `baseMCUInit()`, `SharedParamsInit()`.
2. `checkIfResetLoop()` reads the MCU reset cause and updates the watchdog and
   software reset counters in the shared-params RAM area (indexes 1 and 2).
   It returns true above `maxWdRebootCounter` (10) or `maxSwRebootCounter`
   (15) consecutive resets.
3. `checkIfRebootIntoOpenBltRequested()` reads and clears shared param
   index 0, set by the application's `jump_to_openblt()`.
4. `stayInBootloader` = stay requested or reset loop detected.
5. `BootInit()` (OpenBLT core: `NvmInit()`, communication interfaces,
   `BackDoorInit()`), then the main loop calls `BootTask()`.

The stock OpenBLT backdoor timer is disabled: `BOOT_BACKDOOR_HOOKS_ENABLE` is
1 and `BackDoorEntryHook()` (`firmware/hw_layer/openblt/hooks.c`) always
returns `BLT_TRUE`, so `BackDoorInit()`/`BackDoorCheck()` never start the
application. The main loop decides instead:

- while a host is connected (`ComIsConnected()`), keep running and latch
  `wasConnected`;
- if `stayInBootloader` or `wasConnected`, never auto-start;
- otherwise, once uptime reaches `BOOT_BACKDOOR_ENTRY_TIMEOUT_MS` (500 ms
  default, 0 on several boards, see `board.mk` files), call
  `CpuStartUserProgram()`. If it returns, the image was rejected and the loop
  retries on every pass.

With a zero timeout, `Rs232Init()` in `openblt_usb.cpp` starts USB serial only
when `stayInBootloader` is set or `NvmVerifyChecksum()` fails, so a board with
a valid image boots without enumerating the bootloader's USB device.

### `CpuStartUserProgram()`

`firmware/bootloader/openblt_chibios/openblt_chibios.cpp`. Order of events:

1. `NvmVerifyChecksum()`; on failure return (bootloader stays active).
2. `ComFree()`, Ethernet MAC/PHY shutdown when `BOOT_COM_NET_ENABLE`.
3. `chSysDisable()`, `__cpu_deinit()` (caches off, all MPU regions cleared).
4. `SCB->VTOR` = application base, jump to the reset handler at base + 4.

`CpuUserProgramStartHook()` is not used (`BOOT_CPU_USER_PROGRAM_START_HOOK`
is 0); the copy in `hooks.c` is dead demo code.

### `FlashVerifyChecksum()`

Call chain: `CpuStartUserProgram()` -> `NvmVerifyChecksum()` (`nvm.c`) ->
`FlashVerifyChecksum()` (`openblt_flash.cpp`). Checks, in order:

1. The first 4 bytes at the application base are not erased
   (`intFlashIsErased()`), otherwise invalid.
2. The size at `0x20` is not larger than
   `flashSizeKb() * 1024 - FlashGetUserProgOffset()`, otherwise invalid. An
   erased size field (`0xFFFFFFFF`) fails here.
3. CRC32 over `[0, 0x1C)` then `[0x20, size)` equals the word at `0x1C`.

Limits of this check:

- There is no lower bound on the size. A size below `0x20` makes the second
  CRC length wrap around; the header is never in that state for an image
  produced by the build.
- The stack pointer and reset vector are not sanity-checked separately; they
  are covered only by the CRC.
- The CRC runs over the whole image on every boot, before the application
  starts; the watchdog is not serviced inside the loop.

`BOOT_NVM_CHECKSUM_HOOKS_ENABLE` is 0 and rusEFI's `nvm.c` does not call
`NvmVerifyChecksumHook()`/`NvmWriteChecksumHook()`; the copies in `hooks.c`
are unused.

### Reset-loop protection

Validation by CRC cannot detect an image that is intact but crashes. The reset
counters cover that case: the bootloader increments them on watchdog or
software resets and clears them on any other reset cause; the application
clears both 5 s after boot (`errorHandlerResetCounters()`, called from
`engine_controller.cpp`). When a counter exceeds its limit the bootloader
stays active with a valid image in flash, so the ECU can be re-flashed.

The blinky thread shows state: red LED solid after a watchdog reset, slower
blinking once the start attempt has passed the timeout (image rejected or
stay requested), and three times faster blinking when an F7 is not in
dual-bank mode (`isFlashDualBank()`).

## Validation while flashing

### Host sequence

The console's `OpenBltFlasher` and `XcpLoader`
(`java_console/ecu_shared/.../maintenance/`, `java_console/io/.../libopenblt/`)
drive the XCP session: CONNECT, PROGRAM_START, erase the whole target range
(SET_MTA + PROGRAM_CLEAR), write ascending chunks (SET_MTA + PROGRAM_MAX /
PROGRAM), then a zero-length PROGRAM and PROGRAM_RESET.

Erase-all-then-write-ascending is what makes the start-up CRC effective for an
interrupted update: the header is written in the first chunk, the tail stays
erased, and the CRC over the declared size fails.

### Bootloader callbacks

XCP handlers in `ext/openblt/Target/Source/xcp.c` call the NVM layer in
`firmware/bootloader/openblt_chibios/nvm.c`, which calls the flash driver in
`openblt_flash.cpp`, which uses `firmware/hw_layer/ports/stm32/flash_int.cpp`:

| XCP command | NVM function | rusEFI flash function | What is validated |
| ----------- | ------------ | --------------------- | ----------------- |
| CONNECT | `NvmInit()` | `FlashInit()` | nothing; on H7 resets the write buffer and clears the sticky failure flag |
| PROGRAM_CLEAR | `NvmErase()` | `FlashErase()` | address is not below the application base; skips the erase if the range is already blank; `intFlashErase()` checks controller errors and that each sector reads blank afterwards |
| PROGRAM, PROGRAM_MAX | `NvmWrite()` | `FlashWrite()` | address is not below the application base; `intFlashWrite()` checks supply voltage (PVD) before and during the write and the flash status flags after each word |
| PROGRAM with length 0 | `NvmDone()` | `FlashWriteChecksum()`, `FlashDone()` | `FlashWriteChecksum()` is a no-op returning true; `FlashDone()` flushes the last H7 buffer line |
| PROGRAM_RESET | - | `CpuStartUserProgram()` | full CRC check described above |

Any failure returns `BLT_FALSE`, which the XCP layer reports to the host as
`XCP_ERR_GENERIC`; the host aborts the update.

Notes:

- The bootloader region is protected only by the lower-bound address check in
  `FlashWrite()`/`FlashErase()`. There is no upper-bound check there; on H7
  `intFlashWrite()`/`intFlashErase()` additionally reject ranges outside the
  flash.
- Written data is not read back and compared. Corruption that the flash
  controller does not flag is detected only by the CRC at start.
- H7 programs whole 32-byte ECC lines: `FlashBufferedWrite()` collects bytes
  into `flashBuffer` and `FlashBufferFlush()` writes a line when the address
  leaves it. A failed flush sets `flashWriteFailed`, which fails every later
  write and the final `FlashDone()` until the next CONNECT.
- `BOOT_NVM_HOOKS_ENABLE` defaults to 0. When a board enables it,
  `NvmWriteHook()`/`NvmEraseHook()` run first and can claim an address range
  (external memory) by returning something other than `BLT_NVM_NOT_IN_RANGE`,
  and `NvmDoneHook()` can veto completion. The defaults in `hooks.c` are weak
  no-ops.
- `NvmReinit()` exists in `nvm.c` but the OpenBLT core does not call it; a new
  session re-runs `NvmInit()` from the CONNECT handler.

### After PROGRAM_RESET

PROGRAM_RESET does not reset the MCU. `XcpCmdProgramReset()` calls
`CpuStartUserProgram()` directly:

- valid CRC: the bootloader jumps to the new application and never answers
  the command. `XcpLoader.sendCmdProgramReset()` treats the missing response
  as success.
- invalid CRC: `CpuStartUserProgram()` returns, the bootloader sends a
  positive response and keeps running. Because `wasConnected` is latched, it
  does not retry the start on its own and waits for another session or a
  reset. The host sees the same outcome in both cases; a failed validation is
  visible only as the ECU remaining in bootloader mode.

## Related mechanisms

- **Entering the bootloader from the application**: `jump_to_openblt()`
  (`stm32_common.cpp`) writes shared param 0 and resets. It is triggered by
  the `CMD_REBOOT_OPENBLT` console command and by an OpenBLT XCP CAN frame on
  `BOOT_COM_CAN_CHANNEL_INDEX` (`can_rx.cpp`). The shared-params buffer lives
  in the `.shared` RAM section and carries its own checksum
  (`firmware/hw_layer/openblt/shared_params.c`).
- **Station ID / version probes**: `XcpPacketReceivedHook()` in `hooks.c`
  answers `0xBF` with `XCP_STATION_ID` and `?` with the OpenBLT core version.
  `XcpLoader.requireStationIdCheck()` uses the station ID on the host to
  refuse the wrong target (used for wipe images); the bootloader itself does
  not compare it against the image.
- **Emergency wipe image**: boards with `OPENBLT_WIPE_FLASH_END_EXCLUSIVE`
  ship an erase-only SREC; after it the application base is blank and
  `FlashVerifyChecksum()` fails at its first check, leaving the bootloader
  active.
- **Bootloader version marker**: the `BLxx` marker at `0x08000024` identifies
  the bootloader build and is unrelated to application validation
  (`search:openblt_version`).
- **Application side**: the application does not verify its own CRC.
