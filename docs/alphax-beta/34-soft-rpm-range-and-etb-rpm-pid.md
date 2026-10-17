# 34 — Soft RPM range and ETB RPM PID

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Master uses the hard-limit hysteresis span for soft timing/fuel adjustments and a separately configured linear ETB taper. The branch gives soft timing/fuel a separate range and replaces the throttle taper with an opt-in PID referenced to the effective hard limit, targeting 50 RPM below it. The PID has tunable gains and an initial throttle seed, and can only reduce the current requested opening.

## ECU logic and behavior

1. Separate the soft timing/fuel transition range from hard-limit hysteresis.
2. When selected, run an ETB RPM PID against the effective hard limit minus 50 RPM.
3. Use configured gains and an initial throttle seed to calculate a ceiling on requested opening.
4. Apply the PID only as a reduction; it must not create a higher throttle request than the other controllers demand.
5. Lua-gauge curves and Sport adders can change the effective RPM/boost-cut thresholds.

**Illustrative behavior:** With an effective hard limit of 6,500 RPM, the throttle PID aims at 6,450 RPM while the separate hard limiter still provides its own cut behavior.

## Calibration, dependencies and review limits

Lua-gauge curves or Sport-mode adders can also change RPM and boost-cut limits. These adders, the soft-range split and the ETB PID are logically separate changes.

Check activation/reset behavior and controller ordering against blip, hold and limp targets. Old ETB taper calibration does not directly specify the new PID response.

Existing branch test evidence: [unit_tests/tests/actuators/test_etb.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_etb.cpp).

These test files change by +327 / −18 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 326 changed lines — 307 additions and 19 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+308 / −63**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp) | +199 / −17 | +199 / −33 |
| [firmware/controllers/actuators/electronic_throttle.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.h) | +34 / −0 | +34 / −4 |
| [firmware/controllers/limp_manager.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/limp_manager.cpp) | +74 / −2 | +75 / −26 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-19 | [3aaac9fe3d9](https://github.com/ElDominio/rusefi/commit/3aaac9fe3d9fc17a13c51c37ac8ffb69c719b048) — ETB rev limiter PID and configurable soft RPM limit range | +48 / −11 |
| 2026-07-07 | [480a6f04ca3](https://github.com/ElDominio/rusefi/commit/480a6f04ca360aab17830411a0e599b1e6ab2a1f) — Wire Exhaust Cutout, Sport Pedal, Downshift Blipper and Limiter Adders to Sport Mode | +19 / −25 |
