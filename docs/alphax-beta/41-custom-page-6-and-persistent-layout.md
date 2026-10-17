# 41 — Custom page 6 and persistent layout

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

*INFRASTRUCTURE* — firmware support with an ECU runtime consequence.

## Executive summary

Master already has separate extra pages, including Lua page 5. The branch adds page 6 for custom controls, its default initializer and storage container, generator registration, TS page routing, SD record support and burn/load integration. The custom container's final data version is 23; the main flash-data version is 260912.

## ECU logic and behavior

1. Register custom calibration page 6 separately from Lua page 5.
2. Initialize, expose, burn and reload the custom container through the extra-page path.
3. Derive its internal-flash location from the actual Lua container size and align to 32 bytes.
4. Place the oil-life record afterward with overlap/sector-size checks.
5. On internal-flash-only builds, request deferred full configuration writes when burning extra pages.
6. The final custom version is 23 and main flash-data version is 260912.

**Illustrative behavior:** Increasing a board's Lua capacity moves the derived custom-page offset; using an old fixed offset could overlap calibration storage.

## Calibration, dependencies and review limits

Internal-flash placement derives page 6 from the end of the board-sized Lua container, aligned to 32 bytes. The oil-life record follows it, with overlap/sector-size assertions. Extra-page burns on internal-flash-only builds request a deferred full configuration write. Page growth, field movement, removed fields and renamed settings mean raw configuration images are not interchangeable with master; the report does not establish a complete automatic tune migration.

Check actual sector layout, interrupted writes, reboot persistence and tune migration for each MCU/board. The report establishes source behavior, not a complete safe migration of old raw images.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 530 changed lines — 521 additions and 9 deletions across 10 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+529 / −79**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 2 files are new since the common ancestor (+405 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/console/binary/tunerstudio.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/console/binary/tunerstudio.cpp) | +8 / −0 | +8 / −0 |
| [firmware/console/binary/tunerstudio.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/console/binary/tunerstudio.h) | +1 / −0 | +1 / −0 |
| [firmware/controllers/custom_page.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/custom_page.cpp) | +362 / −0 | +362 / −0 |
| [firmware/controllers/custom_page.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/custom_page.h) | +43 / −0 | +43 / −0 |
| [firmware/controllers/extra_flash_pages.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/extra_flash_pages.cpp) | +63 / −1 | +63 / −1 |
| [firmware/controllers/extra_flash_pages.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/extra_flash_pages.h) | +5 / −3 | +5 / −3 |
| [firmware/controllers/lua/lua_config_page.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/lua/lua_config_page.cpp) | +4 / −4 | +4 / −4 |
| [firmware/controllers/storage.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/storage.cpp) | +29 / −1 | +35 / −60 |
| [firmware/controllers/storage.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/storage.h) | +2 / −0 | +2 / −3 |
| [firmware/controllers/storage_sd.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/storage_sd.cpp) | +4 / −0 | +6 / −8 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-29 | [6097dfa0222](https://github.com/ElDominio/rusefi/commit/6097dfa022285c927c0e46638d2b622f45a0a8e5) — Move AlphaX custom features from TS page 5 to page 6; reserve page 5 for Lua | +24 / −21 |
| 2026-06-29 | [0d9bef019c0](https://github.com/ElDominio/rusefi/commit/0d9bef019c08e7e83e9dbb07d5257ba53221f39c) — Fix flash layout: compute PAGE6_SECTOR_OFFSET from Lua container size | +23 / −14 |

Related integration/history commits outside the measured source-file scope:

- [e3d628f3aab — Fix config generation: register AlphaX page 6 in TSProjectConsumer TS_PAGES table](https://github.com/ElDominio/rusefi/commit/e3d628f3aab11cf03c6cadcda618b30b3579acf6): *INFRASTRUCTURE* — generator page registration.
- [88b171035a9 — Fix page 6 generation: add handlePage(6) and update makefiles](https://github.com/ElDominio/rusefi/commit/88b171035a9af7a7af9ef0bbe0e4af3c93126882): *INFRASTRUCTURE* — generation and makefile follow-up.

Supporting configuration and source: [firmware/integration/config_page_6.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/integration/config_page_6.txt), [java_tools/configuration_definition/src/main/java/com/rusefi/output/TSProjectConsumer.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_tools/configuration_definition/src/main/java/com/rusefi/output/TSProjectConsumer.java).
