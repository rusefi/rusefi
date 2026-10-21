# ECU firmware findings ordered by severity

This report ranks 31 defects in the reviewed checkout, including 19 new findings
(ECU-037 through ECU-055). The 24 fixed findings are tracked separately below;
active reproductions assert current defects.

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
| 1 | ECU-041 | S2 — High | Reproduced | Lua reload retains spark cuts, launch requests and anti-lag throttle opening |
| 2 | ECU-045 | S2 — High | Source-confirmed in production build | Extra-page burns bypass the running-engine flash-write gate |
| 3 | ECU-001 | S2 — High | Reproduced | Invalid wastegate/idle-valve controller outputs still drive the motor |
| 4 | ECU-006 | S2 — High | Reproduced | Injector duty protection ignores saturation of secondary injectors |
| 5 | ECU-017 | S2 — High | Reproduced | Primary injector duty protection ignores actual trims, wall film and cranking mode |
| 6 | ECU-039 | S2 — High | Reproduced | Lean second-bank feedback bypasses lambda protection |
| 7 | ECU-052 | S2 — High | Reproduced | Nitrous AFR arming ignores lean or invalid second-bank feedback |
| 8 | ECU-011 | S2 — High | Reproduced | Shared-coil ignition ignores cylinder timing trims and distributor odd-fire offsets |
| 9 | ECU-012 | S2 — High | Reproduced through delayed driver request | Software knock uses firing-order position as physical cylinder identity |
| 10 | ECU-037 | S2 — High | Reproduced | Disabling trailing sparks during dwell leaves the trailing coil charged |
| 11 | ECU-051 | S2 — High | Production source-confirmed | Removing an active external injector output loses its turn-off path |
| 12 | ECU-046 | S2 — High | Gate/output overlap reproduced | Sync loss permits flash writes before pending coil discharge |
| 13 | ECU-009 | S2 — High | Source-confirmed; reachability reproduced | Ninth/tenth forward gear indexes beyond the boost duty adder array |
| 14 | ECU-032 | S2 — High | Reproduced with software knock enabled | Delayed software-knock window can be attributed to the next cylinder |
| 15 | ECU-044 | S3 — Moderate | Unsafe callback premise reproduced | Sync loss leaves an HPFP start with invalid off-event timing |
| 16 | ECU-033 | S3 — Moderate | Arithmetic reproduced; live-write path source-confirmed | Unburned zero injector flow can yield an infinite pulse duration |
| 17 | ECU-049 | S3 — Moderate | Reproduced | LTFT learns frozen corrections during invalid lambda feedback or fuel cuts |
| 18 | ECU-050 | S3 — Moderate | Reproduced | Fixed timing retains a released launch spark cut |
| 19 | ECU-054 | S3 — Moderate | Reproduced through injector callbacks | Low-RPM wall-film calculation selects the wrong exponential fallback |
| 20 | ECU-055 | S3 — Moderate | Reproduced through trigger and injector callbacks | Fuel cuts freeze wall-film state and under-command recovery fuel |
| 21 | ECU-038 | S3 — Moderate | Reproduced | Multispark holds the trailing coil through the entire restrike sequence |
| 22 | ECU-040 | S3 — Moderate | Reproduced | Odd-fire MAP windows overlap and discard or misattribute pressure samples |
| 23 | ECU-042 | S3 — Moderate | Reproduced | Custom barometric pressure ignores its own calibration endpoints |
| 24 | ECU-043 | S3 — Moderate | Reproduced | Idle timing soft entry retains the previous run’s elapsed-time origin |
| 25 | ECU-013 | S3 — Moderate | Reproduced | Knock response treats retarded timing as hundreds of degrees of advance |
| 26 | ECU-019 | S3 — Moderate | Reproduced | Percentage acceleration enrichment shrinks with batch/simultaneous injection |
| 27 | ECU-026 | S3 — Moderate | Reproduced | Predictive MAP substitutes the previous run's estimate after a quick restart |
| 28 | ECU-007 | S3 — Moderate | Reproduced | DFCO ignition recovery uses a fixed half-second ramp |
| 29 | ECU-047 | S3 — Moderate | Reproduced | Unassigning an active launch switch retains the full spark cut |
| 30 | ECU-048 | S4 — Low | Reproduced | First odd-fire injection uses the previous cylinder's timing offset |
| 31 | ECU-053 | S4 — Low | Reproduced | Removing the speed input leaves DFCO without its absent-sensor fallback |

### ECU-041 — Lua reload retains control requests from the destroyed script

- **Source:** Lua setters in [lua_hooks.cpp](firmware/controllers/lua/lua_hooks.cpp),
  `LuaThread::ThreadTask()` in [lua.cpp](firmware/controllers/lua/lua.cpp),
  `Engine::resetLua()` in [engine.cpp](firmware/controllers/algo/engine.cpp), and
  consumers in `EngineState::updateSparkSkip()`, `LaunchControlBase::update()`,
  `AntilagSystemBase::update()` and `EtbController::getSetpointEtb()`.
- **Trigger:** A script calls `setSparkSkipRatio`, `setSparkHardSkipRatio`,
  `setLaunchTrigger` or `setRollingIdleTrigger`, then the Lua VM reloads and the
  replacement script does not explicitly clear that request. Native launch
  and anti-lag requests additionally require their normal configuration and
  sensor gates to pass.
- **Actual:** `resetLua()` clears the nested Lua state and several subsystem
  overrides but omits `luaSoftSparkSkip`, `luaHardSparkSkip`, `luaLaunchState`
  and `luaAntilagState`. Their consumers continue applying the old requests
  indefinitely, subject to normal controller gates. Anti-lag can continue
  adding throttle and fuel even with the driver's pedal released.
- **Expected:** VM teardown clears every request owned by that script. A new
  script starts with neutral overrides; applicable controllers refresh their
  outputs from the cleared requests.
- **Impact:** Retained full spark cut can stall the engine or prevent restart;
  retained launch restricts torque. Retained anti-lag can maintain unintended
  airflow and altered combustion. Torque, thermal and physical actuator
  consequences depend on calibration and hardware; no vehicle test was run.
- **Severity/confidence:** S2 — High, reproduced. The explicit request, reload
  and permissive native gates narrow exposure; the host tests demonstrate
  persistent commands rather than a measured vehicle acceleration.
- **Validation:** Three `EcuLogicReview.LuaReset*` tests use real Lua bindings
  and the actual teardown reset. The spark test observes an empty scheduler
  after reset with a 100% hard skip, then normal coil charge/discharge after
  an explicit clear. The launch test observes retained 100% skip and 20%
  fuel addition. The anti-lag test observes a retained 20% ETB target, positive
  motor command and 50% fuel correction; explicit clearing returns the
  target to the configured 1% minimum and removes the fuel correction.
  Host execution calls the production reset directly; it does not run the
  ChibiOS Lua thread or physical throttle.
- **Fix direction:** Give all Lua-owned requests a consistent reset lifecycle,
  then refresh cached limiter/controller outputs and cover script reloads
  whose replacement never writes the old request.

### ECU-045 — Extra-page burns bypass the running-engine flash-write gate

