# Acceleration Enrichment and Transient Fueling

`accelEnrichmentMode` selects **three mutually exclusive strategies**:

| Value / enum | TunerStudio name | Correction |
|---|---|---|
| `0` / `AE_MODE_MS_ADDER` | MS Adder | TPS-based extra duration, converted to fuel mass using primary injector flow. |
| `1` / `AE_MODE_PERCENT_ADDER` | Percent Adder | TPS-based extra fuel proportional to the calculated injection mass. |
| `2` / `AE_MODE_PREDICTIVE_MAP` | MAP Prediction | Temporarily raises the MAP used by the speed-density air-mass calculation, then blends back to measured MAP. |

**Wall wetting is independent of this selector** and can operate with any of these modes. Its simple/complex model selector (`complexWallModel`) is a separate choice. MAP Prediction is triggered by TPS movement, not a MAP derivative.

## Where the corrections enter the pipeline

The [fueling overview](fueling_system.md) is conceptual; the relevant execution order is:

```text
SpeedDensityAirmass::getMap()                  <- MAP Prediction, if selected
  -> air mass / base fuel / running or cranking fuel
  -> getInjectionMass()                       <- MS Adder or Percent Adder
  -> Lua fuel adjustment, bank/cylinder trims -> stored injectionMass[cylinder]
  -> InjectionEvent::onTriggerTooth()
       -> WallFuel::adjust()                 <- independent fuel-film correction
       -> split between injector stages
       -> mass-to-duration conversion and scheduling
```

MAP Prediction acts in `SpeedDensityAirmass::getMap()` with a valid MAP sensor. Selecting it disables the two adders globally; an air-mass path that does not call this method does not gain predictive enrichment. Failed MAP uses the ordinary RPM/TPS fallback table, without the prediction blend.

## Shared TPS event detection

`Engine::periodicSlowCallback()` supplies `Sensor::getOrZero(Tps1)` to `TpsAccelEnrichment::onNewValue()` at 20 Hz (50 ms). The detector selects the **largest signed adjacent-sample difference** in its history and publishes that pair as `tpsFrom`, `tpsTo`, and `deltaTps`.

- `deltaTps > tpsAccelEnrichmentThreshold` activates acceleration detection and latches an event for MAP Prediction to consume.
- `deltaTps < -tpsDecelEnleanmentThreshold` activates the adder deceleration path. Because the detector selects the largest signed difference, a zero or positive step elsewhere in the history can mask a throttle-closing step.
- Threshold units are TPS percentage points **per sample**, not percent/second or percent/engine-cycle. For example, 10% -> 20% is a delta of 10.
- In the adder modes, `tpsAccelLookback` is a duration in seconds, converted to a sample count. A zero value disables the adder. More history keeps an earlier opening step eligible longer; this is not a derivative over the entire lookback interval.
- In MAP Prediction, the sample count comes from the largest `predictiveMapBlendDurationValues` entry: `max(2, ceil(maxDuration / 0.05))`. The adder lookback setting is ignored.

The fractional accumulator advances in `onEngineCycleTps()` from the trigger callback at engine-cycle boundaries, independently of TPS sampling. `resetAE()` clears TPS history and fractional state when the engine stops.

## MS Adder and Percent Adder

Both modes share `TpsAccelEnrichment::getTpsEnrichment()`:

1. Return zero if lookback is zero, RPM is below `cranking.rpm`, or the TunerStudio VE-tuning detector is active (when compiled in).
2. On acceleration, interpolate `tpsTpsAccelTable` at the selected **from TPS (X)** and **to TPS (Y)**. Despite their names, `tpsTpsAccelFromRpmBins` and `tpsTpsAccelToRpmBins` contain TPS percentages, not RPM.
3. On deceleration, use `deltaTps * tpsDecelEnleanmentMultiplier`; otherwise start with zero extra fuel.
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

1. Consumes a latched TPS acceleration event and starts prediction only when table MAP exceeds measured MAP and the retrigger lock is clear. It initially uses table MAP directly.
2. Blends from the event's prediction toward the **current measured MAP**:

   ```text
   effectiveMap = initialPredictedMap
                + (currentMeasuredMap - initialPredictedMap) * elapsed / blendDuration
   ```

