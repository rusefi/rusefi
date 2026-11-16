# Acceleration Enrichment and Transient Fueling

`accelEnrichmentMode` selects **four mutually exclusive strategies**:

| Value / enum | TunerStudio name | Correction |
|---|---|---|
| `0` / `AE_MODE_MS_ADDER` | MS Adder | TPS-based extra duration, converted to fuel mass using primary injector flow. |
| `1` / `AE_MODE_PERCENT_ADDER` | Percent Adder | TPS-based extra fuel proportional to the calculated injection mass. |
| `2` / `AE_MODE_PREDICTIVE_MAP` | MAP Prediction | Temporarily raises the MAP used by the speed-density air-mass calculation, then blends back to measured MAP. |
| `3` / `AE_MODE_LUA` | Lua | Script supplies extra fuel mass per cylinder per engine cycle using `setAccelEnrichment(grams)`. |

**Wall wetting is independent of this selector** and can operate with any of these modes. Its simple/complex model selector (`complexWallModel`) is a separate choice. MAP Prediction is triggered by TPS movement, not a MAP derivative.

## Where the corrections enter the pipeline

The [fueling overview](fueling_system.md) is conceptual; the relevant execution order is:

```text
SpeedDensityAirmass::getMap()                  <- MAP Prediction, if selected
  -> air mass / base fuel / running or cranking fuel
  -> getInjectionMass()                       <- MS Adder, Percent Adder or Lua
  -> Lua fuel adjustment, bank/cylinder trims -> stored injectionMass[cylinder]
  -> InjectionEvent::onTriggerTooth()
       -> WallFuel::adjust()                 <- independent fuel-film correction
       -> split between injector stages
       -> mass-to-duration conversion and scheduling
```

MAP Prediction acts in `SpeedDensityAirmass::getMap()` with a valid MAP sensor. Selecting it disables the two adders globally; an air-mass path that does not call this method does not gain predictive enrichment. Failed MAP uses the ordinary RPM/TPS fallback table, without the prediction blend.

## Shared TPS event detection

[tag:tps_rate_detector] `Engine::periodicFastCallback()` feeds `Tps1` to `TpsAccelEnrichment::onNewValue()` at 200 Hz, **before** the fuel math, so the adders and MAP Prediction act on the same sample. An invalid TPS calls `onInvalidTps()`, which clears the history and ends any event.

- **Event confirmation** (`config->tpsEventConfirmation`, 1 to 3 samples, default 2): each sample the detector uses is the median of the last 2N-1 raw samples, so a TPS change must last N samples to be seen. A shorter spike disappears in both directions; a stricter "N consecutive samples above the rate" rule would turn the return of a spike into a false closing, and would never confirm a held step with N = 3 because a step stays above the rate for only two samples (a TPS received over CAN at a low rate moves in such steps). Each extra sample delays every event by 5 ms. Zero (a tune that predates the setting) is repaired to 2 by `applyDefaultsOrFixAfterBurn()`. Tests: `TpsEventConfirmation.*`.
- The TPS rate `tpsRate` (%/s) is measured on these samples against the oldest of the two previous ones, about 10 ms back. The first samples after a reset, until the median window is full and one more sample is available, or after a gap longer than 100 ms, have no rate. A sample closer than 2 ms to the previous one is ignored, so extra fast-callback invocations (first RPM, trigger reconfiguration) do not shorten the window.
- An **opening event** starts when `tpsRate > tpsAccelRateThreshold`. `tpsFrom` is the (filtered) TPS at the start of that rate window, `tpsTo` follows the highest TPS reached, `deltaTps = tpsTo - tpsFrom` and `peakTpsRate` is the highest rate. The start latches one `isAccelEventTriggered()` for MAP Prediction. The event (`isAboveAccelThreshold`) lasts while the rate stays above the threshold plus `tpsAccelHoldTime`.
- A **closing event** mirrors this with `tpsRate < -tpsDecelRateThreshold`: `tpsTo` follows the lowest TPS, `deltaTps` is negative and `peakTpsRate` is the most negative rate. It sets `isBelowDecelThreshold`, drives the adder deceleration path and ends an active MAP Prediction.
- Opening and closing events are exclusive: a new one ends the other. A zero threshold disables that direction; `tpsRate` is still reported.
- `tpsAccelHoldTime` (seconds) also enables the adders: zero disables MS/Percent Adder. MAP Prediction and Lua do not use it.
- Noise: the 10 ms window turns TPS noise into rate. With independent noise of standard deviation s, the rate noise is about 141 * s %/s: at 100 %/s and one-sample confirmation, s = 0.2 % gives an event every few tens of seconds and s = 0.3 % a few per second; two-sample confirmation cuts that by roughly 100 times. The existing TPS smoothing (`tpsExpAverageAlpha`) is not a substitute: it advances on every `Sensor::get()` of the TPS, so its time constant depends on how often other code reads the TPS, and its fast-change reset compares consecutive reads, which during a real stomp rarely differ by its 10 % threshold.
- The thresholds replaced per-50 ms-sample settings. Tunes from earlier firmware do not carry them over: the renamed settings start from their defaults (100 %/s, 100 %/s, 0 s) and the TPS/TPS table axes now mean event start TPS and highest TPS reached, so existing tables need recalibration.

