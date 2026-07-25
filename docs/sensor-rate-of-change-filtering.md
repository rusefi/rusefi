# Rate-of-change handling across sensors and values

rusEFI has no single rate-of-change policy: different sensors and derived values get
different treatment, implemented at different layers with different (mostly hardcoded)
constants. This report inventories all of them, layer by layer, so the differences are
visible in one place.

One overall finding up front: **rusEFI has no per-sensor "value changed too fast"
warnings.** Sensor plausibility is enforced by range checks (`UnexpectedCode::Low/High`
in converters), redundancy split checks (dual TPS/PPS), and timeouts (frequency
sensors) - never by rate-of-change. Where rate of change is handled at all, it is
handled silently: filtered, slew-limited, or consumed as a control input.

## Layer 1: ADC-layer low-pass (Biquad per analog channel)

Every slow analog sensor is filtered by a 2nd-order Biquad low-pass at the ADC
subscription layer (`firmware/hw_layer/adc/adc_subscription.cpp`). Sampling rate is
`SLOW_ADC_RATE` (500 Hz); the cutoff frequency is hardcoded per call site of
`AdcSubscription::SubscribeSensor()`. On the first sample the filter is preloaded via
`cookSteadyState()` so there is no startup ramp.

| sensor(s)                                        | cutoff (-3 dB) | call site                        |
|--------------------------------------------------|---------------:|----------------------------------|
| TPS1/TPS2 primary+secondary (also wastegate pos, idle pos) | 200 Hz | `init_tps.cpp`                    |
| slow MAP, throttle inlet pressure, compressor discharge    | 100 Hz | `init_map.cpp`                    |
| high pressure fuel                               |         100 Hz | `init_fluid_pressure.cpp`         |
| fuel level                                       |         100 Hz | `init_fuel_level.cpp` (see notes) |
| analog MAF                                       |          50 Hz | `init_maf.cpp`                    |
| vbatt, ignition-key voltage                      |          20 Hz | `init_vbatt.cpp`                  |
| baro                                             |          10 Hz | `init_map.cpp`                    |
| oil pressure, low pressure fuel, A/C pressure, auxLinear1-4 | 10 Hz | `init_fluid_pressure.cpp`         |
| aux analog inputs, gear range input              |          10 Hz | `init_aux.cpp`, `init_range.cpp`  |
| thermistors (CLT, IAT, aux temp, oil temp)       |           2 Hz | `init_thermistors.cpp`            |

The spread is intentional (TPS must be fast for ETB and accel enrichment; temperature
physically cannot move fast) but none of it is tunable and none of it is documented in
TS.

## Layer 2: logic-level smoothing (per-sensor, partly tunable)

On top of the ADC filter, several sensors get a second, sensor-specific smoothing
stage:

- **MAP** (`modules/map_averaging/map_averaging.cpp`): per-engine-cycle window
  averaging (angle-based sampling window), then a `mapMinBufferLength` minimum-buffer
  across recent cycles, then `ExpAverage` with tunable `mapExpAverageAlpha`.
  Alpha = 1 means pass-through; `applyDefaultsOrFixAfterBurn()` sanitizes
  out-of-range values back to 1.
- **Accelerator pedal (PPS)** (`init/sensor/init_tps.cpp`): the redundant pair is
  combined first, then `ExpAverage` with tunable `ppsExpAverageAlpha` produces
  `AcceleratorPedal`; the raw combination stays available as
  `AcceleratorPedalUnfiltered`. Same alpha = 1 = disabled convention.
- **Lambda/AFR**: both the analog EGO path (`sensors/impl/ego.cpp`) and the AEM/rusEFI
  CAN wideband path (`sensors/impl/AemXSeriesLambda.cpp`) apply `ExpAverage` with
  tunable `afrExpAverageAlpha`, publishing separate "smoothed" lambda sensors next to
  the raw ones.
- **Analog MAF** (`init_maf.cpp` `MafFilter`): an RPM-adaptive exponential filter on
  top of the 50 Hz ADC filter - `alpha = FAST_CALLBACK_PERIOD * rpm /
  mafFilterParameter`, i.e. the time constant tracks manifold fill time. Filtering
  disables itself entirely above alpha 0.98 (high rpm / small manifold),
  `mafFilterParameter = 0` disables, engine-stopped passes through. Only value in this
  layer whose *time constant* adapts to engine state.
- **VSS** (`frequency_sensor.cpp` + `init_vehicle_speed_sensor.cpp`): per-tooth Biquad
  in event domain; tunable via `vssFilterReciprocal` (filter parameter =
  1/vssFilterReciprocal, hard-clamped to 0.35 for stability; default averages over
  ~20 teeth).
- **Flex/ethanol** (`sensors/flex_sensor.h`): fixed Biquad `configureLowpass(100, 1)`
  on ethanol % (~0.5-1.5 Hz effective, comment in code) and a much slower
  `configureLowpass(1, 0.01)` on fuel temperature.
