# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

rusEFI is an open-source engine control unit firmware for STM32 microcontrollers.

## Session reporting & knowledge capture

After each completed unit of work (a landed feature, a fixed bug, or a finished investigation), and at minimum once per working session:

1. **Prepare the commit message locally in `docs/report.md`.** It is only used as a local source of the commit message. Keep concise notes on what changed, key decisions and why, and validation performed; revise the draft as the work develops. No append-only or dated history is required. Do not stage, commit, or push the report as a log. Keep durable findings and open follow-ups in the relevant project documentation or guidance.
2. **Fold durable, non-obvious knowledge into the matching `docs/AI` file** (into this CLAUDE.md only when most tasks need it): build/tooling quirks, hardware protocols, architecture invariants, recurring debugging root-causes. Skip anything derivable from the code or git history — CLAUDE.md records what the code cannot say.
3. **Update user-facing wiki docs when users need new or corrected guidance**: the rusEFI wiki source may be checked out as a sibling repo at `../rusefi_documentation`. Update the matching page when a change affects how users configure, use, or troubleshoot the product, or makes existing instructions inaccurate. Skip obvious expected behavior and routine fixes that restore it, such as expressions displaying their resolved values. Keep minor implementation details and validation in tests and commit notes; a user-visible code change does not automatically need a documentation addition. Same source-control rules apply there: git add is allowed; never commit or push, leave those actions for the human.

### Current-state documentation

Describe current behavior, configuration, constraints and usage in present tense
in ordinary documentation, including wiki pages, `docs/AI/` and durable guidance.
Do not include commit hashes, revision footers, local checkout folder names,
machine-specific paths, import/review chronology or narratives about earlier
implementations. Git records history. Use repository-relative source paths,
symbols, settings and test names when they help explain the current behavior.
Keep historical analysis in explicitly requested historical reports, not in
current-state documentation.

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

Dependency files, `compile.sh`, ChibiOS DEPDIR and F4 SRAM/CCM notes: [Build and CI Notes](docs/AI/build-and-ci-notes.md).

### Unit Tests

Host-build ADC shadowing and TEST_ENGINE MAF limits: [Unit Test Notes](docs/AI/unit-test-notes.md).

```bash
cd unit_tests
./test.sh

# Run a specific test
./test.sh TestName
```

`test.sh` is the recommended way to run tests as it automatically handles both the build (`make`) and execution.

#### Test-driven bugfixing: coverage first, fix second

