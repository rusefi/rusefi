# Coverage only open issues

**7 host or simulator coverage tickets, 3 tickets needing recovered input artifacts, and 16 hardware validation tickets** remain open. Another 3 open coverage requests already have an implementation or substantial matching coverage. Start with #8371, #7989 and #3306 for software work. The three closed coverage requests below are excluded from the remaining-work counts.

Snapshot: 21 October 2026. The live [GitHub issue inventory](https://github.com/rusefi/rusefi/issues) contained **1,269 open issues and 2,488 comments**, with no pull requests included. REST and GraphQL issue IDs matched, and every exported comment count matched GitHub REST. Source baseline: [master `cd0baa77ae8d`](https://github.com/rusefi/rusefi/commit/cd0baa77ae8dfae395c8ae1b386f1a83c5aa24b7), which matched GitHub master when checked.

A coverage-only deliverable adds tests, assertions, capture fixtures or CI execution for existing behavior. It does not silently include a production behavior fix. Some cases may expose a defect: follow the repository’s coverage-first process, keeping a passing reproduction of current behavior separate from the later fix and changed expectation. Hardware validation is listed separately because its evidence cannot be produced by host tests.

## Host and simulator coverage

| Issue | Priority | Concrete deliverable | Existing overlap or boundary |
| --- | --- | --- | --- |
| [#8371 Real trigger tests: check that RPM stays above 0 after first RPM](https://github.com/rusefi/rusefi/issues/8371) | High | Add an opt-in continuity assertion to `EngineCsvReader`, with a test that obtains RPM and later loses it. Define exclusions for captures with intentional stops or logger holes. | [engine_csv_reader.h](../unit_tests/test-framework/engine_csv_reader.h) currently checks only the first nonzero RPM; do not impose continuity on every real recording. |
| [#7989 test coverage for console closes if we have non-rusEFI serial ports](https://github.com/rusefi/rusefi/issues/7989) | High | Use fake serial streams to exercise an unrelated/non-rusEFI port and then a valid ECU, proving discovery does not terminate the process and still succeeds. | [SerialPortScannerTest.java](../java_console/connectivity/src/test/java/com/rusefi/SerialPortScannerTest.java) and [SerialPortScannerInspectPortsTest.java](../java_console/connectivity/src/test/java/com/rusefi/SerialPortScannerInspectPortsTest.java) provide scanner seams. The linked #7987 describes an actual process-exit regression; scanning-list coverage alone is insufficient. |
| [#3306 unit test for IAC H-Bridge Microstepping](https://github.com/rusefi/rusefi/issues/3306) | Medium | Extend H-bridge idle tests to half, quarter and eighth microsteps; assert both phase duty sequences, direction/inversion and relevant current-limit scaling. | [test_idle_hardware.cpp](../unit_tests/tests/actuators/test_idle_hardware.cpp) drives the actual host DC pool but explicitly uses `STEPPER_FULL`. [stepper_dual_hbridge.cpp](../firmware/hw_layer/stepper_dual_hbridge.cpp) contains the fractional-step behavior. |
| [#3322 Unit test help for Fix cranking->idling taper phase for useSeparate*ForIdle tables #3168](https://github.com/rusefi/rusefi/issues/3322) | Medium | Add an integrated taper scenario for both separate idle VE and separate idle ignition settings; compare idle, taper and running/TPS override behavior. | PR #3168 is about the real idle-phase gate. [test_idle_controller.cpp](../unit_tests/tests/test_idle_controller.cpp) covers phases and [test_ignition_state.cpp](../unit_tests/tests/ignition_injection/test_ignition_state.cpp) mocks the idle controller; combine the relevant behavior without mocking away the transition. Confirm current desired semantics before freezing expectations. |
| [#2076 unit test for current "oops second wheel is not reporting data" handling](https://github.com/rusefi/rusefi/issues/2076) | Medium | Feed a two-wheel pattern with both inputs, then only the primary input; assert decoded angles and resulting scheduled actuation or suppression. | This is specifically a missing-secondary-wheel angle test, not just nonzero RPM. Start in [test_trigger_decoder.cpp](../unit_tests/tests/trigger/test_trigger_decoder.cpp) and the real-trigger helper. |
| [#4709 Automation coverage for RPM not reading](https://github.com/rusefi/rusefi/issues/4709) | Medium | Reproduce the sensor/trigger initialization regression from #4706 through a functional path and assert RPM becomes nonzero after feeding the known waveform. | Existing [test_real_4b11.cpp](../unit_tests/tests/trigger/test_real_4b11.cpp) coverage is useful, but determine whether it actually exercises the failed initialization sequence before adding another waveform-only test. |
| [#3561 need CI for Lua in simulator with rusEFI console](https://github.com/rusefi/rusefi/issues/3561) | Medium | Run Lua through the simulator/Console connection in CI: upload a small script, trigger it, observe deterministic output and reset it. | The issue explicitly says the bug was fixed and it was reopened for CI. Host Lua tests are not the requested simulator integration. [SimulatorFunctionalTest.java](../java_console/autotest/src/main/java/com/rusefi/simulator/SimulatorFunctionalTest.java) is the existing functional entry point. |

## Coverage requiring input artifacts

| Issue | Dependency | Concrete deliverable | Boundary |
| --- | --- | --- | --- |
| [#8864 tri tech logic analyzer recording for test coverage](https://github.com/rusefi/rusefi/issues/8864) | Real trigger recording | Obtain the TriTach recording, then add deterministic replay assertions for synchronization, RPM and angles. | The issue explicitly asks for a capture. Without it, an LLM can prepare a fixture but cannot deliver the requested real-signal coverage. |
| [#3659 hellen81_config merge: ISO-TP firmware unit test](https://github.com/rusefi/rusefi/issues/3659) | Legacy branch recovery | Recover the `hellen81_config` branch tests and compare them with current CAN serial/ISO-TP tests. | Current [test_can_serial.cpp](../unit_tests/tests/controllers/can/test_can_serial.cpp) has extensive transport coverage. The ticket asks to preserve specific branch content; generic ISO-TP tests do not prove that import happened. |
| [#3658 hellen81_config merge: firmware adc unit test](https://github.com/rusefi/rusefi/issues/3658) | Legacy branch recovery | Recover the ADC tests from `hellen81_config`, identify their assertions, and port only missing cases. | Current [test_sensor_init.cpp](../unit_tests/tests/sensor/test_sensor_init.cpp) and ADC callback/shared-sampler tests overlap the subject. The old branch content is the acceptance evidence still needed. |

## Hardware validation

| Issue | Kind | Required evidence |
| --- | --- | --- |
| [#9508 HW CI to use logic analyzer for timing validation](https://github.com/rusefi/rusefi/issues/9508) | Timing capture / HIL | Assert ignition timing using the hardware logic analyzer, with known stimulus and timestamps. Coordinate with #2112 to avoid two parallel timing suites. |
| [#2112 HW_CO assertWave](https://github.com/rusefi/rusefi/issues/2112) | Timing capture / HIL | Assert injector/coil wave timing on real hardware. Specify deterministic sensor inputs and acceptable timing tolerances. |
| [#8102 real test ignitionKeyDigitalPin](https://github.com/rusefi/rusefi/issues/8102) | Manual vehicle or bench test | Record delayed ignition shutoff behavior using the configured ignition input, including USB-powered and ignition-powered transitions. |
| [#8075 manual test WBO FW update 8chan](https://github.com/rusefi/rusefi/issues/8075) | Manual WBO update test | Exercise the wideband firmware update on an 8-channel board and retain before/after version and behavior evidence. |
| [#7973 manual test of wbo prod-2025-05](https://github.com/rusefi/rusefi/issues/7973) | Manual WBO compatibility test | Verify the referenced WBO release with both LSU 4.9 and FAE hardware. The old release may warrant retiring the release-specific task after maintainers choose the supported target. |
| [#8667 first test of CAN-FD](https://github.com/rusefi/rusefi/issues/8667) | CAN FD bench test | Confirm a real CAN FD frame exchange with the specified adapter, bitrate and payload length; classic CAN success is insufficient. |
| [#7684 HW CI to use artificial critical error](https://github.com/rusefi/rusefi/issues/7684) | HIL fault-path test | Issue the artificial critical error command, then assert reboot/recovery and the expected diagnostic evidence. |
| [#7034 Real hardware ICU coverage on nucleo-F767](https://github.com/rusefi/rusefi/issues/7034) | HIL input capture | Add the two physical jumper paths on Nucleo F767 and assert real timer/ICU capture, beyond host peripheral mocks. |
| [#5730 negative test: what happens if we short oscillator?](https://github.com/rusefi/rusefi/issues/5730) | Hardware fault injection | Use a controlled bench to establish behavior when the oscillator is disturbed. An LLM can write the test protocol but cannot establish electrical outcomes. |
| [#5987 lua can bus relay experiments](https://github.com/rusefi/rusefi/issues/5987) | CAN performance experiment | Measure Lua CAN transmit and receive success/rate with the specified hardware and adapters. It is validation-only in intent, with real traffic as the missing input. |
| [#4277 test SocketCAN](https://github.com/rusefi/rusefi/issues/4277) | Manual SocketCAN integration | Run the SocketCAN connector against a dongle and ECU, and record connection plus data/command behavior. |
| [#4262 rev limiter real test data](https://github.com/rusefi/rusefi/issues/4262) | Real vehicle data | Collect rev-limit behavior with the desired cut settings and timing/log evidence. Host limiter tests do not substitute for the requested vehicle evidence. |
| [#1958 ETB hardware-in-the-loop testing](https://github.com/rusefi/rusefi/issues/1958) | ETB HIL | Automate the physical STM32 ETB control loop, using a real or emulated throttle plant. This is a substantial validation setup, not a small host unit test. |
| [#2085 HW CI: debounce via start/stop](https://github.com/rusefi/rusefi/issues/2085) | HIL debounce | Feed controlled pulses into the start button input and assert the debounce/start count through the protocol. |
| [#1670 HW CI should use binary protocol for simulator enable/disable commands](https://github.com/rusefi/rusefi/issues/1670) | Protocol coverage on HIL | Exercise simulator enable/disable via the binary TS command path in the Java hardware suite; verify state instead of relying on a text command side effect. |
| [#1645 MRE analog input test](https://github.com/rusefi/rusefi/issues/1645) | MRE hardware QC | Assert physical analog voltages/divider conversions against the specified QC board values and wiring. |

## Coverage already present

| Issue | Disposition | Evidence |
| --- | --- | --- |
| [#4530 Windows unit tests CI](https://github.com/rusefi/rusefi/issues/4530) | Ready to close | [build-unit-tests-windows.yaml](../.github/workflows/build-unit-tests-windows.yaml) supplies Windows host CI. |
| [#5562 EPIC: testability: bench test commands should have automated coverage using simulator](https://github.com/rusefi/rusefi/issues/5562) | Representative coverage exists | [test_bench_test.cpp](../unit_tests/tests/test_bench_test.cpp) and simulator functional pin tests cover bench commands. Close the broad absence-of-coverage claim or list the remaining commands. |
| [#1811 unit test for start/stop hardware and pin repository](https://github.com/rusefi/rusefi/issues/1811) | Original smoke reproduction exists | [test_hardware_reinit.cpp](../unit_tests/tests/test_hardware_reinit.cpp) exercises repeated stop/start configuration. Retain a narrower follow-up only for missing ownership/state assertions. |

## Closed coverage requests

These tickets are closed as completed and are excluded from all remaining-work counts. Their matching tests are present in the master baseline; this report refresh inspected the source without rerunning them.

| Closed issue | Coverage present | Evidence |
| --- | --- | --- |
| [#7184 Target AFR blend feature test coverage](https://github.com/rusefi/rusefi/issues/7184) | `TargetAfrBlend` exercises the real target-Lambda and cycle-fuel paths: interpolation/clamping, signed adders, independent inputs, precision, invalid-input recovery, load overrides and published AFR/Lambda targets. | [test_fuel_computer.cpp](../unit_tests/tests/ignition_injection/test_fuel_computer.cpp); [coverage commit](https://github.com/rusefi/rusefi/commit/e6cdabcef2dad97d52a06e6dbbb488c5d81f15d4). |
| [#3329 We need coverage for 3328](https://github.com/rusefi/rusefi/issues/3329) | `SensorInit.MapMpx4250Calibration` checks three known MPX4250A voltage/pressure points through the configured slow sensor, combined MAP fallback and fast averaging converter. | [test_sensor_init.cpp](../unit_tests/tests/sensor/test_sensor_init.cpp); [coverage commit](https://github.com/rusefi/rusefi/commit/19d147f0ded8dc6b4e2d4ba0ab22b31d5183e138). |
| [#4258 unit test around fix closed loop fueling if post cranking fuel taper not enabled #4254](https://github.com/rusefi/rusefi/issues/4258) | `ClosedLoopFuel.StartupDelayWithNeutralPostCrankingEnrichment`, `StartupDelayWithZeroPostCrankingEnrichment` and `StartupDelayWithActivePostCrankingEnrichment` exercise the real elapsed-cranking clock and STFT gate. They cover delay boundaries, both trim banks and resetting the delay after restart. | [test_stft.cpp](../unit_tests/tests/test_stft.cpp); [coverage commit](https://github.com/rusefi/rusefi/commit/cd0baa77ae8dfae395c8ae1b386f1a83c5aa24b7). |

## Related tickets with a broader scope

These are deliberately excluded from the strict coverage-only count. Existing tests can be the starting point, but their requested outcome also needs implementation, infrastructure, documentation or investigation.

| Issue | Classification | Reason |
| --- | --- | --- |
| [#8101 idle ignition advance while coasting: real test and tooltop](https://github.com/rusefi/rusefi/issues/8101) | Mixed documentation and real testing | The issue asks for both coasting-ignition logs and an improved tooltip. Split the documentation task from the physical validation. |
| [#4669 Un-ignore tests after gradle migration](https://github.com/rusefi/rusefi/issues/4669) | Test maintenance | An ignored `LiveDataColorTest` still refers to this issue. Audit disabled tests and restore meaningful ones; failures may require fixes or seam changes. |
| [#2656 More low-level firmware coverage for the simulator/unit-tests](https://github.com/rusefi/rusefi/issues/2656) | Test infrastructure epic | Peripheral/engine emulation and a simulated vehicle feedback loop require implementation, not just additional assertions. |
| [#8807 unit test / sandbox to replay logs](https://github.com/rusefi/rusefi/issues/8807) | New replay tool | A tune/log replay application is new tooling. It may enable later coverage but is not itself a test-only ticket. |
| [#7035 bare minimum nucleo H7 HW CI ](https://github.com/rusefi/rusefi/issues/7035) | Hardware CI platform bringup | Provisioning a Nucleo H7 runner and working board support is broader than adding coverage to an existing test target. |
| [#3265 HW CI on Hellen](https://github.com/rusefi/rusefi/issues/3265) | Hardware CI platform bringup | Requires a Hellen board, host/runner ownership and wiring. It is not a self-contained test-only code change. |
| [#494 ETB quality instrumentation ](https://github.com/rusefi/rusefi/issues/494) | Instrumentation and test-suite design | ETB quality comparisons require metrics, a plant and a suite before individual coverage cases can be written. |
| [#313 HIGH: endurance test suite](https://github.com/rusefi/rusefi/issues/313) | Endurance test infrastructure | Define the duration, observables and failure criteria for long-running tests. This is an open-ended validation program. |
| [#9583 unit test technical debt: unitTestBusyWaitHack follow-up](https://github.com/rusefi/rusefi/issues/9583) | Framework design | Review/removal of `unitTestBusyWaitHack` is a framework change, not a missing behavior assertion. |
| [#7245 technical debt: NT -> US -> NT in unit test scheduler](https://github.com/rusefi/rusefi/issues/7245) | Scheduler semantics change | Removing NT/US rounding changes the test executor’s time model and may alter many expectations. |
| [#9349 FW and unit test incremental compilation to co-exist](https://github.com/rusefi/rusefi/issues/9349) | Build isolation defect | Firmware and host incremental build coexistence is build-system behavior. |
| [#6963 Include folders into tests targets in makefile](https://github.com/rusefi/rusefi/issues/6963) | Build rule defect | Duplicate test basenames require unique object/dependency paths; adding a test alone does not fix that. |
| [#7851 sd card logging unit test hack is broken](https://github.com/rusefi/rusefi/issues/7851) | Logging harness defect | The existing SD logging test mechanism is broken; repair its implementation before calling new assertions coverage-only. |
| [#2963 unit test logicdata does not work with 8 channels](https://github.com/rusefi/rusefi/issues/2963) | Artifact writer defect | Eight-channel logicdata serialization is a producer-format bug, even though a test produces the file. |
| [#6214 Unit tests sometimes? always? hang on Linux](https://github.com/rusefi/rusefi/issues/6214) | Sanitizer/environment investigation | The old Linux/ASan startup failure has an environment matrix and an open reproduction question; it is not a coverage request. |
| [#5233 TEST(etb, intermittentTps) local WSL and GHA runs produce different results](https://github.com/rusefi/rusefi/issues/5233) | Test isolation or environment defect | Local versus sharded ETB results need a deterministic reproduction and investigation. |
| [#4122 remote JNI unit test from java console build](https://github.com/rusefi/rusefi/issues/4122) | Build/test placement | Moving JNI tests between build entry points changes CI/build orchestration. |
| [#4016 java jobs depend on unit tests but not on gen_config](https://github.com/rusefi/rusefi/issues/4016) | Generated dependency ordering | Java workflows depending on tests before config generation is a build dependency issue. |
| [#7507 compare cygwin or mingw unit test incremental compilation against same on WSL](https://github.com/rusefi/rusefi/issues/7507) | Build performance experiment | Comparing native Windows and WSL incremental linking is benchmarking, not behavior test coverage. |

## How to execute the software list

Use the smallest existing fixture and exercise the real producer or consumer being protected. In particular, a mocked MAP value does not cover ADC conversion, a mocked target-Lambda lookup does not cover blending, and directly setting the elapsed-cranking clock does not cover its update path.

Run the relevant C++ filter through `unit_tests/test.sh` or the relevant Java module tests. Keep host code portable across Linux, macOS, MSVC and MinGW, and use headless Swing-compatible seams for Java CI. For a test-only change that passes, stop there; if it exposes a bug, preserve the reproduction before changing behavior.

No test execution or physical validation was performed for this inventory. See [closure candidates](open-issue-closure-review.md) and [small LLM tasks](llm-low-hanging-fruit-issues.md).