- **Source:** `TunerStudio::handleBurnCommand()` in
  [tunerstudio.cpp](firmware/console/binary/tunerstudio.cpp),
  `burnExtraFlashPage()` in [extra_flash_pages.cpp](firmware/controllers/extra_flash_pages.cpp),
  `writeToFlashNow()` in [flash_main.cpp](firmware/controllers/flash_main.cpp),
  and `storageManagerThread()` in [storage.cpp](firmware/controllers/storage.cpp).
- **Trigger:** A TunerStudio Lua-page or second-tables-page burn while the
  engine is running on an internal-flash board without MFS, including F407.
  No self-stimulation or explicit forced-write command is required.
- **Actual:** The extra-page branch calls `writeToFlashNow()`, requesting
  `MSG_CMD_WRITE_NOW`. The storage manager bypasses `storageAllowWriteID()`
  for this command and writes the full settings sector. The F4 capability
  explicitly disallows flash writes while running, but this path never
  consults it. Ordinary main-page burns use the deferred request.
- **Expected:** A normal configuration-page burn observes the board's running
  write restriction, regardless of which page changed.
- **Impact:** Internal flash erase/program can stall execution and delay
  ignition/injector callbacks and trigger processing, risking extended dwell,
  incorrect fuel delivery or engine shutdown. The bypass is established;
  erase latency and actual output duration were not measured on hardware.
- **Severity/confidence:** S2 — High, source-confirmed in a production build.
- **Validation:** The F407 firmware builds and links. Disassembly of
  `TunerstudioThread::ThreadTask()` contains both extra-page comparisons and
  their call to `writeToFlashNow()`. The complete request/manager/backend path
  reaches internal flash erase/program without the running gate. Normal host
  builds exclude these page-burn calls with `!EFI_UNIT_TEST`; no host burn
  test is claimed for this finding.
- **Fix direction:** Route normal extra-page burns through deferred storage
  policy while preserving shared-sector consistency.

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

### ECU-039 — Lean second-bank feedback bypasses lambda protection

- **Source:** `LambdaMonitorBase::isCurrentlyGood()` in
  [lambda_monitor.cpp](firmware/controllers/math/lambda_monitor.cpp) reads only
  `SensorType::Lambda1`. In contrast, `ShortTermFuelTrim::getSensorForBankIndex()`
  in [closed_loop_fuel.cpp](firmware/controllers/math/closed_loop_fuel.cpp)
  supports Lambda1/Lambda2 and `EngineState::periodicFastCallback()` applies
  the selected bank's trim using `cylinderBankSelect`.
- **Trigger:** Two lambda banks are configured, the engine exceeds all lambda
  protection activation thresholds, bank 1 is healthy and bank 2 stays lean
  beyond the configured timeout. No other cut or DFCO inhibit is active.
- **Actual:** The monitor continually reports good lambda from bank 1 and
  resets its timer. `LimpManager` permits injection despite bank 2 exceeding
  the same configured lambda limit for longer than the timeout.
- **Expected:** A configured lean-protection limit covers each active feedback
  bank, or the configuration explicitly identifies a narrower protected bank.
- **Impact:** A second-bank fuel-delivery fault can persist without the intended
  protection cut, creating a credible damaging lean-combustion risk under load.
  The test proves the missed cut, not physical mixture or damage.
- **Validation:** `EcuLogicReview.LeanSecondBankDoesNotTriggerLambdaProtection`
  uses the real monitor and limp manager at 3,000 RPM, load 100, 80% TPS,
  target lambda 1.0, allowed deviation 0.1 and a 0.5-second timeout. Lambda1
  0.95 / Lambda2 1.3 remains allowed after 0.6 seconds. Moving the same 1.3
  reading to Lambda1 triggers the cut after 0.6 seconds. A lift below all
  restore thresholds clears it. Sensor samples are supplied by host mocks;
  no exhaust transport or physical injector fault is simulated.
- **Fix direction:** Check all configured fuel-feedback banks with defined
  missing/invalid-sensor behavior, retaining the intended cut latch and recovery.

### ECU-052 — Nitrous AFR arming ignores second-bank feedback

- **Source:** `NitrousController::updateAfrConditionSatisfied()` and
  `onSlowCallback()` in
  [nitrous_controller.cpp](firmware/controllers/algo/nitrous_controller.cpp).
  The AFR gate reads only Lambda1 before commanding the common nitrous relay.
- **Trigger:** A dual-bank engine has nitrous armed with a nonzero maximum AFR,
  healthy first-bank feedback and lean or invalid second-bank feedback. The
  other arming gates pass and no independent injection cut is active.
- **Actual:** The AFR condition remains satisfied, the relay remains on and
  the configured nitrous fuel multiplier remains active. Neither the slow
  arming callback nor the fast fuel-permission check rejects the second bank.
- **Expected:** The configured AFR limit covers each active fuel-feedback bank,
  with invalid feedback preventing activation for a bank that requires it.
- **Impact:** Nitrous can continue adding oxidizer during a second-bank fuel
  fault, defeating its mixture interlock and risking damaging lean combustion.
  This is a separate controller from ECU-039's native lambda-protection timer;
  the native protection does not supply the missing second-bank veto either.
- **Severity/confidence:** S2 — High, reproduced. Nitrous use and permissive
  arming gates narrow exposure; no solenoid flow or physical mixture is measured.
- **Validation:** `NitrousControl.SecondBankAfrDoesNotInhibitRelay` configures
  two cylinder banks, a 13:1 gasoline-scale AFR limit, 3,000 RPM and 25% extra
  fuel. Lambda1 0.8 with Lambda2 1.2, then invalid Lambda2, leaves the relay on.
  Identical lean or invalid feedback on Lambda1 turns it off and removes the
  fuel multiplier; valid first-bank recovery rearms it. Disabling nitrous
  clears the relay through the fast callback. Sensor inputs are host mocks.
- **Fix direction:** Evaluate all active feedback banks in the nitrous gate,
  with explicit missing/invalid policy and single-bank compatibility coverage.

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
  path is confirmed in source. The optional software-knock host test
  `KnockSchedulingReview.RealSparkSamplesFiringOrderPositionInsteadOfCylinder`
  also executes the real dwell, spark and delayed sampling callbacks and
  observes cylinder index 1/bank 0 instead of physical index 2/bank 1 at the
  hardware-driver boundary. ADC acquisition and DSP gain/retard remain untested.
- **Fix direction:** Convert event order to physical cylinder before scheduling
  knock acquisition and retain that identity in each pending sample. Cover
  nontrivial firing orders, both banks and differing per-cylinder gains.

### ECU-037 — Disabling trailing sparks during dwell leaves a coil charged

- **Source:** `turnSparkPinHighStartCharging()` and
  `fireSparkAndPrepareNextSchedule()` in
  [spark_logic.cpp](firmware/controllers/engine_cycle/spark_logic.cpp).
  Both independently check the live `enableTrailingSparks` flag; the charge
  callback schedules only charge, and the later leading-fire callback is
  responsible for scheduling discharge. There is no trailing-coil dwell watchdog.
- **Trigger:** Trailing sparks are enabled and a trailing coil starts dwelling.
  A live tune write disables the feature before the leading spark fires.
  The tuning field is live and has no `requiresPowerCycle` annotation.
