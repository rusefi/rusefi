# 20 — Time-based WOT enrichment

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The new WOT module looks up an AFR adder from elapsed time in the engine state machine's WOT condition, capped at 30 seconds. Leaving WOT resets the timer. The adjusted AFR is also reflected in the published lambda target.

## ECU logic and behavior

1. Read the state machine's WOT flag rather than independently classifying throttle position.
2. When disabled or outside WOT, reset the timer and AFR adder.
3. During WOT, cap elapsed time at 30 seconds and interpolate the AFR-adder curve.
4. The fuel-target calculation applies the adder and publishes the resulting lambda target.

**Illustrative behavior:** Two WOT bursts separated by a non-WOT interval restart the curve instead of accumulating total WOT time.

## Calibration, dependencies and review limits

The calculation is small, but it depends on the state machine, page 6 and the fuel-target pipeline. The diff has no dedicated WOT-enrichment test file.

Check AFR-adder sign and state-machine WOT thresholds. An AFR addition is not a fuel-percentage addition; no dedicated new WOT-enrichment test is present.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 110 changed lines — 110 additions and 0 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+112 / −4**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+67 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/fuel/fuel_computer.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/fuel_computer.cpp) | +43 / −0 | +45 / −4 |
| [firmware/controllers/modules/wot_enrichment/wot_enrichment.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/wot_enrichment/wot_enrichment.cpp) | +40 / −0 | +40 / −0 |
| [firmware/controllers/modules/wot_enrichment/wot_enrichment.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/wot_enrichment/wot_enrichment.h) | +27 / −0 | +27 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +96 / −0 |
