# 23 — Cranking without ECU spark

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

A new option clears spark permission while the existing RPM state reports cranking. It is intended for an external ignition module that supplies its own crank spark. ECU spark resumes after the normal cranking transition; this is separate from master's kick-start behavior and does not itself disable injection.

## ECU logic and behavior

1. Read the existing RPM calculator's cranking state.
2. When the option is enabled and the engine is cranking, clear ECU spark permission with the added reason.
3. Allow normal spark permission evaluation again after the cranking transition.
4. Leave injection permission and the external ignition module's own crank spark outside this option.

**Illustrative behavior:** An ignition module can supply its own cranking sparks while ECU-controlled running ignition resumes after the normal state transition.

## Calibration, dependencies and review limits

It is contained in the protection/spark-permission path plus page-6 configuration and a new clear reason.

Check stop/start and cranking-threshold transitions, plus the actual external module handover. This is separate from kick-start cranking.

Existing branch test evidence: [unit_tests/tests/ignition_injection/test_cranking_no_spark.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_cranking_no_spark.cpp).

These test files change by +65 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 76 changed lines — 74 additions and 2 deletions across 1 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+75 / −26**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/limp_manager.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/limp_manager.cpp) | +74 / −2 | +75 / −26 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-08-30 | [5032f2e023f](https://github.com/ElDominio/rusefi/commit/5032f2e023fba3abc7ca3656bbbbe66722933132) — Add Cranking No-Spark feature for external ignition modules | +8 / −0 |
