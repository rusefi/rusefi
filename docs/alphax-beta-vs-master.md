# AlphaX beta changes compared with master

This report isolates the changes carried by `ElDominio/alphax-beta`, ordered from small documentation and tooling changes to changes in control algorithms, configuration storage, hardware acquisition, and crank synchronization. The branch is a substantial firmware variant: its features share a custom calibration page and an engine state machine, so many cannot be extracted by cherry-picking one commit.

## Comparison scope

Reviewed on 2026-10-17, after fetching both remotes. **Master means upstream `origin/master`**, rather than the older `master` branch in the fork.

| Reference | Pinned revision | Commit date |
| --- | --- | --- |
| AlphaX beta | `ebb877615ee157feaadc9a186d31f193f488c643` | 2026-10-12 |
| Upstream master | `c12cf14983152753a94001f4a101930fb2857fdc` | 2026-10-17 |
| Common ancestor | `9154d40f573276b426b6f87ae6475cb9ea1a888c` | 2026-09-03 |
| Fork master | `0a4b38b9250a981261dc59658b56794dee0e1567` | 2026-10-12 |

The fork's master is 93 commits behind upstream master and has the same common ancestor with AlphaX beta. Consequently the branch-side change inventory is identical with either master reference; the assessment of what upstream already provides uses the newer upstream revision. The checked-out local `master` was `2c1a195bce3`; fetching did not move that branch or change the checkout.

There are **302 branch-side commits and 769 upstream-side commits** since the common ancestor. The branch-side total comprises 294 non-merge commits and 8 merges. `git cherry` finds 10 patch-equivalent non-merge commits in the compared upstream history and 284 without an equivalent there. Those 284 are **not 284 new features**: earlier upstream imports, rewritten commits, documentation, follow-up fixes, and reverted experiments also appear in that set. An exact-subject search finds 108 branch commits whose subject also appears in upstream history; a subject match is a history clue, not proof of patch equivalence.

The net common-ancestor-to-branch diff contains **579 file entries, 138,989 inserted lines, and 4,127 deleted lines**. A direct comparison of the two tips contains 1,517 file entries, because it also includes later upstream changes missing from the fork. This report uses the former to identify branch work, then checks the latter to avoid treating already-upstream behavior as unique. It covers final retained behavior; superseded intermediate designs are identified where they would otherwise mislead an extraction effort.

Two companion inventories make the scope auditable:

- [Every changed file](alphax-beta-vs-master-files.tsv): status, old/new path, line counts, artifact type, corresponding report sections, and comparison with the upstream blob.
- [Every branch-side commit](alphax-beta-vs-master-commits.tsv): full hash, date, merge status, `git cherry` result, and exact-subject upstream matches.

Source links below point to immutable revisions. Test references mean coverage exists in the branch, **not that it passed in this review**. This was a source and history investigation; no branch firmware, simulator, Java build, or hardware test was run.

## Small changes and supporting tools

### 01 Documentation and repository housekeeping

Master does not contain this branch's feature catalogue, CAN ETB feasibility/follow-up notes, CAN tuning overview, misfire/CEL descriptions, crank-angle research, MSQ-converter research, or proposed LLM console-control design in these forms. The branch adds those documents, a Speeduino decoder reference copy, a large historical session report, and a cherry-pick ledger. The LLM-control and MSQ documents describe proposals/research; they do not establish implemented features in this branch.

It also changes `CLAUDE.md`, ignore rules for build output, and checks in a board-local `.claude/settings.local.json`. These are separable documentation/environment changes. The branch's historical report and guidance contain stale intermediate statements and should be read alongside final source. Sources: [FEATURE_FLAGS.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/FEATURE_FLAGS.md), [CAN_TUNING.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/CAN_TUNING.md), [llm-console-control.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/docs/llm-console-control.md), [msq-converter-research.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/docs/msq-converter-research.md), [report.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/docs/report.md), [master-cherry-picks.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/master-cherry-picks.md).

### 02 Firmware build GUI

The branch adds a standalone Tkinter build frontend, `build_gui.py`, above master's command-line build workflow. It selects repository/board/build options, runs builds, presents output, remembers local preferences, and copies firmware and bundle outputs to a chosen destination. Follow-ups ensure the full `rusefi.bin` is included with extracted bundles.

This is independently reviewable host tooling, with dependencies on the repository's existing build scripts and artifact conventions. No dedicated GUI tests are added. Source: [build_gui.py](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/build_gui.py).

### 03 Board configuration GUI

`board_config_gui.py` provides a Tkinter editor for board makefile flags and metadata, including batch board selection and tri-state feature choices. Master has no corresponding script. Its material behavior is writing build configuration, so extraction should preserve the distinction between inherited defaults and explicit `TRUE`/`FALSE` overrides.

This tool can be separated from the firmware features it exposes, but its hardcoded feature list needs to match the destination tree. Source: [board_config_gui.py](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/board_config_gui.py).

### 04 CI selection GUI and reduced board matrix

`board_ci_gui.py` toggles board builds by renaming enabled `meta-info*.env` files to disabled metadata names, using `git mv` where possible. Separately, the branch applies that policy to many board variants, including non-AlphaX boards and some AlphaX variants. It also changes board metadata flags used by bundle generation.

The GUI and the actual board-disable policy are two separable changes. A smaller passing matrix would not establish that disabled boards still build. Exact renames and metadata edits are retained in the file inventory. Sources: [board_ci_gui.py](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/board_ci_gui.py), [meta-info-proteus_f7.disabled_env](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/proteus/meta-info-proteus_f7.disabled_env).

### 05 OpenOCD and DFU GUI

The new `openocd_gui.py` wraps OpenOCD server control, programming, dumping, unlocking, DFU image selection/programming, serial-port discovery, and reboot-to-DFU commands. Master has the underlying programming workflows but no equivalent root-level GUI.

This is a separate desktop tool, not a firmware protocol change. Its programming/unlock actions need testing with the intended programmer and MCU; adding the script alone does not validate those actions. Source: [openocd_gui.py](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/openocd_gui.py).

### 06 Small defaults and diagnostic corrections

Several bounded changes can be reviewed separately: ignition-voltage telemetry falls back to battery voltage when no ignition-sense sensor exists; eight-bin CLT enrichment gets explicit defaults; Lua limiter axes receive ascending defaults; knock spectrogram guards use numeric feature checks; and a speedometer bench command exposes a configurable test frequency. The branch also removes the generic `iacByTpsTaper = 2` initialization and updates presets affected by renamed idle/VSS settings.

