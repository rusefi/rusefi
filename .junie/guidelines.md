### Project Structure and Module Locations

See also CLAUDE.md

rusEFI is an open-source engine control unit firmware for STM32 microcontrollers.

Main firmware is located in `firmware` folder (relative to project root). This is C++ code.

Unit tests are located in `unit_tests` folder (relative to project root). This is C++ code.

This Java Gradle project has some build tools and our frontend application; it's a multi-repo-like structure where some modules are located outside the current root (`java_tools`).

## Frontend

rusEFI console frontend application scans serial ports and well-known local TCP port 29001 to locate an engine control unit embedded device.

An ECU would respond to a HELLO command with a signature String which identifies ECU kind, model and firmware version  (see BundleInfo)

Based on that signature, UI frontend would pull an .ini file with all the metadata related to the exact ECU we have connected to from the internet, and cache it locally in user.home

That .ini file would be parsed into IniFileModel instance giving java code knowledge of desired calibration memory and user interface layout.

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

#### Build and Development:
- When running Gradle commands from `java_tools`, you can refer to these modules by their simple names (e.g., `:ui`, `:ecu_io`) as they are included in `settings.gradle`.
- When navigating the file system, remember that `java_console` is a sibling directory to `java_tools`.
- Source code for `:ui` is in `../java_console/ui/src/main/java`.

#### UI entry points
- rusefi_updater.exe (see console_launcher folder for launch4j) invokes Launcher#main with empty args; the merged launcher handles both the autoupdate flow and the console UI
- rusefi_autoupdate.exe entry point is Autoupdate#main

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

IniFileState: Manages the current state and availability of the INI configuration model within the application lifecycle.CalibrationDialogWidget: The UI manager that orchestrates the layout of the right-side panel, dynamically creating fields and table views.

Node: A hierarchical data structure used to store and retrieve UI configuration and layout preferences.

SensorGauge: The primary factory and logic handler for instantiating individual gauge widgets based on model definitions.

GaugeModel: Represents the technical template for a single gauge, holding its output channel, display limits, and unit labels.

TuningTableView: The core visual component for 3D table editing, managing the JTable rendering and the gradient color logic.

GaugesPanel: Manages the overall layout, rendering, and container logic for the dashboard's collection of gauges.

WellKnownGauges: Provides an enumeration of standard gauges and their associated output channel names for consistent data mapping.

BinaryProtocol Encapsulates the logical state of the connection and handles the retrieval of the configuration image from the controller.

GaugesGrid: Manages the visual container by wrapping a JPanel with a GridLayout and handling its dimensional resets.

GaugesGridElement: Acts as a factory wrapper for individual slots, toggling between gauge and graph modes based on persistent Node configuration.

See CLAUDE.md
