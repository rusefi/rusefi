# 06c — Lua limiter-adder axis defaults

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch initializes previously unseeded Lua limiter-adder axes so default calibration curves can be interpolated meaningfully.

## ECU logic and behavior

1. Custom-page initialization seeds the Lua-gauge axes used by RPM- and boost-limit adder curves.
2. Ordered axes allow interpolation across the configured gauge range.
3. The curves remain additions to existing limits; initialized axes alone do not enable Sport or create a new limiter.
4. Existing tune values and the consumer logic must be reviewed separately from this default initialization.

**Illustrative behavior:** A tuner adjusting a limiter-adder curve starts from ordered bins instead of an all-zero axis.

## Calibration, dependencies and review limits

The operational limiter changes are covered separately in report 34. This report isolates default values; the focused commit is much smaller than the shared initializer file.

Check axis ordering after default initialization and the separate enable/source selection. The full custom_page.cpp count includes defaults for almost every AlphaX feature.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 362 changed lines — 362 additions and 0 deletions across 1 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+362 / −0**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 1 files are new since the common ancestor (+362 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/custom_page.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/custom_page.cpp) | +362 / −0 | +362 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-09-23 | [cf5a5768f4f](https://github.com/ElDominio/rusefi/commit/cf5a5768f4f19554a213a58806593db72d01f111) — Seed a default axis for the Lua Limiter Adders curves | +10 / −0 |
