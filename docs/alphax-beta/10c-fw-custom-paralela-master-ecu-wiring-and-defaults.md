# 10c — fw-custom-paralela-master: ECU wiring and defaults

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

A custom Paralela board package provides a distinct pin map, sensor defaults and firmware feature selection. It explicitly opts into the external CAN throttle build path.

## ECU logic and behavior

1. Board defaults configure local sensors and register board-specific configuration callbacks.
2. F407 and F427 metadata select separate package variants.
3. The build allows selected AlphaX controls, including the external CAN ETB interface, while runtime enable/calibration still determine behavior.
4. Its reduced CLT curve uses the eight-bin default fix and its larger Lua allocation participates in persistent-layout sizing.

**Illustrative behavior:** Selecting this board changes hardware routing and default sensor interpretation before any user tune is applied.

## Calibration, dependencies and review limits

This report counts maintained C/C++ board source. Build metadata, connector YAML, generated headers/INIs and packaging are *INFRASTRUCTURE*. Other runtime features used by the board have their own reports.

Check the exact MCU variant, analog divider and remote ETB runtime configuration. A compile-time opt-in does not prove the external controller is present or calibrated.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 99 changed lines — 99 additions and 0 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+99 / −0**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 3 files are new since the common ancestor (+99 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/config/boards/fw-custom-paralela-master/board_configuration.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/fw-custom-paralela-master/board_configuration.cpp) | +77 / −0 | +77 / −0 |
| [firmware/config/boards/fw-custom-paralela-master/default_tune.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/fw-custom-paralela-master/default_tune.cpp) | +5 / −0 | +5 / −0 |
| [firmware/config/boards/fw-custom-paralela-master/knock_config.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/fw-custom-paralela-master/knock_config.h) | +17 / −0 | +17 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-17 | [286e7402110](https://github.com/ElDominio/rusefi/commit/286e74021107959f1afbe41826f56d060d00709a) — launch control: add clutch-up activation mode; alphax-gold: persist page 5/4 to flash | +99 / −0 |

Related integration/history commits outside the measured source-file scope:

- [c43f3885ecb — Gate external CAN ETB controller behind EFI_EXTERNAL_CAN_ETB, opt-in for fw-custom-paralela-master](https://github.com/ElDominio/rusefi/commit/c43f3885ecb4ef8bb9821518f592de0094ee009f): *INFRASTRUCTURE* — external CAN ETB build opt-in.
- [38bfa6ac98f — paralela: split into F407/F427 meta-info variants, fix SHORT_BOARD_NAME clobbering](https://github.com/ElDominio/rusefi/commit/38bfa6ac98f0d87ce443a672dd9b5b662e93528b): *INFRASTRUCTURE* — F407/F427 package split.
- [b4c81376312 — fw-custom-paralela-master: enable Ghost Cam, Rolling Launch, Sport Pedal, AC Pressure Fan](https://github.com/ElDominio/rusefi/commit/b4c8137631203740833c423c6b9078a4cf197c4f): *INFRASTRUCTURE* — enabled operating features.
- [50a9be936eb — fw-custom-paralela-master: bump LUA_SCRIPT_SIZE 10000 -> 15000](https://github.com/ElDominio/rusefi/commit/50a9be936eb837016f07de1ba9b7e1054da0e2e0): *INFRASTRUCTURE* — Lua capacity change.

Supporting configuration and source: [firmware/config/boards/fw-custom-paralela-master/board.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/fw-custom-paralela-master/board.mk), [firmware/config/boards/fw-custom-paralela-master/connectors/readme.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/fw-custom-paralela-master/connectors/readme.md), [firmware/config/boards/fw-custom-paralela-master/prepend.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/fw-custom-paralela-master/prepend.txt), [firmware/config/boards/fw-custom-paralela-master/readme.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/fw-custom-paralela-master/readme.md).
