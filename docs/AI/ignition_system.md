# Ignition System Architecture

The ignition (spark) system determines when to charge (dwell) and fire (discharge) ignition coils based on engine position, RPM, and load.

## The Spark Pipeline

1.  **Timing Advance Calculation**: Core engine math in `algo/` determines the target advance (e.g., 20° BTDC) based on RPM and Load (MAP or TPS).
2.  **Dwell Calculation**: Determination of coil charging time (in milliseconds) based on battery voltage and RPM.
3.  **Angle Translation**: `engine_cycle/` translates the timing advance into a specific crank angle.
4.  **Event Generation**: Ignition events are created for each cylinder. An event consists of two actions: `START` (start dwelling) and `END` (fire spark).
5.  **Scheduling**: The `EventQueue` handles the execution of these actions at the precise crank angle or time.
6.  **Limp Management**: `LimpManager` can inhibit spark for RPM limiting, protection, or sync requirements.
7.  **Output Driver**: Coils are fired via digital outputs or specialized ignition drivers.

## Ignition Modes
- **Individual Coils (Sequential)**: One output per cylinder, fired every 720°.
- **Wasted Spark**: Outputs shared between companion cylinders, fired every 360°.
- **One Coil (Distributor)**: A single output fired for every cylinder.

## Key Files
- `firmware/controllers/engine_cycle/spark_logic.cpp`: Core scheduling and event generation.
- `firmware/controllers/algo/ignition_state.cpp`: Calculation of base timing and corrections (IAT, CLT, Knock).
- `firmware/controllers/engine_cycle/event_queue.cpp`: The high-precision scheduler.

## Implementation Notes
- **BTDC vs ATDC**: Advance is typically measured in degrees Before Top Dead Center (BTDC). Internal math may use 0-720° cycle coordinates.
- **Dwell Overlap**: rusEFI supports overlapping dwell events (where one coil starts charging before another finishes firing).
- **Wasted Spark on Odd Fire**: Specialized logic in `spark_logic.cpp` handles uneven firing intervals.

Use coilState1 coilState2 etc in logs (see unit_tests/test_results/readme.md for unit tests output) to
assert actual logic state.
