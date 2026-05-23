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

## Key Files
- `firmware/controllers/algo/fuel_math.cpp`: Core mass calculations.
- `firmware/controllers/algo/wall_fuel.cpp`: Transient fueling logic.
- `firmware/controllers/engine_cycle/fuel_computer.cpp`: Orchestration of the fueling pipeline.
- `firmware/controllers/engine_cycle/injection_scheduling.cpp`: Crank-angle event timing.

## Implementation Notes
- All math is floating-point in the `algo/` layer.
- Use `getFuelComputer()` to access the current fueling state.
- Transient fuel (`WallFuel`) is additive to the base fuel mass before injector characterization.