The fractional accumulator advances in `onEngineCycleTps()` from the trigger callback at engine-cycle boundaries, independently of TPS sampling. On engine stop, `resetAE()` clears TPS history, threshold and event state, and fractional fuel. The first TPS sample after a reset establishes a baseline; it does not create an acceleration step from zero.

## MS Adder and Percent Adder

Both modes share `TpsAccelEnrichment::getTpsEnrichment()`:

1. Return zero if `tpsAccelHoldTime` is zero, RPM is below `cranking.rpm`, or the TunerStudio VE-tuning detector is active (when compiled in).
2. On acceleration, interpolate `tpsTpsAccelTable` at the event start TPS (X, `tpsFrom`) and the highest TPS reached (Y, `tpsTo`). Despite their names, `tpsTpsAccelFromRpmBins` and `tpsTpsAccelToRpmBins` contain TPS percentages, not RPM.
3. On deceleration, use `deltaTps * tpsDecelEnleanmentMultiplier`, where `deltaTps` is the total closing of the event; otherwise start with zero extra fuel.
4. Apply the optional fractional accelerator-pump model (`tpsAccelFractionPeriod`, `tpsAccelFractionDivisor`). It retains an accumulated portion and spreads it across engine cycles. With a single unit input, period 1/divisor 2 produces `0.5, 0.25, 0.125, ...`; period 3/divisor 1 produces three portions of `1/3`.
5. Multiply by the RPM curve `tpsTspCorrValues` / `tpsTspCorrValuesBins`, then the CLT/ethanol multiplier `flexAeMult` when enabled.

`getInjectionMass()` interprets the resulting value `E` according to the mode. Let `D = getInjectionModeDurationMultiplier()` and `M = cycleFuelMass * D`:

| Mode | Extra mass added to `M` |
|---|---|
| MS Adder | `getFuelMassForDuration(D * E)` = `D * E * primaryInjectorFlowGramsPerSecond * 0.001` |
| Percent Adder | `M * D * E` |

There is **no division by 100** in Percent Adder. With sequential injection (`D = 1`), `E = 0.20` adds 20%; `E = 20` adds 2000%. The table has scale 1, so this is also its stored value when the other corrections are neutral. For other injection modes, the additional `D` matters: ordinary multi-cylinder batch uses `D = 0.5`, simultaneous uses `1 / cylindersCount`, and single-point or single-cylinder batch uses `1`.

The MS Adder conversion uses injector flow without adding deadtime; final injector characterization happens downstream. Changing adder modes reinterprets the same table values. The TunerStudio table title and TPS AE log name say "ms or fraction"; table, output and gauge units use `ms/frac`. These mean milliseconds in MS Adder and a fraction in Percent Adder. `tpsAccelFuel` reports `E`, before injection-mode scaling; its binary packing scale is unchanged.

## MAP Prediction

`SpeedDensityAirmass::getPredictiveMap()` uses `mapEstimateTable` with `mapEstimateRpmBins` and `mapEstimateTpsBins` to predict MAP in kPa. This is also the table used for MAP-sensor fallback. The blend duration comes from `predictiveMapBlendDurationValues` versus RPM (`predictiveMapBlendDurationBins`), in seconds.

The implementation:

1. Consumes a latched TPS acceleration event and starts prediction only when table MAP exceeds measured MAP and the retrigger lock is clear. It initially uses table MAP directly, starts the session timer and initializes the TPS peak from the current throttle position.
2. Blends from the event's prediction toward the **current measured MAP**:

   ```text
   effectiveMap = initialPredictedMap
                + (currentMeasuredMap - initialPredictedMap) * elapsed / blendDuration
   ```

3. If table MAP rises above the saved prediction as the throttle keeps opening, adopts the higher prediction and restarts the blend timer. The separate session timer continues running, so blend restarts cannot extend the total event time limit. The limit is interpolated at current RPM on each call. [tag:predictive_map_lifetime]
4. Ends prediction when the session duration expires, measured MAP catches up to the prediction/effective MAP, or the TPS detector reports a closing event (`isBelowDecelThreshold`). A lower TPS without a detected closing only lowers the estimate. It drains acceleration events while active. The retrigger lock clears when the shared `isAboveAccelThreshold` flag becomes false; a physical return to closed throttle is not required.

