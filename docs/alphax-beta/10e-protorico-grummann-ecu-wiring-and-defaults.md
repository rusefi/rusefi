# 10e — protorico-grummann: ECU wiring and defaults

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The Grummann package supports a different step-van harness using distributor ignition, single-point injection, stepper idle and a physical coolant sensor.

## ECU logic and behavior

1. One ignition output drives the distributor coil rather than one output per cylinder.
2. The single-point fuel arrangement does not require cam-based sequential cylinder identification.
3. Pins otherwise used for coils/injection are reassigned to harness functions, including stepper idle and A/C input.
4. A physical CLT input replaces the Econoline package's CHT-derived default.

**Illustrative behavior:** Selecting this board changes hardware routing and default sensor interpretation before any user tune is applied.

## Calibration, dependencies and review limits

This report counts maintained C/C++ board source. Build metadata, connector YAML, generated headers/INIs and packaging are *INFRASTRUCTURE*. Other runtime features used by the board have their own reports.

Check repurposed pins, ignition-module bypass behavior and stepper direction on this harness. Do not transfer Econoline output assumptions solely because the hardware module is related.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 210 changed lines — 210 additions and 0 deletions across 1 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+210 / −0**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 1 files are new since the common ancestor (+210 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/config/boards/protorico-grummann/board_configuration.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/protorico-grummann/board_configuration.cpp) | +210 / −0 | +210 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-09-11 | [4321982892c](https://github.com/ElDominio/rusefi/commit/4321982892c33b3701971fd3dbce08fe8e04d8ca) — New board: protorico-grummann | +210 / −0 |

Supporting configuration and source: [firmware/config/boards/protorico-grummann/board.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/protorico-grummann/board.mk), [firmware/config/boards/protorico-grummann/prepend.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/protorico-grummann/prepend.txt), [firmware/config/boards/protorico-grummann/readme.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/protorico-grummann/readme.md).
