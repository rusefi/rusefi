# 30 — Clutch delay valve

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

A new CDV module drives a solenoid around launch/pre-launch. Simple mode releases after a delay following launch exit; Smart mode requires a clutch-pressure window and uses a maximum active duration. Vehicle-speed, clutch-release and pressure-window exits can also end activation.

## ECU logic and behavior

1. Use launch/pre-launch state to determine when valve actuation can start.
2. In Simple mode, hold through the configured delay after launch exit.
3. In Smart mode, require the configured clutch-pressure window and enforce a maximum active interval.
4. Use vehicle speed, clutch-release and pressure conditions to end activation as applicable.
5. Publish the valve state and pressure through the added sensor/output integration.

**Illustrative behavior:** Smart mode can end the valve hold when pressure leaves the allowed window even if a time-only hold would still be active.

## Calibration, dependencies and review limits

The change includes a clutch-pressure sensor, output registration, live state and configuration. It depends on launch state but does not require the complete transmission controller.

Verify pressure units/polarity, switch edges and output release on each exit. This is clutch actuation and does not require enabling the automatic TCU.

Existing branch test evidence: [unit_tests/tests/controllers/modules/cdv_controller/test_cdv_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/controllers/modules/cdv_controller/test_cdv_controller.cpp).

These test files change by +149 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 102 changed lines — 102 additions and 0 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+102 / −0**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+98 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/modules/cdv_controller/cdv_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/cdv_controller/cdv_controller.cpp) | +81 / −0 | +81 / −0 |
| [firmware/controllers/modules/cdv_controller/cdv_controller.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/cdv_controller/cdv_controller.h) | +17 / −0 | +17 / −0 |
| [firmware/init/sensor/init_fluid_pressure.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_fluid_pressure.cpp) | +4 / −0 | +4 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +80 / −0 |
| 2026-06-18 | [c4780944936](https://github.com/ElDominio/rusefi/commit/c478094493637e473e83bbd9751b5142a03d2f8a) — Shift torque speed gate, eco mode MAP gate, CDV smart mode, launch clutch-up fix | +29 / −7 |
