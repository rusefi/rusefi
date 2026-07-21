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
- [Hardware Re-init & requiresPowerCycle](docs/hardware-reinit-and-power-cycle.md) - How Burn applies settings live (activeConfiguration diff, applyNewHardwareSettings stop/start), and the annotated list of reboot-only settings.
- [Java Gradle Structure Review](docs/java-gradle-structure-review.md) - Gradle subproject inventory, dependency graph, and known structural issues in `java_console/` + `java_tools/`.
- [Java Connectivity & UI Unit Testing](docs/java-connectivity-ui-unit-testing.md) - Test approach for the console connectivity/flashing/session layer and Swing UI: established fake/seam patterns and a refactoring-cost-ordered test backlog.

### Key Concepts

- **Event-driven execution**: Trigger events from crank/cam sensors drive the main control loop
- **Angle-based scheduling**: Events scheduled by crank angle, not just time
- **Configuration-driven**: Board and engine parameters externalized; firmware adapts via configuration
- **Calibration Compatibility**: Maintaining [compatibility with older tunes](docs/calibration-compatibility.md) when adding new parameters.
- **ChibiOS RTOS**: Real-time operating system foundation
- **Engine modules**: Engine-asynchronous control logic derives from `EngineModule` and registers in the `type_list` in `firmware/controllers/algo/engine.h`. Before creating a module or making one compile-time optional, search the codebase for `[tag:disable_engine_module]` and read those comments — they document the module lifecycle and the TS-page guard-flag rules (a module that owns a TunerStudio page must have its `EFI_*` flag declared in the board `prepend.txt`, never in `board.mk` or `efifeatures.h`).

#### Generated configuration layout

- `firmware/integration/rusefi_config.txt` defines the parameters stored in persistent configuration (both "configuration", ie which pins do what, and the "calibration" or "tune", like the VE table, timing, etc.). This is the primary input that describes the main `engine_configuration_s` struct and the top-level persistent config layout.
- `firmware/integration/config_page_*.txt` files define additional TunerStudio memory pages, each containing its own struct (e.g. `page2_s`, `page3_s`, `page4_s`). These pages hold data that lives outside the main configuration image — for example, high-speed scatter offsets (`page2_s` / TS page 2), long-term fuel trim tables (`page3_s` / TS page 3), and secondary VE tables with blend controls (`page4_s` / TS page 4). The struct and file numbers match the TunerStudio page numbers.
- Both `rusefi_config.txt` and the `config_page_*.txt` files are processed by the Java tool at `java_tools/configuration_definition` to generate several outputs. It is critical that these match, so that each part of the system can communicate and agree about the in-memory config format.
  - C/C++ headers in `firmware/controllers/generated/` — the main config produces `engine_configuration_generated_structures.h`, while each config page produces a corresponding `page_N_generated.h`.
  - Along with `firmware/tunerstudio/tunerstudio.template.ini`, generates the ini file used by TunerStudio to communicate with the ECU. All tuner-adjustable parameters **MUST** appear in these input files to be useful.
- `firmware/integration/LiveData.yaml` defines objects processed by the same tool to be transmitted from the ECU about the current state of the world. For example sensors, output values, and intermediate calculations useful for logging.
- **Sharing string constants between .txt / .ini / Java** (`VariableRegistry`): a quoted `#define NAME "value"` in a definition/prepend `.txt` becomes a `public static final String` in the generated `VariableRegistryValues.java`, and can be referenced as `@@NAME@@` (verbatim, keeps quotes) or `@#NAME#@` (quotes stripped) in the `.txt` struct definitions and `tunerstudio.template.ini`. Use `@#NAME#@` where a bare identifier is needed — struct/bit field names and `{ }` indicator expressions (see `OUTPUT_CHANNEL_SD_*`). Put such defines in `firmware/integration/rusefi_config_shared.txt`: it is the only prepend read by *both* the main config pipeline (`gen_config_common.sh` — template .ini + `VariableRegistryValues.java`) and the LiveData pipeline (`LiveData.yaml` `prepend:` entries — `output_channels.txt` and friends). Comments (the `;text` part) stay templated in generated C headers and are expanded only for TS output, so don't expect `@@...@@` in comments to resolve in `*_generated.h`.

Code generation is integrated into the Makefile for all four delivery units: each firmware board build, unit tests (`unit_tests/`), the simulator (`simulator/`), and the Java tools. Running `make` in any of these automatically regenerates the required configuration headers and INI files — there is no reason to invoke `gen_config_board.sh`, `gen_config.sh`, or `gen_enum_to_string.sh` directly. Do not attempt to commit any generated files.

Do not `git checkout`/revert build-regenerated files (e.g. `firmware/controllers/lua/generated/value_lookup_generated.cpp`) to "clean up" the working tree after a build: some checked-in copies are stale relative to the checked-in config inputs, and the checkout stamps the file newer than its generator inputs, so the next `make` considers it fresh, skips regeneration, and the build fails on missing struct members. Leave them modified (just never commit them); if already reverted, recover with `touch firmware/integration/rusefi_config.txt` or `make clean`.

### Compiler Flags

- C99 with GNU extensions for C code
- C++20 for firmware code
- C++17 (minimum) for unit tests and host-side tooling; portable C++17 features such as `std::filesystem`, `std::optional`, `std::string_view` and structured bindings are preferred over platform-specific APIs to keep unit tests cross-platform (Linux/macOS/Windows-MSVC/MinGW)
- No RTTI, no exceptions (`-fno-rtti -fno-exceptions`)
- LTO enabled by default

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

## Development Notes

- Supported IDE: Visual Studio Code
- Requires Unix-like OS (Linux, macOS, or Windows WSL)
- All PRs must pass CI gates (firmware builds for all boards, unit tests)
- Wiki: https://wiki.rusefi.com/
- Adding a new trigger: `docs/adding-new-trigger.md`
- TunerStudio General Notes: `.junie/ts-readme.md`

See also .junie/guidelines.md file
