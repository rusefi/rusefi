# DONE — 31 — Cooling fan pressure mode and PWM semantics

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Upstream completion

**DONE on upstream master**, reviewed at [`e3f1c1f935c`](https://github.com/rusefi/rusefi/commit/e3f1c1f935c1c6d47bd1bb0a1df1ef87a71e7dfe). Commit [`3da3feb4de3`](https://github.com/rusefi/rusefi/commit/3da3feb4de30e61d875c4d8a7d3b0b0d45b4be91) covers all six operational changes below: independent Disabled/Relay/Pressure modes, pressure demand without compressor engagement, shared relay/PWM inhibition, demand-space soft-start, inverted off/full-duty mapping and vehicle-speed hysteresis. It also supplies configuration, defaults, UI and telemetry.

Master refreshes diagnostic conditions before output-priority evaluation and stores fan settings in the main configuration instead of AlphaX page 6. It provides pressure mode without the fork's `EFI_AC_PRESSURE_FAN` build gate. The fork's manual `fan_pwm_reinit` diagnostic hook remains different; it is not required for the automatic fan-control behavior covered here.

Commit [`060dc248caa`](https://github.com/rusefi/rusefi/commit/060dc248caa047b059d7cd86d041ae913fd9e2fd) additionally migrates the old A/C enable booleans to Disabled/Relay while preserving existing mode values. This covers that field migration, not every historical PWM calibration. Upstream [fan-control tests](https://github.com/rusefi/rusefi/blob/e3f1c1f935c1c6d47bd1bb0a1df1ef87a71e7dfe/unit_tests/tests/actuators/test_fan_control.cpp) exercise pressure, inverted PWM, soft-start, inhibit gates and hysteresis; [migration tests](https://github.com/rusefi/rusefi/blob/e3f1c1f935c1c6d47bd1bb0a1df1ef87a71e7dfe/java_console/ui/src/test/java/com/rusefi/maintenance/migration/FanAcModeMigratorTest.java) cover the A/C field conversion. Tests were inspected, not rerun.

The analysis and LOC tables below preserve the original comparison against master `c12cf1498315`; statements about the earlier master and missing fork migration describe that historical snapshot, not current upstream status.

## Executive summary

Master already has relay/PWM fan control and, at the pinned revision, PWM stopped-engine and vehicle-speed inhibition fixes. The branch goes further: per-fan A/C modes become Disabled/Relay/Pressure, pressure can request airflow without compressor engagement, vehicle-speed inhibition gains hysteresis, and relay/PWM paths share inhibition logic.

## ECU logic and behavior

1. Evaluate common inhibit conditions before either relay or PWM output.
2. Select Disabled, compressor-relay or A/C-pressure assistance independently for each fan.
3. In pressure mode, pressure can create demand without compressor engagement.
4. For PWM, combine requested airflow in a normalized 0–100% demand space and ramp that demand.
5. Map demand to configured off/full-speed duty endpoints, including reversed endpoints.
6. Use hysteresis for the vehicle-speed inhibition boundary.

**Illustrative behavior:** An inverted PWM fan can use a high duty for off and a low duty for full airflow while the control calculation still increases demand from zero to 100%.

## Calibration, dependencies and review limits

PWM tables now describe 0–100% airflow demand, which is ramped and then mapped between “off” and “full-speed” duty endpoints. Reversed endpoints support inverted hardware. This changes the meaning of existing min/max duty settings, removes the old A/C PWM adder, and changes defaults. The Java boolean migration entries for the old fan toggles are removed; that alone is not evidence of an automatic conversion to the new mode fields.

Recalibrate old min/max duty values because their meaning changed. Compare against current master's already-fixed PWM inhibit behavior; those upstream fixes are not unique additions here.

Existing branch test evidence: [unit_tests/tests/actuators/test_fan_control.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_fan_control.cpp).

These test files change by +235 / −17 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 315 changed lines — 242 additions and 73 deletions across 2 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+246 / −90**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/modules/fan_control/fan_control.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/fan_control/fan_control.cpp) | +180 / −57 | +184 / −74 |
| [firmware/controllers/modules/fan_control/fan_control.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/fan_control/fan_control.h) | +62 / −16 | +62 / −16 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-24 | [bc27caf9033](https://github.com/ElDominio/rusefi/commit/bc27caf90335e64ed4f60e0a900a28261b6e5416) — Unify fan A/C control into a single mode (Disabled/Relay/Pressure) | +10 / −17 |
| 2026-06-29 | [43d748134d1](https://github.com/ElDominio/rusefi/commit/43d748134d13c86f8d4dabbca84afef60e2f6e75) — Add fan disable-at-speed hysteresis to prevent on/off cycling at the threshold speed | +22 / −1 |
| 2026-07-31 | [9977f244e4e](https://github.com/ElDominio/rusefi/commit/9977f244e4e904ceb28fc0fab194ca28c920ce05) — Fan control: A/C pressure mode ignores compressor state | +6 / −4 |
| 2026-09-11 | [12b5fc1690c](https://github.com/ElDominio/rusefi/commit/12b5fc1690cd092d7a9b644ca5687b1e48e3f8d2) — Fan control: shared inhibit gate, demand-space soft-start, inverted-PWM support | +145 / −73 |
