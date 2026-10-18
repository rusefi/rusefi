# DONE — 19 — Burst knock retard

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Upstream completion

**DONE on upstream master**, reviewed at [`bb15031c451`](https://github.com/rusefi/rusefi/commit/bb15031c45118a859e42ca008ecbf6d2795639e2). Commit [`e08f9bf74f1`](https://github.com/rusefi/rusefi/commit/e08f9bf74f1e9aeee0938a146a0f7ee56a62edaa) implements the TPS-acceleration-enrichment trigger, RPM/rate table interpolation, nonnegative retard, linear decay and ignition subtraction described below. Module registration, live telemetry, tuning controls and disabled/zero-table defaults are included.

The [upstream controller](https://github.com/rusefi/rusefi/blob/bb15031c45118a859e42ca008ecbf6d2795639e2/firmware/controllers/modules/burst_knock/burst_knock.cpp) divides AE's largest adjacent TPS jump by the lookback, floored to 0.001 seconds. This is a rate estimate tied to AE history. It refreshes the table value and decay timer on every qualifying sample, then decays from the last value; it does not retain the maximum value seen throughout the transient. Disabling clears the correction and telemetry. The ignition path applies relative retard alongside rolling launch; existing fixed-timing and cranking-correction rules still govern whether corrections apply.

Master places the settings on the main configuration page and registers the module directly, rather than using the fork's page 6 and `EFI_BURST_KNOCK` gate. The adoption commit adds **85 maintained firmware C/C++ lines across five files**: the module `.cpp`/`.h`, `engine.h`, `live_data.cpp` and `ignition_state.cpp`. Generated code, schema, UI, build files, docs and tests are excluded.

The [upstream test file](https://github.com/rusefi/rusefi/blob/bb15031c45118a859e42ca008ecbf6d2795639e2/unit_tests/tests/ignition_injection/test_burst_knock.cpp) contains 288 added lines at the reviewed tip, including follow-up coverage from [`2f1ca596a0d`](https://github.com/rusefi/rusefi/commit/2f1ca596a0de583e6e8340e819fdcb5460a43c79). It exercises the real AE threshold/history, callback ordering, table interpolation, decay/retrigger, disable, negative cells and combined ignition corrections. Tests were inspected, not rerun. The fork analysis and LOC tables below preserve the original comparison.

## Executive summary

The new module uses acceleration enrichment's TPS jump/lookback to index an RPM/TPS-rate retard table. It holds retard during the transient, then decays it to zero. Negative table values cannot advance timing.

## ECU logic and behavior

1. Reuse the TPS acceleration-enrichment delta and divide by a lookback clamped to at least 0.001 seconds.
2. While the acceleration threshold is exceeded, interpolate the RPM/TPS-rate retard table and reset the decay timer.
3. Clamp negative retard to zero.
4. After the transient, linearly reduce the latched retard to zero over the configured decay time.
5. The ignition consumer subtracts the resulting retard.

**Illustrative behavior:** A throttle tip-in can request temporary retard even when no acoustic knock event has been detected.

## Calibration, dependencies and review limits

Despite its name, this is feed-forward transient timing retard, not a new acoustic knock detector. It is reasonably separable with its ignition consumer and page-6 fields.

Check acceleration-enrichment calibration and retard sign. Disabling the module clears its active correction; this is feed-forward protection, not knock detection.

Existing branch test evidence: [unit_tests/tests/ignition_injection/test_burst_knock.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_burst_knock.cpp).

These test files change by +100 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 166 changed lines — 153 additions and 13 deletions across 3 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+153 / −13**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+94 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/ignition/ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp) | +59 / −13 | +59 / −13 |
| [firmware/controllers/modules/burst_knock/burst_knock.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/burst_knock/burst_knock.cpp) | +66 / −0 | +66 / −0 |
| [firmware/controllers/modules/burst_knock/burst_knock.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/burst_knock/burst_knock.h) | +28 / −0 | +28 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +148 / −12 |
