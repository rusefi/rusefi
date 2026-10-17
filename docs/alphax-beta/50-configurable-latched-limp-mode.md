# 50 — Configurable latched limp mode

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

With the new state machine enabled, an ETB jam latches configurable RPM, throttle, boost-target, timing and fueling responses instead of only master's fixed ETB-jam response. Flashing CEL escalation can also latch this mode. The ordinary rev limiter, launch and transient fuel/spark cuts do not themselves imply limp.

## ECU logic and behavior

1. Latch the state-machine limp response on ETB jam or flashing CEL escalation.
2. Apply configured RPM, throttle and boost-target limits plus timing/fueling responses through their consumers.
3. Keep the latch until a power cycle rather than clearing it when the initiating condition briefly disappears.
4. Suppress lower-priority operating modes.
5. Retain the older ETB-jam fallback when the state-machine path is unavailable or disabled.

**Illustrative behavior:** A transient ordinary rev-limit cut does not itself latch limp, whereas a qualifying jam can leave the ECU limited after the immediate jam signal clears.

## Calibration, dependencies and review limits

The latch persists until power cycle and mode priority is enforced across multiple consumers. The old jam response remains as fallback when the state machine is unavailable/disabled. Because this changes fault response, it needs an independent review even if the mode framework is adopted.

Check fault activation, latch lifetime, disabled-state-machine fallback and each consumer. An open-loop boost map that does not use target pressure may not reduce duty when the target is capped.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 1,508 changed lines — 1,466 additions and 42 deletions across 7 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+1,469 / −90**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 1 files are new since the common ancestor (+1,054 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/boost_control.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/boost_control.cpp) | +32 / −10 | +32 / −10 |
| [firmware/controllers/actuators/electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp) | +199 / −17 | +199 / −33 |
| [firmware/controllers/algo/engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp) | +1,054 / −0 | +1,054 / −0 |
| [firmware/controllers/algo/fuel/fuel_computer.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/fuel_computer.cpp) | +43 / −0 | +45 / −4 |
| [firmware/controllers/algo/ignition/ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp) | +59 / −13 | +59 / −13 |
| [firmware/controllers/limp_manager.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/limp_manager.cpp) | +74 / −2 | +75 / −26 |
| [firmware/controllers/limp_manager.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/limp_manager.h) | +5 / −0 | +5 / −4 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +866 / −17 |
