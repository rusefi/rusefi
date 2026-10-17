# 46 — Ghost Cam idle mode

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch adds an idle-only lope mode selected by a switch or Sport mode. It changes VVT targets, AFR, idle target and base air, and swaps in a separate idle-ignition PID. The idle controller bypasses RPM-only coasting classification while active, allowing the intended RPM excursions without immediately disabling the mode.

## ECU logic and behavior

1. Qualify idle, temperature and switch/Sport permission.
2. Overlay VVT targets, AFR, target RPM and base air for the requested lope.
3. Select a separate idle-ignition PID and route its output directly while active.
4. Bypass RPM-only coasting classification so intended lope excursions do not immediately cancel the mode.
5. Restore/reset ordinary idle PID behavior on exit; optional target blending uses the shared slow-transition mechanism.

**Illustrative behavior:** An intentional RPM dip or rise during lope can remain inside Ghost Cam without being interpreted as a normal departure from idle solely because of RPM.

## Calibration, dependencies and review limits

Throttle/speed and temperature conditions still matter, and normal idle PID state is restored/reset on exit. Optional blending shares the Eco transition mechanism. This is a coordinated idle/VVT/fueling feature, not a trigger pattern.

Check throttle/speed exits, cam availability, PID reset and restored targets. This mode changes actual air/fuel/spark/VVT commands, not crank decoding.

Existing branch test evidence: [unit_tests/tests/engine_state_machine/test_engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/engine_state_machine/test_engine_state_machine.cpp).

These test files change by +1,742 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 1,794 changed lines — 1,727 additions and 67 deletions across 6 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+1,730 / −94**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 1 files are new since the common ancestor (+1,054 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp) | +199 / −17 | +199 / −33 |
| [firmware/controllers/actuators/idle_thread.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/idle_thread.cpp) | +264 / −32 | +264 / −34 |
| [firmware/controllers/actuators/vvt.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/vvt.cpp) | +108 / −5 | +109 / −10 |
| [firmware/controllers/algo/engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp) | +1,054 / −0 | +1,054 / −0 |
| [firmware/controllers/algo/fuel/fuel_computer.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/fuel_computer.cpp) | +43 / −0 | +45 / −4 |
| [firmware/controllers/algo/ignition/ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp) | +59 / −13 | +59 / −13 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-30 | [0f2920d82a8](https://github.com/ElDominio/rusefi/commit/0f2920d82a8a4bcdacd44d20e6178b0ce75ecd4c) — Add Ghost Cam Mode and Sport Pedal engine-state-machine tracking | +140 / −2 |
| 2026-07-06 | [4185dbc21de](https://github.com/ElDominio/rusefi/commit/4185dbc21de44c2367850b723c1aaac9fb2a69ce) — Route idle timing PID output directly while Ghost Cam is active | +9 / −0 |
| 2026-07-10 | [b99ea638683](https://github.com/ElDominio/rusefi/commit/b99ea638683b78ad1429c91fd3900b8ea5378793) — Ghost Cam: bypass Coasting classification while lope is active | +13 / −1 |
| 2026-08-26 | [decbc188c13](https://github.com/ElDominio/rusefi/commit/decbc188c1337af6065d40dc8e39d452e60cbeec) — Slow Eco Mode / Ghost Cam target transition | +87 / −24 |
