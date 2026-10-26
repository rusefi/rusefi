# PatchCordHelper pinout tool

[`PatchCordHelper`](../../java_tools/pinout-helper/src/main/java/com/rusefi/pinout/PatchCordHelper.java)
generates interactive HTML and optional Markdown and PNG assembly instructions for a patchcord between an
ECU board and a vehicle adapter board. It reads each board's connector YAML
and photos, pairs pins by their `function` text, then presents the resulting
wires in ordered steps with connector images. It is a command-line generator
in `java_tools/pinout-helper/`, separate from the Console
[Pinout tab](pinout-tab.md).

## Run it

From the repository root:

```bash
./gradlew :pinout-helper:shadowJar
java -jar java_tools/pinout-helper/build/libs/pinout-helper-all.jar \
  <ecu-board-folder> <adapter-board-folder> <output-folder> [--html-only=true|false]
```

Each input folder must contain a `connectors/` directory of `.yaml` files.
The first argument defaults to `firmware/config/boards/hellen/alphax-8chan`,
the second to `../private-hardware/BMW-N52-adapter`, and the output folder to
`<adapter-board-folder>/patchcord-<ecu-board-folder-name>`. The defaults only
work when that adapter directory is available. By default the helper writes
self-contained `patchcord-<adapter-board-folder-name>.html` and
`patchcord-<adapter-board-folder-name>.md`, connector step images,
complete-overview images, and color-dot images beside it. Use
`--html-only=true` to generate only the HTML, as the Miata NC guide does.
The generator preserves existing files in the output directory.
It embeds connector images and lets you select either end of a wire, highlight
the matching end and row, filter by step or text, zoom images, and copy a link
to a selected wire. Each connector pair is displayed in its own horizontal
row with ECU left, adapter right, and colored wire traces between their pin
labels. A plug repeats when it connects to multiple partners. Only wires in
that pair have numbered interactive markers; other matched wires on the same
plug appear as small, full-color dots without labels or click actions.
Filtering hides pairs with no visible wires. The HTML needs no Pandoc or
network connection. Its pin placement follows the `info.image.file` and
`info.pins` convention from
[interactive-pinout](https://github.com/chuckwagoncomputing/interactive-pinout).
At the bottom, a complete diagram shows each wired connector once on its
respective side, all matched pin markers, and a colored left-to-right route
for every wire. This overview always shows the full patchcord, independent
of the pair-row filters.
The tool creates the output directory and overwrites generated files of the same names.

## Input and matching rules

[`ConnectorYaml`](../../java_tools/pinout-helper/src/main/java/com/rusefi/pinout/ConnectorYaml.java)
reads every `.yaml` directly inside each `connectors/` directory, in filename
order. A file needs a top-level `pins` list with at least one entry having
`pin`; its connector name is the YAML filename without `.yaml`. For each pin,
the tool uses `pin`, `function`, `type`, and `color`. It does not use `id`,
`meta`, `class`, or `ts_name` to decide patchcord wiring. For images it uses
`info.image.file` relative to the YAML and matches each pin to its pixel
coordinates in `info.pins` (`pin`, `x`, `y`). See
[Connector Artwork](../imagegen.md) for coordinate guidance.

`PatchCordHelper.normalizeFunction` lowercases function text, removes every
parenthesized segment, replaces punctuation other than `+` and `/` with
spaces, and trims the result. For example, `WBO1 Vs (Un) (pin 6)` and
`WBO1 Vs (OEM 2-8)` both become `wbo1 vs`. Pins with the same nonempty
normalized function are paired one to one. When a function occurs more than
once, the first remaining adapter pin in file and pin order is chosen. The
matching code has no electrical or connector-location checks; review repeated
functions and the generated pairs against the actual hardware. Pins without a
match remain in separate ECU and adapter tables at the end of the document.
Consistent `function:` values from
[Pinout language conventions](../../firmware/config/boards/readme-boards.md#pinout-language-conventions)
improve the match rate.

## Assembly order and output

The `PHASES` list in `PatchCordHelper` assigns matched wires by `type`:

| Step group | Types |
|---|---|
| Power and grounds | `12v`, `5v`, `mr`, `gnd`, `pgnd`, `sgnd`, `agnd` |
| Electronic throttle and pedal | `etb` |
| Communication | `can` |
| Sensors | `av`, `at`, `hall`, `vr`, `din` |
| Wideband oxygen sensor | `wbo` |
| Injectors, coils and other numbered channels | `inj`, `ign`, `ls`, `hs` |
| Everything else | any other or missing type |

The ECU pin's `type` takes precedence; the adapter type is used only when the
ECU type is absent. Within a group, YAML colors are preferences: the ECU pin's
`color` takes precedence over the adapter pin's. The helper assigns only the
nine solid spools in
[official-wire-colors.md](../../java_tools/pinout-helper/src/main/resources/com/rusefi/pinout/official-wire-colors.md).
Unavailable, striped, or missing colors are replaced with an available solid
color. Wires are then sorted by assigned color and ECU pin and split into
substeps so no color appears twice in one step. The phase order represents the
[Universal Patchcord Color Scheme](https://github.com/rusefi/rusefi_documentation/blob/master/Universal-Patchcord-Color-Scheme.md).

The Markdown has one wire table per step, with wire color, ECU pin, adapter
pin, and ECU function. The adapter-pin cell includes a wire-color dot and an
orange paint-marker dot. The marker means to mark the **vehicle adapter end**
of each wire. [`StepImageRenderer`](../../java_tools/pinout-helper/src/main/java/com/rusefi/pinout/StepImageRenderer.java)
renders PNGs for connectors used in that step: colored circles show current
pins, dim grey rings show pins wired in earlier steps on the same connector,
and four orange corner circles identify vehicle-side images. A missing photo
produces a “no photo available” line; a pin without `info.pins` coordinates
remains in the table but has no image marker. The Markdown ends with full
connector images and an all-wire route table. In HTML, that wire's row remains
selectable even if an image or coordinate is missing.

## Change and validation points

- Keep `function` text aligned across ECU and adapter YAML when pins should
  pair; inspect duplicate functions and both unmatched-pin tables after
  generation.
- Keep `type`, `color`, image filenames, and pin coordinates aligned with the
  physical connectors. Matching uses `function`; `type` and `color` affect
  the assembly steps and renderings.
- [`PatchCordHelperTest`](../../java_tools/pinout-helper/src/test/java/com/rusefi/pinout/PatchCordHelperTest.java)
  covers normalization, matching, color splitting, file naming, Markdown
  marker dots, and standalone HTML output. Run it with
  `./gradlew :pinout-helper:test` after code changes.
