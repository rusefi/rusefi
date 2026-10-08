# Lua Scripting API (custom hooks)

This document inventories the custom functions and classes that rusEFI firmware registers
into the user Lua VM, grouped by category. It is intended for AI assistants and developers
working on the scripting surface; user-facing docs live at https://wiki.rusefi.com/Lua-Scripting

## Where hooks are registered

| File | Role |
|-------------------------------------------------------|-------------------------------------------------|
| firmware/controllers/lua/lua_hooks.cpp | Main registration site: `configureRusefiLuaHooks()` registers nearly all functions and the luaaa helper classes |
| firmware/controllers/lua/lua_hooks_util.cpp | `configureRusefiLuaUtilHooks()`: print/interpolate/find* helpers, `mcu_standby` |
| firmware/controllers/lua/lua_hooks_ext.cpp | `configureRusefiLuaHooksExt()`: currently empty extension point |
| firmware/controllers/lua/lua.cpp | Lua thread itself; registers `setTickRate`, invokes the `onTick` callback |
| firmware/controllers/lua/lua_can_rx.cpp | CAN RX queue; invokes the `onCanRx` callback and fills `global_can_data` |
| board `boardConfigureLuaHooks()` | `PUBLIC_API_WEAK` hook for board-specific additions (no in-tree overrides today) |

Helper classes are bound with the vendored `luaaa` binding library
(`firmware/controllers/lua/luaaa/`, `LUAAA_WITHOUT_CPP_STDLIB`). A local specialization of
`PlacementConstructorCaller` lets a class constructor receive `lua_State*` as a hidden first
argument (used by `Sensor`/`LuaSensor` for error reporting and name lookup).

Conventions worth knowing:

- **Indexing**: TS-facing indices (CAN bus, curves, tables, TS buttons, `setLuaGauge`) are
  1-based ("human") and converted with `HUMAN_OFFSET`; sensor indices, PWM channels, aux
  digital inputs and `vin()` are 0-based. See the "what a mess" comment in `lua_hooks.cpp`.
- **Invalid input** usually returns `nil` (bad pin, unknown sensor index, out-of-range gauge)
  or raises a Lua error via `luaL_error` (unknown sensor name, bad CAN bus, bad duty).
- Many hooks are conditional on build flags (`EFI_CAN_SUPPORT`, `EFI_ELECTRONIC_THROTTLE_BODY`,
  `EFI_LAUNCH_CONTROL`, ...). A few are excluded on STM32F4 to save flash
  (`getCanRxDropped`, `getCanBaudRate`, `setEngineTorque`, `getTorque`, `getGpPwm`,
  `disableExtendedCanBroadcast`); boards can also trim with `DISABLE_LUA_SET_TIMING_MULT`,
  `DISABLE_LUA_SET_FUEL_ADD`, `WITH_LUA_PID`, `WITH_LUA_CONSUMPTION`, `WITH_LUA_STOP_ENGINE`.

## Categories

### 1. Reading inputs (sensors, pins, SENT)

Read-only access to the sensor framework (see [sensors_system.md](sensors_system.md)) and raw hardware inputs.

| Function | Notes |
|------------------------------|-------|
| `getSensor(name)` | By-name lookup via `findSensorTypeByName`; Lua error on unknown name; returns nil when sensor invalid |
| `getSensorByIndex(idx)` | 0-based `SensorType` index |
| `getSensorRaw(idx)` | Raw (pre-conversion) value, e.g. volts |
| `hasSensor(idx)` | Boolean: sensor registered |
| `getAuxAnalog(idx)` | Shortcut for `SensorType::AuxAnalog1 + idx` |
| `readPin("PA0")` | Direct `palReadPad` of any named pin (prod only) |
| `getDigital(idx)` | Firmware-interpreted switches: 0=clutch down, 1=clutch up, 2=brake, 3=AC button |
| `getAuxDigital(idx)` | `luaDigitalInputPins[idx]`, 0..LUA_DIGITAL_INPUT_COUNT-1 (8); nil if pin not configured |
| `getSentValue(idx)` / `getSentValues(idx)` | SENT sensor decoded value / raw sig0+sig1 pair (EFI_SENT_SUPPORT) |
| `vin(idx)` | One char (as integer) of the configured VIN, 0-based |

