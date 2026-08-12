# Java Gradle Build — Structure & Class Placement Review

*Snapshot of the current state of the Gradle multi-project build (`java_console/` + `java_tools/`) with suggested improvements. Generated July 2026.*

## 1. Build overview

- Root `settings.gradle` declares **30 unique projects**. Every project lives under `java_console/` or `java_tools/` and is remapped via `project(':x').projectDir = new File(rootRelativePath + ...)`.
- Root `build.gradle` conventions: Java 8 source/target compatibility on a JDK 11 toolchain (`[tag:java8]`, kept for TunerStudio plugin compatibility); `mavenCentral()` everywhere; every subproject automatically gets `java-library`, `java-test-fixtures`, and the shadow plugin plus JetBrains annotations and JUnit 5.
- `dependencies.gradle` centralizes external library coordinates as `ext.global_libs` / `ext.ts_plugin_libs`; versions live in `gradle.properties`.
- A **second Gradle build in `android/`** re-includes the same `java_console` module directories under partially different project names (`shared_io` becomes `:core_io` there). This is why several modules `apply from: '../../android/dependencies.gradle'` (which currently defines only an empty `libs = []`).
- Two projects have **no `build.gradle`** and rely entirely on root conventions: `:version` and `:peak-can-basic` (the latter is an un-initialized git submodule — an empty directory in a fresh checkout).

## 2. Module inventory

### java_console/

| Project | Directory | Classes | Purpose | Depends on |
|---|---|---|---|---|
| `:logging-api` | logging-api | 17 | Vendored DevExperts logging fork (`com.devexperts.*`) | — |
| `:logging` | logging | 3 | `LazyFile`/`Output` utils | logging-api |
| `:shared_io` | shared_io | 24 | Low-level core/IO/net/preferences | logging-api, version |
| `:core_ui` | core_ui | 10 | Minimal Swing helpers (FrameHelper, AutoupdateUtil) | logging-api, shared_io |
| `:inifile` | inifile | 67 | TS ini / project file parsing (`com.opensr5.ini`) | logging-api, version |
| `:models` | models | 71 | Grab-bag: math/DSP, stream writers, generated config, sensors | inifile, logging, shared_io |
| `:ecu_io` | **io** | 207 | Binary protocol, serial/TCP/CAN transports, OpenBLT, autodetect | models, msq-file, peak-can-basic |
| `:connectivity` | connectivity | 8 | Headless connection-controller layer: port scanning results, device discovery helpers (no Swing allowed) | ecu_io |
| `:autoupdate` | autoupdate | 8 | Self-updater (`rusefi_autoupdate.jar`) | core_ui |
| `:shared_ui` | shared_ui | 10 | TS-plugin-shared UI bits | core_ui, ecu_io, models, config_definition_base |
| `:trigger-ui` | trigger-ui | 9 | Trigger wheel visualization | core_ui, shared_ui, models, trigger-image, autoupdate |
| `:ui` | ui | **333** | The console (`rusefi_console.jar`) — plus much non-UI code | ts_plugin, trigger-ui, autotest, luaformatter, enum_to_string |
| `:autotest` | autotest | 31 | HW/simulator functional test runner | ecu_io |
| `:luaformatter` | **luaformatter_module** | 0 | Wrapper compiling the `../luaformatter` submodule (empty when submodule not checked out) | — |
| `:mcp_ecu` | mcp_ecu | 3 | ECU MCP server (fat jar) | ecu_io, **ui**, models, inifile |
| `:mcp_can` | mcp_can | 1 | CAN sniffer MCP server (fat jar) | ecu_io, peak-can-basic |
| `:peak-can-basic` | peak-can-basic | 0 | PCAN JNI wrapper (git submodule, empty in fresh checkout) | — |

### java_tools/

