# 38 — Cylinder-head sensor and thermal estimates

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch adds a configurable CHT input, an optional CLT estimate based on heat transfer from the head versus radiator rejection, and a separate EOT estimate using CHT, intake/ambient temperature and oil pressure. CLT estimation includes thermostat lag, coolant-response lag, fan airflow and saturating speed-related airflow. EOT uses separate heating/cooling time constants and limits heating when oil is not circulating.

## ECU logic and behavior

1. Register and calibrate the physical cylinder-head-temperature input.
2. For estimated CLT, balance heat input from the head against radiator rejection, with thermostat and coolant response lag.
3. Account for fan airflow and a saturating vehicle-speed airflow term.
4. For estimated EOT, combine head and ambient/intake temperature with oil-pressure evidence of circulation and separate heating/cooling response.
5. Publish estimates under the ordinary coolant/oil sensor identities used elsewhere.

**Illustrative behavior:** Changing the CLT estimator calibration can change warmup fueling and fan behavior even though neither consumer was edited, because both read the published coolant temperature.

## Calibration, dependencies and review limits

The physical CHT sensor, CLT model and EOT model are three separable pieces. The estimates publish under normal temperature sensor identities, affecting all downstream users; fallback numbers are calibration assumptions, not measured temperatures.

Verify initial conditions, invalid inputs and real thermal calibration. These model outputs are not independent sensors; the Econoline CHT calibration is explicitly provisional.

Existing branch test evidence: [unit_tests/tests/sensor/test_cht_clt_estimator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/sensor/test_cht_clt_estimator.cpp), [unit_tests/tests/sensor/test_eot_estimator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/sensor/test_eot_estimator.cpp).

These test files change by +403 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 463 changed lines — 452 additions and 11 deletions across 5 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+455 / −18**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 4 files are new since the common ancestor (+410 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/sensors/cht_clt_estimator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/cht_clt_estimator.cpp) | +183 / −0 | +183 / −0 |
| [firmware/controllers/sensors/cht_clt_estimator.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/cht_clt_estimator.h) | +81 / −0 | +81 / −0 |
| [firmware/controllers/sensors/eot_estimator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/eot_estimator.cpp) | +98 / −0 | +98 / −0 |
| [firmware/controllers/sensors/eot_estimator.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/eot_estimator.h) | +48 / −0 | +48 / −0 |
| [firmware/init/sensor/init_thermistors.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_thermistors.cpp) | +42 / −11 | +45 / −18 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-30 | [0f2d76bb6a5](https://github.com/ElDominio/rusefi/commit/0f2d76bb6a5e6e1043b300dac59dab8b7ca3bb01) — Add CHT sensor with CHT-to-CLT and EOT-from-CHT/IAT thermal estimators | +288 / −11 |
| 2026-07-02 | [ff8488bd255](https://github.com/ElDominio/rusefi/commit/ff8488bd2553cecb701036027d8fa2d458510175) — Rewrite CHT-to-CLT estimator as a competing-rate radiator model | +157 / −38 |
| 2026-07-03 | [fd4ae4d1f80](https://github.com/ElDominio/rusefi/commit/fd4ae4d1f80197df55285c46c858787254fd4717) — Convert CLT estimator CHT/rejection curves to tunable base+gain | +21 / −2 |
| 2026-07-03 | [228a4b680db](https://github.com/ElDominio/rusefi/commit/228a4b680dbf4a293b53b8f3e53f42334e1d46a6) — Convert CLT estimator IAT/AAT curve to reference temp + gain | +10 / −1 |
| 2026-07-03 | [e507d97e4b0](https://github.com/ElDominio/rusefi/commit/e507d97e4b053682fd53f8e8d813431edd70fa62) — Convert CLT estimator VSS ram-air curve to a saturating exponential | +21 / −4 |
