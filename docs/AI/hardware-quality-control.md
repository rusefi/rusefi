# Hardware Quality Control & Direct I/O Access

This document catalogs every mechanism for exercising ECU hardware outside of normal engine
operation: bench testing of outputs, direct pin manipulation, the CAN-based QC protocol used by
factory test rigs, smart-driver diagnostics, trigger self-stimulation, and ETB test/calibration
routines. Primary code: `firmware/controllers/bench_test.cpp`,
`firmware/controllers/can/can_bench_test.cpp`, `firmware/controllers/settings.cpp`.

## Layers of output control, safest to most direct

1. **Bench test** (`bench_test.cpp`) - pulses a *configured* `OutputPin` through the normal
   `OutputPin::setValue` path with precise scheduler timing. Respects pin mode
   (open-drain/inverted), reads smart-driver diagnostics afterwards. Refused while engine is
   running (for injector/spark/solenoid/Lua categories).
2. **QC direct pin control** (`can_bench_test.cpp`) - raw `palWritePad`/`gpiochips_writePad` on
   outputs enumerated by board metadata, driven over CAN by an external test rig ("stim" device).
   Engages a sticky `qcDirectPinControlMode` that suppresses all normal firmware writes to pins;
   only a reboot exits this mode.
3. **Desperate-debugging console commands** (`settings.cpp`) - `bench_setpin`/`bench_clearpin`
   force pad mode and write any pin by name at the lowest level, ignoring ownership and QC mode
   entirely.

## 1. Bench test subsystem (`firmware/controllers/bench_test.cpp`)

### Execution model

All bench pulses funnel through `pinbench()` -> `BenchController` thread (`PRIO_BENCH_TEST`,
woken via counting semaphore). `runBench()` loops `count` times: schedules a `benchOn` action
+50us in the future and a `benchOff` action `onTimeMs` later on the main single-shot scheduler
(`engine->scheduler`), then sleeps one full cycle. This gives hardware-timer-accurate pulse
widths, suitable for measuring injectors/coils with a scope. `onTime` above
`TOO_FAR_INTO_FUTURE_US` is rejected with `firmwareError`.

Key behaviors:

- While a pin is under bench test, `getOutputOnTheBenchTest()` returns it and
  `OutputPin::setValue` (`firmware/controllers/system/efi_gpio.cpp`, see `isForce` check) drops
  all *non-forced* writes to that pin - normal firmware logic cannot fight the bench pulse.
  `benchOn`/`benchOff` themselves pass `isForce = true`.
- `benchOff()` reads the smart-driver diagnostic (`output->getDiag()`) after each pulse and
  prints it via `pinDiag2string` - so a bench pulse on a TLE8888/MC33810/etc. channel doubles as
  an open-load / short-circuit check.
- `mainRelayBench()` uses `swapOnOff = true` (pulse is OFF-then-ON, since main relay is normally
  energized).
- `BENCH_CANCEL` / `cancelBenchTest()` clears `isRunningBench` to abort a long loop.
- Progress is reported to TunerStudio via `engine->outputChannels.testBenchIter`.
- Default pulse parameters live in config: `benchTestOnTime` (default 4 ms), `benchTestOffTime`
  (default 500 ms), `benchTestCount` (default 3) - defaults enforced by
  `onConfigurationChangeBenchTest()`.

### What can be pulsed

Injectors and coils (per-cylinder), TCU solenoids, Lua output pins, fuel pump relay, main
relay, starter relay, AC relay, fan 1/2, check-engine light (MIL), HPFP valve, boost valve,
VVT valves 0-3 (`EFI_VVT_PID`), Harley ACR 1/2 (`EFI_HD_ACR`), idle valve
(`startIdleBench()`, `EFI_IDLE_CONTROL`). `bench_mode_e` in
`firmware/controllers/algo/engine_types.h` is the wire enum used to select these over CAN.

### Console commands (registered in `initBenchTest()`)

