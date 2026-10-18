# DONE — 13a — TPS smoothing

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Upstream completion

**DONE on upstream master**, reviewed at [`41e3c63a986`](https://github.com/rusefi/rusefi/commit/41e3c63a9865141686bbfa7940defe0e86a4cb11). Implementation commit [`edaaf10bab6`](https://github.com/rusefi/rusefi/commit/edaaf10bab64e4ab9b60f60684995a8246207c05) adds separate unfiltered TPS identities, independent exponential filters with shared settings, fast-transient reset, calibration repair and tuning controls. Alpha defaults to 1/pass-through; zero threshold or time disables transient reset. Redundancy checks occur before smoothing, and invalid readings/timeouts propagate to consumers immediately.

The [upstream filter](https://github.com/rusefi/rusefi/blob/41e3c63a9865141686bbfa7940defe0e86a4cb11/firmware/controllers/sensors/tps_smoothing.h) treats zero throttle as a real sample, clears history after invalid input or sensor reconfiguration, rejects non-finite readings and passes through invalid live alpha settings until calibration repair. [Sensor registration](https://github.com/rusefi/rusefi/blob/41e3c63a9865141686bbfa7940defe0e86a4cb11/firmware/init/sensor/init_tps.cpp) routes analog, Ford and SENT providers through the filter. It only registers a proxy when an unfiltered source exists, preserving direct OBD/Lua/CAN providers where appropriate. A direct resolved TPS provider bypasses smoothing; an external unfiltered provider must exist before initialization to acquire a proxy. This does not implement the external CAN ETB protocol in report 59.

Like the fork, the average advances on each resolved sensor read, including repeated reads of one acquired sample. Its response therefore depends on consumer read frequency. The transient comparison uses an inclusive change threshold and a strict elapsed-time window.

The adoption commit changes **111 maintained firmware C/C++ lines (+106 / −5 across five files)**: `default_base_engine.cpp`, `sensor_type.h`, `tps.h`, the new `tps_smoothing.h`, and `init_tps.cpp`. Generated files, schema, UI, build files, docs and tests are excluded. The new [upstream tests](https://github.com/rusefi/rusefi/blob/41e3c63a9865141686bbfa7940defe0e86a4cb11/unit_tests/tests/sensor/test_tps_smoothing.cpp) add 297 lines separately, covering zero samples, reset boundaries, independent histories, calibration repair, invalidity/recovery, timeouts, analog/Ford/SENT paths and external-provider ownership. Tests were inspected, not rerun.

The former report 13 combined smoothing with an independent discrepancy-limit change. That remaining change is now tracked in [report 13b](13b-tps-pps-discrepancy-configuration.md): master retains its default cap of 5 rather than the fork's 30. The original four-file LOC scope below is preserved as a historical measurement and still overlaps with discrepancy and CAN ETB work; it is not a remaining-work estimate.

## Executive summary

The original master comparison already has pedal smoothing. The branch adds separate unfiltered TPS1/TPS2 sensor identities and filtered proxies, sharing a configurable exponential-average coefficient. A fast change at least as large as the configured threshold within a time window resets the average. Alpha defaults to 1/pass-through; zero reset settings leave the transient-reset feature disabled.

## ECU logic and behavior

1. Calibrated TPS1/TPS2 values pass through new raw identities and filtered proxy sensors.
2. Each filter maintains the previous raw sample/time and an exponential average.
3. A sufficiently large raw step inside the reset interval resets the average before the new value is accepted.
4. Alpha 1 passes readings through; both reset settings must be positive to activate transient reset.

**Illustrative behavior:** A small steady fluctuation can be smoothed while a rapid throttle movement resets the average, avoiding the same smoothing delay through that step.

## Calibration, dependencies and review limits

This affects TPS consumers, including throttle feedback and acceleration enrichment. The independent TPS/PPS discrepancy calibration change is covered in [report 13b](13b-tps-pps-discrepancy-configuration.md). No dedicated TPS-smoothing test was added in the pinned fork; the final branch report records compilation followed by a CAN-related link failure.

Review noise attenuation against ETB feedback delay, invalid-sensor propagation and interaction with CAN virtual channels. The upstream coverage cited above is separate from the pinned fork's test evidence.

No feature-specific test is claimed for the pinned fork. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 277 changed lines — 253 additions and 24 deletions across 4 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+254 / −33**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/defaults/default_base_engine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/defaults/default_base_engine.cpp) | +82 / −7 | +83 / −16 |
| [firmware/controllers/sensors/tps.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/tps.cpp) | +4 / −0 | +4 / −0 |
| [firmware/controllers/sensors/tps.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/tps.h) | +11 / −0 | +11 / −0 |
| [firmware/init/sensor/init_tps.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_tps.cpp) | +156 / −17 | +156 / −17 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-09-06 | [f4dec5f1f96](https://github.com/ElDominio/rusefi/commit/f4dec5f1f96b4dee90a34cc850722f39d6ed2336) — External CAN ETB: virtual TPS1/TPSB channels, feedforward telemetry, status gauge | +87 / −25 |
| 2026-09-29 | [d3e61a53053](https://github.com/ElDominio/rusefi/commit/d3e61a53053b0c054c9974394f970ae87f268e16) — tps: add ExpAverage smoothing to TPS1/TPS2 with smart fast-transient reset | +90 / −5 |

Supporting configuration and source: [firmware/integration/rusefi_config.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/integration/rusefi_config.txt).
