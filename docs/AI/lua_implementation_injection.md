# Lua implementation injection points

Source review: 2026-10-14, revision `a85e63ec148`. This report describes the
checked-out firmware, not a guarantee that every board enables every hook.
See [Lua API inventory](lua_scripting.md) for the wider scripting API.

**Air mass has an explicit Lua model selection. Idle supports injected control
contributions, but has no equivalent whole-controller Lua selection.** Other
useful seams are fuel mass, ignition advance, boost target/duty, throttle and
wastegate targets, sensor providers, CAN protocols, and dedicated Lua outputs.

## What counts as implementation injection?

There are four different capabilities:

1. **Model/provider replacement:** firmware consumes a result computed by Lua
   through an explicit provider interface (air mass, registered sensors).
2. **Calculation-stage substitution or adjustment:** Lua publishes an adder,
   multiplier, or target contribution that native control code consumes.
3. **Policy/input injection:** Lua supplies activation decisions, virtual
   switches, or cut requests; the native subsystem implements the response.
4. **Independent implementation:** Lua owns a CAN protocol or an output pin and
   implements the logic itself, using firmware transport/PWM primitives.

There is no general Lua equivalent of replacing an `EngineModule`, C++ virtual
method, trigger decoder, or scheduler. Most control hooks store values; the
native controller does not synchronously call a Lua function for its next result.

## Subsystem inventory

| Subsystem | Lua surface | Extent of replacement |
| --- | --- | --- |
| Cylinder air mass / engine load | `setAirmass(grams, loadPercent)`, `getAirmass([mode])`; `fuelAlgorithm = LM_LUA` | Explicit replacement of `AirmassModelBase` result. Native fuel calculation continues downstream. |
| Sensor acquisition / conversion | `Sensor.new(name)`, `:set`, `:invalidate`, `:setTimeout` | Lua can provide a named sensor from CAN, auxiliary voltage, or a custom calculation, if that registry slot is free. |
| Idle air control | `setIdleAdd`, `setIdleRpm`, `setParkNeutral` | Running open-loop contribution and target/input control. Native phase logic, optional PID, actuator handling, and some table-only branches remain. |
| Fuel quantity | `setFuelMult`, `setFuelAdd` | Can substitute the pre-trim injection mass by setting multiplier to zero and supplying the mass with the adder. Native downstream fueling remains. |
| Ignition advance | `setTimingMult`, `setTimingAdd` | Can substitute the advance at `getWrappedAdvance()` by using multiplier zero. Later corrections and spark scheduling remain native. |
| Boost | `setBoostTargetMult`, `setBoostTargetAdd`, `setBoostDutyAdd` | Custom target and feed-forward duty contributions; native enable/validity gates, optional PID, output limits and actuator handling remain. |
| Electronic throttle / electric wastegate | `setEtbAdd`, `setEwgAdd`, `setEtbDisabled` | Position-target additions or disable request. Does not replace the native motor position controller. |
| Launch, shift reduction, traction-like policies, antilag | Spark-skip ratios, ignition/fuel cuts, launch/rolling-idle/torque-reduction triggers | Lua can implement the decision logic, using native cut/scheduling machinery; trigger-only hooks retain native strategy behavior. |
| Driver switches, A/C, start-stop | Virtual switch setters, `setAcDisabled`, `startCrankingEngine`, `stopEngine` | Policy/input injection into existing native state machines. |
| GPPWM, blend/table inputs, nitrous arming | `setLuaGauge`; `LuaGauge1/2` GPPWM channels; nitrous Lua-gauge selection | Script-computed inputs to configurable native logic, not a replacement of those controllers. |
| CAN integration | `canRxAdd`, `canRxAddMask`, callbacks, `txCan` | Script owns message encoding/decoding and protocol state; native CAN transport remains. |
| Dedicated actuators / auxiliary outputs | `startPwm`, `setPwmDuty`, `setPwmFreq`, optional DAC hooks | Script can own a custom control algorithm and dedicated output. This bypasses, rather than injects into, a native controller. |

The registrations and their build guards are in
[lua_hooks.cpp](../../firmware/controllers/lua/lua_hooks.cpp), principally
`configureRusefiLuaHooks()`. Details and consumer paths follow.

## Air mass: the explicit algorithm replacement

