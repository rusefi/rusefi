# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

rusEFI is an open-source engine control unit firmware for STM32 microcontrollers.

## Session reporting & knowledge capture

After each completed unit of work (a landed feature, a fixed bug, or a finished investigation), and at minimum once per working session:

1. **Append** a dated entry to `docs/report.md` — never rewrite or reorder earlier entries. Cover: what was done, key decisions and why, validation performed (tests run, hardware checks), and open follow-ups. Match the file's existing style: plain ASCII, `-`/`->` instead of dashes/arrows, tables for change inventories.
2. **Fold durable, non-obvious knowledge into this CLAUDE.md**: build/tooling quirks, hardware protocols, architecture invariants, recurring debugging root-causes. Skip anything derivable from the code or git history — CLAUDE.md records what the code cannot say.

## Build Commands

Default to building with 12 threads unless otherwise specified (-j12 etc).

### Building Firmware

Each board+chip combination has its own compile script in `firmware/config/boards/<board>/`:

```bash
# Example: Build for Proteus F7
cd firmware/config/boards/proteus
./compile_proteus_f7.sh
```

Outputs are placed in `firmware/deliver/`:
- `rusefi.bin` - Complete image (bootloader + firmware) for blank ECUs
- `rusefi_update.srec` - Update image for bootloader flashing

### Unit Tests

```bash
cd unit_tests
./test.sh

# Run a specific test
./test.sh TestName
```

`test.sh` is the recommended way to run tests as it automatically handles both the build (`make`) and execution.

#### Code Coverage
Coverage reports are generated using `gcovr` (requires Python 3).

```bash
cd unit_tests
./run_coverage.sh
```

This script:
1. Sets up a local Python virtual environment in `unit_tests/venv/`
2. Installs `gcovr`
3. Builds tests with `COVERAGE=yes`
4. Runs all tests
5. Generates HTML and JSON reports in `unit_tests/coverage_reports/`

Exclusion patterns (e.g., for `googletest` or the `unit_tests` directory itself) are defined in `unit_tests/coverage_common.sh`.

The build system does not track compiler-flag changes, so run `make clean` in `unit_tests/` when switching between coverage and non-coverage builds — otherwise stale objects produce a near-zero (or needlessly instrumented) result.

Unit tests use Google Test and run on PC, not on the ECU.

#### Troubleshooting test output

To inspect what a test actually scheduled/executed (events, timings, sniffer/logic traces) call `setUnitTestCreateLogs(true)` (declared in `unit_tests/test-framework/engine_test_helper.h`) before constructing `EngineTestHelper` — typically from `main.cpp` or at the top of an individual test. When enabled, each test writes per-test artifacts (e.g. `unittest_<Suite>_<Name>_trace.json`, logic-data, and engine-sniffer files) into the `unit_tests/test_results/` directory (`TEST_RESULTS_DIR` in `unit_test_logger.h`); the absolute path is printed at process exit by `sayByeBye()`. This is the recommended way to diagnose unexpected scheduler/RPM/injection behavior instead of adding ad-hoc `printf`s.

See also unit_tests/test_results/readme.md for unit tests output.

**Cross-platform requirement**: Unit test code MUST build and run on all supported host platforms — Linux (GCC/Clang), macOS (Clang), and Windows (MSVC and MinGW). Avoid POSIX-only APIs (e.g. `realpath`, `PATH_MAX`, `dirent.h` without guards) unless wrapped in `#ifdef` guards or replaced by portable C++ equivalents. Prefer `std::filesystem` over POSIX path APIs.

### Code Generation

```bash
# Generate configs for a specific board
firmware/gen_config_board.sh <board>

# Generate all board configs
firmware/gen_config.sh

# Generate enum-to-string conversions
firmware/gen_enum_to_string.sh
```

## Architecture

### Directory Structure

- `firmware/config/boards/` - Hardware configuration and defaults for different ECU hardware
- `firmware/config/engines/` - Hardware-agnostic configuration for engines (orthogonal to what ECU you run)
- `firmware/controllers/` - Core control logic
  - `algo/` - Fuel, ignition, and air calculations
  - `actuators/` - Control for engine-asynchronous outputs like electonic throttle, idle, AC, boost, VVT, etc.
  - `engine_cycle/` - Control for engine-synchronous outputs like injection, ignition
  - `sensors/` - Input processing (ADC, thermistors, pressure)
  - `trigger/` - Crank/cam position decoding and sync
  - `can/` - CAN bus communication
  - `lua/` - Runtime scripting
- `firmware/hw_layer/` - Hardware abstraction layer
  - `ports/at32/` (Artery AT32F435) is not used at the moment: both AT32 boards (`at_start_f435`, `m74_9`) are disabled (`meta-info.disabled_env`), so no CI build exercises this port
- `firmware/libfirmware/` - Reusable library code
- `firmware/util/` - Self-contained utilities (no external dependencies)
- `unit_tests/` - Google Test suite
- `simulator/` - Windows/Linux firmware simulator

