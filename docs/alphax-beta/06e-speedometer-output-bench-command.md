# 06e — Speedometer output bench command

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The ECU gains a timed speedometer-output test at a requested frequency. This changes a physical output during diagnostics without requiring a moving vehicle.

## ECU logic and behavior

1. The new bench command first requires an initialized speedometer output.
2. It records the requested frequency and starts a three-second timer.
3. During the test, speedoUpdate writes that frequency and bypasses the normal VehicleSpeed conversion.
4. On expiry, normal pulses-per-kilometer speed output resumes.

**Illustrative behavior:** A bench request can move the speedometer while the vehicle is stationary; after three seconds the configured speed-derived output takes over again.

## Calibration, dependencies and review limits

The command is delivered alongside the OSS/wheel-speed changes, but its timed output override can be reviewed independently.

Verify the unconfigured-pin rejection, output frequency, timeout and return to vehicle-speed control. The command-dispatch source files also contain other bench operations.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 49 changed lines — 47 additions and 2 deletions across 4 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+83 / −88**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/bench_test.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/bench_test.cpp) | +15 / −0 | +51 / −86 |
| [firmware/controllers/can/can_bench_test.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/can/can_bench_test.cpp) | +3 / −2 | +3 / −2 |
| [firmware/controllers/gauges/speedometer.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/gauges/speedometer.cpp) | +26 / −0 | +26 / −0 |
| [firmware/controllers/gauges/speedometer.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/gauges/speedometer.h) | +3 / −0 | +3 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-09-11 | [4b1a95ac583](https://github.com/ElDominio/rusefi/commit/4b1a95ac583369c44692ad231586c29c7a2b1139) — Wheel Speed Sensors: OSS->VehicleSpeed shares driveWheelRevPerKm/finalGearRatio; speedometer bench test | +37 / −0 |