`fuelpumpbench`, `fuelpumpbench2 <ms>`, `fuelbench <on> <off> <count>`,
`fuelbench2 <cyl> <on> <off> <count>`, `sparkbench`, `sparkbench2`, `tcusolbench`,
`acrelaybench`, `fanbench`, `fanbench2`, `fan2bench`, `mainrelaybench`, `starterbench`,
`milbench`, `hpfpbench`, `boostbench`; wideband helpers `update_wideband`,
`set_wideband_index`, `ping_wideband`.

### TunerStudio command dispatch: `executeTSCommand(subsystem, index)`

This is the single entry point for all TS "controller command" buttons, and it is also
reachable over CAN (see section 3). Subsystems (`ts_command_e`):

- `TS_IGNITION_CATEGORY` / `TS_INJECTOR_CATEGORY` / `TS_SOLENOID_CATEGORY` /
  `TS_LUA_OUTPUT_CATEGORY` - per-index bench pulse, **only if RPM is stopped**.
- `TS_BENCH_CATEGORY` -> `handleBenchCategory(index)` - the `bench_mode_e` list above, plus
  LTFT actions (`LTFT_RESET`, `LTFT_APPLY_TO_VE`, `LTFT_DEV_POKE`) and TS Lua buttons
  `LUA_COMMAND_1..4` which just increment `luaCommandCounters[]` (a Lua script polls them via
  `getTsButtonCount(n)`).
- `TS_X14` -> `handleCommandX14(index)` - grab-bag: TPS/pedal calibration grabs, ETB
  autocal/bench/autotune (section 7), `TS_RESET_TLE8888` / `TS_RESET_MC33810` smart-driver
  re-init, trigger stimulator enable/disable (section 6), SD card mode switches, write flash,
  start/stop engine button, wideband firmware update, torque table estimation.
- `TS_WIDEBAND*` - rusEFI wideband CAN index/sensor-type assignment, ping, firmware flash,
  restart.
- `TS_SET_ENGINE_TYPE` / `TS_SET_DEFAULT_ENGINE` - apply engine preset.
- `TS_STOP_ENGINE`, `REBOOT_COMMAND`, `JUMP_DFU_COMMAND`, `JUMP_BLT_COMMAND`.
- `TS_BOARD_ACTION` - board-specific hook via `custom_board_ts_command` override.

## 2. Direct pin access console commands (`firmware/controllers/settings.cpp`)

For "desperate time debugging or hardware validation" - these bypass everything:

- `readpin <pin>` - raw `palReadPad`, prints physical level. Works on any on-chip pin name
  (`PA0`, etc.).
- `bench_setpin <pin>` / `bench_clearpin <pin>` - forces the pad to push-pull output via
  `efiSetPadModeWithoutOwnershipAcquisition` and writes it with raw `palWritePad`, explicitly
  ignoring pin ownership *and* `qcDirectPinControlMode`; echoes the readback.
- `bench_set_output_mode <pin>` - just forces push-pull pad mode.
- `hw_qc_mode` - manually engages QC direct control mode (section 3).
- `qc_output <index>` / `qc_etb <index> <direction>` (registered by `initQcBenchControls()` in
  `can_bench_test.cpp`) - console equivalents of the CAN QC commands, indexing board meta
  outputs rather than pin names.
- Related: `pins` (full pin repository + diagnostics dump, section 5), `adc_report`,
  `adc <idx>`, `adcdebug 1`, `show_sensors` / `set_sensor_mock`, `set_trigger_simulator_pin`,
  and the `set_*_pin` family for live pin reassignment.

## 3. CAN QC protocol - factory test rig ("stim") support

Base address `BENCH_TEST_BASE_ADDRESS = 0x770000`, extended CAN IDs, enums in
`firmware/libfirmware/can/can_common.h` (`bench_test_packet_ids_e`, `bench_test_io_control_e`,
magic header byte `0x66`). Primary consumer is the rusEFI QC stimulation board:
https://github.com/rusefi/rusefi-hardware `digital-inputs/firmware` (the "stim" device). Both
handlers are called unconditionally from the CAN RX path (`can_rx.cpp`); gating is by CAN ID +
magic byte.

