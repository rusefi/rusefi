# Levin

STM32F407VGT6, 8 MHz HSE. Pro 2.3 PCB wiring with Pro 2.4 connector names.
The existing Lite SD routing differs and is not covered by this target.

## Firmware variants

From `firmware/`, using the standard rusEFI build environment:

```sh
# Native USB tuning
bash bin/compile.sh config/boards/levin/meta-info-levin_native.env

# Clean when changing variants, then build CH340/Bluetooth + native USB
make clean
bash bin/compile.sh config/boards/levin/meta-info.env
```

The targets are `levin_native` and `levin`. Both are discovered by the normal
firmware build matrix. Official publishing jobs also upload the corresponding
INI by its firmware signature. Use the INI from the same build as the firmware.

`levin` enables USART1 on PA9 (TX) / PA10 (RX), at 115200 baud for CH340 or
Bluetooth. The bridges share a UART; use one host connection at a time.
Native USB uses PA11/PA12 and remains available in both builds. OpenBLT firmware
updates use native USB, not CH340. No additional UART bootloader is provided.

## Internal wiring

These are physical PCB connections, restored by the board configuration hook
and hidden from TunerStudio pin selectors:

| Function | Pins |
| --- | --- |
| DRV8825 direction / step / enable | PB10 / PB11 / PA15 |
| CAN RX / TX | PD0 / PD1 |
| SD SPI3 clock / MISO / MOSI / CS | PC10 / PC11 / PC12 / PD2 |
| Battery voltage | PA4 |
| Bosch SMD284 barometer | PA5 |

SD is fitted; there is no external flash. Stepper direction, travel, operating
mode and CAN protocol remain configurable. Direction defaults to inverted,
and stepper enable uses normal output mode.

External connector pins remain assignable to compatible functions, including
CRANK, CAM and pulse inputs. Labels such as `PWM OUT 1` identify the physical
channel. Normal pin ownership checks still apply: release a pin's old function
before assigning it to another. Connector YAMLs list the MCU connections.

## Defaults and calibration

- Eight injector and eight ignition assignments, with MINIMAL_PINS as the default
  engine type. This is a board definition, not a Golf or LT1 engine tune.
- IAT PA0 and CLT PA1 use 2490 ohm bias resistors. Calibration remains editable.
- TPS PA2, MAP PA3, wideband analog PB0; spare analog PC2, PC0, PC1.
- ADC reference 3.3 V, analog divider 1.5151515, battery divider 7.0.
- SMD284 transfer: Vout/Vdd = 0.007895 * pressure (kPa), represented by a custom
  0..126.662 kPa calibration at 0..5 V. Barometer calibration is independent of MAP.
- Crank PD3 and cam PD4 are defaults only. Choosing another trigger input is allowed.
- Output polarity and sensor scaling must be verified on the actual board.

## Validation

The owner reports that a native USB build of this board configuration based on
release_20260812 works on the ECU. That is not a hardware test of current master
or of every peripheral. CH340/Bluetooth, SD, CAN, stepper, all output polarities,
ADC scaling, tune persistence and engine timing remain bench-validation items.

Board defaults and fixed-wiring behavior are covered by `LevinBoard.*` host tests.
After INI generation, check labels, enum positions and internal-pin visibility:

```sh
python3 config/boards/levin/tests/check_ini.py tunerstudio/generated/rusefi_levin.ini
```
