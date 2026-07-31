# Hellen Mapping: from Vehicle Connector Pin to STM32 Pin

How a signal on a Hellen-platform board is traced through every layer - from
the physical vehicle-connector pin, through the hellen-one hardware modules,
to the STM32 GPIO the firmware actually toggles - and how each layer is kept
consistent by naming conventions, meta headers, and code generation.

Three repositories participate:

| Repo | Role |
|------|------|
| https://github.com/rusefi/rusefi | Firmware: meta headers, per-board directories, connector YAMLs, code generation |
| https://github.com/rusefi/hellen-one | Hardware framework: proven module gerbers/footprints, gerber-merge tooling, board-ID registry |
| https://github.com/rusefi/uaefi (example) | One board: a KiCAD "frame" design that places hellen-one modules; produces fabrication files |

See also https://wiki.rusefi.com/Hellen-One-Platform and the hellen-one wiki
at https://github.com/andreika-git/hellen-one/wiki.

## The layers, top to bottom

```
vehicle connector pin ("B6")
  -> frame PCB net (board KiCAD design, e.g. uaefi.kicad_pcb)
    -> hellen-one module pad (e.g. mega-mcu100 module footprint)
      -> fixed STM32 port/pin of that module (e.g. PD3)
```

The firmware never hardcodes "PD3" in board code. Instead each layer has a
name, and mapping tables connect the names:

```
connectors/B.yaml:  pin B6 -> meta name MM100_INJ1
hellen_mm100_meta.h:  #define MM100_INJ1 D3
board_configuration.cpp:  Gpio::MM100_INJ1  (preprocessor -> Gpio::D3)
```

## Layer 1: hellen-one hardware modules

hellen-one is a toolset that produces custom PCBs by merging gerber files of
known-good functional modules into a per-vehicle *frame* PCB that carries
little more than the main connector. Modules live in the `modules/` folder of
https://github.com/rusefi/hellen-one, versioned by revision subfolders
(e.g. `mega-mcu100/0.3/`), each containing final gerbers, BOM/CPL, a KiCAD
footprint (`.kicad_mod`) and symbol (`.kicad_sym`). Module examples: `ign8`
(8 ignition drivers), `output`, `power12`, `wbo` (wideband), `vr`,
`vr-max9924`, `knock`, `sd`, `can`, and the MCU modules below.

A board designer places module footprints on the frame (the uaefi frame
places `hellen-one-mega-mcu100-0.3:mega-mcu100`), routes connector pins to
module pads, and pushes to GitHub. GitHub Actions in the board repo run the
hellen-one docker tooling: KiCAD gerber export, then gerber *merge* - the
module images are stamped into the frame gerbers (rotation only by multiples
of 90 degrees). Fabrication outputs land in the board repo's `boards/`
folder, one subfolder per revision (`uaefi-a` ... `uaefi-f2`, revision
declared in `revision.txt`).

Because a module's gerbers are fixed, a module pad always connects to the
same internal signal. For MCU modules that means: **module pad -> STM32
port/pin is a hardware constant**. That constant is what the firmware meta
headers record.

## Layer 2: MCU modules and the meta headers

Two generations of Hellen MCU placement exist:

- **Original MCU boards**: the STM32 sits directly on the frame (modules
  `mcu`, `mcu144`). Signal names use the `H144_` / `H176_` prefixes (by STM32
  package pin count) - e.g. `H144_LS_1` (low-side driver 1), `H144_IGN_1`,
  `H144_IN_CLT`.
- **Megamodule ("mega") boards**: the MCU plus support circuitry is itself a
  swappable module (`mega-mcu64`, `mega-mcu100`, `mega-mcu144`), so the
  frame never carries the MCU. Signal names use `MM64_` / `MM100_` /
  `MM176_` prefixes: `MM100_INJ1`, `MM100_IN_CLT`, `MM100_LED1_RED`.

The mapping "signal name -> STM32 pin" lives in meta headers in
`firmware/config/boards/` of the rusefi repo:

| Header | Contents |
|--------|----------|
| `hellen_mm64_meta.h` | `MM64_*` defines for the 64-pin megamodule |
| `hellen_mm100_meta.h` | `MM100_*` defines for the 100-pin megamodule |
| `hellen_mm176_meta.h` | `MM176_*` defines for the 176-pin megamodule |
| `hellen_premium176_meta.h` | `MMP176_*` defines for the 176-pin STM32F767 "mega-mcu-premium" megamodule |
| `hellen_meta.h` | includes all three MM headers, plus `H144_*`/`H176_*` names and shared SPI/CAN/UART pins |
| `hellen_original_mcu_meta.h` | `H176_*` names specific to the original (non-mega) MCU placement |
| `hellen_all_meta.h` | includes everything |

