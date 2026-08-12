# super-uaefi schematic — text extraction & firmware relevance notes

Source: `docs/super-uaefi-c-schematic.pdf` (28 pages, A2, KiCad 8.0.9 output, Rev **c**, dated 2026-04-29).

This document repeats the extraction process first developed for
[fw-uaefi-Honda-OBD1](https://github.com/rusefi/fw-uaefi-Honda-OBD1) (`docs/schematic-notes.md`
in that repo). A dedicated comparison section at the end articulates the similarities and
differences between the two boards.

## How this file was produced (repeatable process)

### Toolset

Only **poppler-utils** is required (`pdftotext`, `pdfinfo`, `pdffonts`) plus standard shell
tools (`grep`, `tr`, `sed`, `wc`):

```bash
sudo apt-get install poppler-utils     # Debian/Ubuntu/WSL
# brew install poppler                 # macOS
```

The PDF is vector KiCad output (assembled by PyPDF2), so all labels are real text —
**no OCR, no page rendering, no Python PDF libraries needed**. Verify with `pdffonts`:
fonts are listed, therefore text extraction works. (No fonts listed = image-only PDF,
and this recipe does not apply.)

### Step 1 — survey the document

```bash
cd firmware/config/boards/hellen/super-uaefi
PDF=docs/super-uaefi-c-schematic.pdf
pdfinfo $PDF                                   # 28 pages, A2, PyPDF2 producer
pdftotext -layout $PDF /tmp/schematic.txt      # full text, preserving 2D layout
wc -l /tmp/schematic.txt                       # ~3600 lines
```

`-layout` keeps the spatial arrangement, which is essential for schematics: a net label
and its connector pin number stay on the same output line.

### Step 2 — split per page and classify by title block

```bash
for i in $(seq 1 $(pdfinfo $PDF | awk '/^Pages/{print $2}')); do
  pdftotext -layout -f $i -l $i $PDF /tmp/page$i.txt
done

# KiCad title blocks identify most sheets
for i in /tmp/page*.txt; do
  echo "=== $i ==="
  grep -iE "Sheet: /|Title:|Module:|Id: [0-9]+/" $i | head -5
done

# fingerprint ambiguous pages (15-27 have no extractable title block) by part numbers
grep -oiE "STM32[A-Z0-9]+|MAX99[0-9]+|LIS[0-9A-Z]+|TJA[0-9]+|MPX[H0-9]+|TLE[0-9]+|VNLD[0-9]+|ISL9V[0-9]+|MAX318[0-9]+" \
  /tmp/pageN.txt | sort | uniq -c
```

### Step 3 — extract the pin mapping from the top-level page

As on the Honda OBD1 board, page 1 carries everything firmware needs: connector-pin →
signal rows and signal → MCU pin labels like `OUT_INJ1_(PD3)`, `IN_TPS_(PA4)`:

```bash
tr -s ' ' < /tmp/page1.txt | grep -viE '^\s*$' | less          # read the whole page (~310 lines)
grep -oE "[A-Z0-9_]+_\(P[A-H][0-9]+[^)]*\)" /tmp/page1.txt | sort -u   # all signal→STM32-pin labels
```

### Step 4 — cross-check (do not trust layout text blindly)

`pdftotext -layout` interleaves columns on dense pages, so every net↔pin association
below was verified against the firmware sources of truth:

```bash
# pin naming source of truth (three main connectors)
less connectors/60pin.yaml connectors/C-34pins-4k.yaml connectors/26pin_4k.yaml
# firmware pin assignments
less board_configuration.cpp ../uaefi121/mega-uaefi.cpp
# platform symbol → STM32 pin definitions (MM100_* and MM100_MEGA_UAEFI_* variants)
grep <SYMBOL> ../../hellen_mm100_meta.h
# knock config vs parent board
diff knock_config.h ../uaefi121/knock_config.h        # identical on this board
```

Anything where the schematic and the repo disagree is flagged in the discrepancy
section below rather than silently picked from one side.

## Page map and firmware relevance

