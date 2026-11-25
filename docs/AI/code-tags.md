# Documentation Cross-Reference Tags

Full rules for `[tag:...]` comment markers.

### Documentation cross-reference tags

Use searchable comment tags to connect pieces of a non-obvious invariant,
workaround, protocol, or coordinated change that ordinary symbol references
do not reveal. A tag can link firmware, Java, build/config inputs, tests, and
documentation, or separated stages within one file.

- **Syntax and names:** For new topics use `[tag:lower_snake_case]`, with a
  descriptive name for the specific relationship. Put the literal marker in
  the file's normal comment syntax (`//`, `/* */`, `#`, `!`, or `;` as applicable).
  These are search anchors; they do not enforce consistency by themselves.
- **Reuse before inventing:** Search for an existing topic and reuse its exact
  spelling, including case and separators. Existing names such as
  `[tag:lambdaTable]`, `[tag:QC_firmware]`, and `[tag:multi-step-lua-alloc]`, and
  the legacy `search:openblt_version` marker remain valid. Adding a differently
  spelled alias splits the search results. If a rename is necessary, update
  all source, test, documentation, and tooling references together; follow the
  generated-file rules in [configuration-codegen.md](configuration-codegen.md) for generated copies.
- **Explain the connection:** At least one tagged comment must explain the
  shared rule, why it exists, and what breaks if the pieces diverge. At each
  other site, briefly explain its role or point to that explanation by
  file/symbol. A bare tag or issue number is insufficient; an issue link can
  supplement the explanation. Keep one main explanation and short references
  rather than duplicating a long account at every site.
- **Cover the relevant pieces:** Place the same marker next to each independently
  maintained part that must be reviewed together, including existing regression
  tests that protect the rule. Add it to related documentation when useful.
  Tag the relevant test or block rather than an entire unrelated suite; adding
  a tag alone does not require a new test.
- **Keep topics focused:** Prefer a specific invariant over a broad feature
  label. Existing broad groups such as `[tag:offline_tune]` can be reused, but
  introduce a narrower topic when a distinct relationship needs its own search
  results. Avoid tagging every call site or routine code whose relationship
  is already clear from symbols.
- **Maintain the whole group:** Before changing tagged behavior, read all hits
  and check whether the linked code, tests, and explanations still agree.
  Preserve meaningful markers when moving code and remove obsolete references
  when their relationship disappears.
- **Generated files:** Add or edit tags in generator/template inputs. Generated
  copies are useful for tracing output, but repeated hits are not independently
  maintained links. Do not hand-edit or stage generated outputs.
- **INI templates and flash limits:** Keep comments and tags minimal in all
  `.ini` templates and included fragments to conserve microcontroller flash.
  Avoid verbose explanations, repeated comments, and redundant tags; keep
  detailed rationale in source code or documentation. This limit takes
  precedence over the tag-coverage guidance above.

Search from the repository root with a fixed string (the brackets are literal):

```bash
rg -n --hidden -F --glob '!.git/**' '[tag:ts_page_table]' .
rg -n -F 'search:openblt_version' firmware CLAUDE.md .claude
```

For a shorter source review, add `--glob '!firmware/tunerstudio/generated/**'`
to the first command. Good worked examples are `[tag:disable_engine_module]`
(main explanation in `firmware/controllers/core/engine_module.h`) and
`[tag:ts_page_table]` (firmware identifiers/sizes, Java generator, board flags,
INI template, and generator tests).

For user-visible diagnostics that identify a tune setting, follow
`[tag:user_facing_ini_labels]`: use the corresponding human-readable .ini field
label, with setting/dialog context for ambiguous labels, so the user can find
the control. The main explanation is in `firmware/controllers/core/error_handling.h`;
issue [#10363](https://github.com/rusefi/rusefi/issues/10363) links the
`accelEnrichmentMode` diagnostic to its "Accel Enrichment Mode" label in
`firmware/tunerstudio/tunerstudio.template.ini`.
