# Unit Test Notes

Unit-test build details, coverage, output troubleshooting, `.teeth` replay and `EFI_UNIT_TEST` usage.

The host build shadows `firmware/hw_layer/adc/adc_inputs.h` with
`unit_tests/adc_inputs.h`. Including the short name in a host test does not
expose newly added production declarations; keep the mock declaration in sync
when testing a shared ADC entry point. Host tests run the shared ADC subscription
and filtering code with per-channel voltages supplied by `setMockAdcVoltage()`.
Call `AdcSubscription::UpdateSubscribers()` explicitly to deliver samples;
posting directly to a sensor cannot establish that it remains subscribed.
`EngineTestHelper` clears subscriptions and mock voltages between tests. A sensor
subscribed during `EngineTestHelper` construction can carry a zero input divider, so
its samples read 0 V whatever the mock voltage; `SubscribeSensor` keeps an existing
entry for an unchanged channel, so deinit the sensor, set
`analogInputDividerCoefficient`, then init it again (see
`TpsRawVoltageUnavailableWithInvalidCalibration`). These
host tests do not establish hardware DMA acquisition or pin configuration.

`TEST_ENGINE` enables MAF by default. In host tests, `init_maf.cpp`'s static
`mafCurve` binds its table references before `EngineTestHelper` installs `config`;
delivering ADC samples to that MAF converter dereferences an invalid table.
Disable both MAF ADC channels in unrelated subscription tests (see
`MapSensorReconfiguration`); testing MAF delivery requires fixing that lifetime.

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

Linux CI keeps two `ccache` directories under `~/.cache/rusefi-ut`: `coverage`
for the ASan/coverage build and `memcheck` for the unsanitized rebuild. Only
compiler outputs are cached. A successful default-branch job saves the cache;
PRs can restore the latest compatible archive and compile their own changes.

Unit tests use Google Test and run on PC, not on the ECU.

Fuel-pipeline fixtures used by custom boards must initialize `config->lambdaTable`
explicitly: the default lambda calibration only populates a 16x16 table. With
some custom boards 24 RPM bins, a `TEST_ENGINE` fixture otherwise divides by zero and returns
infinite fuel mass, which can silently satisfy floating-point equality checks.

If a fresh unit-test build fails with `can't create build/obj/*.o: No such file or directory`, create `build/obj`, `build/lst`, and `.dep` under `unit_tests/`, then rerun `make -j12`. A partially created `build/` directory can satisfy the current directory prerequisite without its object subdirectory existing (observed during the MCP4728 work, 2026-09-27).

Restoring tracked `page_N_generated.h` files after code generation can leave them
inconsistent with other generated headers while `.config-sentinel` remains current.
If a subsequent build fails a page-size static assertion, force configuration
regeneration with `make -j12 -W ../firmware/integration/rusefi_config.txt`.

When testing actuator init paths, remember that `EngineTestHelper` construction already runs `commonInitEngineController()` -> `initElectronicThrottle()`: with `TEST_ENGINE` (which defaults `etbFunctions[0]=DC_Wastegate`) DC hardware pool slot 0 is started *before* the test body runs, and the `DcHardware::isStarted` latch silently ignores any re-init with different settings. Call `resetDcHardwareForUnitTest()` / `resetIdleHardwareForUnitTest()` (both `EFI_UNIT_TEST` seams; also invoked by the `EngineTestHelper` constructor for cross-test isolation) before re-initializing with test-specific config. The idle stepper stack (`initIdleHardware()`, `StepDirectionStepper`, `DualHBridgeStepper`) compiles in unit tests; in that build `StepperMotor` is an alias of `StepperMotorBase` with no thread — tests drive it by calling `doIteration()` manually (see `test_idle_hardware.cpp`).

#### Troubleshooting test output

`RpmCalculator::setRpmValue()` invokes `periodicFastCallback()` when RPM changes
from zero to nonzero. This recalculates target lambda and other fuel state even
in a test that otherwise calls only one controller. Set controlled fuel inputs
after starting/restarting RPM, or use the recalculated target when checking that
a learned trim is held (see `ShortTermFuelTrimBehavior`).

