# Cold-start logic

This describes the current firmware path from ignition-on through warm-up. The same
logic also handles warm restarts; temperature-dependent tables determine enrichment.
It supplements the [fueling overview](fueling_system.md).

## States and time bases

| Phase | Fuel selection | Transition or duration |
|---|---|---|
| Ignition-on, stopped | Optional simultaneous priming pulse | Scheduled after `primingDelay` seconds plus 100 ms |
| Cranking | Cranking base mass times coolant and throttle multipliers | Positive RPM before reaching `cranking.rpm` |
| Running, after start | Running fuel math, including ASE and WUE | Entered at RPM **greater than or equal to** `cranking.rpm` |
| Warming up | Running fuel math, including WUE | WUE follows current coolant temperature, without a separate timed phase |

`RpmCalculator::isCranking()` includes `CRANKING` and `SPINNING_UP` with positive
cached RPM. Once `RUNNING` is reached, dropping below `cranking.rpm` does **not**
re-enter cranking. The engine must stop before another cranking attempt.

Despite its name, `getRevolutionCounterSinceStart()` advances once per **engine
cycle** (`onNewEngineCycle()`), normally 720 crank degrees on a four-stroke engine.
It advances during cranking and is cleared by `setStopSpinning()`, not by the
transition to running. Both the cranking base table and ASE use this counter.
`timeSinceCrankingInSecs` is a different quantity: its timer is continually reset
while cranking, and measures seconds after leaving cranking.

## Priming pulse

`PrimeController` handles the ignition-on event separately from synchronized
cranking injection. `primeValues` specifies **mg**, indexed by coolant temperature
(`primeBins`). If `flexCranking` is enabled and an ethanol sensor is configured,
`primeFlexTable` replaces that curve, using coolant and ethanol percentage.
The mass is converted to grams and passed through the primary injector model to
obtain milliseconds. A nonpositive resulting duration produces no pulse.

The ignition-on handler skips scheduling when the engine is already spinning, or
when flood clear is enabled and `Tps1` is strictly above 90%. These gates are
checked when scheduling; the delayed `onPrimeStart()` callback does not recheck
them. Coolant and ethanol are read when that callback calculates the pulse.
Invalid coolant skips priming. A configured but invalid ethanol sensor uses 50%.

On targets with backup SRAM, a key-cycle counter suppresses repeated priming when
the ignition is toggled without cranking. Rotation clears it in the slow callback;
a counter above 10 also allows another attempt. Without backup SRAM this repeated
key-cycle suppression is absent. Thus priming is not guaranteed on every key-on.

## Cranking fuel

`getInjectionMass()` always calculates the running base mass, then selects
`getCrankingFuel3()` while the RPM state reports cranking:

```text
cranking cycle mass = base cranking mass * coolant multiplier * throttle multiplier
```

| Setting | Base cranking mass |
|---|---|
| `useRunningMathForCranking = false` ("Fixed") | `crankingCycleBaseFuel`, indexed by coolant and engine-cycle count; table values are mg, converted to grams |
| `useRunningMathForCranking = true` ("Fuel Map") | `getBaseFuelMass()` from the running air/fuel model |

"Fixed" selects a mass table, not a constant pulse width. "Fuel Map" selects the
running **base** calculation, not `getRunningFuel()` with all running corrections.
In particular, the WUE, IAT correction and ASE multipliers from `getRunningFuel()`
are not multiplied into this cranking formula. The air model can still depend on
IAT when using Fuel Map.

The coolant multiplier is `crankingFuelCoef` vs `crankingFuelBins`, or
`crankingFuelFlexTable` vs coolant and `crankingFuelFlexBins` when `flexCranking`
and a configured ethanol sensor select flex operation. The throttle multiplier
is `crankingTpsCoef` vs `crankingTpsBins`, using `DriverThrottleIntent`.
This curve is separate from the flood-clear fuel cut.

