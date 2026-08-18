# Kick-Start Cranking Mode

Kick-start cranking mode (`kickStartCranking`, GitHub issue #4569, motivated by Ural motorcycles) is a fallback spark strategy for engines started by a kick lever: crank speed is too low and too erratic for normal angle-based spark scheduling to place a spark reliably. Instead of predicting a future crank angle, the mode fires directly off the trigger edge:

> "I see the trigger mark, after 3ms I plan to ignite in both cylinders"

Both coils are charged immediately at the primary trigger rising edge and fired one dwell-time later. There is no timing advance calculation, no per-cylinder selection, and no dependence on trigger sync - it is effectively a fixed-timing wasted-spark shot for a twin.

## Activation Conditions

All of these must hold (checked in `handleKickStart()`, `firmware/controllers/trigger/trigger_central.cpp`):

- `engineConfiguration->kickStartCranking` is enabled (a `bit` in `rusefi_config.txt`; TS dialog "Kick-start cranking mode" under General/experimental fields)
- `engineConfiguration->isIgnitionEnabled`
- the event is `SHAFT_PRIMARY_RISING` (after `invertPrimaryTriggerSignal` is applied)
- `Sensor::getOrZero(SensorType::Rpm) < KICK_START_MODE_MAX_RPM` (800 RPM, `firmware/controllers/limp_manager.h`)
- dwell is valid: `engine->ignitionState.getDwell()` returns a positive non-NaN value - otherwise the code refuses to charge a coil it would not know when to release

## Mechanism

Two cooperating halves:

### 1. Direct coil control off the trigger edge (`trigger_central.cpp`)

`handleShaftSignal()` calls `handleKickStart(signal, timestamp)` on every shaft signal, right after the `LimpManager::allowTriggerInput()` gate and *before* trigger decoding, tooth logging, or the `isUsefulSignal()` noise filter. Consequently kick-start works with zero trigger sync - any primary rising edge is enough.

On a qualifying edge:

1. `enginePins.coils[0].setHigh()` and `enginePins.coils[1].setHigh()` - both coils start charging immediately (coil indices 0 and 1 are hardcoded; the feature targets two-cylinder engines).
2. A single static `scheduling_s kickStartScheduling` event is scheduled at `timestamp + dwell` via `engine->scheduler.schedule("kickstart", ...)`, whose callback `kickStartFire()` sets both coils low, producing the spark. The timestamp math uses `sumTickAndFloat(timestamp, MSF2NT(dwellMs))` - the mandatory idiom for adding a float ms offset to an `efitick_t` (see "No float->int64 conversions" in CLAUDE.md).
3. Because one shared `scheduling_s` is reused, if a previous fire event is still pending the scheduler ignores the re-schedule - a burst of edges cannot orphan a charging coil.

Dwell comes from the normal `IgnitionState` path; during cranking `getSparkDwell()` returns `engineConfiguration->ignitionDwellForCrankingMs`, so that setting is what defines the charge time in this mode.

### 2. Suppressing normal spark scheduling (`limp_manager.cpp`)

While `kickStartCranking` is set and RPM is below `KICK_START_MODE_MAX_RPM`, `LimpManager::updateState()` clears `allowSpark` with `ClearReason::KickStart` (code 13, shown as "kickstart" in the TS `sparkCutReason` output channel / `fuelIgnCutCodeList`). `onTriggerEventSparkLogic()` (`spark_logic.cpp`) reads `allowIgnition()` and cuts its scheduled sparks, so the two spark paths never fight over the coil pins.

Only spark is suppressed - fuel scheduling is untouched, so injection during kick-start cranking proceeds through the normal cranking fuel path.

Above 800 RPM the limp cut releases and `handleKickStart()` stops acting, handing the coils back to normal angle-based scheduling. The 800 RPM threshold is a compile-time constant, not a config parameter.

## Key Files

- `firmware/controllers/trigger/trigger_central.cpp` - `handleKickStart()`, `kickStartFire()`, `kickStartScheduling` (guarded by `EFI_ENGINE_CONTROL`)
- `firmware/controllers/limp_manager.h` / `.cpp` - `KICK_START_MODE_MAX_RPM`, `ClearReason::KickStart`, the `allowSpark` clear
- `firmware/integration/rusefi_config.txt` - the `kickStartCranking` bit
- `unit_tests/tests/ignition_injection/test_kickstart.cpp` - coverage: charge/fire off the trigger mark, disabled by default, inactive at/above threshold RPM, refusal on invalid dwell, limp-manager spark suppression

## Limitations / Notes

- Hardcoded to coils 0 and 1; not generalized to other cylinder counts or ignition modes.
- No timing advance: spark lands one dwell-time after the trigger edge, wherever the crank happens to be. The trigger mark placement relative to TDC is therefore part of the "calibration".
- Fires on every primary rising edge below the threshold, including edges that the decoder would reject as noise (the check runs before `isUsefulSignal()`).
- The RPM gate uses the RPM sensor value, which near-zero crank speeds may report as 0 - that is the expected operating point.
