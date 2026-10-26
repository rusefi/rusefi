# Java frontend app (rusEFI Console)

The rusEFI Console is the Java/Swing desktop frontend built as
`console/rusefi_console.jar`. It discovers an ECU, establishes a protocol
session, shows live data, edits and saves calibrations, records and views logs,
and performs device maintenance. The app lives primarily in `java_console/`.
ECU firmware runs on the controller; TunerStudio is a separate desktop
application that can use the same generated firmware definitions.

This page describes the Console's current functional paths and the classes
that own them. The most useful model is a chain of responsibilities:

```text
Launcher / StartupFrame / ConsoleUI
    -> SerialPortScanner + DeviceSessionManager (which device and state?)
    -> LinkManager + BinaryProtocol (transport and ECU commands)
    -> matching INI + ConfigurationImage (how bytes become settings)
    -> SensorCentral (how output bytes become live values)
    -> tabs, tune editor, host logger, and maintenance jobs
```

The INI is a runtime contract with the firmware. It describes configuration
pages and fields, output-channel offsets and conversions, gauges, menus,
dialogs, tables, and curves. A tune file or an ECU byte image cannot be
interpreted safely with an arbitrary INI. The Console selects one by the
firmware or tune signature before exposing the corresponding settings.

## Startup and device discovery

### Process entry and the first window

1. [`Launcher`](../../java_console/ui/src/main/java/com/rusefi/Launcher.java)
   requires Java 11 or newer, initializes file logging and persistent
   preferences, registers the manual INI picker, and finishes any pending
   Console JAR replacement from the software updater. It dispatches command
   line tools when requested, otherwise starts the Swing UI. Its silent
   Console software update check runs in the background.
2. [`ConsoleUI.startUi`](../../java_console/ui/src/main/java/com/rusefi/ConsoleUI.java)
   sets the look and feel and constructs the UI on the Swing event thread.
   An explicit port argument can go directly to the main Console. Normal
   interactive startup opens [`StartupFrame`](../../java_console/ui/src/main/java/com/rusefi/StartupFrame.java).
3. The startup window shows a scanning card, then connection controls if
   discovery does not lead to automatic connection. Its tabs provide
   connection, firmware update, and tune management actions. The user can
   select a discovered port, let a single candidate connect automatically,
   choose an explicit port, or open an `.msq` tune without hardware.
4. Opening the main Console reuses the startup window and its session context.
   The scanner continues running across that handoff. A tune opened offline
   can therefore move into a live session when a compatible ECU appears,
   instead of requiring another Console instance.

Startup also offers an external TunerStudio launch path. It closes the
Console connection before starting TunerStudio so both programs do not
contend for the same serial port. The Java Console is not the TunerStudio
plugin; that integration has its own project and lifecycle.

### What discovery sees

[`ProductionConnectivity`](../../java_console/ui/src/main/java/com/rusefi/ProductionConnectivity.java)
creates the shared [`ConnectivityContext`](../../java_console/connectivity/src/main/java/com/rusefi/ConnectivityContext.java).
The context owns a [`SerialPortScanner`](../../java_console/connectivity/src/main/java/com/rusefi/SerialPortScanner.java)
and the connected ECU target. Hardware probes classify ordinary ECU serial
ports, supported CAN adapters, TCP/simulator endpoints, DFU devices, and
ST-Link probes. The scan results drive the port selector and firmware
choices; they are more than a one-time startup list.

The scanner caches the currently connected serial port so an ongoing probe
does not reopen a port that BinaryProtocol is reading. It keeps scanning for
disappearance, replug, and bootloader devices. Slow probe classes are
throttled. Firmware operations can suspend the scanner entirely while a
job needs exclusive access to a port or USB transition.

The [`DeviceSessionManager`](../../java_console/connectivity/src/main/java/com/rusefi/DeviceSessionManager.java)
combines the link status, latest hardware snapshot, and job executor into
these UI-facing states:

| State | Meaning to the Console |
|---|---|
| `DISCONNECTED` | No live protocol connection or active firmware job. |
| `CONNECTING` | A protocol connection is loading. |
| `CONNECTED` | The ECU protocol and configuration are ready for live views. |
| `FLASHING` | A firmware or related exclusive job owns the transition. |
| `DEVICE_IN_DFU` | A DFU device is visible without a live ECU session. |
| `DEVICE_IN_BLT` | An OpenBLT bootloader is visible without a live ECU session. |

