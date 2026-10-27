# Console Pinout tab

The **Pinout** tab in rusEFI Console shows a board's physical connector pins
beside the pin assignments in the current tune. It works with a connected ECU
or an offline tune whose INI has a recognizable rusEFI signature. The tab is
enabled by `show_pinout_tab` in the Console properties.

## Data path

```text
firmware/config/boards/<board>/connectors/*.yaml + connector images
    -> PinoutsRawPackager -> pinouts_raw/connectors.zip + boards_meta.yaml
    -> PinoutCache -> PinoutPane -> connector tabs, images, pin tables
                                      + current INI/configuration image
```

[`PinoutsRawPackager`](../../java_tools/configuration_definition/src/main/java/com/rusefi/output/PinoutsRawPackager.java)
collects connector YAML and image files from `firmware/config/boards/` and
places them in a ZIP with paths relative to that boards directory. It skips
`iobox.yaml` and nested Git submodules. For each board, `boards_meta.yaml`
maps its `SHORT_BOARD_NAME` from `meta-info*.env` (or the directory name when
none is found) to `files`, `zip_file`, and the archive's SHA-256 `sha`. Multiple
short names can point at the same connector files. The packager merges its
entries with an existing metadata file so other repositories' boards remain
listed. [The upload workflow](../../.github/workflows/upload-pinouts-raw.yaml)
publishes the files under `https://rusefi.com/docs/pinouts_raw/`.

The metadata key must match the **bundle target** parsed from the ECU
signature, or from the offline tune's INI signature. A firmware board name or
connector title alone does not select a pinout. `PinoutPane` loads local
metadata first, then checks the published version in a background thread. It
looks for archives in `pinouts_raw/`, its parent directory, and the per-user
`~/.rusEFI/pinouts/` cache. The base URL and metadata filename come from
`pinout_base_url` and `pinout_meta_name`, with `RE_PINOUT_URL` and
`RE_PINOUT_META` system-property overrides.

[`PinoutCache`](../../java_console/ui/src/main/java/com/rusefi/ui/PinoutCache.java)
checks the archive SHA-256 **and** the requested connector entries. Downloads
go to a temporary file and replace the cached ZIP only after validation. If an
older ZIP still contains that board's connectors, the tab can display it while
updating and keep it if the update fails. Missing metadata, an absent board
entry, or a failed first download leaves the tab without connector data.

## Connector YAML contract

[`PinoutPane`](../../java_console/ui/src/main/java/com/rusefi/ui/PinoutPane.java)
reads each `pins` item as a table row. `pin`, `function`, `type`, `class`,
`ts_name`, and `color` become the Pin, Function, Type, Class, TS Name, and
Pigtail color columns. `info.name` (then `info.title`) labels the connector
in the toolbar's connector switch, with its pin count. `info.image.file` names an image beside the YAML, and `info.pins` gives
pin names with pixel `x`/`y` coordinates in that image. The Console decodes
images with Java `ImageIO`; PNG and JPEG are suitable for the interactive
image. A connector without a readable image still has its table.

The `pin` names in `info.pins` must match the corresponding top-level `pins`
entries. Coordinates refer to the source image pixels; the Console scales
the image and markers together, and sizes the markers from the smallest
distance between two placed pins, so they fill the cavities at any size. For
artwork and coordinate work, see [Connector Artwork](../imagegen.md).