Conventions inside the headers:

- Most defines are the **bare pin token** (`#define MM100_INJ1 D3`, not
  `Gpio::D3`). This dual-use is deliberate: C++ writes `Gpio::MM100_INJ1`
  (the preprocessor pastes to `Gpio::D3`), while the YAML pipeline (below)
  resolves the same macro textually. Some older `H176_*` entries bake in the
  `Gpio::` prefix and thus cannot be used from YAML the same way.
- Analog inputs come in pairs: `MM100_IN_CLT` is the digital pin token
  (`C2`) and `MM100_IN_CLT_ANALOG` is the ADC channel (`EFI_ADC_12`),
  because a config field wants either a `Gpio` or an `adc_channel_e`.
  A few inputs are wired to *two* STM32 pins so the same signal can be
  sampled by ADC and captured by a timer (`MM100_IN_AUX2` C4 vs
  `MM100_IN_AUX2_DIGITAL` E9).
- Functional grouping mirrors the module hardware: `INJ1..8` low-side
  injector drivers, `IGN1..8` ignition IGBT/logic outputs, `OUT_PWM1..6/8`,
  `IN_D1..4` digital inputs, `IN_CRANK`/`IN_CAM`, standard sensor inputs
  (`IN_TPS`, `IN_PPS`, `IN_CLT`, `IN_IAT`, `IN_MAP1..3`, `IN_O2S`,
  `IN_VBATT`), `GP*` general purpose, SPI/CAN/UART buses, LEDs, and the two
  board-ID pins.
- Pin-usage variants get their own prefixed names rather than editing the
  base ones: `MM100_MEGA_UAEFI_*` defines cover the "mega-uaefi" MCU module
  variant (shared firmware glue in the uaefi121 board directory,
  `mega-uaefi.cpp`), which shuffled a few pins (INJ1 to PD0, SPI1 MOSI,
  CAN2, no CAN1) so that uaefi121 could support the ST CAN-bus bootloader.

Headers are chained by `#include`, so a 100-pin board only sees the
namespaces that apply to it (`hellen_mm100_meta.h` alone, or `hellen_meta.h`
for the full set).

## Layer 3: firmware board directories

Hellen firmware boards live in `firmware/config/boards/hellen/<board>/` in
the rusefi repo. Shared makefiles wire in the platform:

- `hellen-common.mk` - always included last: adds `hellen_common.cpp` and
  `hellen_board_id.cpp`, sets `-DHW_HELLEN=1`, inverted LED mode, and the
  standard USART2 console pins (PD5/PD6).
- `hellen-common64/100/144/176.mk` and `hellen-common-mega144/176.mk` - one
  per MCU module size: set the critical-error LED from the right namespace
  (`Gpio::MM100_LED1_RED`), define `HELLEN_BOARD_MM100=TRUE` etc., pick the
  board-ID pins (`HELLEN_BOARD_ID_PIN_1/2`), and per-size extras (ADC mux
  pin, software knock, onboard accelerometer).

Gotcha: core files resolve meta names only when `HW_HELLEN` is set -
`efi_gpio.cpp` includes `hellen_all_meta.h` under `#if HW_HELLEN`, so a
board that passes `-DLED_CRITICAL_ERROR_BRAIN_PIN=Gpio::MM100_LED1_RED`
without including `hellen-common.mk` (which defines `HW_HELLEN=1`) fails
with "'MM100_LED1_RED' is not a member of 'Gpio'".

`board_configuration.cpp` then assigns defaults using only meta names, e.g.
from the uaefi board:

```cpp
engineConfiguration->clt.adcChannel = MM100_IN_CLT_ANALOG;
engineConfiguration->camInputs[0] = Gpio::MM100_IN_D1;   // HALL1
engineConfiguration->mainRelayPin = Gpio::MM100_IGN7;
setHellenVbatt();       // standard Hellen dividers: 4.7k/4.7k analog, 33k/6.8k vbatt
setHellenCan();         // PD0/PD1, same on MM100/144/176
setHellenMegaEnPin();   // megamodule power-management EN pin
```

`hellen_common.cpp` holds the shared helpers: analog divider constants,
CAN pin presets (including the `setHellen64Can` exception), and the
megamodule EN power management - newer mega boards gate SD card, wideband
and external-flash power behind an EN pin, so `hellenEnableEn()` /
`hellenDisableEn()` also register/unregister the MFS storage backend and
unmount the SD card around power transitions.

## Layer 4: connector YAMLs and code generation