The board is a stack: **super-uaefi carrier (superseal connectors) → mega-uaefi module →
mega-mcu100-f7 CPU module + Hellen-One function modules**. Firmware only cares about
connector-pin ↔ signal ↔ STM32 pin, plus analog conditioning and populate-options that
change firmware configuration. Module internals are fixed by the Hellen platform.

| Page | Sheet | Firmware relevance |
|---|---|---|
| **1** | **Top level `super-uaefi`** (super-uaefi.kicad_sch, Id 1/1) | **ESSENTIAL — the only page needed for pin mapping.** 60-pin (A/B), 34-pin (C), 26-pin (D) superseal connectors → signal, and signal → STM32 pin for the whole mcu100-f7 module. Also carrier-local hardware: BTS6143D high-side driver (20D), extra VNLD5160 for INJ7/INJ8, USB, power. All tables below come from here. |
| **2** | mega-uaefi module top (`mega-uaefi`, Id 1/7) | High — routes module signals to submodules; shows which LS/IGN/INJ channel goes through which driver. |
| 3 | /LS/ — low-side drivers | Low — VNLD5160 smart low-side driver internals (OUT_LS1–LS4). |
| 4 | /LS HOT/ — always-hot low side | Low — same driver for OUT_LS5_HOT (main relay) / OUT_LS6_HOT (fan relay). |
| 5 | /MAP/ — onboard MAP option | Medium — MPXH6400 onboard MAP sensor populate-option (affects MAP sensor type calibration; "On-board MAP" pin in `60pin.yaml`). |
| 6 | /Ignition/ | **Medium-high** — ignition outputs are **logic-level (smart coil) by default**; option: remove R870–R872/R876–R878 and populate Q841–Q846 (ISL9V3040 IGBTs) for dumb coils. Changes ignition mode / dwell settings. |
| 7 | /EGT/ | Medium — MAX31855 thermocouple interface on SPI3, **populated and enabled by default on this board** (27C/28C EGT± pins; `EFI_MAX_31855=TRUE` in `board.mk`). |
| 8 | /Injector/ | Low — 3× VNLD5160 = 6 injector channels, driver internals only. |
| 9 | WBO (wideband) module | **Not main-firmware** — STM32F042K6 running separate rusEFI WBO firmware, CAN-attached. This board carries **two** of these modules (WBO1 + WBO2). |
| 10 | Power module (12 V/5 V, main relay, VBAT) | Not relevant (only IN_VIGN key-voltage sense reaches firmware). |
| 11 | Hellen-One Knock module | Medium — op-amp conditioning feeding IN_KNOCK ADC (PA2). |
| 12 | VR-discrete module ("outputs a pulse on rising edge") | Medium — conditions one VR input into a discrete pulse (VR_DISCRETE± on 12D/13D). |
| 13 | VR MAX9924 module | Medium — conditions the other VR input (VR_9924± on 10D/11D); R843 populate-option turns VR− into a Hall input. Firmware default trigger input. |
| 14 | Motor-driver module (TLE9201SG) | Medium — ETB H-bridge, DIR+PWM control (2 channels: DC1, DC2 on 4D–7D). |
| 15–27 | mega-mcu100-f7 CPU-module internals — **STM32F765VGT6** pinout (p19), TJA1051 (p23), SD card, USB, LIS2DH12 accelerometer (p27), fuses | **Not needed** — fixed Hellen platform; already encoded in `hellen_mm100_meta.h`. Reference only. |
| 28 | Hellen-One CAN module (TJA1051T) | Not relevant beyond "CAN1 has an onboard transceiver". |

**TL;DR: page 1 (and to a lesser degree pages 2, 6, 13) is what firmware work needs;
pages 15–28 are platform module internals you can ignore.**

## Connector A/B — 60-pin superseal (TE 6437288-3): sensors & engine outputs

A-side (1A–34A), sensors — verified against `connectors/60pin.yaml`:

| Pin | Signal | STM32 | Pin | Signal | STM32 |
|---|---|---|---|---|---|
| 1A–4A | +5 V (HALL1/HALL2/HALL3/DIN) | — | 15A | IN_TPS2 (TPS 2) | PA1 (`MM100_IN_O2S2`) |
| 5A | IN_TPS1 | PA4 | 16A | IN_DIN (analog / digital, no pull) | PA7 (`MM100_IN_AUX3`) |
| 6A | IN_AIN2 (AUX2 analog) | PA1¹ | 17A | IN_HALL2 (cam intake) | PE13 (`MM100_IN_D2`) |
| 7A | IN_PPS2 | PC4/PE9 (`MM100_IN_AUX2`) | 18A–22A | +5 V | — |
| 8A | IN_HALL1 (VSS / HALL1) | PE12 (`MM100_IN_D1`) | 23A–34A | GNDA sensor ground | — |
| 9A | IN_HALL3 (cam exhaust, SENT-capable) | PE14 (`MM100_IN_D3`) | | | |
| 10A | +5 V | — | | | |
| 11A | IN_CLT | PC2 | | | |
| 12A | IN_IAT | PC3 | | | |
| 13A | IN_PPS1 | PA3 | | | |
| 14A | IN_AIN1 (AUX1 analog) | PB0 (`MM100_IN_AUX1`) | | | |

¹ 6A and 15A share metadata history in the YAML (`MM100_IN_O2S2_ANALOG` on 6A, TPS2 on 15A);
the YAML is authoritative for TS naming.

B-side (1B–26B), engine outputs:

| Pin | Signal | STM32 | Pin | Signal | STM32 |
|---|---|---|---|---|---|
| 1B | Coil 7 — UNUSED | — | 14B | Coil 8 — UNUSED | — |
| 2B | Coil 1 | PC13 (IGN1) | 15B | GND | — |
| 3B | Coil 2 | PE5 (IGN2) | 16B/17B | +5 V | — |
| 4B | Coil 3 | PE4 (IGN3) | 18B | INJ_8 | **PB13 (`MM100_SPI2_SCK`)²** |
| 5B | Coil 4 | PE3 (IGN4) | 19B | INJ_7 | **PB12 (`MM100_SPI2_CS`)²** |
| 6B | Coil 5 | PE2 (IGN5) | 20B | LS1 | PD15 (`MM100_INJ7`) |
| 7B | Coil 6 | PB8 (IGN6) | 21B | LS2 | PD12 (`MM100_INJ8`) |
| 8B | INJ_6 | PA8 | 22B | LS3 | PD13 (`MM100_OUT_PWM1`) |
| 9B | INJ_5 | PD2 | 23B/24B | GNDA | — |
| 10B | INJ_4 | PD10 | 25B | FLEX | PE11 (`MM100_IN_VSS`) |
| 11B | INJ_3 | PD11 | 26B | MAP | PC0 (`MM100_IN_MAP1`) |
| 12B | INJ_2 | PA9 | | | |
| 13B | INJ_1 | PD3 (`MM100_INJ1`) | | | |

² Injectors 7/8 are **not** module injector channels: they are driven by an extra
VNLD5160 (U2) on the super-uaefi carrier itself, with inputs on the MCU's SPI2_CS/SPI2_SCK
pins (page 1, section G). The module's native INJ7/INJ8 channels (PD15/PD12) are instead
routed to connector pins 20B/21B as general low-sides LS1/LS2.

## Connector C — 34-pin superseal (TE 6437288-2): power, comms, WBO2, hot outputs

| Pin | Signal | Pin | Signal |
|---|---|---|---|
| 1C | Ignition switch / VBATT sense (PA5 via divider, `MM100_IN_VBATT`) | 18C | +12 V from main relay OUTPUT |
| 2C | +12 V from main relay OUTPUT | 21C/22C | Power/chassis GND |
| 3C/4C | WBO2 SWD / SWC (programming) | 26C | +12 V Bat+ (hot at all times) |
| 5C | WBO2 Vs (Un) | 27C | EGT+ (MAX31855, SPI3) |
| 6C | WBO2 CalR (Rtrim) | 28C | EGT− |
| 7C | WBO2 Ip | 29C | LS6_HOT — fan relay (PE6, `MM100_IGN8`) |
| 8C | WBO2 Vm | 30C | LS5_HOT — main relay (PB9, `MM100_IGN7`) |
| 9C | WBO2 Heater negative | 31C | LS4 — fuel pump relay (PC6, `MM100_OUT_PWM2`) |
| 10C/11C | +12 V from main relay (input / daisy-chain) | 32C | IN_KNOCK2 (see discrepancy section) |
| 12C–14C | USB VBUS / D− / D+ | 33C/34C | — (blank) |
| 15C | PROG_BUTTON | | |

