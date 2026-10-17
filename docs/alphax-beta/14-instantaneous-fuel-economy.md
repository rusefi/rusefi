# 14 — Instantaneous fuel economy

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch adds a calculation of fuel flow, L/100 km and US MPG from RPM, injector flow, effective pulse width after deadtime, cylinder count and vehicle speed, then publishes an MPG channel/gauge. Master has other fuel-consumption infrastructure but not this added calculator.

## ECU logic and behavior

1. Subtract deadtime from injector pulse width and reject nonpositive effective delivery.
2. Compute events per second as RPM / 120 multiplied by cylinder count.
3. Multiply effective pulse width by nominal injector flow to obtain calculated L/h.
4. Above the minimum vehicle speed, compute L/100 km and convert using 235.214583 / (L/100 km) to US MPG.
5. This is telemetry calculation; it does not alter the injection request.

**Illustrative behavior:** If pulse width is no greater than deadtime, the helper returns zero delivery rather than negative fuel flow.

## Calibration, dependencies and review limits

The formula assumes one injection event per cylinder per 720 degrees, so its interpretation depends on injection strategy; it is not a measured fuel-flow sensor. Dedicated arithmetic tests cover the new helper.

Check injection strategy and flow units before interpreting MPG. The calculation assumes one injection per cylinder per 720 degrees and does not measure actual delivered fuel.

Existing branch test evidence: [unit_tests/tests/ignition_injection/test_fuel_economy_calculator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_fuel_economy_calculator.cpp).

These test files change by +79 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 165 changed lines — 160 additions and 5 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+172 / −40**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+83 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/console/status_loop.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/console/status_loop.cpp) | +77 / −5 | +89 / −40 |
| [firmware/controllers/algo/fuel/fuel_economy_calculator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/fuel_economy_calculator.cpp) | +47 / −0 | +47 / −0 |
| [firmware/controllers/algo/fuel/fuel_economy_calculator.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/fuel_economy_calculator.h) | +36 / −0 | +36 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-08-13 | [8a83d129146](https://github.com/ElDominio/rusefi/commit/8a83d1291460af7d5372936517d7647a872d489e) — Add instantaneous fuel economy calculator + MPG gauge | +113 / −2 |
