# rusEFI tuner frontend

rusEFI tuner is a Java/Swing application. End users run a native `.exe` launcher
(created with [launch4j](http://launch4j.sourceforge.net/), see the `misc/console_launcher`
folder), which is just a thin wrapper that locates a JRE and starts the bundled JAR.

For the standalone Java ChatGPT sign-in and streaming terminal experiment, see
[LLM tab sandbox](llm-tab-sandbox.md) (`./gradlew :ui:runLLMTabSandbox`).

## ChatGPT troubleshooting

The experimental **Troubleshooting** tab provides browser ChatGPT sign-in and
read-only tools using the existing ECU connection in online, non-log-viewer
sessions. See
[Console troubleshooting assistant](llm-troubleshooting.md) for usage, limits,
packaging and developer validation.

## Optional startup tabs

Implement `com.rusefi.ui.plugins.StartupTabProvider` to add tabs to the startup screen.
It has the same `getTitle()` and `createTab(UIContext)` methods as `ConsoleTabProvider`.
The two interfaces are discovered separately: startup providers add tabs after
Update Firmware, Manage Tunes and Connect; console providers add tabs to the main console.

Place the fully qualified implementation class name (one per line) in
`META-INF/services/com.rusefi.ui.plugins.StartupTabProvider` in the plugin JAR's resources.
The class must be public with a public no-argument constructor, and the JAR must be on
the console's runtime classpath (or its classes and resources included in the console JAR).

`createTab` runs on the Swing event dispatch thread once per startup frame and receives
the shared `UIContext`. It must work before an ECU connection or INI is available.
Tabs are appended before restoring the saved tab selection and, like Manage Tunes and
Connect, are disabled while a firmware operation is in progress. Provider failures are
logged; a failure while creating a tab does not prevent later providers from adding theirs.

## Launcher chain

- `rusefi_updater.exe` (configured by `misc/console_launcher/rusefi_updater.xml`) is a launch4j
  wrapper. It does **not** wrap the JAR (`<dontWrapJar>true</dontWrapJar>`); instead it `chdir`s
  into the `console` directory and runs `console/rusefi_console.jar`.
- The **rusEFI tuner** Windows installer provides desktop and Start menu entries
  with the new product name. Its installed launchers retain the technical filenames
  `rusEFI-Grab-Fresh-And-Update-ECU.exe` and `rusEFI-Update-ECU.exe`;
  `installation.properties` selects the launcher for relaunch.
- The default visible product name is **rusEFI tuner**; configured white labels
  display as **\<brand\> tuner**. Technical JAR names and saved settings are unchanged.
- `console/rusefi_console.jar`'s manifest `Main-Class` points at `com.rusefi.Launcher`.
- `com.rusefi.Launcher#main` is invoked with empty args. The single merged launcher handles both
  the auto-update flow and the console UI; there is no longer a separate "basic UI" entry point
  (an old `basic-ui` first argument is accepted and ignored for backwards compatibility).

`Launcher#main` does, in order:

1. Verifies Java 11+.
2. Initializes logging (`FileLogger.init()`) and registers a persistence shutdown hook.
3. Calls `Autoupdate.finalizePendingUpdate()` — see "rusefi_console_pending.jar" below.
4. Runs any requested command-line `ConsoleTools` and returns if one handled the args.
5. Starts a daemon background thread running `Autoupdate.checkForUpdate()`.
6. Calls `ConsoleUI.startUi(...)` to build the Swing UI on the AWT event thread.

## rusefi_console_pending.jar

A running JVM keeps `rusefi_console.jar` locked, so the auto-updater cannot overwrite the very
JAR it is executing from. The update is therefore applied across two process lifetimes using a
staging file named `rusefi_console_pending.jar`
(`Autoupdate.PENDING_CONSOLE_JAR`), kept next to `rusefi_console.jar` in the `console/` directory.

Lifecycle after the user explicitly starts installation:

1. **Stage** — `runManualUpdate` downloads the bundle ZIP and unzips everything
   *except* the console JAR (the running JAR cannot be replaced). The new console JAR is extracted
   under the name `rusefi_console_pending.jar` by `extractConsoleJarAsPending(...)`, so the running
   JAR is left untouched. `runManualUpdate` then reports an `UPDATED` outcome.
2. **Relaunch** — when the user accepts the restart, the window closes through its normal guarded
   exit path (unsaved tune and log-edit prompts) and `Autoupdate.startUpdatedConsole()` starts a new
   `java -jar` process. If a `rusefi_console_pending.jar` exists it launches *from* it (so the new
   code runs immediately), passing `-DSKIP_ONE_INSTANCE_CHECK=true` so the new process does not trip
   the "already running" guard before the old instance releases its server socket. The old JVM exits
   only after the new process has started; otherwise the window stays open.
3. **Finalize** — early in the new process, `Launcher#main` calls `Autoupdate.finalizePendingUpdate()`.
   If the currently running JAR is `rusefi_console_pending.jar`, it copies itself over
   `rusefi_console.jar` and deletes the pending file. This must run before any other code locks
   class files. On Windows the delete fails because the process is running from that file. The next
   launch's `hasPendingUpdate()` recognizes a pending JAR identical to `rusefi_console.jar` as already
   applied, deletes it and does not ask for another restart.

The launch-time availability check runs whatever the auto-update setting. It reads server metadata
but does not download or extract the update. When `AutoupdateProperty` and the
`autoupdate_bundle=true` configuration flag both enable auto-update, an available update is offered
once per session in a dialog; otherwise it is offered only by a banner and the update button or menu
item. Holding Caps Lock skips a download started through the manual installation path
(`isSkipUpdater()`).

## JFrame sequence: auto-update bundle + auto-connect to ECU

This is the end-to-end window flow for the common case: a freshly installed bundle that
auto-connects to a single attached ECU.

1. **`rusefi_updater.exe` → `Launcher#main`** — no JFrame yet. `finalizePendingUpdate()` swaps in any
   staged JAR, then a background `checkForUpdate` thread starts. This checks server metadata only;
   it does not download, extract or restart the application.

2. **`ConsoleUI.startUi` → `awtCode`** (on the AWT event thread):
   - One `SoftwareUpdateCoordinator` wraps the launch-time result and owns the update state for the
     entire UI session. It is the only path that starts an application update, so the startup and
     main windows cannot launch duplicate checks or downloads.
   - If a port/baud was passed on the command line, it skips the splash and builds `ConsoleUI`
     directly with the same coordinator.
   - Otherwise (the normal bundle case) it constructs the splash window:
     `StartupFrame startupFrame = new StartupFrame(...)`. The availability result is passed to the
     startup update controls, and `startupFrame.showUi()` is called.

3. **`StartupFrame` (the splash JFrame)** — built on `FrameHelper`. It runs a serial-port scanner;
   each hardware change calls `applyKnownPorts(...)`, which populates the Connect combo box.

4. **Auto-connect** — in `applyKnownPorts`, if exactly one ECU port is found (and it is the first
   scan and no firmware job is running), `autoConnect(target)` fires automatically:
   - The Connect button shows "Connecting…" and a `ConnectionStatusLogic.Listener` is registered.
   - A `splash-connect` thread calls `uiContext.getLinkManager().startAndConnect(...)`.

5. **Connection established** — when the listener reports connected *and* the binary protocol's
   controller configuration is available, `onSplashConnected(target)` runs on the AWT thread:
   shows "Connected to … — launching console…", hands the live `LinkManager` to the firmware/tune
   tabs, optionally auto-creates a TunerStudio project, runs any applicable auto-launch wizard step,
   then calls `connect(target)`.

6. **Hand-off to the main window** — `connect(...)` calls `prepareForHandoff()`, which saves the
   selected tab, persists config and stops the splash status timers while leaving the maximized
   JFrame and shared port scanner alive for the console. Because the splash already holds a live
   connection, `ConsoleUI` is created with `alreadyConnected=true` so it does **not** restart the
   `LinkManager`. The same update coordinator is passed through this hand-off, preserving available,
   installing, failed and restart-required state without another network request.

7. **`ConsoleUI` → `MainFrame`** — `ConsoleUI` builds the tabbed UI and `MainFrame` reuses the
   already-visible splash JFrame through `FrameHelper`. `mainFrame.getFrame().showFrame(rootPanel)`
   replaces its content with the main console UI.

8. **Update available** — the first active window claims and shows one application-level popup with
   **Update now** and **Later** actions. Its copy explicitly distinguishes the
   desktop application from ECU firmware, and the startup status strip sits above every tab instead
   of inside **Update ECU firmware**. Nothing is installed until the user chooses **Update now**.
   Closing the popup behaves like **Later**, which replaces the popup with a compact, actionable
   full-width warning strip for the current session instead of making the known update disappear.
   The strip uses the same prominent banner treatment in the startup and main windows. The application
   continues offering the latest available release until it is installed. The manual installation
   path stages the update without closing the application. When
   staging completes, the startup screen offers **Restart now** and **Restart later**; the main
   console shows the same choices in a persistent banner and changes the Actions menu entry to
   **Restart Application**. Restarting from the main console passes through the normal unsaved-tune
   and active ECU-operation guards before launching the staged console.

On a later launch, `Autoupdate.hasPendingUpdate()` checks for a readable
`rusefi_console_pending.jar` containing the console launcher class next to the normal console JAR
before any network request. A valid pending artifact restores the same restart-required controls on
both the startup screen and the main console; empty, partial or invalid files are ignored. A process
already running from `rusefi_console_pending.jar` does not show the reminder: that process has
already applied the staged update, even if Windows could not delete the file.

### Failure / variations

- **No ECU found / multiple ports** — auto-connect does not fire; the splash stays up and the user
  picks a port and clicks Connect, which routes through the same `connect(...)` → `ConsoleUI` path.
- **Auto-connect fails or drops** — `onSplashConnectFailed` / `onSplashDisconnected` revert the
  splash to manual mode (auto-connect stays disabled for the rest of the session).
- **Offline tune** — `openOfflineConsole(...)` reuses the splash JFrame and keeps the scanner running,
  so plugging in an ECU later transitions the already-open console online without a second window.