- **Actual:** The leading coil discharges normally, but its callback skips
  scheduling trailing discharge because the flag is now false. The queue can
  become empty while the trailing output remains high. Subsequent disabled
  cycles do not discharge it. The stopped-engine pin sweep eventually clears
  it; `Engine::efiWatchdog()` bypasses that sweep while trigger motion is recent.
- **Expected:** Every started dwell retains a discharge action independently
  of subsequent feature-disable writes, while new disabled cycles do not charge.
- **Impact:** Prolonged coil/igniter energization can overheat hardware or invoke
  its internal protection and disrupt ignition. An eventual uncontrolled
  discharge can occur at the wrong phase. Electrical response and damage are
  inferred and depend on the coil and driver; only logical outputs are tested.
- **Validation:** `EcuLogicReview.DisablingTrailingSparkDuringDwellLeavesCoilCharged`
  schedules real ignition callbacks at 1,000 RPM with 3-ms dwell and a 6-degree
  trailing delay. Disabling after both outputs are high leaves the trailing
  output high with an empty queue, including at 100 ms; the leading output is
  low. The real stopped-engine sweep clears it. The adjacent
  `IgnitionReview.UnchangedTrailingSparkDischargesBothCoils` control discharges
  both outputs normally. This drives scheduling directly, not TunerStudio I/O
  or continuous crank edges.
- **Fix direction:** Preserve trailing charge/discharge ownership and the
  pending plan across feature changes; add bounded dwell protection.

### ECU-051 — Removing an active external output loses its turn-off path

- **Source:** `EnginePins::stopInjectionPins()` / `OutputPin::deInit()` in
  [efi_gpio.cpp](firmware/controllers/system/efi_gpio.cpp), `efiSetPadUnused()`
  in [io_pins.cpp](firmware/hw_layer/io_pins.cpp), and external-driver mode/write
  methods in [gpio_ext.h](firmware/hw_layer/drivers/gpio/gpio_ext.h) and
  [mc33810.cpp](firmware/hw_layer/drivers/gpio/mc33810.cpp).
- **Trigger:** A live pin reassignment/removal, or cylinder-count reduction,
  deinitializes an active injector on an external GPIO driver whose mode setter
  does not turn its output off. EG33 assigns injectors to MC33810 outputs and
  permits those assignments to change; its storage uses external flash.
- **Actual:** Deinitialization never writes the inactive level. It requests an
  unused mode and discards the old pin address. MC33810 inherits the base mode
  setter, which returns failure without changing the output; that return is
  ignored. Its direct output and cached command retain the last on state.
  A queued injection close later updates the logical object, but its pin is
  now unassigned, so it cannot send an off command to the former driver output.
  The stopped-output sweep also skips the uninitialized object.
- **Expected:** Command an output inactive while its old driver address is
  still available, then release it and handle any remaining event ownership.
- **Impact:** An injector can remain commanded on after its scheduled pulse
  ends, risking uncontrolled fuel delivery, flooding or engine damage. Actual
  current, delivered fuel and the driver's hardware fault response were not
  measured. Do not infer unlimited coil dwell: MC33810 also has hardware dwell
  protection, and this finding's concrete example is an injector output.
- **Severity/confidence:** S2 — High, production source-confirmed, with host
  reproduction of lost output ownership and the missing electrical write.
- **Validation:** EG33 F7 firmware builds and links `OutputPin::deInit()`,
  `Mc33810::writePad()` and `GpioChip::setPadMode()`. Its disassembly confirms
  deinit calls only the unused-mode path before clearing the address, while
  the inherited mode setter merely returns -1.
  `gpio.RemovingActiveInjectorLosesPendingPhysicalTurnOff` runs a real timed
  pulse, removes its output during the pulse, and verifies that the scheduled
  close and stopped-output sweep leave the former electrical mock asserted.
  `gpio.RetainedInjectorPinClosesWithPendingEvent` verifies normal closure
  with the same pulse when the pin stays assigned. Host execution does not
  execute SPI or the external driver thread.
- **Fix direction:** Establish an explicit inactive-before-release invariant
  for on-chip and external outputs, including active pulses and pin remapping.

### ECU-046 — Sync loss permits a flash write while a coil is charging

- **Source:** `Engine::OnTriggerSynchronizationLost()` and `efiWatchdog()` in
  [engine.cpp](firmware/controllers/algo/engine.cpp), `storageAllowWriteID()`
  in [storage.cpp](firmware/controllers/storage.cpp), and timed ignition actions
  in [spark_logic.cpp](firmware/controllers/engine_cycle/spark_logic.cpp).
- **Trigger:** An internal-flash settings write is pending when decoder sync
  is lost during dwell. The crank has moved recently and the timed spark
  remains queued. The storage worker runs before that discharge.
- **Actual:** Sync loss sets cached RPM to stopped immediately. The flash
  gate accepts settings and backup writes based on that state, while the
  watchdog leaves outputs alone when crank activity is recent. A charged
  coil and a permitted flash write therefore coexist.
- **Expected:** Flash-stalling storage waits until engine-control outputs
  and pending work are safe; loss of decoded RPM is not proof that the
  rotating engine and its scheduled outputs have stopped.
- **Impact:** An erase started before the pending spark can stall execution
  with the coil energized and miss crank edges, extending dwell or delaying
  resynchronization. Hardware timing and resulting energy were not measured.
  This requires a pending write and a narrow worker window.
- **Severity/confidence:** S2 — High. Gate/output overlap is reproduced;
  the subsequent flash stall and physical consequences are source-inferred.
- **Validation:** `StorageBoardGate.SyncLossAllowsSettingsWriteDuringActiveDwell`
  schedules real dwell/spark callbacks at 1,000 RPM. Running writes are
  refused. Sync loss during dwell leaves the coil high and one spark queued
  even after the watchdog runs, but both settings write gates now pass.
  Without a simulated erase, the queued spark discharges normally.
- **Fix direction:** Define a storage-safe stopped state accounting for
  recent crank activity and outstanding output work; cover sync loss and
  resynchronization with a pending write.

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
  no hardware DSP capture was run.
- **Validation:** The optional `KnockSchedulingReview.DelayedWindowUsesNextSparkCylinderAndBank`
  executes real scheduled callbacks for consecutive V12 sparks at 1000 RPM
  and a 70-degree start. Both windows request cylinder 1/bank 1, although the
  first belongs to cylinder 0/bank 0. The 20-degree control retains both
  identities; a stopped-engine control discards the due window. The F407
  firmware build also links the production scheduler and knock DSP thread.
  Hardware acquisition, DSP timing and physical knock response remain untested.
- **Fix direction:** Bind cylinder and bank to each pending scheduler action,
  and exercise windows that cross one or more later sparks.

### ECU-044 — Sync loss leaves an HPFP start with no finite off-event timing

- **Source:** `HpfpController::onFastCallback()` and `pinTurnOn()` in
  [high_pressure_fuel_pump.cpp](firmware/controllers/engine_cycle/high_pressure_fuel_pump.cpp),
  sync-loss RPM reset in [engine.cpp](firmware/controllers/algo/engine.cpp), and
  `scheduleByAngle()` / `sumTickAndFloat()` timing conversion.
- **Trigger:** An HPFP start has been promoted from the angle queue into a
  timed callback when synchronization is lost before it executes.
