# 17 — Manual injector pressure correction

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Instead of the sensed-pressure square-root flow adjustment, a new mode applies a user table indexed by differential fuel pressure and requested fuel mass. It keeps the nominal flow ratio at 1, multiplies the base duration by the table value, then adds deadtime. It still requires the pressure-sensor path.

## ECU logic and behavior

1. Use the sensed differential fuel-pressure path to select a calibration-table row.
2. Use requested fuel mass for the other table axis.
3. In this mode the nominal flow ratio stays at one; multiply base duration by the interpolated factor.
4. Add injector deadtime after the duration correction.
5. Default factors of one preserve nominal duration, but valid pressure inputs remain necessary.

**Illustrative behavior:** A table factor of 1.10 adds 10% to base duration before deadtime, rather than increasing the whole final pulse by 10%.

## Calibration, dependencies and review limits

The default table is unity and the default dimensions are small; board overrides can enlarge it. The injector-compensation enum gains a wider bit field, so this includes configuration-layout changes.

Verify the pressure reference, units, invalid-pressure handling and deadtime order. The widened mode bit field is a tune-layout change.

Existing branch test evidence: [unit_tests/tests/ignition_injection/test_injector_model.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_injector_model.cpp).

These test files change by +42 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 31 changed lines — 29 additions and 2 deletions across 1 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+29 / −2**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/fuel/injector_model.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/injector_model.cpp) | +29 / −2 | +29 / −2 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-08-13 | [255f5dbe656](https://github.com/ElDominio/rusefi/commit/255f5dbe6564ccb68e9e4d2a923b31754325064b) — Add Manual Pressure Correction injector compensation mode | +13 / −2 |
| 2026-08-14 | [3bb7ddda156](https://github.com/ElDominio/rusefi/commit/3bb7ddda156bf7c78e89651de83eda98e127aeba) — Add missing unit test for Manual Pressure Correction, document build gotchas | +0 / −0 |

Supporting configuration and source: [firmware/integration/rusefi_config.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/integration/rusefi_config.txt).