Session notifications may originate on a scanner or connection thread.
Swing subscribers marshal rendering back to the event thread. The state
manager keeps the live port cached, pauses the connection watchdog before an
exclusive job, suspends scanning when that job starts work, and resumes both
after it finishes. This avoids competing reads during flashing and still
lets the window follow the board through USB re-enumeration.

## Establishing an ECU session

### Transport and connection ownership

[`LinkManager`](../../java_console/io/src/main/java/com/rusefi/io/LinkManager.java)
owns the connection executor and the current [`BinaryProtocol`](../../java_console/io/src/main/java/com/rusefi/binaryprotocol/BinaryProtocol.java).
It selects a connector for serial, TCP, PCAN, SLCAN, or SocketCAN as
appropriate. The connection watchdog can restart a lost connection to the
selected port unless the user has disconnected intentionally. A reconnect
is a new protocol negotiation and configuration read, not simply a repaint
of cached values.

The live connection proceeds in this order:

1. Open a transport stream and exchange a HELLO command to obtain the
   firmware signature.
2. Parse the ECU target in that signature and check it against the Console
   bundle's board compatibility rules. A known incompatible bundle stops
   before configuration is read; an unknown bundle target, such as a local
   simulator build, skips that check.
3. Ask [`RealIniFileProvider`](../../java_console/io/src/main/java/com/rusefi/binaryprotocol/RealIniFileProvider.java)
   for the INI matching that signature. Without one, the Console cannot
   decode the configuration and does not complete the connection.
4. Read configuration page 0 into a `ConfigurationImage` of the INI's declared
   size. The protocol can validate a local cache or read the image in full.
5. Start the output and text pull loop, protocol logger, and UI listeners.
   Only then is the session ready for live tabs and tune operations.

INI lookup checks the bundle's INI location, then the current directory,
then a local cache or download, then TunerStudio's ECU definition cache.
As a last resort the UI can ask the user to pick an INI; a successful choice
is imported into the local cache. The picker is registered in the UI module,
so the protocol and INI provider remain free of Swing dependencies.
Repeated scanner probes for the same missing signature do not repeatedly
open that picker.

The [`UIContext`](../../java_console/ui/src/main/java/com/rusefi/ui/UIContext.java)
is the main window's access point for its LinkManager, INI, working tune,
offline status, and [`SensorLogger`](../../java_console/ecu_shared/src/main/java/com/rusefi/sensor_logs/SensorLogger.java).
It also broadcasts changes to the current configuration image. That is how
views such as Pinout can reflect an edit before the user burns it.

### Live output data and ECU text

BinaryProtocol polls the output block described by the connected INI.
[`SensorCentral`](../../java_console/io/src/main/java/com/rusefi/core/SensorCentral.java)
collects named channel subscriptions and full-output leases from consumers.
[`OutputChannelPollPlan`](../../java_console/io/src/main/java/com/rusefi/binaryprotocol/OutputChannelPollPlan.java)
resolves direct fields, gauges, and expression dependencies into byte ranges,
coalesces nearby ranges, and falls back to a full block when it cannot
resolve a demand. Protocol reads are split into chunks bounded by the INI's
blocking factor.

Each read publishes an [`OutputChannelSnapshot`](../../java_console/io/src/main/java/com/rusefi/core/OutputChannelSnapshot.java)
with valid-byte information and a demand generation. Consumers can tell a
partial poll from a complete output image. SensorCentral decodes values for
subscribers and clears stale values after disconnection. The protocol loop
also retrieves ECU text and configuration-error information, which feed the
Messages view and status indicators.

ConsoleUI changes subscriptions with the selected tab. Gauges and Tuning
declare the channels they use; other tabs currently hold a full-output
lease. Recording a binary log also holds a full-output lease. The selective
poll plan reduces reads when the active consumers have known channel needs,
while a logger still receives complete samples. A tab's visual presence
does not imply that every output byte is always fetched.

## Editing a tune

### How the UI is built from the INI

The [`TuningPane`](../../java_console/ui/src/main/java/com/rusefi/ui/TuningPane.java)
creates the left-hand settings tree from INI menu definitions, including
expression-controlled enablement and visibility. Dialog, table, curve, and
gauge widgets render fields from the same model. The editor maintains a
session `ConfigurationImage`; field widgets read and write values through
the INI's offsets, types, scales, and units. Sections can be detached into
separate windows and restored with the window layout.

The tune image has several distinct meanings:

| Image or state | What it represents |
|---|---|
| ECU configuration image | The bytes read from the connected ECU and tracked by BinaryProtocol. |
| Working image | The editable snapshot shown by Tuning and other tune-aware views. |
| Load/burn baseline | The point used by Discard and the pending-change indicator. |
| `.msq` file | A portable tune representation written or read using an INI. |

