# Sensor Framework Architecture

The sensor framework decouples sensor *producers* (ADC, CAN, frequency inputs, Lua) from *consumers* (fueling, ignition, ETB, logging). Every sensor is registered under a `SensorType` key and read through a uniform static API — consumers never know where a value comes from.

## The Sensor Pipeline

1.  **Acquisition**: Raw input arrives — slow ADC scan, fast (crank-synchronous) ADC, CAN frame, EXTI edge, or Lua script.
2.  **Conversion**: A `SensorConverter` turns the raw value into engineering units (linear, thermistor, table lookup, or a `FuncChain` composing several).
3.  **Storage**: `StoredValueSensor::setValidValue()` / `invalidate()` stores the result with a timestamp; a per-sensor timeout invalidates stale values.
4.  **Composition**: Optional wrappers combine sensors — `RedundantSensor` (two channels must agree, returns average), `FallbackSensor` (primary else fallback), `ProxySensor` (re-expose/filter under another type).
5.  **Consumption**: Control code calls `Sensor::get(type)` (returns `SensorResult`) or `Sensor::getOrZero(type)`; `status_loop.cpp` copies values into output channels for TunerStudio/logs.

## Core API (`firmware/controllers/sensors/core/sensor.h`)

- `Sensor::get(SensorType)` returns `SensorResult` = `expected<float>` — valid value or an `UnexpectedCode` (`Timeout`, `High`, `Low`, `Inconsistent`, `Configuration`).
- `Sensor::getOrZero(type)` is the dominant consumer call; `getOrDefault(type, low, def, high)` maps High/Low codes to clamp values (used for Lambda).
- `Register()` installs a sensor instance into the registry; double registration is a `firmwareError`.
- `Sensor::hasSensor(type)` / `isRedundant(type)` gate optional features and ETB safety logic.

## Key Files
- `firmware/controllers/sensors/core/`: `sensor.{h,cpp}` (registry + static API), `stored_value_sensor.h` (timestamped async storage + timeout), `functional_sensor.h` (raw→converted workhorse), `proxy_sensor.h`, `fallback_sensor.h`.
- `firmware/controllers/sensors/converters/`: `linear_func`, `resistance_func` + `thermistor_func` (Steinhart-Hart), `table_func` (MAF), `func_chain.h` (composition).
- `firmware/controllers/sensors/sensor_type.h`: the `SensorType` enum — registry key for everything.
- `firmware/hw_layer/adc/adc_subscription.cpp`: `AdcSubscription::SubscribeSensor()` binds an analog channel to a `FunctionalSensor` with a per-sensor lowpass `Biquad`.
- `firmware/controllers/modules/map_averaging/map_averaging.cpp`: crank-synchronous MAP sampling (`MapAverager`).
- `firmware/controllers/sensors/frequency_sensor.cpp`, `flex_sensor.cpp`, `can_sensor.h`, `impl/AemXSeriesLambda.cpp`: frequency, flex fuel, and CAN (incl. wideband) producers.
- `firmware/init/sensor/init_sensors.cpp`: master init — `initNewSensors()` at boot, `reconfigureSensors()` on settings change, plus per-category `init_tps.cpp`, `init_thermistors.cpp`, `init_map.cpp`, etc.
- `firmware/controllers/sensors/sensor_checker.cpp`: periodic health checks mapping failures to OBD codes.
- `firmware/libfirmware/util/include/rusefi/expected.h`: the `expected<float>` type underlying `SensorResult`.

## Sensor Categories
- **Thermistors** (CLT/IAT/oil/fuel temp): resistance divider → Steinhart-Hart via `FuncChain`, or linear mode.
- **Pressure** (MAP/oil/fuel/baro): `LinearFunc` with built-in calibrations for common part numbers; `SensorType::Map` is a `FallbackSensor` preferring `MapFast` (crank-synced average) over `MapSlow` (async analog).
- **TPS/Pedal** (`init/sensor/init_tps.cpp`): redundant primary+secondary pairs with agreement checking (`RedundantSensor`, split-range `RedundantFordTps`); `DriverThrottleIntent` proxies pedal-if-present-else-TPS1.
- **Lambda**: analog AFR curve or CAN wideband (`AemXSeriesWideband`); configuring both is a `criticalError`.
- **Frequency-based** (VSS, flex fuel, turbo/input-shaft speed): EXTI edge timing → Hz → converter; rely on `StoredValueSensor` timeouts to fault when edges stop.
- **CAN/OBD**: `consumeObdSensors` reroutes CLT/IAT/TPS/RPM to OBD PID polling and skips analog init.
- **Lua**: `LuaSensor` lets scripts publish any `SensorType`; `AuxAnalog1..8` expose raw voltages to Lua.

## Implementation Notes
- **Redundancy is not fallback**: `RedundantSensor` returns the *average* of two agreeing channels; disagreement beyond `maxDifference` yields `UnexpectedCode::Inconsistent`, not a switch to one channel.
- **MAP min-buffer**: per-cycle averages go through a circular buffer and the *minimum* is used, rejecting intake pulsation spikes.
- **Timeouts** are enforced inside `StoredValueSensor::get()`; timeout 0 means "never expires" (OBD CAN). `Sensor::inhibitTimeouts(true)` suppresses timeouts during flash-erase stalls.
- **Calibration guards** raise firmware errors early: TPS open/closed points too close, primary/secondary calibrations nearly identical (double-wired sensor), non-monotonic thermistor points.
- **Reconfiguration**: `reconfigureSensors()` re-runs category inits; deinit uses `activeConfiguration` (previous settings) so ADC channels move cleanly. `SubscribeSensor`/`Register` are re-entry safe.

## Unit Testing
- `Sensor::setMockValue(type, value)` / `setInvalidMockValue(type)` override any sensor; mocks take precedence over real sensors in the registry. `Sensor::resetRegistry()` runs between tests.
- Framework tests live in `unit_tests/tests/sensor/`; control-logic tests throughout the suite use `setMockValue` to stand up engine state.
- Console equivalents for live debugging: `set_sensor_mock <name> <value>`, `reset_sensor_mocks`, `show_sensors`.