These should be extracted as individual fixes rather than as all of `default_base_engine.cpp` or `status_loop.cpp`, which also contain large feature integrations. Sources: [status_loop.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/console/status_loop.cpp), [default_fuel.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/defaults/default_fuel.cpp), [custom_page.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/custom_page.cpp), [software_knock.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/impl/software_knock.cpp), [speedometer.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/gauges/speedometer.cpp).

### 07 Configuration generator and TunerStudio presentation

The branch introduces a `graph_split` pseudo-field that starts a new generated graph without consuming configuration bytes. It also adds a digit-precision template argument to blend tables, permits unresolved digit placeholders during template parsing, and corrects how event-input pins are added to switch-input choices. Target-AFR blend tables use two display decimals while other blend tables retain one.

These are separate generator/presentation changes. Page-6 registration is a further dependency described in section 41. `FragmentDialogConsumerTest` gains graph-split coverage; that does not cover every generator alteration. Sources: [FragmentDialogConsumer.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_tools/configuration_definition_base/src/main/java/com/rusefi/output/FragmentDialogConsumer.java), [ConfigFieldImpl.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_tools/configuration_definition_base/src/main/java/com/rusefi/ConfigFieldImpl.java), [PinoutLogic.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_tools/configuration_definition/src/main/java/com/rusefi/pinout/PinoutLogic.java), [FragmentDialogConsumerTest.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_tools/configuration_definition/src/test/java/com/rusefi/test/FragmentDialogConsumerTest.java).

### 08 Explicit unsupported-ECU connection and flashing overrides

Master's compatibility checks reject an unsupported ECU/bundle pairing. The branch adds a session-local forced-port registry and a “connect anyway” action that retries blocked ports using the ECU's own reported definition. A separate firmware-update confirmation can permit programming despite the board-target mismatch.

Connection override and flash override are distinct behaviors and should remain separately reviewable. The latter changes a programming guard, not just UI wording. There is no dedicated new override test file in the branch-side diff. Sources: [ForcedEcuOverride.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_console/shared_io/src/main/java/com/rusefi/core/io/ForcedEcuOverride.java), [UnsupportedEcuCardHost.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_console/ui/src/main/java/com/rusefi/UnsupportedEcuCardHost.java), [BinaryProtocol.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_console/io/src/main/java/com/rusefi/binaryprotocol/BinaryProtocol.java), [BootloaderHelper.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_console/ui/src/main/java/com/rusefi/io/BootloaderHelper.java).

### 09 Upstream imports and dependency revisions

The history contains imported work on Ford TFI dwell, flex-fuel cranking/transients, Lua page separation, USB/MSD, Renix triggers, console VE generation/logging, and other upstream fixes. Their presence in the commit list does not make those features unique to AlphaX beta. Twenty-one branch-changed file entries now have exactly the same blob as current upstream; the inventory identifies them explicitly. Examples include CAN startup, H-bridge PWM, hardware-PWM support, and the STM32F4 RTC implementation.

