# ConnectivityContext: Dependency-Injection & Testability Review

Status as of 2026-07-08. Scope: `java_console/ui` connectivity/flashing path —
`ConnectivityContext`, `SerialPortScanner`, their relationship to `UIContext`,
and everything they feed (`StartupFrame`, `DeviceSessionManager`, `DevicePane`,
`ProgramSelector`, `maintenance/*`, `maintenance/jobs/*`).

## TL;DR — is this dependency injection yet?

**The plumbing is done; the injection is not.** `ConnectivityContext` is threaded
as a constructor/method parameter through ~30 classes, and it is the *only*
place in main code that touches `SerialPortScanner.INSTANCE` — a single choke
point, which is exactly the shape you want. But it is an `enum` singleton whose
`getSerialPortScanner()` is hard-wired to `SerialPortScanner.INSTANCE` (also an
enum). Only one instance of either can ever exist, so nothing can actually be
substituted: every "injected" `ConnectivityContext` is the same global, and no
test can hand a consumer fake hardware. Today it is a *token being passed
around*, not a dependency being injected. The good news: because the parameter
threading is already ~95% complete, converting the token into a real injectable
seam is a small, mechanical change (Phase 1–2 below).

## UIContext: the working DI precedent

The console already contains a context object that *does* work as dependency
injection: `com.rusefi.ui.UIContext`. Despite its "key frontend app state
singleton" javadoc, it is not a singleton at all — it is a plain class with no
static `INSTANCE`, holding per-session state (`LinkManager` instance,
`SensorLogger`, `IniFileState`, offline-mode flag, config-image listeners). It
is constructor-injected into ~55 main-code classes, and — the proof that the
pattern pays off — real JUnit tests construct it freely: `new UIContext()`
appears throughout `CalibrationDialogWidgetTest`, `MainMenuTreeWidgetTest`,
`TriggerPaneTest`, `WizardStepDescriptorTest` and a dozen sandboxes. The
offline-tune feature (`[tag:offline_tune]`) exists precisely because a
`UIContext` can be built without an ECU.

`ConnectivityContext` should end up shaped like `UIContext`: a plain class,
constructed at the composition roots, holding its collaborator as an instance
field. The two contexts should stay **separate**, because their lifecycles
differ:

- `UIContext` is per-session/per-window — `ConsoleUI.java:84` builds a fresh
  one per console instance, each with its own `LinkManager`.
- `ConnectivityContext`/`SerialPortScanner` is process-wide — one scan thread
  watches the OS port list for all windows/sessions. Folding it into
  `UIContext` would either spawn duplicate scanners or reintroduce a hidden
  global.

Where a class needs both (e.g. `StartupFrame(ConnectivityContext, UIContext)`),
passing both explicitly is correct and already the established style.

## Current state

### What ConnectivityContext is

`java_console/ui/src/main/java/com/rusefi/ConnectivityContext.java` — 13 lines:

```java
public enum ConnectivityContext {
    INSTANCE;
    public SerialPortScanner getSerialPortScanner() { return SerialPortScanner.INSTANCE; }
    public AvailableHardware getCurrentHardware() { return getSerialPortScanner().getCurrentHardware(); }
}
```

