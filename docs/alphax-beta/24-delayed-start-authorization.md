# 24 — Delayed start authorization

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Master's start-button handling can discard a press when cranking is blocked. The branch keeps sampling the button edge, adds Lua `startDisabled()`, and holds a pending request for one second when Lua authorization is still absent. It retries the original request when all starter gates pass, or drops it on timeout/engine movement.

## ECU logic and behavior

1. Continue sampling the start-button edge even while cranking is blocked.
2. If Lua authorization is absent, retain the press as a pending request for one second.
3. Retry that request when authorization and all normal starter gates pass.
4. Drop it on timeout or engine movement rather than replaying it indefinitely.
5. Lua startDisabled supplies the authorization input.

**Illustrative behavior:** A driver can press Start just before a CAN/Lua authorization arrives and still start within the pending interval without pressing twice.

## Calibration, dependencies and review limits

This is a separate starter-state change; it should not be confused with the no-spark option. No dedicated starter test is added in this diff.

Check a delayed authorization, timeout, engine movement and repeated button edges. The diff adds no dedicated starter test for this pending-request behavior.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 112 changed lines — 95 additions and 17 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+102 / −116**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/lua/lua_hooks.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/lua/lua_hooks.cpp) | +29 / −0 | +36 / −96 |
| [firmware/controllers/start_stop.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/start_stop.cpp) | +57 / −17 | +57 / −20 |
| [firmware/controllers/start_stop.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/start_stop.h) | +9 / −0 | +9 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-20 | [9b485518faa](https://github.com/ElDominio/rusefi/commit/9b485518faaf9d9f83b72db492c943881108f368) — Latch start-button press during pending Lua/CAN cranking authorization; fix AlphaX S550 TPS mux swap; add TCU support to protorico-econoline | +63 / −20 |