The [`TuningToolbarWidget`](../../java_console/ui/src/main/java/com/rusefi/ui/widgets/tune/TuningToolbarWidget.java)
owns Undo, Redo, Discard, Load, Save, and Burn controls. Edits are grouped
into undo steps after a short idle period. A separate short debounce sends
a snapshot of the working image to the ECU while connected.
`BinaryProtocol.uploadChangesWithoutBurn` compares it with the ECU image and
writes only changed byte ranges to ECU RAM. The engine can respond to those
live values, but RAM upload does not persist the tune.

**Burn to ECU** sends the burn command to persist pending changes, then
updates the protocol's cached configuration and the editor's baseline.
Discard restores the last loaded or burned baseline in the editor. The
toolbar's pending-change state is about that baseline, not whether a separate
`.msq` file has been saved. A lost connection before a burn can therefore
leave local edits that have not been persisted on the ECU.

### Loading, saving, and compatibility

Save Tune serializes the working image into an `.msq` using the current
INI; file writing runs in the background. Load Tune reads an `.msq` and
resolves the file's firmware signature. Its fields are mapped into an image
for the connected ECU, with partial-failure reporting when a field cannot
be transferred. On a live ECU, a successful load uploads the resulting
image and burns it. Loading a file is therefore different from merely
opening it for offline editing.

[`OfflineTuneLoader`](../../java_console/ui/src/main/java/com/rusefi/maintenance/OfflineTuneLoader.java)
opens an `.msq` without hardware. It requires a signature in the file,
resolves the matching INI with the same local/download/manual lookup path,
and materializes a configuration image. The Console then marks the session
offline and allows Tuning without live telemetry. A tune with no signature
or no matching INI cannot be interpreted by this path.

When an ECU later connects, the Console reconciles offline work rather than
silently replacing it:

- With the same signature, it can offer to keep and burn local edits or use
  the ECU's tune.
- With a different signature,
  [`OfflineEditMigration`](../../java_console/ui/src/main/java/com/rusefi/maintenance/OfflineEditMigration.java)
  compares the edited image with the original offline baseline. It carries
  only fields that actually changed and also exist in the target INI onto
  a clone of the target ECU image. Fields absent from the target are reported
  as incompatible. Fields that fail conversion are skipped and logged.

This field-level migration limits accidental replacement of unrelated
settings when an offline tune was made for another firmware definition.
The compatibility decision remains visible to the user before writing to
the newly connected ECU.

## Watching, diagnosing, and scripting

| Surface | Functional role and data source |
|---|---|
| [Gauges](../../java_console/ui/src/main/java/com/rusefi/ui/GaugesPanel.java) | Live dashboard values decoded from output channels by SensorCentral. |
| [Messages](../../java_console/ui/src/main/java/com/rusefi/ui/MessagesPane.java) | ECU text and Console messages, plus a command entry and recent commands. |
| [OBD Codes](../../java_console/ui/src/main/java/com/rusefi/ui/obd/ObdCodesPane.java) | Optional online tab for recent and last error-code output channels. Set `show_obd_codes_tab=true` in the bundled `shared_io.properties` to show it; it is hidden by default. Its visible history is for this Console session; clearing the view does not erase ECU codes. |
| Engine Sniffer | Composite trigger/output events acquired on a separate path from ordinary scalar output polling. See [Engine Sniffer](engine_sniffer.md) for acquisition and display limits. |
| [Lua Scripting](../../java_console/ui/src/main/java/com/rusefi/ui/lua/LuaScriptPanel.java) | Editor and controls for ECU Lua scripts; built lazily when its tab is first used. |
| [Knock Analyzer](../../java_console/ui/src/main/java/com/rusefi/ui/KnockPane.java) | Shown when the Console's `UiProperties` feature flag enables it. |
| [CAN/SLCAN](../../java_console/ui/src/main/java/com/rusefi/ui/SlcanTab.java) | Conditional CAN tool whose availability depends on the connection and INI. |
| [AI Troubleshooting](../../java_console/ui/src/main/java/com/rusefi/ui/llm/LLMTab.java) | Online-session tab for its optional troubleshooting workflow, built when opened. Completed conversations and retained tool evidence are saved privately by account in local session files; recent sessions can be resumed or deleted. |