### 2. Publishing virtual sensors (Lua as a sensor source)

Lets a script *be* a sensor - e.g. CAN-sourced coolant temp or pedal position.

| Hook | Notes |
|------------------------------|-------|
| `Sensor` class (`LuaSensor`) | `Sensor.new("Clt")`: registers a `StoredValueSensor` under the named `SensorType`; methods `set(value)`, `invalidate()`, `setTimeout(ms)` (default 100ms staleness), `setRedundant(bool)`; Lua error if the type is already registered; auto-unregisters on GC |
| `setLuaGauge(idx, value)` | 1-based, writes one of `luaGauges[LUA_GAUGE_COUNT]` (8) - purely for display/logging, header comment notes LuaSensor could do the same |
| `setDashLight(idx, bool)` | 0-based magic index (0 left turn, 1 right turn, 2 check engine, 3 high beams, 4 parking brake) writes the matching `dash*` bit directly into `engine->outputChannels`; Lua error on any other index. Firmware never touches these bits, so the script owns them; read back via `getOutput("dashLeftTurn")` etc. `dashCheckEngine` is independent of the firmware-derived `checkEngine` bit |
| `setAirmass(airmass, engineLoadPercent)` | Feeds the `LuaAirmass` model (clamped 0..10 g, 0..1000 %) - used when fuel algorithm is set to Lua; pairs with `getAirmass` below |

### 3. Virtual switch inputs (overriding driver controls)

Boolean state pushed into `engine->engineState.lua.*`, consumed by the respective controllers
instead of / in addition to physical switches.

| Function | Target |
|------------------------------|--------|
| `setClutchUpState(bool)` / `setClutchDownState(bool)` | Clutch switches |
| `setBrakePedalState(bool)` | Brake pedal |
| `setAcRequestState(bool)` | AC request "button" |
| `setTorqueReductionState(bool)` | Flat-shift/torque-reduction trigger |
| `setLaunchTrigger(bool)` | `launchController.luaLaunchState` (EFI_LAUNCH_CONTROL) |
| `setRollingIdleTrigger(bool)` | `antilagController.luaAntilagState` (EFI_ANTILAG_SYSTEM) |

### 4. Closed-loop trims and adjustments (fuel / ignition / boost / idle / ETB)

Numeric trims applied on top of the normal control algorithms. These are the primary
"engine control from Lua" surface; values persist until the script changes them.

| Function | Target |
|------------------------------|--------|
| `setFuelAdd(mg)` / `setFuelMult(coef)` | Fueling (see [fueling_system.md](fueling_system.md)); Add is per-cylinder mg |
| `setTimingAdd(deg)` / `setTimingMult(coef)` | Ignition advance (see [ignition_system.md](ignition_system.md)) |
| `setSparkSkipRatio(r)` / `setSparkHardSkipRatio(r)` | Soft/hard spark-skip (traction/launch style torque cut) |
| `setBoostTargetAdd(v)` / `setBoostTargetMult(v)` / `setBoostDutyAdd(v)` | BoostController closed/open loop (EFI_BOOST_CONTROL) |
| `setIdleAdd(v)` | IdleController offset (EFI_IDLE_CONTROL) |
| `setParkNeutral(boolean)` | Reports P/N state for the signed `idleParkNeutralOffset` running idle correction; false initially and after Lua reset (EFI_IDLE_CONTROL) |
| `setIdleRpm(rpm)` | Flattens the whole `cltIdleRpm` curve to a fixed target |
| `setEtbAdd(pct)` / `setEwgAdd(pct)` | Electronic throttle / wastegate position adjustment |
| `setEngineTorque(nm)` | Publishes torque estimate to `engineState.lua.engineTorque` (not on F4) |

