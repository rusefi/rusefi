# 15 — Manual fuel trim by bank

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Master has cylinder trims and closed-loop bank corrections. The branch adds tuner-entered RPM/load trim tables per fuel bank, converts their signed percentages into multipliers, and applies them in the fuel calculation. The tables share the fuel-trim axes.

## ECU logic and behavior

1. Interpolate a signed percentage from the RPM/load table assigned to the fuel bank.
2. Convert percentage to a multiplier and apply it in the fuel calculation.
3. Use the bank association rather than cylinder-specific tables to select this correction.
4. Keep manual bank trim separate from closed-loop bank correction and individual cylinder trims.

**Illustrative behavior:** A +5% entry means a 1.05 multiplier at that table point; it is not five milliseconds of added injector time.

## Calibration, dependencies and review limits

This is a relatively contained fueling feature, provided the table definitions, defaults, bank selection, TunerStudio editors and calculation move together. Coverage is added to fuel-math tests.

Verify bank assignment, shared axes and interaction with other multiplicative corrections. Test fixtures must initialize the underlying lambda/fuel tables correctly.

Existing branch test evidence: [unit_tests/tests/ignition_injection/test_fuel_math.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_fuel_math.cpp).

These test files change by +50 / −1 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 69 changed lines — 68 additions and 1 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+183 / −23**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/engine2.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine2.cpp) | +17 / −1 | +124 / −1 |
| [firmware/controllers/algo/fuel_math.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel_math.cpp) | +50 / −0 | +58 / −22 |
| [firmware/controllers/algo/fuel_math.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel_math.h) | +1 / −0 | +1 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-08-24 | [676c9c7697a](https://github.com/ElDominio/rusefi/commit/676c9c7697ad3bbaa8a1388770c20372ee37755f) — Add manual per-bank fuel trim table | +14 / −1 |
