# 40 — Engine module storage and F4 RAM placement

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

*INFRASTRUCTURE* — firmware support with an ECU runtime consequence.

## Executive summary

Master keeps its modules in the engine's module list. The branch splits core and AlphaX modules, adds combined iteration/aggregation helpers and routes lifecycle callbacks across both lists. It also moves the production F4 engine allocation out of the fixed CCM section.

## ECU logic and behavior

1. Separate core and AlphaX module storage while preserving combined iteration.
2. Route initialization, periodic callbacks, configuration notifications and ignition/phase events through the new helpers.
3. Aggregate shutdown-related behavior across both lists.
4. Place the production F4 Engine allocation outside its previous fixed CCM section.

**Illustrative behavior:** A module added to the secondary list still needs ignition and shutdown callbacks; moving storage without updating those dispatch paths would silently alter ECU behavior.

## Calibration, dependencies and review limits

This is a system-wide memory/lifecycle change, not merely a board flag. Module lookup, callback ordering, defaults, configuration notifications and shutdown aggregation must remain consistent. Existing feature tests exercise parts of the framework, but do not establish per-board RAM fit.

Check every lifecycle traversal and each target's linker map/RAM fit. This supports operational features but has no independent fuel, spark or actuator calibration law.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 235 changed lines — 207 additions and 28 deletions across 7 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+214 / −57**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/engine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine.cpp) | +18 / −4 | +22 / −9 |
| [firmware/controllers/algo/engine.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine.h) | +133 / −16 | +133 / −16 |
| [firmware/controllers/core/engine_module.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/core/engine_module.h) | +14 / −4 | +14 / −4 |
| [firmware/controllers/engine_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/engine_controller.cpp) | +39 / −1 | +42 / −25 |
| [firmware/controllers/engine_cycle/main_trigger_callback.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/engine_cycle/main_trigger_callback.cpp) | +1 / −1 | +1 / −1 |
| [firmware/controllers/ignition_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/ignition_controller.cpp) | +1 / −1 | +1 / −1 |
| [firmware/rusefi.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/rusefi.cpp) | +1 / −1 | +1 / −1 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-08-15 | [3fa563b9cf0](https://github.com/ElDominio/rusefi/commit/3fa563b9cf03a9254198fe40f14a144bf74a3e14) — Split Engine's module list off the fixed 64k F4 CCM pool; move Engine off CCM fleet-wide on F4 | +153 / −43 |