The remaining CAN queue implementation is substantially shared with current master. Current master additionally rejects submissions quietly while a CAN device is intentionally removed; the branch lacks that later handling. Current master also has newer ISO-TP receive/flow-control fixes and configurable TS-over-CAN bus selection; those are upstream gaps, not branch additions. Sources: [can_msg_tx.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/hw_layer/drivers/can/can_msg_tx.cpp), [master: can_msg_tx.cpp](https://github.com/rusefi/rusefi/blob/c12cf14983152753a94001f4a101930fb2857fdc/firmware/hw_layer/drivers/can/can_msg_tx.cpp), [isotp.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/can/isotp/isotp.cpp), [master: isotp.cpp](https://github.com/rusefi/rusefi/blob/c12cf14983152753a94001f4a101930fb2857fdc/firmware/controllers/can/isotp/isotp.cpp).

The two changed submodule pointers move **backward** relative to the common ancestor. `firmware/libfirmware` points to `8052aed45e6`, nine commits behind the common-ancestor pin and fourteen behind current master. `unit_tests/googletest` points to `35b75a2cba6`, 136 commits behind the pin shared by the common ancestor and current master. Treat these as dependency differences requiring reconciliation, not feature implementations.

## Board support and build integration

### 10 Five new board packages

| Package | Change from master | Main integration considerations |
| --- | --- | --- |
| `alphax-s197-v2` | Ford S197 package based on the uaEFI F4 design, with board defaults, pinout, build metadata and knock configuration. | F4 memory budget, custom feature selection, calibration storage. |
| `alphax-s550-pnp` | S550 package with expanded/multiplexed analog inputs and board-specific defaults. | ADC3 and mux polarity, TPS/A/C-pressure mapping, 40,000-byte Lua configuration. |
| `fw-custom-paralela-master` | Custom board package, including F407/F427 metadata variants and bundle branding. | External CAN ETB opt-in, custom feature flags, 15,000-byte Lua configuration. |
| `protorico-econoline` | Econoline board/pinout with transmission-related wiring and CHT-based temperature support. | OSS/VSS setup, thermal estimator, oil-life defaults, TCU options. |
| `protorico-grummann` | Related Hellen-module step-van harness with distributor ignition, single-point injection, stepper idle wiring and physical CLT. | Repurposed pins and engine-specific tune setup; it is a distinct harness, not just a renamed Econoline board. |

Each package is a separate board change, but its generated headers/INIs are derived artifacts. Some packages consume the shared ADC, speed, storage and control changes later in this report. Sources: [board_configuration.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/alphax-s197-v2/board_configuration.cpp), [board_configuration.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/alphax-s550-pnp/board_configuration.cpp), [board_configuration.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/fw-custom-paralela-master/board_configuration.cpp), [board_configuration.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/protorico-econoline/board_configuration.cpp), [readme.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/protorico-grummann/readme.md).

### 11 Existing board feature selections and memory options

AlphaX two/four/eight-channel, GDI, Gold and Silver variants receive additional feature defines and UI flags. Hellen154 Hyundai variants also enable custom features. Several F7 builds select the larger internal-flash layout to retain Lua/custom pages across reboot. AlphaX Silver disables embedded uncompressed INI storage and selects STM32F429 memory support to make additional SRAM available.

Family defaults generally make custom features opt-in on F4 and enabled on F7, but there are exceptions: wheel-speed support defaults on broadly, and external CAN ETB is explicitly opt-in. Board overrides and standalone simulator/test headers decide the effective build; `FEATURE_FLAGS.md` alone is not authoritative. Sources: [efifeatures.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/stm32f4ems/efifeatures.h), [efifeatures.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/stm32f7ems/efifeatures.h), [board.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/hellen/alphax-silver/board.mk), [board.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/hellen/hellen154hyundai_f7/board.mk), [efifeatures.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/simulator/simulator/efifeatures.h), [efifeatures.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/efifeatures.h).

### 12 Packaging and embedded INI generation

The branch uses the actual branch name in local bundle naming, copies more artifacts instead of symlinking them, removes stale SREC/ZIP outputs before replacement, and includes firmware BIN output in bundle staging. Its simulator bundle targets explicitly depend on the INI RAM disk; the RAM-disk sentinel also depends on the generator script. Windows CI gets an explicit image-generation step.

For a board declaring `EFI_EMBED_INI_MSD=FALSE`, the generator writes a minimal `ramdisk_image` symbol instead of building the uncompressed image. Both image scripts also lose their original skip block. Review these together with current master's newer build rules: some directory-race fixes in the history are already upstream. Sources: [bundle.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/bundle.mk), [gen_image_board.sh](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/bin/gen_image_board.sh), [rusefi_config.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/rusefi_config.mk), [build-firmware-windows.yaml](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/.github/workflows/build-firmware-windows.yaml).

## Bounded sensor and calculation changes

### 13 TPS smoothing and discrepancy configuration

Master already has pedal smoothing. The branch adds separate unfiltered TPS1/TPS2 sensor identities and filtered proxies, sharing a configurable exponential-average coefficient. A fast change exceeding the configured threshold within a time window resets the average. Alpha defaults to 1/pass-through; zero reset settings leave the transient-reset feature disabled.

This affects every TPS consumer, including throttle feedback and acceleration enrichment. Separately, the default maximum permitted TPS/PPS discrepancy setting rises from 5 to 30, while the normal `etbSplit` default stays 5. That limit change is independently reviewable. No dedicated TPS-smoothing test is added; the final branch report records compilation followed by a CAN-related link failure. Sources: [init_tps.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_tps.cpp), [rusefi_config.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/integration/rusefi_config.txt), [default_base_engine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/defaults/default_base_engine.cpp).

### 14 Instantaneous fuel economy

The branch adds a calculation of fuel flow, L/100 km and US MPG from RPM, injector flow, effective pulse width after deadtime, cylinder count and vehicle speed, then publishes an MPG channel/gauge. Master has other fuel-consumption infrastructure but not this added calculator.

The formula assumes one injection event per cylinder per 720 degrees, so its interpretation depends on injection strategy; it is not a measured fuel-flow sensor. Dedicated arithmetic tests cover the new helper. Sources: [fuel_economy_calculator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/fuel_economy_calculator.cpp), [test_fuel_economy_calculator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_fuel_economy_calculator.cpp).

### 15 Manual fuel trim by bank

Master has cylinder trims and closed-loop bank corrections. The branch adds tuner-entered RPM/load trim tables per fuel bank, converts their signed percentages into multipliers, and applies them in the fuel calculation. The tables share the fuel-trim axes.

This is a relatively contained fueling feature, provided the table definitions, defaults, bank selection, TunerStudio editors and calculation move together. Coverage is added to fuel-math tests. Sources: [fuel_math.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel_math.cpp), [engine2.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine2.cpp), [test_fuel_math.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_fuel_math.cpp).

### 16 Injector small-pulse percentage curve

The existing injector nonlinearity modes gain `Curve (%)`. Below the final pulse-width bin, an interpolated percentage multiplies the theoretical pulse width; at or above the last bin the correction is bypassed. The curve is on custom page 6 and gated by `EFI_INJ_PERCENT_CURVE`.

This can be separated from the manual pressure-correction mode, but both modify the same injector model. The added injector test in this diff targets manual pressure correction rather than establishing coverage of the percentage-curve mode. Source: [injector_model.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/injector_model.cpp).

### 17 Manual injector pressure correction

Instead of the sensed-pressure square-root flow adjustment, a new mode applies a user table indexed by differential fuel pressure and requested fuel mass. It keeps the nominal flow ratio at 1, multiplies the base duration by the table value, then adds deadtime. It still requires the pressure-sensor path.

The default table is unity and the default dimensions are small; board overrides can enlarge it. The injector-compensation enum gains a wider bit field, so this includes configuration-layout changes. Sources: [injector_model.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/injector_model.cpp), [rusefi_config.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/integration/rusefi_config.txt), [test_injector_model.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_injector_model.cpp).

### 18 VVT-dependent fuel and ignition corrections

The branch adds intake/exhaust cam-angle versus RPM correction tables. Fuel percentages multiply the normal running-fuel correction; ignition values add to advance corrections. The current implementation reads bank 0 cam positions, rather than implementing independent corrections for every bank.

This feature is separate from the VVT actuator PID redesign in section 56. It needs table/default/UI integration and valid cam positions; invalid positions contribute no correction. Source and coverage: [fuel_math.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel_math.cpp), [ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp), [test_vvt_corrections.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_vvt_corrections.cpp).

### 19 Burst knock retard

The new module uses acceleration enrichment's TPS jump/lookback to index an RPM/TPS-rate retard table. It holds retard during the transient, then decays it to zero. Negative table values cannot advance timing.

Despite its name, this is feed-forward transient timing retard, not a new acoustic knock detector. It is reasonably separable with its ignition consumer and page-6 fields. Sources: [burst_knock.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/burst_knock/burst_knock.cpp), [test_burst_knock.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_burst_knock.cpp).

### 20 Time-based WOT enrichment

The new WOT module looks up an AFR adder from elapsed time in the engine state machine's WOT condition, capped at 30 seconds. Leaving WOT resets the timer. The adjusted AFR is also reflected in the published lambda target.

The calculation is small, but it depends on the state machine, page 6 and the fuel-target pipeline. The diff has no dedicated WOT-enrichment test file. Sources: [wot_enrichment.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/wot_enrichment/wot_enrichment.cpp), [fuel_computer.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/fuel_computer.cpp).

### 21 Launch switch semantics and Lua controls

The branch adds a clutch-up activation choice: launch is active while the clutch-up switch is not asserted. A separate setting lets an asserted clutch-up switch cancel other launch activation modes. A new `setLaunchRpm(rpm)` Lua hook clamps its input to 0–20,000 and changes the live setting.

Keep the positive release check distinct from assuming a nonasserted switch proves clutch depression. Current master already has much of the broader launch/flat-shift machinery seen in historical commits. Sources: [launch_control.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/launch_control.cpp), [lua_hooks.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/lua/lua_hooks.cpp), [test_launch.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_launch.cpp), [test_lua_hooks.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/lua/test_lua_hooks.cpp).

### 22 Priming after trigger teeth

Master schedules priming after a time delay. The branch adds a selectable mode that arms at ignition-on and fires after a configured number of raw trigger events, without waiting for synchronization. Zero teeth is treated as one; ignition-off disarms the pending prime.

This is an isolated priming change with an additional trigger callback and tune fields. The existing flood-clear/fresh-start conditions remain part of the path. Sources: [prime_injection.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/engine_cycle/prime_injection.cpp), [trigger_central.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_central.cpp), [test_startOfCrankingPrimingPulse.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_startOfCrankingPrimingPulse.cpp).

### 23 Cranking without ECU spark

A new option clears spark permission while the existing RPM state reports cranking. It is intended for an external ignition module that supplies its own crank spark. ECU spark resumes after the normal cranking transition; this is separate from master's kick-start behavior and does not itself disable injection.

It is contained in the protection/spark-permission path plus page-6 configuration and a new clear reason. Sources: [limp_manager.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/limp_manager.cpp), [test_cranking_no_spark.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_cranking_no_spark.cpp).

### 24 Delayed start authorization

Master's start-button handling can discard a press when cranking is blocked. The branch keeps sampling the button edge, adds Lua `startDisabled()`, and holds a pending request for one second when Lua authorization is still absent. It retries the original request when all starter gates pass, or drops it on timeout/engine movement.

This is a separate starter-state change; it should not be confused with the no-spark option. No dedicated starter test is added in this diff. Sources: [start_stop.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/start_stop.cpp), [start_stop.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/start_stop.h), [lua_hooks.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/lua/lua_hooks.cpp).

### 25 Alternator feed-forward and output telemetry

The branch adds a target-voltage/RPM base-duty table as an alternative to the fixed PID offset. It removes double application of the offset when combining open- and closed-loop terms, clamps the final duty to 0–100%, and reports the applied duty rather than only the raw PID output. Eco mode can additionally blend toward its own voltage target.

The base-duty/control correction and the Eco-mode integration are separable. The output-channel structure also changes. Sources: [alternator_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/alternator_controller.cpp), [test_alternator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_alternator.cpp), [test_alternator_voltage_target_set_point.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_alternator_voltage_target_set_point.cpp).

### 26 Boost target calculation in open loop

The branch computes boost target even in open-loop mode, allowing that target to serve as the duty table's Y axis. It adds a gear-based target adder and exposes target-temperature adjustments in that workflow. A state-machine limp ceiling can cap the computed target.

The limp ceiling only changes open-loop duty when the duty map actually uses boost target as its axis; it is not a general open-loop duty clamp. Sources: [boost_control.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/boost_control.cpp), [gppwm_channel_reader.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/gppwm_channel_reader.cpp), [test_boost.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_boost.cpp).

### 27 Predictive MAP event lifetime

Master already provides predictive MAP. The branch adds a separate session timer so repeated TPS retriggers cannot extend a prediction past the configured overall duration. It also resets the recorded TPS peak on each new prediction, preventing a previous larger event from immediately cancelling a smaller later event.

These are two small fixes in one subsystem, separable from the other AlphaX controls. No corresponding predictive-MAP test change appears in the branch-side diff. Sources: [speed_density_airmass.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/airmass/speed_density_airmass.cpp), [speed_density_airmass.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/airmass/speed_density_airmass.h).

## Actuator and vehicle control additions

### 28 Intake manifold runner control

The new IMRC controller supports solenoid and timed H-bridge outputs, configurable PWM duties, inversion, and RPM/TPS/MAP thresholds. Enabled thresholds must all be satisfied; it does not open merely because any one configured threshold passes.

This is an independent actuator feature with page-6 and hardware-output dependencies. There is no dedicated IMRC test file in the diff. Sources: [imrc_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/imrc_controller.cpp), [imrc_controller.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/imrc_controller.h).

### 29 Exhaust cutout control

The branch adds digital, PWM and timed H-bridge cutout actuation, switch/Sport/automatic activation, threshold delays, indicator output, and key-on/engine-on movement tests. Automatic operation requires all configured RPM/TPS/MAP checks; Lua can force the automatic path open via `setExhCutoutActive()`.

Basic cutout operation is separable, but Sport mode and the pops-and-bangs inhibit consume shared state. The output ownership and transition behavior deserve separate hardware verification. Sources: [exhaust_cutout.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/exhaust_cutout.cpp), [test_exhaust_cutout.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_exhaust_cutout.cpp).

### 30 Clutch delay valve

A new CDV module drives a solenoid around launch/pre-launch. Simple mode releases after a delay following launch exit; Smart mode requires a clutch-pressure window and uses a maximum active duration. Vehicle-speed, clutch-release and pressure-window exits can also end activation.

The change includes a clutch-pressure sensor, output registration, live state and configuration. It depends on launch state but does not require the complete transmission controller. Sources: [cdv_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/cdv_controller/cdv_controller.cpp), [init_fluid_pressure.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_fluid_pressure.cpp), [test_cdv_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/controllers/modules/cdv_controller/test_cdv_controller.cpp).

### 31 Cooling fan pressure mode and PWM semantics

Master already has relay/PWM fan control and, at the pinned revision, PWM stopped-engine and vehicle-speed inhibition fixes. The branch goes further: per-fan A/C modes become Disabled/Relay/Pressure, pressure can request airflow without compressor engagement, vehicle-speed inhibition gains hysteresis, and relay/PWM paths share inhibition logic.

PWM tables now describe 0–100% airflow demand, which is ramped and then mapped between “off” and “full-speed” duty endpoints. Reversed endpoints support inverted hardware. This changes the meaning of existing min/max duty settings, removes the old A/C PWM adder, and changes defaults. The Java boolean migration entries for the old fan toggles are removed; that alone is not evidence of an automatic conversion to the new mode fields. Sources: [fan_control.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/fan_control/fan_control.cpp), [master: fan_control.cpp](https://github.com/rusefi/rusefi/blob/c12cf14983152753a94001f4a101930fb2857fdc/firmware/controllers/modules/fan_control/fan_control.cpp), [test_fan_control.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_fan_control.cpp).

### 32 Dual and PWM fuel pumps

The existing single relay pump gains dual-relay staging and PWM pressure control. Dual mode enables the secondary pump when RPM, load and TPS thresholds all pass, with hysteresis on release. PWM mode combines a load/RPM pressure target, pressure/RPM base-duty map and PID; invalid pressure falls back to feed-forward. Aggressive relief holds minimum duty during low-demand overpressure until its recovery threshold is crossed.

This combines output initialization, pressure sensing, PID limits and custom configuration. The PWM fallback is the configured minimum duty, including the invalid-output path; that is a material difference from interpreting every inactive condition as a zero-duty relay command. Sources: [fuel_pump.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/fuel_pump/fuel_pump.cpp), [test_fuel_pump_pwm.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_fuel_pump_pwm.cpp).

### 33 Idle air and target rework

The branch changes A/C idle RPM from an absolute floor to an adder, replaces the fixed A/C air offset with a pressure curve, adds three external idle-up switches, and adds selectable cranking air, cranking RPM flare and a CLT-based hold before taper. With flare enabled, cranking air becomes a fading addition to running air. The old idle-return-target ramp is removed from the calculation.

An off-idle controller holds an RPM addition, waits for stability, then decays it; re-cranking cancels it. A clutch/neutral override can retain closed-loop idle at vehicle speed, with a latch to prevent gear-detection chatter. Fixed cranking TPS is a separate ETB override, suppressed at vehicle speeds of 2 km/h or more. These are several extractable subchanges, but they share phase/target logic and later mode overlays. Sources: [idle_thread.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/idle_thread.cpp), [electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp), [test_idle_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_idle_controller.cpp), [test_etb.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_etb.cpp).

### 34 Soft RPM range and ETB RPM PID

Master uses the hard-limit hysteresis span for soft timing/fuel adjustments and a separately configured linear ETB taper. The branch gives soft timing/fuel a separate range and replaces the throttle taper with an opt-in PID referenced to the effective hard limit, targeting 50 RPM below it. The PID has tunable gains and an initial throttle seed, and can only reduce the current requested opening.

Lua-gauge curves or Sport-mode adders can also change RPM and boost-cut limits. These adders, the soft-range split and the ETB PID are logically separate changes. Sources: [limp_manager.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/limp_manager.cpp), [electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp), [test_etb.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_etb.cpp).

### 35 Replacement vehicle and wheel-speed pipeline

Master's legacy physical VSS and built-in vendor CAN VSS paths are replaced. Main vehicle speed selects output-shaft speed, front axle or rear axle; each source can be physical or CAN/Lua-fed. Output-shaft frequency becomes RPM, then uses final-drive ratio and wheel revolutions/km for vehicle speed. A shared percentage-per-second plausibility setting governs separate physical-source filter instances. Wheel slip can be derived from a selected pair of speed sources.

This is a migration, not only added sensors: `can_vss.cpp`, the old converter, legacy tune fields and hardware VSS tests are deleted. Lua examples are adapted to the replacement sensor path. The old vendor choices have no equivalent built-in decoder in this change; CAN/Lua users must provide the new channels. Sources: [init_vehicle_speed_sensor.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_vehicle_speed_sensor.cpp), [init_wheel_speed_sensors.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_wheel_speed_sensors.cpp), [wheel_slip_ratio_source.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/wheel_slip_ratio_source.cpp), [test_wheel_speed_sensors.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/sensor/test_wheel_speed_sensors.cpp), [test_output_shaft_speed_converter.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/sensor/test_output_shaft_speed_converter.cpp).

### 36 Gear detection and transmission slip

Gear detection now respects clutch disengagement and can explicitly use output-shaft speed with engine or input-shaft RPM. The new slip estimate compares actual RPM with the expected RPM in the detected gear. It requires an OSS-based reference, a valid gear/RPM source and minimum vehicle speed, avoiding confusion between tire slip and transmission slip.

The gear-selection extension can be isolated from slip reporting. The slip output is also the foundation for the TCU pressure trim in section 58. Sources: [gear_detector.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/gear_detector/gear_detector.cpp), [test_gear_detector.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_gear_detector.cpp).

### 37 Coordinated traction control

Master looks up throttle/timing/spark effects in their individual consumers. The branch introduces a common traction controller with held corrections and decay, selectable slip or RPM-acceleration Y input, Lua-gauge scaling, and minimum speed/pedal-demand gates. Table entries become positive removal magnitudes; throttle/timing outputs are made subtractive, spark percentages become fractions, and table axes are corrected.

This is a behavior and calibration-format change. Existing tables cannot be assumed to preserve their meaning across sign/axis changes. The branch-side diff has no dedicated traction-control test file, so the implementation and related engine tests are not evidence of full traction coverage. Sources: [traction_control.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/traction_control.cpp), [traction_control.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/traction_control.h), [ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp).

### 38 Cylinder-head sensor and thermal estimates

The branch adds a configurable CHT input, an optional CLT estimate based on heat transfer from the head versus radiator rejection, and a separate EOT estimate using CHT, intake/ambient temperature and oil pressure. CLT estimation includes thermostat lag, coolant-response lag, fan airflow and saturating speed-related airflow. EOT uses separate heating/cooling time constants and limits heating when oil is not circulating.

The physical CHT sensor, CLT model and EOT model are three separable pieces. The estimates publish under normal temperature sensor identities, affecting all downstream users; fallback numbers are calibration assumptions, not measured temperatures. Sources: [init_thermistors.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_thermistors.cpp), [cht_clt_estimator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/cht_clt_estimator.cpp), [eot_estimator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/sensors/eot_estimator.cpp), [test_cht_clt_estimator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/sensor/test_cht_clt_estimator.cpp), [test_eot_estimator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/sensor/test_eot_estimator.cpp).

### 39 Weighted oil-life monitor

A new module accumulates temperature-weighted revolutions and converts the result into remaining oil-life percentage. It selects oil or coolant temperature with fallback, uses fixed temperature zones with tunable weights, exposes reset/set commands and a Lua reset hook, and requests persistence at ignition-off.

This is an estimate based on configured weights, not an oil-quality measurement. Integration extends storage records and delayed main-relay shutdown so the counter can be saved. Ordinary configuration burns also write the record through the common extra-page path. Sources: [oil_life_monitor.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/oil_life_monitor/oil_life_monitor.cpp), [storage.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/storage.cpp), [test_oil_life_monitor.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_oil_life_monitor.cpp).

## Shared architecture and operating modes

### 40 Engine module storage and F4 RAM placement

Master keeps its modules in the engine's module list. The branch splits core and AlphaX modules, adds combined iteration/aggregation helpers and routes lifecycle callbacks across both lists. It also moves the production F4 engine allocation out of the fixed CCM section.

This is a system-wide memory/lifecycle change, not merely a board flag. Module lookup, callback ordering, defaults, configuration notifications and shutdown aggregation must remain consistent. Existing feature tests exercise parts of the framework, but do not establish per-board RAM fit. Sources: [engine.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine.h), [engine_module.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/core/engine_module.h), [engine_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/engine_controller.cpp), [engine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine.cpp).

### 41 Custom page 6 and persistent layout

Master already has separate extra pages, including Lua page 5. The branch adds page 6 for custom controls, its default initializer and storage container, generator registration, TS page routing, SD record support and burn/load integration. The custom container's final data version is 23; the main flash-data version is 260912.

Internal-flash placement derives page 6 from the end of the board-sized Lua container, aligned to 32 bytes. The oil-life record follows it, with overlap/sector-size assertions. Extra-page burns on internal-flash-only builds request a deferred full configuration write. Page growth, field movement, removed fields and renamed settings mean raw configuration images are not interchangeable with master; the report does not establish a complete automatic tune migration. Sources: [config_page_6.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/integration/config_page_6.txt), [custom_page.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/custom_page.cpp), [extra_flash_pages.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/extra_flash_pages.cpp), [storage.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/storage.cpp), [TSProjectConsumer.java](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/java_tools/configuration_definition/src/main/java/com/rusefi/output/TSProjectConsumer.java).

### 42 Engine state machine and shift recognition

The new state machine classifies Off, Cranking, Afterstart, Idle, Coasting, Transient, WOT, Cruising, Overrun, Accelerating and Decelerating conditions, then publishes launch, shift, torque-reduction and operating-mode overlays. Master has subsystem states but no corresponding AlphaX coordinator.

Shift direction uses an accumulated RPM-rate or vehicle-speed-rate signal, clutch edges, optional second-switch handling, minimum speed, latch timing and a stale-window timeout. RPM-rate observation has a tunable cadence; transient hold is measured in milliseconds. State-based STFT region selection is a separate consumer. This is a central dependency of many later items. Sources: [engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp), [engine_state_machine.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.h), [closed_loop_fuel.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/math/closed_loop_fuel.cpp), [test_engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/engine_state_machine/test_engine_state_machine.cpp), [test_stft.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_stft.cpp).

### 43 Sport mode and pedal shaping

Sport mode is a shared flag selected by hardware switch or Lua-gauge threshold. It can drive pedal shaping, exhaust cutout, downshift permission, limiter adders, Ghost Cam and pops-and-bangs permission. Limp suppresses Sport, and Sport suppresses Eco.

Sport Pedal separately multiplies the pedal-derived throttle target by a pedal-indexed curve before idle-range compression, with a clamp. It can also use its own switch. These are two connected features, rather than a second complete throttle map. Sources: [engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp), [electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp), [test_etb.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_etb.cpp).

### 44 Eco operating mode

After qualified cruising, the branch can blend toward an absolute AFR target, ignition addition, optional VVT targets, a throttle multiplier and alternator-voltage target. MAP, speed, RPM, switch/Lua inhibition and operating-state checks govern entry/exit. A post-engagement lock avoids reacting to transients caused by Eco's own actuation; Limp/Sport/inhibition still take priority.

Optional slow transitions ramp target blends over one second. This feature crosses fueling, ignition, throttle, VVT and charging, so extracting only the state-machine gate would not reproduce it. Sources: [engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp), [fuel_computer.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/fuel_computer.cpp), [test_eco_mode.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_eco_mode.cpp).

### 45 Quick warmup

Cold idle can activate coordinated additional air, an absolute lambda target and an absolute idle timing value. The final timing behavior replaces the idle timing curve when separate idle advance is enabled; it is not the earlier historical “timing retard” design.

This requires the state machine and its cold/idle classification, plus the air/fuel/ignition consumers. The absolute-timing gate matters when comparing tunes that do not use separate idle advance. Sources: [engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp), [ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp), [idle_thread.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/idle_thread.cpp).

### 46 Ghost Cam idle mode

The branch adds an idle-only lope mode selected by a switch or Sport mode. It changes VVT targets, AFR, idle target and base air, and swaps in a separate idle-ignition PID. The idle controller bypasses RPM-only coasting classification while active, allowing the intended RPM excursions without immediately disabling the mode.

Throttle/speed and temperature conditions still matter, and normal idle PID state is restored/reset on exit. Optional blending shares the Eco transition mechanism. This is a coordinated idle/VVT/fueling feature, not a trigger pattern. Sources: [engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp), [idle_thread.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/idle_thread.cpp), [vvt.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/vvt.cpp), [test_engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/engine_state_machine/test_engine_state_machine.cpp).

### 47 Pops and bangs

An overrun-driven state machine supplies delay/duration, RPM/temperature gates, optional Sport/cutout permission, an absolute timing override, VE override, additional air and optional periodic spark-cut windows. It inhibits DFCO while active. The branch's normal ignition path returns the P&B timing override early when eligible.

This is a distinct feature from rolling launch or the existing anti-lag controller, although it uses shared spark-skip and air/fuel paths. It needs those integrations and its state-machine tests, not just the calibration fields. Sources: [engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp), [airmass.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/airmass/airmass.cpp), [ignition_state.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/ignition/ignition_state.cpp), [test_pops_and_bangs.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_pops_and_bangs.cpp).

### 48 Points-based check-engine escalation

Master already has range checks, DTC handling and a malfunction indicator. The branch adds debounced TPS circuit/intermittency checks and a point total including existing voltage-range faults and latched misfire. Thresholds control added TPS DTCs and flashing escalation; existing raw range checks remain independently active. The key-on/engine-off lamp check becomes configurable.

Several page-6 CEL enable fields are reserved without matching new checks in `updateCheckEngineTriggering()`, so the field list must not be advertised as implemented oil-pressure/AFR/temperature diagnostics. The diff also gates DTC-manager compilation and SD handling with `EFI_TOOTH_LOGGER`. Sources: [check_engine_light.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/check_engine_light/check_engine_light.cpp), [malfunction_indicator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/malfunction_indicator/malfunction_indicator.cpp), [dtc_manager.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/dtc_manager/dtc_manager.cpp).

### 49 Misfire configuration and activation changes

Misfire detection itself already exists in master. The branch moves its calibration to page 6 and changes monitoring from the idle controller's idle flag to the new state machine's Idle condition, excluding Quick Warmup. The adaptive detector remains recognizably the existing algorithm.

This is a configuration/dependency change rather than a wholly new misfire detector. Updated tests set the new configuration/state. Sources: [misfire_detection.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/misfire_detection.cpp), [test_misfire_detection.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_misfire_detection.cpp).

### 50 Configurable latched limp mode

With the new state machine enabled, an ETB jam latches configurable RPM, throttle, boost-target, timing and fueling responses instead of only master's fixed ETB-jam response. Flashing CEL escalation can also latch this mode. The ordinary rev limiter, launch and transient fuel/spark cuts do not themselves imply limp.

The latch persists until power cycle and mode priority is enforced across multiple consumers. The old jam response remains as fallback when the state machine is unavailable/disabled. Because this changes fault response, it needs an independent review even if the mode framework is adopted. Sources: [limp_manager.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/limp_manager.cpp), [engine_state_machine.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine_state_machine.cpp), [electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp).

### 51 State-aware DFCO

Master's overrun fuel cut becomes selectable Overrun, Decel or Both. Common guards remain separate from physical overrun detection, overrun can require a detected gear, and Decel uses state-machine deceleration with an RPM-rate fallback when vehicle speed is absent. P&B and active up/down shifts suppress the cut.

This depends on state classification and shift behavior, not just renamed DFCO fields. Coverage extends fuel-cut tests. Sources: [dfco.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/fuel/dfco.cpp), [test_fuelCut.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/ignition_injection/test_fuelCut.cpp).

### 52 Downshift rev matching

The new blipper latches a trustworthy pre-shift gear, computes the next lower gear's RPM at current speed, and commands ETB through opening, control and closing phases. It uses feed-forward plus PID, with driver-demand, RPM, speed, optional brake/Sport gates, timeouts and lockout.

It depends on the state machine, calibrated gear ratios/speed and ETB. Its target replaces the pedal-derived target before later limp/rev-limit clamps. Sources: [downshift_blipper.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/downshift_blipper.cpp), [electronic_throttle.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle.cpp), [test_downshift_blipper.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_downshift_blipper.cpp).

### 53 Upshift RPM hold

The new upshift controller commands ETB toward the next higher gear's matching RPM while the shift is in progress. It has ramping, PID, timeout, minimum/maximum RPM and speed gates, driver override and a high-RPM lockout. Flat-shift torque reduction takes priority over the hold.

It shares feed-forward/configuration and dependencies with downshift blipping but has its own controller and tests. The final implementation holds until a real exit condition; earlier short-lived hold behavior is superseded. Sources: [upshift_rpm_hold.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/upshift_rpm_hold.cpp), [test_upshift_rpm_hold.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_upshift_rpm_hold.cpp).

### 54 Launch power ramp

The new module starts a time-indexed ignition-retard curve when launch control releases at WOT. Negative values are clamped away, and once started the curve runs to completion independently of later throttle movement.

The calculation is small but consumes launch state and modifies the common ignition correction. It is separate from rolling launch and flat-shift torque reduction. Sources: [launch_power_ramp.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/launch_power_ramp/launch_power_ramp.cpp), [test_launch_power_ramp.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_launch_power_ramp.cpp).

### 55 Rolling launch

While a hardware/Lua trigger is held and speed/RPM gates pass, rolling launch captures RPM and builds a spark-skip ramp up to a capped target window. It applies fixed timing retard and fuel addition while armed, then ramps the retard out after release.

This is a new moving-vehicle launch feature. The final source uses a graded skip ratio across the captured-RPM window; the early commit subject's “hard spark limiter” shorthand is incomplete. Sources: [rolling_launch.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/modules/rolling_launch/rolling_launch.cpp), [engine2.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/engine2.cpp), [test_rolling_launch.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_rolling_launch.cpp).

### 56 Advanced VVT control

Master's fixed PID offset/proportional gain can be replaced by an oil-pressure base-duty curve and a signed distance-to-target proportional-gain curve, separately for intake/exhaust. Integral bounds become configurable, and advanced mode bypasses the normal near-zero target hysteresis gate. Telemetry reports the scheduled P term and final applied command.

The final controller always runs PID trim over feed-forward; the historical fade/pause-window experiments were removed. Missing oil pressure evaluates the curve at zero pressure. This feature also changes the shared PID API. Sources: [vvt.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/vvt.cpp), [efi_pid.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/util/math/efi_pid.cpp), [test_vvt.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/actuators/test_vvt.cpp).

### 57 First-order RPM estimation

The old `alwaysInstantRpm` boolean becomes Per-cycle/First Order/Instant. First Order extrapolates from cycle RPM using a smoothed cycle-derived rate; spin-up temporarily uses instant RPM until valid cycle measurements can advance the RPM state. Phase-resynchronization disturbances do not update that baseline.

This is a central timing-input change affecting many controls and test fixtures. The enum/boolean migration and spin-up transitions are part of the same unit, not optional follow-ups. Sources: [rpm_calculator.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/engine_cycle/rpm_calculator.cpp), [test_fasterEngineSpinningUp.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/trigger/test_fasterEngineSpinningUp.cpp), [test_rpm_acceleration.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/trigger/test_rpm_acceleration.cpp), [crank_angle_estimation.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/docs/AI/crank_angle_estimation.md).

## Transmission and hardware changes

### 58 Automatic transmission controls

Master's existing TCU framework is narrowed in the normal configured path: defaults/fixup force Automatic plus Generic4 when TCU is enabled. ButtonShiftController is removed and SimpleTransmissionController's shift-solenoid work is folded into Generic4. Automatic control gains an optional idle-to-first action and shift-margin telemetry; current gear is derived from gear detection rather than merely the last command.

The final line-pressure design is a 5×5 RPM/driver-demand base-duty table plus shift, short TCC-lockup, per-gear and slip-trim additions. TCC adds minimum gear/temperature, brake and shift gates. Shift completion requires a sustained detected-gear match, retaining a time fallback where applicable. Slip correction is a bounded, signed accumulator above a speed/demand maximum-slip table, decaying after a clean interval and resetting on a commanded shift. It is not closed-loop measured-pressure regulation.

These are separable TCU units, but the slip trim depends on sections 35–36, and idle-to-first depends on section 42. Earlier three-band pressure control and a duty slew ramp are superseded. Solenoid polarity must match the calibration; signed duty additions are not universally “more pressure.” Sources: [tc_4.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/tcu/tc_4.cpp), [tcu.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/tcu/tcu.cpp), [gc_auto.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/tcu/gc_auto.cpp), [gear_controller.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/tcu/gear_controller.cpp), [test_tcu.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/test_tcu.cpp).

### 59 External CAN throttle controller

The branch adds an opt-in interface to a separate CH32V203 ETB controller. rusEFI sends target, gains, limits, bias curve and calibration over private extended CAN IDs. Raw TPS/pedal telemetry feeds virtual channels through normal calibration/redundancy processing; duty/PID/feed-forward/status telemetry populates existing diagnostics. Local throttle PID/motor driving is bypassed for normal remote operation, while bench, calibration and autotune gain dedicated remote paths.

The ECU still computes the combined target and stops sending normal commands on local faults, pause or invalid setpoints; remote disable then depends on the external controller's command timeout. The external firmware is outside this repository and was not verified here. Receive listeners do not provide independently addressed per-throttle telemetry; current code uses one target stream, so this is not complete independent dual-ETB support. `CanDcMotor::isFault()` remains a stub. Comments disagree about external autotune implementation status, so only the rusEFI-side transport can be established from this tree.

This should be isolated as protocol definitions, virtual sensor ingestion, command/telemetry, calibration/bench operations and fault handling, with end-to-end verification before adoption. `EFI_EXTERNAL_CAN_ETB` is off in the unit-test build. Sources: [can_etb.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/can/can_etb.h), [can_etb_remote.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/can/can_etb_remote.cpp), [init_etb_can.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/init/sensor/init_etb_can.cpp), [electronic_throttle_impl.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/actuators/electronic_throttle_impl.h), [external-etb-can-followups.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/docs/external-etb-can-followups.md).

### 60 Expanded ADC acquisition and mux control

The branch extends STM32 slow-ADC handling with separate ADC1/ADC3 mux selection, additional ADC3 channel groups, up to 48 logical channels, inverted mux polarity, corresponding parent-channel mapping and extra conversion stages. S550 uses the ADCD3 slow path, while raw mux diagnostics and forced GPIO writes help establish mux state. ADC output-pin API changes accompany that forced write.

This is board-sensitive acquisition work, not just extra names in a sensor enum. Master has evolved its own ADC support since the common ancestor, so these changes need to be reconciled at the hardware/conversion-sequence level. The branch-side diff contains no dedicated new ADC acquisition test; host sensor tests cannot establish sampling timing or correct physical channel routing. Sources: [stm32_adc_v2.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/hw_layer/ports/stm32/stm32_adc_v2.cpp), [stm32_adc.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/hw_layer/ports/stm32/stm32_adc.cpp), [adc_onchip_slow.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/hw_layer/adc/adc_onchip_slow.cpp), [efi_output.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/system/efi_output.h), [board.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/alphax-s550-pnp/board.mk).

### 61 Mitsubishi synchronization and sync-loss diagnostics

Three logically distinct decoder changes are retained. `TT_6G72_CRANK` plus `VVT_MITSUBISHI_6G72_BETA` uses crank/cam edge order and levels to establish provisional phase early. The normal cam decoder still provides confirmed phase; provisional phase permits wasted-spark/batch operation without claiming sequential synchronization. The source explicitly limits the fast-path lookup to the observed alignment and requires re-derivation for other alignments.

`TT_36_2_1_1_V2` is an explicitly experimental alternative for 6G75 crank sync. It uses candidate-gap spacing and tooth counts, with bounded coasting through weak gaps. Its tests pin observed error counts and warnings, not clean successful operation; two captures use an unconfirmed assumed time scale. The original trigger remains available, and the attempted “graduate back to the old decoder” design was reverted.

`VVT_MITSUBISHI_6G75_BETA` separately resolves phase from alternating cam-edge counts instead of the old cam gap decoder. Its test confirms that full-phase state is reached, but explicitly does not establish physical cylinder-1/TDC alignment. This distinction is essential before treating it as sequential-operation validation.

A smaller independent addition records the last sync-loss reason for telemetry. The decoder extensions also change common synchronization state and the protection gate, so they need review beyond their waveform definitions. Sources: [trigger_central.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_central.cpp), [trigger_decoder.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/trigger_decoder.cpp), [trigger_mitsubishi.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/trigger/decoders/trigger_mitsubishi.cpp), [test_real_6g72_3000gt.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/trigger/test_real_6g72_3000gt.cpp), [test_real_6g75_v2.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/trigger/test_real_6g75_v2.cpp), [test_real_6g75_beta_cam.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/trigger/test_real_6g75_beta_cam.cpp), [mitsubishi-6g72-fast-crank-cam-sync.md](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/docs/mitsubishi-6g72-fast-crank-cam-sync.md).

### 62 Shared schema and generated delivery surface

Across the features above, the branch changes enums and sensor IDs, live-data layouts, log fields, gauges, menus, console enum mirrors, default tunes, module/build lists and generated firmware/INI artifacts. New per-board generated configuration headers and INIs, plus the compressed embedded INI image, account for much of the line-count growth. These are feature delivery dependencies, not dozens of independent implementations.

For extraction, take maintained configuration/template/source changes with their consumers and regenerate derived artifacts in the destination tree. The file inventory retains every generated entry, test adaptation, deleted legacy file, metadata rename, resource capture and submodule pin so none is silently excluded from the comparison. Sources: [rusefi_config.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/integration/rusefi_config.txt), [LiveData.yaml](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/integration/LiveData.yaml), [output_channels.txt](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/console/binary/output_channels.txt), [tunerstudio.template.ini](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/tunerstudio/tunerstudio.template.ini), [tests.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/unit_tests/tests/tests.mk).

## Dependencies and extraction sequence

Complexity above means the breadth of behavior and dependencies, not patch length or commit age. A practical sequence for selecting work from this branch is:

1. Review documentation, desktop tools and small independent fixes individually; retain current upstream fixes already present.
2. Choose the board targets and reconcile dependency pins/build rules. Establish a clean baseline build before attributing subsequent failures to a feature.
3. Choose the configuration/storage representation and module-memory strategy. Page 6 is a branch implementation choice; an upstream feature does not necessarily need the entire AlphaX page.
4. Port bounded math/sensor/control changes with their tune fields, defaults, UI and tests.
5. Port the state machine before its dependent modes, DFCO behavior and shift-assist controllers.
6. Port speed/gear infrastructure before traction and TCU slip control. Port board ADC wiring before relying on its acquired sensors.
7. Review external CAN ETB and each Mitsubishi decoder separately, including the physical fault/phase behavior that host tests do not establish.

The broad June feature-set commit and many later integration commits mix multiple units. Use the section-level source boundaries and complete inventories to construct patches; the commit list is not a ready-made cherry-pick sequence.

## Validation evidence and remaining limits

The branch adds substantial host coverage for state-machine transitions, Eco/P&B, idle, throttle, blipper/hold, fuel pump, fan, oil-life and thermal calculations, gearing/TCU, priming, launch and trigger replay. Some changed tests only adapt to a renamed field or newly required state. Historical reports also contain corrections retracting earlier claimed validation, which is why this report does not convert historical prose into fresh passing results.

At the branch tip, `docs/report.md` records a unit-test link failure involving `registerCanListener`/`unregisterCanListener`. Static inspection supports the reported dependency problem: ISO-TP constructors/destructors reference the listeners, while the branch's test source list does not include `can_rx.cpp`. Current upstream has additional guards in this area. The failure was not rerun or repaired during this documentation task.

The most consequential compatibility changes are the deleted legacy VSS path, renamed/repurposed idle and fan settings, traction table sign/axis conventions, RPM selector, TCU mode/pressure redesign, added/relocated calibration fields, and output/sensor layout changes. Neither the checked-in generated files nor a successful subset of host tests would prove tune migration, flash persistence, all-board fit, external ETB behavior or physical crank phase.

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