For suspected test-order leaks, run the built binary directly from `unit_tests/`:
`build/rusefi_test --gtest_shuffle --gtest_random_seed=31337 --gtest_repeat=3`.
Record the failing iteration's seed and keep the same test selection when replaying;
changing `--gtest_filter` changes the shuffled order. `test.sh` accepts a name filter,
not arbitrary GoogleTest flags. `EngineTestHelperBase` rewinds simulated time to zero,
while `Sensor::resetRegistry()` only clears registry entries: a file-static
`StoredValueSensor` can retain a previous value/timestamp and appear fresh after the
clock rewind. Reset/invalidate the underlying object, not just its registration.

File-static `FrequencySensor` objects retain edge-timer and biquad history across
the fixture's clock rewind. That can give the next edge a zero elapsed interval,
feeding infinity and then NaN into the filter; invalidating the published value
does not clear this history. Call `resetHistoryForUnitTest()` once at fixture
setup, as in `OutputShaftSpeed.PulseInputTimeoutAndReconfiguration`. Do not reset
it again inside a production-reconfiguration test, which must exercise the real
init/deinit behavior. This test-clock issue does not establish a hardware fault.

For Valgrind, rebuild with `SANITIZE=no`; the normal non-Windows unit-test build
enables ASan. From `unit_tests/`, use `make clean`, `make -j12 SANITIZE=no`, then
`./run_with_valgrind.sh`. `make_for_valgrind.sh` selects the flag but does not clean
old objects, so running it alone after an ASan build can reuse incompatible objects.

CI sets `RUSEFI_SKIP_REPLAY_LOGS=1` only for the Memcheck pass. Formatting diagnostic
rows at 500 Hz dominates the Kawasaki 8-1, Nissan Maxima and Suzuki K6A cranking
replays under instrumentation. The recordings still run in full. Leave the variable
unset when reproducing a failure locally to get the usual MSL/CSV files.

`EngineTestHelper::applyTriggerWaveform()` also increments the global configuration version and notifies hardware controllers. Static ETB controllers can retain PID pointers into an earlier test's destroyed configuration when the next test skips throttle initialization (for example, without TPS sensors). A later configuration notification then reads the stale pointer in `Pid::isSame()`, detectable under Valgrind. For tests that only need trigger/cylinder geometry refreshed, use `engine->updateTriggerConfiguration()` directly; tests of global configuration changes still require ETB lifetime isolation. See the MAP sampling tests and the 2026-09-29 report.

Starter tests must explicitly clear `enginePins.starterControl` in setup and cleanup. `EngineTestHelper` resets injector/coil outputs and pin registrations, but an unassigned starter retains its logical output across tests; the next `doStartCranking()` then sees an already-engaged starter and skips new-engagement behavior.

To inspect what a test actually scheduled/executed (events, timings, sniffer/logic traces) call `setUnitTestCreateLogs(true)` (declared in `unit_tests/test-framework/engine_test_helper.h`) before constructing `EngineTestHelper` — typically from `main.cpp` or at the top of an individual test. When enabled, each test writes per-test artifacts (e.g. `unittest_<Suite>_<Name>_trace.json`, logic-data, and engine-sniffer files) into the `unit_tests/test_results/` directory (`TEST_RESULTS_DIR` in `unit_test_logger.h`); the absolute path is printed at process exit by `sayByeBye()`. This is the recommended way to diagnose unexpected scheduler/RPM/injection behavior instead of adding ad-hoc `printf`s.

See also unit_tests/test_results/readme.md for unit tests output.

`EngineTestHelper` uses Google Test suite/test names directly in artifact paths.
Parameterized names contain `/`; with test logging enabled, the missing parent
directories make the logic-data writer dereference a null `FILE*` on teardown.
Use explicitly named `TEST` cases sharing a helper when their traces are needed.

#### Measuring trigger configuration cost

After building, run `build/rusefi_test --gtest_also_run_disabled_tests
--gtest_filter=TriggerConfigurationBenchmark.*` from `unit_tests/` (on one line).
The opt-in benchmark uses `std::chrono::steady_clock`, not the mocked ECU clock,
and reports microseconds per call for complete reconfiguration, waveform
construction, sync discovery/validation, and output preparation with VVT disabled.
It checks successful initialization but deliberately has no wall-clock threshold.
The default host build uses `-O0` and sanitizers; its timings are not MCU estimates.

