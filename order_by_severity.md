# ECU firmware findings ordered by severity

This report ranks defects that remain in the current firmware. Fixed findings
are tracked separately below; the active reproductions assert current defects.

## Ranking rules

Order findings by credible engine safety and driveability consequence, then by
exposure and likelihood. Severity and confidence are separate judgments.

- **S1 — Critical:** credible unintended torque, loss of essential torque
  control, or immediate severe engine damage during plausible operation.
- **S2 — High:** failure of configured engine protection, damaging combustion
  or boost behavior, or unexpected engine shutdown/loss of propulsion.
- **S3 — Moderate:** repeatable starting, stalling, hesitation, misfire, or
  significant fueling/control error under narrower conditions.
- **S4 — Low:** limited driveability degradation or misleading diagnostics
  without an established direct control consequence.

Confidence labels: **reproduced** (focused execution demonstrates the defect),
**source-confirmed** (complete reachable source path establishes it), and
**candidate** (important premise still requires validation). Host execution
does not establish hardware timing or the physical severity of an outcome.

## Active ranked findings

| Rank | ID | Severity | Confidence | Finding |
| --- | --- | --- | --- | --- |
| 1 | ECU-024 | S2 — High | Reproduced | Ignition schedule rebuild can leave the previously charged coil energized |
| 2 | ECU-015 | S2 — High | Reproduced | A 1% traction spark cut becomes 100% hard cut or 50% soft cut |
| 3 | ECU-001 | S2 — High | Reproduced | Invalid wastegate/idle-valve controller outputs still drive the motor |
| 4 | ECU-006 | S2 — High | Reproduced | Injector duty protection ignores saturation of secondary injectors |
| 5 | ECU-017 | S2 — High | Reproduced | Primary injector duty protection ignores actual trims, wall film and cranking mode |
| 6 | ECU-011 | S2 — High | Reproduced | Shared-coil ignition ignores cylinder timing trims and distributor odd-fire offsets |
| 7 | ECU-012 | S2 — High | Source-confirmed; identity mismatch reproduced | Software knock uses firing-order position as physical cylinder identity |
| 8 | ECU-020 | S2 — High | Reproduced | VVT keeps actuating with absent or indefinitely stale cam feedback |
| 9 | ECU-004 | S2 — High | Reproduced | Invalid ethanol readings determine running-fuel stoichiometry |
| 10 | ECU-005 | S2 — High | Reproduced | Invalid fuel pressure selects injection duration, mass correction and deadtime |
| 11 | ECU-002 | S2 — High | Reproduced | Zero protection delays cause fuel cuts with healthy inputs, including disabled lambda protection |
| 12 | ECU-014 | S2 — High | Reproduced | Invalid oil-pressure payload can assert or release a running fuel cut |
| 13 | ECU-009 | S2 — High | Source-confirmed; reachability reproduced | Ninth/tenth forward gear indexes beyond the boost duty adder array |
| 14 | ECU-027 | S2 — High | Source-confirmed; arithmetic reproduced | Zero HPFP lobes leave a configured GDI pump active with a nonfinite fuel request |
| 15 | ECU-032 | S2 — High | Source-confirmed | Delayed software-knock window can be attributed to the next cylinder |
| 16 | ECU-023 | S2 — High | Reproduced | Invalid flat-shift table input selects arbitrary spark cut and timing |
| 17 | ECU-033 | S3 — Moderate | Arithmetic reproduced; scheduler path source-confirmed | Zero injector flow is accepted and yields an infinite pulse duration |
| 18 | ECU-028 | S3 — Moderate | Reproduced | Kick-start cranking fires coils despite Lua spark cut or required phase sync |
| 19 | ECU-008 | S3 — Moderate | Reproduced | Maximum oil-pressure timeout may already be expired at engine start |
| 20 | ECU-013 | S3 — Moderate | Reproduced | Knock response treats retarded timing as hundreds of degrees of advance |
| 21 | ECU-019 | S3 — Moderate | Reproduced | Percentage acceleration enrichment shrinks with batch/simultaneous injection |
| 22 | ECU-022 | S3 — Moderate | Reproduced | TPS acceleration enrichment appears without a new throttle step after a stop |
| 23 | ECU-026 | S3 — Moderate | Reproduced | Predictive MAP substitutes the previous run's estimate after a quick restart |
| 24 | ECU-021 | S3 — Moderate | Reproduced | VVT activation delay discards fractional seconds |
| 25 | ECU-007 | S3 — Moderate | Reproduced | DFCO ignition recovery uses a fixed half-second ramp |
| 26 | ECU-029 | S3 — Moderate | Reproduced | Failed A/C pressure feedback can re-enable a compressor after a pressure cut |

### ECU-024 — Ignition schedule rebuild can orphan a charged coil

- **Source:** `mainTriggerCallback()` in
  [main_trigger_callback.cpp](firmware/controllers/engine_cycle/main_trigger_callback.cpp)
  clears `ignitionEvents.isReady` after a trigger-configuration change.
  `onTriggerEventSparkLogic()` / `initializeIgnitionActions()` then rewrite
  `IgnitionEvent::outputs` in
  [spark_logic.cpp](firmware/controllers/engine_cycle/spark_logic.cpp). The
  already queued `fireSparkAndPrepareNextSchedule()` callback reads that same
  mutable array. `Engine::efiWatchdog()` in
  [engine.cpp](firmware/controllers/algo/engine.cpp) does not call `stopPins()`
  while the engine is still producing trigger edges.
- **Trigger:** A configuration change rebuilds the trigger/ignition schedule
  during dwell and changes the event's coil mapping, for example from two-coil
  to single-coil ignition. The pending fire callback retains the event pointer.
- **Actual:** The old coil remains high after its scheduled fire time. The
  callback lowers the newly mapped coil instead. Continued trigger activity
  prevents the stopped-engine watchdog from clearing the old output.
- **Expected:** A fire action must lower every output charged by its matching
  dwell action, even if the schedule is rebuilt before fire time.
- **Impact:** Sustained coil energization can overheat or damage a coil/driver
  and removes sparks from that output. Exposure requires a live schedule rebuild
  during a narrow dwell window and a changed coil mapping; host testing does not
  quantify electrical damage or reproduce a full TunerStudio configuration burn.
- **Validation:** `RebuildingIgnitionScheduleRedirectsPendingCoilShutoff`
  charges the second output in two-coil mode, queues a real scheduler fire
  action, models the `mainTriggerCallback()` invalidation, and rebuilds the
  same event in single-coil mode through `onTriggerEventSparkLogic()`. When the
  scheduler fires three milliseconds later, coil zero is low while the
  originally charged output remains high. The test lowers it for cleanup.
- **Fix direction:** Preserve the charged output identity until its scheduled
  discharge, or explicitly discharge all active outputs before remapping;
  cover live configuration and phase/synchronization transitions with a test
  that exercises the full trigger path.

### ECU-015 — Traction spark-cut percent is consumed as a fraction

- **Source:** `getRunningAdvance()` in
  [ignition_state.cpp](firmware/controllers/algo/ignition/ignition_state.cpp),
  `EngineState::updateSparkSkip()` in
  [engine2.cpp](firmware/controllers/algo/engine2.cpp), and
  `SoftSparkLimiter::shouldSkip()` in
  [launch_control.cpp](firmware/controllers/algo/launch_control.cpp).
  `tractionControlIgnitionSkip` is explicitly a 0–100 percent table in the
  configuration definition and tuning UI.
- **Trigger:** Traction's spark-cut table returns a nonzero percentage. At an
  interpolated value of 1% or greater, the behavior is already saturated.
- **Actual:** The raw percentage is added directly to a limiter that expects
  a 0–1 fraction; no division by 100 occurs. With `useHardSkipInTraction`, a
  table value of 1 skips every spark. In soft mode, the consecutive-skip guard
  produces alternating skips, i.e. 50%, instead of 1%.
