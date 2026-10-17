# 36 — Gear detection and transmission slip

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

Gear detection now respects clutch disengagement and can explicitly use output-shaft speed with engine or input-shaft RPM. The new slip estimate compares actual RPM with the expected RPM in the detected gear. It requires an OSS-based reference, a valid gear/RPM source and minimum vehicle speed, avoiding confusion between tire slip and transmission slip.

## ECU logic and behavior

1. Select engine or input-shaft RPM for gear inference and respect clutch disengagement.
2. Use the selected OSS-based reference with gear ratios to derive expected RPM.
3. Require a valid gear and minimum vehicle speed before reporting transmission slip.
4. Compare measured RPM with expected RPM in that gear.
5. Publish the result for the TCU pressure-trim consumer.

**Illustrative behavior:** A spinning tire and a slipping transmission are different measurements: this slip estimate is based on shaft/gear consistency, not front-versus-rear wheel speed.

## Calibration, dependencies and review limits

The gear-selection extension can be isolated from slip reporting. The slip output is also the foundation for the TCU pressure trim in section 58.

Check gear ambiguity, clutch transitions and the RPM source. Incorrect tire/final-drive or gear ratios can appear as transmission slip even without a mechanical slip event.

Existing branch test evidence: [unit_tests/tests/test_gear_detector.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_gear_detector.cpp).

These test files change by +58 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 112 changed lines — 105 additions and 7 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+105 / −7**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/modules/gear_detector/gear_detector.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/gear_detector/gear_detector.cpp) | +90 / −2 | +90 / −2 |
| [firmware/controllers/modules/gear_detector/gear_detector.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/gear_detector/gear_detector.h) | +14 / −0 | +14 / −0 |
| [firmware/init/sensor/init_input_shaft_speed_sensor.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_input_shaft_speed_sensor.cpp) | +1 / −5 | +1 / −5 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-29 | [135f45852f6](https://github.com/ElDominio/rusefi/commit/135f45852f690816fc92d13a8bc29839b414df8a) — Misc actuator/controller fixes: sport pedal live bit, upshift flat-shift priority, gear detector clutch-aware, fuel pump no-sensor fast path, etbSplit default | +15 / −0 |
| 2026-07-31 | [f15bda60222](https://github.com/ElDominio/rusefi/commit/f15bda602228bc854e590f49614c6c724d221792) — TCU: add "shared with main VSS" toggle for input speed sensor | +14 / −2 |
| 2026-08-17 | [b90fe2fb3ee](https://github.com/ElDominio/rusefi/commit/b90fe2fb3ee655f2b61ed221f34a5bfec42acb49) — Wheel Speed Sensors + Gear Setup: axle/OSS speed replaces legacy pin/CAN VSS | +17 / −20 |
| 2026-09-11 | [ad39074bfdb](https://github.com/ElDominio/rusefi/commit/ad39074bfdb5276a42b693d057d85efdd51cabdd) — Gear Setup: transmission slip detection (Detected Gear vs Speed Source) | +51 / −0 |
