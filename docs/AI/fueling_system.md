# Fueling System Architecture

The rusEFI fueling system is a mass-based model that translates configuration and sensor data into precise injector pulse widths. It operates in 17 distinct stages.

## The 17 Stages of Fueling

1.  **Configuration**: User settings in TunerStudio (VE table, injector size, target AFR).
2.  **Sensors**: Raw ADC values (MAP, IAT, CLT, TPS) are converted to physical units.
3.  **Air Mass (Primary)**: Calculation of air mass in the cylinder (Speed Density or Alpha-N).
4.  **Target Lambda**: Lookup from `targetAFR` table.
5.  **Ideal Fuel Mass**: `AirMass / (TargetLambda * StoichRatio)`.
6.  **VE Correction**: Application of the Volumetric Efficiency coefficient.
7.  **Charge Temperature**: Blending IAT and CLT to estimate actual intake valve temperature.
8.  **Air Density Correction**: Ideal gas law adjustments based on charge temperature.
9.  **Barometric Correction**: Compensation for altitude changes.
10. **Transient Fueling**: Four selectable acceleration strategies (MS Adder, Percent Adder, MAP Prediction, Lua cycle-mass injection), plus independent `WallFuel` compensation (tau/beta). See [Acceleration Enrichment](acceleration_enrichment.md) for their actual execution order, units, settings and interactions.
11. **Short-Term Fuel Trim (STFT)**: Closed-loop Lambda correction.
12. **Long-Term Fuel Trim (LTFT)**: Learned corrections from persistent memory (`page3_s`).
13. **Engine Protection (Cuts)**: `LimpManager` applies cuts (Hard/Soft/Boost/Oil).
14. **Injector Characterization**: Conversion from mass to time using `injectorFlow` and `battLagCorr`.
15. **Scheduling**: `engine_cycle/` translates time into crank-angle based events.
16. **Output Logic**: Pulse width is clamped and synchronized with the engine phase.
17. **Hardware Driver**: MOSFETs/GDI drivers fire the physical injectors.

## Injection Timing Reference

`[tag:injection_timing_reference]` The `injectionPhase` table uses crankshaft
degrees relative to each cylinder's compression TDC. Negative values are before
that TDC; positive values are after it. `getInjectionOffset()` wraps the table
result to the engine cycle (720 degrees for four-stroke operation).
The reference is independent of ignition advance and camshaft/VVT valve timing.

`injectionTimingMode` selects the point of the nominal primary injector pulse
that follows the table. `InjectionEvent::computeInjectionAngle()` calculates
its start as follows, then adds the cylinder's firing-order offset and wraps:

| Mode | Start angle before wrapping and cylinder offset |
| --- | --- |
| Start | table angle |
| Center | table angle - half the pulse duration in crank degrees |
| End | table angle - the full pulse duration in crank degrees |

The duration is `engineState.injectionDuration`, converted using the current
microseconds per crank degree. This is the scheduling duration before per-event
wall-wetting adjustment; it is not a measurement of fuel arrival at the valve.
The `injectionOffset` output reports the wrapped cylinder-relative start angle,
even when the table uses Center or End mode.

For four-stroke operation, `-400` wraps to `320`, `-250` to `470`, and `400`
has the same phase as `-320`. Exhaust/intake TDC is at `360` (or `-360`).
Per-cylinder phasing assumes synchronized sequential operation; batch and
simultaneous operation group or repeat pulses.

Source: `fuel_math.cpp::getInjectionOffset()`, `fuel_schedule.cpp`,
`engine2.cpp` and `engine_math.cpp::getPerCylinderFiringOrderOffset()`.
User explanation: wiki `Injection-Modes#injection-timing-reference`.

## Cranking Fuel

While `RpmCalculator::isCranking()` is true, `getCrankingFuel3()` (in `fuel_math.cpp`) supplies the cycle fuel mass instead of `getRunningFuel()`. Reaching `cranking.rpm` enters running mode; a subsequent RPM dip below that threshold does not re-enter cranking until the engine stops. Injection-mode splitting, downstream adjustments/cuts and injector conversion still apply. See [Cold-start logic](cold_start.md) for the full sequence, ASE/WUE interaction and sensor fallbacks.

```
crankingFuel = baseCrankingFuel * coolantTemperatureCoefficient * tpsCoefficient
```

- **`baseCrankingFuel`** comes either from the `crankingCycleBaseFuel` table (coolant x engine-cycle count, mg converted to grams, `useRunningMathForCranking = false`) or from the running **base** fuel math (`useRunningMathForCranking = true`). The latter does not include the WUE, IAT and ASE multipliers applied by `getRunningFuel()`.
- **`tpsCoefficient`** is the `crankingTpsCoef` throttle-compensation curve. The flood-clear fuel cut is a separate gate.
- **`coolantTemperatureCoefficient`** is the coolant enrichment, resolved in one of two ways, selected by a single `flexCranking` toggle:
  1. **`flexCranking` off (or no flex sensor)** — a single coolant curve, `crankingFuelCoef`.
  2. **`flexCranking` on, flex sensor present** — `crankingFuelFlexTable`, a coolant (X, shared `crankingFuelBins`) by ethanol-% (Y, `crankingFuelFlexBins`, 4 rows to keep RAM small) `interpolate3d` lookup. The dedicated ethanol axis lets the whole E0..E100 range be calibrated directly.

