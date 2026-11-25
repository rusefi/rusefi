# Build and CI Notes

Build-system, CI and simulator details moved out of CLAUDE.md.

Firmware and unit-test builds keep dependency files in `firmware/.dep/` and `unit_tests/.dep/` (separate from the object dirs). After a source file is moved or deleted (e.g. a controller promoted into `controllers/modules/`), incremental builds fail with `No rule to make target '<old path>.cpp'` until the stale `.dep` is removed - `make clean` or `rm -rf .dep` fixes it; wiping only `build/obj` does not. Also note the first build after such a wipe can measure several KB larger than an identical follow-up build - for flash-size comparisons, compare consecutive rebuilds of each variant, never a single post-wipe build. `firmware/bin/compile.sh` has been observed to exit 0 even when the underlying `make` failed - check the log (or that `build/rusefi.elf` got a new timestamp), not just the exit code.

After ChibiOS build-rule changes, validate generated-header dependencies with an empty `DEPDIR` (a fresh directory under `/tmp` is sufficient). Existing compiler-generated `.dep` files can hide missing explicit Makefile prerequisites: an incremental build generates the RAM disk correctly while a clean CI build compiles against its checked-in placeholder header.

On STM32F4, a linker error at `.heap_ccm` saying `cannot move location counter
backwards` from an address above `0x10010000` means static data exceeds the
64 KiB CCM RAM region, not flash. Inspect both SRAM and CCM before removing
features. `PERSISTENT_LOCATION=` places the settings container in main SRAM;
the Lua configuration page stays in CCM and Lua can allocate from both heaps.

F4 can overflow both SRAM and CCM in the same build. The linker stops at the
first backwards heap assignment, so fixing main SRAM can expose a second CCM
failure. After linking, inspect `__heap_base__`/`__heap_end__` and
`__heap_ccm_base__`/`__heap_ccm_end__`: the memory-usage table counts the heaps
as allocated and reports 100% even when space remains. Firmware CI samples
boards using their metadata `SKIP_RATE`; a green master run is not evidence
that a particular board was built.

### Hardware CI settings-write timing

Hardware CI's F407 `HighRevTest` is sensitive to asynchronous settings burns: `setEngineType()` queues a forced save, while the Java helper resumes configuration changes after a fixed sleep. Passing master runs can already contain `Flash: validation failed`; do not treat that message alone as a new regression. In PR #10186, adding a 5 s settings retry backoff moved the next erase from the RPM settling period into the 40 s assertion window, producing `engine stopped`, ~1200 ms coil-overcharge warnings, and 6000 -> 0 RPM failures. Self-stimulation explicitly permits settings writes even on F4, where flash erase stalls execution. Check flash-write timing against the assertion window before investigating trigger decoding. The cause of the original validation mismatches was not established by those logs (concurrent mutation of the live configuration is a candidate).

### Hardware CI ST-LINK diagnostics

With st-flash 1.8.0, `Mass erase completed successfully` followed by `Failed to reset device` can still return exit 0: the erase result remains in `err` when reset failure jumps to cleanup (upstream `src/st-flash/flash.c`). An exit-code-only wrapper therefore cannot establish successful reset. Repeated `0 KiB flash` plus reset timeouts indicates invalid target discovery, even if the probe serial and chip ID are readable; check target power, NRST and SWD connections before blaming the firmware image. The zero size also makes the normal flash base fail st-flash's write-region bounds check (`Unknown memory region`).

### Simulator build feature flags

The simulator Makefile does not track changes to board-derived
`TS_PAGE_GUARD_DEFS` as object dependencies. When switching between board
configurations with different feature flags (for example, `EFI_LUA=FALSE` and
Lua enabled), run `make clean` in `simulator/` before validating the new build;
otherwise stale objects can make it appear to pass without compiling the new
configuration.

### Simulator Functional Test (local WSL quirks)

`./gradlew simulatorFunctionalTestLauncherWithSimulator` (repo root; also CI `build-simulator.yaml`, Linux-only) launches `simulator/build/rusefi_simulator` and talks TS protocol over TCP :29001. Two local traps, both hit 2026-08-25:

- **Anything already listening/connecting on :29001 corrupts the run.** Windows-side apps (e.g. `rusefi_dash.exe`) reach the WSL simulator via WSL2 localhost forwarding and auto-connect to every simulator the test launches; the launcher then fails with "No response from simulator". Check `tasklist.exe` for dash/console processes before debugging "mystery" connection failures.
- **Bench-pin timing asserts (`testPwmPin`, `testOutputPin`) are wall-clock based and flaky under WSL2 load** — a loaded machine fails a *different* pin test each run with duration mismatches. The FS-image / CAN / trigger asserts are deterministic; treat lone pin-duration failures locally as environmental and let CI arbitrate.

```bash
# Generate configs for a specific board
firmware/gen_config_board.sh <board>

# Generate all board configs
firmware/gen_config.sh

# Generate enum-to-string conversions
firmware/gen_enum_to_string.sh
```

### Compiler warnings

- Warnings-as-errors differ per compiler: unit tests build under both GCC (Linux/Windows CI) and clang (macOS CI, or `make CC=clang` locally), and each catches things the other silently accepts (e.g. clang's `-Wunused-but-set-variable`/`-Wdynamic-class-memaccess`; GCC 13 and clang 18 both *suppress* `-Wtype-limits`-style tautological-comparison warnings inside template bodies, so a finding reported by one CI compiler may be unreproducible locally). Before declaring a warning fix complete, build unit tests with both: `make -j12` and `make clean && make CC=clang -j12`. GCC-only flags in `firmware/rusefi_rules.mk` must stay behind its `USE_CLANG` guard; `unit_tests/unit_test_rules.mk` sets `USE_CLANG` from clang auto-detection *before* including the shared rules — keep that ordering.

#### Adding feature flags used by shared code

The firmware, simulator, and unit tests have independent feature definitions.
Adding a default to `firmware/config/stm32f4ems/efifeatures.h` covers its F7/H7
inheritors, but does **not** cover `simulator/simulator/efifeatures.h` or
`unit_tests/efifeatures.h`. When adding a flag used in shared `#if` expressions,
give it an explicit value in each applicable feature header, including `FALSE`
for unsupported hardware. Check other standalone port headers when applicable.
Do not suppress `-Wundef` or replace the guards with `#ifdef` to hide a missing
definition.

- MinGW/COFF weak fallback definitions must be visible in the calling
  translation unit. A separate weak definition does not resolve an ordinary
  external reference; weakening only the caller can produce a successful
  link with a call to address zero. Keep the board-hook fallback bodies in
  shared include fragments: Windows callers include them locally, while
  ELF/ARM keeps them in separate translation units for LTO-safe overrides.
  Keep public declarations strong so board implementations stay strong.
  Validate both default and override targets with
  `python3 simulator/tests/test_board_hook_linkage.py`; linking alone is
  insufficient evidence for the Windows fallback path.
