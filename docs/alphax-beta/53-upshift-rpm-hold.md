# 53 — Upshift RPM hold

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The new upshift controller commands ETB toward the next higher gear's matching RPM while the shift is in progress. It has ramping, PID, timeout, minimum/maximum RPM and speed gates, driver override and a high-RPM lockout. Flat-shift torque reduction takes priority over the hold.

## ECU logic and behavior

1. Calculate the next higher gear's matching RPM from the latched shift context.
2. Qualify the hold with speed and minimum/maximum RPM checks.
3. Ramp and regulate the ETB request through the shift, stopping on actual exits or timeout.
4. Allow driver override and high-RPM lockout to prevent an inappropriate hold.
5. Give flat-shift torque reduction priority over RPM hold.

**Illustrative behavior:** If the driver has not completed the shift, the final design can retain the matching-RPM hold until an exit condition instead of ending after the old short hold interval.

## Calibration, dependencies and review limits

It shares feed-forward/configuration and dependencies with downshift blipping but has its own controller and tests. The final implementation holds until a real exit condition; earlier short-lived hold behavior is superseded.

Check high-RPM entry, driver demand, timeout and flat-shift interaction. The downshift and upshift controllers share dependencies but are distinct state machines.

Existing branch test evidence: [unit_tests/tests/actuators/test_upshift_rpm_hold.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_upshift_rpm_hold.cpp).

These test files change by +213 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 543 changed lines — 526 additions and 17 deletions across 4 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+526 / −33**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+321 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp) | +199 / −17 | +199 / −33 |
| [firmware/controllers/actuators/upshift_rpm_hold.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/upshift_rpm_hold.cpp) | +252 / −0 | +252 / −0 |
| [firmware/controllers/actuators/upshift_rpm_hold.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/upshift_rpm_hold.h) | +69 / −0 | +69 / −0 |
| [firmware/controllers/algo/shift_torque_reduction_controller.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/shift_torque_reduction_controller.h) | +6 / −0 | +6 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-19 | [fda2d7e8e68](https://github.com/ElDominio/rusefi/commit/fda2d7e8e689e8a3a966aba8e352c0aa6e3e5dd8) — Add Upshift RPM Hold feature | +323 / −0 |
| 2026-06-22 | [248f91ff828](https://github.com/ElDominio/rusefi/commit/248f91ff828ad4251984fafd2c3091abd48514f6) — Rework Upshift RPM Hold to hold through to a real exit; tooltip/dialog cleanup | +7 / −13 |
| 2026-06-29 | [135f45852f6](https://github.com/ElDominio/rusefi/commit/135f45852f690816fc92d13a8bc29839b414df8a) — Misc actuator/controller fixes: sport pedal live bit, upshift flat-shift priority, gear detector clutch-aware, fuel pump no-sensor fast path, etbSplit default | +6 / −1 |
| 2026-08-16 | [b13bec26e81](https://github.com/ElDominio/rusefi/commit/b13bec26e81d925e69eca62aaa875dd2cae9f821) — Upshift RPM Hold: add high-RPM lockout (over-rev safety) | +7 / −0 |