The Messages view is useful when a connection is technically alive but a
feature is failing: protocol text and configuration errors are separate
from gauge values. Pinout serves a different kind of diagnosis. Its
[`PinoutPane`](../../java_console/ui/src/main/java/com/rusefi/ui/PinoutPane.java)
selects board connector metadata by ECU or offline tune target, verifies
the downloaded archive, and renders connector images and pin tables. It
combines those physical pins with current tune assignments. Selecting a
tune field can highlight its pin; double-clicking an assigned pin can open
the corresponding Tuning field. It listens to the working image, so this
navigation also reflects unburned edits.
See [Console Pinout tab](pinout-tab.md) for the connector data,
cache, and matching rules behind that tab.

The [setup wizard](frontend-app-wizard.md) configures a connected ECU and
burns each completed step with an ECU-backed progress flag. It can launch
automatically when supported steps are incomplete, or from **Launch Wizard**.
The wizard guide covers launch settings, step order, sensor calibration,
output tests, and the standalone VIN prompt used during startup.

Lua Scripting has a separate write path from ordinary tune edits. Its
[`LuaScriptPanel`](../../java_console/ui/src/main/java/com/rusefi/ui/lua/LuaScriptPanel.java)
finds the `luaScript` string field in the active INI. On older layouts it
can read the script from the page-0 image already loaded for Tuning; when
the field is on a secondary page it reads that page directly from the ECU.
**Write to ECU** writes the padded script bytes to the field's page in
ECU RAM and sends a Lua reset command so the running VM reloads the script.
That action does not issue a settings burn. This matters when diagnosing
why a script ran immediately but did not survive an ECU power cycle.

## Recording and viewing data logs

The main window's Logging menu owns Start, Stop, Save tune, and View Last
Log. [`BinaryLoggingMenu`](../../java_console/ui/src/main/java/com/rusefi/ui/console/BinaryLoggingMenu.java)
allows a live-connected user to choose an `.mlg` destination and prevents
an unfinished recording from being opened as the last log. Ctrl+S toggles
the available Start or Stop action.

When Save tune is selected, the menu first reads a tune snapshot, saves
a dated `.msq` beside the log, and includes the tune bytes in the binary
log. Recording begins only if that snapshot succeeds and the same live
connection is still present. Turning off Save tune starts logging without
that preparatory tune read.

SensorLogger uses the INI's datalog labels where available and includes
other raw scalar or enum output fields so incomplete INI datalog sections
still produce useful logs. Computed or host-only channels, bit fields,
and strings are excluded from this raw recorder. It acquires a full-output
lease and writes a row only from a complete output snapshot of the current
poll generation. Stop closes the file and releases the lease.

The [`LogViewerHost`](../../java_console/ui/src/main/java/com/rusefi/ui/logview/LogViewerHost.java)
opens `.mlg` and `.msl` files through its button, Ctrl+O, file drop, or
View Last Log. Loading runs in the background. If opening a replacement
log fails, the previous chart remains visible; chart colors, grid, and
cursor settings carry over between logs. The viewer is useful without
opening a live ECU connection.

Host binary logging, the Log Viewer, and the Engine Sniffer are distinct:
the first records regular output-channel samples, the second reads saved
data logs, and the third displays ECU composite events.

## Device maintenance and software updates

The [`DevicePane`](../../java_console/ui/src/main/java/com/rusefi/DevicePane.java)
renders the shared session state and available hardware. Its
[`ProgramSelector`](../../java_console/ui/src/main/java/com/rusefi/maintenance/ProgramSelector.java)
offers firmware actions appropriate to the detected live ECU, DFU device,
or OpenBLT bootloader. The exact flash route depends on the board, bundle,
and connected hardware. Shared
[`FirmwareOperations`](../../java_console/ecu_shared/src/main/java/com/rusefi/maintenance/FirmwareOperations.java)
and related flashers execute those operations. A firmware rollback picker
is available for bundles that provide suitable LTS rollback candidates.

Each window serializes its firmware and tune jobs through a
[`SingleAsyncJobExecutor`](../../java_console/ui/src/main/java/com/rusefi/ui/basic/SingleAsyncJobExecutor.java),
which supplies progress and ensures exclusive job ownership. Before a
startup-window job opens the ECU port, the splash connection is released.
During a main-window firmware job, the session becomes `FLASHING`, the
watchdog is paused, the scanner is suspended, and live tabs are disabled.
When a device is seen only in DFU or OpenBLT, the Console keeps Device and
other offline-capable tabs available while avoiding frozen live views.
After the job, scanning and connection recovery resume.

A bootloader round trip can change the serial port name. The Console can
follow the newly visible ECU when the former port has disappeared and the
new hardware snapshot identifies a single candidate. It does not guess
among multiple candidate ECUs. See
[hardware reinitialization and power-cycle behavior](../hardware-reinit-and-power-cycle.md)
for related device transition details.