#### F7 injector deadtime experiments

With EFI_ENGINE_CONTROL and EFI_LUA, STM32F7 (and host unit tests) registers
the following in lua_hooks_ext.cpp. The InjectorDeadtimeAutotune module owns
temporary controls; the estimator remains in Lua. Other MCU families have no
bindings or control state. These hooks do not modify or burn calibration.

| Hook | Contract |
| --- | --- |
| `setInjectionModeOverride("sequential" / "batch" / nil)` | Boolean acceptance; nil releases. Requested changes drain queued pulses, recalculate mass and rebuild mappings before scheduling resumes |
| `getInjectionMode()` | Returns effective mode string and a transitioning boolean; wait for false, then allow exhaust/STFT settling |
| `setInjectorDeadtimeAdd(ms)` | Boolean acceptance; finite -2..2 ms, zero releases. All primary-stage injectors; clamps adjusted deadtime at zero after voltage/pressure interpolation |
| `getFuelTrim(bank)` | 1-based bank; returns table with correction (multiplier, 1 = neutral), correctionState, learningState, cell (1-based), saturated and enabled |
| `setLtftLearningDisabled(bool)` | EFI_LTFT_CONTROL only; boolean acceptance; inhibit learning while continuing to apply existing trims |

Each setter independently expires after one second without renewal. Call active
setters from onTick at 10 Hz or faster. A mode release may wait for outstanding
pulse closures and the next fast callback; never rewrite queued output pointers.
No new pulses are scheduled while draining, so a transition can briefly interrupt
fuel delivery and is unsuitable as a high-frequency modulation API.

Nonzero/active requests require a running engine with phase sync, more than one
cylinder, a sequential/batch base tune, staging disabled, INJ_None, and the injector
flow linearization table disabled. Invalid arguments raise Lua errors; unsupported
runtime conditions return false. Cranking retains precedence. Eligibility loss,
engine stop, configuration change or Lua reset clears requests. Uncaught load,
onTick, CAN callback or interactive execution errors revoke requests and latch out
rearming until Lua reset/reload.

Fuel trim cell order is idle, overrun, power, cruise (1..4). State codes follow
stft_state_e; zero is enabled. `enabled` requires both correction and learning
enabled, a running engine, valid lambda and a positive finite correction. Also
reject saturated trims and changed operating conditions when measuring a pair.
Generic getOutput skips the STFT array; generic setCalibration skips injection
mode enums and the voltage/pressure deadtime table. Table writeback is not part
of these hooks. Existing getOutput("m_deadtime") reads prepared primary deadtime.

### 5. Cut / disable / start-stop controls

Hard on/off overrides, mostly booleans in `engineState.lua`, consumed by LimpManager and
controllers (see [protection_system.md](protection_system.md)).

| Function | Notes |
|------------------------------|-------|
| `setIgnDisabled(bool)` / `setFuelDisabled(bool)` | Full ignition/fuel cut |
| `setDfcoDisabled(bool)` | Suppress deceleration fuel cut-off |
| `setEtbDisabled(bool)` | Disable electronic throttle control |
| `setAcDisabled(bool)` | AcController override; `getTimeSinceAcToggleMs()` reads time since last AC state change |
| `stopEngine()` | `doScheduleStopEngine(StopRequestedReason::Lua)` |
| `isEngineStopRequested()` | Queries LimpManager shutdown controller |
| `startCrankingEngine()` | Start-stop starter engagement (`doStartCranking`): clears the previous stop window on a new engagement and sets starterControlPin high, released by slow callback on running RPM or after `startCrankingDuration` seconds (default 3). Repeated calls while engaged neither extend the timeout nor clear a newer stop request; other protection cuts remain active. |
| `restartEtb()` | Re-runs ETB init - workaround for LuaSensor acting as PPS (marked "todo: smarter implementation") |
| `mcu_standby()` | STM32 standby/low-power (opt-in `LUA_STM32_STANDBY` boards; criticalError if called in the first 3s after boot) |

