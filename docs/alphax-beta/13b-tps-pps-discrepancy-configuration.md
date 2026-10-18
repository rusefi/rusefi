# 13b — TPS/PPS discrepancy configuration

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Upstream progress — open

Reviewed master at [`41e3c63a986`](https://github.com/rusefi/rusefi/commit/41e3c63a9865141686bbfa7940defe0e86a4cb11). TPS smoothing is complete in [report 13a](DONE-13a-tps-smoothing.md), but this independent calibration change remains absent. Master's [schema](https://github.com/rusefi/rusefi/blob/41e3c63a9865141686bbfa7940defe0e86a4cb11/firmware/integration/rusefi_config.txt) retains the default `MAX_TPS_PPS_DISCREPANCY` of 5 and `etbSplit` scale of 0.1. Its initialization and default-setting code still use the maximum constant. Board overrides may differ, so 5 is the general default rather than a universal limit for every board.

## Executive summary

The fork raises the general maximum configurable TPS/PPS discrepancy from 5 to 30 percentage points and changes the stored `etbSplit` scale from 0.1 to 0.5. It separately keeps both the normal default and invalid-setting fallback at 5. This allows a tuner to accept larger disagreement between redundant sensor channels without raising the fresh-tune default.

## ECU logic and behavior

1. The configuration schema permits `etbSplit` values up to the new maximum of 30 and represents each stored step as 0.5 percentage points.
2. Base defaults assign `etbSplit = 5`, independently of that maximum.
3. During redundant-pair initialization, a value at or below zero or above the configured maximum is replaced with 5.
4. The resulting threshold configures ordinary or Ford-style redundancy checks for TPS/PPS pairs. TPS smoothing happens after redundancy validation and does not remove this check.

**Illustrative behavior:** With the general schema defaults, a selected threshold of 10 survives the fork's range check, while master treats it as above the maximum and resets it to 5. A fresh tune still starts at 5 in both trees.

## Calibration, dependencies and review limits

This split from the former report 13 preserves the unfinished behavior separately from completed smoothing. The schema scale change also changes the interpretation of raw stored values; the review does not establish a migration for existing binary configuration images. The actual threshold on a board depends on its generated configuration and any overrides. No dedicated test of the fork's wider discrepancy range was identified, and no firmware, simulator or hardware tests were run for this documentation review.

## Lines of code changed

**Isolated retained edits: +2 / −2 maintained C++ lines**, replacing the two assignments from `MAX_TPS_PPS_DISCREPANCY` with literal `5`. The associated operational schema edits are **+2 / −2 lines** separately: the maximum definition and `etbSplit` scale. These are narrow hunk counts; the schema is excluded from the C++ total.

For consistency with the index's file-scope convention, the two shared C++ files have a **262-line retained footprint: +238 / −24**, common ancestor `9154d40f5732` → fork `ebb877615ee1`. Original master `c12cf1498315` → fork is **+239 / −33** for those files. These physical line counts include unrelated changes, comments and blank lines, overlap report 13a and other reports, and must not be added as feature-exclusive totals.

| Measured source file | Common ancestor → fork | Original master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/defaults/default_base_engine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/defaults/default_base_engine.cpp) | +82 / −7 | +83 / −16 |
| [firmware/init/sensor/init_tps.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_tps.cpp) | +156 / −17 | +156 / −17 |

Reproduce the file-scope totals with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions remain in the [comparison scope](../alphax-beta-vs-master.md#comparison-scope). The schema changes are in [firmware/integration/rusefi_config.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/integration/rusefi_config.txt).

## Relevant commits

Both are mixed commits; the C++ delta below is restricted to the two measured source files, compared with each commit's first parent.

| Date | Commit and relevant change | C++ source delta |
| --- | --- | ---: |
| 2026-06-29 | [135f45852f6](https://github.com/ElDominio/rusefi/commit/135f45852f690816fc92d13a8bc29839b414df8a) — Misc actuator/controller fixes; fixes the invalid `etbSplit` fallback at 5 in `init_tps.cpp`. | +1 / −1 |
| 2026-06-29 | [af7d547501f](https://github.com/ElDominio/rusefi/commit/af7d547501fab009549b293e742c7fab00ba085a) — Config/integration update; keeps the base default at 5 and changes the schema maximum/scale. The two relevant schema-line replacements are outside this C++ count. | +1 / −1 |
