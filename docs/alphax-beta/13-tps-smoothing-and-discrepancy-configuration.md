# 13 — TPS smoothing and discrepancy configuration

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Master already has pedal smoothing. The branch adds separate unfiltered TPS1/TPS2 sensor identities and filtered proxies, sharing a configurable exponential-average coefficient. A fast change exceeding the configured threshold within a time window resets the average. Alpha defaults to 1/pass-through; zero reset settings leave the transient-reset feature disabled.

## ECU logic and behavior

1. Calibrated TPS1/TPS2 values pass through new raw identities and filtered proxy sensors.
2. Each filter maintains the previous raw sample/time and an exponential average.
3. A sufficiently large raw step inside the reset interval resets the average before the new value is accepted.
4. Alpha 1 passes readings through; both reset settings must be positive to activate transient reset.
5. The default maximum discrepancy limit changes independently of filtering; etbSplit remains 5.

**Illustrative behavior:** A small steady fluctuation can be smoothed while a rapid throttle movement resets the average, avoiding the same smoothing delay through that step.

## Calibration, dependencies and review limits

This affects every TPS consumer, including throttle feedback and acceleration enrichment. Separately, the default maximum permitted TPS/PPS discrepancy setting rises from 5 to 30, while the normal `etbSplit` default stays 5. That limit change is independently reviewable. No dedicated TPS-smoothing test is added; the final branch report records compilation followed by a CAN-related link failure.

Review noise attenuation against ETB feedback delay, invalid-sensor propagation and interaction with CAN virtual channels. There is no dedicated new smoothing test; the tip reports a unit-test link failure.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

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
| 2026-06-29 | [135f45852f6](https://github.com/ElDominio/rusefi/commit/135f45852f690816fc92d13a8bc29839b414df8a) — Misc actuator/controller fixes: sport pedal live bit, upshift flat-shift priority, gear detector clutch-aware, fuel pump no-sensor fast path, etbSplit default | +1 / −1 |
| 2026-09-06 | [f4dec5f1f96](https://github.com/ElDominio/rusefi/commit/f4dec5f1f96b4dee90a34cc850722f39d6ed2336) — External CAN ETB: virtual TPS1/TPSB channels, feedforward telemetry, status gauge | +87 / −25 |
| 2026-09-29 | [d3e61a53053](https://github.com/ElDominio/rusefi/commit/d3e61a53053b0c054c9974394f970ae87f268e16) — tps: add ExpAverage smoothing to TPS1/TPS2 with smart fast-transient reset | +90 / −5 |

Supporting configuration and source: [firmware/integration/rusefi_config.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/integration/rusefi_config.txt).
