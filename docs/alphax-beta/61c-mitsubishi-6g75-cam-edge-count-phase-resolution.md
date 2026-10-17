# 61c — Mitsubishi 6G75 cam-edge-count phase resolution

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch adds a beta cam decoder that resolves 6G75 phase from alternating edge counts instead of the old cam-gap approach.

## ECU logic and behavior

1. Select VVT_MITSUBISHI_6G75_BETA instead of the older cam-gap decoder.
2. Count cam edges in alternating intervals and use the observed three-versus-four pattern to resolve phase.
3. Publish the resulting full-phase synchronization state through common trigger logic.
4. Carry the registered decoder/build dependencies with the implementation.

**Illustrative behavior:** A replay can reach the full-phase state even though the correspondence to the real cylinder-1 compression TDC has not been established.

## Calibration, dependencies and review limits

The retained test establishes entry to full-phase state, explicitly not physical cylinder-1/TDC alignment. This caveat is independent of the experimental crank decoder's own errors.

Verify physical phase against an independent cylinder/TDC reference before treating the state flag as sequential-operation validation.

Existing branch test evidence: [unit_tests/tests/trigger/test_real_6g75_beta_cam.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/trigger/test_real_6g75_beta_cam.cpp).

These test files change by +48 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 355 changed lines — 351 additions and 4 deletions across 5 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+353 / −29**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/trigger/decoders/trigger_mitsubishi.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/decoders/trigger_mitsubishi.cpp) | +79 / −0 | +79 / −0 |
| [firmware/controllers/trigger/decoders/trigger_mitsubishi.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/decoders/trigger_mitsubishi.h) | +1 / −0 | +1 / −0 |
| [firmware/controllers/trigger/decoders/trigger_structure.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/decoders/trigger_structure.cpp) | +8 / −0 | +8 / −16 |
| [firmware/controllers/trigger/trigger_central.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_central.cpp) | +219 / −3 | +221 / −12 |
| [firmware/controllers/trigger/trigger_central.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_central.h) | +44 / −1 | +44 / −1 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-09-11 | [13dd8ae0184](https://github.com/ElDominio/rusefi/commit/13dd8ae01844796822e31a82eabe9673a24e6e30) — Mitsubishi 6G72/6G75: fast crank/cam sync refinements + research docs | +104 / −46 |
| 2026-09-11 | [59c9630a7ac](https://github.com/ElDominio/rusefi/commit/59c9630a7ac037e64c42dedb65a740b8c4f0e69c) — Mitsubishi trigger: revert 6G75 V2 graduation experiment, add VVT_MITSUBISHI_6G75_BETA cam sync | +202 / −98 |

Related integration/history commits outside the measured source-file scope:

- [8280e29e3b0 — Fix: register VVT_MITSUBISHI_6G75_BETA build dependencies (completes 6G75 beta cam commit)](https://github.com/ElDominio/rusefi/commit/8280e29e3b013b83eaf29a43c01300fbc13a608b): *INFRASTRUCTURE* — registers the beta cam decoder and test dependencies.