The final mapping layer ties *vehicle connector pins* to meta names:
`firmware/config/boards/hellen/<board>/connectors/*.yaml`, one file per
physical connector. The file declares which meta header resolves its names,
then lists pins:

```yaml
meta: config/boards/hellen_mm100_meta.h

pins:
  - pin: B6
    meta: MM100_INJ1
    class: outputs
    type: ls
    color: green
    ts_name: ___ injector output 1
```

`PinoutLogic.java` (part of `java_tools/configuration_definition`, run
automatically by every firmware `make`) parses the YAMLs, textually resolves
each `meta:` reference against the declared header, and generates into
`connectors/`:

- `generated_board_pin_names.h` - `#define PIN_B6 MM100_INJ1` (connector
  naming available to C++);
- `generated_ts_name_by_pin.cpp` - `getBoardSpecificPinName()`, so the
  console/TunerStudio can display "B6 - injector output 1" instead of PD3;
- `generated_outputs.h` plus the pin-name enums baked into the TunerStudio
  `.ini`, so every dropdown in the tune shows connector names.

Notes:

- `___` in a `ts_name` is replaced by the connector pin id, giving names
  like "B6 injector output 1" without repeating the pin.
- A pin that serves two classes (analog input *and* switch input) uses the
  array form: `id: [MM100_IN_O2S2_ANALOG, MM100_IN_O2S2]` with matching
  `class: [analog_inputs, switch_inputs]` - `meta:` is only for the
  single-mapping case.
- Purely electrical pins (power feeds, ETB motor outputs driven by a DC
  driver chip) have `function:`/`type:` but no meta mapping.

The net effect: renumbering a connector, moving a signal to another module
pad, or swapping the MCU module each touches exactly one layer, and the
generated artifacts plus `Gpio::`-namespace compilation catch mismatches.

## Board ID: knowing which board and revision at runtime

Hellen boards self-identify electrically. Two dedicated MCU pins (e.g.
`MM100_BOARD_ID1`/`ID2` = PD9/PD8) connect to an RC network built from two
ID resistors chosen at assembly time. `hellen_board_id.cpp`
(`detectHellenBoardId()`) charges/discharges the capacitor through each
resistor, measures the timings, and solves for R with a Newton's-method
solver, then snaps to the closest values in the E-series tables of
`hellen_board_id_resistors.h`. The pair of resistor indices encodes
`boardId = majorIdx * 100 + minorIdx` (464 possible IDs).

The registry of assigned IDs is `board_id/board_ids.csv` in
https://github.com/rusefi/hellen-one - e.g. id 302 = 1000R/750R =
"uaefi-b". Every *revision* of a board gets its own ID, which is how one
firmware binary applies revision-specific quirks. The same folder holds the
generator scripts that emit a `board_id_xxx.csv` BOM fragment to include
from the board's `bom_replace_*.csv`, so the fab actually populates the
right resistors per revision.

## Extracting the mapping from existing PDFs

The hellen-one modules repo carries no
editable schematic sources for them - the per-revision PDFs are the
authoritative record of a module's internals:
`modules/<name>/<rev>/<name>-schematic.pdf` (plus `-pcb3d.pdf`) in
https://github.com/rusefi/hellen-one. When writing or verifying a meta
header or a connector YAML for a module revision, the mapping has to be
recovered from those PDFs. The good news: they are vector exports with a
full text layer, so no OCR is involved.

### Step 1: get the text layer

```bash
pdftotext -layout mega-mcu100-schematic.pdf mm100.txt
```

(`pdftotext` ships with poppler-utils.) The `-layout` flag preserves rough
2-D placement, which keeps label/value pairs adjacent.

### Step 2: net name -> MCU pin

The schematics annotate every MCU-connected net label with the STM32 pin in
parentheses - `OUT_INJ1 (PD3)`, `IGN8 (PE6)`, `SPI3_SCK (PC10)`. One grep
recovers the whole table:

```bash
grep -oE '[A-Za-z0-9_]+ \(P[A-K][0-9]{1,2}\)' mm100.txt | sort -u
```

For `mega-mcu100/0.3` this yields 63 unique net->pin pairs covering every
injector, ignition, PWM, sensor input, bus, and debug pin.

### Step 3: module pad -> net

The module's castellated edge pads appear in the schematic as one component
(refdes `M100` on mega-mcu100) whose pins are the pad-grid coordinates. In
the extracted text each pad shows up as a visible `NET  PAD` pair
(`OUT_INJ1  E17`) next to an Altium hidden-text pin artifact
(`PIM1000E17` = pin E17 of M100). Listing all pads:

```bash
grep -oE 'PIM1000[A-Z][0-9]+' mm100.txt | sed 's/PIM1000//' | sort -u
```

