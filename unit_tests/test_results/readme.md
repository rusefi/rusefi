## rusEFI unit test output folder

Unit tests write multiple kinds of per-test reports into this folder when artifact logging is enabled via `setUnitTestCreateLogs(true)` (declared in `unit_tests/test-framework/engine_test_helper.h`).

Typical files emitted per test (one set per `TEST(...)` while logging is on):

- `unittest_<Suite>_<Name>.msl` — binary MLG/MLS log (TunerStudio-compatible) with one row per logged tick.
- `unittest_<Suite>_<Name>.csv` — same data as `.msl` but as CSV with a header row (`time_us`, then `name[units]` per field). Best for spreadsheets / quick numeric inspection.
- `unittest_<Suite>_<Name>.ndjson` — same data as one JSON object per line (NDJSON). Best for LLM agents / `jq` / streaming parsers.
- `unittest_<Suite>_<Name>_trace.json` — scheduler/event trace.
- `unittest_<Suite>_<Name>.logicdata` and related sniffer files — digital pin state over time.

At process exit `sayByeBye()` prints the absolute path of this folder and the count of non-zero-size files produced.

### Cleanup

This folder is cleaned at the start of every `rusefi_test` run by `cleanTestResultsFolder()` (see `unit_tests/test-framework/engine_test_helper.cpp`). All regular files are removed **except**:

- `.gitignore`
- `readme.md` (this file)

Keep both of those files committed. Everything else in this folder is generated and safe to delete at any time.
