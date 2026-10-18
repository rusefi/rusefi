# AlphaX beta changes compared with master

This index focuses on firmware changes that affect ECU inputs, decisions, outputs, diagnostics or persistent operation in the [ElDominio alphax-beta fork](https://github.com/ElDominio/rusefi/tree/alphax-beta). Operational changes have separate detailed Markdown reports, ordered from small defaults and diagnostics through control algorithms to transmission, ADC acquisition and crank synchronization. Host tooling, build/schema work and supporting firmware architecture are marked *INFRASTRUCTURE*.

## Master progress

Reviewed upstream `origin/master` at [`41e3c63a9865141686bbfa7940defe0e86a4cb11`](https://github.com/rusefi/rusefi/commit/41e3c63a9865141686bbfa7940defe0e86a4cb11), committed 2026-10-18 UTC, after fetching. It contains 22 commits beyond the original comparison revision, including five since the previous review at `3dd620b8d5b`: TPS smoothing plus four automatic configuration/default-tune regeneration commits (*INFRASTRUCTURE*). The five prior completions remain present. **Six reports are now prefixed `DONE-`** because master implements their operational scope:

| Report | Upstream completion evidence |
| --- | --- |
| [DONE — 06b: Eight-bin warmup defaults](alphax-beta/DONE-06b-eight-bin-warmup-enrichment-defaults.md) | [`78759669096`](https://github.com/rusefi/rusefi/commit/787596690963ee455ed4450485a92b571ed71237); patch-equivalent to the fork fix. |
| [DONE — 06e: Speedometer bench command](alphax-beta/DONE-06e-speedometer-output-bench-command.md) | [`fc5a5a1e381`](https://github.com/rusefi/rusefi/commit/fc5a5a1e3817d46add4107eb82416ead8b27c84a) plus [`de4133f2355`](https://github.com/rusefi/rusefi/commit/de4133f2355036c7bf67f5f775afe604f6621a96); firmware, tests and tuning UI. |
| [DONE — 13a: TPS smoothing](alphax-beta/DONE-13a-tps-smoothing.md) | [`edaaf10bab6`](https://github.com/rusefi/rusefi/commit/edaaf10bab64e4ab9b60f60684995a8246207c05); independent TPS filters, fast-transient reset, invalidity handling, tuning controls and tests. |
| [DONE — 31: Fan pressure mode and PWM semantics](alphax-beta/DONE-31-cooling-fan-pressure-mode-and-pwm-semantics.md) | [`3da3feb4de3`](https://github.com/rusefi/rusefi/commit/3da3feb4de30e61d875c4d8a7d3b0b0d45b4be91) plus [`060dc248caa`](https://github.com/rusefi/rusefi/commit/060dc248caa047b059d7cd86d041ae913fd9e2fd); operational behavior and old A/C-toggle migration. |
| [DONE — 36: Gear detection and transmission slip](alphax-beta/DONE-36-gear-detection-and-transmission-slip.md) | [`0208ad604b6`](https://github.com/rusefi/rusefi/commit/0208ad604b62c7395e8cd88f812037e58195aecf), following reproduction [`e529a799d7a`](https://github.com/rusefi/rusefi/commit/e529a799d7a1a9dec9337594aaeab651edbf87bd); clutch handling, OSS/RPM selection, slip calculation, telemetry and tests. |
| [DONE — 55: Rolling launch](alphax-beta/DONE-55-rolling-launch.md) | [`3dd620b8d5b`](https://github.com/rusefi/rusefi/commit/3dd620b8d5b05839ddc958bb4104bba4bd63558e); captured-RPM spark limiting, hardware/Lua activation, fuel enrichment, timing ramp-out, tuning controls and tests. |

`DONE` means the described operational feature is present upstream; it does not require identical files, AlphaX page placement or every fork-only diagnostic. Each completed report records those differences. Reviewing the intervening source changes found one newly completed feature, TPS smoothing. Former report 13 is split into completed [13a](alphax-beta/DONE-13a-tps-smoothing.md) and open [13b](alphax-beta/13b-tps-pps-discrepancy-configuration.md), because master has not adopted the wider discrepancy limit or changed storage scale. There are now **63 reports: six DONE and 57 open**.

Report [35](alphax-beta/35-replacement-vehicle-and-wheel-speed-pipeline.md) has partial progress: standalone OSS acquisition is implemented, while the broader main-speed/axle/slip pipeline replacement remains open. Slip telemetry alone does not complete the TCU pressure-trim work in report 58. Shared status/default-file edits likewise do not complete ignition-voltage fallback (06a) or fuel economy (14). Preserving external TPS providers during smoothing initialization does not implement the CAN ETB protocol in report 59.

Rolling launch uses the existing shared limiter and its own timing ramp-out. Its integration edits do not complete standing-launch controls (21), coordinated traction control (37), custom-page infrastructure (41) or the separate launch power ramp (54).

This is a source/history review; tests were inspected, not rerun. The comparison revisions, LOC tables and complete inventories below remain the original historical snapshot, so their counts are not a remaining-work estimate.

## Comparison scope

Reviewed on 2026-10-17, after fetching both remotes. **Master means upstream `origin/master`**, rather than the older `master` branch in the fork.

| Reference | Pinned revision | Commit date |
| --- | --- | --- |
| AlphaX beta | `ebb877615ee157feaadc9a186d31f193f488c643` | 2026-10-12 |
| Upstream master | `c12cf14983152753a94001f4a101930fb2857fdc` | 2026-10-17 |
| Common ancestor | `9154d40f573276b426b6f87ae6475cb9ea1a888c` | 2026-09-03 |
| Fork master | `0a4b38b9250a981261dc59658b56794dee0e1567` | 2026-10-12 |

The fork's master is 93 commits behind upstream master and has the same common ancestor with AlphaX beta. Consequently the branch-side change inventory is identical with either master reference; the assessment of what upstream already provides uses the newer upstream revision. The original investigation used these immutable revisions; this documentation expansion retains that snapshot rather than silently switching to newer branch tips.

There are **302 branch-side commits and 769 upstream-side commits** since the common ancestor. The branch-side total comprises 294 non-merge commits and 8 merges. `git cherry` finds 10 patch-equivalent non-merge commits in the compared upstream history and 284 without an equivalent there. Those 284 are **not 284 new features**: earlier upstream imports, rewritten commits, documentation, follow-up fixes, and reverted experiments also appear in that set. An exact-subject search finds 108 branch commits whose subject also appears in upstream history; a subject match is a history clue, not proof of patch equivalence.

The net common-ancestor-to-branch diff contains **579 file entries, 138,989 inserted lines, and 4,127 deleted lines**. A direct comparison of the two tips contains 1,517 file entries, because it also includes later upstream changes missing from the fork. This report uses the former to identify branch work, then checks the latter to avoid treating already-upstream behavior as unique. It covers final retained behavior; superseded intermediate designs are identified where they would otherwise mislead an extraction effort.

Two companion inventories make the scope auditable:

- [Every changed file](alphax-beta-vs-master-files.tsv): status, old/new path, line counts, artifact type, corresponding report sections, and comparison with the upstream blob.
- [Every branch-side commit](alphax-beta-vs-master-commits.tsv): full hash, date, merge status, `git cherry` result, and exact-subject upstream matches.

Source links below point to immutable revisions. Test references mean coverage exists in the branch, **not that it passed in this review**. This was a source and history investigation; no branch firmware, simulator, Java build, or hardware test was run.

## Reading the detailed reports

There are **63 separate reports**. The original section numbers are retained for the complete file inventory; `06a–06e`, `10a–10e`, `13a–13b` and `61a–61d` split previously bundled changes. Inventory section 13 covers both TPS reports. Each report includes an executive summary, control flow, an example, calibration/dependency notes, exact source-file diff counts and clickable fork commit links.

**LOC convention:** numbers below are added/deleted physical lines in the maintained firmware C/C++ files listed in each report, from the common ancestor to the fork tip. They include comments and blank lines. Shared control files serve several features: these are overlapping file-scope measurements, not additive feature-exclusive estimates. Each report also gives current-master comparison counts and per-commit counts so narrow fixes can be inspected separately. Generated artifacts, schema, tests and host tools are excluded from these firmware totals.

## ECU operation changes — simple to complex

### Small defaults and ECU diagnostics

| ID | Detailed report | Added / deleted source lines |
| --- | --- | ---: |
| 06a | [Ignition-voltage telemetry fallback](alphax-beta/06a-ignition-voltage-telemetry-fallback.md) | +77 / −5 |
| 06b | [DONE — Eight-bin warmup enrichment defaults](alphax-beta/DONE-06b-eight-bin-warmup-enrichment-defaults.md) | +36 / −0 |
| 06c | [Lua limiter-adder axis defaults](alphax-beta/06c-lua-limiter-adder-axis-defaults.md) | +362 / −0 |
| 06d | [*INFRASTRUCTURE* — Knock spectrogram feature guard](alphax-beta/06d-knock-spectrogram-feature-guard.md) | +11 / −4 |
| 06e | [DONE — Speedometer output bench command](alphax-beta/DONE-06e-speedometer-output-bench-command.md) | +47 / −2 |

### Board wiring and default behavior

| ID | Detailed report | Added / deleted source lines |
| --- | --- | ---: |
| 10a | [alphax-s197-v2: ECU wiring and defaults](alphax-beta/10a-alphax-s197-v2-ecu-wiring-and-defaults.md) | +231 / −0 |
| 10b | [alphax-s550-pnp: ECU wiring and defaults](alphax-beta/10b-alphax-s550-pnp-ecu-wiring-and-defaults.md) | +286 / −0 |
| 10c | [fw-custom-paralela-master: ECU wiring and defaults](alphax-beta/10c-fw-custom-paralela-master-ecu-wiring-and-defaults.md) | +99 / −0 |
| 10d | [protorico-econoline: ECU wiring and defaults](alphax-beta/10d-protorico-econoline-ecu-wiring-and-defaults.md) | +248 / −0 |
| 10e | [protorico-grummann: ECU wiring and defaults](alphax-beta/10e-protorico-grummann-ecu-wiring-and-defaults.md) | +210 / −0 |

### Sensors, fuel, ignition and startup

| ID | Detailed report | Added / deleted source lines |
| --- | --- | ---: |
| 13a | [DONE — TPS smoothing](alphax-beta/DONE-13a-tps-smoothing.md) | +253 / −24 |
| 13b | [TPS/PPS discrepancy configuration](alphax-beta/13b-tps-pps-discrepancy-configuration.md) | +238 / −24 |
| 14 | [Instantaneous fuel economy](alphax-beta/14-instantaneous-fuel-economy.md) | +160 / −5 |
| 15 | [Manual fuel trim by bank](alphax-beta/15-manual-fuel-trim-by-bank.md) | +68 / −1 |
| 16 | [Injector small-pulse percentage curve](alphax-beta/16-injector-small-pulse-percentage-curve.md) | +29 / −2 |
| 17 | [Manual injector pressure correction](alphax-beta/17-manual-injector-pressure-correction.md) | +29 / −2 |
| 18 | [VVT-dependent fuel and ignition corrections](alphax-beta/18-vvt-dependent-fuel-and-ignition-corrections.md) | +109 / −13 |
| 19 | [Burst knock retard](alphax-beta/19-burst-knock-retard.md) | +153 / −13 |
| 20 | [Time-based WOT enrichment](alphax-beta/20-time-based-wot-enrichment.md) | +110 / −0 |
| 21 | [Launch switch semantics and Lua controls](alphax-beta/21-launch-switch-semantics-and-lua-controls.md) | +47 / −2 |
| 22 | [Priming after trigger teeth](alphax-beta/22-priming-after-trigger-teeth.md) | +259 / −10 |
| 23 | [Cranking without ECU spark](alphax-beta/23-cranking-without-ecu-spark.md) | +74 / −2 |
| 24 | [Delayed start authorization](alphax-beta/24-delayed-start-authorization.md) | +95 / −17 |
| 25 | [Alternator feed-forward and output telemetry](alphax-beta/25-alternator-feed-forward-and-output-telemetry.md) | +69 / −12 |
| 26 | [Boost target calculation in open loop](alphax-beta/26-boost-target-calculation-in-open-loop.md) | +36 / −10 |
| 27 | [Predictive MAP event lifetime](alphax-beta/27-predictive-map-event-lifetime.md) | +14 / −10 |

### Actuator, speed and thermal control

| ID | Detailed report | Added / deleted source lines |
| --- | --- | ---: |
| 28 | [Intake manifold runner control](alphax-beta/28-intake-manifold-runner-control.md) | +215 / −0 |
| 29 | [Exhaust cutout control](alphax-beta/29-exhaust-cutout-control.md) | +480 / −0 |
| 30 | [Clutch delay valve](alphax-beta/30-clutch-delay-valve.md) | +102 / −0 |
| 31 | [DONE — Cooling fan pressure mode and PWM semantics](alphax-beta/DONE-31-cooling-fan-pressure-mode-and-pwm-semantics.md) | +242 / −73 |
| 32 | [Dual and PWM fuel pumps](alphax-beta/32-dual-and-pwm-fuel-pumps.md) | +236 / −9 |
| 33 | [Idle air and target rework](alphax-beta/33-idle-air-and-target-rework.md) | +486 / −53 |
| 34 | [Soft RPM range and ETB RPM PID](alphax-beta/34-soft-rpm-range-and-etb-rpm-pid.md) | +307 / −19 |
| 35 | [Replacement vehicle and wheel-speed pipeline](alphax-beta/35-replacement-vehicle-and-wheel-speed-pipeline.md) | +321 / −278 |
| 36 | [DONE — Gear detection and transmission slip](alphax-beta/DONE-36-gear-detection-and-transmission-slip.md) | +105 / −7 |
| 37 | [Coordinated traction control](alphax-beta/37-coordinated-traction-control.md) | +463 / −31 |
| 38 | [Cylinder-head sensor and thermal estimates](alphax-beta/38-cylinder-head-sensor-and-thermal-estimates.md) | +452 / −11 |
| 39 | [Weighted oil-life monitor](alphax-beta/39-weighted-oil-life-monitor.md) | +334 / −2 |

### Operating states, modes and coordinated control

| ID | Detailed report | Added / deleted source lines |
| --- | --- | ---: |
| 42 | [Engine state machine and shift recognition](alphax-beta/42-engine-state-machine-and-shift-recognition.md) | +1,299 / −0 |
| 43 | [Sport mode and pedal shaping](alphax-beta/43-sport-mode-and-pedal-shaping.md) | +1,717 / −19 |
| 44 | [Eco operating mode](alphax-beta/44-eco-operating-mode.md) | +1,532 / −47 |
| 45 | [Quick warmup](alphax-beta/45-quick-warmup.md) | +1,420 / −45 |
| 46 | [Ghost Cam idle mode](alphax-beta/46-ghost-cam-idle-mode.md) | +1,727 / −67 |
| 47 | [Pops and bangs](alphax-beta/47-pops-and-bangs.md) | +1,492 / −83 |
| 48 | [Points-based check-engine escalation](alphax-beta/48-points-based-check-engine-escalation.md) | +234 / −21 |
| 49 | [Misfire configuration and activation changes](alphax-beta/49-misfire-configuration-and-activation-changes.md) | +31 / −37 |
| 50 | [Configurable latched limp mode](alphax-beta/50-configurable-latched-limp-mode.md) | +1,466 / −42 |
| 51 | [State-aware DFCO](alphax-beta/51-state-aware-dfco.md) | +1,156 / −39 |
| 52 | [Downshift rev matching](alphax-beta/52-downshift-rev-matching.md) | +549 / −17 |
| 53 | [Upshift RPM hold](alphax-beta/53-upshift-rpm-hold.md) | +526 / −17 |
| 54 | [Launch power ramp](alphax-beta/54-launch-power-ramp.md) | +139 / −13 |
| 55 | [DONE — Rolling launch](alphax-beta/DONE-55-rolling-launch.md) | +282 / −14 |
| 56 | [Advanced VVT control](alphax-beta/56-advanced-vvt-control.md) | +124 / −6 |
| 57 | [First-order RPM estimation](alphax-beta/57-first-order-rpm-estimation.md) | +79 / −16 |

### Transmission, acquisition and synchronization

| ID | Detailed report | Added / deleted source lines |
| --- | --- | ---: |
| 58 | [Automatic transmission controls](alphax-beta/58-automatic-transmission-controls.md) | +343 / −373 |
| 59 | [External CAN throttle controller](alphax-beta/59-external-can-throttle-controller.md) | +1,442 / −45 |
| 60 | [Expanded ADC acquisition and mux control](alphax-beta/60-expanded-adc-acquisition-and-mux-control.md) | +326 / −37 |
| 61a | [Mitsubishi 6G72 provisional crank/cam synchronization](alphax-beta/61a-mitsubishi-6g72-provisional-crank-cam-synchronization.md) | +603 / −10 |
| 61b | [Mitsubishi 6G75 experimental tooth-count crank synchronization](alphax-beta/61b-mitsubishi-6g75-experimental-tooth-count-crank-synchronization.md) | +204 / −4 |
| 61c | [Mitsubishi 6G75 cam-edge-count phase resolution](alphax-beta/61c-mitsubishi-6g75-cam-edge-count-phase-resolution.md) | +351 / −4 |
| 61d | [Last synchronization-loss reason telemetry](alphax-beta/61d-last-synchronization-loss-reason-telemetry.md) | +441 / −8 |

## Infrastructure and already-upstream work

These changes support delivery or firmware architecture. They are retained for traceability, but are not presented as additional AlphaX engine-control features. Module lifecycle and calibration persistence have their own detailed reports because their failure directly affects ECU operation.

### 01 *INFRASTRUCTURE* — Documentation and repository housekeeping

Master does not contain this branch's feature catalogue, CAN ETB feasibility/follow-up notes, CAN tuning overview, misfire/CEL descriptions, crank-angle research, MSQ-converter research, or proposed LLM console-control design in these forms. The branch adds those documents, a Speeduino decoder reference copy, a large historical session report, and a cherry-pick ledger. The LLM-control and MSQ documents describe proposals/research; they do not establish implemented features in this branch.

It also changes `CLAUDE.md`, ignore rules for build output, and checks in a board-local `.claude/settings.local.json`. These are separable documentation/environment changes. The branch's historical report and guidance contain stale intermediate statements and should be read alongside final source. Sources: [FEATURE_FLAGS.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/FEATURE_FLAGS.md), [CAN_TUNING.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/CAN_TUNING.md), [llm-console-control.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/docs/llm-console-control.md), [msq-converter-research.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/docs/msq-converter-research.md), [report.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/docs/report.md), [master-cherry-picks.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/master-cherry-picks.md).

### 02 *INFRASTRUCTURE* — Firmware build GUI

The branch adds a standalone Tkinter build frontend, `build_gui.py`, above master's command-line build workflow. It selects repository/board/build options, runs builds, presents output, remembers local preferences, and copies firmware and bundle outputs to a chosen destination. Follow-ups ensure the full `rusefi.bin` is included with extracted bundles.

This is independently reviewable host tooling, with dependencies on the repository's existing build scripts and artifact conventions. No dedicated GUI tests are added. Source: [build_gui.py](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/build_gui.py).

### 03 *INFRASTRUCTURE* — Board configuration GUI

`board_config_gui.py` provides a Tkinter editor for board makefile flags and metadata, including batch board selection and tri-state feature choices. Master has no corresponding script. Its material behavior is writing build configuration, so extraction should preserve the distinction between inherited defaults and explicit `TRUE`/`FALSE` overrides.

This tool can be separated from the firmware features it exposes, but its hardcoded feature list needs to match the destination tree. Source: [board_config_gui.py](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/board_config_gui.py).

### 04 *INFRASTRUCTURE* — CI selection GUI and reduced board matrix

`board_ci_gui.py` toggles board builds by renaming enabled `meta-info*.env` files to disabled metadata names, using `git mv` where possible. Separately, the branch applies that policy to many board variants, including non-AlphaX boards and some AlphaX variants. It also changes board metadata flags used by bundle generation.

The GUI and the actual board-disable policy are two separable changes. A smaller passing matrix would not establish that disabled boards still build. Exact renames and metadata edits are retained in the file inventory. Sources: [board_ci_gui.py](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/board_ci_gui.py), [meta-info-proteus_f7.disabled_env](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/proteus/meta-info-proteus_f7.disabled_env).

### 05 *INFRASTRUCTURE* — OpenOCD and DFU GUI

The new `openocd_gui.py` wraps OpenOCD server control, programming, dumping, unlocking, DFU image selection/programming, serial-port discovery, and reboot-to-DFU commands. Master has the underlying programming workflows but no equivalent root-level GUI.

This is a separate desktop tool, not a firmware protocol change. Its programming/unlock actions need testing with the intended programmer and MCU; adding the script alone does not validate those actions. Source: [openocd_gui.py](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/openocd_gui.py).

### 07 *INFRASTRUCTURE* — Configuration generator and TunerStudio presentation

The branch introduces a `graph_split` pseudo-field that starts a new generated graph without consuming configuration bytes. It also adds a digit-precision template argument to blend tables, permits unresolved digit placeholders during template parsing, and corrects how event-input pins are added to switch-input choices. Target-AFR blend tables use two display decimals while other blend tables retain one.

These are separate generator/presentation changes. Page-6 registration is a further dependency described in section 41. `FragmentDialogConsumerTest` gains graph-split coverage; that does not cover every generator alteration. Sources: [FragmentDialogConsumer.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_tools/configuration_definition_base/src/main/java/com/rusefi/output/FragmentDialogConsumer.java), [ConfigFieldImpl.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_tools/configuration_definition_base/src/main/java/com/rusefi/ConfigFieldImpl.java), [PinoutLogic.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_tools/configuration_definition/src/main/java/com/rusefi/pinout/PinoutLogic.java), [FragmentDialogConsumerTest.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_tools/configuration_definition/src/test/java/com/rusefi/test/FragmentDialogConsumerTest.java).

### 08 *INFRASTRUCTURE* — Explicit unsupported-ECU connection and flashing overrides

Master's compatibility checks reject an unsupported ECU/bundle pairing. The branch adds a session-local forced-port registry and a “connect anyway” action that retries blocked ports using the ECU's own reported definition. A separate firmware-update confirmation can permit programming despite the board-target mismatch.

Connection override and flash override are distinct behaviors and should remain separately reviewable. The latter changes a programming guard, not just UI wording. There is no dedicated new override test file in the branch-side diff. Sources: [ForcedEcuOverride.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_console/shared_io/src/main/java/com/rusefi/core/io/ForcedEcuOverride.java), [UnsupportedEcuCardHost.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_console/ui/src/main/java/com/rusefi/UnsupportedEcuCardHost.java), [BinaryProtocol.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_console/io/src/main/java/com/rusefi/binaryprotocol/BinaryProtocol.java), [BootloaderHelper.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_console/ui/src/main/java/com/rusefi/io/BootloaderHelper.java).

### 09 *INFRASTRUCTURE* — Upstream imports and dependency revisions

The history contains imported work on Ford TFI dwell, flex-fuel cranking/transients, Lua page separation, USB/MSD, Renix triggers, console VE generation/logging, and other upstream fixes. Their presence in the commit list does not make those features unique to AlphaX beta. Twenty-one branch-changed file entries now have exactly the same blob as current upstream; the inventory identifies them explicitly. Examples include CAN startup, H-bridge PWM, hardware-PWM support, and the STM32F4 RTC implementation.

The remaining CAN queue implementation is substantially shared with current master. Current master additionally rejects submissions quietly while a CAN device is intentionally removed; the branch lacks that later handling. Current master also has newer ISO-TP receive/flow-control fixes and configurable TS-over-CAN bus selection; those are upstream gaps, not branch additions. Sources: [can_msg_tx.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/hw_layer/drivers/can/can_msg_tx.cpp), [master: can_msg_tx.cpp](https://github.com/rusefi/rusefi/blob/c12cf14983152753a94001f4a101930fb2857fdc/firmware/hw_layer/drivers/can/can_msg_tx.cpp), [isotp.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/can/isotp/isotp.cpp), [master: isotp.cpp](https://github.com/rusefi/rusefi/blob/c12cf14983152753a94001f4a101930fb2857fdc/firmware/controllers/can/isotp/isotp.cpp).

The two changed submodule pointers move **backward** relative to the common ancestor. `firmware/libfirmware` points to `8052aed45e6`, nine commits behind the common-ancestor pin and fourteen behind current master. `unit_tests/googletest` points to `35b75a2cba6`, 136 commits behind the pin shared by the common ancestor and current master. Treat these as dependency differences requiring reconciliation, not feature implementations.

### 11 *INFRASTRUCTURE* — Existing board feature selections and memory options

AlphaX two/four/eight-channel, GDI, Gold and Silver variants receive additional feature defines and UI flags. Hellen154 Hyundai variants also enable custom features. Several F7 builds select the larger internal-flash layout to retain Lua/custom pages across reboot. AlphaX Silver disables embedded uncompressed INI storage and selects STM32F429 memory support to make additional SRAM available.

Family defaults generally make custom features opt-in on F4 and enabled on F7, but there are exceptions: wheel-speed support defaults on broadly, and external CAN ETB is explicitly opt-in. Board overrides and standalone simulator/test headers decide the effective build; `FEATURE_FLAGS.md` alone is not authoritative. Sources: [efifeatures.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/stm32f4ems/efifeatures.h), [efifeatures.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/stm32f7ems/efifeatures.h), [board.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/hellen/alphax-silver/board.mk), [board.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/hellen/hellen154hyundai_f7/board.mk), [efifeatures.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/simulator/simulator/efifeatures.h), [efifeatures.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/efifeatures.h).

### 12 *INFRASTRUCTURE* — Packaging and embedded INI generation

The branch uses the actual branch name in local bundle naming, copies more artifacts instead of symlinking them, removes stale SREC/ZIP outputs before replacement, and includes firmware BIN output in bundle staging. Its simulator bundle targets explicitly depend on the INI RAM disk; the RAM-disk sentinel also depends on the generator script. Windows CI gets an explicit image-generation step.

For a board declaring `EFI_EMBED_INI_MSD=FALSE`, the generator writes a minimal `ramdisk_image` symbol instead of building the uncompressed image. Both image scripts also lose their original skip block. Review these together with current master's newer build rules: some directory-race fixes in the history are already upstream. Sources: [bundle.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/bundle.mk), [gen_image_board.sh](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/bin/gen_image_board.sh), [rusefi_config.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/rusefi_config.mk), [build-firmware-windows.yaml](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/.github/workflows/build-firmware-windows.yaml).

### 40 *INFRASTRUCTURE* — Engine module storage and F4 RAM placement

[Detailed firmware report](alphax-beta/40-engine-module-storage-and-f4-ram-placement.md) — +207 / −28 in the measured source scope.

Master keeps its modules in the engine's module list. The branch splits core and AlphaX modules, adds combined iteration/aggregation helpers and routes lifecycle callbacks across both lists. It also moves the production F4 engine allocation out of the fixed CCM section.

This is a system-wide memory/lifecycle change, not merely a board flag. Module lookup, callback ordering, defaults, configuration notifications and shutdown aggregation must remain consistent. Existing feature tests exercise parts of the framework, but do not establish per-board RAM fit. Sources: [engine.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine.h), [engine_module.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/core/engine_module.h), [engine_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/engine_controller.cpp), [engine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine.cpp).

### 41 *INFRASTRUCTURE* — Custom page 6 and persistent layout

[Detailed firmware report](alphax-beta/41-custom-page-6-and-persistent-layout.md) — +521 / −9 in the measured source scope.

Master already has separate extra pages, including Lua page 5. The branch adds page 6 for custom controls, its default initializer and storage container, generator registration, TS page routing, SD record support and burn/load integration. The custom container's final data version is 23; the main flash-data version is 260912.

Internal-flash placement derives page 6 from the end of the board-sized Lua container, aligned to 32 bytes. The oil-life record follows it, with overlap/sector-size assertions. Extra-page burns on internal-flash-only builds request a deferred full configuration write. Page growth, field movement, removed fields and renamed settings mean raw configuration images are not interchangeable with master; the report does not establish a complete automatic tune migration. Sources: [config_page_6.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/integration/config_page_6.txt), [custom_page.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/custom_page.cpp), [extra_flash_pages.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/extra_flash_pages.cpp), [storage.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/storage.cpp), [TSProjectConsumer.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_tools/configuration_definition/src/main/java/com/rusefi/output/TSProjectConsumer.java).

### 62 *INFRASTRUCTURE* — Shared schema and generated delivery surface

Across the features above, the branch changes enums and sensor IDs, live-data layouts, log fields, gauges, menus, console enum mirrors, default tunes, module/build lists and generated firmware/INI artifacts. New per-board generated configuration headers and INIs, plus the compressed embedded INI image, account for much of the line-count growth. These are feature delivery dependencies, not dozens of independent implementations.

For extraction, take maintained configuration/template/source changes with their consumers and regenerate derived artifacts in the destination tree. The file inventory retains every generated entry, test adaptation, deleted legacy file, metadata rename, resource capture and submodule pin so none is silently excluded from the comparison. Sources: [rusefi_config.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/integration/rusefi_config.txt), [LiveData.yaml](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/integration/LiveData.yaml), [output_channels.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/console/binary/output_channels.txt), [tunerstudio.template.ini](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/tunerstudio/tunerstudio.template.ini), [tests.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/tests.mk).

## Validation limits

This update verifies documentation, diff counts, source paths, report coverage and commit links. It does not run or modify firmware. Existing branch tests are described as evidence of coverage, not fresh passing results.

The branch-tip session report records a unit-test link failure involving `registerCanListener` and `unregisterCanListener`; the test source list omits `can_rx.cpp` while ISO-TP uses those symbols. That failure was not rerun or repaired here. External CAN ETB behavior depends on firmware outside this repository. The 6G75 tests explicitly do not establish clean crank synchronization or physical cylinder-1/TDC phase.

The major migration concerns remain the removed legacy VSS path, changed fan/idle settings, traction table signs and axes, RPM selector, TCU redesign, configuration placement and sensor/output layouts. A code count or test-file link does not establish tune compatibility, board memory fit or hardware behavior.

## Reproducing the inventory

The inventories are pinned snapshots rather than live remote queries. From a clone containing the fetched revisions:

```bash
git diff --stat 9154d40f573276b426b6f87ae6475cb9ea1a888c ebb877615ee157feaadc9a186d31f193f488c643
git diff --name-status 9154d40f573276b426b6f87ae6475cb9ea1a888c ebb877615ee157feaadc9a186d31f193f488c643
git diff c12cf14983152753a94001f4a101930fb2857fdc ebb877615ee157feaadc9a186d31f193f488c643 -- path/to/file
git log --reverse --oneline c12cf14983152753a94001f4a101930fb2857fdc..ebb877615ee157feaadc9a186d31f193f488c643
git cherry c12cf14983152753a94001f4a101930fb2857fdc ebb877615ee157feaadc9a186d31f193f488c643
```

In the file inventory, additions/deletions are measured from the common ancestor to the branch. `same_as_master` means identical content at the recorded path; `different_from_master` does not by itself prove unique branch authorship. `branch_only_path` is a path-level distinction and can include files moved elsewhere upstream; `deleted_in_branch` means the branch removes a path still present in master. Old/new path columns preserve renames, and the object columns record exact content or submodule IDs (`-` means absent). The listed report sections identify logical review units; shared files can serve several units.

In the commit inventory, `git_cherry_result` is `-` for a patch-equivalent upstream commit, `+` when no equivalent was found in the compared history, or `merge` for a merge commit excluded from that check. Exact-subject matches are explicitly weaker evidence than patch equivalence. The inventory includes all branch-side commits, including work that was subsequently replaced or reverted.
