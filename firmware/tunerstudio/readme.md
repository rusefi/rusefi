All .ini generation starts with tunerstudio.template.ini

Most controllers then include top_level_menu.ini and secondary_panels.ini per rusefi_config.txt defines

## Custom board gauges

Boards overriding `GAUGES_DECLARATIONS_FILE` must also account for references
in inherited tuning pages, curve editors and secondary panels. Hiding a menu
does not remove the referenced page from the generated INI.

Use `TUNING_SECTION_FILE` to provide a board-specific `[Tuning]` layout.
Optional gauge references in shared templates must respect the corresponding
`ts_show_*` feature flag, including references in included files. Boards that
omit those gauges should disable the feature flag in their prepend. Check
`ignore_gauges.txt` too: it removes declarations, not their references.
Keep validation enabled so misspelled or accidentally missing gauges fail
generation.

## VE blend dialogs

`Fuel > VE Blends` opens one combined dialog per available blend. The correction
table is on the left; the blend input, load-axis override, weight curve and live
readouts are on the right. Selecting Zero disables the blend without clearing
the table or curve. The table stays visible but editing is disabled until a
blend input channel is selected.

Blend input controls the weight curve; load-axis override independently selects
the correction table's Y axis. Zero as an override keeps the normal VE load
source. Changing a source does not convert its calibrated axis numbers.
With no override, the table title identifies the effective VE load source
(MAP, TPS, air charge or Lua) in its native units. The table retains its own
load bins and uses the existing per-blend load telemetry for its live pointer.

The input readout follows the selected channel name and uses the native units
shown on the weight curve, such as ethanol %, EGT C or MAP kPa. A table correction
of +10% at 50% weight produces +5% VE correction and a multiplier of 1.05.
Final VE includes all VE corrections. Zero blend telemetry alone does not
distinguish a real zero input from an unavailable sensor.

## Template validation

Conditions (`@@if_FLAG` and `@@if_block FLAG@@` / `@@endif_block`) require
declared `true` or `false` values, ignoring case. Give optional flags explicit
defaults in `integration/rusefi_config.txt`; board prepends can override them.
Unknown names, invalid boolean values, malformed markers and unbalanced blocks
fail generation with the template or included filename and line number.
Blocks cannot be nested. Variables inside disabled blocks are not expanded,
but condition names and marker syntax are still checked.

`0@@if_ts_show_odd_fire` is valid: the condition controls whether the line is
emitted, and `0` remains a menu placeholder. A substitution can immediately
precede a condition, for example `@@LABEL@@@@if_FLAG`.

The generator checks the final INI for unresolved template markers, including
markers introduced by substituted board fragments. Comments are ignored;
semicolons inside quoted strings remain part of the value. Firmware CI runs
the same check through `validate-bundle-ini` before creating any board bundle,
including autoupdate and obfuscated bundles. This check also runs for existing
INIs and custom generators. It does not validate the entire TunerStudio grammar.

To check generated files independently (from the repository root):

```bash
./gradlew :config_definition:shadowJar
java -cp java_tools/configuration_definition/build/libs/config_definition-all.jar \
  com.rusefi.output.GeneratedIniValidator firmware/tunerstudio/generated/*.ini
```

## Field offsets in templates

The configuration generator publishes `@@TS_PAGE_<page>_OFFSET_<field>@@` for
emitted calibration fields. The value is the field's byte offset within its
page, using its emitted INI name (including nested/iterated structure prefixes).
Page 1 is the main configuration; secondary page numbers match `config_page_N.txt`.
These numbers identify the source pages, not the renumbered INI page ordinals
on boards where optional pages are disabled.

Use these substitutions when defining alternate views of existing ECU storage,
instead of copying numeric offsets. For example, a template can reference
`@@TS_PAGE_1_OFFSET_lambdaTable@@` with the appropriate array type and dimensions.
The alias must select the correct enabled INI page and provide its own scaling.
For bit fields, the offset identifies the containing word; it does not specify
the bit index.

Offsets from secondary pages are available to the main template too. The
substitutions are template-only: they do not add ECU storage or exported C/Java
constants. Output channels, structure containers, padding/unused fields, and
empty arrays do not publish calibration offsets.
