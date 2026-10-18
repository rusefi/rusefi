# DONE — 51 — State-aware DFCO

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Upstream completion

**Core state-aware DFCO is DONE on upstream master**, reviewed at adoption commit [`359a64d8a0a`](https://github.com/rusefi/rusefi/commit/359a64d8a0a0b49730c94088e5986aa519472114). The [upstream controller](https://github.com/rusefi/rusefi/blob/359a64d8a0a0b49730c94088e5986aa519472114/firmware/controllers/algo/fuel/dfco.cpp) implements Overrun/Decel/Both selection, common eligibility guards, optional overrun gear gating, state-machine deceleration and absent-speed fallback, and inhibition during either shift direction. **The fork's additional Pops-and-bangs inhibition remains open under [report 47](47-pops-and-bangs.md)**; this completion does not claim that mode or its fuel-cut integration is present.

Every mode honors the feature enable, VE-tuning and Lua inhibition, valid warm coolant, and MAP validity/hysteresis when a MAP sensor exists. Overrun retains throttle/RPM/speed thresholds and RPM/speed hysteresis, optionally requiring `DetectedGear > 0`; invalid, unknown, neutral and nonpositive gear readings then prevent that path. These fuel-cut guards remain independent of the [state machine's physical Overrun classification](https://github.com/rusefi/rusefi/blob/359a64d8a0a0b49730c94088e5986aa519472114/firmware/controllers/modules/engine_state_machine/engine_state_machine.cpp).

Decel requires an enabled state machine and accepts its latest Decelerating flag. That classification requires valid speed and detected neutral gear, subject to primary-state priority. With no configured speed sensor, a positive threshold instead permits `getLastRpmRate() < -smDecelRateThreshold`; a configured but failed speed sensor does not permit this fallback. The fallback does not change the published primary state. Decel is independent of the overrun throttle/RPM/speed/gear gates, and Both accepts either path.

Enabled upshift/downshift flags suppress both paths and restart the fuel-cut entry delay. Disabling the state machine immediately removes its Decel request and shift inhibition, even before the next slow callback clears the flags. Existing injection cut, timing-retard, trim-recovery and `dfcoActive` telemetry paths remain in use; the fork-only `dfcoCutActive` field is not required for this completion.

The [enum](https://github.com/rusefi/rusefi/blob/359a64d8a0a0b49730c94088e5986aa519472114/firmware/controllers/algo/rusefi_enums.h) makes zero select Overrun. The [main-page schema](https://github.com/rusefi/rusefi/blob/359a64d8a0a0b49730c94088e5986aa519472114/firmware/integration/rusefi_config.txt) and [tuning controls](https://github.com/rusefi/rusefi/blob/359a64d8a0a0b49730c94088e5986aa519472114/firmware/tunerstudio/tunerstudio.template.ini) expose modes, gear gating and state-machine rate settings. Zero defaults preserve overrun without a gear requirement or an enabled state machine; master does not adopt the fork's custom-page layout.

The adoption commit changes **91 maintained firmware C/C++ lines: +70 / −21 across three files**: `dfco.cpp` (+58/−20), `dfco.h` (+5/−1) and `rusefi_enums.h` (+7/−0). Schema, tuning UI, documentation and tests are excluded. [Upstream fuel-cut tests](https://github.com/rusefi/rusefi/blob/359a64d8a0a0b49730c94088e5986aa519472114/unit_tests/tests/ignition_injection/test_fuelCut.cpp) separately change by +279/−1 and cover mode combinations, defaults, invalid gear, shared guards and MAP hysteresis, physical-state independence, absent versus failed speed, the strict RPM-rate threshold, shift suppression/delayed re-entry and actual injection behavior. Tests were inspected, not rerun. Historical fork LOC below is unchanged.

## Executive summary

The fork extends overrun fuel cut with selectable Overrun, Decel or Both modes. Common guards remain separate from physical overrun detection, overrun can require a detected gear, and Decel uses state-machine deceleration with an RPM-rate fallback when vehicle speed is absent. Active up/down shifts suppress the cut. This core behavior is now upstream; the fork's additional P&B suppression remains part of open report 47.

## ECU logic and behavior

1. Apply common eligibility guards separately from physical overrun classification.
2. Select Overrun, Decel or Both mode.
3. Optionally require a detected engaged gear for the overrun path.
4. Use the state-machine deceleration classification, including an RPM-rate fallback without vehicle speed.
5. Suppress the cut during active up/down shifts. The fork additionally suppresses it during P&B, whose upstream integration remains tracked in [report 47](47-pops-and-bangs.md).

**Illustrative behavior:** A clutch-assisted shift can inhibit fuel cut even when throttle is closed, avoiding a conflict with the shift-assist torque request.

## Calibration, dependencies and review limits

This depends on state classification and shift behavior, not just renamed DFCO fields. Coverage extends fuel-cut tests.

Check missing-speed behavior, gear validity and re-entry after shift/P&B exits. The changed guards and new mode enum must be migrated with calibration and tests.

Existing branch test evidence: [unit_tests/tests/ignition_injection/test_fuelCut.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_fuelCut.cpp).

These test files change by +47 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 1,195 changed lines — 1,156 additions and 39 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+1,156 / −39**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 1 files are new since the common ancestor (+1,054 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp) | +1,054 / −0 | +1,054 / −0 |
| [firmware/controllers/algo/fuel/dfco.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/dfco.cpp) | +92 / −37 | +92 / −37 |
| [firmware/controllers/algo/fuel/dfco.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/dfco.h) | +10 / −2 | +10 / −2 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-20 | [652c6887882](https://github.com/ElDominio/rusefi/commit/652c6887882d6d3507be3eca9d1a52a1b6540b79) — Add optional gear-gated overrun fuel cut requirement | +5 / −0 |
| 2026-06-29 | [4e3cd5e6267](https://github.com/ElDominio/rusefi/commit/4e3cd5e62675132ef169269c87ed6d28682b8385) — Rework DFCO: split overrun/decel/both modes; refactor guards; gear-engaged check for overrun | +62 / −21 |