- **Expected:** Convert the percentage to a bounded fraction before combining
  it with other spark-cut requests.
- **Impact:** A mild traction intervention can abruptly remove all combustion
  torque or produce severe alternating misfire. Spark cuts do not themselves
  remove the corresponding fuel request, adding unburned-fuel consequences.
- **Validation:** `OnePercentTractionHardCutSkipsEverySpark` and
  `OnePercentTractionSoftCutSkipsHalfTheSparks` use a 1% table, real running
  timing math and real spark limiters at 2,000 RPM. Of 100 decisions, all 100
  are cut in hard mode and 50 in soft mode. These are deterministic saturation
  cases, not statistical assertions about a random 1% sample.
- **Fix direction:** Convert units at the table-to-limiter boundary, keep
  telemetry units explicit, and test low/interpolated and full-scale requests
  plus combinations with launch, shift and Lua cuts. Existing target-ratio
  tests assert addition of the raw table value and therefore miss this defect.

### ECU-001 — Invalid wastegate/idle-valve outputs still drive the motor

- **Source:** `EtbController::setOutput()` in
  [electronic_throttle.cpp](firmware/controllers/actuators/electronic_throttle.cpp),
  and `ClosedLoopController::getOutput()` in
  [closed_loop_controller.h](firmware/controllers/closed_loop_controller.h).
- **Trigger:** DC function is `DC_Wastegate` or `DC_IdleValve`, and the position
  sensor fails, times out, or is unavailable. The generic closed-loop pipeline
  returns `unexpected` when observation fails.
- **Actual:** `setOutput()` takes the non-ETB branch and unconditionally sets
  `isEnabled = true`, skipping its later `!outputValue` check. It calls
  `enable()` and then `set()` using `outputValue.Value` even though that result
  is invalid. `expected<T>` stores an error code and value in the same union;
  a real invalid result does not contain a valid duty command.
- **Expected:** Invalid control results select a defined safe output for each
  actuator; they must never be read as a motor command.
- **Impact:** Position feedback loss can leave wastegate/idle motor control
  energized with an undefined command. Wastegate position can affect boost;
  idle-valve position can cause stalls or excessive idle airflow. Actual motor
  direction and engine consequences depend on hardware and payload bytes.
- **Validation:** `EcuLogicReview.WastegateDrivesMotorFromInvalidOutput` and
  `IdleValveDrivesMotorFromInvalidOutput` pass, explicitly demonstrating that
  an invalid result with a deterministically initialized payload commands 50%
  duty. They establish the missing validity check, not a claim that a real
  sensor failure always commands 50%.
  `WastegateSensorTimeoutStillEnablesMotor` and
  `IdlePositionSensorTimeoutStillEnablesMotor` additionally run real stored
  sensor timeout and controller `update()` paths. The pipeline produces an
  invalid result after timeout, yet the motor is enabled and commanded. A test
  observer initializes only the invalid payload before the output method to
  keep this deterministic without reading inactive/uninitialized union storage.
- **Fix direction:** Validate the result before selecting an actuator mode;
  define the failure action for EWG/idle and test real sensor timeout/recovery
  through `update()` in addition to the output-level regression.

### ECU-006 — Secondary injector saturation bypasses duty protection

- **Source:** The injector duty block in
  [limp_manager.cpp](firmware/controllers/limp_manager.cpp) calls only
  `getInjectorDutyCycle()`. Stage-two duration and duty are computed separately
  in [engine2.cpp](firmware/controllers/algo/engine2.cpp) and
  [fuel_math.cpp](firmware/controllers/algo/fuel_math.cpp).
- **Trigger:** Staging allocates enough mass to the secondary injectors that
  their pulse exceeds the configured duty limit while primary duty remains low.
  The supported 90% maximum staging fraction is sufficient.
- **Actual:** Neither the instant nor sustained limit inspects secondary duty.
  Fuel remains allowed even above 100% secondary duty. The same pulse duration
  on the primary immediately causes the configured protection cut.
- **Expected:** Duty protection must consider every active injector stage,
  using its actual delivery capacity and applicable injection mode.
- **Impact:** The demanded mass can exceed secondary delivery capacity without
  the intended cut, creating a credible lean-combustion/engine-damage risk under
  load. The host test establishes the protection omission, not mixture or damage.
- **Validation:** `SecondaryInjectorOverDutyDoesNotTriggerProtection` uses real
  injector models with secondary flow twice primary flow, 90% staging, 1 ms
  deadtime and 3,000 RPM. Primary duty is 27.5%, secondary duty 115%, with
  95% instant / 85% sustained limits. Injection remains allowed initially and
  after one second (the sustained timeout is 0.5 s).
- **Fix direction:** Apply instant/sustained checks and recovery hysteresis to
  the maximum relevant stage duty; review secondary timing-lock behavior too.

### ECU-017 — Actual primary duty differs from the protected estimate

- **Source:** `EngineState::periodicFastCallback()` in
  [engine2.cpp](firmware/controllers/algo/engine2.cpp), duty calculations in
  [fuel_math.cpp](firmware/controllers/algo/fuel_math.cpp), protection in
  [limp_manager.cpp](firmware/controllers/limp_manager.cpp), and pulse generation in
  [main_trigger_callback.cpp](firmware/controllers/engine_cycle/main_trigger_callback.cpp).
- **Trigger:** Positive cylinder/bank/knock fuel trims or wall-film compensation
  raise the actual pulse above its pre-trim duration. Supported cylinder trims
  alone are sufficient; staging is not required. Separately, cranking and
  running use different injection modes.
- **Actual:** The nominal duration is calculated before cylinder/bank/knock trims
  and wall-film adjustment. Both duty-protection limits and the 90% injection
  timing-lock threshold inspect this nominal value. The scheduler independently
  calculates the longer adjusted pulse, without a corresponding duty check.
- **Expected:** Protect the capacity of the actual injector commands, including
  relevant trims and transient compensation; the timing-lock decision must also
  reflect pulses that can overlap a cycle.
- **Impact:** A configured injector-capacity protection can fail while primary
  injectors saturate, creating a credible underdelivery/lean-combustion risk
  under load. Updating injection phase at saturation can additionally miss
  pulses. Host tests establish excessive commanded duty and missing protection,
  not physical mixture, missed pulses or damage. This differs from ECU-006,
  which omits the secondary stage even for its nominal duration.
- **Validation:** `CylinderFuelTrimSaturatesInjectorWithoutDutyProtection` uses
  3,000 RPM, a 32.8 ms nominal pulse (82% duty), and an allowed +25% cylinder
  trim. The real scheduler commands 41 ms (102.5% duty) while 95% instant / 85%
  sustained protection permits fuel, including after one second. The timing
  lock also stays disengaged. `WallFuelSaturatesInjectorWithoutDutyProtection`
  uses the real wall controller with tau 0.5 s / beta 0.5: a nominal 24 ms / 60%
  pulse becomes 48 ms / 120% on an initially empty film, again without a cut.
  Both tests execute open/close callbacks and verify cleanup.
- **Fix direction:** Separate nominal scheduling estimates from actual per-output
  capacity accounting. Include both stages, injection mode, trim and wall-film
  effects, and define protection timing for one-pulse transients and hysteresis.
- **Cranking-mode subcase:** `getInjectorDutyCycle()` and its stage-two sibling
  multiply by the configured running mode's pulse count even while cranking
  uses a different mode. `CrankingBatchDutyIsUnderreportedAsRunningSequential`
  uses 600 RPM, an 800-RPM cranking threshold and 110 ms batch pulses. Actual
  requested duty is 110%, but the estimate is 55% and both protection limits
  permit injection, including after the sustained timeout. Conversely,
  `CrankingSequentialDutyIsOverreportedAsRunningSimultaneous` uses 50 ms pulses
  at 600 RPM: real sequential duty is 25%, but the running simultaneous mode
  makes the estimate 100%, triggering an instant fuel cut and a credible
  no-start. These two tests exercise mode selection and real limp decisions;
  they do not execute the corresponding injector pulses.

