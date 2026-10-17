# 58 — Automatic transmission controls

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Master's existing TCU framework is narrowed in the normal configured path: defaults/fixup force Automatic plus Generic4 when TCU is enabled. ButtonShiftController is removed and SimpleTransmissionController's shift-solenoid work is folded into Generic4. Automatic control gains an optional idle-to-first action and shift-margin telemetry; current gear is derived from gear detection rather than merely the last command.

## ECU logic and behavior

1. Select Automatic plus Generic4 through the normal TCU configuration path; remove ButtonShift and fold Simple solenoid work into Generic4.
2. Optionally request first gear at idle and report detected gear rather than only the last command.
3. Require sustained detected-gear agreement for shift completion, with the applicable timeout fallback.
4. Calculate line-pressure duty from a 5x5 RPM/driver-demand map plus shift, short lockup and per-gear additions.
5. Gate TCC by gear, temperature, brake and shift conditions.
6. Accumulate bounded signed slip trim above a speed/demand allowed-slip map, decay it after a clean interval and reset it on a commanded shift.

**Illustrative behavior:** A commanded shift resets learned slip trim; a subsequent excessive slip measurement adds a calibrated duty correction rather than measuring hydraulic pressure.

## Calibration, dependencies and review limits

The final line-pressure design is a 5×5 RPM/driver-demand base-duty table plus shift, short TCC-lockup, per-gear and slip-trim additions. TCC adds minimum gear/temperature, brake and shift gates. Shift completion requires a sustained detected-gear match, retaining a time fallback where applicable. Slip correction is a bounded, signed accumulator above a speed/demand maximum-slip table, decaying after a clean interval and resetting on a commanded shift. It is not closed-loop measured-pressure regulation.

These are separable TCU units, but the slip trim depends on sections 35–36, and idle-to-first depends on section 42. Earlier three-band pressure control and a duty slew ramp are superseded. Solenoid polarity must match the calibration; signed duty additions are not universally “more pressure.”

Check solenoid polarity, gear-ratio inference, shift completion and TCC transitions. The final design removes the earlier three-band base pressure and duty slew ramp; signed duty is not universally proportional to hydraulic pressure.

Existing branch test evidence: [unit_tests/tests/test_tcu.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_tcu.cpp).

These test files change by +237 / −68 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 716 changed lines — 343 additions and 373 deletions across 12 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+343 / −373**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/tcu/buttonshift.cpp](https://github.com/ElDominio/rusefi/blob/9154d40f573276b426b6f87ae6475cb9ea1a888c/firmware/controllers/tcu/buttonshift.cpp) | +0 / −87 | +0 / −87 |
| [firmware/controllers/tcu/buttonshift.h](https://github.com/ElDominio/rusefi/blob/9154d40f573276b426b6f87ae6475cb9ea1a888c/firmware/controllers/tcu/buttonshift.h) | +0 / −29 | +0 / −29 |
| [firmware/controllers/tcu/gc_auto.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/tcu/gc_auto.cpp) | +32 / −0 | +32 / −0 |
| [firmware/controllers/tcu/gear_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/tcu/gear_controller.cpp) | +8 / −5 | +8 / −5 |
| [firmware/controllers/tcu/gear_controller.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/tcu/gear_controller.h) | +8 / −2 | +8 / −2 |
| [firmware/controllers/tcu/simple_tcu.cpp](https://github.com/ElDominio/rusefi/blob/9154d40f573276b426b6f87ae6475cb9ea1a888c/firmware/controllers/tcu/simple_tcu.cpp) | +0 / −38 | +0 / −38 |
| [firmware/controllers/tcu/simple_tcu.h](https://github.com/ElDominio/rusefi/blob/9154d40f573276b426b6f87ae6475cb9ea1a888c/firmware/controllers/tcu/simple_tcu.h) | +0 / −18 | +0 / −18 |
| [firmware/controllers/tcu/tc_4.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/tcu/tc_4.cpp) | +153 / −179 | +153 / −179 |
| [firmware/controllers/tcu/tc_4.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/tcu/tc_4.h) | +22 / −4 | +22 / −4 |
| [firmware/controllers/tcu/tc_4l6x.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/tcu/tc_4l6x.h) | +0 / −2 | +0 / −2 |
| [firmware/controllers/tcu/tcu.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/tcu/tcu.cpp) | +89 / −9 | +89 / −9 |
| [firmware/controllers/tcu/tcu.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/tcu/tcu.h) | +31 / −0 | +31 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-07-31 | [25cfc53bb38](https://github.com/ElDominio/rusefi/commit/25cfc53bb388836ef7c441683c1f4537b5b70167) — TCU: wire up Automatic gear controller mode | +3 / −0 |
| 2026-08-01 | [8831d576169](https://github.com/ElDominio/rusefi/commit/8831d5761691d167d1ed2d5074c1e6ea41699383) — TCU: trim Transmission Settings to Automatic+Generic4, add idle-shift-to-first and gauges | +49 / −1 |
| 2026-09-06 | [57c53f02e2a](https://github.com/ElDominio/rusefi/commit/57c53f02e2ac7726be3ef1f12037b759560f3b9e) — TCU: simplify line pressure control to 3-band TPS demand, add TCC lock-up gating | +92 / −177 |
| 2026-09-11 | [47b939d3fc7](https://github.com/ElDominio/rusefi/commit/47b939d3fc779e42aeb8e459feb3f2df605e1b0f) — TCU: line pressure redesigned as a 2D RPM x TPS table + shift/lock-up/gear adders | +170 / −72 |
| 2026-09-11 | [f4d898df348](https://github.com/ElDominio/rusefi/commit/f4d898df34826db0ab2b1351f4fb6c2464a9dd5e) — TCU: slip-based closed-loop line pressure trim (no pressure sensor) | +73 / −2 |
| 2026-09-11 | [75699a900f3](https://github.com/ElDominio/rusefi/commit/75699a900f3f9cd0f2bf76f6cef5dd92414e8b2c) — TCU: remove ButtonShiftController, merge SimpleTransmissionController into Generic4TransmissionController | +41 / −191 |
| 2026-09-12 | [0c5f07d37e4](https://github.com/ElDominio/rusefi/commit/0c5f07d37e46286671532da3a8da75bd43209d54) — TCU: Max Allowed Slip as VSS x driver-demand table, remove line pressure duty ramp | +17 / −32 |