Fuel Map already includes flex-adjusted stoichiometry. Its cranking flex table
adds enrichment on top of that correction; it is not a replacement for the
ethanol stoichiometric correction. See [legacy tune migration](fueling_system.md#legacy-tune-migration).

The result is a cycle mass, not the final pulse width. Injection-mode splitting,
downstream fuel adjustments/cuts, injector characterization and scheduling still
apply. Cranking can use a different injection mode from running. See
[acceleration enrichment](acceleration_enrichment.md) for its separate RPM gates
and the wall-film model's cranking bypass.

## After-start enrichment (ASE) and warm-up enrichment (WUE)

Once running, `getRunningFuel()` multiplies base mass by barometric, IAT, coolant
(WUE) and post-cranking (ASE) corrections, with other enabled features adding
their own adjustments. ASE and WUE can apply together; they are not consecutive
exclusive modes.

- **ASE:** `postCrankingFactor`, indexed by coolant (`postCrankingCLTBins`) and
  engine-cycle count (`postCrankingDurationBins`). Values below 1 are clamped to
  1. Once the count is **greater than** the last bin, the factor becomes 1 even
  if the table's final value is higher. At the last bin itself, the table still
  applies. Because cranking consumes this count, a long cranking attempt can
  partly or completely exhaust ASE before the engine reaches running RPM.
- **WUE:** `cltFuelCorr` vs `cltFuelCorrBins`, evaluated at the current coolant
  temperature. It is a ratio: 1.00 is neutral and 1.20 adds 20%. It has no ASE-style
  elapsed-count cutoff. Invalid coolant returns 1.

For example, WUE 1.20 and ASE 1.10 multiply to 1.32 before other corrections.
The closed-loop fuel controller has separate running-state, startup-delay,
coolant and lambda gates. Finishing ASE or reaching a neutral WUE value alone
does not enable closed-loop correction.

## Sensor fallbacks and flood clear

These lookups do not share a single substitute coolant temperature:

| Lookup | Invalid input behavior |
|---|---|
| Priming coolant | Skip pulse |
| Fixed cranking base-table coolant | Use 0 C |
| Cranking coolant multiplier | Use 20 C |
| ASE coolant | Use 0 C |
| WUE coolant / running IAT correction | Neutral multiplier, 1 |
| Cranking throttle multiplier | Neutral multiplier, 1 |
| Configured ethanol sensor in priming/cranking flex tables | Use 50% ethanol |

With `isCylinderCleanupEnabled`, the normal fuel-permission path cuts fuel when
the engine is **not running** and `DriverThrottleIntent` is strictly above 90%.
This differs from the priming scheduling gate, which reads `Tps1`. Do not assume
the two are interchangeable on an electronic-throttle setup. The cranking TPS
multiplier does not enable or disable this separate cut.

## Air, spark and diagnostics

Cranking idle air uses the coolant-indexed `cltCrankingCorr` curve. The ordinary
position-based crank-to-idle taper uses the same engine-cycle counter described
above, with a coolant-dependent duration. See [idle control](idle_system.md)
for phase selection and the modeled-flow alternative.

Cranking uses `ignitionDwellForCrankingMs`. Advance comes from the separate
cranking curve when selected; otherwise `crankingTimingAngle` is blended toward
running advance at the cranking RPM threshold. Global fixed timing overrides
that advance selection. `useAdvanceCorrectionsForCranking` controls the normal
advance corrections in cranking. See [ignition](ignition_system.md) and the
separate [kick-start spark path](kick-start.md).

For a start log, compare RPM/state, coolant, throttle intent, cranking base/final
mass, WUE/ASE multipliers, injector pulse width and fuel-cut reason. The standard
`engine` output (log label `Engine`) contains independent running, cranking, ASE
and WUE bits; ASE/WUE bits reflect non-neutral correction values and do not prove
those multipliers are being applied to the cranking branch. The
`injectionPrimingCounter` records fired prime pulses. Injector pin-state traces
can distinguish a calculated fuel request from an actual scheduled injection.

## Source and existing coverage

- [RPM state and cycle counter](../../firmware/controllers/engine_cycle/rpm_calculator.cpp)
- [Priming](../../firmware/controllers/engine_cycle/prime_injection.cpp)
- [Fuel selection and correction math](../../firmware/controllers/algo/fuel_math.cpp)
- [Flood-clear fuel permission](../../firmware/controllers/limp_manager.cpp)
- [Closed-loop fuel gates](../../firmware/controllers/math/closed_loop_fuel.cpp)
- [Fuel-math tests](../../unit_tests/tests/ignition_injection/test_fuel_math.cpp)
- [Priming tests](../../unit_tests/tests/ignition_injection/test_startOfCrankingPrimingPulse.cpp)
