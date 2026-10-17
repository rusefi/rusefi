# 06a — Ignition-voltage telemetry fallback

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch reports battery voltage as VIgn when the board has no ignition-sense sensor. This is an ECU telemetry correction with no new voltage acquisition hardware.

## ECU logic and behavior

1. The telemetry publisher checks whether an ignition-voltage sensor is configured.
2. When that sensor is absent, the VIgn output uses the battery-voltage reading.
3. This changes the reported channel; it does not add a physical ignition-sense input or redefine the starter authorization logic.

**Illustrative behavior:** A board with battery sensing but no ignition-sense circuit now shows battery voltage in the VIgn gauge instead of a missing ignition measurement.

## Calibration, dependencies and review limits

The surrounding status publisher also contains unrelated new channels. Its full-file count therefore exceeds this small fallback change; the focused commit below provides the narrow patch.

Check a board with ignition sensing and one without it. Consumers must not interpret the fallback gauge as independent evidence of key position.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 82 changed lines — 77 additions and 5 deletions across 1 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+89 / −40**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/console/status_loop.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/console/status_loop.cpp) | +77 / −5 | +89 / −40 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-09-06 | [ffc0c187661](https://github.com/ElDominio/rusefi/commit/ffc0c18766115bcb2a1cad6e4d8bb82639328508) — sensors: fall back to battery voltage for VIgn gauge when no ignition-sense circuit | +7 / −1 |