### 6. Outputs: PWM, DAC, power control

| Function | Notes |
|------------------------------|-------|
| `startPwm(idx, freqHz, duty)` | 0-based channel into `luaOutputPins[LUA_PWM_COUNT]` (8); freq clamped 1..1000 Hz, duty 0..1; Lua error on bad duty |
| `setPwmDuty(idx, duty)` / `setPwmFreq(idx, freqHz)` | Update a running channel (same clamps) |
| `setDacVoltage(ch, volts)` | EFI_DAC only |
| `initI2cDac(scl, sda, ldac)` | LUA_I2C_DAC; reserve software I2C and address-programming pins |
| `setI2cDac(ch, code)` / `setI2cDacVoltage(ch, volts[, vdd])` | LUA_I2C_DAC; external MCP4728 channels **1..20**, boolean write result |
| `setI2cDacChannels(address, a, b, c, d)` | LUA_I2C_DAC; chip address bits **0..4**, four raw 12-bit codes |
| `hellenEnablePower()` / `hellenDisablePower()` | Hellen board EN (megasquirt-style main relay) control (HW_HELLEN) |
| `getFan()` / `getFan2()` / `getAcRelay()` | Read back logic state of firmware-driven relay outputs |
| `getGpPwm(idx)` | Read GPPWM output duty (refreshed by periodicSlowCallback; not on F4 unless `WITH_LUA_GET_GPPWM_STATE`) |
| `getEtbTarget()` | Current ETB controller target |

### 7. CAN bus

TX, RX filtering with optional per-filter callbacks, and bus management.
RX plumbing lives in `lua_can_rx.cpp` (frames are queued from the CAN thread and delivered
on the Lua tick).

| Function | Notes |
|------------------------------|-------|
| `txCan(id, data)` or `txCan(bus, id, isExt, data)` | bus is 1-based; data is a Lua array table, max 8 bytes; validates std/ext ID range |
| `canRxAdd(id)` / `(bus, id)` / `(id, callback)` / `(bus, id, callback)` | Exact-ID RX filter |
| `canRxAddMask(id, mask)` + bus/callback variants | Masked RX filter |
| `onCanRx(bus, id, dlc, data)` | Global fallback callback for filters registered without their own callback |
| `global_can_data` | Legacy/workaround table populated when `enableCanRxWorkaround()` was called |
| `canSetBaud(bus, baud)` | Runtime baud change; Lua error on failure |
| `canSetListenMode(bus, listenOnly)` | Runtime listen-only (silent mode: no ACK, no TX) toggle, switchable back and forth; overrides `can*ListenMode` config until reboot; Lua error on failure |
| `getCanBaudRate(busIdx)` | Configured baud for CAN1/CAN2 (not on F4) |
| `enableCanTx(bool)` | Master `engine->allowCanTx` gate |
| `disableExtendedCanBroadcast()` | Turns off rusEFI periodic broadcast (not on F4) |
| `getCanRxDropped()` | RX queue overflow counter (not on F4) |
| `crc8_j1850(data, trimLength)` | SAE J1850 CRC8 helper for building CAN payloads |

### 8. Configuration, calibration and script tables

Access to persistent config (see "Generated configuration layout" in CLAUDE.md) plus the
script-owned curves/tables/settings defined in the tune.

| Function | Notes |
|------------------------------|-------|
| `getCalibration("name")` / `setCalibration("name", value, incrementVersion)` | By-name access to any scalar `engine_configuration_s` field (string lookup - not for hot paths) |
| `burnConfig()` | Requests config write to flash (registered under `CMD_BURNCONFIG` = "burnconfig") |
| `getGlobalConfigurationVersion()` | Detect external tune changes |
| `curve(humanIdx, x)` | 1-based script curve lookup |
| `table3d(humanIdx, x, y)` | 1-based script table lookup |
| `findCurveIndex("name")` / `findTableIndex("name")` | Name -> 1-based index, nil if missing; resolve once at script start |
| `findSetting("name", default)` | Named `scriptSetting[]` value with fallback |
| `getOutput("name")` | By-name read of any TS output channel (calls `updateTunerStudioState()` for freshness) |

