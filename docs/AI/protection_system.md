# Engine Protection System (LimpManager)

The `LimpManager` is the central authority for engine safety. It monitors engine conditions and determines when to inhibit injection, ignition, or electronic throttle (ETB).

## Core Concepts

### ClearReason

Each `LimpManager` fuel/spark prohibition carries a `ClearReason` (defined in `limp_manager.h`). This reason is used for:

- Logging the cause of a cut.
- Informing the user via TunerStudio (displayed in the "Fuel/Ignition Cut Code" gauge).
- Internal logic (e.g., preventing spark cuts in certain conditions to avoid backfires).

`Clearable::clear()` retains the first reason that clears each output during
an update; it does not record every simultaneous condition. Permanent
prohibitions take priority over transient ones when permissions are read.
Percentage spark skipping and ETB target tapering use separate paths and need
not produce a `LimpManager` cut code.

### Transient vs. Permanent Cuts

- **Permanent Cuts**: Managed via `m_allowInjection`, etc. These usually trigger a fatal error or a state that requires a power cycle or specific recovery.
- **Transient Cuts**: Managed via `m_transientAllowInjection`, etc. These are evaluated every `onFastCallback` (e.g., RPM limit, Boost cut).

## RPM limiting

[Rev limiters](rev_limiters.md) documents the constant/coolant-based hard cut,
its shared hysteresis and soft timing/fuel window, independent throttle taper,
and latched fault ceilings. The main cut starts strictly above its limit and
clears strictly below limit minus hysteresis. Coolant mode replaces the
constant limit and still applies stored soft corrections, even though their
UI panel is hidden. Cut code 4 ("ETB RPM limit") identifies the 1500 RPM jam
fault fuel cut, not the configurable ETB throttle taper.

## Protection warnings

`BoostCut`, `InjectorDutyCycle`, `LambdaProtection`, and the latched ETB jam
(`EtbProblem`) report P0234, C9013, P0171, and P0638 respectively. A single
`LimpManager` timer limits this group to one warning per 30 seconds, including
when the active reason changes. The first warning can be reported immediately.
The limiter only gates diagnostics; cut decisions and recovery are unchanged.
ETB reporting runs from `updateState` so a jam remains eligible for reporting
after another warning consumed the interval, even if `reportEtbJammed` is not
called again.

## Launch cuts

[Launch control](launch_control.md) independently enables fuel and ignition
cuts. `LimpManager` requests `ClearReason::LaunchCut` only when launch is active
and RPM is strictly above Launch RPM. Pre-launch spark skipping and the 100%
skip request at exactly Launch RPM use the spark limiter instead, so launch
can suppress sparks without a launch full-cut code. The launch guide describes
the activation gates, RPM latch, timing/enrichment effects and live-data fields.

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
