# 45 — Quick warmup

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Cold idle can activate coordinated additional air, an absolute lambda target and an absolute idle timing value. The final timing behavior replaces the idle timing curve when separate idle advance is enabled; it is not the earlier historical “timing retard” design.

## ECU logic and behavior

1. Qualify the cold-idle operating condition in the state machine.
2. Add configured warmup air and select the warmup lambda target.
3. When separate idle advance is enabled, replace the idle timing curve with an absolute warmup timing value.
4. Stop applying the coordinated mode when its qualifying state ends.
5. The final absolute-timing implementation supersedes the earlier retard-based design.

**Illustrative behavior:** A warmup timing setting of 5 degrees requests that absolute idle advance when eligible; it does not subtract five degrees from the ordinary timing curve.

## Calibration, dependencies and review limits

This requires the state machine and its cold/idle classification, plus the air/fuel/ignition consumers. The absolute-timing gate matters when comparing tunes that do not use separate idle advance.

Check cold-to-warm and idle-to-running transitions, plus the separate-idle-advance enable. A tune lacking that enable does not receive this timing override.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 1,465 changed lines — 1,420 additions and 45 deletions across 4 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+1,422 / −51**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 1 files are new since the common ancestor (+1,054 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/idle_thread.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/idle_thread.cpp) | +264 / −32 | +264 / −34 |
| [firmware/controllers/algo/engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp) | +1,054 / −0 | +1,054 / −0 |
| [firmware/controllers/algo/fuel/fuel_computer.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/fuel_computer.cpp) | +43 / −0 | +45 / −4 |
| [firmware/controllers/algo/ignition/ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp) | +59 / −13 | +59 / −13 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +838 / −32 |
| 2026-08-24 | [f3171b5b2f7](https://github.com/ElDominio/rusefi/commit/f3171b5b2f75d7a3f148d236f85133c2399ea907) — Quick Warmup: replace timing retard with absolute idle timing override | +5 / −5 |
