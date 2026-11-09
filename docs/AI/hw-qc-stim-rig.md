# HW QC Stim Rig (tester side)

The end-of-line test fixture ("stim" board) runs its own ChibiOS firmware from the
https://github.com/rusefi/rusefi-hardware repository, `digital-inputs/firmware/`. This page
covers the tester and how its tables must agree with the ECU's board metadata. The ECU side
of the protocol (`can_bench_test.cpp`, `qcDirectPinControlMode`, `getBoardMeta*`) is covered in
[Hardware Quality Control](hardware-quality-control.md), section 3.

## Hardware and build

- MCU: STM32 Nucleo-F429ZI (`ST_NUCLEO144_F429ZI` ChibiOS board), mounted on the stim PCB
  (https://github.com/rusefi/stim, interactive pinout https://rusefi.com/docs/pinouts/stim/).
- Results go to USB CDC serial with ANSI colours, plus LEDs: green = last cycle passed,
  red = failure, blue blinks continuously.
- CAN1 on PD0/PD1, 500 kbit/s, extended IDs. The tester holding `LINE_BUTTON` at power-up
  selects **listen mode**: it sends nothing except the periodic count request and runs no tests.
- The tester uses `ext/libfirmware` (a submodule, updated by that repo's
  `update-libfirmware-reference` workflow) for `can/can_common.h` (`bench_test_packet_ids_e`,
  `bench_test_io_control_e`, `BENCH_HEADER`) and `board_id/` (board ID dictionary and
  `qc_stim_meta.h`). rusEFI includes the same library as `firmware/libfirmware`. A protocol or
  board ID change takes effect only after both repositories reference the new library.
- `qc_stim_meta.h` defines `libPROTEUS_STIM_QC`, `libHELLEN_4CHAN_STIM_QC` and
  `libHELLEN_2CHAN_STIM_QC`. `engine_type_impl.cpp` static-asserts that they equal the
  `engine_type_e` values, so the tester can request a preset by number.

## Test cycle (`main.cpp` `ConsoleThread`)

`main()` sends `CAN_BENCH_GET_COUNT` every 500 ms. Any valid `HW_QC_IO_CONTROL` frame puts the
ECU into QC mode, which turns on the QC telemetry broadcast. These requests therefore also
wake the ECU and start its telemetry. Each cycle runs:

1. `startNewCanTest()` forgets the detected board, the meta counts, the counters and WBO
   status. Every cycle re-detects the board, so the next ECU can be connected without a
   tester reset.
2. `testEcuDigitalOutputs()` waits up to 5 s for both `BOARD_STATUS` and `IO_META_INFO`,
   then toggles each output over CAN and reads it back through the analog mux (below).
3. `stimulateEcuDigitalInputs()` toggles eight stimulus lines (PE0, PE7..PE13) ten times at
   50 ms intervals. `checkDigitalInputCounterStatus()` then checks the counters.
4. `testEcuDcOutputs()` checks each H-bridge.
5. The cycle passes only if every stage passes, the CAN checks pass (`isHappyCanTest`) and
   the ECU uptime in `BOARD_STATUS` exceeds 10 s. The uptime check catches an ECU that reset
   during the cycle. The tester then waits 5 s and repeats.

## Board detection and expectation table (`test_logic.cpp`)

`boardConfigs[]` contains one `BoardConfig` per production board. Some live in
`test_logic_harley.cpp` and `test_logic_nissan.cpp`. `receiveBoardStatus()` matches the
`BOARD_STATUS` board ID (`getBoardId()`: Hellen resistor ID or `STATIC_BOARD_ID_*`) against
each entry's zero-terminated `boardIds[]`. An unknown ID fails the cycle. If
`desiredEngineConfig != -1` and the reported engine type differs, the tester sends
`CAN_BENCH_SET_ENGINE_TYPE`; the ECU applies the preset and reboots. That cycle fails
(uptime) and the next one tests with the QC preset.

`BoardConfig` fields:

| Field | Meaning |
|-------|---------|
| `channels[32]` | Expected raw voltages: `RAW_ANALOG_1` -> 0..7, `RAW_ANALOG_2` -> 8..15, `RAW_LUA_ANALOG_1` -> 16..23, `RAW_LUA_ANALOG_2` -> 24..31. `nullptr` name = not checked. |
| `eventExpected[7]` | Primary, secondary, VVT1..4, VSS counters in `EVENT_COUNTERS` that must change |
| `buttonExpected[3]` | Brake, clutch-up, A/C counters (`BUTTON_COUNTERS`) |
| `auxDigitalExpected[8]` | Lua digital input counters (`AUX_DIGITAL_COUNTERS`) |
| `outputNames[]` | Labels for the output ordinals, used only in reports |
| `wboUnitsCount`, `wboStartIndex` | Number of on-board rusEFI wideband controllers that must send `WB_DATA_BASE_ADDR` frames, and their index offset |
| `highSideStartingIndex` | Stim test pad for the first high-side output (0 = high sides follow the low sides) |
| `dcHackValue` | Polarity flip for the DC-output check (per board, empirical) |

### Analog check

The ECU packs each channel as `RAW_TO_BYTE(v) = v * 255 / 5` of the raw sensor voltage, which
is ADC pin voltage, before sensor conversion (`sendQcBenchRawAnalogValues`). The tester computes
`5 * b / 255 * mulCoef` and requires it within `[acceptMin, acceptMax]`. The stim board
supplies fixed voltages through resistor dividers (`UP_xx`/`DOWN_xx`, `VOLT_xx` in
`test_logic_config.h`, named after stim connector pins). `PULLED_UP_RANGE`/`PULLED_DOWN_RANGE`
account for the ECU's own input pull resistor in parallel. `CLT_VALUE`/`IAT_VALUE` model the
ECU's thermistor bias resistor (`HELLEN_R` 4.7 k, `ALPHA2CH_R`/`PROTEUS_R` 2.7 k). For BATT,
`mulCoef` is the board's VBatt divider (`HELLEN_VBATT_MULT` must match
`hellen_common.cpp`), and the range is 9-15 V. A board input divider change in rusEFI requires
a matching update here. Channel order follows the `values_1`/`values_2`/`lua_values_1` arrays
in `can_bench_test.cpp`. Reordering those arrays breaks every tester table.

