# 43 — Sport mode and pedal shaping

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Sport mode is a shared flag selected by hardware switch or Lua-gauge threshold. It can drive pedal shaping, exhaust cutout, downshift permission, limiter adders, Ghost Cam and pops-and-bangs permission. Limp suppresses Sport, and Sport suppresses Eco.

## ECU logic and behavior

1. Read Sport selection from its switch or Lua-gauge threshold.
2. Apply mode priority: limp suppresses Sport and Sport suppresses Eco.
3. Expose permission to cutout, blipper, limiter additions, Ghost Cam and P&B consumers.
4. For Sport Pedal, multiply pedal-derived target by the pedal-indexed curve before idle-range compression and clamp the result.
5. A separate pedal-shaping switch can select that behavior independently where configured.

**Illustrative behavior:** Sport selection can change pedal response and allow a downshift blip without substituting an entirely separate engine fuel map.

## Calibration, dependencies and review limits

Sport Pedal separately multiplies the pedal-derived throttle target by a pedal-indexed curve before idle-range compression, with a clamp. It can also use its own switch. These are two connected features, rather than a second complete throttle map.

Review each enabled consumer and mode exit. A Sport flag must not be mistaken for a request to bypass later throttle fault or RPM-limit clamps.

Existing branch test evidence: [unit_tests/tests/actuators/test_etb.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_etb.cpp).

These test files change by +327 / −18 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 1,736 changed lines — 1,717 additions and 19 deletions across 4 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+1,718 / −59**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+1,444 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp) | +199 / −17 | +199 / −33 |
| [firmware/controllers/actuators/exhaust_cutout.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/exhaust_cutout.cpp) | +390 / −0 | +390 / −0 |
| [firmware/controllers/algo/engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp) | +1,054 / −0 | +1,054 / −0 |
| [firmware/controllers/limp_manager.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/limp_manager.cpp) | +74 / −2 | +75 / −26 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-29 | [135f45852f6](https://github.com/ElDominio/rusefi/commit/135f45852f690816fc92d13a8bc29839b414df8a) — Misc actuator/controller fixes: sport pedal live bit, upshift flat-shift priority, gear detector clutch-aware, fuel pump no-sensor fast path, etbSplit default | +2 / −1 |
| 2026-06-30 | [0f2920d82a8](https://github.com/ElDominio/rusefi/commit/0f2920d82a8a4bcdacd44d20e6178b0ce75ecd4c) — Add Ghost Cam Mode and Sport Pedal engine-state-machine tracking | +75 / −1 |
| 2026-07-07 | [bad3e839579](https://github.com/ElDominio/rusefi/commit/bad3e839579af16cc3d6d1550bc34ed5831164fa) — Add Sport Mode overlay to Engine State Machine | +79 / −73 |
| 2026-07-07 | [480a6f04ca3](https://github.com/ElDominio/rusefi/commit/480a6f04ca360aab17830411a0e599b1e6ab2a1f) — Wire Exhaust Cutout, Sport Pedal, Downshift Blipper and Limiter Adders to Sport Mode | +39 / −48 |