### QC direct control mode (`qcDirectPinControlMode`)

Any valid `HW_QC_IO_CONTROL` (0x770002) frame calls `setHwQcMode()`:

- Sticky until reboot - "should never be engaged on a real vehicle".
- `OutputPin::setValue` silently drops **all** non-forced writes to **every** pin
  (`efi_gpio.cpp`) - firmware logic is disconnected from the outputs so the rig has exclusive
  control.
- On Hellen boards, powers up the board via `hellenEnableEn("HW QC")` if needed.
- Forces the QC telemetry broadcast on (`can_tx.cpp`: `enableExtendedCanBroadcast ||
  isHwQcMode()`).

### Rig -> ECU commands (`processCanQcBenchTest`, ID 0x770002, `data[0]=0x66`, `data[1]=cmd`)

- `CAN_BENCH_GET_COUNT` - reply with board meta counts (`IO_META_INFO`).
- `CAN_QC_OUTPUT_CONTROL_SET` / `_CLEAR` - direct write of output number `data[2]` from the
  board meta output array (raw pad write, on-chip or `gpiochips_writePad` for smart-driver
  pins; pad mode forced if the pin is unowned).
- `CAN_QC_ETB` - `qcSetEtbState(dcIndex, direction)`: raw drive of the DC motor H-bridge pins
  (`etbIo[]` control/direction/disable), handling both two-wire (TLE7209/L6205) and
  direction+disable (TLE9201/VNH2SP30) wiring.
- `CAN_BENCH_SET_ENGINE_TYPE` - apply preset `data[2]` and reboot.
- `CAN_BENCH_EXECUTE_BENCH_TEST` - run any `bench_mode_e` bench function.
- `CAN_BENCH_START_PIN_TEST` / `CAN_BENCH_QUERY_PIN_STATE` / `CAN_BENCH_END_PIN_TEST` - reset /
  query pin toggle statistics (`PIN_STATE` reply: toggle counter + per-state durations).
  Toggle stats are collected only in simulator builds (`OutputPin::pinToggleCounter` /
  `durationsInStateMs`, saved after a bench run by `getSavedBenchTestPinStates`).

### ECU -> rig telemetry (broadcast when QC mode or `enableExtendedCanBroadcast`)

`EVENT_COUNTERS` (trigger primary/secondary edge counts, 4x VVT, VSS - lets the rig verify
digital inputs by counting stimulus edges), `BUTTON_COUNTERS` (brake/clutch/AC switch
counters), `AUX_DIGITAL_COUNTERS` (Lua digital inputs), `RAW_ANALOG_1`/`RAW_ANALOG_2`/
`RAW_LUA_ANALOG_1` (raw 0-5V sensor voltages packed one byte per channel - TPS, PPS, MAP,
CLT, IAT, VBatt, aux analogs...), `BOARD_STATUS` (Hellen board ID, uptime, engine type),
`IO_META_INFO`, `MANUAL_PIN_TEST` (echo for `qc_output`).

### User control over CAN (`processCanEcuControl`, non-QC)

`ECU_CAN_BUS_USER_CONTROL` (0x77000C, `data[0]=0x66`) unpacks subsystem/index and calls
`executeTSCommand` - the *entire* TS bench/command surface is drivable over CAN, e.g.
`0x77000C 0x66 0x00 0x14 0x00 0x09 0x00` = start/stop engine. Also here:
`ECU_REQ_CALIBRATION` - query any scalar calibration by field hash (see `fields_api.txt`),
reply in `ECU_GET_CALIBRATION`; `DASH_ALIVE` -> `ECU_IMAGE_INFO`. There is also an ISO-TP
wrapper of the full TS protocol over CAN (see `can_common.h` `ECU_ISO_TP_SETTINGS`).