The legacy 1D `crankingFuelCoefE100` curve (and its E0→E100 blend) is gone from the firmware path. The field is **retired** (renamed with the `unused` prefix so its byte slot stays reserved and the migration framework ignores it); old tunes that calibrated it are carried over to `crankingFuelFlexTable` by `CrankingFlexTableMigrator` (see below).

## Priming Pulse

The priming pulse (`PrimeController::getPrimeDuration()`) uses a fuel-mass table in **mg**, converted through the injector model to milliseconds. It is scheduled on ignition-on after `primingDelay` plus 100 ms, subject to rotation, flood-clear and repeated-key-cycle gates. Invalid coolant skips the pulse. Its table selection is driven by the **same** `flexCranking` toggle:
- **off / no flex sensor** — 1D `primeValues` curve vs coolant (`primeBins`).
- **on, flex sensor present** — `primeFlexTable`, a coolant by ethanol-% `interpolate3d` lookup (shares `primeBins` for the coolant axis; 4-row ethanol axis `primeFlexBins`).

### Flex-fuel interaction with `useRunningMathForCranking`

Be careful: in running-math mode the `baseCrankingFuel` already reflects the **flex-adjusted stoichiometric ratio** (ethanol shifts stoich from ~14.7 toward ~9.0). The cranking flex multiplier therefore acts as **additional** cold-start enrichment on top of that, not as the full ethanol fuel correction — calibrate it accordingly to avoid double-counting ethanol.

### Legacy tune migration

The 2D table fields are appended config so existing tunes stay valid. Two mechanisms keep old tunes working:
- `applyDefaultsOrFixAfterBurn()` seeds each 2D table from its corresponding 1D curve (E0 / `primeValues`) for any tune whose ethanol axis is still all-zero, so turning flex on is neutral until calibrated.
- `CrankingFlexTableMigrator` (`java_console`, part of the `ComposedTuneMigrator` pipeline) reconstructs `crankingFuelFlexTable` from a user's old `crankingFuelCoef` (E0) and retired `crankingFuelCoefE100` (E100) curves, reproducing the legacy 85%-anchored blend at each ethanol node — so people who had tuned the old flex cranking curves keep their behaviour.

See `docs/calibration-compatibility.md` and `TuneMigrator.java`.

## Rev-limit fuel correction and cuts

The [main rev limiter](rev_limiters.md) progressively adds running fuel over
the same RPM window used for hard-cut hysteresis. Cut fuel on RPM limit enables
both the soft fuel multiplier and the hard fuel cut; the multiplier reaches
its maximum at the selected limit, while the cut latches strictly above it.
The fuel adder does not restore injection during a cut. See the limiter guide
for coolant-based limits, forced fault cuts, STFT recovery delay and the current
difference between STFT and LTFT learning gates during general cuts.

## Overrun and deceleration fuel cut

[tag:state_aware_dfco] `DfcoController` runs in the fast fuel loop. The
`dfcoFuelCutMode` enum selects Overrun (0), Decel (1), or Both (2). Zero is the
old-tune default; `coastingFuelCutRequiresGear` defaults false. Existing tunes
therefore retain overrun operation without enabling the engine state machine.

Common guards require `coastingFuelCutEnabled`, no VE-tuning or Lua inhibition,
valid CLT above `coastingFuelCutClt`, and acceptable MAP. MAP is optional; a
configured invalid sensor inhibits fuel cut. The MAP threshold retains its
+/-1 kPa hysteresis. These guards do not change physical SM Overrun classification.

Overrun requires valid driver throttle intent below `coastingFuelCutTps`, RPM
above `coastingFuelCutRpmHigh` and speed at least `coastingFuelCutVssHigh`. It
holds the cut through the RPM/speed hysteresis bands until RPM or speed falls
below the corresponding low threshold, throttle opens, or a common guard fails.
Missing speed reads as zero; zero speed thresholds allow speed-independent
operation. With `coastingFuelCutRequiresGear`, only valid `DetectedGear > 0`
qualifies; neutral, unknown, invalid and nonpositive readings prevent this path.

Decel requires an enabled `EngineStateMachine`. It consumes the latest
`engineSmIsDecelerating` flag, including the SM's valid-speed/zero-gear and
primary-state rules. If no vehicle-speed sensor is configured, a positive
`smDecelRateThreshold` instead enables the fallback
`getLastRpmRate() < -smDecelRateThreshold`. A configured but invalid VSS does not
enable that fallback. The fallback uses the held RPM rate independently of the
primary state; it does not reclassify the SM as Decelerating. Decel bypasses the
Overrun TPS, RPM, speed and gear gates. Both accepts either path.