### Deep Dive AI Guidance
For detailed technical documentation intended for AI assistants, see:
- [Fueling System](docs/AI/fueling_system.md) - Mass-based fueling pipeline (17 stages).
- [Ignition System](docs/AI/ignition_system.md) - Timing calculation and spark scheduling.
- [Engine Protection](docs/AI/protection_system.md) - LimpManager and cut logic.
- [Sensor Framework](docs/AI/sensors_system.md) - Sensor registry, conversion pipeline, redundancy and mocking.
- [Scheduling & Timing](docs/AI/scheduling_system.md) - Microsecond timer, event queue/executor, angle-based scheduling, periodic callback rates (fast 200 Hz / slow 20 Hz) and other fixed-rate loops.
- [Lua Scripting API](docs/AI/lua_scripting.md) - Custom Lua hooks (lua_hooks.cpp and friends) grouped by category, indexing conventions, how to add a hook.
- [SD Card Logging](docs/AI/sd_card_logging.md) - SD thread mode state machine, .mlg/.teeth formats, f_expand pre-allocation.
- [Configuration Storage](docs/AI/configuration_storage.md) - Storage manager, INT_FLASH/MFS/SD backends, double-copy settings write, extra flash pages piggybacked on the settings sector.
- [Hellen Board Mapping](docs/hellen-board-mapping.md) - Connector-pin -> hellen-one module -> STM32 pin mapping chain: meta headers (H144_/MM100_ namespaces), connector YAMLs + PinoutLogic codegen, resistor-based board ID; also how to recover the mapping from module schematic PDFs (pdftotext + Altium hidden-text artifacts).
- [Hardware Re-init & requiresPowerCycle](docs/hardware-reinit-and-power-cycle.md) - How Burn applies settings live (activeConfiguration diff, applyNewHardwareSettings stop/start), and the annotated list of reboot-only settings.
- [Board Configuration Override Hooks](docs/board-configuration-override-hooks.md) - custom_board_DefaultConfiguration vs ConfigOverrides vs fix_configuration vs validateConfig vs OnConfigurationChange: lifecycle timelines and decision guide.
- [Sensor Rate-of-Change Handling](docs/sensor-rate-of-change-filtering.md) - per-sensor inventory of low-pass cutoffs, ExpAverage smoothing, slew limiting and rate-based control inputs; notably there are NO rate-of-change plausibility warnings anywhere.
- [Java Gradle Structure Review](docs/java-gradle-structure-review.md) - Gradle subproject inventory, dependency graph, and known structural issues in `java_console/` + `java_tools/`.
- [Java Connectivity & UI Unit Testing](docs/java-connectivity-ui-unit-testing.md) - Test approach for the console connectivity/flashing/session layer and Swing UI: established fake/seam patterns and a refactoring-cost-ordered test backlog.

### Key Concepts

- **Event-driven execution**: Trigger events from crank/cam sensors drive the main control loop
- **Angle-based scheduling**: Events scheduled by crank angle, not just time
- **Configuration-driven**: Board and engine parameters externalized; firmware adapts via configuration
- **Calibration Compatibility**: Maintaining [compatibility with older tunes](docs/calibration-compatibility.md) when adding new parameters.
- **ChibiOS RTOS**: Real-time operating system foundation
- **Config validate vs fix separation**: `validateConfigOnStartUpOrBurn()` is read-only validation; ALL configuration mutation on startup/burn belongs in `applyDefaultsOrFixAfterBurn()` (returns true if it changed anything). Board-specific fixes go in the `custom_board_fix_configuration` override (same changed-flag contract); `custom_board_validateConfig` must never mutate config.
- **Engine modules**: Engine-asynchronous control logic derives from `EngineModule` and registers in the `type_list` in `firmware/controllers/algo/engine.h`. Before creating a module or making one compile-time optional, search the codebase for `[tag:disable_engine_module]` and read those comments — they document the module lifecycle and the TS-page guard-flag rules (a module that owns a TunerStudio page must have its `EFI_*` flag declared in the board `prepend.txt`, never in `board.mk` or `efifeatures.h`).

#### Generated configuration layout

- `firmware/integration/rusefi_config.txt` defines the parameters stored in persistent configuration (both "configuration", ie which pins do what, and the "calibration" or "tune", like the VE table, timing, etc.). This is the primary input that describes the main `engine_configuration_s` struct and the top-level persistent config layout.
- `firmware/integration/config_page_*.txt` files define additional TunerStudio memory pages, each containing its own struct (e.g. `page2_s`, `page3_s`, `page4_s`). These pages hold data that lives outside the main configuration image — for example, high-speed scatter offsets (`page2_s` / TS page 2), long-term fuel trim tables (`page3_s` / TS page 3), and secondary VE tables with blend controls (`page4_s` / TS page 4). The struct and file numbers match the TunerStudio page numbers.
- A board can append extra fields to `engine_configuration_s` by providing `board_engine_configuration.txt` in its board directory (picked up by `gen_config_common.sh`); e.g. the `can3BaudRate`/`can3RxPin`/`can3TxPin` fields exist only on boards that declare them there, guarded in shared C++ by `#if (EFI_CAN_BUS_COUNT >= 3)` with the count itself raised via the board's `prepend.txt`. Note: adding/renaming these board `.txt` inputs may not trigger config regeneration on the next `make` - `touch firmware/integration/rusefi_config.txt` to force it.
- Both `rusefi_config.txt` and the `config_page_*.txt` files are processed by the Java tool at `java_tools/configuration_definition` to generate several outputs. It is critical that these match, so that each part of the system can communicate and agree about the in-memory config format.
  - C/C++ headers in `firmware/controllers/generated/` — the main config produces `engine_configuration_generated_structures.h`, while each config page produces a corresponding `page_N_generated.h`.
  - Along with `firmware/tunerstudio/tunerstudio.template.ini`, generates the ini file used by TunerStudio to communicate with the ECU. All tuner-adjustable parameters **MUST** appear in these input files to be useful.