| Project | Directory | Classes | Purpose | Depends on |
|---|---|---|---|---|
| `:version` | version | 1 | `UiVersion.CONSOLE_VERSION` constant | — |
| `:msq-file` | msq-file | 14 | JAXB model of .msq tune files | inifile |
| `:trigger-image` | trigger-image | 8 | Trigger wheel image rendering | logging-api |
| `:tune-tools` | tune-tools | 19+5 | Tune→C conversion (`TuneCanToolRunner`, `TS2C`) | config_definition, ecu_io |
| `:proxy_server` | proxy_server | 2 main / **19 test** | Remote-tuning proxy backend | logging-api, ecu_io |
| `:gcc_map_reader` | gcc_map_reader | 2 | GCC map file analyzer | — |
| `:enum_to_string` | enum_to_string | 15 | Enum→string codegen | logging |
| `:enum_generator` | enum_generator | 1 | Enum codegen helper | logging, shared_io |
| `:bin2header` | bin2header | 1 | Binary→C header | — |
| `:config_definition_base` | **configuration_definition_base** | 55 | Config codegen core (parser + output consumers) | inifile, enum_to_string, enum_generator |
| `:config_definition` | **configuration_definition** | 17+30 | Config codegen CLI driver | config_definition_base, trigger-image, msq-file |
| `:ts_plugin_launcher` | ts_plugin_launcher | 7 | TS plugin bootstrap loader | autoupdate |
| `:ts_plugin` | ts_plugin | 24+5 | TunerStudio plugin body | core_ui, shared_ui, ecu_io, inifile, ts_plugin_launcher, autoupdate |
| `:ts_screenshots` | ts_screenshots | 10 | TS screenshot automation | inifile, shared_ui |

### Dependency graph properties

The graph is a **DAG — no cycles**. Hubs: `logging-api` (≥7 dependents), `ecu_io` (8 dependents incl. testFixtures consumers), then `inifile`, `shared_io`, `models`, `core_ui`. Terminal deliverables: `ui`, `mcp_*`, `tune-tools`, `proxy_server`, `ts_screenshots`, `gcc_map_reader`, `bin2header`.

Note that `java_console` vs `java_tools` is **not a layering boundary**: tools depend on console modules (`ts_plugin` → `core_ui`, `tune-tools` → `ecu_io`) and console modules depend on tools (`shared_io` → `version`, `ui` → `enum_to_string`, `shared_ui` → `config_definition_base`).

## 3. Findings — current state issues

### 3.1 Severe

1. **`proxy_server`'s production code lives in `src/test/java`.** `src/main` contains only a 2-file vendored log4j shim (`com.devexperts.logging`); the entire server — `Backend`, `BackendLauncher`, `ControllerConnectionState`, the `com.rusefi.proxy.client` code, 19 classes — sits in `src/test/java/com/rusefi/server/` with zero `@Test` annotations. The module has no `Main-Class`/shadowJar, so the server can only be exercised through the test task.
2. **`:mcp_ecu` depends on the full Swing `:ui` module** (`java_console/mcp_ecu/build.gradle`). A headless stdio MCP server transitively pulls the entire console, flatlaf, syntax editors, etc. into its fat jar.
3. **Massive split packages.** The same Java package spans multiple modules in ~25 cases, the worst being: `com.rusefi.core` (shared_io, io, inifile, models), `com.rusefi` (5 modules), `com.rusefi.io` (io, models, ui), `com.rusefi.ui` (io, trigger-ui, ui), `com.rusefi.util` (4 modules), `com.rusefi.maintenance[.migration.*]` (io ↔ ui), `com.rusefi.binaryprotocol` (io, ui), `com.rusefi.tools.tune` (config_definition_base, config_definition, tune-tools), `com.opensr5[.ini]` (inifile, io, logging-api). Split packages break JPMS, confuse IDEs/shadow merging, and mean no module owns its API.
4. **`:ui` is a 333-class dumping ground.** Large clusters have no Swing dependency at all: tune migration domain logic (`com.rusefi.maintenance.*`, ~100 classes smeared across ui and io), sensor log parsing (`com.rusefi.sensor_logs`), the ANTLR LiveData parser (`com.rusefi.livedata.*`), binary protocol helpers, autodetect, and CLI entry points (`ConsoleTools`, `MassUpdater`).

### 3.2 Layering smells

5. **`:ts_plugin_launcher` → `:autoupdate` → `:core_ui`**: the tiny bootstrap that TunerStudio loads transitively includes Swing UI code from other modules (plus its own byte-identical copy of `VerticalFlowLayout`, see below).
6. **Swing leaks into headless modules** (verified imports):
   - `shared_io/src/main/java/com/rusefi/core/io/BundleInfoStrategy.java` — `JOptionPane.showConfirmDialog` in the lowest-level IO module.
   - `io/src/main/java/com/rusefi/io/ConnectionWatchdog.java` — uses `javax.swing.Timer`.
   - `models/src/main/java/com/rusefi/core/MessagesCentral.java` — `SwingUtilities.invokeLater`.
