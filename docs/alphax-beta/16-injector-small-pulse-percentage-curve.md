# 16 — Injector small-pulse percentage curve

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The existing injector nonlinearity modes gain `Curve (%)`. Below the final pulse-width bin, an interpolated percentage multiplies the theoretical pulse width; at or above the last bin the correction is bypassed. The curve is on custom page 6 and gated by `EFI_INJ_PERCENT_CURVE`.

## ECU logic and behavior

1. Select the percentage-curve nonlinearity mode.
2. Below the final pulse-width bin, interpolate the correction percentage and multiply theoretical pulse width.
3. At or above the final bin, bypass this small-pulse correction.
4. Carry the custom-page curve and compile-time feature gate with the injector-model change.

**Illustrative behavior:** A 110% entry increases the theoretical small pulse by 10%; a pulse beyond the curve's final bin uses the uncorrected value.

## Calibration, dependencies and review limits

This can be separated from the manual pressure-correction mode, but both modify the same injector model. The added injector test in this diff targets manual pressure correction rather than establishing coverage of the percentage-curve mode.

Check continuity at the final bin and the mode enum/layout migration. The added manual-pressure test does not establish percentage-curve coverage.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

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
| 2026-06-29 | [f9073947234](https://github.com/ElDominio/rusefi/commit/f9073947234e82853008a5fb60839ce506d0983b) — Add Injector Small Pulse % Correction Curve: nonlinear flow correction via user-supplied PW vs. percent curve (EFI_INJ_PERCENT_CURVE) | +16 / −0 |
