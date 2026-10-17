# 39 — Weighted oil-life monitor

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

A new module accumulates temperature-weighted revolutions and converts the result into remaining oil-life percentage. It selects oil or coolant temperature with fallback, uses fixed temperature zones with tunable weights, exposes reset/set commands and a Lua reset hook, and requests persistence at ignition-off.

## ECU logic and behavior

1. Select the configured oil/coolant temperature source with fallback.
2. Accumulate engine revolutions weighted by the selected temperature zone.
3. Convert accumulated exposure into remaining life percentage using the configured life budget.
4. Support reset/set commands and the Lua reset hook.
5. At ignition-off, request persistence while delayed main-relay shutdown preserves time for the save.
6. Ordinary configuration burns also write the extra record.

**Illustrative behavior:** The same number of engine revolutions can consume different indicated life in different configured temperature zones.

## Calibration, dependencies and review limits

This is an estimate based on configured weights, not an oil-quality measurement. Integration extends storage records and delayed main-relay shutdown so the counter can be saved. Ordinary configuration burns also write the record through the common extra-page path.

Check reset persistence, ignition-off write completion and temperature-source fallback. This estimates service life from configured weights; it does not measure contamination or oil chemistry.

Existing branch test evidence: [unit_tests/tests/test_oil_life_monitor.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_oil_life_monitor.cpp).

These test files change by +235 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 336 changed lines — 334 additions and 2 deletions across 4 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+340 / −61**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+304 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/main_relay.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/main_relay.cpp) | +1 / −1 | +1 / −1 |
| [firmware/controllers/modules/oil_life_monitor/oil_life_monitor.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/oil_life_monitor/oil_life_monitor.cpp) | +233 / −0 | +233 / −0 |
| [firmware/controllers/modules/oil_life_monitor/oil_life_monitor.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/oil_life_monitor/oil_life_monitor.h) | +71 / −0 | +71 / −0 |
| [firmware/controllers/storage.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/storage.cpp) | +29 / −1 | +35 / −60 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-07-31 | [f2f6f046559](https://github.com/ElDominio/rusefi/commit/f2f6f0465599e041b31dfe26a4729f07113d0574) — Add Weighted Engine Oil Life Monitor (EFI_OIL_LIFE_MONITOR) | +300 / −1 |
| 2026-08-01 | [1ec04c68b79](https://github.com/ElDominio/rusefi/commit/1ec04c68b79734bd3f5542e1fef11057fdeeea79) — Oil Life Monitor: add set_oil_life console command | +27 / −0 |