7. **`:models` is misnamed** — it is a utilities/algorithms/stream-writer grab-bag (BinarySearch, CyclicBuffer, Logicdata/VCD writers, generated config, enums), not a data-model module. Conversely `:ecu_io` mostly lives up to its name but also hosts `com.rusefi.ui.livedocs`, `com.rusefi.tools.online`, and 29 tune-migration migrator classes.
8. **`:tune-tools` → `:config_definition`** — a runtime tune tool depending on the build-time code generator.

### 3.3 Duplication

9. **`org.putgemin.VerticalFlowLayout`**: byte-identical 588-line copies in `ts_plugin_launcher` and `ui` (verified with `cmp`).
10. Near-duplicates: `UiUtils` (ts_screenshots vs trigger-ui), `DialogModel` (ts_screenshots `com.rusefi.xml` vs inifile `com.opensr5.ini`), `TestHelper` (io testFixtures vs autotest main), `UiLinkManagerHelper` duplicated *within* `ui` (default package + `com.rusefi.io`), `FindFileHelperTest` duplicated within `shared_io`.
11. Repeated build logic that could be a convention plugin: the jaxb+jaxb_runtime pair (msq-file, autoupdate, ts_screenshots, ui), httpclient/httpcore in 5 modules, shadowJar `jsr305`/`annotations` exclusions in 3 modules, hamcrest/icu4j excludes in 2 each. `proxy_server` pulls `org.glassfish:javax.json` directly instead of `global_libs.javaxJson` (its own TODO flags this).

### 3.4 Hygiene / correctness nits