### ECU-011 — Shared-coil ignition uses the wrong cylinder's timing and offset

- **Source:** `prepareCylinderIgnitionSchedule()` in
  [spark_logic.cpp](firmware/controllers/engine_cycle/spark_logic.cpp) maps the
  event to a coil before selecting `timingAdvance[coilIndex]`.
  [engine2.cpp](firmware/controllers/algo/engine2.cpp) computes that array by
  physical cylinder, including each cylinder's trim.
- **Trigger:** Distributor (`IM_ONE_COIL`) or even-fire wasted ignition with
  different per-cylinder timing trims; distributor ignition on an odd-fire
  engine with nonzero per-cylinder timing offsets.
- **Actual:** Every distributor event selects the first cylinder's timing,
  although the computed per-cylinder timing array contains the requested trims.
  Wasted ignition uses the paired cylinder's timing for half the cycle. The same
  coil-derived index selects odd-fire geometry adjustments, so distributor
  events also ignore offsets for cylinders other than the first.
- **Expected:** Select combustion timing by the firing cylinder, independently
  of which physical ignition output serves it.
- **Impact:** A cylinder-specific safety retard can silently be ignored, leaving
  that cylinder more advanced than calibrated and increasing knock risk under
  susceptible conditions. Trims in the opposite direction can reduce torque.
  Equal cylinder trims mask that subcase. Ignoring odd-fire geometry can place
  ignition tens of crank degrees from its intended position, causing severe
  misfire/torque loss and potentially damaging combustion.
- **Validation:** `DistributorUsesFirstCylinderTimingTrimForEveryCylinder`
  runs the real fast-callback timing math and ignition schedule preparation.
  Cylinder 3 is computed at 11 degrees BTDC (15 base minus 4 trim), but its
  scheduled spark is 165 degrees in the cycle, i.e. 15 BTDC at its 180-degree
  firing position. The scheduled diagnostic also reports the wrong 15 degrees.
  `WastedSparkUsesPairedCylinderTimingTrim` shows a requested 9 BTDC on cylinder
  4 scheduled as 15 BTDC. `DistributorIgnoresOddFireCylinderOffset` uses a
  six-cylinder 90/150-degree firing pattern: cylinder 2's spark is scheduled at
  110 cycle degrees instead of 80, thirty degrees late.
- **Fix direction:** Separate firing-cylinder identity from coil identity for
  timing, odd-fire offsets and knock attribution; test all shared-coil modes.

### ECU-012 — Knock sensing confuses firing order with physical cylinder

- **Source:** `fireSparkAndPrepareNextSchedule()` in
  [spark_logic.cpp](firmware/controllers/engine_cycle/spark_logic.cpp),
  `Engine::onSparkFireKnockSense()` / `getCylinderKnockBank()` in
  [knock_controller.cpp](firmware/controllers/engine_cycle/knock_controller.cpp),
  and acquisition/result handling in
  [software_knock.cpp](firmware/controllers/sensors/impl/software_knock.cpp).
- **Trigger:** Software knock is enabled and firing order differs from physical
  cylinder numbering, with cylinder-specific sensor channels or gain settings.
- **Actual:** Ignition passes `event->cylinderIndex`, the position in firing
  order, directly into knock sampling. Sampling treats it as a physical cylinder:
  it selects `knockBankCylN`, labels the acquired result, applies that cylinder's
  gain, and uses its timing for retard calculation. No firing-order conversion
  occurs along the path.
- **Expected:** Sampling channel, gain, result identity and timing must refer
  to the cylinder whose combustion event is being measured.
- **Impact:** The wrong bank's sensor/gain can miss knock or generate false
  knock response. Per-cylinder diagnostics are mislabeled. Global retard does
  not correct the wrong acquisition channel or threshold/gain selection.
- **Validation:** `KnockIdentityUsesFiringOrderPositionInsteadOfCylinderNumber`
  initializes the real ignition schedule for order 1-3-4-2 and proves that the
  event for physical cylinder 3 supplies identity 1 (physical cylinder 2),
  selecting a different configured knock bank. The complete caller/consumer
  path is confirmed in source. The standard host build disables
  `EFI_SOFTWARE_KNOCK`, so ADC scheduling/acquisition is not executed here.
- **Fix direction:** Convert event order to physical cylinder before scheduling
  knock acquisition and retain that identity in each pending sample. Cover
  nontrivial firing orders, both banks and differing per-cylinder gains.

### ECU-020 — VVT control has no valid/fresh cam-feedback gate

- **Source:** `TriggerCentral::getVVTPosition()` / `handleVvtCamSignal()` in
  [trigger_central.cpp](firmware/controllers/trigger/trigger_central.cpp),
  `VvtController::observePlant()` / `onFastCallback()` in
  [vvt.cpp](firmware/controllers/actuators/vvt.cpp), and the generic pipeline in
  [closed_loop_controller.h](firmware/controllers/closed_loop_controller.h).
- **Trigger:** VVT's RPM/temperature/delay gates permit control, but a cam has
  never supplied a usable measurement or its signal disappears while crank
  pulses continue. Both basic and advanced control consume the same feedback.
- **Actual:** The cam angle is a zero-initialized float, returned as a valid
  `expected` without a measurement-valid flag, timestamp or timeout. After
  acquisition it retains the last angle indefinitely. Missing-cam diagnostics
  do not invalidate this value or inhibit the VVT controller. The PID continues
  driving the solenoid against fictitious/stale feedback.
- **Expected:** Enable closed-loop cam control only with a usable measurement
  for that cam. Loss of feedback must select a defined safe output and reset or
  suspend integration until acquisition is restored.
- **Impact:** Open-ended solenoid actuation after cam-signal loss can move the
  actual cam away from its intended target, with poor combustion, torque loss,
  stalls and potentially damaging timing depending on the engine's mechanical
  constraints. The host tests establish continued PWM commands; they do not
  establish actual cam travel, valve contact or damage.
- **Validation:** `VvtActuatesWithoutAnyCamMeasurement` configures a cam input,
  starts at 1,500 RPM and receives no cam edges. The zero angle is valid and the
  real controller commands 20% PWM. `VvtKeepsDrivingAfterCamPulsesStop` acquires
  a 71-degree measurement through actual crank/cam callbacks at 1,200 RPM,
  then continues crank pulses for 30 seconds with no more cam edges. Feedback
  remains valid at 71 degrees and the real PWM path still commands 20% for a
  target 20 degrees above the stale measurement.
- **Fix direction:** Track per-cam acquisition and freshness, not only global
  engine synchronization. Cover first acquisition, separate-bank dropout,
  missing-cam diagnostics, stop/restart and re-acquisition under both modes.

### ECU-004 — Invalid ethanol readings determine running-fuel stoichiometry

- **Source:** `FuelComputer::getStoichiometricRatio()` in
  [fuel_computer.cpp](firmware/controllers/algo/fuel/fuel_computer.cpp),
  [flex_sensor.h](firmware/controllers/sensors/flex_sensor.h), and
  [stored_value_sensor.h](firmware/controllers/sensors/core/stored_value_sensor.h).
- **Trigger:** A configured flex-fuel sensor times out or reports an invalid
  frequency. Registration remains present; `hasSensor()` does not mean the
  current reading is valid.
- **Actual:** After checking presence, the fuel computer interpolates between
  the primary and secondary stoichiometric ratios using `flex.Value` without
  testing `flex.Valid`. A failed reading contains an error code, not a usable
  ethanol value. `getCycleFuel()` uses this ratio to compute requested fuel mass.
- **Expected:** A defined failed-flex policy must select a usable fuel
  composition or protection response. Invalid payload bytes cannot select it.
- **Impact:** Sensor failure can change the base fuel request unpredictably.
  An inappropriate gasoline-like ratio on ethanol fuel can underfuel the engine;
  a rich result can degrade driveability. The direction/size on real hardware
  depends on the invalid payload and calibration, and is not established here.
