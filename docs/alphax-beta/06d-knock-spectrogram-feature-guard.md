# 06d — Knock spectrogram feature guard

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

*INFRASTRUCTURE* — firmware support with an ECU runtime consequence.

## Executive summary

The branch corrects the build-time guard for optional knock spectrum processing. The resulting ECU image now respects a FALSE feature setting.

## ECU logic and behavior

1. An undefined KNOCK_SPECTROGRAM now receives a FALSE default.
2. Numeric #if guards replace #ifdef tests around FFT/spectrogram code.
3. A board explicitly defining the feature as FALSE consequently excludes that optional path.
4. The normal knock signal processing must be distinguished from the optional spectrum diagnostic.

**Illustrative behavior:** A build using -DKNOCK_SPECTROGRAM=FALSE no longer includes the spectrum path simply because the macro exists.

## Calibration, dependencies and review limits

This affects which diagnostic code executes on the ECU and its resource use. It is marked infrastructure because it changes feature selection rather than the engine-control law.

Compile with the macro absent, FALSE and TRUE. This is a compile-time diagnostic selection fix, not a new acoustic knock-retard strategy.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 15 changed lines — 11 additions and 4 deletions across 1 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+39 / −57**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/sensors/impl/software_knock.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/impl/software_knock.cpp) | +11 / −4 | +39 / −57 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-09-23 | [c3345b58b60](https://github.com/ElDominio/rusefi/commit/c3345b58b609a9106be343b33e809238ac440fa5) — Fix KNOCK_SPECTROGRAM tristate DDEFS check (#ifdef -> #if) | +11 / −4 |