12. **Duplicate include** (fixed July 2026): `:logging-api` was declared twice in `settings.gradle`.
13. **Name↔directory mismatches** requiring manual `projectDir` remaps: `:ecu_io`→`io`, `:config_definition[_base]`→`configuration_definition[_base]`, `:luaformatter`→`luaformatter_module`. Naming style is also mixed (dash vs underscore) despite the comment at `settings.gradle:18`.
14. **Likely bug**: `java_tools/bin2header/build.gradle` — `destinationDirectory = file( '$rootDir/../..' )` uses single quotes, so `$rootDir` is never interpolated; the jar lands in a literal `$rootDir/../..` path relative to the project.
15. **Broken package directory** (fixed July 2026): `enum_generator/src/main/java/com.rusefi/EnumGenerator.java` — the directory was literally named `com.rusefi` (with a dot); now moved to `com/rusefi/`.
16. **Default-package classes in main source sets**: `ts_screenshots/src/main/java/ScreenCaptureLauncher.java`, `ui/src/main/java/MassUpdater.java`, `ui/src/main/java/UiLinkManagerHelper.java`.
17. **Test-ish packages shipped in main jars**: `msq-file` `com.opensr5.ini.test`, `enum_to_string` `com.rusefi.util.test`, `io` `com.rusefi.binaryprotocol.test.Bug3923`, sandbox mains in `models` (`LogicdataStreamFile*Sandbox`).
18. **Out-of-tree source/resource reaches** (fragile but some are intentional): `luaformatter_module` srcDir → `../luaformatter/src` (empty submodule); `trigger-image` resources from `../../unit_tests/triggers.txt`; `io` test `java.library.path` → `../../unit_tests/build`; `autotest` hard-codes `firmware/tunerstudio/generated/rusefi_f407-discovery.ini`; `shared_ui` hard-codes `../../java_tools/ts_plugin_launcher/lib/TunerStudioPluginAPI.jar`; `autoupdate` uses `evaluationDependsOn` (self-described "yes nasty sorry"); `autoupdate` and `ui` shadow jars write into `${rootDir}/console` (`[dark-magic-jar]`, PR #9051).
19. **Orphans / dead artifacts**: `java_console/tools/` (legacy `com.irnems` DSP filters, no build file, not in settings.gradle); `java_tools/pcb_sync/` (Ant-based, outside Gradle); `java_tools/ts2c.jar` (committed jar with zero references anywhere — the live logic is `tune-tools`' `TS2C.java`); `java_console/mcp_python/` (Python, informational only).
20. **Blanket plugin application**: every subproject gets shadow + test-fixtures even when meaningless (`:version` is one interface); meanwhile `proxy_server` and `luaformatter_module` apply `id 'java'`/`java` redundantly on top of the injected `java-library`.

## 4. Suggested improvements

### Quick wins (no restructuring, low risk)

1. ~~Delete the duplicate `:logging-api` include in `settings.gradle`.~~ Done.
2. Fix `bin2header`'s `'$rootDir/../..'` quoting (or better, stop writing jars outside `build/`).
3. ~~Rename `enum_generator`'s `com.rusefi` directory to `com/rusefi/`.~~ Done.
4. Add package declarations to the three default-package classes; delete the dead `ui/src/main/java/UiLinkManagerHelper.java` duplicate and the stray `FindFileHelperTest` copy.
5. Delete `java_tools/ts2c.jar`; decide whether `java_console/tools/` and `java_tools/pcb_sync/` are dead (delete) or alive (make them modules).
6. Remove the redundant `apply from: '../../android/dependencies.gradle'` calls (the file defines an empty list) or make it define something real.
7. De-duplicate `VerticalFlowLayout` — keep one copy in `core_ui` (or `shared_ui`) and depend on it.
8. Fix the three Swing leaks: replace `javax.swing.Timer` in `ConnectionWatchdog` with a scheduled executor; invert `BundleInfoStrategy`'s JOptionPane into a callback supplied by the UI layer; give `MessagesCentral` a pluggable executor instead of `SwingUtilities.invokeLater`.

### Medium effort

9. **Move `proxy_server`'s server from `src/test/java` to `src/main/java`**, keep the real tests in test, and give it a `Main-Class`/shadowJar so it is a deployable artifact.
10. **Break `:mcp_ecu`'s dependency on `:ui`** — identify the handful of classes it actually uses and move them down to `ecu_io`/`models`/a new headless module.
11. Move test helpers out of main source sets (`com.opensr5.ini.test`, `com.rusefi.util.test`, `Bug3923`, `*Sandbox` mains) into `test`/`testFixtures` — the `java-test-fixtures` plugin is already applied everywhere.
12. Consolidate `com.rusefi.tools.tune` into one module (currently config_definition_base + config_definition + tune-tools).
13. Introduce a small **convention plugin** (`buildSrc/` or an included build) for: the fat-jar-with-jaxb pattern, the shared shadowJar exclusions, and the httpclient stack — replacing copy-pasted blocks. Apply shadow/test-fixtures selectively there instead of on all 30 projects.
14. Align project names with directories: either rename the dirs (`io`→`ecu_io`, `configuration_definition*`→`config_definition*`, `luaformatter_module` merge) or the project ids, and pick one naming convention (Gradle's own docs prefer dashes).

### Longer term (directional)

> **Progress (July 2026):** a `:connectivity` module now exists between `:ecu_io` and `:ui` as the home for the headless connection-controller layer. First move-only batch: `SerialPortType`, `PortResult`, `AvailableHardware`, `SerialPortCache`, `UiLinkManagerHelper` (from `:ui`) and `ScannerHelper`, `OpenbltDetectorStrategy`, `PlainSerialPortScanner` (from `:ecu_io`). Since moved as well: `SerialPortScanner`, `ConnectivityContext`, `PortScanner`, `RecurringStep`, and `DeviceSessionManager` + `SessionState` (the former Swing blocker was dissolved by extracting the `DeviceSessionManager.JobExecutor` seam that `SingleAsyncJobExecutor` now implements). Next candidate: give `ConnectedEcuTarget`'s static volatile (in `:shared_io`) a home in the connectivity layer.

15. **Resolve split packages module by module** — this is the single highest-leverage cleanup because it unlocks everything else. Target: each package has exactly one owning module. Start with the worst offenders: `com.rusefi.core`, `com.rusefi.io`, `com.rusefi.util`, `com.opensr5.*`.
16. **Extract the tune-migration domain** (~100 classes currently split between `io` and `ui`) into its own headless module — it is business logic, testable without Swing, and the biggest coherent cluster inside `ui`.
17. Slim `:ui` further by moving `sensor_logs`, `livedata` (ANTLR parser), and CLI entry points (`ConsoleTools`) into non-UI modules; rename `:models` to what it is (e.g. `util`/`algo`) or actually make it models by relocating the stream writers and buffers.
18. Make `ts_plugin_launcher` truly minimal: inline the few UI helpers it needs so it does not drag `autoupdate`→`core_ui`, keeping the TS-loaded bootstrap small and stable.
19. Decide the fate of the empty submodule-backed projects (`peak-can-basic`, `luaformatter`): document `git submodule update --init` as a build prerequisite, or guard their inclusion in `settings.gradle` on directory presence so a fresh checkout configures cleanly.
