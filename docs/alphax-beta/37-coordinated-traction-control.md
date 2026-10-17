# 37 — Coordinated traction control

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Master looks up throttle/timing/spark effects in their individual consumers. The branch introduces a common traction controller with held corrections and decay, selectable slip or RPM-acceleration Y input, Lua-gauge scaling, and minimum speed/pedal-demand gates. Table entries become positive removal magnitudes; throttle/timing outputs are made subtractive, spark percentages become fractions, and table axes are corrected.

## ECU logic and behavior

1. Apply minimum vehicle-speed and driver-demand gates before correction.
2. Select wheel slip or RPM acceleration as the table input and observe the configured check cadence.
3. Look up positive removal magnitudes and optionally scale them from a Lua gauge.
4. Hold the active corrections and decay them after the event.
5. Subtract throttle/timing corrections in their consumers and convert spark percentage to a fractional skip request.

**Illustrative behavior:** A spark-removal value of 20 means a 0.20 skip fraction; a positive timing-removal entry must retard, not advance.

## Calibration, dependencies and review limits

This is a behavior and calibration-format change. Existing tables cannot be assumed to preserve their meaning across sign/axis changes. The branch-side diff has no dedicated traction-control test file, so the implementation and related engine tests are not evidence of full traction coverage.

Migrate table signs and axes explicitly. Check low-speed gating, decay and interaction with the common spark-skip path; no dedicated new traction-control test file appears in the diff.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 494 changed lines — 463 additions and 31 deletions across 5 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+570 / −47**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+188 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp) | +199 / −17 | +199 / −33 |
| [firmware/controllers/algo/engine2.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine2.cpp) | +17 / −1 | +124 / −1 |
| [firmware/controllers/algo/ignition/ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp) | +59 / −13 | +59 / −13 |
| [firmware/controllers/algo/traction_control.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/traction_control.cpp) | +140 / −0 | +140 / −0 |
| [firmware/controllers/algo/traction_control.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/traction_control.h) | +48 / −0 | +48 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +301 / −17 |
| 2026-08-17 | [b87644d4b9a](https://github.com/ElDominio/rusefi/commit/b87644d4b9ae60c03a6c9ae4f65de56831632d9a) — Traction Control: ETB/timing drop tables now positive-magnitude, fix spark-skip scaling | +8 / −2 |
| 2026-08-21 | [407b9725d22](https://github.com/ElDominio/rusefi/commit/407b9725d22f5f83228a4cf14d71e8d863a412ec) — Traction Control: fix slip/speed axis transpose, add configurable slip-check rate | +40 / −15 |
| 2026-08-22 | [f584eb9ff63](https://github.com/ElDominio/rusefi/commit/f584eb9ff63cc982f1ecdf29c74ff4b1cf98d9e5) — Traction Control: add minimum vehicle speed / driver demand gate | +13 / −0 |
