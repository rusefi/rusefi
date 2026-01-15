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
make
./build/rusefi_test

# Run a specific test
./build/rusefi_test --gtest_filter=TestName
```

Unit tests use Google Test and run on amd64/aarch64, not on the ECU.

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

### Key Concepts

- **Event-driven execution**: Trigger events from crank/cam sensors drive the main control loop
- **Angle-based scheduling**: Events scheduled by crank angle, not just time
- **Configuration-driven**: Board and engine parameters externalized; firmware adapts via configuration
- **ChibiOS RTOS**: Real-time operating system foundation

#### Generated configuration layout

- `firmware/rusefi_config.txt` defines the parameters stored in persistent configuration (both "configuration", ie which pins do what, and the "calibration" or "tune", like the VE table, timing, etc.)
- That file is processed by the java tool at `java_tools/configuration_definition` to generate several outputs. It is critical that these match, so that each part of the system can communicate and agree about the in-memory config format.
  - C/C++ headers in `firmware/generated`
  - Along with `firmware/tunerstudio/tunerstudio.template.ini`, generates the ini file used by TunerStudio to communicate with the ECU. All tuner-adjustable parameters **MUST** appear in this file to be useful.
- `firmware/integration/LiveData.yaml` defines objects processed by the same tool to be transmitted from the ECU about the current state of the world. For example sensors, output values, and intermediate calculations useful for logging.

These are all automatically regenerated as part of running `make`, so no direct script invocation is required. Do not attempt to commit any generated files.

### Compiler Flags

- C99 with GNU extensions for C code
- C++20 for firmware code
- No RTTI, no exceptions (`-fno-rtti -fno-exceptions`)
- LTO enabled by default

### Build Conditionals

Key preprocessor flags that control compilation:
- `EFI_PROD_CODE=1` - Production firmware
- `EFI_UNIT_TEST=1` - Unit test build
- `EFI_SIMULATOR=1` - Simulator build

## Embedded Code Practices

- **Static allocation**: Prefer static allocation over dynamic (`new`/`malloc`). Memory is limited and fragmentation must be avoided.
- **Performance matters**: This is a hard real-time application. Fuel and ignition events must fire at precise crank angles. Avoid unnecessary computation in hot paths. Use lower priority threads for expensive computation.
- **No exceptions**: C++ exceptions are disabled. Use return values or error codes for error handling.
- **No RTTI**: `dynamic_cast` and `typeid` are unavailable.
- **Interrupt safety**: Be mindful of code that runs in interrupt context vs. thread context. Use appropriate synchronization primitives.
- **Stack usage**: Keep stack allocations small. Large arrays should be static or global, not local variables.

## Development Notes

- Supported IDE: Visual Studio Code
- Requires Unix-like OS (Linux, macOS, or Windows WSL)
- All PRs must pass CI gates (firmware builds for all boards, unit tests)
- Wiki: https://wiki.rusefi.com/
