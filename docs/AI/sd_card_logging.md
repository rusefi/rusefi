# SD Card Logging

How rusEFI writes engine logs to the SD card: the SD thread and its mode state
machine, the two log formats (.mlg and .teeth), the write/buffering path, and the
f_expand pre-allocation strategy that protects the filesystem from power loss.

Everything below is gated by `EFI_FILE_LOGGING`.

## Key files

| File | Role |
|------|------|
| `firmware/hw_layer/mmc_card.cpp` | SD thread: card init, mount, mode state machine, log file lifecycle |
| `firmware/hw_layer/mmc_card_util.cpp` | Persistent log file index (`rusefi.ind`), `incLogFileName()` |
| `firmware/console/binary_mlg_log/binary_mlg_logging.cpp` | MLG binary format writer (header + data records) |
| `firmware/console/binary/tooth_logger.cpp` | Trigger tooth ("composite") logger: RAM buffers + .teeth/CSV file output |
| `firmware/controllers/system/file_writer.h` | `FileBufferedWriter`: 512-byte buffered writer on top of FatFS `f_write` |
| `firmware/controllers/modules/sd_log_trigger/` | Conditional logging: start/stop based on RPM/TPS/MAP/VSS or a button |
| `firmware/ext/FatFS/` | FatFS library, project config in `ffconf.h` (`FF_USE_EXPAND 1`) |

## Hardware paths

Two mutually exclusive block-device backends, selected at build time per board:

- **SPI** (`HAL_USE_MMC_SPI`): MMC-over-SPI via ChibiOS `MMCD1`. CS pin and SPI
  device come from `engineConfiguration->sdCardCsPin` / `sdCardSpiDevice`.
  On successful `blkConnect()` the SPI bus is locked *permanently* - the SD card
  takes exclusive ownership of that SPI device.
- **SDIO/SDMMC** (`EFI_SDC_DEVICE`): 4-bit native interface, faster.

## The SD thread and mode state machine

A single thread (`MMCmonThread`, "SD Card Logger") owns the card, the FatFS mount
and the shared `resources` union (one `FIL` descriptor / format buffer / MSD block
buffer - the modes are exclusive so the memory is unioned). Other threads must not
touch these; `sdTestWrite1Mb()` shows how to do independent file I/O with its own
static `FIL`.

Boot sequence: wait for `boardSdCardEnable()`, check backup RAM state from the
previous boot (counts "unsafe unmount" power-offs while mounted), init the card
(give up until next reboot on failure), then briefly mount once to drop/scan crash
report files (`errorHandlerWriteReportFile`). After that it loops forever:

1. `sdModeSelector()` picks the desired mode: a user request (`sdmode` console
   command or TS) always wins and sticks until power-off; otherwise USB connected
   -> `PC` (mass storage), no USB and no ignition voltage -> `UNMOUNT` (power-off
   imminent), else -> `ECU` (logging). `alwaysWriteSdCard` forces ECU mode even
   with USB present.
2. `sdModeSwitcher()` transitions: every switch first passes through `IDLE`
   (`sdModeSwitchToIdle()` stops the logger + unmounts, or detaches MSD), then
   enters the target mode (mount + start logger, attach MSD, or format).
3. `sdModeExecuter()` does one iteration of the current mode's work; only ECU
   mode has real work - everything else sleeps 100ms.

Modes: `IDLE`, `ECU` (logging), `PC` (card exposed to host as USB mass storage),
`UNMOUNT`, `FORMAT`. Current mode/status are persisted to battery-backed RAM
(`SdBackupState`) on every change so the next boot can detect unclean shutdown.

Note the CDC console + MSD composite-USB caveat in CLAUDE.md: switching PC -> ECU
yanks the medium from the host and usually drops the USB console connection.

## ECU (logging) mode

Each executer iteration first checks the `SdLogTrigger` module (`shouldLog()`):
conditional logging closes the current file when trigger conditions stop being met,
so each logging session lands in its own file. When logging is allowed, one of two
loggers runs, chosen by `engineConfiguration->sdTriggerLog`:

### Regular .mlg log (`sdLoggerMlg` -> `MLG::writeSdLogLine`)

