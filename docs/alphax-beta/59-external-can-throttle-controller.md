# 59 — External CAN throttle controller

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch adds an opt-in interface to a separate CH32V203 ETB controller. rusEFI sends target, gains, limits, bias curve and calibration over private extended CAN IDs. Raw TPS/pedal telemetry feeds virtual channels through normal calibration/redundancy processing; duty/PID/feed-forward/status telemetry populates existing diagnostics. Local throttle PID/motor driving is bypassed for normal remote operation, while bench, calibration and autotune gain dedicated remote paths.

## ECU logic and behavior

1. Keep target computation on rusEFI but transmit normal throttle requests to the external controller using private extended CAN identifiers.
2. Send gains, limits, bias/calibration and remote bench/autotune commands through the added protocol.
3. Register raw remote TPS/pedal values as virtual channels using normal calibration and redundancy processing.
4. Adapt freshness timeout to telemetry cadence; the virtual TPS path uses 300 ms rather than the physical ADC's 10 ms.
5. Populate duty/PID/feed-forward/status diagnostics from received telemetry.
6. On local faults, pause or invalid targets, stop normal command transmission; external timeout behavior must disable the remote actuator.

**Illustrative behavior:** A remote throttle can continue doing its local motor control while rusEFI supplies target updates; loss of those updates must be handled by the external firmware's timeout.

## Calibration, dependencies and review limits

The ECU still computes the combined target and stops sending normal commands on local faults, pause or invalid setpoints; remote disable then depends on the external controller's command timeout. The external firmware is outside this repository and was not verified here. Receive listeners do not provide independently addressed per-throttle telemetry; current code uses one target stream, so this is not complete independent dual-ETB support. `CanDcMotor::isFault()` remains a stub. Comments disagree about external autotune implementation status, so only the rusEFI-side transport can be established from this tree.

This should be isolated as protocol definitions, virtual sensor ingestion, command/telemetry, calibration/bench operations and fault handling, with end-to-end verification before adoption. `EFI_EXTERNAL_CAN_ETB` is off in the unit-test build.

Verify bus loss, stale sensor values, remote disable and fault reporting end to end. CanDcMotor::isFault is still a stub, independent dual-ETB addressing is incomplete, and the external firmware is outside this review.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 1,487 changed lines — 1,442 additions and 45 deletions across 9 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+1,450 / −184**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 3 files are new since the common ancestor (+849 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/actuators/electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp) | +199 / −17 | +199 / −33 |
| [firmware/controllers/actuators/electronic_throttle_impl.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle_impl.h) | +151 / −0 | +151 / −0 |
| [firmware/controllers/bench_test.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/bench_test.cpp) | +15 / −0 | +51 / −86 |
| [firmware/controllers/can/can_etb.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/can/can_etb.h) | +267 / −0 | +267 / −0 |
| [firmware/controllers/can/can_etb_remote.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/can/can_etb_remote.cpp) | +288 / −0 | +288 / −0 |
| [firmware/controllers/can/can_rx.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/can/can_rx.cpp) | +0 / −5 | +23 / −48 |
| [firmware/controllers/can/can_tx.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/can/can_tx.cpp) | +72 / −6 | +21 / −0 |
| [firmware/init/sensor/init_etb_can.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_etb_can.cpp) | +294 / −0 | +294 / −0 |
| [firmware/init/sensor/init_tps.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_tps.cpp) | +156 / −17 | +156 / −17 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-09-02 | [66ca87b4424](https://github.com/ElDominio/rusefi/commit/66ca87b4424dc56f7a8424126e213b5f2736bca0) — Add external CAN ETB controller support | +805 / −3 |
| 2026-09-02 | [c43f3885ecb](https://github.com/ElDominio/rusefi/commit/c43f3885ecb4ef8bb9821518f592de0094ee009f) — Gate external CAN ETB controller behind EFI_EXTERNAL_CAN_ETB, opt-in for fw-custom-paralela-master | +24 / −11 |
| 2026-09-06 | [f4dec5f1f96](https://github.com/ElDominio/rusefi/commit/f4dec5f1f96b4dee90a34cc850722f39d6ed2336) — External CAN ETB: virtual TPS1/TPSB channels, feedforward telemetry, status gauge | +465 / −176 |
| 2026-09-07 | [1b697bed7d6](https://github.com/ElDominio/rusefi/commit/1b697bed7d6d0aaebb61b52793efd7d50b50f49e) — ETB: route driverIntent to the pedal under external CAN ETB mode | +7 / −2 |
| 2026-09-23 | [694a9faa7df](https://github.com/ElDominio/rusefi/commit/694a9faa7df0cfa54c2b6f9ba6ee4c7fe91e7753) — External CAN ETB: wire board pTerm onto ETB_FEEDFORWARD's spare bytes | +24 / −4 |

Supporting configuration and source: [docs/external-etb-can-followups.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/docs/external-etb-can-followups.md).
