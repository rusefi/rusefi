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
