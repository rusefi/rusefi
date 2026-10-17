# 61b — Mitsubishi 6G75 experimental tooth-count crank synchronization

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

An experimental 6G75 crank decoder uses tooth counts and candidate-gap spacing with bounded weak-gap coasting. It is a distinct selectable algorithm with explicitly limited validation.

## ECU logic and behavior

1. Select the separate TT_36_2_1_1_V2 waveform/decoder path.
2. Use candidate-gap spacing plus tooth counts to establish crank synchronization.
3. Allow bounded coasting through weak gaps.
4. Retain the original trigger as another selection.
5. Do not rely on the attempted graduation-back-to-original behavior, which was reverted.

**Illustrative behavior:** A weak candidate gap may be coasted through rather than immediately handled as a clean definitive synchronization point.

## Calibration, dependencies and review limits

This is crank synchronization only. The separate 6G75 beta cam report covers phase resolution, and its full-phase state is also not physical TDC validation.

The replay tests pin observed errors/warnings and assume an unconfirmed time scale for two captures. They do not establish clean operating synchronization or calibrated timing.

Existing branch test evidence: [unit_tests/tests/trigger/test_real_6g75_v2.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/trigger/test_real_6g75_v2.cpp).

These test files change by +131 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 208 changed lines — 204 additions and 4 deletions across 5 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+204 / −48**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/trigger/decoders/trigger_structure.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/decoders/trigger_structure.cpp) | +8 / −0 | +8 / −16 |
| [firmware/controllers/trigger/decoders/trigger_universal.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/decoders/trigger_universal.cpp) | +14 / −0 | +14 / −0 |
| [firmware/controllers/trigger/decoders/trigger_universal.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/decoders/trigger_universal.h) | +4 / −0 | +4 / −0 |
| [firmware/controllers/trigger/trigger_decoder.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_decoder.cpp) | +132 / −3 | +132 / −31 |
| [firmware/controllers/trigger/trigger_decoder.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_decoder.h) | +46 / −1 | +46 / −1 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-09-07 | [4beccd0cdbb](https://github.com/ElDominio/rusefi/commit/4beccd0cdbbd05c0ae96fb5f3b5ea638e2d4dbd0) — Mitsubishi 6G75: experimental TT_36_2_1_1_V2 trigger with tooth-count sync | +137 / −0 |

Related integration/history commits outside the measured source-file scope:

- [13dd8ae0184 — Mitsubishi 6G72/6G75: fast crank/cam sync refinements + research docs](https://github.com/ElDominio/rusefi/commit/13dd8ae01844796822e31a82eabe9673a24e6e30): Common trigger-state refinements, including the intermediate 6G75 experiment.
- [59c9630a7ac — Mitsubishi trigger: revert 6G75 V2 graduation experiment, add VVT_MITSUBISHI_6G75_BETA cam sync](https://github.com/ElDominio/rusefi/commit/59c9630a7ac037e64c42dedb65a740b8c4f0e69c): Reverts the 6G75 graduation experiment while adding beta cam synchronization.