[`getAirmassModel()`](../../firmware/controllers/algo/fuel_math.cpp) selects
speed-density, MAF, alpha-N, or `LM_LUA` (enum value 3). The Lua selection returns
the static [`LuaAirmass`](../../firmware/controllers/algo/airmass/lua_airmass.h)
object. Its `getAirmass()` simply returns the last published `AirmassResult`.

`setAirmass(airmass, engineLoadPercent)` writes:

- Cylinder air charge in **grams per cylinder**, clamped to 0..10 g.
- Engine load in percent, clamped to 0..1000%.

This replaces the charge/load estimate, not the entire fueling system.
`getBaseFuelMass()` still computes normalized filling and airflow telemetry,
applies the ignition-load override selection, and converts air to fuel through
the native fuel computer. Cranking/running corrections and injection scheduling
remain downstream; see [fueling_system.md](fueling_system.md).

Lua can also call `getAirmass(mode)` to reuse a built-in model and implement a
custom blend or correction. This returns only cylinder mass, not the load field.
When the configured strategy is Lua, argument-free `getAirmass()` reads back the
last Lua value; it does not invoke speed-density implicitly.

The initial result is 0 g and 100% load. There is no age check, automatic fallback
to another model, or clear of this static object in `Engine::resetLua()`.

## Idle: a useful seam with phase-dependent limits

[`IdleController::getRunningOpenLoop()`](../../firmware/controllers/actuators/idle_thread.cpp)
adds `luaAdd` to the native table value alongside fan, A/C, Park/Neutral, antilag,
and taper contributions, then clamps the running result. `setIdleAdd()` writes
that contribution in percentage points of the internal idle demand.

The exact limits matter when implementing a Lua idle controller:

- Cranking returns the cranking table directly, bypassing `luaAdd`.
- The dedicated coasting-table branch also bypasses `luaAdd`.
- The crank-to-idle transition blends the running result with the cranking value.
- Automatic mode adds native closed-loop correction after open-loop computation.
- With modeled-flow idle enabled, the demand is converted to air mass and flow,
  split between timing and airflow, and then converted to valve position. An
  adder is therefore not necessarily a direct valve-duty change. Modeled-flow
  idle also forces automatic mode.
- `applyIACposition()` still handles the native PWM/stepper/DC/ETB output path.

A custom Lua PID can publish through `setIdleAdd()` while a conventional idle
configuration disables native closed-loop correction and accounts for native
base/additional contributions. That is a partial substitution, with the above
phase behavior. There is no `setIdlePosition()` hook or Lua idle-controller enum.

`setIdleRpm(rpm)` **rewrites every value of `config->cltIdleRpm`**. It is not an
ephemeral target override and a Lua reset does not restore the old curve. Native
A/C target selection and return ramp can still alter the effective target.
`setParkNeutral(bool)` supplies an input for the configured signed idle offset.
See [idle_system.md](idle_system.md).

## Fuel and ignition: substitution at a calculation stage

The actual formulas are:

```text
EngineState::fastCallback():
    untrimmedInjectionMass = getInjectionMass(rpm) * lua.fuelMult + lua.fuelAdd

IgnitionState::getWrappedAdvance():
    angle = getAdvance(rpm, engineLoad) * luaTimingMult + luaTimingAdd
    wrapAngle(angle)
```

Sources: [engine2.cpp](../../firmware/controllers/algo/engine2.cpp),
[fuel_math.cpp](../../firmware/controllers/algo/fuel_math.cpp), and
[ignition_state.cpp](../../firmware/controllers/algo/ignition/ignition_state.cpp).

For finite native values, multiplier zero plus a Lua adder substitutes the value
at that point. The native calculation is still evaluated; this does not bypass
its input requirements or errors.

**`setFuelAdd()` uses grams per injection, not milligrams.** The binding stores
the number without conversion; `getInjectionMass()` returns grams and
`engine_state.txt` labels the field `g`. Injector staging, trims, wall wetting,
injector characterization and scheduling still follow the mass calculation.
This is not a raw pulse-width or injector-edge API.

`setTimingAdd()` uses degrees. Knock and limiting retard can still be applied
after `getWrappedAdvance()`, and per-cylinder timing/scheduling remain native.
There is no corresponding Lua hook to replace dwell calculation or schedule a
coil edge at a crank angle. See [ignition_system.md](ignition_system.md).