- File format: MegaLogViewer binary MLG version 2
  (https://www.efianalytics.com/TunerStudio/docs/MLG_Binary_LogFormat_2.0.pdf).
  First call writes the file header (`MLVLG`, format 02) plus one descriptor per
  field; every later call appends one fixed-size data record: 4-byte prefix
  (block type, rolling counter, 16-bit timestamp at 10us) + all field values +
  1-byte additive checksum.
- The field list is the generated `log_fields_generated.h`; count is reported by
  `sdinfo` as "SD card fields".
- Write rate: `sdCardLogFrequency` Hz, clamped to 1..250, paced with
  `chThdSleepUntilWindowed()` in `mlgLogger()`.
- `f_sync` runs only every `F_SYNC_FREQUENCY` (10) lines, not per line.
- Rollover: when the buffered size would cross `LOGGER_MAX_FILE_SIZE` (32MB) the
  file is closed and a new one started.
- `MLG::resetFileLogging()` must be called per new file so the header is written
  again and the rolling counter restarts.

### Trigger tooth .teeth log (`sdLoggerTooth`)

Used when `sdTriggerLog` is enabled; records every trigger/cam edge, coil,
injector, sync, TDC event as a packed `composite_logger_s`. The tooth logger
double-buffers in the shared BigBuffer region between interrupt-context producers
and the SD thread. `ToothLoggerWriter()` waits up to 3 seconds for a filled
buffer; on timeout it flushes and closes the file, and the next tooth event opens
a fresh file. Format is raw binary records, or CSV when `sdTriggerLogCsv` is set
(format is latched at file creation, no on-the-fly switch). No 32MB rollover
check applies to .teeth files.

### File naming

`prepareLogFileName()`: `re_<date-time>.mlg`/`.teeth` when the RTC has a valid
date, otherwise `re_<index>` where the index is persisted in `rusefi.ind` on the
card (`incLogFileName()` in `mmc_card_util.cpp`).

## Write path and the f_expand pre-allocation logic

All log writes go through `FileBufferedWriter` (`file_writer.h`), a 512-byte
`BufferedWriter` over `f_write()`. On any FatFS write error it latches
`failed = true` and the logger closes the file and stops.

The important power-loss defense is in `sdLoggerCreateFile()` /
`sdLoggerCloseFile()` (`mmc_card.cpp`):

- On file create, `f_expand(fd, LOGGER_MAX_FILE_SIZE, 1)` pre-allocates the full
  32MB as a *contiguous* cluster chain (opt=1 = allocate now, not just find;
  requires `FF_USE_EXPAND 1` in `ffconf.h`, plus `FF_FS_READONLY 0` and
  `FF_FS_MINIMIZE 0`).
- All FAT/allocation-table updates therefore happen once, up-front. Subsequent
  `f_write()` calls inside the pre-allocated area only write data sectors and
  never touch FAT structures, so a sudden power cut mid-logging can lose
  buffered/unsynced data but should not corrupt the filesystem itself.
- `f_expand()` failure (e.g. fragmented card with no 32MB contiguous run, or
  card smaller than the request) is deliberately non-fatal: logging proceeds and
  FatFS falls back to normal cluster-at-a-time allocation on write - functional,
  but without the corruption protection.
- On close, `f_truncate()` shrinks the file back to the actually-written size,
  returning the unused tail of the pre-allocation to free space. A file that was
  never closed cleanly shows up as a full 32MB file with trailing garbage - MLG
  readers stop at the last valid record.
- Both loggers pre-allocate (they share `sdLoggerCreateFile()`), but only the
  .mlg logger enforces the 32MB size cap/rollover; a .teeth file that outgrows
  the pre-allocation silently degrades to normal allocation.

## Status reporting and console commands

Output channels: `sd_present`, `sd_error` (last FRESULT), `sd_formating`,
`sdLoggingState` (SdLogTrigger state), `sd_logging_internal` (logger active),
`sd_active_wr`/`sd_active_rd` (block device busy).

Console: `sdinfo` (status, pinout, SPI clocks, current file), `sdmode
<pc|ecu|off|unmount|auto|format>`, `sdsuppresslogging`, `sd_test_write1mb`
(throughput test), `del <file>`, `delreports`.

## Simulator

The simulator variant of `FileBufferedWriter` writes `rusefi_simulator_log.mlg`
in the working directory; `initMmc()`/`mountMmc()` are stubbed to succeed.