Enabled SM upshift/downshift overlays suppress either path. Suppression resets
the entry delay, so eligibility must again satisfy `dfcoDelay` before cutting.
Disabling the SM immediately removes its Decel request and shift suppression,
even before the slow callback resets its flags. The existing `dfcoActive`
telemetry, ignition retard and trim-recovery handling use the same controller.

`unit_tests/tests/ignition_injection/test_fuelCut.cpp` covers mode selection,
gear validity, missing/failed speed, common guards, physical-state independence,
shift suppression and delayed re-entry, alongside the injection-pipeline tests.

## Launch fuel and trim learning

[Launch control](launch_control.md) multiplies running fuel by
`1 + launchFuelAdderPercent / 100` only in the full launch state, not in the
pre-launch window. The independent launch fuel cut requires RPM strictly above
Launch RPM and can suppress injection despite this enrichment.

Both pre-launch and full launch pause STFT cell updates and LTFT learning.
Existing corrections remain applicable; pausing learning does not reset trims
to neutral. Fuel math and trim learning run before the launch-state update in
the fast callback, so they observe its previous iteration's state. See the
launch guide for exact gates, thresholds, diagnostics and regression tests.

## Time-based WOT enrichment

`WotEnrichment` runs after `EngineStateMachine` in the slow callback and consumes
its WOT flag. Both runtime enable settings must be on. Its eight-point curve
maps continuous WOT time to an AFR offset, with elapsed time capped at 30 seconds.
The first WOT sample starts at zero. Leaving WOT, disabling either feature,
stopping the engine, or burning settings clears the timer and correction.
Defaults are disabled, with a 0..30 second axis and zero offsets.

`FuelComputerBase::getCycleFuel()` applies the offset after the fuel-dependent
stoichiometric conversion: `AFR = stoich * baseLambda + wotAdder` and
`fuelMass = airMass / AFR`. It republishes `targetLambda = AFR / stoich`, so
closed-loop feedback uses the same target. Negative offsets richen; positive
offsets lean. This is an AFR adjustment, not a percentage of fuel. Corrections
that produce nonpositive or nonfinite AFR are ignored, retaining the base target.

Tests in `test_wot_enrichment.cpp` cover state-machine integration, timing and
reset boundaries, curve interpolation, the cap, both signs, flex-fuel stoich,
published targets and invalid adjusted AFR. The state classifier and its
separate STFT-region option are documented in [Engine State Machine](engine_state_machine.md).

## Fuel Consumption Tracking

Downstream of the pipeline, the `TripOdometer` engine module (`firmware/controllers/modules/trip_odometer/`, guarded by `MODULE_ODOMETER` / `EFI_VEHICLE_SPEED`) accounts for fuel actually injected:

- **Input**: `main_trigger_callback.cpp` calls `consumeFuel(actualInjectedMass, nowNt)` on each injection event, where `actualInjectedMass = numberOfInjections * (injectionMassStage1 + injectionMassStage2)` — i.e. post-cut, post-trim staged mass, not the ideal fuel mass.
- **Total consumed**: accumulated as integer grams plus a float sub-gram remainder (`getConsumedGrams()` / `getConsumedGramsRemainder()`) for dynamic range on long runs. RAM-only; resets on power cycle.
- **Instantaneous rate**: `getConsumptionGramPerSecond()` — grams divided by time since the previous pulse; forced to 0 if pulses are more than 0.2 s apart.
- **Also tracked**: trip distance (from `SensorType::VehicleSpeed`), engine run time, ignition-on time.

Consumers: TunerStudio output channels `totalFuelConsumption` (grams) and `fuelFlowRate` (gram/s) in `status_loop.cpp`; verbose CAN broadcast (`can_verbose.cpp`); OBD-II fuel rate PID (`obd2.cpp`); CAN dash conversions (`can_dash.cpp`); Lua hooks `getConsumedGrams` / `getConsumedGramsRemainder` / `getConsumptionGramPerSecond` (`lua_hooks.cpp`). There is no built-in per-hour, fuel-economy, or persistent lifetime-total channel — consumers derive those from g/s, distance, and fuel density.

## Key Files
- `firmware/controllers/algo/fuel_math.cpp`: Core mass calculations.
- `firmware/controllers/algo/wall_fuel.cpp`: Transient fueling logic.
- `firmware/controllers/algo/fuel/fuel_computer.cpp`: Fuel-computer helpers.
- `firmware/controllers/engine_cycle/main_trigger_callback.cpp`: Injection preparation and crank-angle event timing.
- `firmware/controllers/modules/trip_odometer/trip_odometer.cpp`: Consumed-fuel and fuel-rate accounting.

## Implementation Notes
- All math is floating-point in the `algo/` layer.
- Use `getFuelComputer()` to access the current fueling state.
- Transient fuel (`WallFuel`) is additive to the base fuel mass before injector characterization.

Use injectorState1 injectorState2 etc in logs (see unit_tests/test_results/readme.md for unit tests output) to
assert actual logic state.