Some builds omit `setTimingMult` (`DISABLE_LUA_SET_TIMING_MULT`) or `setFuelAdd`
(`DISABLE_LUA_SET_FUEL_ADD`), so these substitutions are build-dependent.

## Boost, throttle, and wastegate

[`BoostController`](../../firmware/controllers/actuators/boost_control.cpp)
applies target multiplier/addition after its target table and blends, but before
temperature target adders. Target additions are in the MAP target's units (kPa).
Multiplier zero substitutes that stage's target, not the final target after
temperature correction.

`setBoostDutyAdd()` adds percentage points after the open-loop table's
temperature multipliers; blend and gear adders follow. A custom Lua boost
regulator can use this seam with native boost in open-loop mode and other
contributions configured appropriately. Native RPM/TPS/MAP activation gates,
invalid-input safe duty, enable state and 0..100% output clamping still apply.
In closed-loop mode the native PID also contributes. The result drives either
the boost solenoid or the electric wastegate position path.

[`EtbController`](../../firmware/controllers/actuators/electronic_throttle.cpp)
adds `setEtbAdd()` after the pedal-map/idle target combination; later native
adjustments and constraints still apply. `setEwgAdd()` adds to the wastegate
position supplied by boost control. Both additions expire after **0.2 seconds**
without refresh. Native motor control and fault/disable logic remain in charge.
`setEtbDisabled()` disables control; it does not hand its pins to Lua.

## Sensor providers and configurable Lua-gauge inputs

The `LuaSensor` binding in
[lua_hooks.cpp](../../firmware/controllers/lua/lua_hooks.cpp) registers a
`StoredValueSensor` under a named `SensorType`. This allows custom acquisition,
conversion, filtering, or CAN decoding to feed ordinary firmware consumers.
Registration fails if the type already has a provider: configure ownership
before creating the Lua sensor. Keep the Lua object referenced while in use;
garbage collection unregisters it.

The default freshness timeout is **100 ms**, adjustable with `:setTimeout(ms)`.
`:set(value)` refreshes validity; `:invalidate()` explicitly removes validity.
`:setRedundant(true)` declares a property to consumers, rather than implementing
redundancy checks for the script. See [sensors_system.md](sensors_system.md).

`setLuaGauge(1..8, value)` is also a sensor-provider path, **not just telemetry**:
the pre-registered gauges have a 5-second timeout in
[init_aux.cpp](../../firmware/init/sensor/init_aux.cpp).
[readGppwmChannel()](../../firmware/controllers/actuators/gppwm_channel_reader.cpp)
exposes gauges 1 and 2 to callers such as boost open-loop axes and blend logic.
[NitrousController](../../firmware/controllers/algo/nitrous_controller.cpp)
can use any of the eight gauges for threshold-based arming when configured for
`LUA_GAUGE`; its other arming/activation checks remain native.

## Policy injection and independent implementations

| Capability | Native consumer / boundary |
| --- | --- |
| `setSparkSkipRatio`, `setSparkHardSkipRatio` | `EngineState::updateSparkSkip()` combines requests with native launch/traction/antilag logic; native spark code executes the skips. |
| `setIgnDisabled`, `setFuelDisabled` | [LimpManager](../../firmware/controllers/limp_manager.cpp) accepts Lua cut requests alongside other cut sources. Clearing Lua's request does not clear other protection requests. |
| `setDfcoDisabled` | [DfcoController](../../firmware/controllers/algo/fuel/dfco.cpp) can be inhibited by Lua. |
| `setLaunchTrigger`, `setRollingIdleTrigger` | [Launch](../../firmware/controllers/algo/launch_control.cpp) and [antilag](../../firmware/controllers/algo/antilag_system.cpp) use these inputs in their respective Lua activation modes. These setters currently take integer arguments. |
| `setTorqueReductionState` | [ShiftTorqueReductionController](../../firmware/controllers/algo/shift_torque_reduction_controller.cpp) uses the request when its corresponding activation condition is selected. |
| Clutch/brake/A/C request setters | [Engine input accessors](../../firmware/controllers/algo/engine.cpp) use Lua values when the corresponding physical-input path is not selected. |
| `setAcDisabled` | [AcController](../../firmware/controllers/actuators/ac_control.cpp) accepts an inhibit; normal native conditions still govern engagement. |
| `startCrankingEngine`, `stopEngine` | Requests to native starter/shutdown logic, not replacements for starter timing or protection. |

