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

## Lua implementation injection

[tag:lua_accel_enrichment] Select **Accel Enrichment Mode = Lua** and call
`setAccelEnrichment(grams)` from the script. The argument is finite, non-negative
extra fuel mass **per cylinder per engine cycle**, held until replaced. Zero
turns it off. Negative, NaN and infinite arguments clear the request and raise a
Lua error. Requires a build with `EFI_LUA` and `EFI_ENGINE_CONTROL`.

The script owns event detection, magnitude, decay, RPM and flex compensation.
Lua mode ignores the native TPS table, thresholds, lookback, fractional pump,
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
| `tpsFrom`, `tpsTo`, `deltaTps`, `isAboveAccelThreshold` | Selected historical TPS step and event threshold, not necessarily the latest step. |
| `valueFromTable`, `extraFuel`, `accumulatedValue`, `cycleCnt` | Adder internals. Early-return paths can leave these diagnostic fields unchanged. |
| `tpsAccelFuel` | Native adder result; zero in MAP Prediction or Lua does not mean transient correction is inactive. |
| `luaAccelFuel` | Applied Lua extra mass in grams per cylinder per cycle, before injection-mode scaling; zero when inactive, expired or suppressed. |
| `fallbackMap`, `effectiveMap`, `isMapPredictionActive` | Table prediction, MAP used by speed density, and prediction state. |
| `predTimerResetCnt`, `mapPredEventOver` | Prediction starts and duration expirations; not every timer restart or early exit. |
| `flexAeMultiplier`, `flexWwTauMultiplier`, `flexWwBetaMultiplier` | Last evaluated flex multipliers; bypassed paths need not refresh them. |

## TunerStudio transient-fueling screens

`Fuel -> AE / Transient fueling` has five entries, built from
`tunerstudio/transient_fueling.ini` with gauges declared in
`tunerstudio/transient_gauges.ini`:

- **Acceleration enrichment settings** owns the only AE mode selector,
  VE-edit suspension and STFT holdoff settings. TPS detector/pump controls
  appear only in the modes that consume them.
- **TPS / Lua AE calibration** shows the TPS table/RPM curve for adders or Lua
  instructions and applied cycle-mass telemetry. This menu is hidden in MAP
  Prediction mode. The MAP table and duration curve are not embedded here.
- **MAP prediction and sensor fallback** separately owns the estimated-MAP
  table. In predictive Speed Density it also shows the blend-duration curve.
  Other Speed Density modes expose the same table as an advanced sensor
  fallback. With MAP Prediction and another fuel algorithm, it explains the
  incompatibility. There is no duplicate editor or strategy selector.
  Automatic TPS-history help sits directly above the duration curve, visible
  only in predictive Speed Density. It explains the highest-duration history
  rule separately from current-RPM blend interpolation. It is not a section
  of Acceleration enrichment settings or the sensor-fallback view.
- **Wall wetting compensation** groups Basic constants or Advanced curves with
  film mass/correction monitors. Advanced always exposes all four curves:
  tau/CLT, tau/MAP, beta/CLT and beta/MAP. The firmware checks registered MAP
  sensor availability, not just the analog MAP pin; only the ECU decides
  whether the MAP multipliers apply. The mg correction gauge uses
  `wallFuelCorrectionValue`, not the raw `wallFuelCorrection` value in grams.
  Both describe injection event 0; the correction may remain unchanged during
  bypass and must not be interpreted as proof that the model is active.
- **Flex fuel transient compensation** owns the shared `flexFuelTransientComp`
  switch. It is visible when injection is enabled and a digital flex input is
  assigned (excluding NONE/INVALID), regardless of E0 or a reading error. The
  switch remains accessible when disabled. Additive flex calibration is hidden
  in MAP/Lua; wall-film flex tables apply to every AE mode and Basic/Advanced.
  Their shared CLT/ethanol bins must be edited with that shared scope in mind.
  The additive AE multiplier is called directly from accel_enrichment.cpp;
  it does not require wall compensation to be active. Base stoichiometry and
  flex cranking/priming have separate controls.