- **Actual:** The inactive fast update zeros the pump request but does not
  cancel the queued start. Sync loss makes `oneDegreeUs` NaN. `pinTurnOn()`
  has no active/RPM recheck: it energizes the valve and then derives the
  turn-off time from that NaN value. Conversion to integer ticks has no
  finite guard, so there is no defined portable turn-off scheduling result.
- **Expected:** Cancel or refuse the pending start after sync loss, or use
  a validated bounded duration that still guarantees valve turn-off.
- **Impact:** HPFP activation can occur after control is inactive with
  unreliable pulse duration, affecting rail pressure, fueling and restart.
  A permanently energized valve or specific physical failure is not claimed:
  behavior after the undefined conversion was deliberately not executed.
- **Severity/confidence:** S3 — Moderate; complete source path confirmed,
  pending-callback and invalid-timing prerequisites reproduced.
- **Validation:** `HPFP.SyncLossRetainsPendingStartWithInvalidOffTiming` uses
  the real HPFP controller, a production-valid 12-minus-1 wheel, angle queue
  and sync-loss reset. After the inactive update it observes the unchanged
  valve-on callback and NaN timing basis, then removes the event without
  executing undefined behavior. `HPFP.PendingStartClosesValveWithValidRpm`
  runs the same setup without sync loss and observes valve activation and
  normal timed turn-off. The F407 ELF contains both HPFP callbacks.
- **Fix direction:** Include HPFP events in sync-loss cancellation and give
  every energized-valve path a finite, bounded off time independent of later
  RPM invalidation.

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

### ECU-049 — LTFT learns a frozen correction while bank feedback is unusable

- **Source:** `ShortTermFuelTrim::getCorrection()` in
  [closed_loop_fuel.cpp](firmware/controllers/math/closed_loop_fuel.cpp),
  `LongTermFuelTrim::learn()` in
  [long_term_fuel_trim.cpp](firmware/controllers/long_term_fuel_trim.cpp), and
  their handoff in [engine2.cpp](firmware/controllers/algo/engine2.cpp).
- **Trigger:** A nonzero short-term trim has been learned, LTFT learning is
  enabled, and lambda feedback becomes invalid or fuel/spark is cut while
  the engine remains in the globally permitted STFT correction state.
- **Actual:** STFT freezes its cell adjustment and reports a per-bank disabled
  learning state. LTFT checks only the global correction state and its special
  launch gate; it keeps integrating the stale adjustment into long-term cells.
  The same omission applies to the per-bank DFCO and AE holdoff states.
- **Expected:** LTFT learning pauses for each bank whenever its upstream
  short-term estimate is frozen because feedback is unsuitable for learning.
- **Impact:** Stored correction can drift without fresh mixture evidence and
  compound the frozen short-term adjustment or contaminate other visited
  load/RPM cells, affecting fueling during recovery and later operation.
  Persistence writes and resulting mixture were not measured in these tests.
- **Severity/confidence:** S3 — Moderate, reproduced for invalid lambda and
  a sustained fuel cut; DFCO/AE share the source path but are not separately tested.
- **Validation:** `LTFT.InvalidLambdaStillLearnsFrozenShortTermCorrection` and
  `FuelCutStillLearnsFrozenShortTermCorrection` first learn a real 10% STFT
  correction. They then execute the normal STFT-to-LTFT handoff at 200 Hz for
  one second. STFT reports the applicable disabled learning state and stays
  at 10%, while LTFT grows above 6% with a one-second time constant. The neutral
  second bank stays neutral. Disabling global STFT correction stops LTFT growth.
- **Fix direction:** Carry per-bank learning eligibility with the correction
  result and apply it consistently to LTFT, including cut-recovery holdoffs.

### ECU-050 — Fixed timing retains a released launch spark cut

- **Source:** `IgnitionState::getAdvance()` / `getRunningAdvance()` in
  [ignition_state.cpp](firmware/controllers/algo/ignition/ignition_state.cpp),
  `EngineState::updateSparkSkip()` in [engine2.cpp](firmware/controllers/algo/engine2.cpp).
- **Trigger:** Launch has set a full hard-skip request, timing mode changes
  to fixed, and launch is then released without another Lua skip setter call.
- **Actual:** Native skip-target refresh is embedded in running timing-table
  calculation. Fixed timing returns before it, so launch updates clear their
  request but the spark limiter retains its prior 100% target indefinitely.
  The same structure can omit newly requested native skips in fixed mode.
- **Expected:** Skip arbitration follows current control requests independently
  of whether timing angle comes from a fixed value or a table.
- **Impact:** Continued full spark cut can stall the engine or prevent firing
  during timing setup. This requires a mode transition with an existing cut;
  normal dynamic timing refreshes it.
- **Severity/confidence:** S3 — Moderate, reproduced.
- **Validation:** `EcuLogicReview.FixedTimingRetainsReleasedLaunchSparkCut`
  runs real periodic updates before and after mode change and release. Ten
  fixed-mode updates leave the limiter at 100% although launch requests zero.
  Actual spark scheduling produces no dwell events. Returning to dynamic
  timing clears the limiter and restores normal coil charge/discharge.
- **Fix direction:** Refresh skip arbitration in the common control-update
  path; cover fixed timing and separate cranking-timing branches.

### ECU-054 — Low-RPM wall-film computation uses the wrong exponential fallback

- **Source:** `WallFuelController::onFastCallback()` and `WallFuel::adjust()` in
  [wall_fuel.cpp](firmware/controllers/algo/wall_fuel.cpp), `expf_taylor_impl()`
  in [rusefi_math.cpp](firmware/libfirmware/util/src/rusefi_math.cpp), and
  `InjectionEvent::onTriggerTooth()` in
  [main_trigger_callback.cpp](firmware/controllers/engine_cycle/main_trigger_callback.cpp).
- **Trigger:** Wall-film compensation is enabled outside cranking with
  `rpm * effectiveTau < 60`. A 0.03-second Basic-model time constant at
  1,900 RPM is a supported calibration and reaches this condition without
  nonfinite inputs or an unburned invalid tune.
- **Actual:** The exponent is below -2, where the approximation returns 0.818
  instead of approaching zero. This overstates retained film and prevents the
  intended beta-to-alpha clamp. Empty-film fuel demand and later transient
  corrections jump at the approximation boundary.
- **Expected:** The exponential approximation remains accurate or conservatively
  bounded over the controller's entire reachable input range. Alpha decreases
  continuously as time per cycle increases relative to the evaporation constant.
- **Impact:** Large transient fueling errors can cause a rich stumble or an
  incorrect subsequent wall-film subtraction. Exposure requires a short
  effective time constant relative to RPM; the default calibration is not
  claimed to hit this boundary in normal running.
- **Severity/confidence:** S3 — Moderate, reproduced through scheduled outputs.
- **Validation:** `EcuLogicReview.LowRpmWallFilmUsesWrongExponentialFallback`
  compares initially empty films at 2,100 and 1,900 RPM with tau 0.03 seconds,
  beta 0.5, 10 mg requested fuel and a 1 g/s injector without deadtime. The
  2,100-RPM control agrees with the exponential model within 0.1 ms. At
  1,900 RPM alpha becomes 0.818 instead of about 0.122, and the real scheduled
  pulse is 20 ms instead of about 11.4 ms. Both injector callbacks execute and
  close the output. It does not measure mixture or fuel-film mass on hardware.
