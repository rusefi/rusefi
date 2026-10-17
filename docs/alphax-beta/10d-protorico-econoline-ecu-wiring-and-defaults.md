# 10d — protorico-econoline: ECU wiring and defaults

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The Econoline package maps its Ford harness and uses a cylinder-head sensor to estimate coolant temperature. Its oil-life default then consumes estimated coolant temperature.

## ECU logic and behavior

1. Harness-specific injector and ignition ordering is set by board configuration.
2. The harness CLT-labelled input is registered as CHT because the engine uses a head sensor.
3. CLT-from-CHT estimation is enabled, feeding ordinary downstream coolant consumers.
4. Oil-life monitoring is enabled with coolant temperature as its primary source; transmission/speed wiring uses the new infrastructure.

**Illustrative behavior:** Selecting this board changes hardware routing and default sensor interpretation before any user tune is applied.

## Calibration, dependencies and review limits

This report counts maintained C/C++ board source. Build metadata, connector YAML, generated headers/INIs and packaging are *INFRASTRUCTURE*. Other runtime features used by the board have their own reports.

The source calls the generic CHT thermistor calibration a placeholder pending actual Ford sensor calibration. Validate that conversion before treating warmup, fan or oil-life behavior as calibrated.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 248 changed lines — 248 additions and 0 deletions across 2 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+248 / −0**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+248 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/config/boards/protorico-econoline/board_configuration.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/protorico-econoline/board_configuration.cpp) | +237 / −0 | +237 / −0 |
| [firmware/config/boards/protorico-econoline/knock_config.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/protorico-econoline/knock_config.h) | +11 / −0 | +11 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +218 / −0 |
| 2026-06-20 | [9b485518faa](https://github.com/ElDominio/rusefi/commit/9b485518faaf9d9f83b72db492c943881108f368) — Latch start-button press during pending Lua/CAN cranking authorization; fix AlphaX S550 TPS mux swap; add TCU support to protorico-econoline | +16 / −12 |
| 2026-08-01 | [50f59f32130](https://github.com/ElDominio/rusefi/commit/50f59f321307f33b210a7b66d48f991dda7674ea) — protorico-econoline: enable CLT-from-CHT estimation | +21 / −1 |
| 2026-08-03 | [4b2390c8096](https://github.com/ElDominio/rusefi/commit/4b2390c809616f4d980e0ca311423bb2ad26f083) — protorico-econoline: enable Oil Life Monitor by default | +5 / −0 |

Supporting configuration and source: [firmware/config/boards/protorico-econoline/board.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/protorico-econoline/board.mk), [firmware/config/boards/protorico-econoline/prepend.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/protorico-econoline/prepend.txt), [firmware/config/boards/protorico-econoline/readme.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/protorico-econoline/readme.md).
