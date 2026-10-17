# 49 — Misfire configuration and activation changes

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Misfire detection itself already exists in master. The branch moves its calibration to page 6 and changes monitoring from the idle controller's idle flag to the new state machine's Idle condition, excluding Quick Warmup. The adaptive detector remains recognizably the existing algorithm.

## ECU logic and behavior

1. Read relocated misfire calibration from page 6.
2. Require the state machine's Idle condition for monitoring.
3. Exclude Quick Warmup from that monitoring condition.
4. Continue using the existing adaptive detector and expose its result to CEL escalation.

**Illustrative behavior:** The same RPM irregularity may be monitored in normal idle but deliberately excluded during Quick Warmup.

## Calibration, dependencies and review limits

This is a configuration/dependency change rather than a wholly new misfire detector. Updated tests set the new configuration/state.

Check startup, idle entry/exit and configuration migration. This is an activation/calibration change to an existing detector, not evidence of a new general driving-condition misfire algorithm.

Existing branch test evidence: [unit_tests/tests/test_misfire_detection.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_misfire_detection.cpp).

These test files change by +33 / −28 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 68 changed lines — 31 additions and 37 deletions across 2 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+32 / −38**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/misfire_detection.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/misfire_detection.cpp) | +19 / −29 | +20 / −30 |
| [firmware/controllers/algo/misfire_detection.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/misfire_detection.h) | +12 / −8 | +12 / −8 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +359 / −0 |