### 9. Engine/vehicle state queries

| Function | Notes |
|------------------------------|-------|
| `getEngineState()` | 0=stopped, 1=cranking/spinning-up, 2=running |
| `getAirmass([mode])` | Cylinder airmass from configured (or explicitly given) airmass model |
| `getTorque()` | Interpolates the page-4 torque table at current RPM/TPS (not on F4) |
| `getTimeSinceTriggerEventMs()` | Staleness of last trigger event |
| `isFirmwareError()` | `hasFirmwareError()` |
| `getConsumedGrams()` / `getConsumedGramsRemainder()` / `getConsumptionGramPerSecond()` / `resetOdometer()` | TripOdometer fuel-consumption tracking (EFI_VEHICLE_SPEED + MODULE_ODOMETER) |
| `secondsSinceTsActivity()` | Time since last TunerStudio/console request - e.g. for "PC connected" logic |
| `getTsButtonCount(humanIdx)` | Click counter of TS Lua buttons 1..LUA_BUTTON_COUNT (10) |

### 10. Helper classes (luaaa bindings)

Stateful objects constructed from Lua; all statically allocated per luaaa placement-new.

| Class | Methods | Notes |
|-------------|----------------------------------------|-------|
| `Timer` | `reset()`, `getElapsedSeconds()` | Binds firmware `Timer` |
| `Sensor` | see category 2 | |
| `Pid` | `get(target, input)`, `setOffset()`, `reset()` | `LuaPid`, ctor(p, i, d, min, max) (WITH_LUA_PID) |
| `IndustrialPid` | + `setDerivativeFilterLoss()`, `setAntiwindupFreq()` | `LuaIndustrialPid` |
| `Biquad` | `filter(x)`, `configureLowpass(sampleRate, cutoffHz)` | `LuaBiQuad` filter |
| `SignalDebounce` | `set(bool)`, `get()` | ctor(period sec) |

### 11. Framework, utility and test/dev hooks

| Function | Notes |
|------------------------------|-------|
| `onTick()` | Script-defined callback the Lua thread invokes periodically |
| `setTickRate(hz)` | Frequency of `onTick` (registered in lua.cpp; clamped 1..2000 Hz; above 150 Hz firmware suggests `enableCanRxWorkaround`) |
| `print(msg)` | `efiPrintf` with "LUA:" prefix (also plain printf under unit tests) |
| `interpolate(x1, y1, x2, y2, x)` | Two-point linear interpolation |
| `random()` | tinymt32 float 0..1 (fixed seed) |
| `selfStimulateRPM(rpm)` | Enable/adjust internal trigger stimulator; rpm<1 disables (EFI_EMULATE_POSITION_SENSORS, dev/testing) |
| `advanceTimeUs(us)` | Unit-test-only virtual clock advance |
| `testFunc()` | Invoked by `luaCommand` unit-test path in lua.cpp |

## Adding a new hook

1. Prefer `lua_hooks.cpp` (or `lua_hooks_util.cpp` for pure utilities); board-specific hooks
   go in a board `boardConfigureLuaHooks()` override.
2. Guard with the same `EFI_*` flag as the subsystem you touch; consider excluding from
   STM32F4 if flash-hungry and non-essential (see `#if !defined(STM32F4)` precedent).
3. Follow the indexing conventions above (1-based only for TS-facing entities, convert via
   `HUMAN_OFFSET`).
4. Return nil for invalid runtime input, `luaL_error` for programming errors.
5. Unit-testable hooks: keep them outside `#if !EFI_UNIT_TEST` and add coverage in
   `unit_tests/tests/lua/` (`testLuaExecString` style); update the wiki page for users.