- **Validation:** `InvalidEthanolPayloadControlsFuelStoichiometry` registers a
  sensor that always reports invalid, with initialized payloads solely to make
  misuse deterministic. With configured ratios 15 and 10, changing the unusable
  payload changes the returned ratio between both extremes. No protection
  response occurs in this function. Real flex timeout/error paths are traced
  in source; a hardware failure is not simulated.
- **Fix direction:** Define a consistent failed-flex fallback/last-known-value
  policy and apply it to running fueling and any composition-based adjustments.
  Existing cranking/priming code already explicitly uses a 50% fallback, whereas
  running stoichiometry currently has none.

### ECU-005 — Invalid fuel pressure controls injection duration and deadtime

- **Source:** `InjectorModelWithConfig::getInjectionDuration()`, `updateState()`,
  `getFuelPressure()`, `getFuelDifferentialPressure()`, and `getDeadtime()` in
  [injector_model.cpp](firmware/controllers/algo/fuel/injector_model.cpp);
  production `Engine::updateSlowSensors()` in
  [engine.cpp](firmware/controllers/algo/engine.cpp).
- **Trigger:** Differential pressure is unavailable, including `ICM_None` /
  manual-compensation modes (the switch returns `unexpected`), or a sensed-rail
  mode with failed/missing fuel pressure or required MAP input.
  Additionally, the direct flow-linearization table or HPFP manual compensation
  is enabled and its registered fuel-pressure sensor fails or times out.
- **Actual:** `updateState()` unconditionally reads the invalid result's
  `.Value` into `pressureCorrectionReference`. `getDeadtime()` uses that value
  as the pressure axis of the lag table. In the legacy absolute-pressure mode,
  `getFuelPressure()` adds MAP to that invalid payload and wraps the result as
  **valid**, losing the failure state.
  The direct linearization path uses raw `FuelPressureInjector.Value` to select
  the entire pulse duration. Manual HPFP compensation checks sensor presence
  but then uses raw `FuelPressureHigh.Value` to scale commanded fuel mass.
- **Expected:** Use a defined reference pressure/failure fallback for lag
  correction, and preserve failure validity when computing absolute pressure.
- **Impact:** Full pulse duration or commanded mass can change based on an
  invalid result, with credible substantial under/overfueling and damaging
  combustion risk in these optional pressure-based modes. Pressure-dependent
  deadtime also changes without a valid pressure input. Exposure includes normal operation
  with compensation disabled if the lag table varies by pressure. A table with
  identical pressure rows masks the numerical effect. Actual invalid bytes and
  resultant pulse-width error are not established by host tests.
- **Validation:** `InvalidDifferentialPressureBecomesValidAbsolutePressure`
  and `InvalidDifferentialPressureControlsDeadtimeAxis` override the pressure
  provider with initialized invalid payloads. They demonstrate lost validity
  and direct selection of the deadtime axis by unusable data. The periodic
  caller is production-only, so normal host fast/slow callback tests do not
  automatically exercise this update; these reproductions call it explicitly.
  `InvalidFuelPressureSelectsLinearizedInjectionDuration` requests the same
  10 mg mass and gets 5 ms versus 1 ms solely from changing an invalid payload.
  `InvalidHighPressureSelectsManualFuelMassCorrection` similarly gets 11 ms
  versus 21 ms. Both use real model/table calculations and registered sensors;
  the initialized invalid payloads establish misuse without predicting real
  error-storage bytes.
- **Fix direction:** Resolve valid pressure or a defined fallback before
  updating the lag axis; propagate invalid state from `getFuelPressure()`.
  Apply an explicit failure policy before the direct-duration and manual-mass
  lookup paths too.
  Cover no-compensation mode, absent and timed-out pressure/MAP, both legacy
  axis modes, and a lag table whose rows differ.

### ECU-002 — Zero protection delays cause false fuel cuts

- **Source:** `LimpManager::updateState()` in
  [limp_manager.cpp](firmware/controllers/limp_manager.cpp),
  `LambdaMonitorBase::update()` in
  [lambda_monitor.cpp](firmware/controllers/math/lambda_monitor.cpp), and
  `Timer::hasElapsedUs()` in
  [timer.cpp](firmware/libfirmware/util/src/timer.cpp).
- **Trigger:** A relevant delay is zero, which the setting ranges in
  [rusefi_config.txt](firmware/integration/rusefi_config.txt) explicitly allow.
  The injector case requires duty at or above the 20% recovery threshold;
  the lambda case requires the operating point to be outside its restore
  conditions; oil checks require a running engine and their respective enable
  conditions.
- **Actual:** Healthy input resets a timer, then an unconditional timeout check
  sees `hasElapsedSec(0) == true` and requests a cut anyway:
  - `minimumOilPressureTimeout = 0` cuts with pressure above the minimum.
  - `maxOilPressureTimeout = 0` cuts with pressure below a configured maximum.
  - `maxInjectorDutySustainedTimeout = 0` cuts at 30% duty with limits of 85%
    sustained / 95% instant.
  - `lambdaProtectionTimeout = 0` latches a lambda cut even with
    `lambdaProtectionEnable = false` and healthy lambda.
- **Expected:** Zero delay means immediate reaction to an actual fault, and
  disabled lambda protection cannot request a new cut.
- **Impact:** A valid calibration can cause no-start/stall or abrupt loss of
  propulsion; the disabled lambda case is particularly misleading to a tuner.
  Default positive lambda/low-oil/injector delays avoid those subcases. The
  high-oil timeout has no explicit nonzero assignment in shared defaults, so
  enabling a maximum-pressure table deserves particular attention.
- **Validation:** Four zero-delay review tests exercise real
  monitor/limp logic. The oil tests also show the same healthy inputs allow
  injection with a positive 0.1-second delay.
- **Fix direction:** Gate each timeout by the actual fault predicate and
  protection enable state. Preserve existing latch/recovery behavior, and add
  healthy/faulty zero-delay and positive-delay boundary coverage separately.

### ECU-014 — Invalid oil pressure chooses whether running fuel is cut

- **Source:** The maximum-pressure branch in
  [limp_manager.cpp](firmware/controllers/limp_manager.cpp).
- **Trigger:** A maximum oil-pressure limit is enabled and the registered
  pressure sensor fails or times out while the engine is running.
- **Actual:** `hasSensor()` permits the branch, which compares `oilp.Value`
  without checking validity. Invalid storage controls timer reset and whether
  the maximum-pressure timeout cuts fuel.
- **Expected:** Apply a defined unavailable-sensor policy; invalid payload bytes
  cannot determine either pressure protection or recovery.
- **Impact:** A failed sensor can cause an arbitrary propulsion cut or suppress
  protection. This is separate from startup and zero-delay timer behavior.
- **Validation:** `InvalidOilPressurePayloadDeterminesProtectionCut` uses a
  registered invalid sensor, a 500 kPa maximum and positive 1-second timeout.
  An initialized invalid payload of 400 allows fuel; 600 followed by 1.1 s
  causes an oil-pressure cut; changing the still-invalid payload to 400
  releases it. These values expose the missing check, not real failure bytes.
- **Fix direction:** Check the result before comparison and make timer/latch
  behavior explicit for sensor loss, recovery, enable changes and engine stop.

### ECU-009 — Upper forward gears overrun the boost adder array

- **Source:** `BoostController::getOpenLoop()` in
  [boost_control.cpp](firmware/controllers/actuators/boost_control.cpp),
  [gear_detector.cpp](firmware/controllers/modules/gear_detector/gear_detector.cpp), and
  settings in [rusefi_config.txt](firmware/integration/rusefi_config.txt).
- **Trigger:** A configured ninth or tenth forward gear is detected and the
  boost open-loop calculation runs. `totalGearsCount` allows ten, and gear
  detection has ten ratio entries.
