# 10a — alphax-s197-v2: ECU wiring and defaults

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

A new S197 board package defines the ECU pin assignments and default configuration for its harness. These assignments determine which physical outputs and sensors the control algorithms use.

## ECU logic and behavior

1. Board initialization selects injector, coil and sensor wiring.
2. Defaults include CLT/IAT, pedal/TPS, flex, oil-pressure and A/C-pressure inputs.
3. The package supplies a board knock configuration and an F4 feature/memory selection.
4. Generated connector and tune artifacts describe these maintained defaults but are not additional algorithms.

**Illustrative behavior:** Selecting this board changes hardware routing and default sensor interpretation before any user tune is applied.

## Calibration, dependencies and review limits

This report counts maintained C/C++ board source. Build metadata, connector YAML, generated headers/INIs and packaging are *INFRASTRUCTURE*. Other runtime features used by the board have their own reports.

Verify harness pin correspondence and output ordering before relying on an engine tune. The board package does not validate the shared control features it enables.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 231 changed lines — 231 additions and 0 deletions across 2 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+231 / −0**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+231 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/config/boards/alphax-s197-v2/board_configuration.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/alphax-s197-v2/board_configuration.cpp) | +220 / −0 | +220 / −0 |
| [firmware/config/boards/alphax-s197-v2/knock_config.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/alphax-s197-v2/knock_config.h) | +11 / −0 | +11 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +233 / −0 |
| 2026-06-29 | [c4bcade6498](https://github.com/ElDominio/rusefi/commit/c4bcade6498d06501c06857d23d09340fbb8a0ee) — Update alphax-s197-v2 and alphax-s550-pnp board configurations | +2 / −1 |

Supporting configuration and source: [firmware/config/boards/alphax-s197-v2/board.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/alphax-s197-v2/board.mk), [firmware/config/boards/alphax-s197-v2/prepend.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/alphax-s197-v2/prepend.txt), [firmware/config/boards/alphax-s197-v2/readme.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/alphax-s197-v2/readme.md).