- **Fix direction:** Correct the approximation outside its intended interval
  or use a bounded full-domain implementation, with tests around the -2
  boundary and the minimum supported effective time constant.

### ECU-055 — Fuel cuts freeze wall-film state and under-command recovery fuel

- **Source:** `handleFuel()` in
  [main_trigger_callback.cpp](firmware/controllers/engine_cycle/main_trigger_callback.cpp)
  returns when `LimpManager::allowInjection()` is false, before each event's
  `WallFuel::adjust()` can update its film estimate. The model and its per-cycle
  decay are in [wall_fuel.cpp](firmware/controllers/algo/wall_fuel.cpp).
- **Trigger:** Wall-film compensation has accumulated a film estimate, then a
  fuel cut inhibits injection while the engine continues rotating. Fuel is
  subsequently restored without a stopped-engine reset.
- **Actual:** Every blocked trigger event leaves both the film mass and the
  adjustment counter unchanged. On recovery, the controller subtracts fuel
  release from the pre-cut film, ignoring evaporation during the intervening
  unfueled cycles. Wall-film compensation therefore commands too little fuel
  relative to its own decay model.
- **Expected:** Advance the film model during unfueled cycles while ensuring
  no fuel is injected. Recovery uses the decayed state and the current request.
- **Impact:** A lean recovery transient can cause hesitation or misfire after
  a cut. The physical size depends on real deposition/evaporation and airflow;
  the host evidence establishes inconsistent model state and commanded pulses.
- **Severity/confidence:** S3 — Moderate, reproduced. This is distinct from
  ECU-054's numerical-domain error and ECU-017's duty-protection omission.
- **Validation:** `EcuLogicReview.FuelCutFreezesWallFilmUntilInjectionResumes`
  fills the model through 100 real trigger/injector cycles at 3,000 RPM, tau
  0.5 seconds, beta 0.5 and 10 mg desired fuel. A one-second Lua fuel cut spans
  25 more trigger events: outputs stay off, no events are queued, and the film
  estimate remains unchanged. Applying the model's existing alpha to those
  zero-fuel cycles would leave less than 20% of the original film. Releasing
  the cut schedules about 10 ms instead of the more than 18 ms required by
  that decayed estimate; the output closes normally. The test calls the real
  main trigger callback with controlled phase/RPM, not a crank/cam decoder.
  Lua is the reproduced cut source; other cuts share the same early return.
  DFCO instead supplies zero requested mass through the event path and is not
  claimed to freeze the film in this way.
- **Fix direction:** Separate fuel-film state advancement from permission to
  energize injectors, preserving zero delivery during cuts and covering recovery.

### ECU-038 — Multispark extends trailing-coil dwell through every restrike

- **Source:** `turnSparkPinHighStartCharging()` schedules trailing charge for
  each leading dwell, but `fireSparkAndPrepareNextSchedule()` schedules trailing
  discharge only in its `sparksRemaining == 0` branch in
  [spark_logic.cpp](firmware/controllers/engine_cycle/spark_logic.cpp).
- **Trigger:** Trailing sparks and multispark are both enabled at an RPM where
  extra sparks are requested. These are independently accepted settings.
- **Actual:** The trailing coil charges for the main event and stays charged
  throughout the restrikes. Repeated charge callbacks only set an already-high
  pin high; the one trailing spark occurs after the final leading restrike.
  Its dwell therefore includes all intervening spark gaps and restrike dwells.
- **Expected:** Trailing-coil dwell must obey a defined duration independent of
  the number of leading restrikes. Supporting repeated trailing sparks or one
  final trailing spark both require correctly paired, bounded charge intervals.
- **Impact:** Excessive dwell can overheat the coil/igniter or trigger internal
  current/dwell protection; trailing spark timing and energy can differ from
  the requested plan, causing misfire or starting/driveability problems. Actual
  electrical current, temperature and combustion are not measured.
- **Validation:** `EcuLogicReview.MultisparkHoldsTrailingCoilUntilLastRestrike`
  computes three extra sparks from supported settings at 1,000 RPM. Real
  callbacks produce leading sparks at 5/8/11/14 ms. The trailing output stays
  continuously high from 3 to 15 ms: 12-ms dwell despite a 3-ms main request
  and 2-ms restrike dwell. It finally returns low and the queue empties.
- **Fix direction:** Define trailing/multispark composition explicitly and pair
  each trailing dwell with its discharge and watchdog. This is separate from
  ECU-037: it occurs in steady configuration without a live write.

### ECU-040 — Odd-fire MAP windows overwrite another cylinder's accumulator

- **Source:** `MapAveragingModule::onFastCallback()` clamps window duration
  using the uniform `engineCycle / cylindersCount` interval in
  [map_averaging.cpp](firmware/controllers/modules/map_averaging/map_averaging.cpp).
  Start angles include individual odd-fire offsets. Every window shares the
  same `MapAverager`; `start()` resets its accumulator/identity and `stop()`
  disables it without checking which window owns it.
- **Trigger:** All-cylinder MAP sampling on an odd-fire engine, with the
  configured duration longer than a short firing interval but within the
  uniform-period clamp. A six-cylinder 90/150-degree layout permits a
  110-degree window, overlapping across its 90-degree intervals.
- **Actual:** The second start erases the first cylinder's accumulated samples.
  The first stop publishes the second cylinder's partial measurement and stops
  its accumulation early. Its later stop republishes that same partial value
  with a new timestamp; samples after the premature stop are ignored.
- **Expected:** Windows must retain independent ownership or be bounded to
  the actual next sampling start, including odd-fire offsets and wraparound.
- **Impact:** The fast MAP value used for fueling/load can differ materially
  from the intended pressure-window average, producing fueling/timing errors
  and misfire or driveability problems. Physical pressure traces and resulting
  combustion are not simulated.
- **Validation:**
  `MapSamplingAngle.OddFireOverlappingWindowsDiscardAndMisattributeSamples`
  exercises the real phase scheduling, callbacks and voltage converter. With
  starts at 0/90 degrees at 1,000 RPM, a 110-degree window loses cylinder 1's
  20-kPa sample and reports cylinder 2 at 100 kPa, dropping its later 40-kPa
  sample. A 50-degree control correctly reports 20 kPa and the second window's
  70-kPa mean. The test uses deterministic samples and no ADC/DMA acquisition.
- **Fix direction:** Bound windows using actual cylinder geometry or provide
  separate accumulators and matching completion identities. Cover wrapping
  windows, changed RPM and live window changes as well as steady odd-fire geometry.

### ECU-042 — Custom barometric pressure uses the MAP calibration

- **Source:** `getMapCfg()` / `configureMapFunction()` / `initMap()` in
  [init_map.cpp](firmware/init/sensor/init_map.cpp), and
  `SpeedDensityAirmass::getCompensatedMap()` in
  [speed_density_airmass.cpp](firmware/controllers/algo/airmass/speed_density_airmass.cpp).
- **Trigger:** An analog baro channel uses `MT_CUSTOM` with pressure endpoints
  different from the MAP sensor. The Baro dialog exposes separate low/high
  pressure values, but both converters take the custom MAP configuration.