- **Actual:** `gearBasedOpenLoopBoostAdder` has ten entries, but boost indexes
  it by `gear + 1` to include reverse and neutral. Only forward gears 1–8 fit;
  ninth/tenth gear access indexes 10/11 without a bounds check.
- **Expected:** All accepted detected gears must have a valid adder or a
  defined fallback. Detector configuration and boost table capacity must agree.
- **Impact:** An out-of-bounds read enters wastegate duty calculations; behavior
  is undefined and can produce incorrect boost commands. Exposure is narrow
  (upper gears), and no specific numerical or physical outcome is asserted.
- **Validation:** `GearDetectorCanExceedBoostAdderForwardGearCapacity` exercises
  the real detector with ten supported ratios, RPM and output-shaft speed,
  proving it produces gear 10 while the boost table accommodates only eight
  forward gears. The source then establishes the unchecked read. The portable
  test deliberately stops before executing undefined behavior. Existing
  `BoostControl.BoostOpenLoopYAxis` also supplies gear 9, but its passing result
  does not establish bounds safety for the scaled-channel element access.
- **Fix direction:** Size the table for every supported gear including reverse
  and neutral, and validate/bound the sensor-to-index conversion.

### ECU-027 — Zero HPFP lobes do not disable a configured GDI pump

- **Source:** `validateGdi()` in
  [engine_controller.cpp](firmware/controllers/engine_controller.cpp) accepts
  zero `hpfpCamLobes`, and `HpfpLobe::findNextLobe()` in
  [high_pressure_fuel_pump.cpp](firmware/controllers/engine_cycle/high_pressure_fuel_pump.cpp)
  explicitly handles zero as an inactive lobe configuration. In production,
  however, `isGdiEngine()` uses the valve pin's initialized state, and
  `HpfpController::onFastCallback()` gates activity on that state, RPM and pump
  volume without checking `hpfpCamLobes`. `HpfpQuantity::calcFuelPercent()`
  divides positive requested fuel by the zero lobe count.
- **Trigger:** A GDI tune retains an assigned HPFP valve pin and nonzero pump
  volume while the lobe count is set to zero, for example during a live tuning
  change. Zero is within the exposed configuration range and passes validation.
- **Actual:** On the production path the controller can remain active. The
  fuel-per-lobe request becomes nonfinite; with default host tables, the
  downstream requested activation angle still exceeds the minimum, so the
  scheduler can continue commanding the valve despite the zero-lobe setting.
- **Expected:** Zero lobes must disable valve scheduling, or the configuration
  must be rejected before a running controller uses it.
- **Impact:** An unintended or malformed high-pressure pump command can disrupt
  rail pressure and GDI fueling, risking loss of propulsion or damaging
  combustion. The exact hardware pressure result is not established; this
  finding depends on a valve pin remaining assigned when lobes are zero.
- **Validation:** `ZeroHpfpLobesProduceNonfiniteFuelRequest` exercises the real
  configuration validator, quantity and angle calculations with nonzero fuel
  mass, zero lobes and pump volume 0.29 cc. Validation accepts the tune, and
  calculation produces a nonfinite fuel request and an activation angle
  above `hpfpMinAngle`. The host `isGdiEngine()` deliberately uses lobe count
  rather than the production pin-state check, so the host test does not execute
  the full active controller path; production gate reachability is source-traced.
- **Fix direction:** Make the production active condition require positive
  lobe count; reject zero divisors before fuel calculation and clear any
  queued valve actions during reconfiguration.

### ECU-032 — Delayed knock callbacks use the next cylinder's identity

- **Source:** `Engine::onSparkFireKnockSense()` in
  [knock_controller.cpp](firmware/controllers/engine_cycle/knock_controller.cpp)
  writes each spark's cylinder into one static `cylinderNumberCopy`, then
  schedules `startKnockSampling()` by
  `knockDetectionWindowStart`. That callback reads the same static variable
  for the sensor bank and for `onStartKnockSampling()`. The scheduler in
  [rpm_calculator.cpp](firmware/controllers/engine_cycle/rpm_calculator.cpp)
  applies the configured angle without capping it to a firing interval.
- **Trigger:** Software knock is enabled and the configured window begins
  after the next cylinder fires. On an evenly firing V12, consecutive sparks
  are 60 crank degrees apart; a 70-degree window is accepted by the tuning
  range of -1000 to 1000 degrees. V12 configurations are supported, including
  `PROTEUS_BMW_M73`.
- **Actual:** The next spark overwrites `cylinderNumberCopy` before the
  earlier window callback runs. Sampling selects the later cylinder's knock
  bank and credits its result to that later cylinder instead of the cylinder
  whose spark scheduled the window.
- **Expected:** Each delayed window must carry its own cylinder identity
  through the scheduled action, independent of later sparks.
- **Impact:** Sampling may use the wrong sensor bank, and the processed signal
  uses the wrong cylinder's gain, peak record and base timing when calculating
  the global retard amount. That can weaken or misdirect knock response under
  knock. The consequence depends on tune, engine layout, and actual knock;
  no hardware DSP capture or host reproduction was run for this path.
- **Validation:** Source path and accepted configuration range establish the
  overwrite. The default 20-degree start does not meet this condition on a
  V12. This remains source-confirmed, with a multi-cylinder timing test and
  actual software-knock build still needed.
- **Fix direction:** Bind cylinder and bank to each pending scheduler action,
  and exercise windows that cross one or more later sparks.

### ECU-023 — Invalid flat-shift axis controls ignition cuts and timing

- **Source:** `readGppwmChannel()` in
  [gppwm_channel_reader.cpp](firmware/controllers/actuators/gppwm_channel_reader.cpp),
  `ShiftTorqueReductionController::getSparkSkipRatio()` /
  `getTorqueReductionIgnitionRetard()` /
  `updateTimeConditionSatisfied()` in
  [shift_torque_reduction_controller.cpp](firmware/controllers/algo/shift_torque_reduction_controller.cpp),
  consumed in [engine2.cpp](firmware/controllers/algo/engine2.cpp) and
  [ignition_state.cpp](firmware/controllers/algo/ignition/ignition_state.cpp).
- **Trigger:** Flat-shift torque reduction is active and one of its optional
  GPPWM axes selects a sensor that fails or times out, such as TPS.
- **Actual:** The channel reader preserves invalidity, but all three flat-shift
  table lookups use the returned `.Value` without checking it. Unusable payload
  bytes can select an arbitrary spark-cut percentage, replacement timing and
  time limit. The hard spark limiter consumes the cut ratio; the ignition
  calculation uses the timing table directly while flat shift is active.
- **Expected:** An invalid axis must select an explicit fault policy, with no
  table lookup driven by invalid storage. The active torque intervention must
  transition predictably when input validity changes.
- **Impact:** A sensor fault during a shift can cause a much larger or smaller
  torque cut than calibrated, including full ignition cut, or a large timing
  change. A sudden full cut can cause loss of propulsion; altered timing can
  affect combustion. Physical outcome depends on calibration and actual error
  payload; the initialized host payloads do not predict hardware bytes.
- **Validation:** `InvalidFlatShiftAxisPayloadSelectsSparkCutAndTiming` registers
  a TPS sensor that always returns invalid. With an active shift and a 0–100%
  cut table, changing only the initialized unusable payload from 0 to 100
  changes the requested hard cut from 0% to 100% and the timing table output
  from +20 to -10 degrees. The consumer path is traced in production source;
  the test calls the two table consumers directly rather than scheduling sparks.
- **Fix direction:** Validate each selected axis before lookup, define whether
  a sensor loss releases or maintains the torque intervention, and test the
  time-axis path plus actual limiter/scheduler behavior.

### ECU-033 — Zero injector flow yields an infinite scheduled pulse width

