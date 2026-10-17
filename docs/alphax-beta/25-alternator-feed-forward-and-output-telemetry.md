# 25 — Alternator feed-forward and output telemetry

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch adds a target-voltage/RPM base-duty table as an alternative to the fixed PID offset. It removes double application of the offset when combining open- and closed-loop terms, clamps the final duty to 0–100%, and reports the applied duty rather than only the raw PID output. Eco mode can additionally blend toward its own voltage target.

## ECU logic and behavior

1. Determine the requested voltage, including an optional Eco blend.
2. Use the new target-voltage/RPM base-duty map when configured.
3. Combine feed-forward and PID correction without applying the fixed offset twice.
4. Clamp the final command to 0–100%.
5. Publish the applied duty, allowing logs to distinguish actuator command from an unconstrained PID term.

**Illustrative behavior:** A negative combined control result now reports and commands zero duty rather than presenting the negative PID result as the applied output.

## Calibration, dependencies and review limits

The base-duty/control correction and the Eco-mode integration are separable. The output-channel structure also changes.

Check PID offset compatibility, output polarity and Eco entry/exit. Logs from old and new versions may use the same channel name for different meanings.

Existing branch test evidence: [unit_tests/tests/actuators/test_alternator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_alternator.cpp), [unit_tests/tests/actuators/test_alternator_voltage_target_set_point.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_alternator_voltage_target_set_point.cpp).

These test files change by +33 / −2 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 81 changed lines — 69 additions and 12 deletions across 1 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+69 / −12**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/alternator_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/alternator_controller.cpp) | +69 / −12 | +69 / −12 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +42 / −6 |
| 2026-09-11 | [952fa3e1471](https://github.com/ElDominio/rusefi/commit/952fa3e1471b3c82c1658789ac087423e2b42894) — Alternator PID: clamp final duty to >=0, alternatorStatus.output now the real applied duty | +23 / −7 |
