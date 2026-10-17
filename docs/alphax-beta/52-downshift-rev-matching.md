# 52 — Downshift rev matching

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The new blipper latches a trustworthy pre-shift gear, computes the next lower gear's RPM at current speed, and commands ETB through opening, control and closing phases. It uses feed-forward plus PID, with driver-demand, RPM, speed, optional brake/Sport gates, timeouts and lockout.

## ECU logic and behavior

1. Latch a credible pre-shift gear before clutch/gear inference becomes ambiguous.
2. Use speed and the next lower gear ratio to compute matching RPM.
3. Qualify driver demand, RPM, speed and optional brake/Sport permission.
4. Progress through opening, feed-forward/PID control and closing phases with timeout/lockout.
5. Replace the pedal-derived ETB target at the shift-assist stage, then allow later limit/fault clamps to reduce it.

**Illustrative behavior:** A downshift requests the RPM implied by the lower gear at current speed, rather than a fixed throttle pulse for every shift.

## Calibration, dependencies and review limits

It depends on the state machine, calibrated gear ratios/speed and ETB. Its target replaces the pedal-derived target before later limp/rev-limit clamps.

Check stale gear/speed, driver override, unsuccessful shifts and target clamping. The LUT/gear calibration and state-machine shift detection are prerequisites.

Existing branch test evidence: [unit_tests/tests/actuators/test_downshift_blipper.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_downshift_blipper.cpp).

These test files change by +329 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 566 changed lines — 549 additions and 17 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+549 / −33**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+350 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/downshift_blipper.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/downshift_blipper.cpp) | +282 / −0 | +282 / −0 |
| [firmware/controllers/actuators/downshift_blipper.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/downshift_blipper.h) | +68 / −0 | +68 / −0 |
| [firmware/controllers/actuators/electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp) | +199 / −17 | +199 / −33 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +440 / −5 |
| 2026-06-22 | [d615a5789a1](https://github.com/ElDominio/rusefi/commit/d615a5789a1d8dd6828fe1f00bfff5b786ab5ee8) — Rework Downshift Blipper rev-match phasing; gate it with Lua instead of scaling | +44 / −25 |
| 2026-07-07 | [480a6f04ca3](https://github.com/ElDominio/rusefi/commit/480a6f04ca360aab17830411a0e599b1e6ab2a1f) — Wire Exhaust Cutout, Sport Pedal, Downshift Blipper and Limiter Adders to Sport Mode | +15 / −56 |
