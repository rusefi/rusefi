# Engine state machine and shift recognition

`EngineStateMachine` in `controllers/modules/engine_state_machine` classifies
operating state at the slow-callback cadence. It is runtime-disabled by default.
Settings live on the main configuration page. The port follows AlphaX
`ebb877615ee1`, report 42, via `rusefi.5` revision `1d6e2edbdfc`. It retains
the AlphaX state/shift algorithms and their lifecycle behavior. Integration
relocates settings from the custom page, registers the module and adapts the
existing launch/idle interfaces. Separate operating-mode actuator features and
time-based WOT enrichment are not part of this module.

## Primary state and display overlays

The priority order is Off, Cranking, WOT, AE-driven Transient, RPM-rate
Accelerating/Decelerating, then IdleController phase. WOT uses
`DriverThrottleIntent > smWotTpsThreshold` (default 90%). Driver throttle intent
comes from the pedal on electronic-throttle setups and TPS on cable-throttle
setups. AE's acceleration or deceleration threshold requests Transient, held for
`smTransientHoldTimeMs` after the last qualifying sample.

IdleController phase maps CrankToIdleTaper to Afterstart and Idling to Idle.
Coasting maps to Overrun only with throttle below `coastingFuelCutTps`, RPM above
`coastingFuelCutRpmHigh`, and an engaged transmission. Otherwise it stays
Coasting. Running with closed throttle above a nonzero `maxIdleVss` also maps to
Coasting; the remaining case is Cruising. Without idle control, the phase
fallback is Running.

RPM rate is recomputed at `smRpmRateWindowMs`, floored to the 50 ms slow-callback
period, using actual elapsed time. The value holds between updates and is read
by classification on the next callback. Sampling runs inside shift detection;
in RPM mode it requires at least one shift-direction switch selection and is
skipped during flat-shift torque reduction.
Zero acceleration/deceleration thresholds disable those respective states.
Acceleration can be held for `smAccelHoldMs`; a coasting phase breaks that hold.
Decelerating requires a registered vehicle-speed sensor and a valid
neutral/unknown DetectedGear value of zero. The VSS sample itself is not checked
for validity. The Overrun clutch check uses shift-direction switch selections,
defaulting to engaged when neither direction selects a switch.

`getCurrentState()` returns the primary state. `engineSmCurrentState` is the
display state: Upshifting overrides Downshifting, which overrides LaunchControl,
which overrides the primary state. Primary bits remain available during those
overlays. Flat-shift torque reduction provides an authoritative upshift and
torque-reduction flag. Temperature flags classify CLT below the cold threshold,
above the hot threshold, or in between; invalid CLT falls back to Operating.

## Shift recognition

Clutch Up is true with the pedal at rest; its falling edge opens a shift window.
Clutch Down is true at full press; its rising edge opens a window. Each
direction can select either switch or None. A single configured direction trusts
its switch; two configured directions require accumulated rate evidence. When
both share a switch, the accumulator sign selects the candidate direction.

The accumulator is bounded to -100..100. RPM mode builds positive evidence in
Accelerating and negative evidence while RPM falls. VSS mode integrates signed
vehicle-speed changes and requires a registered VSS sensor. In VSS mode, the numeric
acceleration/deceleration thresholds also serve as km/h/s normalization values;
primary-state classification continues to interpret them as RPM/s. Idle decays
the accumulator; launch is only a display overlay and does not suppress its
updates. The first VSS sample is compared with zero, and invalid VSS also reads
as zero. These can create false direction evidence.

Gain and decay use elapsed seconds. `smAccumulatorSnapTimeMs` optionally clears
opposite-sign evidence after sustained Accelerating or Decelerating/Overrun.
Evidence freezes during an open clutch window. Release closes the window and
clears its accumulator; a window also expires after more than 3000 ms.

With `smSecondClutchSwitchAvailable`, the complementary switch can open the
window early; Clutch Down release can close a Clutch Up window early. Confirmed
direction can be held by `smShiftLatchTimeMs`, with optional extension on a new
full Clutch Down press. Clutch Up at rest always cancels the shift overlay.
An absent VSS sensor suppresses VSS-mode shift flags; an invalid sample does
not. Disabling sets `engineSmEnabled` false and skips further updates, retaining
previous state and history. Engine-stop and configuration-change hooks inherit
the base module's no-op behavior. A subsequent enabled slow callback recomputes
the primary state, but does not reset accumulated history.

## STFT integration

Both `useEngineStateMachine` and `stateBasedStft` must be enabled to replace
RPM/load region selection. Idle, Afterstart and Coasting select the idle cell;
Overrun selects overrun; WOT selects power; all remaining primary states select
cruise. Existing per-region gains, limits and learning gates are unchanged.
The fast fuel loop observes the latest completed slow-callback state.

## Reproduced defects and deferred fixes

The `EngineStateMachineRegression.KnownBug*` tests deliberately assert current
bad behavior in the AlphaX-style import `d63e3cbace1`. They pass before a fix;
update the same expectations when fixing each defect in a subsequent change.
These are inherited defects/limitations, not evidence that this port introduced
them:

- Disable, engine-stop notification and settings burn retain the previous WOT
  flag. The three lifecycle tests establish WOT first, then invoke each path.
- With neither shift direction configured, RPM mode never samples the RPM ramp
  and cannot classify Accelerating.
- Launch is a display overlay, so the primary-state check in the accumulator
  fails to suppress launch-time shift evidence.
- The first VSS sample is compared with zero and creates positive evidence
  even without a measured change in speed.
- Invalid VSS is treated as zero, creating false negative shift evidence.
- Decelerating checks sensor existence rather than VSS sample validity. Its
  reproduction configures one shift direction so the separate RPM-sampling
  defect cannot mask this path.
- Overrun ignores a configured, pressed clutch unless that switch is assigned
  to a shift direction. The reproduction also verifies the resulting overrun
  STFT cell and the change to idle when the same switch is assigned.

`HeldClutchDoesNotCreateFreshShiftAfterBurn` protects a behavior the import
already gets right: burning settings after a shift times out must not invent
another press edge. The deferred reset implementation in `rusefi.5` revision
`a8907d449ae` initializes previous clutch levels to false and breaks this case.
That implementation and its other lifecycle/rate/VSS fixes were not imported.

## Verification

`unit_tests/tests/engine_state_machine/test_engine_state_machine.cpp` covers
state priority, transient hold, rate cadence, clutch edges, accumulator
direction, latches, stale-window timeout and temperature overlays. These are
adapted AlphaX tests, restricted to the imported features.
`unit_tests/tests/test_stft.cpp` covers region mapping and threshold fallback.
The separate regression commit adds nine bad-behavior reproductions and the
held-clutch burn guard without changing production code.
