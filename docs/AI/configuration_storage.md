# Configuration Storage

How rusEFI persists settings and other runtime records: the unified storage
manager, the three pluggable backends (internal flash, ChibiOS MFS on external
SPI flash, SD card), the double-copy settings write, and the "extra flash
pages" that piggyback on the settings sector.

This is the subsystem touched by PR #9949 ("flash_main: fix for hybrid
INT_FLASH+MFS builds") - see the history note at the end.

Everything below is gated by `EFI_CONFIGURATION_STORAGE`, which is derived
(not set directly) in `firmware/controllers/core/common_headers.h`:

```c
#define EFI_CONFIGURATION_STORAGE (EFI_STORAGE_INT_FLASH == TRUE) || (EFI_STORAGE_MFS == TRUE) || (EFI_STORAGE_SD == TRUE)
```

## Key files

| File | Role |
|------|------|
| `firmware/controllers/storage.h` | `SettingStorageBase` interface, `StorageType`, `StorageItemId`, `StorageStatus` |
| `firmware/controllers/storage.cpp` | Storage manager: backend registry, manager thread + mailbox, pending read/write bitmaps, engine-running write deferral |
| `firmware/controllers/flash_main.cpp` | Settings-record logic: double-copy write, CRC/version validation, read fallback chain, console commands |
| `firmware/controllers/storage_flash.cpp` | `SettingStorageFlash` backend - raw internal MCU flash via `flash_int` |
| `firmware/controllers/storage_mfs.cpp` | `SettingStorageMFS` backend - ChibiOS MFS (Managed Flash Storage), usually on external SPI NOR |
| `firmware/controllers/storage_sd.cpp` | `SettingStorageSD` backend - files on the FatFS SD card |
| `firmware/controllers/extra_flash_pages.cpp/.h` | Extra config pages (second tables, Lua script) piggybacked on the settings sector |
| `firmware/hw_layer/ports/*/mpu_util.cpp` | Per-port `mcuCanFlashWhileRunning()`, `getFlashAddrFirstCopy()`/`getFlashAddrSecondCopy()`, sector geometry |
| `firmware/config/boards/*/board_storage.cpp` | Board glue for MFS: `boardInitMfs()` + `boardGetMfsConfig()` (SPI pins, SNOR driver, bank layout) |
| `firmware/hw_layer/ports/stm32/use_higher_level_flash_api.mk` | Makefile fragment a board includes to turn on `EFI_STORAGE_MFS` |

## Build flags and board combinations

Three independent backend flags; a board may enable any subset:

- `EFI_STORAGE_INT_FLASH` - legacy internal-flash storage. Defaults to `TRUE`
  in `firmware/config/stm32f4ems/efifeatures.h`; boards opt out via
  `DDEFS += -DEFI_STORAGE_INT_FLASH=FALSE`.
- `EFI_STORAGE_MFS` - ChibiOS MFS, enabled by including
  `use_higher_level_flash_api.mk` (which also sets `MFS_CFG_MAX_RECORDS=8`).
  Used by `microrusefi` (when `BOARD_HAS_EXT_FLASH=yes`), `subaru_eg33`,
  `m74_9`, `at_start_f435`.
- `EFI_STORAGE_SD` - settings-adjacent records as files on the SD card
  (requires `EFI_SUPPORT_FATFS`).

Because `EFI_STORAGE_INT_FLASH` defaults on, a board that includes the MFS
fragment without disabling internal flash gets a **hybrid INT_FLASH+MFS**
build (e.g. microrusefi with external W25Q flash). In hybrids the split is:

- Settings (`EFI_SETTINGS_RECORD_ID` + backup) live in **internal flash**:
  `SettingStorageMFS::isIdSupported()` explicitly rejects the two settings IDs
  when `EFI_STORAGE_INT_FLASH == TRUE`.