- **Actual:** The baro converter ignores `baroSensor.lowValue/highValue`.
  With 0–5 V / 0–120 kPa baro and 0–200 kPa MAP, a 4 V baro sample publishes
  160 kPa instead of 96 kPa. Editing baro endpoints does nothing; editing MAP
  endpoints changes both channels. MAP plausibility bounds also govern baro.
- **Expected:** Custom baro pressure uses its own configured endpoints.
- **Impact:** Incorrect barometric correction changes fuel calculations and,
  with compensated MAP enabled, fuel/spark table load selection. The example
  changes 40 kPa measured MAP to 26.67 kPa load instead of 42.22 kPa. Actual
  fuel/timing changes depend on the corresponding tables; no engine outcome
  or ADC electrical accuracy was measured.
- **Severity/confidence:** S3 — Moderate, reproduced.
- **Validation:** `SensorInit.CustomBaroUsesMapCalibrationInsteadOfBaroEndpoints`
  initializes the real registered converter, posts raw voltages and executes
  compensated-MAP calculation. Reinitializing after separate baro/MAP edits
  establishes the wrong dependency; a preset baro curve is the unaffected
  control. Host ADC subscriptions are stubs, so this covers conversion and
  consumption, not sampling hardware.
- **Fix direction:** Pass the sensor-specific custom endpoints and plausibility
  policy to each converter; retain independent MAP/baro calibration coverage.

### ECU-043 — Idle timing soft entry retains the previous run's time origin

- **Source:** `IdleController::determinePhase()`, `getIdleTimingAdjustment()`
  and `onEngineStop()` in [idle_thread.cpp](firmware/controllers/actuators/idle_thread.cpp).
- **Trigger:** Idle timing PID and a positive `idleTimingSoftEntryTime` are
  enabled. The engine first reaches idle some time after cranking, stops, and
  restarts into idle earlier in the new run.
- **Actual:** The initial idle transition stores `m_crankTaperEndTime` and its
  soft-entry endpoint only when that member is zero. Stop resets the RPM PID
  but leaves these times intact. `timeSinceCrankingInSecs` restarts at zero,
  so timing-error amplification stays zero until the new run reaches the old
  timestamp. An idle entry at 20 seconds followed by restart/idle at 1 second
  suppresses correction through 20 seconds despite a configured 2-second ramp.
- **Expected:** Each restart establishes a fresh idle timing soft-entry interval.
- **Impact:** A configured fast idle stabilization mechanism is absent during
  the restart period; idle sag or stalling under load is plausible. The test
  demonstrates suppressed timing correction, not a measured stall.
- **Severity/confidence:** S3 — Moderate, reproduced.
- **Validation:** `idle_v2.RestartRetainsPreviousIdleTimingSoftEntryEpoch` runs
  real phase classification, stop handling and timing PID. It observes 0°
  instead of 10° correction at four seconds into the second run, then 5° at
  21 seconds and 10° at 23 seconds. A fresh-controller control reaches 10° at
  four seconds. The fixture supplies the per-run elapsed-time field directly;
  it does not replay crank edges or simulate engine response.
- **Fix direction:** Reset per-run idle timing entry state with engine stop,
  and cover repeated crank-to-idle transitions with different taper lengths.

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

### ECU-047 — Unassigning an active launch switch retains its control request

- **Source:** `LaunchControlBase::isInsideSwitchCondition()` in
  [launch_control.cpp](firmware/controllers/algo/launch_control.cpp).
- **Trigger:** Switch-input launch is active, then a live tune changes
  `launchActivatePin` to Unassigned while launch remains enabled and its
  RPM, TPS and speed conditions still permit operation.
- **Actual:** An invalid pin skips sampling without clearing
  `launchActivatePinState`. Subsequent updates keep the old active request,
  launch fuel addition and full spark skip. Releasing the former switch has
  no effect because it is no longer read.
- **Expected:** Removing the selected input makes its request inactive.
- **Impact:** Unexpected torque restriction or stalling can continue after
  the input is removed. Exposure is limited to live input reconfiguration
  while the cached switch was active; normal switch release works.
- **Severity/confidence:** S3 — Moderate, reproduced.
- **Validation:** `LaunchControl.UnassigningActiveSwitchRetainsLaunchCut`
  samples a real mocked input, unassigns it, releases the old pin and runs
  three controller/skip updates. The 100% hard skip and 20% fuel addition
  persist. Reassigning the released input clears both effects. This tests
  live configuration consumption, not physical GPIO reinitialization.
- **Fix direction:** Clear cached input state when the pin is unavailable;
  review the equivalent anti-lag input cache, which has the same source pattern
  but a production-only GPIO read that this test does not execute.

### ECU-048 — First odd-fire injection uses the previous cylinder's offset

- **Source:** `InjectionEvent::update()` and `computeInjectionAngle()` in
  [fuel_schedule.cpp](firmware/controllers/engine_cycle/fuel_schedule.cpp),
  first-ready schedule construction in `handleFuel()` in
  [main_trigger_callback.cpp](firmware/controllers/engine_cycle/main_trigger_callback.cpp).
- **Trigger:** The first ready sequential schedule for an odd-fire engine,
  or an output-mode transition that changes an event's physical cylinder.
  A tooth consumes the rebuilt angle before the next schedule refresh.
- **Actual:** The angle is computed using the previous `cylinderNumber`, then
  that identity is changed to the current injector. Initial events all carry
  cylinder zero, so another cylinder can receive cylinder one's odd-fire
  correction for its first pulse. The next update uses the right identity.
- **Expected:** An angle and its output selection use the same cylinder.
- **Impact:** Transient injection-phase error during startup or mode changes.
  The test demonstrates a 32-degree error, not an incorrect pulse quantity,
  sustained error or measured misfire; engine impact depends on injection type.
- **Severity/confidence:** S4 — Low, reproduced; limited duration and no
  demonstrated significant driveability consequence justify the lower rank.
- **Validation:** `injectionScheduling.FirstOddFireScheduleUsesPreviousCylinderIdentity`
  builds a real two-cylinder sequential schedule with -19/+13 degree offsets.
  Cylinder two opens at 341 degrees instead of 373 and the scheduled callbacks
  operate the correct physical injector. Its pulse closes normally and the
  close callback refreshes the angle to 373 degrees. This exercises schedule
  construction and dispatch, not a full crank-signal replay.
- **Fix direction:** Resolve cylinder identity before deriving the angle;
  cover initial readiness and simultaneous-to-sequential transitions.

### ECU-053 — Removing the speed input blocks DFCO's absent-sensor fallback

- **Source:** `deInitVehicleSpeedSensor()` in
  [init_vehicle_speed_sensor.cpp](firmware/init/sensor/init_vehicle_speed_sensor.cpp)
  calls `FrequencySensor::deInit()` in
  [frequency_sensor.cpp](firmware/controllers/sensors/frequency_sensor.cpp),
  which releases the pin but neither unregisters nor invalidates its provider.
  `DfcoController::getState()` in
  [dfco.cpp](firmware/controllers/algo/fuel/dfco.cpp) selects its RPM-rate
  fallback only when `Sensor::hasSensor(VehicleSpeed)` is false.
- **Trigger:** State-aware Decel fuel cut is configured, then the pulse VSS
  input is changed to Unassigned and applied without rebooting. There is no
  replacement speed provider, and RPM continues falling beyond the configured
  deceleration threshold.
