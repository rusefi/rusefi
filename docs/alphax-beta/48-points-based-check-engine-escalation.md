# 48 — Points-based check-engine escalation

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Master already has range checks, DTC handling and a malfunction indicator. The branch adds debounced TPS circuit/intermittency checks and a point total including existing voltage-range faults and latched misfire. Thresholds control added TPS DTCs and flashing escalation; existing raw range checks remain independently active. The key-on/engine-off lamp check becomes configurable.

## ECU logic and behavior

1. Debounce added TPS circuit/intermittency checks and combine points with existing voltage-range faults and latched misfire.
2. Compare the total with escalation thresholds and generate the associated TPS DTCs/flashing behavior.
3. Keep pre-existing raw range checks independently active.
4. Use configurable key-on/engine-off bulb-check behavior.
5. Do not interpret reserved CEL enable fields as implemented checks when updateCheckEngineTriggering has no corresponding logic.

**Illustrative behavior:** A page-6 oil-pressure diagnostic checkbox is not evidence of a functioning new oil-pressure check when no corresponding code contributes points.

## Calibration, dependencies and review limits

Several page-6 CEL enable fields are reserved without matching new checks in `updateCheckEngineTriggering()`, so the field list must not be advertised as implemented oil-pressure/AFR/temperature diagnostics. The diff also gates DTC-manager compilation and SD handling with `EFI_TOOTH_LOGGER`.

Check debounce/reset, simultaneous faults and flashing-to-limp propagation. The tooth-logger compilation guard is *INFRASTRUCTURE* within this otherwise operational diagnostic change.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 255 changed lines — 234 additions and 21 deletions across 5 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+282 / −219**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/modules/check_engine_light/check_engine_light.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/check_engine_light/check_engine_light.cpp) | +141 / −18 | +141 / −18 |
| [firmware/controllers/modules/check_engine_light/check_engine_light.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/check_engine_light/check_engine_light.h) | +57 / −1 | +57 / −1 |
| [firmware/controllers/modules/dtc_manager/dtc_manager.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/dtc_manager/dtc_manager.cpp) | +1 / −1 | +1 / −1 |
| [firmware/controllers/modules/malfunction_indicator/malfunction_indicator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/malfunction_indicator/malfunction_indicator.cpp) | +29 / −1 | +74 / −162 |
| [firmware/controllers/modules/malfunction_indicator/malfunction_indicator.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/malfunction_indicator/malfunction_indicator.h) | +6 / −0 | +9 / −37 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-20 | [592e3812699](https://github.com/ElDominio/rusefi/commit/592e38126994a8b0439afa32b908e96656205c0e) — Add Check Engine Triggering: points-gated CEL with TPS Stuck High/Low | +125 / −2 |
| 2026-08-16 | [82134fad194](https://github.com/ElDominio/rusefi/commit/82134fad194b7435af15f46ee278fe812c434d5c) — DTC manager: gate compilation on EFI_TOOTH_LOGGER | +1 / −1 |
| 2026-08-17 | [341e8a63ae5](https://github.com/ElDominio/rusefi/commit/341e8a63ae529a0d17733987d946de6c3d8c1e39) — check_engine_light: gate MisfireController usage behind EFI_MISFIRE_DETECTION | +2 / −0 |

Related integration/history commits outside the measured source-file scope:

- [a57c6889221 — Malfunction Indicator: Key-On-Engine-Off bulb check](https://github.com/ElDominio/rusefi/commit/a57c6889221f07f357c575033ac831806981a4e0): Key-on/engine-off bulb check in the earlier gauges/malfunction_indicator.cpp location.