### Counters

A counter passes when it reports two different non-zero values within one cycle
(`Counter::applyNewValue`). Counters are byte-truncated, so wrap-around is harmless. An
expected input that stays at zero fails as `ZERO ... counter`. The ECU must map that input to
a pin wired to a stim line. This is what the `*_STIM_QC` presets or the board defaults
provide.

## Output test: the ordinal contract with `getBoardMeta*`

The tester has no ECU pin knowledge. It addresses outputs only by **ordinal** into the ECU's
`getBoardMetaOutputs()` array (the board's `custom_board_getMetaOutputs` callback, typically a
`static Gpio OUTPUTS[]` in `board_configuration.cpp`):

- `IO_META_INFO`: `[2]` total outputs, `[3]` low-side count, `[4]` DC count.
- The first `lowSideCount` ordinals are tested as low side, the rest as high side. Board
  arrays must list low sides first. The default low-side count equals the total.
- Ordinal *i* is checked on stim test pad *i*, or for high sides
  `highSideStartingIndex + (i - lowSideCount)` when that index is set. The ECU array order
  must therefore match the order in which the board's QC harness wires outputs to stim pads,
  as well as the tester's `outputNames[]`.
- The test pads form three 16-channel CD4067 mux banks (pad = bank * 16 + address; ADDR pins
  PC8/PC6/PC5/PA6, inverted by `XOR_MAGIC`; mux off PA7), 48 pads (`COUNT`). Each bank has an
  ADC input (PF4/PF5/PF3, ADC3) and a pull-up/pull-down control line (PD8, PB11, PC7; PC4 for
  pad 49).
- Each output gets four cycles of `CAN_QC_OUTPUT_CONTROL_SET`/`_CLEAR` (inverted for low
  side), a settle delay (2 ms; 100 ms for `STATIC_BOARD_ID_UAEFU121_SBC`, whose relays are
  mechanical), then an ADC read: above 0.7 V = high. Every cycle must match.
- An ECU-reported count above the tester's name table only loses the label
  (`getOutputName` returns null). An index of 48 or more is refused. On the ECU side, an
  ordinal at or above `getBoardMetaOutputsCount()` raises `criticalError("QC pin index ...")`.

A wrong `OUTPUTS[]` order, or a low-side count that does not match the array, shows up as BAD
channels on the rig, not as an ECU build or unit-test failure. Review the board's QC harness
and the matching `BoardConfig` when changing either.

### DC outputs

For each DC index *d* < DC count, the tester drives `CAN_QC_ETB` (direction on/off) and checks
the two motor terminals on pads `39 - 2d` and `38 - 2d` (`LAST_DIGITAL_PIN`). It expects
opposite levels on the two pads, and the polarity changes with `dcHackValue`. Boards report a
DC count of 0 when the selected preset uses the H-bridge pins for something else (for example
uaefi's `boardGetMetaDcOutputsCount` for some engine types).

## Adding a board to QC

1. ECU (rusEFI): implement `custom_board_getMetaOutputs`/`OutputsCount`/`LowSideOutputsCount`/
   `DcOutputsCount` in the board's `setup_custom_board_overrides()`, with low sides first, in
   harness order.
2. Make sure the board reports a unique board ID (Hellen ID resistors or a
   `STATIC_BOARD_ID_*` in libfirmware `boards_dictionary.h`).
3. Tester (rusefi-hardware): add a `BoardConfig` with the board IDs, expected analog ranges
   derived from the board's input schematic, expected counters, output names in the same
   order, and WBO count. Set `desiredEngineConfig` if the board defaults do not route inputs
   to the stim lines.
