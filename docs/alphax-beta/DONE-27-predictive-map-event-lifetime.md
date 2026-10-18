# DONE — 27 — Predictive MAP event lifetime

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Upstream completion

**DONE on upstream master**, reviewed at [`bd98053cc7f`](https://github.com/rusefi/rusefi/commit/bd98053cc7ffecbb1a323c632a1319671af1a73a). Commit [`7dff676ec6c`](https://github.com/rusefi/rusefi/commit/7dff676ec6c85703799cf9e6c8e02510d707c258) implements both fixes. The [upstream calculation](https://github.com/rusefi/rusefi/blob/bd98053cc7ffecbb1a323c632a1319671af1a73a/firmware/controllers/algo/airmass/speed_density_airmass.cpp) and [timer storage](https://github.com/rusefi/rusefi/blob/bd98053cc7ffecbb1a323c632a1319671af1a73a/firmware/controllers/algo/airmass/speed_density_airmass.h) match the pinned fork's logic; the remaining differences in those files are comments.

The overall session timer starts only when a new event begins. Rising-TPS retriggers restart the blend timer without extending the session deadline; at the configured duration the calculation returns measured MAP. Each new event also initializes its own TPS peak, so release detection does not inherit a larger previous event. Existing release/rearm gates and diagnostic counters remain in place, and the tuning description now explains that retriggers cannot extend the total duration.

The adoption commit changes **9 maintained firmware C/C++ lines: +8 / −1 across these two files**. Its [upstream tests](https://github.com/rusefi/rusefi/blob/bd98053cc7ffecbb1a323c632a1319671af1a73a/unit_tests/tests/ignition_injection/test_fuel_math.cpp) add 109 lines separately. They check repeated retriggers through the exact session deadline, measured-MAP fallback, rearming for a subsequent session, and a smaller event retaining its own peak until actual throttle release. Tests were inspected, not rerun. Historical fork LOC below is unchanged.

## Executive summary

At the original comparison revision, master already provided predictive MAP. The branch adds a separate session timer so repeated TPS retriggers cannot extend a prediction past the configured overall duration. It also resets the recorded TPS peak on each new prediction, preventing a previous larger event from immediately cancelling a smaller later event. Both fixes are now upstream as described above.

## ECU logic and behavior

1. Keep a separate overall prediction-session timer from the retrigger/blend timer.
2. Enforce the configured session duration even if TPS retriggers the prediction.
3. Initialize the stored TPS peak on each new event.
4. Use the new event's peak when deciding whether throttle movement cancels prediction.

**Illustrative behavior:** A smaller throttle event following a larger one no longer inherits the older peak and immediately cancels for that reason.

## Calibration, dependencies and review limits

These are two small fixes in one subsystem, separable from the other AlphaX controls. No corresponding predictive-MAP test change appears in the branch-side diff; upstream coverage is linked above.

Exercise repeated retriggers up to the duration cap and a large event followed by a small event. Existing predictive MAP is retained; these are two lifetime/reset fixes.

No feature-specific test is claimed for the pinned fork. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 24 changed lines — 14 additions and 10 deletions across 2 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+14 / −10**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/airmass/speed_density_airmass.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/airmass/speed_density_airmass.cpp) | +10 / −10 | +10 / −10 |
| [firmware/controllers/algo/airmass/speed_density_airmass.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/airmass/speed_density_airmass.h) | +4 / −0 | +4 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-08-15 | [ce60e947dff](https://github.com/ElDominio/rusefi/commit/ce60e947dff54a508936d1b18adc42eb5ba67a5e) — Fix Predictive MAP blend-duration cap not being a hard cap | +9 / −1 |
| 2026-09-18 | [c80e5e96e0f](https://github.com/ElDominio/rusefi/commit/c80e5e96e0f79fedee5c516afc8d76b24dbad900) — Predictive MAP: reset m_tpsPeak on new event trigger | +5 / −0 |