`Engine::updateTriggerConfiguration()` logs and returns when the RPM sensor
reports a positive value. At zero (or unavailable) RPM it performs these stages
without a ChibiOS system lock. Geometry tests must configure the engine before
setting a positive RPM, or explicitly stop, reconfigure, and restart.
`TriggerConfiguration.UpdateWhileRunning` checks that rejected updates preserve
both waveform and derived output geometry and that an explicit call at zero RPM
applies the changes. Waveform construction calls
`findAngleMatch()` for every appended edge, giving N(N-1)/2 approximate float
comparisons for N edges, even though `addEvent()` requires increasing angles.
Sync initialization runs the real decoder until synchronized and then feeds six
additional waveform cycles through `assertSyncPosition()` in production as well
as tests. Active VVT shapes each get their own initialization. The simulation
advances synthetic timestamps without sleeping. Production additionally has
logging, timer and optional perf-trace costs that the host does not reproduce;
in particular, enabled sniffer/verbose sync logging can format gaps during the
synthetic decoder run. Measure the same stages with a hardware cycle counter to
attribute MCU latency. The RPM gate is a snapshot, not synchronization against
new trigger edges arriving during initialization. Skipped updates are not queued
for retry when the engine stops; see `docs/hardware-reinit-and-power-cycle.md`.

#### Replaying `.teeth` (rusEFI tooth logger) captures in trigger tests

`.teeth` files under `unit_tests/tests/trigger/resources/` are rusEFI's own tooth-logger exports, not logic-analyzer traces, and they carry two traps: (1) the logger records in bursts, so long captures contain periodic ~0.6 s holes with no edges at all - each hole costs one resync error that no gap window can remove, so assert error counts per clean section rather than a global zero; (2) the `Sync`/`TDC` columns are the recording ECU's own decoder state and serve as ground truth - if the unit-test error counter increments at the same timestamps the `Sync` column drops, the test reproduces the field behaviour and the remaining errors are in the signal, not the decoder. Captures longer than a few seconds overflow the 16 MB per-test log cap - wrap the test in `ScopedUnitTestCreateLogs logDisabler(false)` (see `test_real_genmax_24_2.cpp`, `test_real_bmw_e90_cam.cpp`).

**Cross-platform requirement**: Unit test code MUST build and run on all supported host platforms — Linux (GCC/Clang), macOS (Clang), and Windows (MSVC and MinGW). Avoid POSIX-only APIs (e.g. `realpath`, `PATH_MAX`, `dirent.h` without guards) unless wrapped in `#ifdef` guards or replaced by portable C++ equivalents. Prefer `std::filesystem` over POSIX path APIs.

MinGW can leave an ordinary cross-translation-unit call unresolved when the
implementation alone carries `PUBLIC_API_WEAK`, even with its object linked
directly. A two-file reproduction fails with MinGW GCC 13 and passes with Linux
GCC/Clang; making the definition strong fixes the MinGW link. Windows CI GCC
14.2 hit this when sensor tests first called `getAnalogInputDividerCoefficient`
directly (run 37217674429). Check weak/strong symbol linkage before adding mocks
or changing source lists. Preserve production board overrides when changing
weak definitions; migrate them to explicit board callbacks or narrowly scope
the host-build adjustment.

#### Using `EFI_UNIT_TEST` in code

- `#if EFI_UNIT_TEST` — include a host-only path: stub out HAL/ChibiOS/board-specific calls, expose extra accessors for tests, or substitute portable C++ for embedded primitives.
- `#if !EFI_UNIT_TEST` — exclude code that cannot compile on the host (board pin macros, ChibiOS threads/HAL, MCU registers, etc.).
- For three-way splits, combine with `EFI_PROD_CODE` / `EFI_SIMULATOR` rather than negating one flag.
- The flag is set by `unit_tests/Makefile` (and the unit-test CMake/IDE projects). It is **never** set by board `compile_*.sh` scripts or the simulator build.

#### Relationship to test infrastructure

`EFI_UNIT_TEST` is the *compile-time* gate; the *runtime* test scaffolding it enables lives in `unit_tests/test-framework/` — most notably `EngineTestHelper` and `setUnitTestCreateLogs(true)` (see the "Troubleshooting test output" section above).
