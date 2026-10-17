# 51 — State-aware DFCO

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Master's overrun fuel cut becomes selectable Overrun, Decel or Both. Common guards remain separate from physical overrun detection, overrun can require a detected gear, and Decel uses state-machine deceleration with an RPM-rate fallback when vehicle speed is absent. P&B and active up/down shifts suppress the cut.

## ECU logic and behavior

1. Apply common eligibility guards separately from physical overrun classification.
2. Select Overrun, Decel or Both mode.
3. Optionally require a detected engaged gear for the overrun path.
4. Use the state-machine deceleration classification, including an RPM-rate fallback without vehicle speed.
5. Suppress the cut during P&B and active up/down shifts.

**Illustrative behavior:** A clutch-assisted shift can inhibit fuel cut even when throttle is closed, avoiding a conflict with the shift-assist torque request.

## Calibration, dependencies and review limits

This depends on state classification and shift behavior, not just renamed DFCO fields. Coverage extends fuel-cut tests.

Check missing-speed behavior, gear validity and re-entry after shift/P&B exits. The changed guards and new mode enum must be migrated with calibration and tests.

Existing branch test evidence: [unit_tests/tests/ignition_injection/test_fuelCut.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_fuelCut.cpp).

These test files change by +47 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 1,195 changed lines — 1,156 additions and 39 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+1,156 / −39**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 1 files are new since the common ancestor (+1,054 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp) | +1,054 / −0 | +1,054 / −0 |
| [firmware/controllers/algo/fuel/dfco.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/dfco.cpp) | +92 / −37 | +92 / −37 |
| [firmware/controllers/algo/fuel/dfco.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/dfco.h) | +10 / −2 | +10 / −2 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-20 | [652c6887882](https://github.com/ElDominio/rusefi/commit/652c6887882d6d3507be3eca9d1a52a1b6540b79) — Add optional gear-gated overrun fuel cut requirement | +5 / −0 |
| 2026-06-29 | [4e3cd5e6267](https://github.com/ElDominio/rusefi/commit/4e3cd5e62675132ef169269c87ed6d28682b8385) — Rework DFCO: split overrun/decel/both modes; refactor guards; gear-engaged check for overrun | +62 / −21 |
