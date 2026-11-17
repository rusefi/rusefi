This module creates visual instructions how to assemble and test a patchcord.

Problem: we have multiple plugs on the ECU side, and we have multiple plugs on vehicle adapter side.

* we need visual instructions on how to add wires in phases
* we need help in making patchcord tidy by finding a way not to tangle the wires between the plugs


ECU `.yaml` colors are assembly preferences; the helper uses only the nine
available solid spools listed in
[official-wire-colors.md](src/main/resources/com/rusefi/pinout/official-wire-colors.md).
Use the YAML files to reference images and pin coordinates. See
[interactive-pinout](https://github.com/chuckwagoncomputing/interactive-pinout),
which renders individual pinouts. The PatchCordHelper HTML uses the same image
and coordinate convention to connect selectable pins on both sides of a patchcord.

for order of assembly by functional blocks use https://github.com/rusefi/rusefi_documentation/blob/master/Universal-Patchcord-Color-Scheme.md

Use paint marker on VEHICLE side of the patchcord - VEHICLE side images have orangle cirles in all four cornes.

Each step has at most one wire of each available color. Repeated colors cause
the helper to create smaller steps within the same functional group.

## Usage

```
./gradlew :pinout-helper:shadowJar
java -jar java_tools/pinout-helper/build/libs/pinout-helper-all.jar [ecuBoardFolder] [breakoutBoardFolder] [outputFolder] [--html-only=true|false]
```

Run from the rusefi repo root. Defaults are alphax-8chan and ../private-hardware/BMW-N52-adapter;
output goes to `<breakoutBoardFolder>/patchcord-<ecuBoardFolderName>/` as
`patchcord-<breakoutBoardFolderName>.html` (interactive, self-contained) and
`patchcord-<breakoutBoardFolderName>.md` (printable steps), plus per-step and
complete-overview PNGs. Pass `--html-only=true` to generate just the HTML, as
the Miata NC guide does. Existing files in the output directory are preserved.
For example, open `patchcord-BMW-N52-adapter.html` in a browser. Select a pin
or wire row to highlight both ends; filter by step or search by pin, function,
or color; zoom the images; and copy a link to the selected wire. No Pandoc is
needed for this HTML. Each connector pair occupies its own horizontal row:
the ECU is always left, the adapter is always right, and colored traces run
left to right. A connector appears again for another partner. Only this pair's
wires have numbered, interactive markers; other matched wires on the same plug
appear as smaller, full-color, noninteractive dots. Pairs with no visible wires
are hidden by the filters. The bottom of the HTML guide shows every wired ECU
and adapter connector once, with all matched pins and colored wire routes;
this complete view stays visible while filtering the pair rows. The Markdown
ends with the same all-wire connector overview and route table.

### Optional static conversion or PDF

With Pandoc installed, run this from the output folder to convert the Markdown
to a separate, static HTML file. `--embed-resources` includes the step PNGs.

```bash
pandoc -f gfm -t html5 --standalone --embed-resources \
  patchcord-BMW-N52-adapter.md -o patchcord-BMW-N52-adapter-static.html
```

Use the actual generated Markdown filename for another adapter. To make a PDF,
open either HTML file in a browser and choose **Print → Save as PDF**.

`PatchCordHelper` matches ECU pins to adapter pins by function text (all parenthesized text such
as `(pin 6)` / `(OEM 2-8)` / descriptive notes is ignored), groups the resulting wires into assembly steps following
the Universal Patchcord Color Scheme wiring order, and renders each step's pins onto the connector
photos using the `info:` x/y coordinates - current-step pins as solid circles in wire color,
previously wired pins as dimmed circles. In the wire tables the `Adapter pin` column starts with
a wire color dot followed by an orange paint marker dot (`dot-*.png`), repeating per wire the
"paint marker goes on the vehicle end" rule that the orange image corners show per connector.
Striped, missing, and unavailable YAML colors are assigned one of the available
solid colors in the generated wiring instructions. Pins with no function match
on the other side are listed for manual review; improving YAML `function:`
texts improves the match rate.

## Current implementation

* `ConnectorYaml` reads each side's `connectors/*.yaml` files and their `info:` image paths and pin coordinates.
* `PatchCordHelper` matches `function:` text, assigns phases by `type:`, splits duplicate wire colors across steps, and writes interactive HTML plus optional Markdown.
* `InteractivePatchcordHtml` embeds the source connector images, pin coordinates, styles, and controls into a standalone HTML file.
* `WireColors`, `StepImageRenderer`, and `DotRenderer` generate the colored markers and connector PNGs.

See [the detailed guide](../../docs/AI/pinout-helper.md) for the matching rules, output format, and validation points.
