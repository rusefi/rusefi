# 22 — Priming after trigger teeth

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Master schedules priming after a time delay. The branch adds a selectable mode that arms at ignition-on and fires after a configured number of raw trigger events, without waiting for synchronization. Zero teeth is treated as one; ignition-off disarms the pending prime.

## ECU logic and behavior

1. In tooth-count mode, ignition-on arms a pending priming request.
2. Raw trigger events advance the count before full synchronization is available.
3. A configured count of zero is treated as one.
4. The pending prime fires through the existing priming conditions after the threshold and disarms on ignition-off.
5. The time-delay mode remains a separate selectable path.

**Illustrative behavior:** The first crank movement can trigger priming after a few edges without waiting for the decoder to establish sequential phase.

## Calibration, dependencies and review limits

This is an isolated priming change with an additional trigger callback and tune fields. The existing flood-clear/fresh-start conditions remain part of the path.

Check flood clear, fresh-start eligibility, ignition cycling and exactly-once firing. Counting raw edges is not equivalent to counting confirmed engine revolutions.

Existing branch test evidence: [unit_tests/tests/ignition_injection/test_startOfCrankingPrimingPulse.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_startOfCrankingPrimingPulse.cpp).

These test files change by +64 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 269 changed lines — 259 additions and 10 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+261 / −19**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/engine_cycle/prime_injection.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/engine_cycle/prime_injection.cpp) | +32 / −7 | +32 / −7 |
| [firmware/controllers/engine_cycle/prime_injection.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/engine_cycle/prime_injection.h) | +8 / −0 | +8 / −0 |
| [firmware/controllers/trigger/trigger_central.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_central.cpp) | +219 / −3 | +221 / −12 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-08-15 | [a358f82a3b9](https://github.com/ElDominio/rusefi/commit/a358f82a3b9aa56297563d40e29bda15b4494983) — Priming pulse: fire on trigger-tooth count instead of fixed delay | +46 / −7 |