## Connector D — 26-pin (4k, TE 6473423-1 family): WBO1, ETB, triggers, buttons

| Pin | Signal | Pin | Signal |
|---|---|---|---|
| 1D | WBO1 Vs (Un) | 14D | WBO1 SWC |
| 2D | WBO1 Vm | 15D | WBO1 Ip |
| 3D | WBO1 Heater negative | 16D | GNDA |
| 4D | DC2− (ETB/stepper) | 17D | Power GND |
| 5D | DC2+ | 18D | CANL |
| 6D | DC1− (usually ETB) | 19D | CANH |
| 7D | DC1+ | 20D | **High-side output** (BTS6143D, driven by PD7 = `MM100_LED2_GREEN`) |
| 8D | WBO1 CalR | 21D | WBO1 SWD |
| 9D | KNOCK (PA2, other sensor pin to GNDA) | 22D | +12 V from main relay OUTPUT |
| 10D | VR_9924− | 23D | GNDA |
| 11D | VR_9924+ | 24D | BUTTON1 (PB1, `MM100_IN_CRANK`) |
| 12D | VR_DISCRETE− | 25D | BUTTON2 (PA6, `MM100_IN_CAM`) |
| 13D | VR_DISCRETE+ | 26D | BUTTON3 (PE15, `MM100_IN_D4`) |

## Board signal → STM32 pin (mega-mcu100-f7 module edge, from page 1)

Outputs:

| Signal | STM32 | Signal | STM32 |
|---|---|---|---|
| IGN1 | PC13 | INJ1 | **PD3** |
| IGN2 | PE5 | INJ2 | PA9 |
| IGN3 | PE4 | INJ3 | PD11 |
| IGN4 | PE3 | INJ4 | PD10 |
| IGN5 | PE2 | INJ5 | PD2 |
| IGN6 | PB8 | INJ6 | PA8 |
| IGN7 (LS5_HOT, main relay) | PB9 | INJ7 (→ 20B LS1) | PD15 |
| IGN8 (LS6_HOT, fan relay) | PE6 | INJ8 (→ 21B LS2) | PD12 |
| PWM1 (LS3) | PD13 | PWM4 (DC1 second pin) | PC8 |
| PWM2 (LS4, fuel pump) | PC6 | PWM5 (DC2 PWM) | PC9 |
| PWM3 (DC1 PWM) | PC7 | PWM6 (VR threshold) | PD14 |

Inputs & comms:

| Signal | STM32 | Signal | STM32 |
|---|---|---|---|
| UART8_TX = VR MAX9924 out (default trigger) | PE1 | IN_TPS | PA4 |
| UART8_RX = VR-discrete out | PE0 | IN_PPS | PA3 |
| IN_CRANK (BUTTON1 on this board) | PB1 | IN_KNOCK | PA2 |
| IN_CAM (BUTTON2 on this board) | PA6 | IN_CLT | PC2 |
| IN_VSS (FLEX on this board, 25B) | PE11 | IN_IAT | PC3 |
| IN_D1 (HALL1, 8A) | PE12 | IN_MAP1 | PC0 |
| IN_D2 (HALL2, 17A) | PE13 | IN_MAP2 (onboard MAP option) | PC1 |
| IN_D3 (HALL3/SENT, 9A) | PE14 | IN_O2S / CAN wakeup | PA0 |
| IN_D4 (BUTTON3 on this board) | PE15 | IN_O2S2 | PA1 |
| UART2 TX / RX | PD5 / PD6 | IN_AUX1 | PB0 |
| I2C SCL / SDA (barometer) | PB10 / PB11 | IN_AUX2 | PC4/PE9 |
| CAN1 TX / RX (onboard transceiver) | **PD1 / PD0** | IN_AUX3 | **PA7** |
| OUT_PWR_EN | PE10 | IN_AUX4 | PC5 |
| IN_VIGN (key voltage) | PA5 | SPI2_CS / SPI2_SCK (drive INJ7/INJ8) | PB12 / PB13 |
| SPI3 (MAX31855 EGT): SCK PC10, MISO PC11, CS PA15 | | SPI1 (SD + LIS2DH12 accelerometer) | onboard |