- Everything else (LTFT, extra pages) can go to MFS - and, for extra pages,
  *also* to internal flash (see the PR #9949 note below).

## Record IDs

`StorageItemId` doubles as the MFS record id (0 is reserved - MFS limitation):

| ID | Name | Content | Producer/consumer |
|----|------|---------|-------------------|
| 1 | `EFI_SETTINGS_RECORD_ID` | `persistentState` (whole `persistent_config_container_s`) | `flash_main.cpp` |
| 2 | `EFI_SETTINGS_BACKUP_RECORD_ID` | second copy of the same | `flash_main.cpp` |
| 3 | `EFI_LTFT_RECORD_ID` | long-term fuel trim tables | `long_term_fuel_trim.cpp` |
| 4 | `EFI_SECOND_TABLES_RECORD_ID` | TS page 4 (secondary VE + blends) | `extra_flash_pages.cpp` -> `second_tables` |
| 5 | `EFI_LUA_PAGE_RECORD_ID` | Lua script config page | `extra_flash_pages.cpp` -> `lua_config_page` |

## The storage manager (`storage.cpp`)

`storages[STORAGE_TOTAL]` is a registry indexed by `StorageType`
(`STORAGE_INT_FLASH`, `STORAGE_MFS_INT_FLASH`, `STORAGE_MFS_EXT_FLASH`,
`STORAGE_SD_CARD`). Backends self-register via `storageRegisterStorage()`;
the SD backend registers/unregisters at runtime as the card comes and goes
(`sdCardGetCurrentMode() == SD_MODE_ECU` gates `isReady()`).

`storageWrite(id, ...)` / `storageRead(id, ...)` iterate **all** registered,
ready backends that claim the id (`isIdSupported`). Writes go to every such
backend (mirroring); the call reports `Ok` if at least one backend succeeded.
Reads also iterate all backends - a later backend's successful read overwrites
the buffer, so with both INT_FLASH and MFS holding a copy the MFS copy
(higher `StorageType` index) effectively wins.

Asynchronous operation goes through a dedicated low-priority thread
("storage manger", `PRIO_STORAGE_MANAGER`, larger stack when MFS/SD is in)
fed by a 16-entry mailbox. Commands: `WRITE`, `WRITE_NOW`, `READ`, `PING`,
`REG`, `UNREG`. Requests set bits in `pendingWrites`/`pendingReads` bitmaps
(bit index = record id); every 100 ms poll the thread retries any pending id
whose backend is available, so a write requested before a backend is ready
(e.g. SD not yet mounted) completes later without the caller caring.

Write deferral: `storageAllowWriteID()` blocks settings writes while the
engine is spinning **if** the MCU stalls on internal flash writes.
`mcuCanFlashWhileRunning()` is per-port: F4 always false, F7 true only for
dual-bank 2MB parts without the `EFI_FLASH_USE_1500_OF_2MB` layout, H7 true
except STM32H723. Boards can also veto via the `custom_board_allowFlashNow`
override. A `WRITE_NOW` command (`writeToFlashNow()` / console `writeconfig`)
bypasses this check entirely. When the write does stall the CPU,
`SettingStorageFlash::store()` re-arms the watchdog with
`WATCHDOG_FLASH_TIMEOUT_MS` around the erase/write.

Reboot coordination: `storageIsBusy()` reports queued-or-in-flight work
(mailbox depth + pending bitmap); `storageWaitIdle(timeoutMs)` polls it, with
`STORAGE_WAIT_IDLE_TIMEOUT_MS` (10 s) sized to cover a double-copy settings
write plus MFS worst-case garbage collection. Builds without any storage get
link-time stubs so reboot paths still work.

## Settings write and read (`flash_main.cpp`)

`writeToFlashNowImpl()` (executed on the manager thread):

1. Reset the config burn timer (`suspendLinearTimeWatcher`).
2. Stamp `persistentState.size`, `.version` (`FLASH_DATA_VERSION`), `.crc`.
3. `storageWrite(EFI_SETTINGS_RECORD_ID, ...)` - on the INT_FLASH backend
   this erases the primary sector, writes, verifies erased-ness before write,
   and compares back after write.
4. On `EFI_STORAGE_INT_FLASH` builds, if step 3 succeeded:
   `burnExtraFlashPages()` rewrites all extra pages into the just-erased
   shared sector (see below). Since PR #9949 this runs on hybrid
   INT_FLASH+MFS builds too.
5. `storageWrite(EFI_SETTINGS_BACKUP_RECORD_ID, ...)` - second copy. Some
   MCUs lack room for it; `NotSupported` for the backup is not a failure.

`readConfiguration()` reads the first copy, validates CRC + version + size,
falls back to the backup copy on any failure. `readFromFlash()` then maps the
result: `CrcFailed`/`NotFound`/`Failed`/`NotSupported` -> full reset to
`DEFAULT_ENGINE_TYPE` (with a warning for CRC failure); a CRC that is all 1s
is interpreted as blank flash (fresh chip), not corruption;
`IncompatibleVersion` -> reset but preserving the engine type.

Console commands (registered in `initFlash()`): `readconfig`, `writeconfig`,
`burnconfig` (deferred until engine stop), `resetconfig`, `rewriteconfig`,
`ltftwrite`.

Startup order (`rusefi.cpp` / `engine_configuration.cpp`): `initFlash()` ->
`initStorage()` -> `readFromFlash()` -> `loadExtraPages()` - extra pages are
loaded immediately after the main config, before TunerStudio connectivity,
so a validation failure cannot leave all-zero extra pages that a subsequent
TS burn would persist.

## Extra flash pages (`extra_flash_pages.cpp`)

Extra pages (TS page 4 second tables, Lua config page) are RAM-resident
structs wrapped in `ExtraPageContainer<TData, Version>` - version tag + data +
CRC32, `alignas(32)` for STM32H7 flash-word writes.

On INT_FLASH boards they do not get their own sectors: they live at **fixed
byte offsets inside the primary settings sector** - `PAGE4_SECTOR_OFFSET` =
72 KB, `LUA_PAGE_SECTOR_OFFSET` = 76 KB. Fixed offsets are deliberate: an
offset derived from `sizeof(persistent_config_container_s)` would shift when
the config struct grows and silently corrupt reads across firmware updates
(a `static_assert` keeps the config below 72 KB).

Consequences of sharing the sector:

- An extra page can only be written **immediately after a main-config burn
  has erased the sector** (`SettingStorageFlash::store()` skips erase for
  extra-page ids and fails if the area is not blank). Hence
  `burnExtraFlashPage(id)` on INT_FLASH-only boards simply triggers a full
  `writeToFlashNow()`, which piggybacks all extra pages via
  `burnExtraFlashPages()`.
- Reading a blank extra-page area returns `NotFound` -> defaults are used.
- Exception: the usual STM32F7 build without `EFI_FLASH_USE_1500_OF_2MB` is a
  dual-bank 2 MB device with settings starting in 16 KB sectors. The current
  settings payload does not erase as far as the fixed 72/76 KB offsets, so a
  repeated piggyback write would fail. Because bank mode and flash size are
  detected only at runtime, the compile-time guard conservatively makes
  internal extra pages unsupported for every unflagged F7 build. Such builds
  need another enabled and ready backend, normally SD; without one, extra
  pages revert to defaults after reboot.

On MFS/SD boards (and the simulator), `burnExtraFlashPage(id)` writes just
that record directly to all backends.

`resetExtraPages()` is called from `resetConfigurationExt()` so every config
reset (preset apply, version mismatch, CRC failure) regenerates extra pages
consistent with page 1.

## MFS backend specifics

ChibiOS MFS provides wear-leveled record storage over two banks with garbage
collection. Board glue supplies the SNOR driver and `MFSConfig`; e.g.
microrusefi uses a W25Q 8 MB SPI NOR with 2 x 512 KB banks - a settings write
takes ~147 ms normally and ~4.75 s when GC kicks in (roughly every 19th
write), which is why `STORAGE_WAIT_IDLE_TIMEOUT_MS` is generous.

MFS init can take significant time, so `initStorage()` avoids it on the main
init path when possible: if the INT_FLASH backend is up (i.e. settings have a
home), MFS registration is deferred to the manager thread (`MSG_CMD_REG`).
Only when MFS is the *sole* settings backend is it initialized synchronously,
bracketed by `startWatchdog(WATCHDOG_MFS_START_TIMEOUT_MS)` because no thread
is feeding the watchdog yet. `initStorageMfs()` returns false (backend simply
absent) if `boardInitMfs()` or `mfsStart()` fails - e.g. the optional
external flash chip is missing or dead.

## SD backend specifics

`SettingStorageSD` stores only non-settings records, as files in the SD root:
`ltft.bin`, `second_tables.bin`, `lua_script.bin`. It requires the card in
`SD_MODE_ECU` (not handed to the PC as USB mass storage) and takes the
FatFS `FsGuard` lock around each operation, coexisting with SD logging. It is
registered/unregistered dynamically as the card mounts/unmounts.

## History: PR #9949 (July 2026)

`writeToFlashNowImpl()` used to guard the extra-pages piggyback with
`#if (EFI_STORAGE_INT_FLASH == TRUE) && (EFI_STORAGE_MFS != TRUE)` - on
hybrid builds extra pages were assumed to live in MFS only. But the settings
burn still erases the whole internal-flash sector, and on hybrids the
INT_FLASH backend *does* map extra-page ids, so:

- if the optional MFS hardware was absent or failed to init, extra pages had
  no working backend at all and were wiped by every settings burn;
- even with MFS healthy, the internal-flash copy of the extra pages was left
  blank/stale after every burn.

The fix (commit `af6f0e93802`) relaxes the guard to plain
`#if EFI_STORAGE_INT_FLASH == TRUE`: after a successful settings write the
extra pages are always rewritten into the now-blank shared region, MFS or
not. `storageWrite()` mirroring means MFS (when present) still receives its
copy. Review discussion noted the storage code paths have accumulated
complexity and suggested eventually migrating everything to the unified
ChibiOS MFS implementation (internal-flash MFS included - the so-far-unused
`STORAGE_MFS_INT_FLASH` slot).
