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
INI for pin selectors (`PinoutLookup.isPinSetting`). These are enum fields that
choose from one of the pin lists the INI reader recognises (`EnumIniField.isPinEnum()`:
`$output_pin_e_list`, `$switch_input_pin_e_list`, `$adc_channel_e_list` and
others), or whose keys match the convention `.*pins?\d*` (case insensitive), such
as `fuelPumpPin` and `injectionPins1`. The pin lists catch analog inputs such as
`map_sensor_hwChannel` and `tps1_1AdcChannel`, whose names do not follow the
convention; their choices are also expanded `ts_name`s ("D9 MAP"). The same
check places Tuning's Pinout button beside a selector. It reads each field's
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
setting's Tuning page and marks the setting's row there (`FieldHighlight`: a
tint and an accent bar in the Pinout selection colour) until the next mouse
press anywhere in the console or another page is shown; for a free pin the card names the choice to pick in a
pin setting. Conversely, the **Pinout** button (connector icon) beside a pin selector in Tuning
opens Pinout and selects that enum value, clearing any search or filter that
would hide it. Resting the mouse on that button shows a hover card
(`PinoutPreview`, via `PinoutLink.preview` and `HoverPreview`): the pin and its
connector, the connector drawing with the pin ringed and the other pins faded,
and the pin's type, pigtail and tune use. Without a drawing, or when the YAML
does not place the pin, the card has the text only; when the board's pinout
lacks the pin the button keeps its plain tooltip. The card stays until the
mouse leaves, unlike a tooltip. Both directions depend on an exact `ts_name` match.

## Assigning a pin from Pinout

Pinout can also assign a pin, the reverse of choosing one in a Tuning pin selector. The pin
card's **Assign…** (free pin) or **Change…** (used pin) button, or a click on a table row's
**Free · Assign…**, opens a picker
([`PinAssignPanel`](../../java_console/ui/src/main/java/com/rusefi/ui/PinAssignPanel.java)).
Fixed pins have no button. Its list comes from
[`PinAssignments`](../../java_console/ui/src/main/java/com/rusefi/ui/PinAssignments.java):
every pin selector shown in a Tuning dialog whose enum choices include the pin's expanded
`ts_name`, grouped by that dialog's title in Tuning order. The INI's choice lists therefore
decide which outputs, analog inputs or switch inputs suit a pin, and a selector no dialog shows
is left out. Each row shows where that setting is now.

A setting Tuning currently hides or disables is marked "(off)", because assigning a pin does not
turn its feature on. `PinAssignments.Conditions` evaluates the expressions on the way to the
field: the field's own enable/visible expressions, those of each `panel =` line embedding its
dialog, and those of the `subMenu` entry that opens the page. Any that is false hides the
setting; one the console cannot evaluate does not. `ExpressionEvaluator.extractVariables` names
the settings the false expressions test ("because of Enable injection, Cylinders count"). The
first of them that a dialog shows is where **Assign and open in Tuning** goes after assigning; if
none is shown, it opens the setting itself. The button appears only for such settings. The
Functions view gives the same reason in its tooltip.

A pin has one function: **Assign** sets the chosen setting to the pin and every other pin
selector on that pin to `NONE`. **Unassign** clears them all. A selector without a `NONE`
choice is left unchanged.

The edit goes through `TuningPane.applyEdit()`, wired in `ConsoleUI` (which builds Tuning
first if needed, and refuses while firmware is flashing). It is therefore an ordinary Tuning
edit: one undo step, uploaded without burning, counted as an unsaved change, and kept by Burn or
Save. `applyEdit` also redraws the open Tuning page and detached panels from the edited tune.
Those views hold their own copy of the tune, which would otherwise overwrite the Pinout edit on
the next navigation. Tune use refreshes from the resulting config-image notification.

**Swap with…** on the pin card ([`PinSwapPanel`](../../java_console/ui/src/main/java/com/rusefi/ui/PinSwapPanel.java))
exchanges what two pins do, e.g. after wiring a connector the other way round. It lists the
board's other non-fixed pins by connector, with their current use. `PinAssignments.swapProblem`
greys out a pin, with the reason, when any setting on one pin has no choice for the other.
Pairs of free pins are left out because there is nothing to swap. `PinAssignments.swap` moves
every pin selector on each pin, including selectors no dialog shows, as one `applyEdit`. A swap
with a free pin is a move. Coverage:
[`PinoutAssignTest`](../../java_console/ui/src/test/java/com/rusefi/ui/PinoutAssignTest.java).

Lua reaches pins through tune settings (`luaOutputPins`, `luaDigitalInputPins`), so those show as
used like any other setting. The exception is `readPin("PE11")`, which takes the MCU pin name. The
console cannot map that name to a connector pin: the YAMLs name pins through `meta` headers that
are not in the pinout archive. So a pin a script reads this way can still show as Free.
[`LuaReadPins`](../../java_console/ui/src/main/java/com/rusefi/ui/LuaReadPins.java) finds
`readPin` calls outside comments. When the script has any, the Assign and Swap pickers add a
note naming the pins passed as string literals and asking the user to check.
`PinoutPane.luaScriptSource` provides the script:
- When the script is on the main page (older firmware, or an offline tune), it is read from the
  tune image.
- When it is on its own page (newer firmware), it is read from the ECU on the communication
  thread. The note then appears in the open picker.
- An offline tune whose script is on its own page gives no note. Coverage: `LuaReadPinsTest`.

## Functions view

The toolbar's **Pins | Functions** switch replaces the connector pages with
[`PinoutFunctionsPage`](../../java_console/ui/src/main/java/com/rusefi/ui/PinoutFunctionsPage.java),
a board-wide list modelled on an I/O resources sheet. It lists every pin selector shown in a
Tuning dialog (`PinAssignments.settings`), in Tuning order, with its dialog, its pin and connector,
and that pin's YAML `function`. It has an **All / Assigned / Not set** filter. The tab's search
covers setting labels, dialogs, field names, pins, pin functions and connectors.

The **Pin** column offers **Not set** plus this board's pins (those with a `ts_name`) that the
setting's enum accepts, each marked with what uses it now. A current choice the pinout does not
list stays in the list. Choosing a pin goes through `PinAssignments.setPin`: a pin replaces its
other users, as with Assign, and Not set clears only that setting. Edits use the same
`applyEdit` path. The list follows tune changes through the config-image notification.
Without a tune editor the column is read-only. A double-click on a setting opens it in Tuning.
The chosen view persists across boards; Tuning's jump to a pin switches back to Pins.

## Settings applied only after a restart

The INI's `requiresPowerCycle = <field>` lines in `[ConstantsExtensions]` (e.g. `gppwm1_pin`,
`vvtPins1`, `startStopButtonPin`, `auxAnalogInputs1`) reach the console as
`IniFileModel.getRequiresPowerCycle()`, compared case-insensitively. Each
`PinAssignments.Candidate` carries `needsRestart`. When an assignment or swap changes such a
setting, the Assign and Swap pickers end their effect text with "Takes effect after you burn and
restart the ECU: …". Clearing a reboot-only setting counts as changing it, including its
Unassign tooltip. The Functions view and the Assign list mark these settings "(restart)".
See [Hardware Re-init & requiresPowerCycle](../hardware-reinit-and-power-cycle.md) for why each
entry is reboot-only.

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
  for Tune use and cross-tab navigation, and `PinoutAssignTest` for pin assignment.
