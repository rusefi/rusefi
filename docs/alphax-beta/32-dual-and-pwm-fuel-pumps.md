# 32 — Dual and PWM fuel pumps

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The existing single relay pump gains dual-relay staging and PWM pressure control. Dual mode enables the secondary pump when RPM, load and TPS thresholds all pass, with hysteresis on release. PWM mode combines a load/RPM pressure target, pressure/RPM base-duty map and PID; invalid pressure falls back to feed-forward. Aggressive relief holds minimum duty during low-demand overpressure until its recovery threshold is crossed.

## ECU logic and behavior

1. Select single relay, staged dual relay or pressure-regulated PWM operation.
2. Dual staging requires configured RPM, load and TPS checks together, with release hysteresis.
3. PWM combines a load/RPM pressure target, pressure/RPM base duty and PID correction.
4. If pressure is invalid, use the feed-forward path instead of computing feedback from an invalid observation.
5. Aggressive relief latches during low-demand overpressure and releases at its lower recovery threshold.
6. An invalid output requests configured minimum duty; priming requests maximum duty.

**Illustrative behavior:** During low fuel demand with excess pressure, relief holds minimum pump duty until pressure recovers instead of winding down the PID indefinitely.

## Calibration, dependencies and review limits

This combines output initialization, pressure sensing, PID limits and custom configuration. The PWM fallback is the configured minimum duty, including the invalid-output path; that is a material difference from interpreting every inactive condition as a zero-duty relay command.

Check minimum-duty electrical meaning, no-pressure fallback, prime duration and both relief thresholds. Minimum duty may be nonzero and should not be interpreted as relay off.

Existing branch test evidence: [unit_tests/tests/actuators/test_fuel_pump_pwm.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_fuel_pump_pwm.cpp).

These test files change by +399 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 245 changed lines — 236 additions and 9 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+236 / −9**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/modules/fuel_pump/fuel_pump.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/fuel_pump/fuel_pump.cpp) | +195 / −8 | +195 / −8 |
| [firmware/controllers/modules/fuel_pump/fuel_pump.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/fuel_pump/fuel_pump.h) | +37 / −1 | +37 / −1 |
| [firmware/init/sensor/init_fluid_pressure.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_fluid_pressure.cpp) | +4 / −0 | +4 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +203 / −9 |
| 2026-06-29 | [135f45852f6](https://github.com/ElDominio/rusefi/commit/135f45852f690816fc92d13a8bc29839b414df8a) — Misc actuator/controller fixes: sport pedal live bit, upshift flat-shift priority, gear detector clutch-aware, fuel pump no-sensor fast path, etbSplit default | +9 / −1 |
| 2026-08-14 | [8b03c29b748](https://github.com/ElDominio/rusefi/commit/8b03c29b74820e907b499361260094bb22b21e8f) — Add Aggressive Pressure Relief mode for PWM fuel pump control | +9 / −0 |
| 2026-08-15 | [60ecde896bc](https://github.com/ElDominio/rusefi/commit/60ecde896bcaff9774371dc6eae8a916c7869368) — Add hysteresis to Aggressive Pressure Relief, fix FP duty gauge precision | +16 / −4 |