[`Autoupdate`](../../java_console/autoupdate/src/main/java/com/rusefi/autoupdate/Autoupdate.java)
updates the **Console application** itself. It checks in the background
when enabled; a downloaded replacement JAR is finalized on the next
launch. That path is separate from flashing ECU firmware through Device.

## Main-window composition and availability

[`ConsoleUI`](../../java_console/ui/src/main/java/com/rusefi/ConsoleUI.java)
assembles the window from panels rather than implementing each feature
inside one class. Gauges, Log Viewer, Messages, Lua Scripting, Engine
Sniffer, Tuning, Pinout, Manage Tunes, Device, OBD Codes, and other
conditional tabs are added according to connection mode, INI, bundle
metadata, and `UiProperties` flags. Tuning and Lua are constructed lazily
unless a saved tab selection needs them immediately. The Console restores
the last selected tab and detached calibration windows.

This has several consequences when reading the source:

- Offline mode exposes tune and other offline-capable work, but no live
  sensor values or ECU commands.
- The Device tab changes controls with `CONNECTED`, `FLASHING`, and
  bootloader states. It does not own the link watchdog or hardware scanner.
- Manage Tunes depends on bundle tune-management metadata being available.
- Some tabs are supplied by `ServiceLoader` providers, so they may not be
  found in the central list of built-in tab constructors.
- The Live Data tab line is commented out due to its current breakage; Logs
  Manager is guarded by a false condition; the Online tab is commented out.
  Their source classes are not evidence that those tabs are active.

The menu bar provides file load/save and log opening, ECU and Console
update actions, logging controls, and shortcuts. Window overlays and
status indicators surface connection changes, configuration errors,
unburned tune edits, and update progress. The main window coordinates
those surfaces; transport state comes from LinkManager and
DeviceSessionManager, while tune and log state comes from their owners.

## Code map and generated inputs

| Gradle project | Main responsibility |
|---|---|
| `:ui` (`java_console/ui`) | Launcher, startup and main Swing windows, tabs, tuning widgets, user actions, maintenance presentation. |
| `:connectivity` (`java_console/connectivity`) | Hardware discovery, connected target, and device session state without Swing dependencies. |
| `:ecu_io` (`java_console/io`) | Transports, LinkManager, binary protocol, INI selection, output polling and publication. |
| `:ecu_shared` (`java_console/ecu_shared`) | Reusable firmware maintenance, host logging, and ECU services. |
| `:inifile` | INI parsing and models used to interpret firmware definitions. |
| `:models` and `:shared_io` | Shared configuration/output models, utilities, and preferences. |
| `:autoupdate` | Console software update checks and staged JAR replacement. |

The [configuration fields](../../firmware/integration/rusefi_config.txt),
[live-data definition](../../firmware/integration/LiveData.yaml), and
[TunerStudio INI template](../../firmware/tunerstudio/tunerstudio.template.ini)
feed the generators in `java_tools/configuration_definition`. They produce
matching C, Java, and INI artifacts. When a field layout or output channel
changes, the firmware and frontend need the regenerated definition together.
The signature selects the right definition; its page sizes and field offsets
determine how the frontend reads and writes ECU bytes.

For an unfamiliar frontend change, these are productive entry points:

| Question | Start at |
|---|---|
| Why is a device absent or misclassified? | `SerialPortScanner` and the hardware probes used by `ProductionConnectivity`. |
| Why does the app reject or fail to decode an ECU? | `BinaryProtocol.connectAndReadConfiguration`, board compatibility, and `RealIniFileProvider`. |
| Why is a live value stale or too slow? | `SensorCentral` demand, `OutputChannelPollPlan`, and the active tab's subscriptions. |
| Why did a tune change disappear or persist? | `TuningToolbarWidget`, the working image baseline, `uploadChangesWithoutBurn`, and `burn`. |
| Why is a tab unavailable while updating? | `DeviceSessionManager` state and `DevicePane` tab gating. |
| Why does a log omit a channel? | `SensorLogger.getOutputChannels` and the INI's raw output-field definitions. |

The [Java Gradle structure review](../java-gradle-structure-review.md)
explains project placement. The
[connectivity and UI testing guide](../java-connectivity-ui-unit-testing.md)
describes test seams and headless Swing constraints.

Build the Console JAR from the repository root with
`./gradlew :ui:shadowJar`; its configured output is
`console/rusefi_console.jar`. Run UI module tests with
`./gradlew :ui:test`.