For an independent implementation, `onTick`, sensor reads, script curves/tables,
`Pid`/`IndustrialPid` and `Biquad` provide the computation tools. Dedicated Lua
PWM channels use `luaOutputPins[]`, zero-based channel indices, duty 0..1, and
frequency clamped to 1..1000 Hz. A constant duty of 0 or 1 provides an on/off
output. Allocate pins to Lua rather than concurrently assigning them to a
native actuator. DAC availability depends on the board/build.

CAN callbacks and `txCan()` support custom sensor, dashboard, body-controller,
transmission, or gateway protocols. RX is queued and handled on the Lua thread;
it is not an interrupt-context Lua callback. Existing examples include
[MoTeC LTC input](../../firmware/controllers/lua/examples/motec-ltc-input.lua),
[Nissan TCU integration](../../firmware/controllers/lua/examples/nissan-tcu.lua),
[four-speed TCU](../../firmware/controllers/lua/examples/TCU-4-speed.txt), and
[launch policy](../../firmware/controllers/lua/examples/launch_control.lua).
These demonstrate patterns, not validation of a particular vehicle setup.

## Boundaries and lifecycle

- No dedicated shared Lua replacement hooks were found for VVT control, native
  fan/fuel-pump algorithms, injector characterization, dwell, crank/cam decoding,
  or angle scheduling. A custom dedicated PWM implementation is a different
  ownership path and does not inherit the replaced subsystem's native behavior.
- `setEngineTorque()` publishes a field, but the reviewed source does not wire
  that field into a native torque-control strategy. It should not be described
  as a torque-model replacement. `getTorque()` independently reads a native table.
- `setCalibration()` changes supported named configuration fields. It is not a
  general function-replacement API; configuration version notifications and
  reinitialization requirements still apply. Lua reset does not undo tune edits.
- All hooks require Lua support, with additional subsystem/board guards. Airmass
  bindings and PWM hooks are excluded from unit-test builds; ETB adjustment hooks
  require `EFI_PROD_CODE`. Check the registration guards for the target build.
- [lua.cpp](../../firmware/controllers/lua/lua.cpp) defaults to a 5 ms sleep per
  iteration (nominal 200 Hz); `setTickRate()` permits 1..2000 Hz. CAN handling,
  interactive work and script execution occur before that sleep, so this is not
  a guaranteed execution frequency or crank-synchronous callback.
- An `onTick()` error is logged and the loop continues. It does **not**
  automatically reset all published values. Fuel/ignition/idle/boost adjustments
  have no general freshness timeout. Sensors and ETB/EWG additions have their
  own timeout rules described above.
- [Engine::resetLua()](../../firmware/controllers/algo/engine.cpp) resets selected
  adjustments on VM restart, including fuel/ignition, boost, idle and P/N state.
  It is not universal: static Lua airmass, calibration edits, and fields outside
  its reset list (including spark-skip and launch/antilag trigger fields) should
  not be assumed cleared. Normal VM teardown also unregisters Lua sensors,
  deinitializes Lua pins and clears CAN subscriptions.
- `boardConfigureLuaHooks()` and `configureRusefiLuaHooksExt()` are C++ extension
  sites for adding APIs in a firmware build, not script-loadable subsystem slots.

## Review and validation

This is a source-traced inventory: registrations were followed to their native
consumers, including control formulas, bypass branches, units, build guards,
timeouts and reset paths. No firmware behavior was changed and no hardware
validation or test execution was performed for this documentation change.

Existing coverage useful for future implementation work includes
[Lua bindings/sensors](../../unit_tests/tests/lua/test_lua_hooks.cpp),
[idle control](../../unit_tests/tests/test_idle_controller.cpp),
[ETB](../../unit_tests/tests/actuators/test_etb.cpp), and boost
[target corrections](../../unit_tests/tests/actuators/boost/test_closed_loop_adders.cpp)
and [duty corrections](../../unit_tests/tests/actuators/boost/test_open_loop_multipliers.cpp).
Native controller tests do not by themselves establish that a production-only
Lua binding is present or that its real-time behavior is suitable.
