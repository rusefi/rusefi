# 44 — Eco operating mode

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

After qualified cruising, the branch can blend toward an absolute AFR target, ignition addition, optional VVT targets, a throttle multiplier and alternator-voltage target. MAP, speed, RPM, switch/Lua inhibition and operating-state checks govern entry/exit. A post-engagement lock avoids reacting to transients caused by Eco's own actuation; Limp/Sport/inhibition still take priority.

## ECU logic and behavior

1. Require qualified cruising plus configured MAP, speed, RPM and inhibit conditions.
2. On entry, select Eco AFR, ignition addition, optional VVT targets, throttle multiplier and charging target.
3. Optionally ramp the targets over one second.
4. Use a post-entry lock to tolerate state disturbance caused by Eco's own actuation.
5. Retain higher-priority Limp, Sport and explicit inhibition exits.

**Illustrative behavior:** Reducing throttle on Eco entry can itself disturb RPM/load; the lock prevents that disturbance alone from immediately toggling Eco off and on.

## Calibration, dependencies and review limits

Optional slow transitions ramp target blends over one second. This feature crosses fueling, ignition, throttle, VVT and charging, so extracting only the state-machine gate would not reproduce it.

Check target blending and priority across all consumers. AFR is an absolute target while ignition is an addition, so the settings use different semantics.

Existing branch test evidence: [unit_tests/tests/ignition_injection/test_eco_mode.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_eco_mode.cpp).

These test files change by +374 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 1,579 changed lines — 1,532 additions and 47 deletions across 6 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+1,535 / −72**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 1 files are new since the common ancestor (+1,054 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/alternator_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/alternator_controller.cpp) | +69 / −12 | +69 / −12 |
| [firmware/controllers/actuators/electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp) | +199 / −17 | +199 / −33 |
| [firmware/controllers/actuators/vvt.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/vvt.cpp) | +108 / −5 | +109 / −10 |
| [firmware/controllers/algo/engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp) | +1,054 / −0 | +1,054 / −0 |
| [firmware/controllers/algo/fuel/fuel_computer.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/fuel_computer.cpp) | +43 / −0 | +45 / −4 |
| [firmware/controllers/algo/ignition/ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp) | +59 / −13 | +59 / −13 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +865 / −23 |
| 2026-06-18 | [c4780944936](https://github.com/ElDominio/rusefi/commit/c478094493637e473e83bbd9751b5142a03d2f8a) — Shift torque speed gate, eco mode MAP gate, CDV smart mode, launch clutch-up fix | +10 / −7 |
| 2026-07-10 | [b9fe5f780c2](https://github.com/ElDominio/rusefi/commit/b9fe5f780c245a668f669d10f554ff98a70f8793) — Eco Mode: don't drop on transient RPM noise (AE blips + own actuation) | +24 / −5 |
| 2026-08-17 | [a856567c22c](https://github.com/ElDominio/rusefi/commit/a856567c22c604746d251b170ea8260a60e20f30) — Eco Mode: add RPM ceiling gate + post-engage state-change lock | +33 / −3 |
| 2026-08-26 | [decbc188c13](https://github.com/ElDominio/rusefi/commit/decbc188c1337af6065d40dc8e39d452e60cbeec) — Slow Eco Mode / Ghost Cam target transition | +74 / −18 |
