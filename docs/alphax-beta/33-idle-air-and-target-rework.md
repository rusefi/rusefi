# 33 — Idle air and target rework

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch changes A/C idle RPM from an absolute floor to an adder, replaces the fixed A/C air offset with a pressure curve, adds three external idle-up switches, and adds selectable cranking air, cranking RPM flare and a CLT-based hold before taper. With flare enabled, cranking air becomes a fading addition to running air. The old idle-return-target ramp is removed from the calculation.

## ECU logic and behavior

1. Build the running idle target with the new A/C RPM adder and external idle-up inputs.
2. Use pressure-dependent A/C air compensation instead of a fixed offset.
3. Apply selected cranking air, flare and CLT hold/taper behavior; flare treats cranking air as an addition to running air.
4. Hold and decay the off-idle RPM addition, cancelling it on re-crank. The generic iacByTpsTaper = 2 default is removed, and affected presets are cleaned up.
5. Use clutch/neutral override and a latch to retain closed-loop idle at speed without gear-detection chatter.
6. The separate fixed cranking ETB target is suppressed at vehicle speed of 2 km/h or more.

**Illustrative behavior:** An A/C setting of 100 RPM now means add 100 RPM to the base target, rather than treating 100 RPM as an absolute minimum target.

## Calibration, dependencies and review limits

An off-idle controller holds an RPM addition, waits for stability, then decays it; re-cranking cancels it. A clutch/neutral override can retain closed-loop idle at vehicle speed, with a latch to prevent gear-detection chatter. Fixed cranking TPS is a separate ETB override, suppressed at vehicle speeds of 2 km/h or more. These are several extractable subchanges, but they share phase/target logic and later mode overlays.

Review calibration migration and transitions among cranking, idle, coasting and running. Mode overlays such as Ghost Cam and Quick Warmup add further air/timing behavior after these base changes.

Existing branch test evidence: [unit_tests/tests/actuators/test_etb.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_etb.cpp), [unit_tests/tests/test_idle_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_idle_controller.cpp).

These test files change by +814 / −80 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 539 changed lines — 486 additions and 53 deletions across 4 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+486 / −76**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp) | +199 / −17 | +199 / −33 |
| [firmware/controllers/actuators/idle_thread.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/idle_thread.cpp) | +264 / −32 | +264 / −34 |
| [firmware/controllers/actuators/idle_thread.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/idle_thread.h) | +22 / −3 | +22 / −8 |
| [firmware/controllers/math/closed_loop_idle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/math/closed_loop_idle.cpp) | +1 / −1 | +1 / −1 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +224 / −28 |
| 2026-06-30 | [1d8f1507f8e](https://github.com/ElDominio/rusefi/commit/1d8f1507f8ebf3a2c16c21ae5c158a0023e85a92) — Fix cranking idle RPM flare: cranking air is an adder, not exclusive | +2 / −1 |
| 2026-07-02 | [b1e76378ec0](https://github.com/ElDominio/rusefi/commit/b1e76378ec0db4239ad1b4657e435bac6fc50067) — Abort off-idle RPM adder on re-crank from any state | +8 / −0 |
| 2026-07-06 | [4185dbc21de](https://github.com/ElDominio/rusefi/commit/4185dbc21de44c2367850b723c1aaac9fb2a69ce) — Route idle timing PID output directly while Ghost Cam is active | +9 / −0 |
| 2026-08-03 | [8342fa07833](https://github.com/ElDominio/rusefi/commit/8342fa07833c9056a6a06305b2ea7a3a84bfee68) — A/C idle: pressure-based adder table + RPM target -> RPM adder | +16 / −6 |
| 2026-08-23 | [e42ddbb2678](https://github.com/ElDominio/rusefi/commit/e42ddbb2678889194f289105dbf8b07990623f14) — Add ETB-only "TPS Target for Cranking" override; fix pre-existing GPPWM_BoostTarget enum gap | +8 / −0 |
| 2026-08-23 | [4497813a315](https://github.com/ElDominio/rusefi/commit/4497813a315a54dbc55fd9cb6f19e76250045d63) — Idle: add clutch/neutral override for the maxIdleVss closed-loop gate | +13 / −1 |
| 2026-08-30 | [5103bce05aa](https://github.com/ElDominio/rusefi/commit/5103bce05aaad4bd726df8f43a61d5b16ab835da) — Fix idle-while-cruising Idling/Running chatter (VSS gate neutral override) | +25 / −3 |
| 2026-08-30 | [41a5e0d6dcf](https://github.com/ElDominio/rusefi/commit/41a5e0d6dcfcb5320efa7b2e46b79a114bad8614) — Suppress cranking ETB TPS-target blip while vehicle is moving | +9 / −2 |