### Decoding Altium hidden-text artifacts

Altium PDF exports embed invisible object identifiers that pdftotext dumps
along with the real text. They are recognizable by prefix and are actually
useful once decoded:

| Artifact | Meaning | Example |
|----------|---------|---------|
| `CO<refdes>` | component | `COM100` = component M100 |
| `PI<refdes>0<pad>` | pin of a component | `PIM1000E17` = M100 pad E17 |
| `NL<net>` | net label, `_` rendered as `0` | `NLOUT0INJ1` = net OUT_INJ1 |
| `PO<port>` | sheet port | `POUART80X` ~ UART8_?X |

Artifact strings are sometimes duplicated or concatenated
(`POUART80XPOUART80X`); treat them as a machine-readable bonus channel, and
read the plain visible text for anything ambiguous.

### Step 4: cross-check against the firmware meta header

Never generate a meta header from a PDF blindly - diff the extracted table
against the existing `hellen_mm*_meta.h` and reconcile every difference.
A real cross-check of `mega-mcu100/0.3` against `hellen_mm100_meta.h`:

- 47 of 63 nets match the firmware defines exactly (after trivial renames
  like `OUT_INJ1` -> `MM100_INJ1`).
- 2 are genuinely swapped: the schematic says `IN_AUX3 (PC5)` /
  `IN_AUX4 (PA7)` while firmware defines `MM100_IN_AUX3 A7` /
  `MM100_IN_AUX4 C5`. This is the known aux3/4 flip called out by a comment
  in `hellen_meta.h` - hardware net names and firmware names disagree, and
  the firmware names won. Exactly the kind of trap the cross-check exists
  to catch.
- The rest differ only in naming: `IN_VIGN` = `MM100_IN_VBATT`,
  `OUT_PWR_EN` = `MM100_GP8`, `USBID` = `MM100_USB1ID`, and SWD/USB/I2C
  pins have no meta define at all.

The comparison is a few lines of scripting: parse `NET (Pxn)` pairs from
the text dump, parse `#define MM100_<NAME> <pin>` from the header, join and
report matches / mismatches / one-sided entries.

### Pitfalls

- `-layout` interleaves the sheet's columns: grep for self-contained pairs
  (`NET (PIN)`); do not try to read whole rows as table rows. When a pair
  looks ambiguous, confirm it visually on the rendered page.
- Net names are per-module namespaces: on the ign8 module, `IGN1..8` are
  the *inputs* from the frame and `OUT_IGN1..8` the driver outputs; the
  same technique applies but the direction must come from reading the
  schematic, not the name.
- PCB plots (gerber PDFs such as a board repo's `gerber/*.pdf`) carry no
  net annotations - they are only useful visually. For frame boards, which
  are all KiCAD, skip PDFs entirely: `.kicad_sch`/`.kicad_pcb` are text
  formats, so hierarchical labels and net assignments grep directly.
- If `pdftotext` returns nothing, the PDF is image-only and needs rendering
  (`pdftoppm`) plus manual reading or OCR - not currently the case for any
  hellen-one module PDF.
- When parsing a P-CAD ASCII netlist (`*-pcad.PCB`) as the ground truth,
  extract `(net ...)` blocks with *brace matching*, never with a regex that
  assumes consecutive `(node ...)` entries - a net block containing any
  other sub-element silently truncates, and the dropped nets read as
  "unrouted". A real audit of mega-mcu-premium lost 27 of 312 nets this way
  (exactly the Ethernet RMII + eMMC buses) and produced a false
  "not routed" conclusion. Sanity-check by counting `(net ` occurrences vs
  parsed nets, and corroborate suspicious absences against copper: count
  `netNameRef "<net>"` occurrences (tracks/vias) and compare with a
  known-routed net's count.

## Worked example: uaefi injector 1

1. Hardware: https://github.com/rusefi/uaefi frame places module
   `mega-mcu100/0.3`; connector pin B6 is routed to the module pad that the
   module internally wires to STM32 pin PD3. Fab files for the current
   revision (`revision.txt`, e.g. `f2`) are under `boards/uaefi-f2/`.
2. `hellen_mm100_meta.h`: `#define MM100_INJ1 D3`.
3. `connectors/B.yaml`: `pin: B6` -> `meta: MM100_INJ1`,
   `ts_name: ___ injector output 1`.
4. Generated: `PIN_B6` define, TunerStudio shows "B6 injector output 1" in
   the injection-output dropdown.
5. `board_configuration.cpp` defaults injector 1 to `Gpio::MM100_INJ1`
   (i.e. `Gpio::D3`) and the tune can remap it to any other listed pin.
