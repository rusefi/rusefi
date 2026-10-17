# 18 — VVT-dependent fuel and ignition corrections

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch adds intake/exhaust cam-angle versus RPM correction tables. Fuel percentages multiply the normal running-fuel correction; ignition values add to advance corrections. The current implementation reads bank 0 cam positions, rather than implementing independent corrections for every bank.

## ECU logic and behavior

1. Read intake/exhaust cam position from bank 0.
2. Interpolate cam-angle/RPM corrections for available positions.
3. Convert fuel percentages into multiplicative running-fuel corrections.
4. Add timing corrections to the advance calculation.
5. Invalid cam positions contribute no correction; this does not synthesize bank-specific cam measurements.

**Illustrative behavior:** Changing cam position can alter fueling and spark even when RPM and the main load table lookup stay unchanged.

## Calibration, dependencies and review limits

This feature is separate from the VVT actuator PID redesign in section 56. It needs table/default/UI integration and valid cam positions; invalid positions contribute no correction.

Check angle sign/reference, invalid-position behavior and bank-0 assumptions. Keep these compensation tables separate from the VVT actuator PID redesign.

Existing branch test evidence: [unit_tests/tests/test_vvt_corrections.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_vvt_corrections.cpp).

These test files change by +108 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 122 changed lines — 109 additions and 13 deletions across 2 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+117 / −35**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/fuel_math.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel_math.cpp) | +50 / −0 | +58 / −22 |
| [firmware/controllers/algo/ignition/ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp) | +59 / −13 | +59 / −13 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +89 / −12 |