- `firmware/integration/LiveData.yaml` defines objects processed by the same tool to be transmitted from the ECU about the current state of the world. For example sensors, output values, and intermediate calculations useful for logging.
- **Sharing string constants between .txt / .ini / Java** (`VariableRegistry`): a quoted `#define NAME "value"` in a definition/prepend `.txt` becomes a `public static final String` in the generated `VariableRegistryValues.java`, and can be referenced as `@@NAME@@` (verbatim, keeps quotes) or `@#NAME#@` (quotes stripped) in the `.txt` struct definitions and `tunerstudio.template.ini`. Use `@#NAME#@` where a bare identifier is needed — struct/bit field names and `{ }` indicator expressions (see `OUTPUT_CHANNEL_SD_*`). Put such defines in `firmware/integration/rusefi_config_shared.txt`: it is the only prepend read by *both* the main config pipeline (`gen_config_common.sh` — template .ini + `VariableRegistryValues.java`) and the LiveData pipeline (`LiveData.yaml` `prepend:` entries — `output_channels.txt` and friends). Comments (the `;text` part) stay templated in generated C headers and are expanded only for TS output, so don't expect `@@...@@` in comments to resolve in `*_generated.h`.

Code generation is integrated into the Makefile for all four delivery units: each firmware board build, unit tests (`unit_tests/`), the simulator (`simulator/`), and the Java tools. Running `make` in any of these automatically regenerates the required configuration headers and INI files — there is no reason to invoke `gen_config_board.sh`, `gen_config.sh`, or `gen_enum_to_string.sh` directly. Do not attempt to commit any generated files.

Internal extra-page storage is intentionally disabled for every STM32F7 build without `EFI_FLASH_USE_1500_OF_2MB`, not just the usual dual-bank 2 MB runtime layout. Bank mode and flash size are detected only at runtime, while `storage_flash.cpp` uses a conservative compile-time guard. On the typical dual-bank layout the current roughly 17 KB settings write erases only 32 KB, so it does not prepare the fixed extra-page offsets at 72/76 KB for piggyback writes. Such builds require a ready alternate backend (normally SD or MFS), otherwise extra pages return to defaults after reboot. Older comments about those offsets overlapping the backup described the former roughly 65 KB persistent container and are no longer true.

Do not `git checkout`/revert build-regenerated files (e.g. `firmware/controllers/lua/generated/value_lookup_generated.cpp`) to "clean up" the working tree after a build: some checked-in copies are stale relative to the checked-in config inputs, and the checkout stamps the file newer than its generator inputs, so the next `make` considers it fresh, skips regeneration, and the build fails on missing struct members. Leave them modified (just never commit them); if already reverted, recover with `touch firmware/integration/rusefi_config.txt` or `make clean`.

### Compiler Flags