- **Source:** The injector-flow field in
  [rusefi_config.txt](firmware/integration/rusefi_config.txt) accepts zero.
  `validateConfig()` in
  [engine_controller.cpp](firmware/controllers/engine_controller.cpp) does not
  reject it. `InjectorModelBase::getBaseDurationImpl()` in
  [injector_model.cpp](firmware/controllers/algo/fuel/injector_model.cpp)
  divides positive fuel mass by the prepared mass-flow rate. The scheduling
  path in
  [main_trigger_callback.cpp](firmware/controllers/engine_cycle/main_trigger_callback.cpp)
  rejects NaN and negative stage-one durations but not positive infinity,
  then converts that duration to an integer tick offset.
- **Trigger:** The primary injector flow is set to zero while normal
  mass-to-duration injector modeling is active and positive fuel mass is
  requested. A live tune change or malformed saved tune can reach this state;
  the default flow is nonzero.
- **Actual:** Validation succeeds and the model returns positive infinity
  for the pulse duration. The scheduler's finite-value gate does not reject
  it; a float-to-integer conversion of infinity has undefined C++ behavior.
- **Expected:** Reject zero flow for the active model or inhibit injection
  before a nonfinite pulse reaches scheduling.
- **Impact:** A zero-flow tune can produce unpredictable pulse scheduling and
  loss of normal fueling. The host test establishes validation and infinity,
  not the physical injector output or a particular target's integer-cast
  behavior.
- **Validation:** `ZeroInjectorFlowPassesValidationAndProducesInfinitePulse`
  calls startup validation, prepares the primary injector model with zero
  flow, and observes an infinite duration for positive mass.
- **Fix direction:** Validate positive effective flow for the selected
  injector mode and guard scheduled durations with `std::isfinite` before
  converting to ticks. Cover primary and secondary staging configurations.

### ECU-028 — Kick-start cranking bypasses ignition permission

- **Source:** `handleShaftSignal()` calls `handleKickStart()` in
  [trigger_central.cpp](firmware/controllers/trigger/trigger_central.cpp) after
  checking `allowTriggerInput()`, but `handleKickStart()` checks only its mode,
  `isIgnitionEnabled`, trigger edge, RPM and dwell before charging and firing
  both coils. `LimpManager::updateState()` in
  [limp_manager.cpp](firmware/controllers/limp_manager.cpp) can deny ignition
  for Lua cut or missing required phase synchronization without denying trigger
  input.
- **Trigger:** Kick-start cranking is enabled below 800 RPM while a Lua spark
  cut is active, or while `isPhaseSyncRequiredForIgnition` is set but full phase
  synchronization has not yet been achieved.
- **Actual:** Both coils charge and fire from the primary trigger edge even
  though `LimpManager::allowIgnition()` is false. Normal angle-based ignition
  obeys that permission; the direct kick-start path bypasses it.
- **Expected:** The direct cranking spark path must honor the same ignition
  inhibition decisions as normal scheduling, except any explicitly documented
  and narrowly defined kick-start override.
- **Impact:** A safety or control-requested spark cut can fail during cranking;
  pre-sync firing may cause a backfire or ignition when phase is explicitly
  required. Consequence depends on engine hardware and configuration. Host
  tests observe coil commands, not electrical discharge or combustion.
- **Validation:** `KickStartSparkBypassesLuaIgnitionCut` sets an actual Lua
  ignition cut, confirms the limp manager denies ignition, then drives a real
  primary trigger edge and observes both coils charge and fire.
  `KickStartSparkBypassesRequiredPhaseSync` uses a half-moon crank trigger and
  explicit phase requirement, confirms phase is unsynchronized and ignition is
  denied, then observes the same coil sequence.
- **Fix direction:** Gate kick-start charging on actual ignition permission and
  define ordering with `ClearReason::KickStart` so the special mode can still
  suppress only normal scheduling without suppressing its own permitted spark.

### ECU-008 — High oil-pressure delay is expired on the first running sample

- **Source:** Maximum-pressure and stopped-engine branches of
  [limp_manager.cpp](firmware/controllers/limp_manager.cpp).
- **Trigger:** A maximum oil-pressure table is enabled, pressure initially
  exceeds it as the engine starts, and a positive debounce timeout is configured.
- **Actual:** The stopped branch resets the low-pressure timer but not the
  high-pressure timer. The high-pressure timer begins in the expired state or
  retains elapsed stopped time. The first high running sample can cut fuel
  immediately instead of waiting for the configured duration.
- **Expected:** A new high-pressure episode should receive its selected delay;
  stopped time must not count as time spent above the running pressure limit.
- **Impact:** A brief startup pressure spike can produce a false cut, stumble
  or failed start. This is distinct from the zero-delay fault in ECU-002.
- **Validation:** `HighOilPressureTimeoutAlreadyExpiredAtEngineStart` waits
  10 seconds stopped with zero pressure, then starts at 1,000 RPM / 600 kPa
  against a 500 kPa maximum and a 1-second timeout. Fuel cuts immediately.
  After a healthy running sample resets the timer, the same high pressure
  allows fuel at 0.9 seconds and cuts after 1.1 seconds.
- **Fix direction:** Reset/rearm the high-pressure timer at the appropriate
  engine-state and enable transitions; define invalid-sensor handling separately.

### ECU-013 — Wrapped retarded timing produces excessive knock response

- **Source:** `KnockControllerBase::onKnockSenseCompleted()` in
  [knock_controller.cpp](firmware/controllers/engine_cycle/knock_controller.cpp)
  and the wrapped `timingAdvance[]` producer in
  [engine2.cpp](firmware/controllers/algo/engine2.cpp).
- **Trigger:** Knock is detected while commanded ignition timing is after TDC.
- **Actual:** The engine stores -10 degrees as 710 cycle degrees. Knock response
  subtracts its -20-degree reference directly from 710, calculating 730 degrees
  of margin instead of 10. Even mild aggression immediately reaches the maximum
  retard setting.
- **Expected:** Use a signed physical advance for the retard calculation and
  apply the intended aggression to the true distance from its reference.
- **Impact:** Excessive retard can cause an abrupt torque drop, hesitation,
  misfire and additional exhaust heating during already-retarded operation.
  The configured maximum retard still bounds the result.
- **Validation:** `KnockRetardTreatsWrappedNegativeTimingAsLargeAdvance` runs
  actual timing math for -10 degrees and a real knock controller. One knock
  with 10% aggression applies the entire 8-degree limit instead of 1 degree.
- **Fix direction:** Normalize the stored angle before calculating retard and
  test values around TDC and the minimum-timing reference, including two-stroke
  cycle semantics.

### ECU-019 — Percentage AE depends on how the cycle is split into injections

- **Source:** `getInjectionMass()` in
  [fuel_math.cpp](firmware/controllers/algo/fuel_math.cpp), with correction
  generation in [accel_enrichment.cpp](firmware/controllers/algo/accel_enrichment.cpp).
- **Trigger:** Percent Adder acceleration enrichment is selected with batch
  or simultaneous injection on a multi-cylinder engine.
- **Actual:** Base cycle mass is multiplied by the injection-mode duration
  multiplier `D`, then extra percentage mass is calculated as that already
  scaled mass times `D * E`. Splitting a cycle into two/four injections reduces
  the requested enrichment fraction by two/four instead of preserving it.
  The existing configuration help describes the current scaling; it does not
  remove the inconsistency of a fractional correction changing with pulse count.
- **Expected:** Apply a dimensionless enrichment fraction once to cycle or
  per-injection fuel mass, with pulse-count conversion applied once overall.
- **Impact:** A calibration transferred from sequential to batch/simultaneous
  injection can supply too little transient enrichment, causing lean tip-in,
  hesitation or stalling. Individually retuned AE tables can mask the fault;
  exposure is a transient driveability error, not established steady-load damage.
- **Validation:** `PercentAccelEnrichmentIsReducedByInjectionModeTwice` uses
  the real TPS event detector, a 0.20-fraction table, neutral corrections and
  finite fuel mass from a controlled air-mass input. The same 0-to-20 TPS step
  gives 20% extra fuel in sequential mode, 10% in batch and 5% in four-cylinder
  simultaneous mode, each compared with its own unenriched baseline.
