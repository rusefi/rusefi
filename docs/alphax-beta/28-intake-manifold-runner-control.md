# 28 — Intake manifold runner control

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The new IMRC controller supports solenoid and timed H-bridge outputs, configurable PWM duties, inversion, and RPM/TPS/MAP thresholds. Enabled thresholds must all be satisfied; it does not open merely because any one configured threshold passes.

## ECU logic and behavior

1. Evaluate each enabled RPM, TPS and MAP opening threshold.
2. Require all configured conditions to pass before opening.
3. Translate the desired state into solenoid PWM or timed H-bridge motion, with configured inversion and duties.
4. Keep logical open/closed state distinct from the electrical direction and drive duration.

**Illustrative behavior:** High RPM alone does not open the runners when an enabled TPS threshold still fails.

## Calibration, dependencies and review limits

This is an independent actuator feature with page-6 and hardware-output dependencies. There is no dedicated IMRC test file in the diff.

Check disabled thresholds, output inversion and H-bridge travel timing. There is no dedicated new IMRC test file; a logical transition does not establish mechanical end position.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 215 changed lines — 215 additions and 0 deletions across 2 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+215 / −0**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+215 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/imrc_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/imrc_controller.cpp) | +171 / −0 | +171 / −0 |
| [firmware/controllers/actuators/imrc_controller.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/imrc_controller.h) | +44 / −0 | +44 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-29 | [ac3688e3051](https://github.com/ElDominio/rusefi/commit/ac3688e3051bfb7efe116e866010dd95fccf5dd2) — Add IMRC Controller: solenoid and H-bridge intake manifold runner control with RPM/TPS/MAP triggers | +207 / −0 |
| 2026-06-30 | [cc6f879c4de](https://github.com/ElDominio/rusefi/commit/cc6f879c4de3f03dbb27792de6d6685a38f05156) — Fix IMRC/exhaust cutout multi-threshold logic to AND configured checks | +13 / −5 |