### Board metadata (`getBoardMetaOutputs` and friends)

Declared in `engine_configuration.h`, weak defaults in `engine_configuration.cpp` (0 outputs,
all-low-side). Each QC-supported board (`proteus`, `microrusefi`, most `hellen/*` boards)
provides an ordered `Gpio` array of its physical outputs plus `getBoardMetaOutputsCount()`,
`getBoardMetaLowSideOutputsCount()`, `getBoardMetaDcOutputsCount()` in its
`board_configuration.cpp`. The rig addresses outputs by ordinal, so it needs no per-board pin
knowledge - it reads the counts from `IO_META_INFO`.

### QC engine presets

`engine_type_e` includes rig-oriented presets: `PROTEUS_STIM_QC = 73`,
`HELLEN_4CHAN_STIM_QC = 74`, `HELLEN_2CHAN_STIM_QC = 75` (`custom_engine.cpp` /
`hellen board` code). These map trigger/cam/VSS/button inputs and analog channels onto the
stim harness wiring so input counters and raw analog telemetry line up with what the rig
generates.

## 4. Test-rig hardware note

The QC fixture ("stim" board) generates trigger/analog/digital stimulus into the ECU harness
connector and observes ECU outputs; it drives the CAN protocol above. See
https://github.com/rusefi/rusefi-hardware (digital-inputs firmware) and the wiki CAN page.
`FW_WIPE_OUT` and `HW_QC_TESTER_PRESENT` CAN IDs are reserved for the tester/OpenBLT side -
firmware currently neither sends nor handles them.

## 5. Smart output-driver diagnostics

`brain_pin_diag_e` (`rusefi_enums.h`) is a bitmask: `PIN_OPEN`, `PIN_SHORT_TO_GND`,
`PIN_SHORT_TO_BAT`, `PIN_OVERLOAD`, `PIN_DRIVER_OVERTEMP`, `PIN_DRIVER_OFF`, or
`PIN_UNKNOWN` (no diagnostics - plain on-chip GPIO).

- `OutputPin::getDiag()` -> `gpiochips_getDiag()` -> per-chip `getDiag()` implementations:
  TLE8888, MC33810 (incl. ignition-specific faults: max-dwell/overcurrent -> `PIN_OVERLOAD`,
  no/short spark duration -> `PIN_OPEN`), TLE6240, L9779, TLE9104, MC33972 (inputs), DRV8860,
  MSIO-box CAN GPIO (`firmware/hw_layer/drivers/gpio/`).
- **`pins` console command** (`pin_repository.cpp` `reportPins()`) lists every pin with owner,
  and for every smart-driver pin prints `diagnostic: <open_load|short_to_gnd|...>` via
  `pinDiag2string`, then dumps per-chip debug counters (`gpiochips_debug()` - resets,
  overvoltage/undervoltage events, watchdog counters).
- **Continuous monitoring**: `sensor_checker.cpp` polls injector and coil diagnostics, raises
  OBD codes (`getCodeForInjector`/`getCodeForIgnition`) and publishes per-channel
  `injectorDiagnostic[]` / `ignitorDiagnostic[]` TS output channels; a broken injector also
  feeds fueling compensation (`brokenInjector`).
- **Bench integration**: every bench pulse prints the pin's diag on turn-off (section 1).
- Chip maintenance commands: `tle8888` (register dump), `tle8888init`, `reset_9201`,
  `msioinfo`; TS buttons `TS_RESET_TLE8888` / `TS_RESET_MC33810`.

## 6. Trigger stimulator / self-stimulation (`trigger_emulator_algo.cpp`)

Lets the ECU generate its own crank/cam waveform to validate the whole trigger decode +
scheduling chain without an engine:

- `enableTriggerStimulator()` - **direct self-stimulation**: the emulator PWM callback feeds
  `handleShaftSignal`/`handleVvtCamSignal` in software (`directSelfStimulation = true`), no
  wiring needed. Trigger hardware inputs are ignored while active. Also bypasses the
  ignition-voltage limp condition so an ECU on a USB cable can "run".
- `enableExternalTriggerStimulator()` - drives real waveforms on
  `triggerSimulatorPins[]`/`camSimulatorPin` for external loop-back wiring into the trigger
  inputs (or into another ECU) - this exercises the input capture hardware too.
- `disableTriggerStimulator()` stops both.
- Controlled by TS X14 buttons (`TS_TRIGGER_STIMULATOR_ENABLE`/`_DISABLE`/
  `TS_EXTERNAL_TRIGGER_STIMULATOR_ENABLE`, requires `EFI_EMULATE_POSITION_SENSORS`), console
  `rpm <n>` (set emulated RPM), Lua `selfStimulateRPM(rpm)` (rpm < 1 disables), and the
  simulator always runs with self-stim on.
- `triggerinfo` console command reports `self=` state and simulator pins.

## 7. Electronic throttle: bench test, autocal, autotune (`electronic_throttle*.cpp/h`)

- **ETB bench test** (`etbBenchTestStart(idx)`, TS `TS_ETB_BENCH_TEST_0/1`): open-loop
  `motor->set(0.5)` push for 300 ms, then disable - a quick "does the throttle blade move"
  check. Normal closed-loop update is suspended during the test.
- **TPS autocal** (`etbAutocal(function)`, TS `TS_ETB_AUTOCAL_*`, `TS_EWG_AUTOCAL_*`, console
  `etbautocal`): refuses while RPM > 0; drives the throttle fully open (+50% duty, 1 s),
  records primary/secondary raw voltages, then fully closed (-50%, 1 s), records minimums.
  Sanity check: < 0.5 V travel -> `firmwareError` "check your wiring". The `_FAST` variants
  write `tpsMin/tpsMax/...` directly into config; the normal variants report values to
  TunerStudio's calibration channel for user confirmation. Works for Throttle1, Throttle2 and
  Wastegate.
- **PID autotune** (`TS_ETB_START_AUTOTUNE`/`_STOP_AUTOTUNE`) and jam-protection disable
  (`TS_ETB_DISABLE_JAM_DETECT`) round out the ETB test surface.
- QC rigs bypass all of this with raw H-bridge control via `CAN_QC_ETB` / `qc_etb` (section 3).
- TPS/pedal endpoint "grab" commands (`TS_GRAB_TPS_CLOSED/OPEN`, `TS_GRAB_PEDAL_UP/WOT`)
  capture current ADC readings as calibration endpoints.

## 8. Simulator/unit-test observability

- `OutputPin::pinToggleCounter` counts logic transitions in unit-test and simulator builds;
  simulator builds additionally track `durationsInStateMs[2]` - queried over CAN
  (`CAN_BENCH_QUERY_PIN_STATE`) and used by simulator-based QC of the bench logic itself
  (`getSavedBenchTestPinStates`).
- Simulator runs bench iterations with a floor of 2 (`pinbench` clamps `count`).

## Quick reference: which tool for which job

| Goal | Mechanism |
|------|-----------|
| Verify an injector/coil/relay clicks with exact timing | bench test (TS buttons or `fuelbench`/`sparkbench`/...) |
| Detect open load / short on a smart-driver output | bench pulse diag print, `pins` command, sensor_checker OBD codes |
| Factory end-of-line test of every I/O | CAN QC protocol + stim rig, `STIM_QC` engine presets |
| Poke an arbitrary pin with no config | `bench_setpin` / `bench_clearpin` / `readpin` |
| Validate trigger decode without an engine | trigger self-stimulation (`rpm` command / TS stimulator buttons) |
| Verify ETB wiring and calibrate TPS range | ETB bench test + autocal |
| Drive bench functions from a dash/tester without TS | `ECU_CAN_BUS_USER_CONTROL` CAN frames |
