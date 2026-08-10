This module creates visual instructions how to assemble and test a patchcord.

Problem: we have multiple plugs on the ECU side, and we have multiple plugs on vehicle adapter side.

* we need visual instructions on how to add wires in phases
* we need help in making patchcord tidy by finding a way not to tangle the wires between the plugs


for wire colors use ECU .yaml files, use same .yaml files to reference images and pin coordinates. See https://github.com/chuckwagoncomputing/interactive-pinout
which renders individual pinouts.

for order of assembly by functional blocks use https://github.com/rusefi/rusefi_documentation/blob/master/Universal-Patchcord-Color-Scheme.md

Use paint marker on VEHICLE side of the patchcord - VEHICLE side images have orangle cirles in all four cornes.

Each step of the instructions should not have two wires of the same color - break into smaller steps if wires duplicate.

## Usage

```
./gradlew :pinout-helper:shadowJar
java -jar java_tools/pinout-helper/build/libs/pinout-helper-all.jar [ecuBoardFolder] [breakoutBoardFolder] [outputFolder]
```

Run from the rusefi repo root. Defaults are super-uaefi and ../private-hardware/BMW-N52-adapter;
output goes to `<breakoutBoardFolder>/patchcord/` as `patchcord.md` plus per-step PNG images.

`PatchCordHelper` matches ECU pins to adapter pins by function text (parenthesized suffixes such
as `(pin 6)` / `(OEM 2-8)` are ignored), groups the resulting wires into assembly steps following
the Universal Patchcord Color Scheme wiring order, and renders each step's pins onto the connector
photos using the `info:` x/y coordinates - current-step pins as solid circles in wire color,
previously wired pins as dimmed circles. Pins with no function match on the other side are listed
in the document for manual wiring; improving the yaml `function:` texts improves the match rate.

## Map of existing relevant code

YAML parsing and pin model - `:config_definition` / `:config_definition_base`, package `com.rusefi.pinout`:
* `PinoutLogic` (`java_tools/configuration_definition`) - canonical SnakeYAML parser for board connector yamls: `pin`, `meta`, `class`, `type`, `color`, `ts_name`, `function` keys, meta-header indirection (e.g. `MM100_INJ6` -> STM32 pin), array-form `id`/`class` pins.
* `BoardInputs`, `FileSystemBoardInputsReaderImpl`, `PinoutLogicConstants` (`java_tools/configuration_definition_base`) - abstraction for enumerating a board's yaml files; `FileSystemBoardInputsImpl` in `configuration_definition`.
* Usage examples: `PinoutLogicTest`, `PinoutLogicIntegratedTest`, `PinoutLogicHellen112_17_Test`.

Packaging yamls + connector photos:
* `com.rusefi.output.PinoutsRawPackager` (`java_tools/configuration_definition`) - sweeps `firmware/config/boards/**/connectors/`, zips yamls and images into `pinouts_raw/connectors.zip` with a `boards_meta.yaml` index (published to rusefi.com/docs). This is how pinout data already travels from the firmware tree to consumers.

Rendering with images, coordinates and wire colors - `java_console/ui`:
* `com.rusefi.ui.PinoutPane` - interactive-pinout consumer: loads `boards_meta.yaml`/`connectors.zip`, parses the yaml `info:` section (`title`, `image.file`, per-pin `x`/`y` coordinates - same schema as interactive-pinout), draws pin markers on the connector photo (`PinCoord`, `ConnectorImagePanel`).
* `com.rusefi.ui.util.PinColors` - wire-color-string -> `java.awt.Color` resolution, including striped colors like `orange/brown` (`pigtailToColors`), plus pin-type -> color legend mirroring the web CSS.

Headless image-file generation pattern:
* `java_tools/trigger-image` - existing example of a build-time tool rendering images to files; template for producing static "phase N" assembly instruction images.

Notes:
* The yaml model and file discovery should come from `:config_definition` / `:config_definition_base` as project dependencies.
* `PinColors` and the coordinate parsing currently live inside the Swing-coupled `:ui` module (`PinoutPane`); reuse means either depending on `:ui` or extracting the parsing/color code into a shared module.
* The assembly-order source (Universal-Patchcord-Color-Scheme.md) lives in the separate rusefi_documentation repo and has no Java representation yet.
