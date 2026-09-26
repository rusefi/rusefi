All .ini generation starts with tunerstudio.template.ini

Most controllers then include top_level_menu.ini and secondary_panels.ini per rusefi_config.txt defines

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