## Populate-options and notes that affect firmware configuration

* **Ignition outputs are logic-level (smart coil) by default.** For dumb coils: remove
  R870–R872, R876–R878 and populate Q841–Q846 (ISL9V3040 IGBTs, page 6). Firmware dwell
  settings must match the populated variant. (Identical option and refdes as on
  uaefi-Honda-OBD1 — same mega-uaefi module.)
* **VR MAX9924 input:** install 4.7 kΩ R843 to use VR− as a Hall input; install R770/R775
  to dampen very hot VR signals (pages 1, 13).
* **CAN wakeup:** move resistor R143 → R145 to enable CAN wakeup on PA0; the AUX1 analog
  input becomes unavailable (page 1).
* **Onboard MAP option** MPXH6400 (page 5) on IN_MAP2/PC1 — if populated, MAP sensor type
  in tune must match; external MAP signal is 26B → IN_MAP1/PC0.
* **Two autonomous WBO controllers** (each an STM32F042K6 with its own rusEFI WBO
  firmware, CAN-attached) — main firmware sees them as CAN widebands "First" and "Second"
  (`canReWidebandHwIndex_enum` in `prepend.txt`), not as analog inputs. WBO1 LSU pins on
  connector D, WBO2 LSU pins on connector C.
* **EGT**: MAX31855 on SPI3 is enabled by default in `board_configuration.cpp`
  (`is_enabled_spi_3`, CS = PA15, MOSI intentionally unset — one-way communication).
* **ETB**: two TLE9201SG H-bridges (DC1/DC2), DIR + PWM control; shared
  `setUaefiBoardDefaultETBPins()` also supports stepper-idle wiring (GM stepper comment
  in `mega-uaefi.cpp`). `prepend.txt` notes users need the ETB-pin option visible to
  un-select it when using the H-bridges for stepper idle.
* **20D high-side output**: carrier-level BTS6143D driven through an AO3400A level
  shifter by PD7 — the pin the platform calls `MM100_LED2_GREEN`. This is why
  `getRunningLedPin()` returns `Gpio::Unassigned` with the comment "this one is used to
  drive 20D".
* **PB12/PB13 dual role**: page 1 labels them `SPI2_CS_/_CAN2_RX` and
  `SPI2_SCK_/_CAN2_TX`, but on this board they drive the carrier's INJ7/INJ8 VNLD5160 —
  so CAN2 is not available (`ts_show_can2 false` in `prepend.txt`). Nets `CAN2H`/`CAN2L`
  on the mega-uaefi module edge are annotated "mm100 only", i.e. they matter only for the
  F4 (`mega_mcu100`) module variant of this platform.
* **SENT**: `EFI_SENT_SUPPORT=TRUE` in `board.mk`; 9A/HALL3 (`MM100_IN_D3`) is declared
  as both `event_inputs` and `sent_inputs` in `60pin.yaml`.
* **Default sensor/trigger wiring** (`board_configuration.cpp`): trigger =
  UART8_TX/MAX9924 ("VR2 max9924 is the safer default"), cam 1/2 = HALL1/HALL2
  (IN_D1/IN_D2), VSS = IN_D3, TPS2 = AUX1, PPS2 = AUX2, VR threshold PWM = OUT_PWM6.

## ⚠ Discrepancies / gaps found during extraction

