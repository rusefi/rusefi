# 21 — Launch switch semantics and Lua controls

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch adds a clutch-up activation choice: launch is active while the clutch-up switch is not asserted. A separate setting lets an asserted clutch-up switch cancel other launch activation modes. A new `setLaunchRpm(rpm)` Lua hook clamps its input to 0–20,000 and changes the live setting.

## ECU logic and behavior

1. The added clutch-up activation mode interprets a nonasserted clutch-up switch as the launch-active condition.
2. A separate cancellation option uses a positively asserted clutch-up switch to cancel other activation modes.
3. The Lua hook clamps launch RPM to 0–20,000 and changes the live setting.
4. Existing RPM, speed and other launch gates still determine the final launch response.

**Illustrative behavior:** With clutch-up cancellation enabled, releasing the clutch can cancel button-selected launch without treating every missing switch assertion as a valid pedal-depressed measurement.

## Calibration, dependencies and review limits

Keep the positive release check distinct from assuming a nonasserted switch proves clutch depression. Current master already has much of the broader launch/flat-shift machinery seen in historical commits.

Check switch polarity, unconfigured inputs and interaction with flat shift. The Lua change is a live setting change, not evidence of an automatic persistent burn.

Existing branch test evidence: [unit_tests/tests/lua/test_lua_hooks.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/lua/test_lua_hooks.cpp), [unit_tests/tests/test_launch.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_launch.cpp).

These test files change by +119 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 49 changed lines — 47 additions and 2 deletions across 2 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+54 / −98**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/launch_control.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/launch_control.cpp) | +18 / −2 | +18 / −2 |
| [firmware/controllers/lua/lua_hooks.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/lua/lua_hooks.cpp) | +29 / −0 | +36 / −96 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-17 | [286e7402110](https://github.com/ElDominio/rusefi/commit/286e74021107959f1afbe41826f56d060d00709a) — launch control: add clutch-up activation mode; alphax-gold: persist page 5/4 to flash | +5 / −2 |
| 2026-06-18 | [c4780944936](https://github.com/ElDominio/rusefi/commit/c478094493637e473e83bbd9751b5142a03d2f8a) — Shift torque speed gate, eco mode MAP gate, CDV smart mode, launch clutch-up fix | +3 / −1 |
| 2026-06-25 | [e96cde44624](https://github.com/ElDominio/rusefi/commit/e96cde446243b8092bf44a619e4e8eb0a2875446) — Add Rolling Launch Control: button-held captured-RPM hold via hard spark limiter to spool turbo while moving; flat timing/fuel retard held, linear timing ramp-out on release; EFI_ROLLING_LAUNCH page-5 feature | +7 / −0 |
| 2026-08-26 | [0502e7e5d36](https://github.com/ElDominio/rusefi/commit/0502e7e5d3643e3c75076a1d363b4d4dd3908141) — Add setLaunchRpm(rpm) Lua hook | +5 / −0 |
