# 61a — Mitsubishi 6G72 provisional crank/cam synchronization

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch adds an early provisional phase path for the 6G72 crank/cam combination. It changes when the ECU can proceed with initial firing modes while retaining a later confirmed-phase distinction.

## ECU logic and behavior

1. Use TT_6G72_CRANK with VVT_MITSUBISHI_6G72_BETA.
2. Observe crank/cam edge order and levels to select a provisional phase early.
3. Keep provisional phase distinct from fully confirmed cam synchronization.
4. Permit the intended wasted-spark/batch operation while the ordinary cam decoder establishes confirmed phase.
5. Limit the fast lookup to the observed alignment documented in source.

**Illustrative behavior:** The ECU can obtain useful early phase information without claiming it has already verified sequential cylinder identity.

## Calibration, dependencies and review limits

Common decoder state and the protection gate change with the waveform. Counts for those shared files also include the separate 6G75 and sync-diagnostic work.

Validate the specific crank/cam alignment and physical cylinder-1 timing. Other alignments require re-deriving the lookup; host replay alone is not a universal phase guarantee.

Existing branch test evidence: [unit_tests/tests/trigger/test_real_6g72_3000gt.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/trigger/test_real_6g72_3000gt.cpp).

These test files change by +101 / −4 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 613 changed lines — 603 additions and 10 deletions across 8 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+606 / −87**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/limp_manager.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/limp_manager.cpp) | +74 / −2 | +75 / −26 |
| [firmware/controllers/trigger/decoders/trigger_mitsubishi.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/decoders/trigger_mitsubishi.cpp) | +79 / −0 | +79 / −0 |
| [firmware/controllers/trigger/decoders/trigger_mitsubishi.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/decoders/trigger_mitsubishi.h) | +1 / −0 | +1 / −0 |
| [firmware/controllers/trigger/decoders/trigger_structure.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/decoders/trigger_structure.cpp) | +8 / −0 | +8 / −16 |
| [firmware/controllers/trigger/trigger_central.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_central.cpp) | +219 / −3 | +221 / −12 |
| [firmware/controllers/trigger/trigger_central.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_central.h) | +44 / −1 | +44 / −1 |
| [firmware/controllers/trigger/trigger_decoder.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_decoder.cpp) | +132 / −3 | +132 / −31 |
| [firmware/controllers/trigger/trigger_decoder.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_decoder.h) | +46 / −1 | +46 / −1 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-09-07 | [cc6b6a6d840](https://github.com/ElDominio/rusefi/commit/cc6b6a6d840d923fe5101d0fee04ceff30bedbb9) — Mitsubishi 6G72: fast crank+cam sync via TT_6G72_CRANK + VVT_MITSUBISHI_6G72_BETA | +135 / −7 |
| 2026-09-11 | [13dd8ae0184](https://github.com/ElDominio/rusefi/commit/13dd8ae01844796822e31a82eabe9673a24e6e30) — Mitsubishi 6G72/6G75: fast crank/cam sync refinements + research docs | +104 / −46 |
