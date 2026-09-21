All .ini generation starts with tunerstudio.template.ini

Most controllers then include top_level_menu.ini and secondary_panels.ini per rusefi_config.txt defines

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
