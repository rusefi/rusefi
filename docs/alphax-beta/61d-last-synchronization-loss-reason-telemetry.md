# 61d — Last synchronization-loss reason telemetry

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The ECU records and publishes the last trigger synchronization-loss reason, improving diagnosis without introducing a new crank pattern.

## ECU logic and behavior

1. Record a reason when the common trigger logic loses synchronization.
2. Retain that reason in trigger state for publishing through telemetry/logging.
3. Expose the last event reason so a log can distinguish failure paths.
4. Do not treat the diagnostic channel itself as a change to the acceptance criteria for a trigger pattern.

**Illustrative behavior:** After a sync-loss event, a logged reason can explain which decoder path dropped synchronization even when RPM later recovers.

## Calibration, dependencies and review limits

This is a small operational diagnostic addition that can be reviewed independently of the Mitsubishi decoder experiments.

Check reason assignment and reset/retention semantics for each loss path. The shared trigger-file totals contain much larger decoder changes; the focused commit isolates this diagnostic.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 449 changed lines — 441 additions and 8 deletions across 4 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+443 / −45**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/trigger/trigger_central.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_central.cpp) | +219 / −3 | +221 / −12 |
| [firmware/controllers/trigger/trigger_central.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_central.h) | +44 / −1 | +44 / −1 |
| [firmware/controllers/trigger/trigger_decoder.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_decoder.cpp) | +132 / −3 | +132 / −31 |
| [firmware/controllers/trigger/trigger_decoder.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_decoder.h) | +46 / −1 | +46 / −1 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-08-25 | [5934d72729a](https://github.com/ElDominio/rusefi/commit/5934d72729afdf9666ba820145fda03cddc7ca44) — Add trigger last-sync-loss-reason logged channel | +17 / −0 |