History: introduced for "console closes if we have non-rusEFI serial ports"
(#7989, commit `7379c1e2ec6`, message: *"only:probably proper way forward"*),
extended in #8601 (`2ebe1d460fc`). The intent was clearly to create a DI seam;
the conversion of call sites is essentially finished, the seam itself was never
opened.

### What's already good

- **Single choke point**: `SerialPortScanner.INSTANCE` is referenced from
  exactly one main-code location — inside `ConnectivityContext` itself. No one
  bypasses the context to reach the scanner singleton.
- **Parameter threading is ~95% done**: `ProgramSelector`, `StartupFrame`,
  `DeviceSessionManager`, `BasicUpdaterPanel`, `TuneManagementTab`,
  `ImportTuneControl`/`ImportTuneFileChooser`, `TrimsTab`, `FirmwareUpdateTab`,
  all of `maintenance/` (`CalibrationsHelper`, `CalibrationsUpdater`,
  `BinaryProtocolExecutor`, `DfuFlasher`, `TuneUploader`,
  `OutputChannelsHelper`) and all `maintenance/jobs/*` except one receive
  `ConnectivityContext` via constructor or method parameter.
- **Value types are test-friendly**: `AvailableHardware` and `PortResult` are
  plain constructible classes in the `connectivity` module — fakes need no
  mocking framework.

### Remaining `ConnectivityContext.INSTANCE` call sites

| Site | Classification |
|---|---|
| `ConsoleUI.java:157,349,499` | Composition root — acceptable (this is where the object graph is built) |
| `MassUpdater.java:219` (`main`) | Composition root — acceptable |
| `MassUpdater.java:193` | **Leak** — the class already receives `connectivityContext` in its constructor (line 39) but the OpenBLT job creation reaches for the global anyway |
| `DevicePane.java:48,88` | **Leak** — constructor builds `ProgramSelector` from the global; `render()` falls back to the global. `DevicePane` already receives `DeviceSessionManager`, which holds the context |
| `OpenBltManualJob.java:31,54` | **Leak** — the only job class not taking the context in its constructor (its siblings `OpenBltAutoJob`/`DfuAutoJob`/`AbstractAutoFlashJob` all do) |
| `FirmwareUpdateTabSandbox`, `BltSwitchSandbox` (test sandboxes) | Composition roots for manual hardware testing — acceptable |

### Static state behind the seam

Even with a perfect `ConnectivityContext` seam, `SerialPortScanner` itself is
where the untestable coupling lives:

- `SerialPortScanner` is an **enum singleton** with mutable state: `portCache`,
  `knownHardware`, `listeners`, `probeThreads`, suspend/resume state, and the
  device-probe throttle (`lastDeviceProbeMs`, `lastDfuConnected`, …). This
  state machine (cache retention, Unknown-retry policy, listener notification
  on change, suspend-waits-for-probes) is real logic with real bug history
  (`[tag:better_ux_for_flashing]`) and **none of it is unit-testable** because
  the instance cannot be constructed fresh, and its collaborators are statics.
- Hard-wired statics called from inside the scan loop:
  - `LinkManager.getCommPorts()` — OS port enumeration
  - `ConnectionStatusLogic.INSTANCE.isConnected()` — global connection state
  - `DfuFlasher.detectSTM32BootloaderDriverState()`, `StLinkFlasher.detectStLink()`,
    `MaintenanceUtil.detectPcan()` — shell out to PowerShell on Windows
  - `CalibrationsHelper.readCurrentCalibrationsWithoutSuspendingPortScanner()`,
    `BufferedSerialIoStream.openPort()`, `TcpConnector.getAvailablePorts()`
- Module-layering debt (already noted in the class comment): the scanner lives
  in `ui` and depends on `maintenance/*`, so it cannot move to the
  `connectivity` module as-is.

### Testability today

- **Zero automated tests** exercise anything through `ConnectivityContext` or
  `AvailableHardware`: no `*Test.java` under `java_console/ui/src/test`
  references either type. The only consumers in test scope are two manual
  sandboxes that require real hardware. The entire flashing/session state
  machine (`DeviceSessionManager`, `ProgramSelector` menu logic,
  `StartupFrame` port-list updates, job suspend/resume choreography) is
  covered only by hand testing.
- **The types cannot be mocked**: both `ConnectivityContext` and
  `SerialPortScanner` are enums (final classes), and the project uses
  `mockito-core:4.11.0` (`dependencies.gradle:11`) without `mockito-inline`,
  which cannot mock final classes. So even the classes that dutifully accept a
  `ConnectivityContext` parameter can only ever be handed the real global.

## Improvement plan

Ordered so each phase is independently useful, small, and low-risk. Phases 1–2
are mechanical; Phase 3 opens the seam; Phase 4 makes the scanner itself
testable.

### Phase 1 — close the leaks (no behavior change)

1. `OpenBltManualJob`: accept `ConnectivityContext` in the constructor like its
   sibling jobs; update the call sites that create it.
2. `MassUpdater:193`: use the `connectivityContext` field instead of `INSTANCE`.
3. `DevicePane`: take `ConnectivityContext` as a constructor parameter (or
   expose it from `DeviceSessionManager`, which already holds it) and drop both
   `INSTANCE` references.

After this, `INSTANCE` appears only in `ConsoleUI`, `MassUpdater.main` and the
sandboxes — i.e. only at composition roots.

### Phase 2 — make ConnectivityContext a real class (the UIContext shape)

Convert the enum to a plain class that *owns* its scanner reference, matching
the pattern `UIContext` already proves out:

```java
public class ConnectivityContext {
    public static final ConnectivityContext INSTANCE = new ConnectivityContext(SerialPortScanner.INSTANCE);
    private final SerialPortScanner scanner;
    public ConnectivityContext(SerialPortScanner scanner) { this.scanner = scanner; }
    ...
}
```

Call sites don't change (`ConnectivityContext.INSTANCE` still works at the
roots). This alone doesn't unlock fakes (the scanner is still an enum) but it
removes the "only one instance can ever exist" property and is a prerequisite
for Phase 3.

