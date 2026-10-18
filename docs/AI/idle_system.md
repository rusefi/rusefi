# Idle Air Control (Idle) System Architecture

The idle subsystem keeps the engine running smoothly when the driver is off the throttle.
It manipulates an Idle Air Control (IAC) actuator - typically a stepper motor, DC motor, or
PWM solenoid - to admit just enough air past the closed throttle plate, and may also
nudge ignition timing for fast-response trim.

## The Idle Control Pipeline

1.  **Target RPM**: A target idle RPM is interpolated from a coolant-temperature curve
    (cold engines idle higher), with an optional bump while the A/C button is pressed.
    Hysteresis thresholds (`IdleEntryRpm` / `IdleExitRpm`) are computed to prevent
    oscillation between idle and coasting states.
2.  **Phase Classification**: Engine state is classified as `Cranking`, `CrankToIdleTaper`,
    `Idling`, `Coasting`, or `Running` based on RPM, TPS, VSS, and the cranking taper.
3.  **Open-Loop Position**: A base IAC position is computed from a CLT/RPM correction
    table indexed by final target RPM, with additions for A/C, cooling fans, external
    switches, Park/Neutral, antilag, Lua scripts, and TPS/RPM tapers. Cranking can
    select a separate CLT curve or running air; with flare, cranking air is additive.
4.  **Closed-Loop PID**: When in `Idling` phase with valid TPS in automatic mode, an RPM
    PID adds a correction on top of the open-loop value, with anti-windup, dead-zone, and
    soft-entry handling.
5.  **Timing Trim**: An optional separate PID applies small ignition-advance corrections
    at idle to react faster than airflow changes can.
6.  **Modeled-Flow Idle**: An alternate path converts the desired IAC % into a target
    airmass, splits the high-frequency component into the timing PID, and maps the
    remaining airflow back to a valve position via an idle-flow estimate table.
7.  **Actuation**: The final percentage is sent to the IAC hardware (`applyIACposition`).

## Key Files

- `firmware/controllers/actuators/idle_thread.cpp`: Main idle controller logic - phase
  detection, target RPM, open-loop, closed-loop PID, modeled-flow, and the fast-callback
  entry point.
- `firmware/controllers/actuators/idle_thread.h`: `IdleController` / `IIdleController`
  interface and `Phase` enum.
- `firmware/controllers/actuators/idle_hardware.cpp`: Hardware abstraction that drives
  the actual IAC actuator from the controller's output percentage.
- `firmware/controllers/actuators/dc_motors.cpp` and `stepper.cpp`: Concrete IAC actuator
  drivers.

## Implementation Notes

- **Park/Neutral**: `idleParkNeutralOffset` is a signed percentage-point addition to
  running open-loop position, separate from `luaAdd`. Lua reports the state with
  `setParkNeutral(boolean)`; it defaults to false and clears on Lua reset. Zero
  calibration preserves existing tunes. Like `luaAdd`, it does not affect the
  cranking curve or the optional dedicated coasting table.

- **Hysteresis**: Re-enter idle when RPM drops below `target + idlePidRpmUpperLimit`, but
  declare coasting only above `target + 1.5 * idlePidRpmUpperLimit` to avoid flapping.
- **Cranking air and flare**: `crankingAirAmountEnabled` defaults on. Without flare,
  the cranking curve supplies the opening and blends into running air. Disabling
  it uses running air throughout. `crankingIdleRpmFlareEnabled` adds a CLT-based
  RPM target (`cltCrankingRpmAdder`, sharing `cltCrankingCorrBins`). With both
  enabled, the cranking air curve is added to running air and fades to zero.
  `afterCrankingIACtaperHoldDuration` holds before the existing taper duration;
  both use CLT curves and engine cycles counted from rotation start, including
  cranking. Zero taper duration completes at the hold boundary. Modeled-flow
  idle bypasses this hold/taper and flare by using a completed taper fraction.
  Closed-loop air/timing remain disabled during cranking and taper. See
  [Cold-start logic](cold_start.md#states-and-time-bases).
- **A/C Pre-Bump**: We respond to the A/C *button* rather than the relay, because the relay
  is intentionally delayed so airflow bump arrives before the compressor load.
  `acIdleRpmAdder` adds RPM to the coolant target. Air comes from the eight-point
  `acIdleAdderByPressure` curve, using `AcPressure` in kPa; missing/invalid pressure
  selects the first value. Air applies in Idling/CrankToIdleTaper and also to the
  optional dedicated coasting table. It does not add running air on throttle.
- **External switches**: Three `idleUpSwitchPins`, input modes and `idleUpAdder`
  percentages add to running open-loop air. They do not change RPM targets and
  do not modify the exclusive cranking curve or dedicated coasting table.
- **Off-idle target**: A positive `offIdleRpmAdder` arms in Running/Coasting,
  holding full addition on entry into idle/taper. Once absolute RPM change per
  fast-callback interval is below `offIdleRpmStabilityThreshold`, the wait timer
  starts. Stability is not rechecked during waiting. After `offIdleWaitTime`,
  the addition decays linearly over `offIdleRpmAdderDecayTime`. Running/Coasting
  rearms it; cranking, stop, configuration change or zero adder clears it.
  Flare and off-idle additions affect the final target and air-table lookup,
  while entry/exit thresholds use only the coolant target plus A/C RPM addition.
  `offIdleAdderStateIndex` logs 0 inactive, 1 armed, 2 stabilizing, 3 waiting,
  4 decaying; `offIdleAdderCurrentRpm` logs the addition.
- **Moving idle**: `idleVssGateClutchOverride` bypasses `maxIdleVss` when the
  transmission is disengaged or valid detected gear equals zero. Neutral
  latches through transient gear matches. Throttle, invalid TPS, RPM above
  exit, cranking, configuration changes, stop or returning below the speed
  gate clear the latch. Clutch inputs work without enabling state classification.
- **Fixed cranking ETB**: `crankingTpsTargetEnabled` selects `cranking.tpsTarget`
  as actual TPS percent while the RPM calculator reports cranking and VSS is
  below 2 km/h. Missing/invalid speed is zero. Normal ETB initialization and
  actuator safety checks still apply; the opening bypasses pedal/idle blending.
- **Dashpot**: After a throttle release, TPS used for the taper is held and then decayed
  linearly toward zero, smoothing the return-to-idle and reducing stall risk.
- **Timing PID**: Only active in `Phase::Idling`; resets otherwise to avoid carrying stale
  integration into transients.

## Tune migration

The console's `IdleAirMigrator` converts fixed A/C air to a flat pressure curve
and keeps cranking air enabled for legacy tunes. It converts the old absolute
A/C RPM floor to `max(0, floor - warmIdle)`, capped at 1000 RPM, where `warmIdle`
is the last point of the previous idle RPM curve. Review this approximation
across coolant temperatures: a floor cannot be represented exactly by an adder.
Direct TunerStudio imports require manual review of renamed settings.

The return-to-idle target ramp has no effect; configure the opt-in off-idle
addition instead. Generic and affected engine presets start with no TPS dashpot
air addition; migrated explicit dashpot calibration remains intact.
