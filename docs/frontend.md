# rusEFI console frontend

The rusEFI console is a Java/Swing application. End users run a native `.exe` launcher
(created with [launch4j](http://launch4j.sourceforge.net/), see the `misc/console_launcher`
folder), which is just a thin wrapper that locates a JRE and starts the bundled JAR.

## Launcher chain

- `rusefi_updater.exe` (configured by `misc/console_launcher/rusefi_updater.xml`) is a launch4j
  wrapper. It does **not** wrap the JAR (`<dontWrapJar>true</dontWrapJar>`); instead it `chdir`s
  into the `console` directory and runs `console/rusefi_console.jar`.
- `console/rusefi_console.jar`'s manifest `Main-Class` points at `com.rusefi.Launcher`.
- `com.rusefi.Launcher#main` is invoked with empty args. The single merged launcher handles both
  the auto-update flow and the console UI; there is no longer a separate "basic UI" entry point
  (an old `basic-ui` first argument is accepted and ignored for backwards compatibility).

`Launcher#main` does, in order:

1. Verifies Java 11+.
2. Initializes logging (`FileLogger.init()`) and registers a persistence shutdown hook.
3. Calls `Autoupdate.finalizePendingUpdate()` — see "rusefi_console_pending.jar" below.
4. Runs any requested command-line `ConsoleTools` and returns if one handled the args.
5. Starts a daemon background thread running `Autoupdate.runSilentUpdate(...)`.
6. Calls `ConsoleUI.startUi(...)` to build the Swing UI on the AWT event thread.

## rusefi_console_pending.jar

A running JVM keeps `rusefi_console.jar` locked, so the auto-updater cannot overwrite the very
JAR it is executing from. The update is therefore applied across two process lifetimes using a
staging file named `rusefi_console_pending.jar`
(`Autoupdate.PENDING_CONSOLE_JAR`), kept next to `rusefi_console.jar` in the `console/` directory.

Lifecycle:

1. **Stage** — the background `runSilentUpdate` downloads the bundle ZIP and unzips everything
   *except* the console JAR (the running JAR cannot be replaced). The new console JAR is extracted
   under the name `rusefi_console_pending.jar` by `extractConsoleJarAsPending(...)`, so the running
   JAR is left untouched. `runSilentUpdate` then reports a non-null "please restart" message.
2. **Relaunch** — when the user accepts the restart, `Autoupdate.relaunchConsole()` starts a new
   `java -jar` process. If a `rusefi_console_pending.jar` exists it launches *from* it (so the new
   code runs immediately), passing `-DSKIP_ONE_INSTANCE_CHECK=true` so the new process does not trip
   the "already running" guard before the old instance releases its server socket. The old JVM exits.
3. **Finalize** — early in the new process, `Launcher#main` calls `Autoupdate.finalizePendingUpdate()`.
   If the currently running JAR is `rusefi_console_pending.jar`, it copies itself over
   `rusefi_console.jar` and deletes the pending file. This must run before any other code locks
   class files. (On Windows the delete may fail silently; it is then cleaned up on the next update.)

Auto-update only runs when it is enabled both by `AutoupdateProperty` and the
`autoupdate_bundle=true` configuration flag. Holding Caps Lock at startup skips the update download
(`isSkipUpdater()`).

## JFrame sequence: auto-update bundle + auto-connect to ECU

This is the end-to-end window flow for the common case: a freshly installed bundle that
auto-connects to a single attached ECU.

1. **`rusefi_updater.exe` → `Launcher#main`** — no JFrame yet. `finalizePendingUpdate()` swaps in any
   staged JAR, then the background `runSilentUpdate` thread starts (no UI of its own; it reports
   progress through a `bannerCallback`).

2. **`ConsoleUI.startUi` → `awtCode`** (on the AWT event thread):
   - If a port/baud was passed on the command line, it skips the splash and builds `ConsoleUI`
     directly.
   - Otherwise (the normal bundle case) it constructs the splash window:
     `StartupFrame startupFrame = new StartupFrame(...)`. The background updater's `bannerCallback`
     is wired to `startupFrame.restartConsole()`, and `startupFrame.showUi()` is called.

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

6. **Hand-off to the main window** — `connect(...)` calls `disposeFrameAndProceed()`, which saves the
   selected tab, persists config, **disposes the splash JFrame**, stops the splash status timers and
   the port scanner. Because the splash already holds a live connection, `ConsoleUI` is created with
   `alreadyConnected=true` so it does **not** restart the `LinkManager`.

7. **`ConsoleUI` → `MainFrame`** — `ConsoleUI` builds the tabbed UI and `MainFrame` creates the main
   application JFrame (again via `FrameHelper`). `mainFrame.getFrame().showFrame(rootPanel)` displays
   it. This is the main console window the user works in.

8. **Pending restart (if an update was downloaded while running)** — if the background updater
   completes a download, the wired `bannerCallback` invokes `startupFrame.restartConsole()`, which
   disposes the frame and calls `Autoupdate.relaunchConsole()` to start the staged
   `rusefi_console_pending.jar` (see above), restarting the whole sequence with the new code.

### Failure / variations

- **No ECU found / multiple ports** — auto-connect does not fire; the splash stays up and the user
  picks a port and clicks Connect, which routes through the same `connect(...)` → `ConsoleUI` path.
- **Auto-connect fails or drops** — `onSplashConnectFailed` / `onSplashDisconnected` revert the
  splash to manual mode (auto-connect stays disabled for the rest of the session).
- **Offline tune** — `openOfflineConsole(...)` disposes the splash but keeps the scanner running, so
  plugging in an ECU later transitions the already-open console online without a second window.
