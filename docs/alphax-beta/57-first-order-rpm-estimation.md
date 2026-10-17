# 57 — First-order RPM estimation

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The old `alwaysInstantRpm` boolean becomes Per-cycle/First Order/Instant. First Order extrapolates from cycle RPM using a smoothed cycle-derived rate; spin-up temporarily uses instant RPM until valid cycle measurements can advance the RPM state. Phase-resynchronization disturbances do not update that baseline.

## ECU logic and behavior

1. Replace the old instant-RPM boolean with a three-mode selector.
2. In First Order mode, form a smoothed rate from cycle measurements and extrapolate from the cycle baseline.
3. During spin-up, use instant RPM until cycle measurements can advance the state.
4. Avoid incorporating phase-resynchronization disturbances into that baseline.
5. Keep the selector migration and spin-up fixes with the estimator.

**Illustrative behavior:** The ECU can react between completed cycles to an accelerating engine, while startup still gets a usable instantaneous RPM estimate.

## Calibration, dependencies and review limits

This is a central timing-input change affecting many controls and test fixtures. The enum/boolean migration and spin-up transitions are part of the same unit, not optional follow-ups.

Check initial edges, transition out of SPINNING_UP, phase resync and acceleration sign. Many downstream controllers consume RPM, so plausible display values alone do not validate scheduling.

Existing branch test evidence: [unit_tests/tests/trigger/test_fasterEngineSpinningUp.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/trigger/test_fasterEngineSpinningUp.cpp), [unit_tests/tests/trigger/test_rpm_acceleration.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/trigger/test_rpm_acceleration.cpp).

These test files change by +79 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 95 changed lines — 79 additions and 16 deletions across 2 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+79 / −16**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/engine_cycle/rpm_calculator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/engine_cycle/rpm_calculator.cpp) | +71 / −15 | +71 / −15 |
| [firmware/controllers/engine_cycle/rpm_calculator.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/engine_cycle/rpm_calculator.h) | +8 / −1 | +8 / −1 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-25 | [258250831f6](https://github.com/ElDominio/rusefi/commit/258250831f67c9573578ac9e383a14f4265466fc) — Add rpmUpdateMode: Per-cycle, First Order, Instant | +37 / −18 |
| 2026-07-06 | [1b78eb9cfcd](https://github.com/ElDominio/rusefi/commit/1b78eb9cfcd555859540426988cc8b1efd20085c) — Make First Order RPM rate smoothing tunable (rpmRateSmoothingPct) | +6 / −4 |
| 2026-08-24 | [17d0436c857](https://github.com/ElDominio/rusefi/commit/17d0436c8577bb4aecb33dea9491fbfc6c4c3b6e) — RPM_UPDATE_FIRST_ORDER: use instant RPM during spin-up | +10 / −3 |
| 2026-08-24 | [459e3fbdb0c](https://github.com/ElDominio/rusefi/commit/459e3fbdb0c44b09068b6fa2c3a45c9d9fc3db6f) — RPM_UPDATE_FIRST_ORDER: fix engine latched in SPINNING_UP forever | +15 / −1 |

Supporting configuration and source: [docs/AI/crank_angle_estimation.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/docs/AI/crank_angle_estimation.md).
