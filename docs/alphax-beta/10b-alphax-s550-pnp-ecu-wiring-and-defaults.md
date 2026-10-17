# 10b — alphax-s550-pnp: ECU wiring and defaults

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

A new S550 package routes ECU inputs through an expanded ADC/multiplexer arrangement. Follow-ups correct TPS routing and place the A/C-pressure input in persistent board defaults.

## ECU logic and behavior

1. The board selects the ADC3 slow-acquisition and mux arrangement described in report 60.
2. CLT uses logical ADC39, IAT ADC47 and A/C pressure ADC43 in the retained configuration.
3. TPS calibration includes the secondary track direction.
4. The larger Lua allocation and feature selection also affect flash layout and image fit.

**Illustrative behavior:** Selecting this board changes hardware routing and default sensor interpretation before any user tune is applied.

## Calibration, dependencies and review limits

This report counts maintained C/C++ board source. Build metadata, connector YAML, generated headers/INIs and packaging are *INFRASTRUCTURE*. Other runtime features used by the board have their own reports.

Check both mux states from power-on, TPS track direction and A/C-pressure freshness. A correct configuration number is insufficient if acquisition samples the wrong physical channel.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 286 changed lines — 286 additions and 0 deletions across 2 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+286 / −0**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+286 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/config/boards/alphax-s550-pnp/board_configuration.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/alphax-s550-pnp/board_configuration.cpp) | +269 / −0 | +269 / −0 |
| [firmware/config/boards/alphax-s550-pnp/knock_config.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/alphax-s550-pnp/knock_config.h) | +17 / −0 | +17 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +280 / −0 |
| 2026-06-20 | [9b485518faa](https://github.com/ElDominio/rusefi/commit/9b485518faaf9d9f83b72db492c943881108f368) — Latch start-button press during pending Lua/CAN cranking authorization; fix AlphaX S550 TPS mux swap; add TCU support to protorico-econoline | +3 / −3 |
| 2026-06-29 | [c4bcade6498](https://github.com/ElDominio/rusefi/commit/c4bcade6498d06501c06857d23d09340fbb8a0ee) — Update alphax-s197-v2 and alphax-s550-pnp board configurations | +3 / −1 |

Related integration/history commits outside the measured source-file scope:

- [88e36bfb459 — Fix A/C Pressure reading wrong on power-on on alphax-s550-pnp](https://github.com/ElDominio/rusefi/commit/88e36bfb45982f619a1cb1167209506ef6f38610): ADC/mux fix for the S550 A/C-pressure reading at power-on.

Supporting configuration and source: [firmware/config/boards/alphax-s550-pnp/board.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/alphax-s550-pnp/board.mk), [firmware/config/boards/alphax-s550-pnp/prepend.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/alphax-s550-pnp/prepend.txt).