* **IN_KNOCK2 (pin 32C) is on the schematic but absent from the firmware.** Page 1 shows
  `IN_KNOCK2` on 34-pin connector pin 32 and routed near the J1/J2 jumper blocks
  (PULL_UP/SEL/PULL_DOWN networks), but `C-34pins-4k.yaml` documents no function for 32C,
  `knock_config.h` defines only channel 1 (PA2), and no `_(Pxy)` MCU-pin label exists for
  it in the PDF text. Treat 32C as a second knock provision that is not wired into
  firmware; if it is ever enabled, the YAML, `knock_config.h` and TS options must all be
  updated together.
* **Connector-label vs platform-symbol mismatches are intentional, not errors**: 24D–26D
  BUTTONs use pins named `MM100_IN_CRANK`/`MM100_IN_CAM`/`MM100_IN_D4`; 25B FLEX uses
  `MM100_IN_VSS`; 20D high-side uses `MM100_LED2_GREEN`. The platform names describe the
  mcu100 module edge, not this carrier's function — always trust the connector YAMLs +
  `board_configuration.cpp` for the user-facing meaning.
* **No knock-pin discrepancy on this board** (unlike the Honda OBD1 repo, whose local
  `knock_config.h` was stale at PA3): here `knock_config.h` is byte-identical to
  `uaefi121/knock_config.h` and matches the schematic label `IN_KNOCK_(PA2)`.
* 33C/34C are blank in both the schematic table and the YAML (commented out) —
  consistent, just unpopulated.

## Key ICs (for reference)

| IC | Function | Page |
|---|---|---|
| **STM32F765VGT6** | Main MCU (mega-mcu100-f7 module) | 19 |
| VNLD5160TR-E | Dual smart low-side drivers (injectors, LS outputs; +1 on carrier for INJ7/8) | 1, 3, 4, 8 |
| ISL9V3040D3ST | Ignition IGBTs (dumb-coil option, DNP by default) | 6 |
| MAX9924UAUB+ | VR conditioner (default trigger input) | 13 |
| TLE9201SG | DC motor H-bridge (ETB) ×2 | 14 |
| MPXH6400 | Onboard MAP option | 5 |
| MAX31855 | EGT thermocouple interface (enabled by default) | 7 |
| STM32F042K6 | Standalone WBO controller ×2 | 9 |
| TJA1051T/3 | CAN transceiver | 23, 28 |
| LIS2DH12TR | Accelerometer (SPI1) | 27 |
| BTS6143D | High-side driver (20D output, on carrier) | 1 |

## Comparison with uaefi-Honda-OBD1

Both boards are members of the same **mega-uaefi platform**: a vehicle/package-specific
carrier board hosting the identical `mega-uaefi` engine-driver module (rev 0.5 on both)
plus a Hellen mcu100-class CPU module. `board.mk` here even compiles the shared
`../uaefi121/mega-uaefi.cpp`, and the uaefi121 sources carry a comment naming
fw-uaefi-Honda-OBD1 as a consumer of the same module code.

### Similarities

* **Same 28-page schematic structure, page for page**: top level (1), mega-uaefi top (2),
  LS (3), LS HOT (4), MAP (5), Ignition (6), EGT (7), Injector (8), WBO (9), power (10),
  knock (11), VR-discrete (12), VR MAX9924 (13), motor driver (14), CPU-module internals
  (15–27), CAN module (28). The extraction recipe transfers unchanged.
* **Same function-module inventory**: VNLD5160 low-sides, ISL9V3040 dumb-coil option with
  the *same refdes* (R870–872/R876–878 ↔ Q841–846), MAX9924 + VR-discrete trigger
  conditioners (same R843/R770/R775 options), TLE9201 ×2 for ETB, MAX31855 EGT, MPX-family
  onboard MAP option, knock module, CAN-attached STM32F042 WBO, LIS2DH12 accelerometer,
  R143→R145 CAN-wakeup option.
* **Largely identical signal → MCU pin map**: all eight IGN pins, INJ2–INJ8, all PWM pins,
  UART8_TX/RX as VR inputs ("F40x does not have UART8" on the Honda; plain EXTI use is the
  same on both), TPS/PPS/CLT/IAT/MAP/O2S analog set, IN_D1–D4, I2C, UART2, SPI3, PE10
  power-enable, PA5 VIGN, PA2 knock.
