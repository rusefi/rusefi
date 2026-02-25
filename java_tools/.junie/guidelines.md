### Project Structure and Module Locations

rusEFI is an open-source engine control unit firmware for STM32 microcontrollers.

Main firmware is located in `firmware` folder (relative to project root). This is C++ code.

Unit tests are located in `unit_tests` folder (relative to project root). This is C++ code.

This Java Gradle project has some build tools and our frontend application; it's a multi-repo-like structure where some modules are located outside the current root (`java_tools`).

#### Key Module Locations:
- `:ui` is located in `../java_console/ui`
- `:core_io` is located in `../java_console/shared_io`
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
