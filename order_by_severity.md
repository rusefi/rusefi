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
| 8 | ECU-005 | S2 — High | Reproduced | Invalid fuel pressure selects injection duration, mass correction and deadtime |
| 9 | ECU-009 | S2 — High | Source-confirmed; reachability reproduced | Ninth/tenth forward gear indexes beyond the boost duty adder array |
| 10 | ECU-032 | S2 — High | Source-confirmed | Delayed software-knock window can be attributed to the next cylinder |
| 11 | ECU-033 | S3 — Moderate | Arithmetic reproduced; live-write path source-confirmed | Unburned zero injector flow can yield an infinite pulse duration |
| 12 | ECU-013 | S3 — Moderate | Reproduced | Knock response treats retarded timing as hundreds of degrees of advance |
| 13 | ECU-019 | S3 — Moderate | Reproduced | Percentage acceleration enrichment shrinks with batch/simultaneous injection |
| 14 | ECU-026 | S3 — Moderate | Reproduced | Predictive MAP substitutes the previous run's estimate after a quick restart |
| 15 | ECU-021 | S3 — Moderate | Reproduced | VVT activation delay discards fractional seconds |
| 16 | ECU-007 | S3 — Moderate | Reproduced | DFCO ignition recovery uses a fixed half-second ramp |
| 17 | ECU-029 | S3 — Moderate | Reproduced | Failed A/C pressure feedback can re-enable a compressor after a pressure cut |

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
- **Validation:** `TractionSparkSkip.OnePercentHardCutSkipsEverySpark` and
  `TractionSparkSkip.OnePercentSoftCutSkipsHalfTheSparks` use a 1% table, real
  running timing math and spark limiters at 2,000 RPM. Of 100 decisions, all 100
  are cut in hard mode and 50 in soft mode. These are deterministic saturation
  cases, not statistical assertions about a random 1% sample.
- **Fix direction:** Convert units at the table-to-limiter boundary, keep
  telemetry units explicit, and test low/interpolated and full-scale requests
  plus combinations with launch, shift and Lua cuts. Target-ratio tests assert
  addition of the raw table value and do not enforce percent conversion.

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

### ECU-033 — Unburned zero injector flow can yield an infinite pulse width

- **Source:** `TunerStudio::handleWriteChunkCommand()` in
  [tunerstudio.cpp](firmware/console/binary/tunerstudio.cpp) copies settings
  directly to the live configuration before `finishPendingBurn()` validates
  them. `validateConfig()` in
  [engine_controller.cpp](firmware/controllers/engine_controller.cpp) rejects
  zero or nonfinite primary flow, and secondary flow when staging is enabled,
  at startup and burn. `getInjectionMass()` in
  [fuel_math.cpp](firmware/controllers/algo/fuel_math.cpp) prepares the injector
  model from live settings; `InjectorModelBase::getBaseDurationImpl()` in
  [injector_model.cpp](firmware/controllers/algo/fuel/injector_model.cpp)
  divides by that flow. The stage-one scheduler in
  [main_trigger_callback.cpp](firmware/controllers/engine_cycle/main_trigger_callback.cpp)
  rejects NaN and negative durations but not positive infinity.
- **Trigger:** A running ECU starts with valid injector flow, then receives a
  zero-flow settings write before the burn and its validation complete. The
  normal fuel calculation runs during that interval.
- **Actual:** Startup and burn validation reject the zero flow, but the live
  write has already changed the value consumed by `prepare()`. Positive fuel
  mass then produces an infinite duration; conversion to integer ticks in the
  scheduler has undefined C++ behavior. The burn error does not guard the
  interval before validation.
- **Expected:** An invalid live write must not reach pulse scheduling, and
  duration conversion must reject nonfinite values.
- **Impact:** A live zero-flow edit can briefly disrupt injection or produce
  unpredictable scheduling before the burn error. The host test establishes
  the model result, not the physical injector output or a target-specific cast.
- **Validation:** `UnburnedZeroInjectorFlowStillProducesInfinitePulse` first
  validates a positive-flow tune, changes the live flow to zero, observes an
  infinite model duration for positive fuel mass, then confirms that burn
  validation rejects the value. `InjectorFlowRegression.SecondaryZeroFlowRejectedOnlyWhenStaged`
  confirms that zero secondary flow is accepted while unused and rejected when
  staging is enabled. The TunerStudio write and scheduler paths are source-traced
  rather than driven end-to-end in the host test.