For example, predicted MAP 85 kPa and measured MAP 65 kPa initially use 85 kPa. Halfway through the blend, if measured MAP has risen to 75 kPa, effective MAP is 80 kPa.

The effective MAP feeds both the physical air-mass calculation and the speed-density load/VE path (including barometric normalization when configured). It does not replace the registered MAP sensor reading used elsewhere.

The TPS/TPS extra-fuel table, fractional settings, adder RPM correction and `flexAeMult` do not apply in this mode; `getTpsEnrichment()` returns zero immediately. That early return also means the adders' cranking-RPM and VE-tuning checks do not gate MAP Prediction itself.

## Lua implementation injection

[tag:lua_accel_enrichment] Select **Accel Enrichment Mode = Lua** and call
`setAccelEnrichment(grams)` from the script. The argument is finite, non-negative
extra fuel mass **per cylinder per engine cycle**, held until replaced. Zero
turns it off. Negative, NaN and infinite arguments clear the request and raise a
Lua error. Requires a build with `EFI_LUA` and `EFI_ENGINE_CONTROL`.

The script owns event detection, magnitude, decay, RPM and flex compensation.
Lua mode ignores the native TPS table, thresholds, hold time, fractional pump,
RPM multiplier and `flexAeMult`. MAP prediction is also inactive. The existing
MS Adder, Percent Adder and MAP Prediction values (0, 1, 2) keep their meaning;
the fourth choice fits the existing two-bit configuration field.

`getInjectionMass()` adds `D * luaExtraCycleMass` to the native injection mass,
where `D` is the injection-mode duration multiplier. For a four-cylinder engine,
`setAccelEnrichment(0.004)` adds 4 mg per sequential injection, 2 mg per batch
injection, or 1 mg per simultaneous injection. Single-point uses `D = 1`, matching
the native cycle-mass convention. This is a correction per engine cycle, not a
one-shot quantity consumed each time Lua calls the setter.

Refresh within **200 ms** (normally every `onTick`); an expired request contributes
zero. Lua VM reset, engine stop, `resetAE()` and any configuration-change notification
clear the request. No script means zero Lua AE, with no native-AE fallback.
Stopped/cranking operation, RPM below the cranking threshold, DFCO and active
VE-tuning detection suppress application. Normal protection cuts still govern
whether injection is scheduled.

Positive applied Lua AE resets the same acceleration timer used by STFT's
`noFuelTrimAfterAccelTime` holdoff. Zero, expired or suppressed requests do not
extend that timer. The separate float output `luaAccelFuel` (log label
`Fuel: Lua AE cycle mass`, grams) reports the applied cycle correction before
injection-mode scaling. `tpsAccelFuel` stays zero in Lua mode so the legacy
ms/fraction channel retains its units and packing. The standard `Engine` state's
acceleration bit follows applied Lua mass rather than native TPS flags.

The usual Lua fuel multiplier/adder, trims, staging, wall wetting, injector
characterization and scheduling remain downstream. `setFuelAdd()` is independent:
it adds grams per injection at a later stage and does not provide AE holdoff or
the 200 ms freshness rule. See [Lua injection inventory](lua_implementation_injection.md).

## Wall wetting and flex fuel

`WallFuelController::onFastCallback()` computes parameters, while each injection event owns a `WallFuel` state updated by `adjust()` on the trigger path. This corrects fuel **mass**, after cylinder/bank trims and before splitting between injector stages.

- Simple model: `wwaeTau` is the evaporation time constant in seconds; `wwaeBeta` is the fraction of injected fuel deposited on the wall.
- Complex model: CLT curves `wwTauCltValues` / `wwBetaCltValues`, multiplied by MAP curves `wwTauMapValues` / `wwBetaMapValues` when a MAP sensor is configured.
- Flex compensation multiplies tau and beta by `flexWwTauMult` and `flexWwBetaMult`. It remains applicable with MAP Prediction.
- Wall wetting is bypassed while cranking, below 100 RPM, or when effective tau or beta is below `0.01`.

The controller computes `alpha = exp(-120 / (rpm * tau))` using `expf_taylor`, and limits beta to alpha. For desired injection mass `M` and existing film mass `W`:

```text
commandedMass = max(0, (M - (1 - alpha) * W) / (1 - beta))
nextWallMass  = alpha * W + beta * commandedMass
```

Thus wall wetting can add or subtract commanded fuel as the film fills or releases fuel, without a TPS acceleration event.

All three flex multiplier tables share `flexTransientCltBins` and `flexTransientEthanolBins`. `getFlexTransientMult()` returns neutral `1.0` when `flexFuelTransientComp` is off or no ethanol sensor is configured. With a configured but invalid sensor, it uses 50% ethanol; invalid CLT uses 90 C. An interpolated multiplier at or below `0.01` also becomes `1.0`, so an all-zero table is neutral.

## Closed-loop interaction and diagnostics