### Phase 3 — interface out the scanner (unlocks consumer testing)

Consumers use a narrow surface of `SerialPortScanner`: `getCurrentHardware()`,
`addListener()`, `suspend()`, `resume()`, `cachePort()`, `invalidatePort()`,
`stopTimer()`/`restartTimer()`. Extract that as an interface (e.g.
`PortScanner`, could live in the `connectivity` module), have
`SerialPortScanner` implement it, and make `ConnectivityContext` hold the
interface type.

Payoff: a ~40-line hand-written `FakePortScanner` (scripted
`AvailableHardware` sequences, recorded suspend/resume/cachePort calls) makes
the currently-untested state machines testable with plain JUnit — no
mockito-inline needed, consistent with the project's existing fake/sandbox
style. First tests to write, in value order:

1. `DeviceSessionManager` — session-state transitions as hardware
   appears/disappears/changes type (highest regression risk area,
   `[tag:better_ux_for_flashing]`).
2. `ProgramSelector.apply()` — menu contents per `AvailableHardware` shape
   (DFU present, OpenBLT present, ECU-with-OpenBLT, nothing).
3. Job suspend/resume choreography — `BinaryProtocolExecutor` and
   `AbstractAutoFlashJob` call suspend/invalidate/resume in the right order and
   always resume on failure paths.

### Phase 4 — make the scanner itself testable

Split `SerialPortScanner`'s *policy* (cache retention, Unknown-retry,
listener-on-change, probe throttling, suspend-waits-for-in-flight-probes) from
its *probes* (the static hardware calls). Convert the enum to a class whose
constructor takes a small probe-provider (port lister, port inspector, device
probes, ECU-connected supplier), with a production factory wiring the current
statics. `SerialPortScanner.INSTANCE` becomes a static field. Then the scan
state machine gets direct unit tests with scripted probe results — including
the historically buggy areas: stale-port handling, Unknown non-caching,
reconnect cache pre-population, probe-thread interruption on suspend.

### Phase 5 (optional, later) — layering

Once Phase 4 decouples the probes, the scanner policy + `PortScanner`
interface + `ConnectivityContext` can move from `ui` to the `connectivity`
module, resolving the layering debt noted in the scanner's class comment.
Not required for testability; do it only if/when module boundaries matter.

### Explicitly not recommended

- **Adding a DI framework** (Guice/Dagger/Spring): the object graph is small
  and hand-wired at two roots; a framework adds weight without benefit here.
- **Adding `mockito-inline` to mock the enums in place**: it would paper over
  the design instead of fixing it, and hand-written fakes are cheaper and more
  readable for this surface.
