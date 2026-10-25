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

Run from the rusefi repo root. Defaults are alphax-8chan and ../private-hardware/BMW-N52-adapter;
output goes to `<breakoutBoardFolder>/patchcord-<ecuBoardFolderName>/` as `patchcord-<breakoutBoardFolderName>.md`
(for example `patchcord-BMW-N52-adapter.md`) plus per-step PNG images.

`PatchCordHelper` matches ECU pins to adapter pins by function text (all parenthesized text such
as `(pin 6)` / `(OEM 2-8)` / descriptive notes is ignored), groups the resulting wires into assembly steps following
the Universal Patchcord Color Scheme wiring order, and renders each step's pins onto the connector
photos using the `info:` x/y coordinates - current-step pins as solid circles in wire color,
previously wired pins as dimmed circles. In the wire tables the `Adapter pin` column starts with
a wire color dot followed by an orange paint marker dot (`dot-*.png`), repeating per wire the
"paint marker goes on the vehicle end" rule that the orange image corners show per connector.
A magenta `dot-unknown.png` means the yaml declares no `color:` for that pin. Pins with no function match on the other side are listed
in the document for manual wiring; improving the yaml `function:` texts improves the match rate.

## Current implementation

* `ConnectorYaml` reads each side's `connectors/*.yaml` files and their `info:` image paths and pin coordinates.
* `PatchCordHelper` matches `function:` text, assigns phases by `type:`, splits duplicate wire colors across steps, and writes the Markdown.
* `WireColors`, `StepImageRenderer`, and `DotRenderer` generate the colored markers and connector PNGs.

See [the detailed guide](../../docs/AI/pinout-helper.md) for the matching rules, output format, and validation points.