- **Fix direction:** Validate before a new flow value becomes active, or retain
  the last valid flow until validation succeeds. Guard scheduled durations
  with `std::isfinite` before converting to ticks. Cover running primary and
  enabled staged-secondary updates.

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
| ECU-004 | Invalid ethanol readings could select secondary-fuel stoichiometry from unusable payload bytes. | `FuelComputer::getStoichiometricRatio()` uses primary stoichiometry when flex feedback is missing or invalid. `FuelComputer.InvalidEthanolPayloadDoesNotControlFuelStoichiometry` covers different invalid payloads. |
| ECU-002 | Zero protection delays could cut fuel despite healthy inputs or disabled lambda protection. | Lambda, low- and high-oil pressure, and sustained injector-duty cuts now require their fault conditions. The four `limp.zero*DelayDoesNotCut*` tests, `LambdaMonitor.ZeroTimeoutRequiresBadReading`, and `limp.disablingLambdaProtectionReleasesActiveCut` cover immediate faults and healthy or disabled states. |
| ECU-028 | Kick-start cranking could fire coils despite a Lua ignition cut or required phase sync. | `handleKickStart()` checks ignition permission and permits only its own `KickStart` clear reason. `kickStart.sparkRespectsLuaIgnitionCut` and `sparkRespectsRequiredPhaseSync` cover both denied paths. |
| ECU-008 | The high oil-pressure timeout could already be expired on the first running sample. | The timer resets while stopped or without valid high-pressure feedback, then begins on the first running sample. `limp.highOilPressureTimeoutAlreadyExpiredAtEngineStart` and `highOilPressureTimeoutStartsAtFirstRunningSample` cover startup and elapsed time. |
| ECU-014 | Invalid oil-pressure payload could control a running maximum-pressure fuel cut. | The high-pressure branch requires a valid sensor result before comparison, so invalid payload bytes cannot trigger its timer or cut. `LimpRegression.InvalidOilPressurePayloadDoesNotTriggerMaxCut` checks both payload values after the timeout. |
| ECU-020 | VVT could drive a solenoid without a cam measurement or after cam pulses stopped. | Per-cam update timers now require synchronized feedback newer than one second; missing or stale feedback disables PWM and resets control state. `Vvt.DoesNotActuateWithoutAnyCamMeasurement` and `Vvt.StopsDrivingAfterCamPulsesStop` cover both cases. |
| ECU-022 | TPS acceleration enrichment could replay across a quick resync or appear from held throttle after a stopped reset. | Stop resets TPS event state and fuel fractions; the first sample after reset seeds the baseline. `fuel.PreviousTpsAccelerationClearedOnFastPhaseResync` and `fuel.StoppedTpsResetDoesNotInventAccelerationFromHeldThrottle` cover both callback orders. |
| ECU-023 | Invalid flat-shift axis payload could select spark cut, timing and time limit. | Flat-shift lookup paths require valid axis input and use a defined fallback for an unavailable time axis. `ShiftTorqueReduction.InvalidFlatShiftAxisPayloadCannotChangeSparkCutOrTiming` covers cut, retard and time-gate behavior. |
| ECU-027 | Zero HPFP lobes could leave a configured GDI pump active with a nonfinite fuel request. | Zero lobes now disable GDI pump activation and yield zero quantity and angle requests, even with an assigned valve pin and nonzero pump volume. `HPFP.ZeroLobesDisableConfiguredPump` covers the validator, quantity calculation and active-state gate. |

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
- Host `make -j12` succeeds. All 26 remaining `EcuLogicReview.*` tests,
  `LimpRegression.InvalidOilPressurePayloadDoesNotTriggerMaxCut`,
  `HPFP.ZeroLobesDisableConfiguredPump`, and
  `InjectorFlowRegression.SecondaryZeroFlowRejectedOnlyWhenStaged` pass on GCC
  with ASan/bounds enabled (29 focused tests). The review tests assert current
  defects; the regression tests assert corrected behavior.
- The full GCC ASan/bounds suite passes: 1,836 tests from 301 suites. Command:
  `cd unit_tests && make -j12 && build/rusefi_test`.
- The build regenerated tracked generated files. They remain unstaged, per
  repository guidance; no production source has been changed for this review.