The screens use visibility expressions rather than merely disabling unused
controls. They intentionally omit STFT correction/state gauges and target
lambda/AFR gauges or graph lines. At the user's later request, Wall Wetting
alone shows measured wideband 1 via `selectedAfrLambda1`, following the
project's `useLambdaOnInterface` display preference. Other transient screens
still omit mixture monitoring. STFT configuration remains available.
The new status panels use only existing channels; intermediate TPS fields and
flex multipliers can retain their last evaluation on bypassed paths. There is
no inferred Lua expiry reason or wall-enable indicator. Pressure comparisons
use raw kPa channels consistently, and the CLT monitor uses raw Celsius.

The status panels use multi-column numeric readouts instead of tall stacks
of circular gauges. Settings/help are anchored North and only editors/graphs
consume extra space. Wall curves form two columns, evaporation and deposition,
each with CLT and MAP. Embedded circular gauges were removed from these six
curve editors; their live tracking channels remain unchanged. Curve sizes are
preferred-size hints, not a guarantee about every TunerStudio/DPI combination.

TPS/Lua calibration uses one six-value readout panel per mode: two rows and
three columns. The first row is RPM, TPS and ECU RPM acceleration. The second
row is injection pulse, table value and corrected AE for MS/Percent, or
injection pulse, applied Lua cycle mass and coolant temperature for Lua.
Advanced fractional-pump diagnostics remain in a separate conditional group.

Settings, TPS/Lua calibration, MAP and Wall Wetting also display the existing
`rpmAcceleration` ECU channel as a compact signed RPM/s readout; calibration
graphs include it to compare engine response with TPS/fueling. It is telemetry
only, not an AE detection input added by this UI. `status_loop.cpp` copies
`RpmCalculator::getRpmAcceleration()` into that channel without a new host-side
derivative. Preserve its existing calculation semantics: `rpmRate` is updated
in the cycle-period path as `(rpm - previousRpmValue) / (mult * periodSeconds)`,
where `mult` is engine-cycle degrees / 360. This normalization means it must
not be assumed to be an independently measured physical derivative (for a
720-degree cycle it includes the extra factor of two in the denominator).
Forced instant RPM and discarded phase-resync samples do not update it; stop
resets it. The signed 16-bit channel and ECU calculation were not changed.

No ECU algorithm, table storage or binary telemetry layout is changed by this
organization. See `tunerstudio/tests/test_transient_ui.py` for generated-INI regression and
same-board calibration/telemetry compatibility checks.

## Source and existing coverage

- [Mode enum](../../firmware/controllers/algo/rusefi_enums.h), [mode configuration](../../firmware/controllers/algo/accel_enrichment.engineConfiguration.txt), [table definitions](../../firmware/integration/rusefi_config.txt), [TunerStudio panels](../../firmware/tunerstudio/tunerstudio.template.ini).
- [TPS detector and adders](../../firmware/controllers/algo/accel_enrichment.cpp), [mass conversion](../../firmware/controllers/algo/fuel_math.cpp), [predictive MAP](../../firmware/controllers/algo/airmass/speed_density_airmass.cpp).
- [Wall model](../../firmware/controllers/algo/wall_fuel.cpp), [injection integration](../../firmware/controllers/engine_cycle/main_trigger_callback.cpp), [flex multiplier helper](../../firmware/controllers/algo/flex_transient.h), [closed-loop gating](../../firmware/controllers/math/closed_loop_fuel.cpp).
- [TPS tests](../../unit_tests/tests/test_accel_enrichment.cpp): history, sampling versus engine-cycle updates, fractional portions, predictive-mode adder bypass and flex multiplier.
- The same file's `LuaAccelEnrichment` tests exercise the Lua binding, mass scaling, native-mode isolation, expiry, reset, DFCO/cranking and VE-tuning suppression. [TunerStudio tests](../../unit_tests/tests/test_tunerstudio.cpp) cover the Lua acceleration state bit.
- [Fuel math tests](../../unit_tests/tests/ignition_injection/test_fuel_math.cpp): `AirmassModes.PredictiveMapCalculation` and `PredictiveMapBlendTowardRisingSensor` cover blending, catch-up and sensor fallback.
- [Wall-wetting tests](../../unit_tests/tests/ignition_injection/test_fuel_wall_wetting.cpp): film math, injection-event scheduling and flex compensation.
