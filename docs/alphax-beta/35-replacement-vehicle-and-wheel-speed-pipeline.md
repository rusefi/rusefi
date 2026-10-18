# 35 — Replacement vehicle and wheel-speed pipeline

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Upstream progress — partial

Rechecked master at [`3dd620b8d5b`](https://github.com/rusefi/rusefi/commit/3dd620b8d5b05839ddc958bb4104bba4bd63558e). Commit [`0208ad604b6`](https://github.com/rusefi/rusefi/commit/0208ad604b62c7395e8cd88f812037e58195aecf) supplies a standalone physical output-shaft-speed input, frequency-to-RPM conversion, filtering, timeout/reconfiguration handling and a registry slot available to a Lua/CAN provider when no physical pin is assigned. This supports the completed [gear/slip report 36](DONE-36-gear-detection-and-transmission-slip.md).

This report remains open: master retains legacy VehicleSpeed and vendor CAN VSS, and has not adopted the fork's OSS/front/rear main-speed selection, axle-speed inputs, percentage-per-second plausibility filtering or wheel-slip source pipeline. The historical analysis and LOC tables below still refer to the original master comparison.

## Executive summary

Master's legacy physical VSS and built-in vendor CAN VSS paths are replaced. Main vehicle speed selects output-shaft speed, front axle or rear axle; each source can be physical or CAN/Lua-fed. Output-shaft frequency becomes RPM, then uses final-drive ratio and wheel revolutions/km for vehicle speed. A shared percentage-per-second plausibility setting governs separate physical-source filter instances. Wheel slip can be derived from a selected pair of speed sources.

## ECU logic and behavior

1. Acquire output-shaft, front-axle and rear-axle speed from configured physical or CAN/Lua-fed sources.
2. Convert OSS frequency to shaft RPM, then use final-drive ratio and wheel revolutions/km to derive road speed.
3. Apply separate physical-source plausibility-filter state with a common percentage-per-second setting.
4. Route the selected source to the normal VehicleSpeed identity.
5. Derive tire slip from the configured pair of speed sources.
6. Remove the old physical converter and built-in vendor CAN VSS path.

**Illustrative behavior:** A Lua CAN decoder that formerly created VehicleSpeed must feed the chosen replacement channel, such as WheelSpeedFront, and select that channel as the main speed source.

## Calibration, dependencies and review limits

This is a migration, not only added sensors: `can_vss.cpp`, the old converter, legacy tune fields and hardware VSS tests are deleted. Lua examples are adapted to the replacement sensor path. The old vendor choices have no equivalent built-in decoder in this change; CAN/Lua users must provide the new channels.

Verify source selection, tire/final-drive calibration, low-speed filtering and stale CAN/Lua values. Deleted vendor decoders and hardware VSS tests are migration work, not retained compatibility.

Existing branch test evidence: [unit_tests/tests/sensor/test_output_shaft_speed_converter.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/sensor/test_output_shaft_speed_converter.cpp), [unit_tests/tests/sensor/test_wheel_speed_sensors.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/sensor/test_wheel_speed_sensors.cpp).

These test files change by +245 / −0 against the common ancestor; this is separate from firmware-source LOC.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 599 changed lines — 321 additions and 278 deletions across 13 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+321 / −288**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 6 files are new since the common ancestor (+264 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/can/can_vss.cpp](https://github.com/ElDominio/rusefi/blob/9154d40f573276b426b6f87ae6475cb9ea1a888c/firmware/controllers/can/can_vss.cpp) | +0 / −225 | +0 / −229 |
| [firmware/controllers/can/can_vss.h](https://github.com/ElDominio/rusefi/blob/9154d40f573276b426b6f87ae6475cb9ea1a888c/firmware/controllers/can/can_vss.h) | +0 / −15 | +0 / −15 |
| [firmware/controllers/sensors/converters/axle_speed_converter.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/converters/axle_speed_converter.h) | +40 / −0 | +40 / −0 |
| [firmware/controllers/sensors/converters/output_shaft_speed_converter.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/converters/output_shaft_speed_converter.h) | +17 / −0 | +17 / −0 |
| [firmware/controllers/sensors/converters/vehicle_speed_converter.h](https://github.com/ElDominio/rusefi/blob/9154d40f573276b426b6f87ae6475cb9ea1a888c/firmware/controllers/sensors/converters/vehicle_speed_converter.h) | +0 / −23 | +0 / −23 |
| [firmware/controllers/sensors/converters/wheel_speed_plausibility_filter.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/converters/wheel_speed_plausibility_filter.h) | +56 / −0 | +56 / −0 |
| [firmware/controllers/sensors/frequency_sensor.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/frequency_sensor.cpp) | +5 / −1 | +5 / −7 |
| [firmware/controllers/sensors/frequency_sensor.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/frequency_sensor.h) | +2 / −0 | +2 / −0 |
| [firmware/controllers/sensors/wheel_slip_ratio_source.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/wheel_slip_ratio_source.cpp) | +79 / −0 | +79 / −0 |
| [firmware/controllers/sensors/wheel_slip_ratio_source.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/wheel_slip_ratio_source.h) | +9 / −0 | +9 / −0 |
| [firmware/init/sensor/init_aux_speed_sensor.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_aux_speed_sensor.cpp) | +4 / −1 | +4 / −1 |
| [firmware/init/sensor/init_vehicle_speed_sensor.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_vehicle_speed_sensor.cpp) | +46 / −13 | +46 / −13 |
| [firmware/init/sensor/init_wheel_speed_sensors.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_wheel_speed_sensors.cpp) | +63 / −0 | +63 / −0 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-07-10 | [66783d4b26e](https://github.com/ElDominio/rusefi/commit/66783d4b26e409e65e67493ed31f2f6a9fbcd906) — Add plausibility filter to VSS reading | +51 / −1 |
| 2026-08-17 | [b90fe2fb3ee](https://github.com/ElDominio/rusefi/commit/b90fe2fb3ee655f2b61ed221f34a5bfec42acb49) — Wheel Speed Sensors + Gear Setup: axle/OSS speed replaces legacy pin/CAN VSS | +310 / −364 |
| 2026-09-11 | [4b1a95ac583](https://github.com/ElDominio/rusefi/commit/4b1a95ac583369c44692ad231586c29c7a2b1139) — Wheel Speed Sensors: OSS->VehicleSpeed shares driveWheelRevPerKm/finalGearRatio; speedometer bench test | +7 / −3 |
