### Scope of this file

`CLAUDE.md` (at the repository root) is considered part of these guidelines — read it first. This file only adds Junie-specific guidance and frontend/Java details that are **not** already covered in `CLAUDE.md`. Do not duplicate content from `CLAUDE.md` here; if something belongs in both audiences, put it in `CLAUDE.md` and reference it from here.

See also: `CLAUDE.md`, `docs/adding-new-trigger.md`, `.junie/ts-help-topic.md`, `.junie/ts-readme.md`, `README-mcp.md`, `java_console/mcp_lua/README.md`, `firmware/controllers/lua/examples/`, [Lua-Scripting.md](https://github.com/rusefi/rusefi_documentation/blob/master/Lua-Scripting.md).

## Frontend (Java console)

`CLAUDE.md` focuses on the C++ firmware / unit tests / simulator. This section covers the Java side, which `CLAUDE.md` does not describe.

rusEFI console frontend application scans serial ports and well-known local TCP port 29001 to locate an engine control unit embedded device.

`PortDetectorSandbox` (`java_console/ui/src/test/java/com/rusefi/autodetect/PortDetectorSandbox.java`) is a handy manual sandbox for testing serial port auto-detection logic against real hardware. Run its `main()` to continuously poll and print detected ports and signatures.

An ECU would respond to a HELLO command with a signature String which identifies ECU kind, model and firmware version (see `BundleInfo`).

Based on that signature, UI frontend would pull an `.ini` file with all the metadata related to the exact ECU we have connected to from the internet, and cache it locally in `user.home`.

That `.ini` file would be parsed into an `IniFileModel` instance giving java code knowledge of desired calibration memory and user interface layout.

The Java side is a Gradle project with build tools and the frontend application; it's a multi-repo-like structure where some modules are located outside the current root (`java_tools`).

#### Key Module Locations:
- `:ui` is located in `../java_console/ui`
- `:shared_io` is located in `../java_console/shared_io`
- `:core_ui` is located in `../java_console/core_ui`
- `:autoupdate` is located in `../java_console/autoupdate`
- `:shared_ui` is located in `../java_console/shared_ui`
- `:ecu_io` is located in `../java_console/io`
- `:logging` is located in `../java_console/logging`
- `:logging-api` is located in `../java_console/logging-api`
- `:inifile` is located in `../java_console/inifile`
- `:models` is located in `../java_console/models`
- `:autotest` is located in `../java_console/autotest`
- `:luaformatter` is located in `../java_console/luaformatter_module`
- `:mcp_lua` is located in `../java_console/mcp_lua` — MCP server exposing rusEFI Lua tooling (set/get scripts, messages, commands) over stdio JSON-RPC. See `java_console/mcp_lua/README.md` for the exposed tools (`set_lua`, `get_lua`, `lua_reset`, `read_messages`, `wait_for_message`, …) and the typical LLM iteration loop.
- `:mcp_can` is located in `../java_console/mcp_can` — MCP server for read-only CAN bus sniffing via PCAN hardware

#### MCP servers and Lua iteration
- The `:mcp_lua` module is the recommended way for an LLM (Junie / Claude Desktop / JetBrains AI / Cursor) to iterate on Lua scripts against a real ECU: write a candidate script, `set_lua` to upload + burn + `luareset`, then `wait_for_message` / `read_messages` to observe `print(...)` / `efiPrintf` output. See `java_console/mcp_lua/README.md` for the full architecture (`LuaService` in `:ecu_io`, `MessagesCentral` listener) and gotchas (stdio transport, single ECU connection, ASCII-only `LUASCRIPT`).
- **By default target a real ECU** connected via serial port. The Win32 simulator is not reliable for MCP workflows (socket binding issues, process instability). Use serial port autodetect (omit `--port`) or pass the explicit serial port name.
- Reference Lua scripts live in `firmware/controllers/lua/examples/` (e.g. `launch_control.lua`, `CruiseCheck.lua`, `gdi4-communication.lua`, `man-in-the-middle.txt`, `DBW-controller.txt`, `dash-sweep.lua`, …). When asked to write or improve a Lua script, **start by scanning that folder** for an analogous example before drafting from scratch, then iterate via `:mcp_lua`. Read `firmware/controllers/lua/examples/readme.md` first — its "Gotchas" section covers common pitfalls (Lua-fed sensors time out unless `:set()` is refreshed every `onTick`; failing `txCan` on an unACKed bus stalls the Lua thread).
- For CAN-bus context that a Lua script might react to, the read-only `:mcp_can` PCAN sniffer can be used to capture frames first.

#### Build and Development:
- When running Gradle commands from `java_tools`, you can refer to these modules by their simple names (e.g., `:ui`, `:ecu_io`) as they are included in `settings.gradle`.
- When navigating the file system, remember that `java_console` is a sibling directory to `java_tools`.
- Source code for `:ui` is in `../java_console/ui/src/main/java`.

#### UI entry points
- `rusefi_updater.exe` (see `console_launcher` folder for launch4j) invokes `Launcher#main` with empty args; the merged launcher handles both the autoupdate flow and the console UI.
- `rusefi_autoupdate.exe` entry point is `Autoupdate#main`.

#### TS templating:
- Pattern: `#define CAM_INPUT_1_1_NAME "Cam sensor bank 1 intake"` in `rusefi_config.txt`
- Usage: `@@CAM_INPUT_1_1_NAME@@` in `tunerstudio.template.ini`
- This pattern is used for dynamic strings in the TunerStudio UI.

#### Autoupdate:
- Logic for bypassing: Caps Lock key skips the automatic update process.
- Configuration: `autoupdate_bundle=true` in `shared_io.properties` controls the global auto-update behavior.

#### Subject domain
- Timing: In the context of engine control, "timing" usually refers to **ignition timing advance** (the angle before TDC when the spark occurs).

#### Key frontend java application classes
AbstractIoStream: Base class for communication streams that manages byte counters, activity tracking, and listener notification.

IniFileModel: The primary interface for accessing configuration data, including fields, tables, menus, and gauges parsed from the INI.

ArrayIniField: A specialized field for handling multidimensional data arrays, managing their rows, columns, offsets, and multipliers.

FieldType: Enum defining the data types (FLOAT, INT16, etc.) and their specific storage sizes and binary reading logic.

GroupMenuModel: A container used to nest multiple menu items together under a single parent group.

IniField: The abstract foundation for every configuration parameter. It defines the core identity (name) and the specific memory address (offset) where a value resides within the controller's binary image.

MenuItem: A marker interface implemented by sub-menus and groups to be part of the navigation hierarchy.

MenuModel: Represents the top-level menu structure used to build the navigation tree in the UI.

SensorCentral: The central singleton hub that tracks real-time sensor values and dispatches updates to registered listeners.

SubMenuModel: A concrete menu entry that links a display name to a specific key (like a table ID).

TableModel: A metadata container (POJO) that stores a table's configuration, such as its ID, titles, axis labels, and bin constants.

UIContext: Acts as the main application state singleton, providing access to essential components like the link manager and the INI file state.

IniFileState: Manages the current state and availability of the INI configuration model within the application lifecycle.

CalibrationDialogWidget: The UI manager that orchestrates the layout of the right-side panel, dynamically creating fields and table views.

Node: A hierarchical data structure used to store and retrieve UI configuration and layout preferences.

SensorGauge: The primary factory and logic handler for instantiating individual gauge widgets based on model definitions.

GaugeModel: Represents the technical template for a single gauge, holding its output channel, display limits, and unit labels.

TuningTableView: The core visual component for 3D table editing, managing the JTable rendering and the gradient color logic.

GaugesPanel: Manages the overall layout, rendering, and container logic for the dashboard's collection of gauges.

WellKnownGauges: Provides an enumeration of standard gauges and their associated output channel names for consistent data mapping.

BinaryProtocol: Encapsulates the logical state of the connection and handles the retrieval of the configuration image from the controller.

GaugesGrid: Manages the visual container by wrapping a JPanel with a GridLayout and handling its dimensional resets.

GaugesGridElement: Acts as a factory wrapper for individual slots, toggling between gauge and graph modes based on persistent `Node` configuration.

### Bug Fix Process
- **Mandatory separation**: When fixing a bug, you must first create and commit (or submit in a separate prompt) a reproduction test case that fails without the fix.
- Only after the reproduction test case is verified to fail, you may proceed with implementing and committing the fix.
- Verification of the fix must be done using the same reproduction test case.
- The reproduction test source file must be `git add`-ed when created (see "Source Control Hygiene" in `CLAUDE.md`).
