# Name-to-Value Lookups & Log Field Metadata

Three generated per-field tables flatten the same struct definitions into C++ arrays/switches,
each serving a different consumer. They are frequently confused because they walk the same
inputs with the same iterator machinery — but they differ in direction (lookup by name vs
iterate all), mutability, naming universe, and flash layout.

| | config value lookup | output value lookup | MLG log fields |
|---|---|---|---|
| generator class | `GetConfigValueConsumer` | `GetOutputValueConsumer` | `SdCardFieldsContent` |
| driven by | `ConfigDefinition` (`-field_lookup_file`, see `gen_config_common.sh`) | `LiveDataProcessor` | `LiveDataProcessor` (same pass as output lookup) |
| input | `rusefi_config.txt` ts-fields (`engine_configuration_s` / `persistent_config_s`) | `LiveData.yaml` (output_channels + every live-data module struct) | `LiveData.yaml` (same walk) |
| generated file | `firmware/controllers/lua/generated/value_lookup_table_generated.cpp` (+ dead switch variant, see below) | `firmware/controllers/lua/generated/output_lookup_generated.cpp` | `firmware/console/binary/generated/log_fields_generated.h` |
| shape | `const ConfigParameter allParameters[]` = `{djb2 hash, getter lambda, setter lambda}`, sorted by unsigned hash; binary search at runtime (`value_lookup.cpp`) | `getOutputValueByHash(int)` — one big `switch` on djb2 hash | `static const constinit MLG::Entries::Field fields[]` — address/scale/type/name/units/digits/category baked `consteval` |
| direction | name -> value, **read-write** | name -> value, **read-only** | no lookup: iterated in order to emit .mlg header + records |
| key | exact struct field name (dotted path for nested, case-insensitive djb2 `djb2lowerCase`) | same | **human gauge name** = first line of the field's `;comment`, <= 34 chars (`MSQ_LENGTH_LIMIT`) |
| consumers | Lua `getCalibration`/`setCalibration` (`lua_hooks.cpp`), console `get_value`/`set` (`settings.cpp`) | Lua `getOutput` (`lua_hooks.cpp`) | SD `.mlg` logger (`mmc_card.cpp` -> `binary_mlg_logging.cpp`), unit-test logger CSV/NDJSON via `MLG::forEachField` |
| entry count (f407-discovery, 2026-08) | 738 params | 656 cases | ~850 fields |
| flash cost (f407-discovery) | ~41 K (8.9 K rodata table + ~30 K of 2x738 getter/setter lambdas) | ~20 K, but **no standalone symbol**: LTO inlines the switch into the Lua `getOutput` hook lambda (the ~20 KB `configureRusefiLuaHooks` lambda) | ~27 K rodata (32 B/entry + name/unit/category strings) |

## What they share

- **Iterator machinery**: all three use `PerFieldWithStructuresIterator` over `ConfigField`
  trees in `java_tools/configuration_definition_base/.../output/`; the two lookups share
  `VariableRecord`, `HashUtil.hash` (== firmware `djb2lowerCase`, case-insensitive) and the
  switch/fallback emission helpers (static methods on `GetOutputValueConsumer`).
- **Field filtering** (both lookups): skip unused fields, arrays, iterate-duplicates,
  directives, and non-primitive types; bit fields are included. `SdCardFieldsContent`
  differs: it skips enums (`_e` types) but includes bit fields with block offset/bit number.
- **Board specificity**: all outputs include `board_lookup.h` and vary per board
  (`#if` conditionals per live-data fragment/module in the output lookup and MLG fields) —
  entry counts above are for one board.
- **Naming siblings**: `DataLogConsumer` (TS ini `[Datalog]` section) uses the *same*
  `getHumanGaugeName` as the MLG fields, so `.mlg` field names match TunerStudio datalog
  entries — and both differ from the lookup keys, which are raw struct member names.
  One datum therefore has two string identities: `getOutput("coolant")`-style member name
  for Lua vs "CLT" gauge-comment name in logs.

## Runtime lookup mechanics

- Config: `value_lookup.cpp` `findParamByHash()` binary-searches `allParameters` (sorted by
  unsigned hash at generation time); `getConfigValueByHash`/`getConfigValueByName`/
  `setConfigValueByName` are thin wrappers. A parameter with no setter returns false on set.
- Output: the generated `switch` compiles to a compare tree; `getOutputValueByName` hashes
  then delegates. Module fields are read through `engine->module<X>()->...` expressions,
  wrapped in the module's `#if` conditional.
- MLG: `Field` constructors are `consteval` in prod builds — any runtime logic in an
  initializer silently moves the array to RAM (see comment in `mlg_field.h`); keep
  initializers compile-time constant. `writeSdLogLine()` walks the array serializing raw
  bytes with per-field multiplier metadata already written in the file header.

## Gating / flash reduction

- `EFI_LUA_LOOKUP = FALSE` (board `board.mk`, e.g. `hellen121vag`) drops **both** lookups:
  `lua.mk` swaps the two generated TUs + `value_lookup.cpp` for `value_lookup_stubs.cpp`
  (name getters return `EFI_ERROR_CODE`, setter returns false). Saves ~60 K on flash-tight
  boards at the cost of Lua getCalibration/setCalibration/getOutput and console get/set-by-name.
- MLG fields compile with the console (`console.mk` -> `binary_mlg_logging.cpp`); the ~27 K
  scales with output-channel count, not with which lookups are enabled.

## Sharp edges

- **Dead generated file**: `value_lookup_generated.cpp` (switch-based getter + string-compare
  setter) is still emitted by `GetConfigValueConsumer` (first `-field_lookup_file` argument)
  but is NOT in any `.mk` — only the table variant + `value_lookup.cpp` are compiled. Both
  define the same symbols; adding both to a build is an ODR clash. The `.md` sibling
  (`value_lookup_generated.md`) is the user-facing parameter list for the wiki.
- **Hash-collision handling is asymmetric**: the (dead) switch variant emits a
  `strEqualCaseInsensitive` fallback chain for conflicting hashes; the live table variant
  has NO collision handling — two names with equal djb2 hashes would silently resolve to
  whichever the binary search hits. Currently there are zero conflicts; the generators
  compute conflicts (`getHashConflicts`) so a collision would be visible in the dead file
  as a fallback entry, not as a build failure.
- **MLG name length**: `getHumanGaugeName` throws at generation time past 34 chars
  (`MSQ_LENGTH_LIMIT`, web backend constraint) — a too-long first comment line on any
  live-data field breaks codegen for the whole board.
- `GetOutputValueConsumer` output has no `getOutputValueByHash` symbol in LTO board builds
  (inlined into the Lua hook); when sizing flash by symbol, its cost hides inside the
  `configureRusefiLuaHooks` lambda, while `allParameters` + lambdas stay attributable.
