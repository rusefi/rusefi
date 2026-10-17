# 29 — Exhaust cutout control

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch adds digital, PWM and timed H-bridge cutout actuation, switch/Sport/automatic activation, threshold delays, indicator output, and key-on/engine-on movement tests. Automatic operation requires all configured RPM/TPS/MAP checks; Lua can force the automatic path open via `setExhCutoutActive()`.

## ECU logic and behavior

1. Choose switch, Sport or automatic activation and evaluate the applicable gates.
2. Automatic operation combines enabled RPM/TPS/MAP thresholds with AND logic and transition delays.
3. Lua can request the automatic path open through setExhCutoutActive.
4. Translate the command to digital, PWM or timed H-bridge actuation and update the indicator.
5. Key-on/engine-on tests and P&B integration use the same actuator state.

**Illustrative behavior:** A high-load automatic opening waits for the configured conditions and delay instead of opening when only one threshold passes.

## Calibration, dependencies and review limits

Basic cutout operation is separable, but Sport mode and the pops-and-bangs inhibit consume shared state. The output ownership and transition behavior deserve separate hardware verification.

Check mode priority, Lua request clearing, transition timing and output ownership. Calibrate H-bridge movement duration on the actual valve.

Existing branch test evidence: [unit_tests/tests/actuators/test_exhaust_cutout.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_exhaust_cutout.cpp).

These test files change by +107 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 480 changed lines — 480 additions and 0 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+487 / −96**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+451 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/exhaust_cutout.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/exhaust_cutout.cpp) | +390 / −0 | +390 / −0 |
| [firmware/controllers/actuators/exhaust_cutout.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/exhaust_cutout.h) | +61 / −0 | +61 / −0 |
| [firmware/controllers/lua/lua_hooks.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/lua/lua_hooks.cpp) | +29 / −0 | +36 / −96 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +450 / −0 |
| 2026-06-30 | [cc6f879c4de](https://github.com/ElDominio/rusefi/commit/cc6f879c4de3f03dbb27792de6d6685a38f05156) — Fix IMRC/exhaust cutout multi-threshold logic to AND configured checks | +13 / −5 |
| 2026-07-07 | [480a6f04ca3](https://github.com/ElDominio/rusefi/commit/480a6f04ca360aab17830411a0e599b1e6ab2a1f) — Wire Exhaust Cutout, Sport Pedal, Downshift Blipper and Limiter Adders to Sport Mode | +27 / −23 |
