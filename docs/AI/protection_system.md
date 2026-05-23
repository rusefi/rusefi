# Engine Protection System (LimpManager)

The `LimpManager` is the central authority for engine safety. It monitors engine conditions and determines when to inhibit injection, ignition, or electronic throttle (ETB).

## Core Concepts

### ClearReason
Every cut is associated with a `ClearReason` (defined in `limp_manager.h`). This reason is used for:
- Logging the cause of a cut.
- Informing the user via TunerStudio (displayed in the "Fuel/Ignition Cut Code" gauge).
- Internal logic (e.g., preventing spark cuts in certain conditions to avoid backfires).

### Transient vs. Permanent Cuts
- **Permanent Cuts**: Managed via `m_allowInjection`, etc. These usually trigger a fatal error or a state that requires a power cycle or specific recovery.
- **Transient Cuts**: Managed via `m_transientAllowInjection`, etc. These are evaluated every `onFastCallback` (e.g., RPM limit, Boost cut).

## How-To: Adding an Engine Protection Feature

To add a new protection feature (e.g., "Fuel Pressure Protection"), follow this 4-step pattern:

1.  **Define the Reason**: Add a new entry to the `ClearReason` enum in `limp_manager.h`.
    ```cpp
    enum class ClearReason : uint8_t {
        ...
        FuelPressure,
    };
    ```
2.  **Add Configuration**: Add thresholds and timeouts to `rusefi_config.txt`.
    ```text
    float fuelPressureThreshold;
    float fuelPressureTimeout;
    ```
3.  **Implement Detection**: Add the monitoring logic to `LimpManager::updateState` in `limp_manager.cpp`.
    ```cpp
    if (fuelPressure < config->fuelPressureThreshold) {
        m_fuelPressureTimer.reset(nowNt);
    }
    if (m_fuelPressureTimer.hasElapsedSec(config->fuelPressureTimeout)) {
        allowFuel.clear(ClearReason::FuelPressure);
    }
    ```
4.  **Expose to User**: Update `tunerstudio.template.ini` (usually handled by code generation, but verify the Gauge list) so the user can see why the engine stopped.

## Key Files
- `firmware/controllers/limp_manager.cpp`: Main protection logic.
- `firmware/controllers/shutdown_controller.cpp`: Logic for graceful vs. immediate engine stops.