- **Actual:** The removed provider remains registered. Its last value remains
  valid until timeout, then the failed-but-present sensor prevents both valid
  VSS classification and the absent-VSS fallback. Fuel cut stays disabled.
- **Expected:** Removing the pulse input vacates its registry slot and discards
  its sample, allowing the documented no-VSS strategy or a replacement provider.
- **Impact:** The selected deceleration fuel cut stops working after live input
  removal; fuel consumption and engine-braking behavior can differ from the
  same tune after restart. No direct stall or large torque effect is established.
- **Severity/confidence:** S4 — Low, reproduced; limited configuration transition.
- **Validation:** `StateAwareDfco.RemovingSpeedInputBlocksAbsentSensorFallback`
  acquires 36 km/h through real frequency conversion, then establishes Decel
  classification and fuel cut from falling RPM. Category deinit/init with an
  unassigned pin leaves the provider registered; after its 500 ms timeout,
  continued RPM decline no longer cuts fuel. Explicitly unregistering the
  obsolete provider immediately restores fallback with unchanged RPM history.
  The test calls the production category functions used by a burn; host builds
  omit the outer hardware reconfiguration and EXTI pin operations. It observes
  DFCO's cut decision, not physical injector pulses or vehicle deceleration.
- **Fix direction:** Pair pulse-provider registration with teardown and
  invalidation, respecting registry ownership when another provider is present.

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
| ECU-024 | Ignition schedule rebuild could leave the previously charged coil energized. | Each ignition event retains the physical outputs charged for its pending fire callback, so remapping the event does not redirect discharge. `ignition.ScheduleRebuildTurnsOffPreviouslyChargedCoil` covers a two-coil to single-coil rebuild during dwell. |
| ECU-015 | A 1% traction spark cut could saturate the hard limiter or produce a 50% soft cut. | Traction percentages are divided by 100 before entering either spark limiter. The `TractionSparkSkip.OnePercent*` and `TenPercent*` tests cover both modes and normalized ratios. |
| ECU-005 | Invalid fuel pressure could select injection duration, manual mass correction and pressure-dependent deadtime. | Direct duration lookup uses nominal reference pressure when feedback is invalid; manual HPFP correction skips the invalid reading; deadtime uses nominal differential pressure plus MAP for the legacy absolute axis. The three `InjectorModel.InvalidFuelPressure*` tests cover these production paths. `UnavailablePressureUsesReferenceBeforeStateUpdate` covers nominal deadtime before the first sensor update, including the legacy absolute axis; `UnavailablePressureInterpolatesDeadtimeAfterRepeatedUpdates` checks interpolation and pulse duration over repeated updates with compensation disabled or feedback absent/invalid. |
| ECU-021 | VVT activation delay could discard fractional seconds and start cam actuation early. | Milliseconds are converted to floating-point seconds before comparison with engine runtime. The four `Vvt.ActivationDelay*` tests check zero, subsecond, exact-second and fractional-second delays with fresh cam feedback, verifying both the activation gate and PWM output around the delay boundary. |
| ECU-029 | Failed A/C pressure feedback could re-enable the compressor after a pressure cut. | An invalid reading with a configured ADC pressure channel keeps the compressor off. `AcPressureTest.FailedPressureFeedbackKeepsCompressorOffAfterCut` covers initially invalid feedback, high/low cuts, failure and recovery; `UnconfiguredPressureSensorDoesNotBlockCompressor` covers the optional-sensor case. Sensor mocks alone do not configure the ADC channel. |

## Candidates under investigation

- Software-knock overlapping requests are demonstrated by
  `KnockSchedulingReview.OverlappingWindowsRequestBothSamples` and
  `RestrikesScheduleOverlappingWindowsForOneCombustion`: two restrikes produce
  three requests 1.5 ms apart, each requesting 8.33 ms of sampling. The real
  driver rejects requests while acquisition or DSP owns its single buffer.
  Acquisition length clamping, processing completion and which later requests
  are accepted still need hardware-driver coverage before ranking a separate
  missed-window or duplicate-retard finding. Delayed identity is ECU-032.
- On failed CLT during cranking, `getCrankingFuel3()` uses 0 °C for its base
  fuel table but 20 °C for its coolant multiplier. The default base table is
  flat, so a material effect needs a supported temperature-varying tune and
  a defensible fail-safe target before ranking.
- After oil pressure has passed the five-second startup check, an invalid
  pressure result skips the running minimum-pressure protection branch.
  Whether sensor loss should cut fuel, limit RPM, or preserve propulsion is a
  policy decision; the current source provides no explicit fallback there.

- On-chip output teardown selects input mode with a pull-up. Whether this
  energizes a former coil/injector control input depends on the board circuit
  and external pull resistors. Verify electrical behavior on a specific board;
  ECU-051 establishes the distinct external-driver omission without assuming
  that on-chip electrical consequence.

## Candidates narrowed or rejected

- A proposed 100%-secondary staging starvation case is not reachable through
  the normal staging computation, which clamps to 90%. The primary-pulse early
  return alone is not ranked without a realistic supported calibration.
- `InjectorModelWithConfig::getFuelPressure()` still wraps an invalid
  differential-pressure payload as a valid absolute result. Its former
  production caller now uses a validated fallback, and no current production
  caller remains. Keep the helper's validity semantics in mind if it is reused.
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

- A timer-preemption race during trigger-event promotion is not established
  for normal STM32 EXTI triggering: the trigger interrupt outranks the timer,
  and the executor's inline cancellation of other pending events is covered.
  An ADC-trigger variant would need its actual interrupt ordering and an enabled
  board configuration before a concurrency reproduction supports a finding.
- Compensated MAP clamps the barometric factor to 0.5..1.5 before division.
  A hypothetical valid zero baro reading does not create a divide-by-zero path.
- Flat-shift consumers check its enable flag, so an old diagnostic activation
  bit after disabling does not by itself demonstrate retained torque reduction.

- A permanent wall-film carryover after a fully observed engine stop is not
  established: the stopped slow callback resets each event's film mass.
  ECU-055 instead concerns a still-rotating engine during a fuel cut.
- The stale VSS registration does not establish a fatal Lua sensor collision:
  `LuaSensor` checks for an existing provider and raises a recoverable Lua
  error before the registry's fatal duplicate check. ECU-053 ranks the proved
  DFCO decision, without claiming an ECU shutdown.

## Coverage and validation limits

This is a bounded source/host-test review of embedded business logic. It does
not certify functional safety, cover every board/configuration, or validate
electrical behavior on an engine. The following checks bound the review.

- Areas inspected: limp arbitration and protection timers, lambda monitor,
  boost target/output paths, ETB validity/jam/initialization paths, gear detector,
  sensor registry and stored values, fuel-pressure compensation, flex fueling,
  nitrous activation, anti-lag, and knock control. Inspection does not mean all
  behavior in these areas is covered.
- Prior inventory validation (not a substitute for the current review):
  host `make -j12` succeeded with GCC and ASan/bounds enabled. All 78 focused
  tests pass: 19 `EcuLogicReview.*` defect reproductions, the oil-pressure and
  injector-flow regressions, the complete VVT, A/C-pressure, injector-model and
  traction-spark-skip suites, and the ignition schedule-rebuild regression.
  Reproductions assert current defects; regressions assert corrected behavior.
