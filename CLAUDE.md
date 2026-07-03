# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

rusEFI is an open-source engine control unit firmware for STM32 microcontrollers.

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
- `firmware/libfirmware/` - Reusable library code
- `firmware/util/` - Self-contained utilities (no external dependencies)
- `unit_tests/` - Google Test suite
- `simulator/` - Windows/Linux firmware simulator

### Deep Dive AI Guidance
For detailed technical documentation intended for AI assistants, see:
- [Fueling System](docs/AI/fueling_system.md) - Mass-based fueling pipeline (17 stages).
- [Ignition System](docs/AI/ignition_system.md) - Timing calculation and spark scheduling.
- [Engine Protection](docs/AI/protection_system.md) - LimpManager and cut logic.

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

Code generation is integrated into the Makefile for all four delivery units: each firmware board build, unit tests (`unit_tests/`), the simulator (`simulator/`), and the Java tools. Running `make` in any of these automatically regenerates the required configuration headers and INI files — there is no reason to invoke `gen_config_board.sh`, `gen_config.sh`, or `gen_enum_to_string.sh` directly. Do not attempt to commit any generated files.

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

## Embedded Code Practices

- **Static allocation only**: Embedded firmware uses only static memory allocation. No heap usage (`new`, `malloc`, `std::vector`, `std::string`, `std::map`, etc.) is permitted in production firmware code. Use fixed-size containers like `cyclic_buffer` from `rusefi/containers/cyclic_buffer.h` instead. Memory is limited and fragmentation must be avoided.
- **Performance matters**: This is a hard real-time application. Fuel and ignition events must fire at precise crank angles. Avoid unnecessary computation in hot paths. Use lower priority threads for expensive computation.
- **No exceptions**: C++ exceptions are disabled. Use return values or error codes for error handling. rusEFI distinguishes three kinds of errors — `warning()` (recoverable runtime), `configError()` (recoverable bad tune) and `firmwareError()` / `criticalError()` (unrecoverable). See the header comment in `firmware/controllers/core/error_handling.h` for when to use which.
- **No RTTI**: `dynamic_cast` and `typeid` are unavailable.
- **Interrupt safety**: Be mindful of code that runs in interrupt context vs. thread context. Use appropriate synchronization primitives.
- **Stack usage**: Keep stack allocations small. Large arrays should be static or global, not local variables.

## MCP Servers

rusEFI provides two MCP (Model Context Protocol) servers for LLM-driven tooling over stdio JSON-RPC:

- **`:mcp_ecu`** (`java_console/mcp_ecu`) — `EcuMcpServer`: connect to an ECU, upload/download Lua scripts, send commands, and capture ECU messages. Entry point: `com.rusefi.mcp.EcuMcpServer`.
- **`:mcp_can`** (`java_console/mcp_can`) — `CanSnifferMcp`: read-only CAN bus sniffing via PCAN hardware (connect, read packets, wait for packet, status). Entry point: `com.rusefi.mcp.CanSnifferMcp`.

## Serial Connectivity

All rusEFI serial connections use the USB CDC (Communications Device Class) profile. Baud rate is irrelevant and never a concern — the USB serial profile handles throughput natively regardless of any baud rate setting in host software or code.

## Development Notes

- Supported IDE: Visual Studio Code
- Requires Unix-like OS (Linux, macOS, or Windows WSL)
- All PRs must pass CI gates (firmware builds for all boards, unit tests)
- Wiki: https://wiki.rusefi.com/
- Adding a new trigger: `docs/adding-new-trigger.md`
- TunerStudio General Notes: `.junie/ts-readme.md`

See also .junie/guidelines.md file
