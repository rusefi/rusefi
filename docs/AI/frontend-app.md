# Java frontend app (rusEFI Console)

The rusEFI Console is the desktop Java/Swing app built as
`console/rusefi_console.jar`. It discovers and connects to an ECU, shows live
data and diagnostics, edits tunes, records and views logs, and guides firmware
updates. It is the native frontend in `java_console/`; the ECU firmware and the
separate TunerStudio plugin have their own entry points. The Console and
TunerStudio use the firmware's generated INI definitions to interpret ECU
configuration and output channels.

## From launch to a live session

1. [`Launcher`](../../java_console/ui/src/main/java/com/rusefi/Launcher.java)
   checks the Java runtime, loads local preferences, starts a background
   software update check, and dispatches command-line tools or the Swing UI.
2. [`ConsoleUI`](../../java_console/ui/src/main/java/com/rusefi/ConsoleUI.java)
   opens a specified port directly, or shows
   [`StartupFrame`](../../java_console/ui/src/main/java/com/rusefi/StartupFrame.java).
   The startup view scans for ECUs and bootloaders, offers connection and
   firmware update controls, and can open a saved tune without an ECU.
3. [`SerialPortScanner`](../../java_console/connectivity/src/main/java/com/rusefi/SerialPortScanner.java)
   tracks available hardware, including serial ports, supported CAN paths,
   TCP endpoints, DFU devices, and ST-Link probes. The scan remains active
   after the main window opens so device removal and bootloader transitions
   can be handled in the same session.
4. [`LinkManager`](../../java_console/io/src/main/java/com/rusefi/io/LinkManager.java)
   owns the ECU connection and communication executor.
   [`BinaryProtocol`](../../java_console/io/src/main/java/com/rusefi/binaryprotocol/BinaryProtocol.java)
   exchanges commands, configuration, text messages, and output data with the
   ECU. Its firmware signature selects a matching INI through
   [`RealIniFileProvider`](../../java_console/io/src/main/java/com/rusefi/binaryprotocol/RealIniFileProvider.java).
   The INI supplies field layouts, units, menus, and output-channel metadata;
   a stale INI cannot safely decode a different firmware's configuration.
5. [`SensorCentral`](../../java_console/io/src/main/java/com/rusefi/core/SensorCentral.java)
   publishes decoded output values to interested views. Views request the
   channels they need; the protocol can poll selected ranges or the full
   output block. [`UIContext`](../../java_console/ui/src/main/java/com/rusefi/ui/UIContext.java)
   holds per-session access to the link, INI, working tune, and host logger.

## Main user workflows

| Workflow | What the Console provides | Main owner |
|---|---|---|
| Watch the ECU | Gauges, status, messages, and connection feedback from live output channels and ECU text. | [`GaugesPanel`](../../java_console/ui/src/main/java/com/rusefi/ui/GaugesPanel.java), [`MessagesPane`](../../java_console/ui/src/main/java/com/rusefi/ui/MessagesPane.java) |
| Edit a tune | An INI-driven menu of settings, tables, and curves. Edits live in a working configuration image; the user can load or save an `.msq`, upload changes, and burn them to ECU storage. Offline tune editing starts from a file and reconciles local edits when an ECU connects. | [`TuningPane`](../../java_console/ui/src/main/java/com/rusefi/ui/TuningPane.java), [`TuningToolbarWidget`](../../java_console/ui/src/main/java/com/rusefi/ui/widgets/tune/TuningToolbarWidget.java) |
| Record and inspect data | Host-side binary logging, optional tune snapshots, and a viewer for `.mlg` and `.msl` files. The Engine Sniffer displays trigger and output events; it has separate acquisition limits described in [Engine Sniffer](engine_sniffer.md). | [`BinaryLoggingMenu`](../../java_console/ui/src/main/java/com/rusefi/ui/console/BinaryLoggingMenu.java), [`LogViewerHost`](../../java_console/ui/src/main/java/com/rusefi/ui/logview/LogViewerHost.java) |
| Diagnose and configure | Lua script editing, ECU messages, pinout and tune-field navigation, trouble codes, and conditional tools such as the knock analyzer and CAN sniffer. The initial setup wizard is offered when enabled and incomplete. | [`ConsoleUI`](../../java_console/ui/src/main/java/com/rusefi/ConsoleUI.java), [`PinoutPane`](../../java_console/ui/src/main/java/com/rusefi/ui/PinoutPane.java) |
| Maintain the device | Detect a live ECU or DFU/OpenBLT bootloader, update or roll back firmware, and recover the session after re-enumeration. Firmware jobs use the shared maintenance services; Console software updates are a separate path. | [`DevicePane`](../../java_console/ui/src/main/java/com/rusefi/DevicePane.java), [`FirmwareOperations`](../../java_console/ecu_shared/src/main/java/com/rusefi/maintenance/FirmwareOperations.java), [`Autoupdate`](../../java_console/autoupdate/src/main/java/com/rusefi/autoupdate/Autoupdate.java) |

The visible tabs depend on connection mode, firmware capabilities, and
`UiProperties` flags. For example, the live ECU path includes a Device tab,
while offline mode exposes tune editing without live telemetry. The old Live
Data and Logs Manager tab additions are currently disabled in `ConsoleUI`;
their source files do not imply an active tab.

## Code boundaries

| Gradle project | Responsibility |
|---|---|
| `:ui` (`java_console/ui`) | Swing startup and main window, tabs, tuning widgets, user actions. |
| `:connectivity` (`java_console/connectivity`) | Hardware discovery and ECU session state, without Swing dependencies. |
| `:ecu_io` (`java_console/io`) | Transport implementations, binary protocol, connection management, output publication. |
| `:ecu_shared` (`java_console/ecu_shared`) | Reusable firmware maintenance, logging, and other ECU services used by the Console and ECU MCP server. |
| `:inifile`, `:models`, `:shared_io` | INI parsing, configuration and output models, shared utilities and preferences. |
| `:autoupdate` | Console software updater. |

The Gradle project graph and placement details are in the
[Java Gradle structure review](../java-gradle-structure-review.md). The
[connectivity and UI testing guide](../java-connectivity-ui-unit-testing.md)
covers test seams and headless Swing constraints. `java_tools/configuration_definition`
generates the firmware and INI artifacts consumed by the app; changing the
wire layout requires regenerating those artifacts together.

Build the Console jar from the repository root with
`./gradlew :ui:shadowJar`; its configured output is
`console/rusefi_console.jar`. Run the UI tests with `./gradlew :ui:test`.