`ShortTermFuelTrim` inhibits correction while `getTimeSinceAcell() < noFuelTrimAfterAccelTime`. The native adders reset that timer on acceleration or deceleration; MAP Prediction resets it when prediction starts; Lua resets it while positive extra mass is applied. The "Inhibit closed loop fuel after accel" setting is available in TunerStudio for all four modes.

| Inspect | Meaning / limitation |
|---|---|
| `engine` (log label `Engine`) | TunerStudio/MLV state byte: bits 0/1 running/cranking, 2/3 after-start/warmup, 4 acceleration (including active MAP Prediction or applied Lua AE), 5 TPS deceleration or DFCO; bits 6/7 unused. Native adder threshold flags and the signed applied adder cover detection and decay. Wall wetting is not represented. |
| `tpsFrom`, `tpsTo`, `deltaTps`, `tpsRate`, `peakTpsRate`, `isAboveAccelThreshold`, `isBelowDecelThreshold` | Current event: start TPS, extreme TPS, total change, current and peak rate, and the event flags. |
| `valueFromTable`, `extraFuel`, `accumulatedValue`, `cycleCnt` | Adder internals. Early-return paths can leave these diagnostic fields unchanged. |
| `tpsAccelFuel` | Native adder result; zero in MAP Prediction or Lua does not mean transient correction is inactive. |
| `luaAccelFuel` | Applied Lua extra mass in grams per cylinder per cycle, before injection-mode scaling; zero when inactive, expired or suppressed. |
| `wallFuelCorrection` | Signed wall-wetting correction in mg for injection event 0, logged as `Fuel: wall correction`. Positive adds fuel, negative subtracts fuel. |
| `wallFuel` | Estimated fuel film mass in mg for injection event 0, logged as `Fuel: wall amount`. |
| `fallbackMap`, `effectiveMap`, `isMapPredictionActive` | Table prediction, MAP used by speed density, and prediction state. |
| `predTimerResetCnt`, `mapPredEventOver` | Prediction starts and duration expirations; not every timer restart or early exit. |
| `flexAeMultiplier`, `flexWwTauMultiplier`, `flexWwBetaMultiplier` | Last evaluated flex multipliers; bypassed paths need not refresh them. |

`[tag:wall_fuel_telemetry]` The wall-model fields are the single source for TunerStudio,
SD logging and Lua `getOutput()`, all reporting mg. The model fields and
`WallFuel::adjust()`/`getWallFuel()` remain in grams. The field metadata specifies
scale 1000: TunerStudio applies it through the INI, SD logs store it in their
header, and Lua applies it on lookup. SD and Lua automatically use the field
metadata; no conversion occurs in the injection loop.
The former packed `wallFuelCorrectionValue` and `wallFuelAmount` channels have
been removed; update dashboards/scripts to `wallFuelCorrection` and `wallFuel`.
Earlier TunerStudio definitions exposed these retained names in grams.
The standard log labels remain unchanged.
Bypassed adjustments can leave the last correction unchanged.

## Source and existing coverage

- [Mode enum](../../firmware/controllers/algo/rusefi_enums.h), [mode configuration](../../firmware/controllers/algo/accel_enrichment.engineConfiguration.txt), [table definitions](../../firmware/integration/rusefi_config.txt), [TunerStudio panels](../../firmware/tunerstudio/tunerstudio.template.ini).
- [TPS detector and adders](../../firmware/controllers/algo/accel_enrichment.cpp), [mass conversion](../../firmware/controllers/algo/fuel_math.cpp), [predictive MAP](../../firmware/controllers/algo/airmass/speed_density_airmass.cpp).
- [Wall model](../../firmware/controllers/algo/wall_fuel.cpp), [injection integration](../../firmware/controllers/engine_cycle/main_trigger_callback.cpp), [flex multiplier helper](../../firmware/controllers/algo/flex_transient.h), [closed-loop gating](../../firmware/controllers/math/closed_loop_fuel.cpp).
- [TPS tests](../../unit_tests/tests/test_accel_enrichment.cpp): history, sampling versus engine-cycle updates, fractional portions, predictive-mode adder bypass and flex multiplier.
- The same file's `LuaAccelEnrichment` tests exercise the Lua binding, mass scaling, native-mode isolation, expiry, reset, DFCO/cranking and VE-tuning suppression. [TunerStudio tests](../../unit_tests/tests/test_tunerstudio.cpp) cover the Lua acceleration state bit.
- [Fuel math tests](../../unit_tests/tests/ignition_injection/test_fuel_math.cpp): `AirmassModes.PredictiveMap*` cover blending, catch-up, sensor fallback, the session duration cap across retriggers, rearming, and TPS peak reset between events.
- [Wall-wetting tests](../../unit_tests/tests/ignition_injection/test_fuel_wall_wetting.cpp): film math, injection-event scheduling and flex compensation.
