# Firmware Debugging Notes

Hardware-level pitfalls and debugging evidence: ChibiOS ADC, nested locks, AT32, H7, faults, MPU, reboots, OpenBLT.

### ChibiOS ADC linear-callback state

Current ChibiOS invokes a non-circular ADC conversion's completion callback
after returning the driver to `ADC_READY`. `adcIsBufferComplete()` is meaningful
only for circular conversions, where it distinguishes full- from half-buffer
callbacks. A linear callback that chains another conversion must not guard on
`adcIsBufferComplete()` or manually assign the driver state; call the I-class
start API directly from the callback (see `slowAdcEndCB()` in
`firmware/hw_layer/ports/stm32/stm32_adc_v2.cpp`).

### Nested system locks: caught only by the simulator

Code running under `chibios_rt::CriticalSectionLocker`/`chSysLock*` (for
example `ToothLoggerBufferPool::appendI()`) must call only I-class functions.
Calling a helper that takes its own locker, such as
`TriggerCentral::getCurrentEnginePhase()`, makes ChibiOS state checking halt
the simulator with `chSysHalt: SV#4` (thread) or `SV#6` (ISR). Unit tests have
no lock state checker and pass; the simulator functional test (CI
`build-simulator.yaml`) is the first place this shows up. Use or add an `...I`
variant (`getCurrentEnginePhaseI()`).

### AT32 port

- `ports/at32/` (Artery AT32F435) is not used at the moment: both AT32 boards (`at_start_f435`, `m74_9`) are disabled (`meta-info.disabled_env`), so no CI build exercises this port
- AT32 uses STM32-named compatibility headers, but those names do not establish register semantics. For example, Artery CRM_CTRLSTS bit 25 is reserved even though the compatibility header defines RCC_CSR_BORRSTF there. Check the official Artery register layout before porting STM32 low-level code.
- AT32 RTC backup registers are separate from backup SRAM: `EFI_BACKUP_SRAM=FALSE` does not mean the chip lacks RTC backup registers. Artery's AT32F435/437 SDK places ERTC at 0x40002800 and its twenty 32-bit data registers at offsets 0x50..0x9C, matching the compatibility header's RTC/BKP registers. This permits reuse of the STM32 `backupRamLoad`/`backupRamSave` implementation; the STM32 backup-SRAM object remains excluded on AT32. Persistence across actual power loss still depends on board backup power and clock configuration.
- The AT32 ChibiOS port uses the older SPI API (`end_cb`) even with newer ChibiOS RT configuration versions: its hal_lld.h does not select HAL_LLD_SELECT_SPI_V2. Do not infer SPIConfig fields solely from the RT version.

For H7 ADC mux work, verify DMA placement against the linked ELF and MPU settings,
not just the `NO_CACHE` name: H743 and H723 use different non-cacheable regions.
Build commands, host callback tests, and hardware checks: [H7 ADC mux](docs/h7-adc-mux.md).