3. If table MAP rises above the saved prediction as the throttle keeps opening, adopts the higher prediction and restarts the blend timer.
4. Ends prediction when the duration expires, measured MAP catches up to the prediction/effective MAP, or TPS falls more than the acceleration threshold below the tracked peak. It drains acceleration events while active. The retrigger lock clears when the shared `isAboveAccelThreshold` flag becomes false; a physical return to closed throttle is not required.

For example, predicted MAP 85 kPa and measured MAP 65 kPa initially use 85 kPa. Halfway through the blend, if measured MAP has risen to 75 kPa, effective MAP is 80 kPa.

The effective MAP feeds both the physical air-mass calculation and the speed-density load/VE path (including barometric normalization when configured). It does not replace the registered MAP sensor reading used elsewhere.

The TPS/TPS extra-fuel table, fractional settings, adder RPM correction and `flexAeMult` do not apply in this mode; `getTpsEnrichment()` returns zero immediately. That early return also means the adders' cranking-RPM and VE-tuning checks do not gate MAP Prediction itself.

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

`ShortTermFuelTrim` inhibits correction while `getTimeSinceAcell() < noFuelTrimAfterAccelTime`. The adders reset that timer on acceleration or deceleration; MAP Prediction resets it when prediction starts. The "Inhibit closed loop fuel after accel" setting is available in TunerStudio for all three modes.

| Inspect | Meaning / limitation |
|---|---|
| `engine` (log label `Engine`) | TunerStudio/MLV state byte: bits 0/1 running/cranking, 2/3 after-start/warmup, 4 TPS acceleration (including active MAP Prediction), 5 TPS deceleration or DFCO; bits 6/7 unused. Adder threshold flags and the signed applied adder cover detection and decay. Wall wetting is not represented. |
| `tpsFrom`, `tpsTo`, `deltaTps`, `isAboveAccelThreshold` | Selected historical TPS step and event threshold, not necessarily the latest step. |
| `valueFromTable`, `extraFuel`, `accumulatedValue`, `cycleCnt` | Adder internals. Early-return paths can leave these diagnostic fields unchanged. |
| `tpsAccelFuel` | Adder result; zero in MAP Prediction does not mean transient correction is inactive. |
| `fallbackMap`, `effectiveMap`, `isMapPredictionActive` | Table prediction, MAP used by speed density, and prediction state. |
| `predTimerResetCnt`, `mapPredEventOver` | Prediction starts and duration expirations; not every timer restart or early exit. |
| `flexAeMultiplier`, `flexWwTauMultiplier`, `flexWwBetaMultiplier` | Last evaluated flex multipliers; bypassed paths need not refresh them. |

## Source and existing coverage

- [Mode enum](../../firmware/controllers/algo/rusefi_enums.h), [mode configuration](../../firmware/controllers/algo/accel_enrichment.engineConfiguration.txt), [table definitions](../../firmware/integration/rusefi_config.txt), [TunerStudio panels](../../firmware/tunerstudio/tunerstudio.template.ini).
- [TPS detector and adders](../../firmware/controllers/algo/accel_enrichment.cpp), [mass conversion](../../firmware/controllers/algo/fuel_math.cpp), [predictive MAP](../../firmware/controllers/algo/airmass/speed_density_airmass.cpp).
- [Wall model](../../firmware/controllers/algo/wall_fuel.cpp), [injection integration](../../firmware/controllers/engine_cycle/main_trigger_callback.cpp), [flex multiplier helper](../../firmware/controllers/algo/flex_transient.h), [closed-loop gating](../../firmware/controllers/math/closed_loop_fuel.cpp).
- [TPS tests](../../unit_tests/tests/test_accel_enrichment.cpp): history, sampling versus engine-cycle updates, fractional portions, predictive-mode adder bypass and flex multiplier.
- [Fuel math tests](../../unit_tests/tests/ignition_injection/test_fuel_math.cpp): `AirmassModes.PredictiveMapCalculation` and `PredictiveMapBlendTowardRisingSensor` cover blending, catch-up and sensor fallback.
- [Wall-wetting tests](../../unit_tests/tests/ignition_injection/test_fuel_wall_wetting.cpp): film math, injection-event scheduling and flex compensation.