- **Fix direction:** Preserve the fraction across pulse counts and explicitly
  plan compatibility for tunes that compensate for the present extra scaling.
  Do not change MS Adder's physical duration conversion as part of this fix.

### ECU-022 — TPS enrichment appears without a new post-stop throttle step

- **Source:** `TpsAccelEnrichment::onEngineStop()` / `resetAE()` / `onNewValue()` /
  `getTpsEnrichment()` in
  [accel_enrichment.cpp](firmware/controllers/algo/accel_enrichment.cpp),
  `Engine::OnTriggerSynchronizationLost()` in
  [engine.cpp](firmware/controllers/algo/engine.cpp), and stopped slow-callback
  ordering in
  [engine_controller.cpp](firmware/controllers/engine_controller.cpp). Fuel
  consumption is in
  `getInjectionMass()` in [fuel_math.cpp](firmware/controllers/algo/fuel_math.cpp).
- **Trigger:** (1) Trigger synchronization is lost after an acceleration event
  and regained before the next slow callback; or (2) the stopped slow callback
  resets TPS history while throttle is held open, then samples that same steady
  throttle and the engine restarts.
- **Actual:** Stop notification calls `onEngineStop()`, which resets only Lua
  enrichment. Its cached acceleration threshold survives a fast phase resync.
  The stopped slow callback does call `resetAE()`, clearing the fractional pump
  and history, but its subsequent `onNewValue()` compares the held TPS value
  with a zeroed history slot and invents a fresh acceleration event. After RPM
  returns, either path can add enrichment without a new TPS step.
- **Expected:** A synchronization loss should invalidate the old transient,
  and the first sample after a stopped reset should seed history rather than
  count as acceleration.
- **Impact:** Extra startup fuel can cause a rich stumble, flooding or a failed
  restart. The host test measures requested fuel mass, not combustion or injector
  delivery. Fast resync exposure lasts until TPS history updates. The stopped
  reset path can recur on every slow callback while TPS remains nonzero.
- **Validation:** `PreviousTpsAccelerationReplaysOnFastPhaseResync` uses a real
  0-to-20 TPS event, 0.20-fraction table and controlled air-mass input. It
  computes 20% enrichment, calls the real trigger-synchronization-loss path,
  regains RPM before the next slow tick, and still gets 20% extra mass.
  `StoppedTpsResetInventsAccelerationFromHeldThrottle` executes the relevant
  stopped reset followed by the real periodic slow sample on twenty ticks
  spanning two seconds at a steady 20% TPS; it observes a false event and 20%
  enrichment after RPM returns. The prior
  two-second fractional-pump test was removed because it omitted the required
  stopped slow resets and did not represent production callback ordering.
- **Fix direction:** Clear cached threshold, delta and event state on stop and
  configuration changes. Seed TPS history on the first sample after reset so
  a held position cannot create an acceleration event. Cover callback ordering
  and stop/resync timing in integration tests.

### ECU-026 — Predictive MAP state survives an engine stop

- **Source:** `SpeedDensityAirmass::getPredictiveMap()` and its persistent
  `m_isMapPredictionActive`, `m_predictionTimer`, `m_sessionTimer`, and
  `m_initialPredictedMap` fields in
  [speed_density_airmass.cpp](firmware/controllers/algo/airmass/speed_density_airmass.cpp)
  and [speed_density_airmass.h](firmware/controllers/algo/airmass/speed_density_airmass.h).
  The production speed-density instance in
  [fuel_math.cpp](firmware/controllers/algo/fuel_math.cpp) is static and has no
  engine-stop reset. `Engine::onEngineStopped()` notifies modules, but this
  air-mass object is not an engine module.
- **Trigger:** An acceleration activates predictive MAP, the engine stops, and
  it restarts before the configured blend duration expires. The tunable duration
  permits up to five seconds.
- **Actual:** With a valid MAP sensor and no new acceleration event, the first
  new-run calculation continues blending the previous run's predicted pressure.
  The old activation and throttle-release state survive the stop.
- **Expected:** A new engine run starts MAP prediction from current sensor
  readings and new TPS events, with the previous run's blend discarded.
- **Impact:** A stale, high MAP estimate can command substantial excess startup
  fuel and distort load-based spark or fuel tables, causing a rich stumble or
  failed restart. The effect depends on stop length and calibration; the host
  test proves the returned pressure, not physical fuel delivery.
- **Validation:** `PredictiveMapBlendReplaysAfterQuickEngineRestart` activates
  a 100-kPa prediction against a valid 40-kPa MAP input with a two-second blend.
  It executes the actual stop/restart path, waits 200 ms, and supplies no new TPS
  event. The new-run `getMap()` still returns 94 kPa and reports prediction
  active. The test uses an isolated instance of the same production class.
- **Fix direction:** Reset predictive MAP state on engine stop and on relevant
  configuration changes; exercise the production static instance through the
  fuel path as follow-up integration coverage.

### ECU-021 — Millisecond VVT startup delay is rounded down to whole seconds

- **Source:** `VvtController::onFastCallback()` in
  [vvt.cpp](firmware/controllers/actuators/vvt.cpp), and `vvtActivationDelayMs`
  in [rusefi_config.txt](firmware/integration/rusefi_config.txt).
- **Trigger:** The configured delay is not an exact multiple of 1,000 ms and
  the other VVT activation gates permit control.
- **Actual:** The unsigned integer millisecond setting is divided by integer
  `MS_PER_SECOND` before comparison to floating-point elapsed seconds. A
  1,500 ms delay becomes 1 second; any positive delay below 1,000 ms becomes zero.
- **Expected:** Honor the entire configured delay before diverting oil flow to
  VVT, as the parameter's description specifies.
- **Impact:** Cam actuation can begin up to almost one second early during
  oil-pressure buildup. Startup instability is credible; physical pressure
  loss or engine damage is not established by these host tests.
- **Validation:** `VvtActivationDelayTruncatesMillisecondsToWholeSeconds`
  sets 1,500 ms at 1,500 RPM. The real controller drives zero at 900 ms, then
  commands 20% PWM at 1,100 ms, still 400 ms before the selected delay expires.
- **Fix direction:** Compare elapsed milliseconds or use floating-point units
  conversion. Cover subsecond, fractional-second, exact-second and zero delays.

### ECU-007 — DFCO timing recovery ignores the selected ramp duration

- **Source:** `DfcoController::getTimingRetard()` in
  [dfco.cpp](firmware/controllers/algo/fuel/dfco.cpp).
  `dfcoRetardRampInTime` explicitly promises timing recovery over the selected
  period; its allowed range is 0–1 second.
- **Trigger:** Fuel resumes after deceleration cut with a nonzero timing-retard
  setting and a ramp duration other than 0.5 seconds.
- **Actual:** The setting controls only the final timeout gate. Interpolation
  always uses 0.5 seconds. Longer ramps end early; shorter ramps discard the
  remaining retard abruptly when the configured duration expires.
- **Expected:** Timing should progress continuously over the selected interval,
  with an explicit immediate-recovery policy for zero duration.
- **Impact:** Faster or discontinuous torque recovery can produce a driveline
  jerk/hesitation during throttle reapplication.
- **Validation:** `DfcoOneSecondTimingRampEndsAfterHalfSecond` enters real DFCO
  with 20 degrees retard and a 1-second recovery, then resumes fuel. Retard is
  10 degrees at 250 ms and zero at 500 ms, instead of 15 and 10 degrees.
- **Fix direction:** Use the configured duration in the interpolation; cover
  short, long and zero ramps and transitions back into DFCO.

### ECU-029 — Failed A/C pressure sensor releases its compressor cut