- VVT activation coverage observes PWM with fresh cam feedback immediately
  before, at and after positive delay boundaries, plus zero delay at and after
  startup. A/C coverage configures the ADC channel explicitly; sensor mocks
  alone represent neither sensor wiring nor the configured-failure path.
- The prior full GCC ASan/bounds suite passed: 1,851 tests from 303 suites. Command:
  `cd unit_tests && make -j12 && build/rusefi_test`.
  The 78 focused tests also pass three shuffled runs with seeds 31337–31339
  (`--gtest_shuffle --gtest_random_seed=31337 --gtest_repeat=3`).
- Host execution does not exercise software-knock ADC acquisition/DSP or
  establish physical injector/coil/solenoid behavior. Optional host coverage
  executes the real knock scheduling branch up to the hardware-driver boundary.
- The build regenerates tracked generated files. They remain unstaged, per
  repository guidance; production source is unchanged by this review.

### Completed review validation

- Review scope: production logic in checkout `3ff25e8539a`, with investigation
  starting from `c43dbed6810`. A final fetch finds `origin/master` at
  `439833ec0ef`: the checkout is ten commits ahead and 32 behind. The fetched
  changes were inspected, including the GPPWM NaN fix, ADC host subscriptions,
  RPM startup coverage and LiveData naming changes. They were not merged.
  The ranked engine-control paths are unchanged by those upstream logic edits;
  the already-fixed upstream GPPWM defect is not added to this inventory.
- `make -C unit_tests -j12` succeeds with GCC and ASan/bounds instrumentation.
  After all final test edits, **1,875 tests from 304 suites** pass normally
  and in a separate fresh full shuffled process (seed 31337). Run from
  `unit_tests/`: `build/rusefi_test`, then
  `build/rusefi_test --gtest_shuffle --gtest_random_seed=31337`.
  These runs include all four continuation reproductions and Lua mock cleanup.
- This expanded selection passes **224 tests in each of three shuffled runs**,
  seeds 31337–31339, from `unit_tests/`:

  ```sh
  build/rusefi_test --gtest_filter='EcuLogicReview.*:IgnitionReview.*:MapSamplingAngle.*:SensorInit.*:idle_v2.*:HPFP.*:StorageBoardGate.*:LTFT.*:LaunchControl.*:injectionScheduling.*:LimpRegression.*:InjectorFlowRegression.*:gpio.*:Nitrous*:StateAwareDfco.*:fuel.*:LuaHooks.*' --gtest_shuffle --gtest_random_seed=31337 --gtest_repeat=3
  ```

- The optional software-knock build succeeds:

  ```sh
  make -C unit_tests -j12 BUILDDIR=build-knock-review DEPDIR=build-knock-review/.dep KNOCK_SCHEDULING_TEST=yes
  ```

  All **six `KnockSchedulingReview.*` tests** pass three shuffled runs with
  seeds 31337–31339. Run from `unit_tests/`:
  `build-knock-review/rusefi_test --gtest_filter='KnockSchedulingReview.*' --gtest_shuffle --gtest_random_seed=31337 --gtest_repeat=3`.
  This executes real scheduling and a driver-boundary observer; it establishes
  neither ADC acquisition nor DSP completion. The default suite keeps software
  knock disabled. Separate dependency directories avoid mixed variant objects.
- Both targeted firmware builds succeeded earlier in this review, with fresh
  linked ELFs checked in addition to the script exit status. The continuation
  changes only tests and documentation; their production source is unchanged:

  ```sh
  cd firmware
  bash bin/compile.sh config/boards/f407-discovery/meta-info.env -j12
  bash bin/compile.sh config/boards/subaru_eg33/meta-info-subaru_eg33_f7_no_bl.env -j12
  ```

  F407 links the real knock scheduler/driver and HPFP callbacks; its disassembly
  confirms extra-page burns call the forced-write path. EG33 links MC33810,
  output deinitialization and the inherited mode setter; disassembly confirms
  the missing off write. Neither build executes physical I/O or a flash stall.
- Validation failures were resolved or bounded. HPFP fixture attempts initially
  used too few trigger teeth; the passing setup uses a 12-minus-1 wheel and
  satisfies the production shape constraint. The output-removal fixture needed
  a firing order matching its one-cylinder configuration. Shuffling exposed
  `SensorInit.Map` consuming a retained static fast-MAP value; fixture
  invalidation now establishes its intended fallback precondition.
- The unrestricted **in-process full-suite repeat** is not clean: later
  iterations retain state in existing GPIO, SENT, CAN queue, K-line, event queue,
  Lua-sensor and TCU fixtures. Independently running only
  `EventQueue.simple:gpioext.testGpioExt` with `--gtest_repeat=2` reproduces both
  failures without any new review test selected. The event test retains its
  file-static previous value; the GPIO test retains registrations/counters.
  The Lua sensor-read tests now clear their mocks after a separate fresh
  full shuffle reproduced the collision. Running only
  `LuaHooks.TestGetSensorByName:LuaHooks.LuaSensor` reproduced it before that
  fixture cleanup. The repaired three-test getter/creation selection passes
  three in-process iterations, and all 22 `LuaHooks.*` cases are included in
  the 224-case shuffled selection. The other unrelated repeat-state fixtures
  remain unchanged.
- The continuation adds four passing reproductions: nitrous second-bank AFR
  arming, VSS teardown blocking the no-speed DFCO fallback, a wall-film
  exponential-domain error and frozen wall-film state during a fuel cut.
  Together with the earlier additions, coverage grows by **24 normal host
  cases and six optional knock cases**. The added normal cases use real
  controller transitions and, for the wall-film cases, scheduled injector
  callbacks; the report identifies mocked inputs and physical limits.
- Inventory checks confirm 31 active / 24 fixed findings, 55 unique IDs,
  matching rank/detail order and valid repository links. `git diff --check`
  passes. All new coverage files are tracked; generated files, artifacts and
  the ignored local `docs/report.md` draft remain unstaged. The user advanced
  HEAD during the review; the agent did not commit, push or rewrite history.

### Prioritized remaining coverage

1. Capture actual output levels on an MC33810 board during active output
   removal, and on F407 during extra-page burns or a pending burn after sync
   loss. Measure injector closure and coil dwell against scheduled callbacks.
2. Exercise the real software-knock ADC/DSP driver under overlapping windows
   and multispark: quantify buffer ownership, length clamping, rejected
   requests and cylinder attribution through DSP completion.
3. Verify enabled ADC-trigger interrupt ordering and asynchronous sensor
   snapshots on the target MCU. Portable host tests do not establish ISR
   preemption or atomicity of shared multiword timestamps.
4. Resolve the oil-pressure sensor-loss policy. Dual-bank nitrous arming now
   has relay-level coverage in ECU-052; solenoid flow and physical mixture
   remain unmeasured. Measure wall-film recovery transients after cuts and
   validate the short-time-constant calibration range on hardware.
5. Replay full crank/cam signals across odd-fire injection-mode changes and
   rapid restart to quantify ECU-048's phase error and the physical effects
   of other retained state. Hardware effects remain separate from the
   demonstrated commands and scheduler states.
