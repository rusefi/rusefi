### Project Structure and Module Locations

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
- rusefi_updater.exe (see console_launcher folder for launch4j) invokes Autoupdate#main with args="basic-ui" which points at BasicStartupFrame::runTool: Focuses on firmware updater
- rusefi_autoupdate.exe entry point is Autoupdate#main
- rusefi_console.exe invokes Launcher#main with empty args

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

CalibrationDialogWidget: The UI manager that orchestrates the layout of the right-side panel, dynamically creating fields and table views.

TuningTableView: The core visual component for 3D table editing, managing the JTable rendering and the gradient color logic.
