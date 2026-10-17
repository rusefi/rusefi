# 55 — Rolling launch

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

While a hardware/Lua trigger is held and speed/RPM gates pass, rolling launch captures RPM and builds a spark-skip ramp up to a capped target window. It applies fixed timing retard and fuel addition while armed, then ramps the retard out after release.

## ECU logic and behavior

1. Require the hardware/Lua trigger plus configured vehicle-speed and RPM gates.
2. Capture the activation RPM and construct the target window.
3. Ramp spark skip across that window instead of using only a binary hard cut.
4. Apply the configured held timing retard and fuel addition while armed.
5. After release, ramp timing retard out rather than dropping it instantly.

**Illustrative behavior:** The hold follows an RPM captured while moving, so its trigger and release behavior differ from standing launch control.

## Calibration, dependencies and review limits

This is a new moving-vehicle launch feature. The final source uses a graded skip ratio across the captured-RPM window; the early commit subject's “hard spark limiter” shorthand is incomplete.

Check arming gates, target-window caps, shared skip requests and release transitions. Early commit wording calls this a hard limiter, but the retained source implements graded skip.

Existing branch test evidence: [unit_tests/tests/test_rolling_launch.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_rolling_launch.cpp).

These test files change by +146 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 296 changed lines — 282 additions and 14 deletions across 5 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+397 / −36**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+156 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/engine2.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine2.cpp) | +17 / −1 | +124 / −1 |
| [firmware/controllers/algo/fuel_math.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel_math.cpp) | +50 / −0 | +58 / −22 |
| [firmware/controllers/algo/ignition/ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp) | +59 / −13 | +59 / −13 |
| [firmware/controllers/modules/rolling_launch/rolling_launch.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/rolling_launch/rolling_launch.cpp) | +122 / −0 | +122 / −0 |
| [firmware/controllers/modules/rolling_launch/rolling_launch.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/rolling_launch/rolling_launch.h) | +34 / −0 | +34 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +100 / −12 |
| 2026-06-25 | [e96cde44624](https://github.com/ElDominio/rusefi/commit/e96cde446243b8092bf44a619e4e8eb0a2875446) — Add Rolling Launch Control: button-held captured-RPM hold via hard spark limiter to spool turbo while moving; flat timing/fuel retard held, linear timing ramp-out on release; EFI_ROLLING_LAUNCH page-5 feature | +167 / −0 |