- C99 with GNU extensions for C code
- C++20 for firmware code
- C++17 (minimum) for unit tests and host-side tooling; portable C++17 features such as `std::filesystem`, `std::optional`, `std::string_view` and structured bindings are preferred over platform-specific APIs to keep unit tests cross-platform (Linux/macOS/Windows-MSVC/MinGW)
- No RTTI, no exceptions (`-fno-rtti -fno-exceptions`)
- LTO enabled by default
- Warnings-as-errors differ per compiler: unit tests build under both GCC (Linux/Windows CI) and clang (macOS CI, or `make CC=clang` locally), and each catches things the other silently accepts (e.g. clang's `-Wunused-but-set-variable`/`-Wdynamic-class-memaccess`; GCC 13 and clang 18 both *suppress* `-Wtype-limits`-style tautological-comparison warnings inside template bodies, so a finding reported by one CI compiler may be unreproducible locally). Before declaring a warning fix complete, build unit tests with both: `make -j12` and `make clean && make CC=clang -j12`. GCC-only flags in `firmware/rusefi_rules.mk` must stay behind its `USE_CLANG` guard; `unit_tests/unit_test_rules.mk` sets `USE_CLANG` from clang auto-detection *before* including the shared rules — keep that ordering.

### Build Conditionals

Key preprocessor flags that control compilation. These three are **mutually exclusive** — any given translation unit is compiled in exactly one of these modes:

- `EFI_PROD_CODE=1` - Production firmware build (cross-compiled for STM32, ChibiOS available, real HAL).
- `EFI_SIMULATOR=1` - Desktop simulator build (`simulator/`), ChibiOS available via the simulator port.
- `EFI_UNIT_TEST=1` - Host-side Google Test build under `unit_tests/`. No ChibiOS, no real HAL — runs as a plain native binary on Linux (GCC/Clang), macOS (Clang) and Windows (MSVC and MinGW).

#### Using `EFI_UNIT_TEST` in code

- `#if EFI_UNIT_TEST` — include a host-only path: stub out HAL/ChibiOS/board-specific calls, expose extra accessors for tests, or substitute portable C++ for embedded primitives.
- `#if !EFI_UNIT_TEST` — exclude code that cannot compile on the host (board pin macros, ChibiOS threads/HAL, MCU registers, etc.).
- For three-way splits, combine with `EFI_PROD_CODE` / `EFI_SIMULATOR` rather than negating one flag.
- The flag is set by `unit_tests/Makefile` (and the unit-test CMake/IDE projects). It is **never** set by board `compile_*.sh` scripts or the simulator build.

#### Cross-platform requirement under `EFI_UNIT_TEST`

Any code reachable from a unit-test build (`unit_tests/` itself, plus firmware sources guarded by `#if EFI_UNIT_TEST` or compiled unconditionally into the test binary) **must build on all four host toolchains**: Linux GCC/Clang, macOS Clang, Windows MSVC, Windows MinGW. Prefer portable C++17 (`std::filesystem`, `std::string`, `std::string_view`, `std::optional`, structured bindings) over POSIX-only (`realpath`, `PATH_MAX`, `dirent.h`) or Win32-only APIs. Guard any unavoidable platform-specific call with an explicit `#ifdef`.

#### Relationship to test infrastructure

`EFI_UNIT_TEST` is the *compile-time* gate; the *runtime* test scaffolding it enables lives in `unit_tests/test-framework/` — most notably `EngineTestHelper` and `setUnitTestCreateLogs(true)` (see the "Troubleshooting test output" section above).

## Source Control Hygiene

- **Never push — only a human pushes.** Claude may commit locally, but `git push` (to any remote, any branch) is reserved for the human. Leave commits on the local branch and say they are ready to push.
- **Stage new files immediately**: When you create a new source file (C/C++ headers/sources, Java/Kotlin sources, unit tests, scripts, build files, resources, docs, etc.), run `git add <path>` as part of the same change so it shows up in `git status` / `git diff` and is not lost on the next clean or branch switch.
- Do not stage build artifacts or generated files (see "Do not attempt to commit any generated files" above), IDE-local files, or user-specific configs.

## Coding Style

- Always use curly brackets for `if` statements, even for single-line blocks.
- Reflection is prohibited in Java unit tests — add an explicit `...ForUnitTest`/`...ForTests` seam on the production class or widen a member to package-private instead (see [Java Connectivity & UI Unit Testing](docs/java-connectivity-ui-unit-testing.md)).

## Embedded Code Practices

- **Static allocation only**: Embedded firmware uses only static memory allocation. No heap usage (`new`, `malloc`, `std::vector`, `std::string`, `std::map`, etc.) is permitted in production firmware code. Use fixed-size containers like `cyclic_buffer` from `rusefi/containers/cyclic_buffer.h` instead. Memory is limited and fragmentation must be avoided.
- **Performance matters**: This is a hard real-time application. Fuel and ignition events must fire at precise crank angles. Avoid unnecessary computation in hot paths. Use lower priority threads for expensive computation.
- **No exceptions**: C++ exceptions are disabled. Use return values or error codes for error handling. rusEFI distinguishes three kinds of errors — `warning()` (recoverable runtime), `configError()` (recoverable bad tune) and `firmwareError()` / `criticalError()` (unrecoverable). See the header comment in `firmware/controllers/core/error_handling.h` for when to use which.
- **No RTTI**: `dynamic_cast` and `typeid` are unavailable.
- **Interrupt safety**: Be mindful of code that runs in interrupt context vs. thread context. Use appropriate synchronization primitives.
- **Stack usage**: Keep stack allocations small. Large arrays should be static or global, not local variables.

## Java Version Constants

- When modifying Java code (`java_console/`, `java_tools/`), bump `UiVersion.CONSOLE_VERSION` in `java_tools/version/src/main/java/com/rusefi/UiVersion.java` to the current date in `YYYYMMDD` format as part of the same change (no-op if it already shows today's date).
- `Autoupdate.AUTOUPDATE_VERSION` in `java_console/autoupdate/src/main/java/com/rusefi/autoupdate/Autoupdate.java` is a separate, manually managed version for the updater executable. Bump it (same `YYYYMMDD` format) only when the change affects updater behavior, i.e. code that runs inside `rusefi_autoupdate.exe`.

## MCP Servers

rusEFI provides two MCP (Model Context Protocol) servers for LLM-driven tooling over stdio JSON-RPC:

- **`:mcp_ecu`** (`java_console/mcp_ecu`) — `EcuMcpServer`: connect to an ECU, upload/download Lua scripts, send commands, and capture ECU messages. Entry point: `com.rusefi.mcp.EcuMcpServer`.
- **`:mcp_can`** (`java_console/mcp_can`) — `CanSnifferMcp`: read-only CAN bus sniffing via PCAN hardware (connect, read packets, wait for packet, status). Entry point: `com.rusefi.mcp.CanSnifferMcp`.

## Serial Connectivity

All rusEFI serial connections use the USB CDC (Communications Device Class) profile. Baud rate is irrelevant and never a concern — the USB serial profile handles throughput natively regardless of any baud rate setting in host software or code.

### USB Mass Storage SCSI (known Wireshark false-positive)

When sniffing the ECU's USB link, Wireshark flags the SCSI `Mode Sense(6)` (opcode 0x1a) replies as *"Malformed Packet: SCSI: length of contained item exceeds length of containing item."* This is **not** bad wire data. The reply is a valid but *short* Caching mode page (page code 0x08, `PageLength = 0x0a`) instead of the SBC-2 mandated 0x12; Wireshark's dissector decodes the full 20-byte caching-page layout, overruns the buffer, and raises the exception. Windows accepts the reply and the device works. The response is hardcoded in the ChibiOS-Contrib USB-MSD SCSI target (`os/hal/src/hal_usb_msd.c`, a submodule usually not checked out), used by `firmware/hw_layer/mass_storage/mass_storage_device.cpp`. Treat it as cosmetic unless a host actually rejects it.

### CDC console and MSD share ONE composite USB device — SD mode switch can drop the console link

The USB serial console (CDC) and the SD-card USB mass storage are **interfaces on a single composite `USBD1` device**, not separate devices. The config descriptor is fixed at 3 interfaces — MSD IF0 + CDC-control IF1 + CDC-data IF2 (`firmware/hw_layer/ports/stm32/serial_over_usb/usbcfg.cpp`, `NUM_INTERFACES`/`DESCRIPTOR_SIZE`); MSD is always enumerated whenever `HAL_USE_USB_MSD` is built in. Switching the SD card between PC/MSD and ECU/logging does **not** re-enumerate USB — `attachMsdSdCard`/`deattachMsdSdCard` (`mass_storage_init.cpp`) only hot-swap LUN1's backing block device (real SD card ↔ null device `ND1`) on the already-running MSD controller.

Consequence (observed, `SdEcuPcCycleSandbox`): switching **PC/MSD → ECU** yanks the mounted mass-storage medium out from under the host. Windows' usbstor stack recovers by resetting/re-enumerating the whole composite device; the firmware then takes `USB_EVENT_RESET`/`SUSPEND`, whose handler calls `sduSuspendHookI(&SDU1)` (`usbcfg.cpp`), tearing down the CDC channel. The console link drops host-side (`write failed: wrote 0 but expected 11`, port closes) even though the SD switch itself succeeded firmware-side. So a console-driven SD-mode soak cannot span multiple cycles over one connection — either address it firmware-side (return SCSI "medium not present"/unit-attention for an orderly host eject instead of swapping to a dead LUN) or reconnect the host link after each switch.

**MSD thread wedge → periodic CDC disconnects (confirmed on hardware + USBPcap, 2026-07)**: if the host abandons a mass-storage command mid-data-phase (cancels its IN URB without sending a Bulk-Only Mass Storage Reset — Windows usbstor does exactly this), the MSD thread blocks forever: `lib_scsi` `data_read10` → `scsi_transport_transmit(_wait)` → `usbTransmit`/`usbTransmitWait`, which is `osalThreadSuspendS` with **no timeout** (`firmware/ChibiOS/os/hal/src/hal_usb.c`). The existing `m_botResetPending`/`transportAbandoned()` escape hatch only fires on the class-specific BOT reset request, which Windows does not send in this scenario. A wedged MSD thread never re-arms the bulk-OUT endpoint, so every subsequent CBW NAKs forever; usbstor then resets the whole composite device on a ~20 s timeout cycle, cancelling the CDC IRPs each time — the user-visible symptom is *recurring CDC console/TS disconnects*, with MSD as the hidden culprit. Diagnose with console `sdinfo`: `MSD: executing opcode 0x28 for <huge> ms` = wedged (diagnostics from `MassStorageController::printDiagnostics`). Wire-level signature (USBPcap): all-endpoint `USBD_STATUS_CANCELED` storms, a CBW submit whose IRP survives ~20 s then cancels, no CSW/STALL ever returned.

## MPU Guard Pages (ChibiOS PORT_ENABLE_GUARD_PAGES on F7)

Enabling guard pages is NOT just the two `chconf.h` defines — four coupled constraints, all learned the hard way (FOME hit every one; see docs/report.md 2026-08-05):

- **32-byte alignment of `__main_thread_stack_base__` is load-bearing.** ChibiOS `mpuSetRegionAddress`/`mpuConfigureRegion` write the address raw into MPU RBAR, whose bits [4:0] are the VALID/REGION selector fields — a misaligned base silently programs a *different region number*, so the guard lands nowhere useful and stale mappings persist. The ChibiOS `rules_stacks.ld` only `ALIGN(8)`s `.mstack`/`.pstack`, so alignment is inherited from the RAM region origin: this is why `_OpenBLT_Shared_Params_Size` in the F7/H7 linker scripts must be 32 (not 16) — bootloader builds offset `ram0` by that amount. `THD_WORKING_AREA` threads are safe (`PORT_WORKING_AREA_ALIGN` becomes 32 automatically).
- **The bootloader must clear the MPU before jumping to the app** (`__cpu_deinit` in `openblt_chibios.cpp`): it builds with the same `chconf.h`, so it enables guard pages too, and the app would fault before its own `port_init` runs.
- **`MemManage_Handler_C` must `mpuDisable()` first**, else saving fault state double-faults into a silent lockup — masking the two failures above as a mystery hang.
- **Nothing may read a thread's `wabase` directly** — the first `PORT_GUARD_PAGE_SIZE` (32) bytes are no-access even to privileged code (`AP_NA_NA`); see `CountFreeStackSpace` in `eficonsole.cpp` (simulator port has no `PORT_GUARD_PAGE_SIZE`, hence the 0 fallback there).

Region assignment on F7: nocache = `MPU_REGION_6` (`mcuconf.h`), guard = `MPU_REGION_7` (`chconf.h`) — keep them distinct; the guard region is reprogrammed on every context switch (`__port_set_region`), so any other user of that region number is clobbered continuously.

## Microsecond Scheduler Invariant (TIM5/other OC timers)

`hwTimerCallback` in `microsecond_timer_stm32.cpp` **disables the compare notification (CC1IE) before** invoking the callback; `setHardwareSchedulerTimer()` -> `portSetHardwareSchedulerTimer()` (`pwmEnableChannelNotificationI`) is the **only** code that re-enables it. Therefore nothing in the scheduler path may early-return before the arm: a `hasFirmwareError()` gate there (removed 2026-08-11) silently killed the whole event scheduler (soft PWM, watchdog buddy, every scheduled event) on the first ISR after any `firmwareError()` - deterministic brick-until-reboot, WDT firing 2 s later. Engine safety after a fatal error is NOT supposed to come from stopping the scheduler: `firmwareErrorV()` calls `getLimpManager()->fatalError()` which cuts ignition/injection/ETB/trigger input, and `EtbController::setOutput()` gates on `allowElectronicThrottle()`. Bench-calibration failures (ETB TPS autocal) are `warning()`-level, not `firmwareError()`.

## m74_9 / Itelma ETB: TLE9201 enable chain (ETC_EN) and the inverted-disable gotcha

The TLE9201 H-bridge DIS pin (11) is active-low at the chip, but on m74_9 the MCU-side enable is ACTIVE-HIGH and inverted through a transistor: PB13 (ETC_EN) -> Q5A (MUN5311DW1 NPN, inverts) -> DIS pulled up to +5V (R23). PB13 high = Q5A on = DIS low = bridge enabled. Consequences:

- **rusEFI's disable-pin mechanism cannot express this**: `DcHardware::start()` calls `OutputPin::initPin(msg, pin)` with hardcoded `OM_DEFAULT` (pin low = enable) and there is NO `disablePinMode` config field (checked `dc_io` in `rusefi_config.txt`). Assigning `disablePin = B13` would keep DIS high (tristate) whenever the firmware thinks the bridge is enabled. Boards with an inverted enable chain must drive the enable GPIO directly from board init (`m74_9_boardInitHardware` sets PB13 high); the runtime disable is then PWM=0, on which the TLE9201 coasts.
- **Boot is safe by construction**: PB13's weak pullup (~40k) against Q5A's internal 10k base-emitter divider leaves ~0.66V at the base - below Vbe, Q5A stays off, DIS is pulled to +5V, outputs tristate until firmware drives PB13.

The L9779 WDA output (pin 38) nets to ETC_WD -> Q5B -> DIS as a redundant hardware kill (L9779 algorithmic watchdog can cut the bridge); likely depopulated ("not soldered" near R20). The KiCad netlist disagrees with the physical board (KiCad says DIS -> +3V3, physically +5V via R23) - trust 0-ohm measurements over netlist Y-positions.

## m74_9 / AT32: fast ADC (TIM6 -> ADC2) root cause - DMAMUX TBL_SEL never enabled

The AT32F4xx port in the rusEFI ChibiOS fork reuses the unmodified STM32 ADCv2/DMAv1 LLDs (`firmware/ChibiOS/os/hal/ports/AT32/AT32F4xx/platform.mk` includes STM32 LLDs). Fixed 2026-08-15 after MAP read 0 (fast ADC dead, `adc_report` showed `fast 0 samples` forever).

Root cause chain:
- `hal_lld_init` called `dmaInit()` right after `rccResetAHB1()` with the DMA1/DMA2 clocks OFF - writes to the unclocked AT32 DMA are silently dropped, so `DMA1->MUXSEL = 1` never landed. `TBL_SEL` stayed 0 (fixed default request table), and the `REQSEL` values programmed into `DMA_MUXCxCTRL` by `dmaSetRequestSource()` were ignored.
- With the default table, only channels whose fixed default request matches their peripheral work: ADC1 on DMA1_CH1 works (slow ADC OK), ADC2 on DMA1_CH2 does not -> ADC2 converts but nobody drains the data register -> `ADC_ERR_OVERFLOW` (`lastErr=2`) on every conversion -> MAP (the only fast channel) read 0.
- Contributing bugs in the fork's Artery headers: `at32_registry.h` lacked `STM32_DMA_HAS_DMAMUXSEL TRUE`, and `DMA_TypeDef.MUXSEL` in `os/common/ext/Artery/AT32F4xx/at32f435xx.h` sat at offset 0x78 instead of 0x100 (the `ch[7]` array stride is 0x10 while the hardware channel stride is 0x14 - only the `DMA1_Channelx` base macros are hardware-correct, never use the struct array).

Fixes (all in the ChibiOS fork AT32 port): `hal_lld.c` enables `rccEnableDMA1/2(false)` before `dmaInit()`; `at32_registry.h` defines `STM32_DMA_HAS_DMAMUXSEL TRUE`; `at32f435xx.h` pads `DMA_TypeDef` so MUXSEL/MUXC/MUXG land at 0x100/0x104/0x120. Also `hw_layer/ports/at32/at32f4/cfg/mcuconf.h`: `STM32_ADC_ADCPRE = ADC_CCR_ADCPRE_DIV6` -> Artery ADCDIV = HCLK/4 = 72 MHz (the old DIV4 value maps to ADCDIV = HCLK/3 = 96 MHz, above the 80 MHz RM maximum).

Bench verification (m74_9, 2026-08-15): `fast 45692 samples`, `F ch[0] @ PA1` samples valid, `fastadcdiag` shows `MUXSEL=0x00000001`, `fast err` no longer growing (`lastErr=0`). Note: `fastAdcErrorCount` is a uint8 **skip counter**, not a hardware error counter - `AdcDevice::startConversionI()` increments it whenever a TIM6 tick lands while the previous conversion is still ACTIVE (its completion ISR was delayed past the tick by an interrupt-disabled window / long ISR), and it wraps at 255, so it can appear to decrease. `lastErr=0` + growing conversion count = healthy ADC; the same counting happens on all GPT-triggered F4 boards. `fastadcdiag` is a board-local console command in `m74_9/board_configuration.cpp` that dumps TIM6/GPTD6/ADC2/DMA/DMAMUX state - keep it for AT32 bring-up. `m74_9/efifeatures.h` leaves `EFI_USE_FAST_ADC` at the default TRUE; the `!EFI_USE_FAST_ADC` stubs added to `stm32_adc_v2.cpp` (2026-08-15, commit 988b087fd46) remain as a valid slow-ADC-only configuration.

## EFI_USE_OPENBLT: USE_OPENBLT=yes does NOT enable the app-side OpenBLT code

## Console app and the CAN flasher cannot share the PCAN adapter (m74_9 bench)

When the rusEFI Java console is open with a CAN/ISO-TP connection (console-over-CAN on a PCAN adapter), the bus goes dead for everything else: no ACKs (the ECU's TXs raise TERR and TEC climbs to error-passive) and no frames are visible to other tools - even though the adapter's Initialize/Write calls still succeed. Kill all Java (close the console) before running `openblt_can.sh`; the bus comes back immediately. Also: an XCP probe session holds the bootloader forever (wasConnected), so the ECU stays in the bootloader and the console cannot connect until the ECU is restarted - `--probe` in the flasher sends PROGRAM_RESET at the end to return the ECU to the app.

## EFI_USE_OPENBLT: USE_OPENBLT=yes does NOT enable the app-side OpenBLT code

`USE_OPENBLT=yes` in `meta-info.env` only adds `hw_layer/openblt/shared_params.c` to the build. The C++ side (`can_rx.cpp` CAN trigger, `jump_to_openblt()` body in the port's `*_common.cpp`, `reboot_openblt` console action, `show_blt_version`) is guarded by `EFI_USE_OPENBLT`, which is defined ONLY in `config/stm32f4ems/efifeatures.h` with default **FALSE** - and no board overrides it. A board that runs on top of the OpenBLT bootloader MUST `#define EFI_USE_OPENBLT TRUE` in its own `efifeatures.h` **before** including the stm32f4ems header (that header uses `#ifndef`).

Also: `efifeatures.h` edits do not trigger a rebuild (object deps track `pch/pch.h`, not headers included by the pch) - `touch firmware/pch/pch.h` after any efifeatures change. And do not verify preprocessor state with `strings` on the ELF (DWARF matches compiled-out string literals) or on LTO object files (GIMPLE bitcode) - use `objdump -s -j .rodata` / disassembly of the linked ELF instead.

## F-port pins are ADC3-only (STM32F4 / AT32F435): EFI_ADC3_SLOW

On both STM32F4 and AT32F435 every F-port pin (PF3-PF10) is an ADC3-only input - the ADC1/ADC2 slow+fast sampling cannot see them at all. Boards that wire CLT/IAT/O2/AC-pressure to F-pins (m74_9 does: CLT=PF5=ADC3_IN15, IAT=PF6=ADC3_IN4) need the `EFI_ADC3_SLOW` capability (stm32_adc_v2.cpp): it starts ADC3, samples the 8 F-pin channels (physical IN4,5,6,7,8,9,14,15 -> rusEFI EFI_ADC_32..39, order per the `adcChannels[]` table in stm32_adc.cpp) with a blocking `adcConvert` in the slow loop, and stores them at slow-buffer indices 32..39. Cannot be combined with `EFI_SOFTWARE_KNOCK` (compile error guards it). With ADC3 slow sampling the open-input reading of a thermistor pin (1500 ohm pullup to +5V, 3.3V-referenced ADC) is ~3.28V raw - the resistance conversion then reports 0 ohms/invalid, which is expected until a sensor is connected.

## OpenBLT Bootloader Version Marker ("BLxx")

The OpenBLT bootloader binary carries no version of its own; rusEFI stamps an ASCII marker (currently `BL08`, historically `BL07`/`BL06` etc.) into the third *reserved* DWORD of the bootloader's vector table, at flash address `0x08000024`. To bump the version, use the `/bump-blt-version` skill (`.claude/skills/bump-blt-version/SKILL.md`) - it walks through both edits and the consistency check. The version is defined in **two places that MUST be bumped together** (both are tagged with the grep marker `search:openblt_version`):

1. `firmware/bin/set_bl_bin_version.sh` - a `printf | dd` that patches the ASCII bytes into the composite `rusefi.bin` (`$(DBIN)`) at offset `0x24`. Invoked from `firmware/bundle.mk` (`.h2d-sentinel` rule) only when `USE_OPENBLT=yes`, *after* hex2dfu has merged bootloader + firmware. So the marker is a post-build patch on the deliverable, not part of the compiled bootloader.
2. `BLT_CURRENT_VERSION` in `firmware/hw_layer/ports/mpu_util.h` - the same four bytes as a little-endian u32 (`0x37304C42` = 'B','L','0','7'), compiled into the **application** firmware.

Why this detects stale bootloaders: `rusefi_update.srec` updates only the application region and never rewrites the bootloader, so the marker at `0x08000024` stays whatever the last *full* `rusefi.bin` flash wrote. The console command `show_blt_version` (`firmware/console/eficonsole.cpp`, `EFI_USE_OPENBLT` only) reads flash via `getOpenBltVersion()` and prints `CURRENT` vs `UNEXPECTED` against `BLT_CURRENT_VERSION` - `UNEXPECTED` means the ECU runs new firmware on an old (or unstamped) bootloader.

Gotchas:
- There is no single source of truth: bumping the script but not the header (or vice versa) makes every ECU report `UNEXPECTED` (or lets stale bootloaders report `CURRENT`). The script's comment acknowledges this.
- A version bump does not change bootloader behavior by itself - it is a manual declaration that the bootloader build has materially changed; bump both places in the same commit as the bootloader change (see history: `BL06` 21e3d069285, `BL07` 438cf802ac8 / PR #9815).
- Only `rusefi.bin` gets stamped. Images produced by other rules (e.g. the `$(BUILDDIR)/rusefi.srec` rule builds from a separately generated `$(DBIN_CRC)`) do not pass through the stamping step.

## 60-2 trigger sync windows: asymmetric [1.6, 3.75] + second gap [0.8, 1.2]

`TT_TOOTHED_WHEEL_60_2` overrides the default `initializeSkippedToothTrigger` windows. The asymmetry is load-bearing for real cranking:

- **Low side widened to 1.6** (36-2 precedent, PR #4138): at first combustion the crank accelerates so hard that the missing-teeth gap (36 deg) takes less than 2.25x the preceding tooth time. With the old default the decoder misses the sync point, counts exactly 58 events (one full revolution of real teeth) and fires C9002 "expected 58/0 got 58/0" right where the sync point should have been - the count is exactly right, only the ratio check failed. Symptom on the car: engine catches, C9002, then spins without running.
- **High side stays at 3.75** (NOT 3.5 like 36-2): real 60-2 cranking data (`unit_tests/tests/trigger/resources/trigger_adc_real1.csv`, 322 RPM) shows the gap systematically stretched to ~3.6-3.75 by compression ripple. Copying 36-2's 3.5 upper limit desyncs nearly every revolution on that data.
- **Second gap [0.85, 1.15]** (tighter than 36-2's [0.7, 1.3]): 60-2 teeth are twice as dense in time as 36-2's, so adjacent teeth barely change ratio even under hard acceleration (6000 RPM/s at 300 RPM gives ~1.07). The tighter window rejects two false-sync patterns: the noise-shifted-tooth case that breaks `test_trigger_noiseless.cpp` noise#1 with a wider gap1, and the misfire-distorted pair (gap0 1.60 / gap1 1.20) that false-synced mid-revolution on a running m74_9 engine (C9003 "expected 58 got 51") and killed it - with [0.85, 1.15] the distortion is just one noisy tooth and the real gap re-syncs cleanly.
- Keeping the midpoint at 3.0 means `syncRatioAvg` stays 3, so the noiseless trigger filter gap prediction is unchanged.

If a field setup still shows C9002 at start (ratio outside [1.6, 3.75]), TunerStudio's "Use custom sync ratio" (`overrideTriggerGaps` + `triggerGapOverrideFrom/To` + `gapTrackingLengthOverride`) widens the window without a rebuild.

## Development Notes

- Supported IDE: Visual Studio Code
- Requires Unix-like OS (Linux, macOS, or Windows WSL)
- All PRs must pass CI gates (firmware builds for all boards, unit tests)
- Wiki: https://wiki.rusefi.com/
- Adding a new trigger: `docs/adding-new-trigger.md`
- TunerStudio General Notes: `.junie/ts-readme.md`

See also .junie/guidelines.md file