Follow [TDB Test Driven Bugfixing](https://github.com/rusefi/rusefi/wiki/TDB-Test-Driven-Bugfixing):

1. First deliver a passing reproduction test that explicitly asserts the current bad behavior. Keep this coverage change separate from the fix.
2. In a subsequent change, fix the bug and update the same test to expect correct behavior. Changing the expectations demonstrates that the coverage exercises the fix.

Do not deliver the fix before coverage, combine the initial coverage and fix, or fix a bug without new coverage or adjusted expectations. Commits and merges remain human actions under Source Control Hygiene.

Coverage, test-output troubleshooting and `.teeth` replay: [Unit Test Notes](docs/AI/unit-test-notes.md).

Hardware CI timing, simulator feature flags, the WSL functional test and config generation commands
(`gen_config_board.sh`, `gen_config.sh`, `gen_enum_to_string.sh`): [Build and CI Notes](docs/AI/build-and-ci-notes.md).

## Architecture

### Directory Structure

- `firmware/config/boards/` - Hardware configuration and defaults for different ECU hardware. When adding or editing pin entries in board `connectors/*.yaml` files, follow the canonical `function:`/`type:` naming ("Pinout language conventions" - e.g. `Injector N`, `Smart Ignition Coil N`) documented in `firmware/config/boards/readme-boards.md`.
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
  - `ports/at32/` - not used at the moment; see [Firmware Debugging Notes](docs/AI/firmware-debugging-notes.md)
- `firmware/libfirmware/` - Reusable library code
- `firmware/util/` - Self-contained utilities (no external dependencies)
- `unit_tests/` - Google Test suite
- `simulator/` - Windows/Linux firmware simulator

### Deep Dive AI Guidance
For detailed technical documentation intended for AI assistants, see:
- [Firmware Business Logic Testing](docs/firmware-business-logic-testing.md) - Host-test seams, fuel-trim and PWM behavior tests, A/C state ownership and LTFT persistence follow-ups.
- [Fueling System](docs/AI/fueling_system.md) - Mass-based fueling pipeline (17 stages).
- [Cold-start Logic](docs/AI/cold_start.md) - Priming, cranking-state hysteresis, mass tables, ASE/WUE overlap, cycle counters and sensor fallbacks.
- [Acceleration Enrichment](docs/AI/acceleration_enrichment.md) - Four selectable AE modes including Lua mass injection, independent wall wetting, the 200 Hz TPS rate detector, correction units, flex compensation and diagnostics.
- [Ignition System](docs/AI/ignition_system.md) - Timing calculation and spark scheduling.
- [Launch Control](docs/AI/launch_control.md) - Activation modes, RPM arming latch, pre-launch timing/skip ramps, full cuts, shared-clutch flat shift, fuel-trim learning and diagnostic limitations.
- [Rev Limiters](docs/AI/rev_limiters.md) - Constant/CLT hard limits, strict hysteresis, timing/fuel ramps, independent ETB taper, fault ceilings, cut priority and diagnostics.
- [Engine State Machine](docs/AI/engine_state_machine.md) - Operating-state classification, shift recognition and optional STFT region selection.
- [VVT Control](docs/AI/vvt_control.md) - Activation gates, oil-pressure base duty, scheduled P gain, integral bounds and diagnostics.
- [Engine Protection](docs/AI/protection_system.md) - LimpManager and cut logic.
- [Sensor Framework](docs/AI/sensors_system.md) - Sensor registry, conversion pipeline, redundancy and mocking.
- [Kick-Start Cranking](docs/AI/kick-start.md) - kickStartCranking mode: both coils charged off the trigger edge and fired a dwell-time later below 800 RPM, normal spark suppressed via ClearReason::KickStart.
- [Scheduling & Timing](docs/AI/scheduling_system.md) - Microsecond timer, event queue/executor, angle-based scheduling, periodic callback rates (fast 200 Hz / slow 20 Hz) and other fixed-rate loops.
- [Lua Scripting API](docs/AI/lua_scripting.md) - Custom Lua hooks (lua_hooks.cpp and friends) grouped by category, indexing conventions, how to add a hook.
- [Lua Implementation Injection](docs/AI/lua_implementation_injection.md) - Subsystem replacement vs. adjustment hooks, native consumers, phase limits, units, timeouts and reset behavior.
- [SD Card Logging](docs/AI/sd_card_logging.md) - SD thread mode state machine, .mlg/.teeth formats, f_expand pre-allocation.
- [Engine Sniffer](docs/AI/engine_sniffer.md) - Digital Sniffer, tooth/composite capture, RPM gates, formats, diagnostic artifacts, and relevant MCP tools and limits.
- [Lookups & Log Field Metadata](docs/AI/lookup.md) - The three generated per-field tables (config value_lookup, output_lookup, MLG log fields): shared codegen machinery, differing direction/naming/flash shape, EFI_LUA_LOOKUP gating, dead switch-variant ODR hazard.
- [Hardware Quality Control & Direct I/O](docs/AI/hardware-quality-control.md) - Bench test subsystem, direct pin console commands, CAN QC protocol (0x770000) for factory test rigs, smart-driver diagnostics, trigger self-stimulation, ETB bench/autocal.
- [HW QC Stim Rig](docs/AI/hw-qc-stim-rig.md) - Tester firmware in rusefi-hardware `digital-inputs/firmware`: test cycle, board detection, analog/counter expectations, output ordinals that must match `getBoardMeta*` and the QC harness.
- [Configuration Storage](docs/AI/configuration_storage.md) - Storage manager, INT_FLASH/MFS/SD backends, double-copy settings write, extra flash pages piggybacked on the settings sector.
- [OpenBLT Firmware Validation](docs/AI/openblt_firmware_validation.md) - Build-time CRC32/size header at `.vectors + 0x1C`, start-up `FlashVerifyChecksum()` and main-loop start decision, per-XCP-command NVM/flash callbacks while flashing, reset-loop counters, PROGRAM_RESET outcome.
- [Firmware Flash Usage](docs/firmware-flash-usage.md) - uaefi121 measurements, feature-removal tradeoffs, flash-region accounting, and reproducible size comparisons.
- [Hellen Board Mapping](docs/hellen-board-mapping.md) - Connector-pin -> hellen-one module -> STM32 pin mapping chain: meta headers (H144_/MM100_ namespaces), connector YAMLs + PinoutLogic codegen, resistor-based board ID; also how to recover the mapping from module schematic PDFs (pdftotext + Altium hidden-text artifacts).
- [Connector Artwork](docs/imagegen.md) - Photo-to-drawing workflow and shared pin coordinates. Image generation does not reliably honor requested canvas dimensions or pixel positions; align the export and measure the final terminal centers before updating YAML.
- [Connector Placement Overlays](docs/AI/connector-placement-overlays.md) - Add connector context in unused image space while preserving the canvas and terminal pixels so YAML coordinates stay valid; remeasure coordinates if they change.
- [Hardware Re-init & requiresPowerCycle](docs/hardware-reinit-and-power-cycle.md) - How Burn applies settings live (activeConfiguration diff, applyNewHardwareSettings stop/start), and the annotated list of reboot-only settings.
- [Board Configuration Override Hooks](docs/board-configuration-override-hooks.md) - custom_board_DefaultConfiguration vs ConfigOverrides vs fix_configuration vs validateConfig vs OnConfigurationChange: lifecycle timelines and decision guide.
- [Sensor Rate-of-Change Handling](docs/sensor-rate-of-change-filtering.md) - per-sensor inventory of low-pass cutoffs, ExpAverage smoothing, slew limiting and rate-based control inputs; notably there are NO rate-of-change plausibility warnings anywhere.
- [Java Gradle Structure Review](docs/java-gradle-structure-review.md) - Gradle subproject inventory, dependency graph, and known structural issues in `java_console/` + `java_tools/`.
- [Java Frontend App](docs/AI/frontend-app.md) - Console startup, ECU connection and data flow, main user workflows, and module boundaries.
- [TunerStudio User Folders](docs/AI/tunerstudio-user-folders.md) - Every read/write of `~/.efiAnalytics` and TunerStudio project folders: plugin install, ecuDef cache priming and fallback, project auto-creation, post-migration check, plugin tune/log access.
- [Console Pinout Tab](docs/AI/pinout-tab.md) - Connector YAML packaging, cache validation, pin rendering, tune assignments, and Tuning navigation.
- [PatchCordHelper Pinout Tool](docs/AI/pinout-helper.md) - Patchcord wiring matches, assembly steps, connector images, and generated instructions.
- [Java Connectivity & UI Unit Testing](docs/java-connectivity-ui-unit-testing.md) - Test approach for the console connectivity/flashing/session layer and Swing UI: established fake/seam patterns and a refactoring-cost-ordered test backlog.
- [Build and CI Notes](docs/AI/build-and-ci-notes.md) - Dependency files, compile.sh, hardware CI timing, simulator flags, config generation commands, feature flags, compiler warnings, MinGW weak linkage.
- [Unit Test Notes](docs/AI/unit-test-notes.md) - ADC shadowing, coverage, test-output troubleshooting, `.teeth` replay, `EFI_UNIT_TEST` usage.
- [Firmware Debugging Notes](docs/AI/firmware-debugging-notes.md) - ChibiOS ADC callbacks, nested locks, AT32, H7 ADC mux, faults, stack overflow, MPU guard pages, reboot evidence, OpenBLT marker.
- [Console Notes](docs/AI/console-java-notes.md) - Swing invariants and tests, keyboard shortcuts, themes, Java version constants.
- [Configuration and Code Generation](docs/AI/configuration-codegen.md) - Key concepts, generated configuration layout, generated files never committed.
- [Documentation Cross-Reference Tags](docs/AI/code-tags.md) - Full `[tag:...]` rules.
- [Connectivity Notes](docs/AI/connectivity-notes.md) - MCP servers, serial, SLCAN, USB MSD/CDC, WeAct.

### Key Concepts

Configuration, generated layout and the generated files that must never be committed: [Configuration and Code Generation](docs/AI/configuration-codegen.md).

### Compiler Flags

- C99 with GNU extensions for C code
- C++20 for firmware code
- C++17 (minimum) for unit tests and host-side tooling; portable C++17 features such as `std::filesystem`, `std::optional`, `std::string_view` and structured bindings are preferred over platform-specific APIs to keep unit tests cross-platform (Linux/macOS/Windows-MSVC/MinGW)
- No RTTI, no exceptions (`-fno-rtti -fno-exceptions`)
- LTO enabled by default
- Warnings-as-errors differ per compiler (GCC vs clang); build with both when touching warnings: [Build and CI Notes](docs/AI/build-and-ci-notes.md).

### Build Conditionals

Key preprocessor flags that control compilation. These three are **mutually exclusive** — any given translation unit is compiled in exactly one of these modes:

- `EFI_PROD_CODE=1` - Production firmware build (cross-compiled for STM32, ChibiOS available, real HAL).
- `EFI_SIMULATOR=1` - Desktop simulator build (`simulator/`), ChibiOS available via the simulator port.
- `EFI_UNIT_TEST=1` - Host-side Google Test build under `unit_tests/`. No ChibiOS, no real HAL — runs as a plain native binary on Linux (GCC/Clang), macOS (Clang) and Windows (MSVC and MinGW).

Adding feature flags used by shared code: [Build and CI Notes](docs/AI/build-and-ci-notes.md).

Using `EFI_UNIT_TEST` in code and its relation to test infrastructure: [Unit Test Notes](docs/AI/unit-test-notes.md).

#### Cross-platform requirement under `EFI_UNIT_TEST`

Any code reachable from a unit-test build (`unit_tests/` itself, plus firmware sources guarded by `#if EFI_UNIT_TEST` or compiled unconditionally into the test binary) **must build on all four host toolchains**: Linux GCC/Clang, macOS Clang, Windows MSVC, Windows MinGW. Prefer portable C++17 (`std::filesystem`, `std::string`, `std::string_view`, `std::optional`, structured bindings) over POSIX-only (`realpath`, `PATH_MAX`, `dirent.h`) or Win32-only APIs. Guard any unavoidable platform-specific call with an explicit `#ifdef`.

## Source Control Hygiene

- **Never commit or push — only a human does either.** Both `git commit` and `git push` (to any remote, any branch) are reserved for the human. Leave changes uncommitted in the working tree and summarize what changed; the human commits and pushes.
- **Git add is allowed; stage new files immediately**: When you create a new source file (C/C++ headers/sources, Java/Kotlin sources, unit tests, scripts, build files, resources, docs, etc.), run `git add <path>` as part of the same change so it shows up in `git status` / `git diff` and is not lost on the next clean or branch switch.
- Do not stage `docs/report.md` (local commit-message preparation only), build artifacts or generated files (see "Do not attempt to commit any generated files" in [configuration-codegen.md](docs/AI/configuration-codegen.md)), IDE-local files, or user-specific configs. The report is excluded from the new-file staging rule even if it is already tracked.

## Coding Style

- Always use curly brackets for `if` statements, even for single-line blocks.
- Reflection is prohibited in Java unit tests — add an explicit `...ForUnitTest`/`...ForTests` seam on the production class or widen a member to package-private instead (see [Java Connectivity & UI Unit Testing](docs/java-connectivity-ui-unit-testing.md)).

- Link non-obvious invariants across files with `[tag:lower_snake_case]` comment markers; reuse an existing tag's exact spelling and explain the shared rule at one site. Full rules: [Documentation Cross-Reference Tags](docs/AI/code-tags.md).

## Embedded Code Practices

H7 ADC mux, null-pointer faults, log buffers and F4 stack overflow: [Firmware Debugging Notes](docs/AI/firmware-debugging-notes.md).

- **Static allocation only**: Embedded firmware uses only static memory allocation. No heap usage (`new`, `malloc`, `std::vector`, `std::string`, `std::map`, etc.) is permitted in production firmware code. Use fixed-size containers like `cyclic_buffer` from `rusefi/containers/cyclic_buffer.h` instead. Memory is limited and fragmentation must be avoided.
- **Performance matters**: This is a hard real-time application. Fuel and ignition events must fire at precise crank angles. Avoid unnecessary computation in hot paths. Use lower priority threads for expensive computation.
- **No exceptions**: C++ exceptions are disabled. Use return values or error codes for error handling. rusEFI distinguishes three kinds of errors — `warning()` (recoverable runtime), `configError()` (recoverable bad tune) and `firmwareError()` / `criticalError()` (unrecoverable). See the header comment in `firmware/controllers/core/error_handling.h` for when to use which.
- **No RTTI**: `dynamic_cast` and `typeid` are unavailable.
- **Interrupt safety**: Be mindful of code that runs in interrupt context vs. thread context. Use appropriate synchronization primitives.
- **Stack usage**: Keep stack allocations small. Large arrays should be static or global, not local variables.
- **No float→int64 conversions**: CI (`firmware/check_illegal_conversion.sh`) fails any board image containing `__aeabi_f2lz`. The usual trigger is adding a float time offset to an `efitick_t` timestamp (e.g. `timestamp + MS2NT(floatMs)` — `MS2NT` promotes the int64 to float and back, losing precision). Use `sumTickAndFloat(timestamp, MSF2NT(floatMs))` (or `USF2NT` for µs) from `firmware/util/efitime.h` instead; see `spark_logic.cpp` for the idiom.

## Further notes

- Console keyboard shortcuts, themes, Java version constants, Swing: [Console Notes](docs/AI/console-java-notes.md)
- MCP servers, serial, SLCAN, USB MSD/CDC, WeAct: [Connectivity Notes](docs/AI/connectivity-notes.md)
- MPU guard pages, reboot evidence, OpenBLT version marker: [Firmware Debugging Notes](docs/AI/firmware-debugging-notes.md)
- MinGW/COFF weak linkage: [Build and CI Notes](docs/AI/build-and-ci-notes.md)

## Development Notes

- Supported IDE: Visual Studio Code
- Requires Unix-like OS (Linux, macOS, or Windows WSL)
- All PRs must pass CI gates (firmware builds for all boards, unit tests)
- Wiki: https://wiki.rusefi.com/
- Adding a new trigger: `docs/adding-new-trigger.md`
- TunerStudio General Notes: `.junie/ts-readme.md`

See also .junie/guidelines.md file

Pixi's Windows m2-bash can report OSTYPE=cygwin while uname -s reports MSYS_NT. Platform checks based on uname must include MSYS as well as CYGWIN and MINGW.