- **Knock** (`software_knock.cpp`): Biquad configured as a *band-pass* at the knock
  frequency - included here for completeness, its job is extraction, not smoothing.

## Layer 3: explicit rate limiting (slew) on derived values

These do not smooth a measurement - they cap how fast a computed value may move:

- **tCharge** (`algo/engine2.cpp` `updateTChargeK` -> `limitRateOfChange()` in
  `util/efilib.cpp`): the only user of the generic asymmetric limiter. Defaults:
  rise limited to 1 degC/s, fall to 12.5 degC/s (`tChargeAirIncrLimit` /
  `tChargeAirDecrLimit`, tunable). Asymmetry models thermal mass: hot soak builds
  slowly, cool inrush acts fast.
- **Fan PWM soft start** (`modules/fan_control/fan_control.cpp`): upward-only slew so
  a 0-100% ramp takes `softStartSec` seconds (step computed at the 20 Hz slow
  callback); downward steps are unlimited.
- **Idle return target ramp**: `idleReturnTargetRampDuration` rate-limits how fast the
  idle RPM target returns, avoiding a target step when leaving coasting.

Notably NOT rate-limited: **injector pulse width**. Nothing caps cycle-to-cycle pulse
growth; protection is level-based only - the LimpManager duty-cycle cut
(`maxInjectorDutyInstant` 110% / `maxInjectorDutySustained` 96% for 0.5 s, hysteresis
resets below 20% duty, warning `CUSTOM_TOO_LONG_FUEL_INJECTION`), plus an injection
*timing* update freeze above 90% duty (`engine2.cpp`). The wall-wetting model and the
fractional accel-pump spread transient fuel across cycles, which softens pulse growth
only as a side effect. Pulse growth IS however *measured*: `actualLastInjectionRatio`
(and its stage-2 twin) logs new-vs-previous pulse duration for cylinder 0
(`main_trigger_callback.cpp`) - an observation-only output channel nothing consumes,
i.e. the observability hook for growth-based diagnostics already exists.

Related but inverted: the **idle controller** splits the airmass error *by frequency*
instead of limiting it - a high-pass Biquad (`configureHighpass(20, 1)`,
`idle_thread.cpp`) sends fast content to ignition timing (fast actuator) and the
low-frequency remainder to the idle valve (slow actuator).

## Rate of change consumed as a control input

- **TPS/TPS acceleration enrichment** (`algo/accel_enrichment.cpp`): deltaTps measured
  over a `tpsAccelLookback` cycle buffer, thresholded by
  `tpsAccelEnrichmentThreshold`; deceleration uses `tpsDecelEnleanmentMultiplier`.
  The *output* is itself rate-shaped by the fractional pump imitation
  (`tpsAccelFractionPeriod` / `tpsAccelFractionDivisor` spread the extra fuel over
  several cycles).
- **MAP rate-of-change / predictive MAP** (`AE_MODE_PREDICTIVE_MAP`): alternative AE
  mode driven by MAP slope instead of TPS slope.
- **ETB autotune** (`electronic_throttle.cpp`): measured oscillation amplitude/period
  are smoothed with a fixed EMA (alpha = 0.05) before the Astrom-Hagglund math -
  measurement conditioning, hardcoded.

## Time-domain spike rejection (signal, not value)

- **Trigger noise filter** (`TriggerNoiseFilter::noiseFilter`, `trigger_central.cpp`,
  opt-in via `useNoiselessTriggerDecoder`): rejects edges whose accumulated
  high/low period is implausibly short vs the previous period (empirical 2/3 margin,
  widened at expected sync gaps). This is the closest thing to a rate-of-change
  *plausibility* check in the codebase, and it operates on the trigger signal, not on
  a sensor value.

## Observations / possible follow-ups

1. **No rate-of-change warnings exist.** A CLT that jumps 40 degC between two 500 Hz
   samples passes the 2 Hz filter silently instead of raising a wiring/connector
   warning. If plausibility diagnostics are ever wanted, the natural seam is
   `AdcSubscription::UpdateSubscribers()` (it sees raw vs filtered values for every
   analog channel in one place).
2. **Fuel level is filtered at 100 Hz** - same as MAP, 50x faster than baro, despite
   being the slowest-moving quantity in the system (slosh argues for the *most*
   filtering). No additional smoothing exists downstream of the ADC filter. Likely a
   copy-paste default worth revisiting.
3. **Constants are scattered and mostly hardcoded**: ADC cutoffs live in per-sensor
   init call sites; only MAP/PPS/AFR alphas, `mafFilterParameter`,
   `vssFilterReciprocal` and the tCharge limits are tunable. The three ExpAverage
   alphas share the "1 = disabled" convention enforced by
   `applyDefaultsOrFixAfterBurn()`, while `mafFilterParameter` uses "0 = disabled" -
   inconsistent conventions for the same concept.
4. **Redundant-sensor comparison (TPS/PPS split check) is the de-facto noise
   detector** today: a noisy channel eventually diverges from its partner and fails
   the split check - a warning caused by noise, but triggered by disagreement, not by
   rate.
