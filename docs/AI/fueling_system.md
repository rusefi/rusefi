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
10. **Wall Wetting (Transient)**: `WallFuel` model (tau/beta) accounts for fuel sticking to intake walls.
11. **Short-Term Fuel Trim (STFT)**: Closed-loop Lambda correction.
12. **Long-Term Fuel Trim (LTFT)**: Learned corrections from persistent memory (`page3_s`).
13. **Engine Protection (Cuts)**: `LimpManager` applies cuts (Hard/Soft/Boost/Oil).
14. **Injector Characterization**: Conversion from mass to time using `injectorFlow` and `battLagCorr`.
15. **Scheduling**: `engine_cycle/` translates time into crank-angle based events.
16. **Output Logic**: Pulse width is clamped and synchronized with the engine phase.
17. **Hardware Driver**: MOSFETs/GDI drivers fire the physical injectors.

## Cranking Fuel

While the engine is below `cranking.rpm` the pipeline above is bypassed and `getCrankingFuel3()` (in `fuel_math.cpp`) computes the per-cylinder injected mass as:

```
crankingFuel = baseCrankingFuel * coolantTemperatureCoefficient * tpsCoefficient
```

- **`baseCrankingFuel`** comes either from the 3D `crankingCycleBaseFuel` table (coolant x engine-revolutions, `useRunningMathForCranking = false`) or directly from the running math (`useRunningMathForCranking = true`).
- **`tpsCoefficient`** is the `crankingTpsCoef` curve (flood-clear / throttle compensation).
- **`coolantTemperatureCoefficient`** is the coolant enrichment, resolved in one of two ways, selected by a single `flexCranking` toggle:
  1. **`flexCranking` off (or no flex sensor)** — a single coolant curve, `crankingFuelCoef`.
  2. **`flexCranking` on, flex sensor present** — `crankingFuelFlexTable`, a coolant (X, shared `crankingFuelBins`) by ethanol-% (Y, `crankingFuelFlexBins`, 4 rows to keep RAM small) `interpolate3d` lookup. The dedicated ethanol axis lets the whole E0..E100 range be calibrated directly.

The legacy 1D `crankingFuelCoefE100` curve (and its E0→E100 blend) is gone from the firmware path. The field is **retired** (renamed with the `unused` prefix so its byte slot stays reserved and the migration framework ignores it); old tunes that calibrated it are carried over to `crankingFuelFlexTable` by `CrankingFlexTableMigrator` (see below).

## Priming Pulse

The priming pulse (`PrimeController::getPrimeDuration()`) mirrors the cranking design and is driven by the **same** `flexCranking` toggle:
- **off / no flex sensor** — 1D `primeValues` curve vs coolant (`primeBins`).
- **on, flex sensor present** — `primeFlexTable`, a coolant by ethanol-% `interpolate3d` lookup (shares `primeBins` for the coolant axis; 4-row ethanol axis `primeFlexBins`).

### Flex-fuel interaction with `useRunningMathForCranking`

Be careful: in running-math mode the `baseCrankingFuel` already reflects the **flex-adjusted stoichiometric ratio** (ethanol shifts stoich from ~14.7 toward ~9.0). The cranking flex multiplier therefore acts as **additional** cold-start enrichment on top of that, not as the full ethanol fuel correction — calibrate it accordingly to avoid double-counting ethanol.

### Legacy tune migration

The 2D table fields are appended config so existing tunes stay valid. Two mechanisms keep old tunes working:
- `defaultsOrFixOnBurn()` seeds each 2D table from its corresponding 1D curve (E0 / `primeValues`) for any tune whose ethanol axis is still all-zero, so turning flex on is neutral until calibrated.
- `CrankingFlexTableMigrator` (`java_console`, part of the `ComposedTuneMigrator` pipeline) reconstructs `crankingFuelFlexTable` from a user's old `crankingFuelCoef` (E0) and retired `crankingFuelCoefE100` (E100) curves, reproducing the legacy 85%-anchored blend at each ethanol node — so people who had tuned the old flex cranking curves keep their behaviour.

See `docs/calibration-compatibility.md` and `TuneMigrator.java`.

## Key Files
- `firmware/controllers/algo/fuel_math.cpp`: Core mass calculations.
- `firmware/controllers/algo/wall_fuel.cpp`: Transient fueling logic.
- `firmware/controllers/engine_cycle/fuel_computer.cpp`: Orchestration of the fueling pipeline.
- `firmware/controllers/engine_cycle/injection_scheduling.cpp`: Crank-angle event timing.

## Implementation Notes
- All math is floating-point in the `algo/` layer.
- Use `getFuelComputer()` to access the current fueling state.
- Transient fuel (`WallFuel`) is additive to the base fuel mass before injector characterization.

Use injectorState1 injectorState2 etc in logs (see unit_tests/test_results/readme.md for unit tests output) to
assert actual logic state.
