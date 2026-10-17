# 56 — Advanced VVT control

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Master's fixed PID offset/proportional gain can be replaced by an oil-pressure base-duty curve and a signed distance-to-target proportional-gain curve, separately for intake/exhaust. Integral bounds become configurable, and advanced mode bypasses the normal near-zero target hysteresis gate. Telemetry reports the scheduled P term and final applied command.

## ECU logic and behavior

1. In advanced mode, interpolate an oil-pressure-dependent base duty separately for intake and exhaust.
2. Schedule proportional gain from signed distance to target.
3. Run PID trim continuously over feed-forward with configured integral bounds.
4. Bypass the ordinary near-zero-target hysteresis gate in this mode.
5. Use zero pressure as the lookup input when oil pressure is unavailable.
6. Publish scheduled P and the final command.

**Illustrative behavior:** The same angular error can receive different gain depending on direction, while oil pressure changes the feed-forward duty underneath it.

## Calibration, dependencies and review limits

The final controller always runs PID trim over feed-forward; the historical fade/pause-window experiments were removed. Missing oil pressure evaluates the curve at zero pressure. This feature also changes the shared PID API.

Check sign/direction, missing-pressure fallback, integral saturation and cam-specific calibration. The fade and pause-window commits are historical steps that later commits remove.

Existing branch test evidence: [unit_tests/tests/actuators/test_vvt.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_vvt.cpp).

These test files change by +190 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 130 changed lines — 124 additions and 6 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+125 / −39**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/vvt.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/vvt.cpp) | +108 / −5 | +109 / −10 |
| [firmware/util/math/efi_pid.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/util/math/efi_pid.cpp) | +9 / −1 | +9 / −24 |
| [firmware/util/math/efi_pid.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/util/math/efi_pid.h) | +7 / −0 | +7 / −5 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-08-13 | [e0b4c596c22](https://github.com/ElDominio/rusefi/commit/e0b4c596c22b53ba7e3c8d13c39c7aedd64a69a5) — Add VVT Advanced Mode + PID iTerm clamps | +70 / −0 |
| 2026-08-15 | [78f6e90cc31](https://github.com/ElDominio/rusefi/commit/78f6e90cc3127f1126af75d90f137169808cf65a) — VVT Advanced Mode: replace PID fade with a pause window, dedupe vvtDistances | +23 / −20 |
| 2026-08-15 | [b1c52f69e65](https://github.com/ElDominio/rusefi/commit/b1c52f69e65c5bb78354c0fc64372f23fdd1f1fa) — VVT Advanced Mode: base duty vs. oil pressure, gain-scheduled P vs. distance | +33 / −19 |
| 2026-08-16 | [4ed5b508a6a](https://github.com/ElDominio/rusefi/commit/4ed5b508a6a2ca4486ce482e1a3f344ab6f86713) — VVT Advanced Mode: drop the PID pause window | +9 / −20 |
| 2026-08-17 | [2e647e74110](https://github.com/ElDominio/rusefi/commit/2e647e741102a3824cf4c24e39250441137c03eb) — VVT Advanced Mode: skip near-zero hysteresis gate, decouple min-RPM from cranking | +14 / −5 |
