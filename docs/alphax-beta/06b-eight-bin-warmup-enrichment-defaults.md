# 06b — Eight-bin warmup enrichment defaults

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Eight-bin coolant-enrichment builds gain populated default axes and multipliers. Previously this configuration missed the sixteen-bin-only initializer and retained zero-initialized values.

## ECU logic and behavior

1. Default initialization now has an explicit CLT_CURVE_SIZE == 8 branch.
2. It initializes temperature bins at -40, -20, 0, 20, 40, 60, 80 and 100 degrees C.
3. The corresponding multipliers are 1.50, 1.42, 1.28, 1.12, 1.06, 1.03, 1 and 1.
4. This takes effect when defaults are created; it does not continuously overwrite a loaded enrichment tune.

**Illustrative behavior:** With the new defaults, a cold 0-degree-C coolant reading selects a 1.28 warmup multiplier on an eight-bin build.

## Calibration, dependencies and review limits

This corrects default tune construction on reduced-table boards, including the Paralela configuration. It is separable from the fuel-control algorithms and custom-page architecture.

Verify both eight- and sixteen-bin builds and preserve user tune values during normal load. The same source file also contains a P&B default unrelated to this fix.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 36 changed lines — 36 additions and 0 deletions across 1 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+36 / −0**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/defaults/default_fuel.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/defaults/default_fuel.cpp) | +36 / −0 | +36 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-09-23 | [579681192f1](https://github.com/ElDominio/rusefi/commit/579681192f1a8aa65047683df4402b39436482e2) — Add warmup fuel enrichment default for CLT_CURVE_SIZE==8 boards | +32 / −0 |