- **Source:** `AcController::getAcState()` in
  [ac_control.cpp](firmware/controllers/actuators/ac_control.cpp) applies the
  configured minimum and maximum refrigerant-pressure checks only inside
  `if (acPressure.Valid)`. An invalid reading skips both checks and continues
  to the A/C request. The same function denies A/C when CLT is invalid.
- **Trigger:** A configured A/C pressure sensor fails or times out after a
  pressure limit has already turned the compressor off, while A/C remains
  requested and other permissive inputs are valid.
- **Actual:** The next slow callback bypasses the existing high-pressure
  hysteresis state and reasserts the compressor relay even though pressure is
  unknown. An installation without a pressure sensor may intentionally permit
  A/C; this finding concerns failure of a previously working sensor.
- **Expected:** Loss of configured pressure feedback must not release a
  pressure-protection cut or energize the compressor under unknown pressure.
- **Impact:** Re-engagement during a high-pressure fault can stress or damage
  the compressor or refrigerant circuit. The host test observes logical relay
  state, not physical pressure or clutch operation. If a fan is configured for
  A/C-pressure demand, its separate invalid-pressure path in
  [fan_control.cpp](firmware/controllers/modules/fan_control/fan_control.cpp)
  also removes that fan demand; coolant-temperature demand may still run it.
- **Validation:** `FailedAcPressureSensorReleasesCompressorPressureCut` starts
  with valid 250-kPa pressure above a 200-kPa maximum and confirms the relay
  is off. It then makes the same sensor invalid; the next slow callback turns
  the relay on with A/C still requested.
- **Fix direction:** Distinguish an absent optional pressure sensor from a
  configured sensor that has become invalid, and retain the safe compressor-off
  state on feedback loss. Cover low- and high-pressure failures and recovery.

## Fixed findings

| ID | Former defect | Current behavior and regression coverage |
| --- | --- | --- |
| ECU-010 | A batch-to-sequential transition could orphan an open secondary injector. | Injection callbacks carry a snapshot of the output mask, so in-flight close actions target the injectors they opened. `fuelControl.CrankingToRunningClosesSecondaryBatchInjector` and the adjacent staged-batch controls cover the transition. |
| ECU-036 | A failed pedal could arm anti-lag and keep the ETB target open. | Anti-lag requires valid driver intent; ETB suppresses its anti-lag adder when the pedal or intent is invalid. `etb.failedPedalDisarmsAntilagAndClosesEtbTarget` covers the target and motor command. |
| ECU-031 | Releasing clutch-down could bypass the starter timeout. | The slow button callback services an engaged starter before checking permission for a new start. `StartStopTest.ReleasingClutchDownDoesNotBypassStarterCrankingTimeout` covers release without RPM detection. |
| ECU-025 | Nitrous could remain commanded during a fuel-protection cut. | Fast and slow nitrous callbacks clear the relay when `LimpManager::allowInjection()` is false. `NitrousControl.NitrousRelayTurnsOffDuringBoostFuelCut` covers boost cut and recovery. |
| ECU-035 | Invalid vehicle speed could arm a launch spark cut. | A configured launch speed gate requires a valid speed reading. `LaunchControl.InvalidVehicleSpeedDoesNotEnableLaunchSparkCut` covers invalidation after a valid above-threshold reading. |
| ECU-016 | Disabling launch could retain its prior spark cut. | The disabled update path clears cached launch state and spark skip ratio. `LaunchControl.DisablingActiveLaunchControlClearsSparkCut` covers an active cut followed by disabling. |
| ECU-030 | Brake and clutch interlocks could allow a button start with the required pedal released. | The button callback reads the brake and clutch-up state values. `StartStopTest.BrakeInterlockRequiresPressedPedal` and `ClutchUpInterlockRequiresActiveInput` cover both denied and permitted starts. |
| ECU-003 | Anti-lag could remain active after its configured maximum duration. | The timer condition is evaluated on every update; a zero maximum keeps the feature unrestricted. `Actuators.AntilagStopsAfterConfiguredTimeout` covers timeout and rearming. |
| ECU-034 | Invalid MAP feedback could clear an active overboost fuel cut. | The boost-cut hysteresis retains its state until a valid MAP reading falls below the resume threshold. `limp.BoostFuelCutRemainsActiveWhenMapSensorFails` covers sensor loss and recovery. |
| ECU-018 | Priming could inject with fuel disabled or after delayed startup conditions changed. | `PrimeController::onPrimeStart()` rechecks start permission and injection permission when the pulse is due. The `priming.PrimeRespectsDisabledInjection`, `DelayedPrimeSkipsAfterEngineStarts`, `DelayedPrimeRespectsFloodClear`, and `AllowedPrimeStillInjects` tests cover denial and normal delivery. |

## Candidates under investigation

- Software-knock pending cylinder identity is now ECU-032 for a configured
  window beginning after the next spark. Multispark interactions remain to
  be checked separately: `fireSparkAndPrepareNextSchedule()` schedules a
  knock window after each restrike, so sampling may repeat or overlap for one
  combustion event. The driver-level effect is unverified.
- On failed CLT during cranking, `getCrankingFuel3()` uses 0 °C for its base
  fuel table but 20 °C for its coolant multiplier. The default base table is
  flat, so a material effect needs a supported temperature-varying tune and
  a defensible fail-safe target before ranking.
- After oil pressure has passed the five-second startup check, an invalid
  pressure result skips the running minimum-pressure protection branch.
  Whether sensor loss should cut fuel, limit RPM, or preserve propulsion is a
  policy decision; the current source provides no explicit fallback there.

## Candidates narrowed or rejected

- A proposed 100%-secondary staging starvation case is not reachable through
  the normal staging computation, which clamps to 90%. The primary-pulse early
  return alone is not ranked without a realistic supported calibration.
- Identical pressure rows mask the deadtime subcase of ECU-005, but optional
  direct-duration/manual-mass pressure tables also consume invalid values and
  can change the entire pulse. Those paths justify its high severity.
- A generic timer-saturation concern is not an immediate short-timeout defect
  on the primary STM32 targets: their native tick multiplier is 4, not the
  host test build's 100. Do not infer a production limit from host timing alone.
- The `starterRelayDisable` logical output is asserted below the cranking RPM
  threshold despite its name and configuration comment. Relay contact wiring
  and output polarity can invert the electrical effect, so the source alone
  does not establish an unsafe starter-disable behavior.
- Fuel-pump aggressive relief holding configured minimum duty is expressly
  described by its tune field; it is not a zero-duty relief promise.
- A two-second fractional TPS-pump carryover test omitted the production
  stopped slow reset. ECU-022 now uses callback order that occurs in the
  firmware and does not claim long-lived fractional balance.
- Equal TPS/PPS calibration endpoints are rejected by `LinearSensorUnit`'s
  minimum voltage-span check. A hypothetical NaN throttle path needs an actual
  reachable non-finite source before it warrants a finding; resolved analog
  TPS additionally rejects non-finite values in `filterTps()`.

## Coverage and validation limits

This is a bounded source/host-test review of embedded business logic. It does
not certify functional safety, cover every board/configuration, or validate
electrical behavior on an engine. The following checks bound the review.

- Areas inspected: limp arbitration and protection timers, lambda monitor,
  boost target/output paths, ETB validity/jam/initialization paths, gear detector,
  sensor registry and stored values, fuel-pressure compensation, flex fueling,
  nitrous activation, anti-lag, and knock control. Inspection does not mean all
  behavior in these areas is covered.
- Host `make -j12` succeeds. All 43 remaining `EcuLogicReview.*` tests and
  14 master regression tests named in the fixed-findings table pass
  together on GCC with ASan/bounds enabled (57 tests total). The review tests
  intentionally assert current defects; the fixed-findings tests assert the
  corrected behavior.
- The full GCC ASan/bounds suite passes: 1,824 tests from 297 suites. Command:
  `cd unit_tests && make -j12 && build/rusefi_test`.
- The build regenerated tracked generated files. They remain unstaged, per
  repository guidance; no production source has been changed for this review.
