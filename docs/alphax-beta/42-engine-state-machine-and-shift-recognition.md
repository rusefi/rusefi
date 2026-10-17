# 42 — Engine state machine and shift recognition

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The new state machine classifies Off, Cranking, Afterstart, Idle, Coasting, Transient, WOT, Cruising, Overrun, Accelerating and Decelerating conditions, then publishes launch, shift, torque-reduction and operating-mode overlays. Master has subsystem states but no corresponding AlphaX coordinator.

## ECU logic and behavior

1. Classify the engine from RPM, throttle/load, speed and configured thresholds.
2. Track startup/idle/coast/transient/WOT/cruise/acceleration conditions separately from launch and mode overlays.
3. Recognize shift direction using a rate accumulator, clutch edges and optional second-switch handling.
4. Apply observation cadence, latch timing and stale-window expiration.
5. Express transient hold in milliseconds rather than a callback count.
6. Expose the state to STFT region selection and dependent control features.

**Illustrative behavior:** Off-throttle travel above the idle-speed gate is classified as Coasting instead of ordinary stationary idle, changing which dependent modes can engage.

## Calibration, dependencies and review limits

Shift direction uses an accumulated RPM-rate or vehicle-speed-rate signal, clutch edges, optional second-switch handling, minimum speed, latch timing and a stale-window timeout. RPM-rate observation has a tunable cadence; transient hold is measured in milliseconds. State-based STFT region selection is a separate consumer. This is a central dependency of many later items.

Check transition priority, stale inputs and time-base independence. A mode flag alone is not its actuator effect; the corresponding consumer reports describe those effects.

Existing branch test evidence: [unit_tests/tests/engine_state_machine/test_engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/engine_state_machine/test_engine_state_machine.cpp), [unit_tests/tests/test_stft.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_stft.cpp).

These test files change by +1,793 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 1,299 changed lines — 1,299 additions and 0 deletions across 4 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+1,299 / −0**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+1,268 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp) | +1,054 / −0 | +1,054 / −0 |
| [firmware/controllers/algo/engine_state_machine.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.h) | +214 / −0 | +214 / −0 |
| [firmware/controllers/math/closed_loop_fuel.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/math/closed_loop_fuel.cpp) | +25 / −0 | +25 / −0 |
| [firmware/controllers/math/closed_loop_fuel.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/math/closed_loop_fuel.h) | +6 / −0 | +6 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +800 / −0 |
| 2026-06-18 | [c4780944936](https://github.com/ElDominio/rusefi/commit/c478094493637e473e83bbd9751b5142a03d2f8a) — Shift torque speed gate, eco mode MAP gate, CDV smart mode, launch clutch-up fix | +14 / −11 |
| 2026-06-20 | [0daf3580054](https://github.com/ElDominio/rusefi/commit/0daf35800544100e54a20412633a4fe6d33d5da7) — Fix clutch-up shift detection edge and add second-switch support | +62 / −28 |
| 2026-06-29 | [53b81084cc1](https://github.com/ElDominio/rusefi/commit/53b81084cc131057622a5736011afe949c7b46e7) — Rework Engine SM shift detection: replace history-window with rate accumulator; add Accelerating/Decelerating states | +215 / −143 |
| 2026-07-02 | [d290f944cef](https://github.com/ElDominio/rusefi/commit/d290f944cef02d881eaeacce0a3378c9d8b38582) — Add configurable averaging window for Engine SM RPM rate (RPM/t) | +58 / −10 |
| 2026-07-06 | [5b1b6444c2b](https://github.com/ElDominio/rusefi/commit/5b1b6444c2b8742d28b7bfb899a423bba82b63ef) — Add Accelerating-hold latch to Engine State Machine (smAccelHoldMs) | +15 / −0 |
| 2026-07-07 | [bad3e839579](https://github.com/ElDominio/rusefi/commit/bad3e839579af16cc3d6d1550bc34ed5831164fa) — Add Sport Mode overlay to Engine State Machine | +82 / −75 |
| 2026-07-10 | [9bae8c75b0a](https://github.com/ElDominio/rusefi/commit/9bae8c75b0aace8d424cf8f05d75d659a7cc1930) — Engine State Machine: classify off-throttle rolling above maxIdleVss as Coasting | +49 / −15 |
| 2026-07-10 | [c8a510c3c0d](https://github.com/ElDominio/rusefi/commit/c8a510c3c0ddaa1ab36205d8ceb118e1fd1a716b) — Engine State Machine: add time-based shift accumulator snap (smAccumulatorSnapTimeMs) | +36 / −0 |
| 2026-09-18 | [6c8d191c23c](https://github.com/ElDominio/rusefi/commit/6c8d191c23cfa20823e9fa085f1515d483a76934) — Engine SM: convert transient hold-off from callback count to milliseconds | +14 / −14 |
