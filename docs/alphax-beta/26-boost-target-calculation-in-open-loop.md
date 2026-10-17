# 26 — Boost target calculation in open loop

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch computes boost target even in open-loop mode, allowing that target to serve as the duty table's Y axis. It adds a gear-based target adder and exposes target-temperature adjustments in that workflow. A state-machine limp ceiling can cap the computed target.

## ECU logic and behavior

1. Calculate the boost target even when boost duty is open loop.
2. Apply target corrections including gear and temperature additions.
3. Allow that target to index the open-loop duty map.
4. Apply the state-machine limp target ceiling at target calculation.
5. The ceiling only affects open-loop duty if that duty map actually uses target pressure as an axis.

**Illustrative behavior:** A gear-based target addition can move the open-loop lookup to a different duty row without closing a pressure-feedback loop.

## Calibration, dependencies and review limits

The limp ceiling only changes open-loop duty when the duty map actually uses boost target as its axis; it is not a general open-loop duty clamp.

Check duty-map axis selection and target units. Do not infer a guaranteed limp duty reduction from a target ceiling on a map indexed by another quantity.

Existing branch test evidence: [unit_tests/tests/actuators/test_boost.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_boost.cpp).

These test files change by +91 / −1 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 46 changed lines — 36 additions and 10 deletions across 2 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+36 / −10**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/boost_control.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/boost_control.cpp) | +32 / −10 | +32 / −10 |
| [firmware/controllers/actuators/gppwm_channel_reader.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/gppwm_channel_reader.cpp) | +4 / −0 | +4 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-18 | [4b7e4bf9278](https://github.com/ElDominio/rusefi/commit/4b7e4bf92786f41e3622405e997f11dbe6b62532) — test: add boost open-loop blend bias regression test | +0 / −0 |
| 2026-08-21 | [3ea6cbc638a](https://github.com/ElDominio/rusefi/commit/3ea6cbc638ac5f079153f3d1c2454be6390acb81) — Boost open loop: compute real target for the "Boost target kPa" Y axis option | +22 / −12 |
| 2026-08-21 | [187d3fa1e5b](https://github.com/ElDominio/rusefi/commit/187d3fa1e5b70d9cbfa16653048e287405f2a3f0) — Boost control follow-up: CLT/IAT adder ungating, target gear adder, enum fix | +4 / −0 |

Related integration/history commits outside the measured source-file scope:

- [e42ddbb2678 — Add ETB-only "TPS Target for Cranking" override; fix pre-existing GPPWM_BoostTarget enum gap](https://github.com/ElDominio/rusefi/commit/e42ddbb2678889194f289105dbf8b07990623f14): *INFRASTRUCTURE* — boost-target enum gap correction in a mixed ETB commit.