- **Null-pointer derefs on STM32 look like wild-pointer bus faults, not faults near 0**: address 0x0 is the readable ITCM alias of flash (the vector table — the bootloader's on OpenBLT boards), so reading through a null struct pointer *succeeds* and returns flash image content; only a *subsequent* hop through that junk faults, with BFAR = junk_value + member_offset. Decoding rule for HardFault dumps: if `faulting_reg + ldr_offset == BFAR` and the reg value is not a valid RAM/flash *pointer*, check whether the reg value equals the flash *content* at `0x00000000 + ldr_offset` — that proves a null-head/null-pointer walk, not memory corruption (this is exactly how issue #9435, the `TriggerScheduler::cancel` null-head `LL_DELETE2` walk, was decoded). Related: rusEFI's vendored utlist `LL_DELETE2`/`LL_APPEND2` dereference the list head without a null check on their search-loop branches.
- **Text-logging line buffers are not guaranteed null-terminated - never `strlen` them**: `efiPrintf` formats into a 256-byte `LogLineBuffer` with `chvsnprintf` (which always terminates at or before `buffer[size-1]` and returns the *untruncated* length); any longer line (a big Lua `print()`, a long `%s`) is truncated. `priv::terminateLogLine()` re-adds the trailing `LOG_DELIMITER` while preserving the terminator, and `LogBuffer::writeInternal()` bounds its read with `memchr(..., maxLength)`. The pre-#10159 code overwrote `buffer[255]` with the delimiter, so the flusher's `strlen` walked out of the `lineBuffers[]` array and rebooted the ECU under heavy Lua printing. Keep both invariants when touching `loggingcentral.cpp`.
- **Stack overflow on F4 is silent and hits the neighbour below**: ChibiOS puts `thread_t` at the *top* of a `THD_WORKING_AREA`, the stack grows down to `wabase`, and `CH_DBG_ENABLE_STACK_CHECK` only compares SP against `wabase` when that thread is switched *out* - a deep call that returns before the next context switch is never detected, and the bytes it clobbered belong to whatever `.bss` object the linker placed just below (often another thread's working area, whose `thread_t` sits at its top). A corrupted `thread_t` gives a garbage SP on the next switch, the exception-entry push faults, the core locks up and IWDG resets with no cookie: "watchdog reset, no report". `THD_WORKING_AREA(x, N)` is N + 188 (`thread_t`) + 204 (FPU ext+int context) + 128 (`PORT_INT_REQUIRED_STACK`); `threadsinfo` reports the fill-pattern watermark measured from `wabase`. The stack-usage workflow (`.github/workflows/firmware-stack-usage.yaml`, `docs/firmware_stack_usage.md`) only sees threads tagged `RUSEFI_STACK_ROOT*`; an untagged `ThreadController` (e.g. `DtcManager`) is invisible to it.

## MPU Guard Pages (ChibiOS PORT_ENABLE_GUARD_PAGES on F7)

Enabling guard pages is NOT just the two `chconf.h` defines — four coupled constraints, all learned the hard way (FOME hit every one; see docs/report.md 2026-08-05):

- **32-byte alignment of `__main_thread_stack_base__` is load-bearing.** ChibiOS `mpuSetRegionAddress`/`mpuConfigureRegion` write the address raw into MPU RBAR, whose bits [4:0] are the VALID/REGION selector fields — a misaligned base silently programs a *different region number*, so the guard lands nowhere useful and stale mappings persist. The ChibiOS `rules_stacks.ld` only `ALIGN(8)`s `.mstack`/`.pstack`, so alignment is inherited from the RAM region origin: this is why `_OpenBLT_Shared_Params_Size` in the F7/H7 linker scripts must be 32 (not 16) — bootloader builds offset `ram0` by that amount. `THD_WORKING_AREA` threads are safe (`PORT_WORKING_AREA_ALIGN` becomes 32 automatically).
- **The bootloader must clear the MPU before jumping to the app** (`__cpu_deinit` in `openblt_chibios.cpp`): it builds with the same `chconf.h`, so it enables guard pages too, and the app would fault before its own `port_init` runs.
- **Every fault handler must `mpuDisable()` first** (`MemManage_Handler_C`, and since #10160 also `HardFault_Handler_C`/`UsageFault_Handler_C` - see the escalation bullet below), else saving fault state double-faults into a silent lockup — masking the two failures above as a mystery hang.
- **Nothing may read a thread's `wabase` directly** — the first `PORT_GUARD_PAGE_SIZE` (32) bytes are no-access even to privileged code (`AP_NA_NA`); see `CountFreeStackSpace` in `eficonsole.cpp` (simulator port has no `PORT_GUARD_PAGE_SIZE`, hence the 0 fallback there).

Region assignment on F7: nocache = `MPU_REGION_6` (`mcuconf.h`), guard = `MPU_REGION_7` (`chconf.h`) — keep them distinct; the guard region is reprogrammed on every context switch (`__port_set_region`), so any other user of that region number is clobbered continuously.

- **A guard-page hit is not always a MemManage exception.** With PRIMASK set (`port_disable`, `chSysHalt`), or when the fault happens inside another fault handler, it escalates to HardFault - so *every* handler in `firmware/hw_layer/main_hardfault.c` must `mpuDisable()` first and call `logHardFault()` (which stamps the backup-SRAM cookie *before* dereferencing `sp`) before touching the faulting stack. A second fault inside a fault handler is a core lockup: the IWDG then resets the ECU with `Reset Cause: Independent hardware watchdog` and no cookie, i.e. "rebooted, no info" (issue #10160).

## Reading a boot log's reboot evidence

`errorHandlerShowBootReasonAndErrors()` prints `Reset Cause`, OpenBLT `WD/SW resets`, `Power cycle count` and a `Last error type ...` block from `BackupSramData` (backup SRAM, `BKUP_RAM_NOINIT`, 4 KB at 0x40024000 on F4/F7, 0x38800000 on H7). Decoding rules:
- Since #10146 every deliberate `rebootNow()` stamps a `Reboot` cookie and every fault/panic path stamps its own, so `Reset Cause: NVIC_SystemReset` with *no* `Last error type` line means the backup SRAM did not persist, not that a reboot path was missed. `Power cycle count` is incremented on every `errorHandlerInit()`, so a `0` after any non-power reset is the same signal (RTC time still being correct only proves the backup *domain* is alive, not the SRAM content). Only RDP level 1->0 (readout unprotect / some programmer "unlock" flows) erases BKPSRAM on F4/F7.
- H7: ChibiOS `hal_lld_init()` has `rccEnableBKPRAM()` commented out ("not tested and unfinished"), so `baseMCUInit()` (`stm32_common.cpp`) enables the BKPRAM clock + backup regulator itself; before that change H7 boards could never keep a cookie or a boot counter. Not yet verified on H7 hardware.
- `SW resets` has been observed as `0` right after genuine app soft resets on F7 (#9931, #10160) even though the bootloader's `checkIfResetLoop()` should have counted them; the app zeroes both counters 5 s after boot (`errorHandlerResetCounters()`), so a non-zero value is only ever visible on the boot immediately after a reset. Root cause of the `0` not identified - do not treat those counters as evidence either way.
- **A critical error latched before `initHardware()` silently kills SD reports**: `initHardware()` returns at once when `hasFirmwareError()` is set (no SPI pad/pin setup) and `initMmc()` refuses the card, so the MMC thread parks at `die` and `errorHandlerWriteReportFile()` is never reached - the boot console is then the only evidence. This is why the watchdog-reset error is raised by `errorHandlerRaiseWatchdogResetError()` from `commonEarlyInit()` *after* `initHardware()` (not inside `errorHandlerInit()`), and why `initMmc()` exempts exactly that error via `errorHandlerIsWatchdogResetError()`. A watchdog reset with no cookie is reported as `NNNNN_fail_Watchdog.txt` (formerly `_fail_none.txt`, which the report-exists scan ignored). Any new "previous boot" error raised in `errorHandlerInit()` (e.g. `rethrowHardFault`, which additionally reads `engineConfiguration` before `loadConfiguration()`) has the same problem unless it goes through the deferred path.

## OpenBLT Bootloader Version Marker ("BLxx")

The OpenBLT bootloader binary carries no version of its own; rusEFI stamps an ASCII marker (currently `BL10`, historically `BL07`/`BL06` etc.) into the third *reserved* DWORD of the bootloader's vector table, at flash address `0x08000024`. To bump the version, use the `/bump-blt-version` skill (`.claude/skills/bump-blt-version/SKILL.md`) - it walks through both edits and the consistency check. The version is defined in **two places that MUST be bumped together** (both are tagged with the grep marker `search:openblt_version`):

1. `firmware/bin/set_bl_bin_version.sh` - a `printf | dd` that patches the ASCII bytes into the composite `rusefi.bin` (`$(DBIN)`) at offset `0x24`. Invoked from `firmware/bundle.mk` (`.h2d-sentinel` rule) only when `USE_OPENBLT=yes`, *after* hex2dfu has merged bootloader + firmware. So the marker is a post-build patch on the deliverable, not part of the compiled bootloader.
2. `BLT_CURRENT_VERSION` in `firmware/hw_layer/ports/mpu_util.h` - the same four bytes as a little-endian u32 (`0x37304C42` = 'B','L','0','7'), compiled into the **application** firmware.

Why this detects stale bootloaders: `rusefi_update.srec` updates only the application region and never rewrites the bootloader, so the marker at `0x08000024` stays whatever the last *full* `rusefi.bin` flash wrote. The console command `show_blt_version` (`firmware/console/eficonsole.cpp`, `EFI_USE_OPENBLT` only) reads flash via `getOpenBltVersion()` and prints `CURRENT` vs `UNEXPECTED` against `BLT_CURRENT_VERSION` - `UNEXPECTED` means the ECU runs new firmware on an old (or unstamped) bootloader.

Gotchas:
- There is no single source of truth: bumping the script but not the header (or vice versa) makes every ECU report `UNEXPECTED` (or lets stale bootloaders report `CURRENT`). The script's comment acknowledges this.
- A version bump does not change bootloader behavior by itself - it is a manual declaration that the bootloader build has materially changed; bump both places in the same commit as the bootloader change (see history: `BL06` 21e3d069285, `BL07` 438cf802ac8 / PR #9815).
- Only `rusefi.bin` gets stamped. Images produced by other rules (e.g. the `$(BUILDDIR)/rusefi.srec` rule builds from a separately generated `$(DBIN_CRC)`) do not pass through the stamping step.
