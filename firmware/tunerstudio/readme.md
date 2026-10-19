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

## Boost blends

`Boost Blends` has two Open Loop and two Closed Loop entries. Each opens one
expanding correction table with its input, load override, weight curve and six
numeric readouts. Input Zero keeps the table visible with editing disabled.
Closed Loop entries require Open + Closed Loop mode; Open Loop blends remain
part of the output calculation in that mode too.

Open Loop table values add percentage points to the control output. A +10 cell
at 50% weight adds 5 points, taking 40% to 45% before later adjustments. Override
Zero inherits the base open-loop table's Y Axis setting. Readouts show input,
RPM, weight, calculated correction, table load and final boost output. The
native correction telemetry has integer resolution.

Closed Loop table values add kPa offsets to the pressure target. A +20 cell at
50% weight adds 10 kPa before Lua and temperature target corrections. Override
Zero uses driver demand: the configured pedal signal, otherwise TPS. The table
label and load readout identify this default as Accel Pedal % or TPS %. Readouts show
the target correction in kPa and the final target in absolute kPa, preserving
the existing numeric channel and its 0.1-kPa correction resolution.

Input weighting and table load are independent. Changing a source does not
convert calibrated axis numbers. Configured and threshold status do not prove
that a correction reaches the actuator; skipped calculations can leave old
readouts, and sensor failures or unmet thresholds can select safe output.
Help explains these cases. Calibration storage and boost calculations are
unchanged; the UI identifies pressure corrections without changing the legacy
telemetry schema.

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