* **Same firmware skeleton**: both use `MM100_*` symbols from `hellen_mm100_meta.h`,
  Hellen board-ID machinery, `setHellenMegaEnPin()`/`setHellenVbatt()`, VR threshold on
  OUT_PWM6, SD + accelerometer on SPI1, TunerStudio over USB CDC.
* **Same repurposing tricks**: PE11 (`IN_VSS`) carries FLEX on both boards; the UART8 pin
  pair carries the VR trigger inputs on both.

### Differences

| Aspect | uaefi-Honda-OBD1 | super-uaefi |
|---|---|---|
| CPU module | mcu100 with **STM32F429VGT6** (F4) | **mega-mcu100-f7** with **STM32F765VGT6** (`PROJECT_CPU=ARCH_STM32F7`) |
| INJ1 pin | PD0 (`MM100_MEGA_UAEFI_INJ1` — F4 module shuffled pins for the CAN ST bootloader) | **PD3** (`MM100_INJ1`) |
| CAN1 | Not available on the F4 mega-uaefi variant; CAN on PB13/PB5 (CAN2, `mega-uaefi.cpp`) | Real **CAN1 on PD1/PD0** with onboard transceiver (`setHellenCan()`); CAN2 impossible because PB12/PB13 drive INJ7/8 |
| IN_AUX3 | PD1, digital-only | **PA7**, analog-capable (16A "IN_DIN" analog/digital) |
| Connectors | OEM **Honda OBD1 A/B/C/D** header + Molex Micro-Fit J1–J5/J7 adapters | Generic **TE superseal 60 + 34 + 26** pin headers, custom enclosure |
| Injector count | 4 used (Honda D-series), module channels repurposed (INJ7→VTEC-era outputs etc.) | Full **8 injectors**: 6 module channels + 2 extra on a carrier-local VNLD5160 driven by SPI2_CS/SCK (PB12/PB13) |
| Coils | 4 logic-level coil outputs + ICM output (IGN7) + fan (IGN8) on OEM pins | **6 coil outputs** (2B–7B); IGN7/IGN8 dedicated to LS5_HOT main relay / LS6_HOT fan relay |
| Wideband | One onboard WBO controller | **Two** onboard WBO controllers (First/Second CAN widebands in `prepend.txt`) |
| High side | BTS6143D used for VTEC solenoid | BTS6143D drives generic **20D high-side**, controlled by PD7 (`MM100_LED2_GREEN`) — green LED sacrificed (`getRunningLedPin()` returns Unassigned) |
| Cam/trigger defaults | Crank = MAX9924 (UART8_TX), cam = VR-discrete (UART8_RX) — fixed Honda 24+1 pattern | Trigger = MAX9924 (UART8_TX), cams default to **HALL1/HALL2** (IN_D1/IN_D2); VR-discrete available but not default |
| Buttons | None (OEM harness switches instead: brake, A/C, start…) | **BUTTON1–3** on 24D–26D (PB1/PA6/PE15) |
| EGT | J7 populate option | Dedicated 27C/28C pins, enabled by default |
| Knock channels | 1 (and the repo had a stale PA3 `knock_config.h` — flagged there) | 1 wired + **IN_KNOCK2 provision on 32C** not yet in firmware; `knock_config.h` correct (PA2) |
| SENT | not configured | `EFI_SENT_SUPPORT=TRUE`, HALL3/IN_D3 SENT-capable |
| Bootloader | F4 CAN ST bootloader considerations (pin shuffle) | `USE_OPENBLT=yes` |
| Purpose | Plug-and-play adapter for one OEM ECU footprint (Honda OBD1) | Universal/flagship variant of uaefi121 ("A variation of uaefi121 using F7 and superseal headers") |

The one-sentence summary: **same engine-driver platform, different chassis** — the Honda
board bends the mega-uaefi module into an OEM connector with F4 economics, while
super-uaefi exposes the module's full capability (8 injectors, 6 coils, dual WBO, CAN1,
more analog inputs) on generic superseal connectors with the F7 CPU module.