`ts_name` is the bridge to the tune's pin enum values. The Console replaces
`___` with the physical `pin` name before matching, as does
[`PinoutLogic`](../../java_tools/configuration_definition/src/main/java/com/rusefi/pinout/PinoutLogic.java)
when generating board-specific pin choices. A different spelling in the YAML
and generated INI leaves **Tune use** blank and prevents cross-tab navigation.
`function` describes the physical capability; `type` controls marker color;
`color` supplies the optional pigtail color. For canonical function and type
terms, see [Pinout language conventions](../../firmware/config/boards/readme-boards.md#pinout-language-conventions).

[`PinColors`](../../java_console/ui/src/main/java/com/rusefi/ui/util/PinColors.java)
maps every `type` the boards use to a `PinType` coloured by family (power,
grounds, analog inputs, speed and switch inputs, outputs, communication;
theme keys `Console.Pinout.type.*`). A pin without a known `type` takes one
from its `function` words (grounds, then signals such as VR or Hall), then
its `class`, then power words; the hover card and legend show the result,
and a pin none of these place is "Untyped". A new `type` keyword therefore
needs an entry in `PinType`, or it shows as inferred or Untyped. Adding the
missing `type` values to the board YAMLs remains the better fix.

`PinoutLogic` also uses `id` or `meta` with `class` to generate firmware pin
names and INI choices. `meta` resolves through the YAML's top-level meta
header; a pin entry cannot specify both `id` and `meta`. Pins without those
generation fields can still appear as physical rows in the Console. The raw
packager does not run `PinoutLogic`, so packaging a YAML does not validate
firmware pin mappings or regenerate an INI.

## Tune assignments and navigation

[`ConsoleUI`](../../java_console/ui/src/main/java/com/rusefi/ConsoleUI.java)
creates the tab and connects it to Tuning. `PinoutPane` searches the active
INI for enum fields whose keys match the convention `.*pins?\d*` (case
insensitive), such as `fuelPumpPin` and `injectionPins1`. It reads each field's
value from the latest configuration image, matches that value to an expanded
`ts_name`, and shows the field's UI label in **Tune use**. Multiple fields
assigned to one pin appear together. `NONE` and `INVALID` assignments are
ignored.

The image listener updates **Tune use** after a working tune edit, before a
Burn. The offline path seeds it from the loaded tune image; the connected path
falls back to the controller configuration until a newer image arrives.
Each connector is a [`PinoutConnectorPage`](../../java_console/ui/src/main/java/com/rusefi/ui/PinoutConnectorPage.java).
Markers are coloured by **Type**, **Pigtail** wire colour or **Tune use**
(used by the tune, free: a `ts_name` no setting uses, or fixed: no
`ts_name`, i.e. power and ground). The legend chips under the drawing list
what the colours mean on that connector, with counts; choosing a chip shows
only those pins. The table's **All / Used / Free / Fixed** filter and the
toolbar search (pin, function, type, wire, tune name or tune use, across all
connectors, with each connector's match count on its switch button) narrow
the same set: the table lists only those pins and the drawing fades the
rest. Hovering a marker shows the pin's function, type, wire colour and tune
use.

The drawing, the table and the pin card on the right always show the same
selected pin. The card gives its function, tune use, type (marked when
inferred), pigtail, tune name and class. For a used pin, **Open in Tuning**,
a click on the table's Tune use link or a double-click on the row opens the
setting's Tuning page; for a free pin the card names the choice to pick in a
pin setting. Conversely, the **W** button beside a pin selector in Tuning
opens Pinout and selects that enum value, clearing any search or filter that
would hide it. Both directions depend on an exact `ts_name` match. Pinout is
a viewer and navigator; assigning a pin happens in Tuning.

## When changing the helper

- Edit connector data in the board's `connectors/*.yaml` and keep image names,
  coordinates, and `ts_name` values aligned with the generated INI choices.
- Check that the relevant bundle target resolves to a `boards_meta.yaml`
  entry, and that its `files` paths exist in the archive. The archive hash
  alone does not establish that a board's files are present.
- Use [`PinoutLogic` tests](../../java_tools/configuration_definition/src/test/java/com/rusefi/pinout/PinoutLogicIntegratedTest.java)
  for generated pin choices, [`PinoutCacheTest`](../../java_console/ui/src/test/java/com/rusefi/ui/PinoutCacheTest.java)
  for download and archive behavior, and
  [`PinoutTuneUseNavigationTest`](../../java_console/ui/src/test/java/com/rusefi/ui/PinoutTuneUseNavigationTest.java)
  for Tune use and cross-tab navigation.
