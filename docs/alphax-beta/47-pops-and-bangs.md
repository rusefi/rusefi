# 47 — Pops and bangs

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

An overrun-driven state machine supplies delay/duration, RPM/temperature gates, optional Sport/cutout permission, an absolute timing override, VE override, additional air and optional periodic spark-cut windows. It inhibits DFCO while active. The branch's normal ignition path returns the P&B timing override early when eligible.

## ECU logic and behavior

1. Recognize eligible overrun and apply temperature/RPM and optional Sport/cutout permission.
2. Run configured start delay and active duration.
3. Provide absolute timing, VE and extra-air overrides, with optional periodic spark-cut windows.
4. Suppress DFCO while P&B is active so the requested fueled behavior is possible.
5. The normal ignition path returns the eligible P&B timing override early.

**Illustrative behavior:** Enabling P&B changes deceleration fuel-cut eligibility as well as timing; adopting only its timing field would not reproduce the behavior.

## Calibration, dependencies and review limits

This is a distinct feature from rolling launch or the existing anti-lag controller, although it uses shared spark-skip and air/fuel paths. It needs those integrations and its state-machine tests, not just the calibration fields.

Check entry/exit timing, spark-window duty and restoration of normal DFCO. Review absolute VE/timing units and the shared spark-skip consumer.

Existing branch test evidence: [unit_tests/tests/ignition_injection/test_pops_and_bangs.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_pops_and_bangs.cpp).

These test files change by +396 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 1,575 changed lines — 1,492 additions and 83 deletions across 6 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+1,599 / −85**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 1 files are new since the common ancestor (+1,054 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/idle_thread.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/idle_thread.cpp) | +264 / −32 | +264 / −34 |
| [firmware/controllers/algo/airmass/airmass.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/airmass/airmass.cpp) | +6 / −0 | +6 / −0 |
| [firmware/controllers/algo/engine2.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine2.cpp) | +17 / −1 | +124 / −1 |
| [firmware/controllers/algo/engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp) | +1,054 / −0 | +1,054 / −0 |
| [firmware/controllers/algo/fuel/dfco.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/dfco.cpp) | +92 / −37 | +92 / −37 |
| [firmware/controllers/algo/ignition/ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp) | +59 / −13 | +59 / −13 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +855 / −53 |
