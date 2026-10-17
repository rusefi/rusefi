# 54 — Launch power ramp

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The new module starts a time-indexed ignition-retard curve when launch control releases at WOT. Negative values are clamped away, and once started the curve runs to completion independently of later throttle movement.

## ECU logic and behavior

1. Detect the falling edge of active launch control.
2. Start only when driver throttle intent meets the configured WOT threshold at that release edge.
3. Interpolate time-indexed ignition retard until the final time bin is passed.
4. Clamp negative curve values to zero.
5. Once started, finish the time curve regardless of later throttle movement; disabling the feature clears active state.

**Illustrative behavior:** Lifting slightly after launch does not by itself restart or abort an already active power-ramp curve.

## Calibration, dependencies and review limits

The calculation is small but consumes launch state and modifies the common ignition correction. It is separate from rolling launch and flat-shift torque reduction.

Check launch-release edge detection, repeated launches and the last-bin boundary. This applies retard only and does not directly command fuel or throttle.

Existing branch test evidence: [unit_tests/tests/test_launch_power_ramp.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_launch_power_ramp.cpp).

These test files change by +99 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 152 changed lines — 139 additions and 13 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+139 / −13**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+80 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/ignition/ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp) | +59 / −13 | +59 / −13 |
| [firmware/controllers/modules/launch_power_ramp/launch_power_ramp.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/launch_power_ramp/launch_power_ramp.cpp) | +58 / −0 | +58 / −0 |
| [firmware/controllers/modules/launch_power_ramp/launch_power_ramp.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/launch_power_ramp/launch_power_ramp.h) | +22 / −0 | +22 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +134 / −12 |
