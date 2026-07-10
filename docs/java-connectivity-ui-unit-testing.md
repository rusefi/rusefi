# Java Console: Unit Test Approach for Connectivity Management & UI

Status as of 2026-07-08. Scope: `java_console` connectivity/flashing/session path
(`connectivity`, `io`, `shared_io`, `ui` modules) and the Swing UI around it.

## Why this document

The week of 2026-07-01..08 delivered a burst of connectivity features with real
brick-a-board consequences — DFU bootloader detection and in-session firmware
update (#9771), following a renumbered ECU port across OpenBLT/DFU reboots,
recovering a corrupted board with the same-session tune, and safeguards against
flashing firmware for the wrong board — followed by a deliberate testability
refactoring: the headless `connectivity` module, `ConnectivityContext`
dependency injection, the `PortScanner` and `HardwareProbes` seams, and the
first automated tests for this layer (`SerialPortScannerTest`,
`DeviceSessionManagerTest`, `ProgramSelectorTest`).

That refactoring history is recorded in `docs/java-connectivity-context-review.md`
(deleted once complete; retrievable via
`git show 148cb160d48^:docs/java-connectivity-context-review.md`). This document
is the follow-up: **how to test this layer going forward**, and a concrete
backlog ordered by how much production refactoring each test requires —
starting with tests that require none.

## The established approach

These patterns are already proven in the tree. New tests should follow them
rather than invent new machinery.

1. **Hand-written fakes, no new mocking machinery.** The project uses
   `mockito-core` (cannot mock final classes) and deliberately does *not* add
   `mockito-inline`. Seams are narrow interfaces + plain fakes:
   `FakePortScanner` (`java_console/connectivity/src/testFixtures/java/com/rusefi/FakePortScanner.java`,
   shared with `:ui` tests via `testFixtures(project(':connectivity'))`)
   implements `PortScanner` with scripted `fireHardwareChange(AvailableHardware)`
   and recorded `suspendCount`/`resumeCount`/`cachedPorts`/`invalidatedPorts`;
   `SerialPortScannerTest.FakeProbes` implements `SerialPortScanner.HardwareProbes`
   with scripted port lists, inspection results, device-probe flags and a fake
   clock. Mockito is fine for simple interface stubs (`mock(LinkManager.class)`
   in `ConnectionStatusIconTest`), but stateful collaborators get a fake.

2. **Constructor-injected seams, production wiring only at composition roots.**
   `SerialPortScanner(HardwareProbes, boolean startTimerOnFirstListener)` and
   `ConnectivityContext(PortScanner)` are the model. Production graphs are
   assembled in `ProductionConnectivity.CONTEXT` / `ConsoleUI` / `MassUpdater`;
   nothing else may reach for a singleton. When opening a new seam, copy this
   shape: extract the narrow interface the consumer actually uses, keep a
   production implementation wiring the old statics, change no call sites.

3. **Drive policy loops directly; never depend on real threads, sleeps or wall
   clock.** `SerialPortScannerTest` constructs the scanner with
   `startTimerOnFirstListener=false` and calls `findAllAvailablePorts()` per
   "cycle"; time for the device-probe throttle comes from `probes.now()`. Tests
   that need `Thread.sleep`, timing margins, or the real 300 ms `RecurringStep`
   loop are wrong — add a clock/step seam instead (see Tier 3).

4. **Resettable globals are tolerated, bounded, and reset around every test.**
   `DeviceSessionManagerTest` mutates `ConnectionStatusLogic.INSTANCE` and
   restores it in `@BeforeEach`/`@AfterEach`; `ConnectedEcuTargetTest` constructs
   a fresh `ConnectedEcuTarget` per test (the static holder was eliminated
   2026-07-08 — the process-wide instance now lives in `ConnectivityContext`,
   shared with the console `LinkManager` so `BinaryProtocol` can record the
   board at connect time) and writes `last_connected_board.txt` under the
   settings dir. This is acceptable (it serializes tests) but do not spread it —
   injecting a connection-status provider is a known later cleanup.

5. **Swing components are tested headlessly, off-screen.**
   `ConnectionStatusIconTest` (`java_console/ui/src/test/java/com/rusefi/ui/widgets/`)
   is the template:
   - instantiate the real component with mocked/fake constructor params;
   - inject state through the same channels production uses — constructor
     params, `JComponent` client properties (`bootloaderMode`, `isUpdating`),
     and the resettable `ConnectionStatusLogic.INSTANCE`;
   - assert **colors** by painting the `Icon` into a
     `BufferedImage(TYPE_INT_ARGB)` and checking pixel channel dominance
     (pure Java2D, needs no display), and **text** via `getToolTipText()` /
     title strings;
   - flush listener-driven updates with `SwingUtilities.invokeAndWait(() -> {})`.

   CI runs `./gradlew test` on `ubuntu-latest` *without* an X display
   (`xvfb-run` wraps only the TriggerImage step), so a test must never need a
   real frame/peer — off-screen painting and component getters only.

6. **`*Sandbox.java` files are manual hardware harnesses, not tests.** They have
   `main()` and no `@Test`, so `gradlew test` skips them automatically
   (`BltSwitchSandbox`, `InspectPortSandbox`, `FirmwareUpdateTabSandbox`, ~35
   more). Anything requiring a physical board or live serial port belongs there;
   automated tests must run hardware-free.

7. **Where tests live.** JUnit 5 is wired for every subproject via the root
   `build.gradle` (`allprojects` + `useJUnitPlatform()`). Pure decision logic
   tests sit next to their module (`shared_io` has `FindFileHelperTest`,
   `ConnectedEcuTargetTest`, `BoardCompatibilityTest`); the connectivity-policy
   tests (`SerialPortScanner*Test`, `DeviceSessionManagerTest`, etc.) now live in
   `connectivity/src/test`, with shared fakes such as `FakePortScanner` in
   `connectivity/src/testFixtures` so `:ui` tests can consume them too.

## Coverage today (2026-07-08)

| Area | Test | What it covers |
| --- | --- | --- |
| Scan policy | `connectivity` `SerialPortScannerTest` (13) | ttyS filtering, ECU caching vs Unknown retry, stale-node drop, unplug/replug eviction, listener-only-on-change, synthetic DFU port, device-probe throttle + last-known reuse, probe skip while connected, TCP not cached, `cachePort`/`invalidatePort`, ECU-first sort |
| Session state machine | `connectivity` `DeviceSessionManagerTest` (15) | OpenBLT/DFU detection + precedence + disappearance, initial-port pre-cache, re-cache on reconnect, CONNECTING/CONNECTED, snapshot to late subscribers, FLASHING + watchdog pause/resume + post-flash rescan choreography (`FakeJobExecutor`) |
| Flash mode/port decisions | `ui` `ProgramSelectorTest` | `mainButtonModeFor` (DFU/OpenBLT/live/offline), `resolveFlashPort` (bootloader wins, offline DFU>OpenBLT preference) |
| Tune merge recovery | `ui` `CalibrationsHelper*Test` | `decidePostMerge` all actions (#9756 crash path), partial-merge failed-field tracking, `isUiContext` |
| Firmware file targeting | `shared_io` `FindFileHelperTest` ×2 | `extractTargetFromFirmwareName`, `findXNumberOfFile` |
| Persisted board target | `shared_io` `ConnectedEcuTargetTest` | live → persisted-file → bundle fallback chain |
| Board compatibility | `shared_io` `BoardCompatibilityTest` | exact/wildcard/allowlist/QC-bypass matching |
| UI status | `ui` `ConnectionStatusIconTest` | red/green/purple priority, bootloader tooltip, offline tooltip (offline+disconnected, connected/bootloader precedence, property-change reaction), null tab pane |
| Update-available check | `ui` `MainFrameUpdateCheckTest` | `needsFirmwareUpdate` null/unparseable inputs, new-format hash compare, legacy date compare |
| Connectivity value types & helpers | `connectivity` `SerialPortCacheTest`, `AvailableHardwareTest`, `PortResultTest`, `SerialPortTypeTest`, `SerialPortScannerInspectPortsTest`, `RecurringStepTest` | cache eviction semantics, snapshot filtering/equality, PortResult identity contract, sort ranks, probe fan-out (dead port dropped, crash ⇒ Unknown), suspend-latch handshake — the Tier 1 batch, added 2026-07-08 |
| Flash brick guard | `ui` `MaintenanceUtilTest` | `confirmFirmwareMatchesBoard`: matching/case-insensitive target, unparseable name no-false-alarm, `_QC_` compatibility, mismatch declined/accepted via `UserConfirm` seam; `ensureFirmwareForConnectedTarget` unverified-declined + live-verified-skip; plus the `containsPattern` normalization (Tiers 2–3, 2026-07-08) |
| Firmware selection by board target | `shared_io` `FindFileHelperTargetSelectionTest` | newest-match wins, related-name (`_pro`) rejected, exact match beats newer foreign image, `older-fw` archive recovery, `.bin` variant, null/blank/unmatched ⇒ null; Tier 2, added 2026-07-08 |
| Device tab presentation decisions | `ui` `DevicePaneTest` | bootloader-state classification, DFU/OpenBLT guidance text (platform-aware), offline-capable tab lock list; Tier 2, added 2026-07-08 |
| Port classification pipeline | `ui` `EcuHardwareProbesInspectTest` | OpenBLT-first probing, stale-node/vanish ⇒ dropped, ECU-with/without-OpenBLT + calibrations, 3-attempt retry with between-attempts backoff, interrupt handling; Tier 3 item 1, added 2026-07-08 |
| Post-flash reconnect wait | `ui` `AbstractAutoFlashJobAwaitEcuPortTest` | `awaitEcuPort`: immediate ECU, follow renumbered port, bootloader-grace flicker vs persistent OpenBLT/DFU giveup, timeout, interrupt; Tier 3 item 3, added 2026-07-08 |
| Offline-mode UI state | `ui` `UIContextOfflineModeTest`, `TuningToolbarStateLabelTest` | `UIContext` offline flag default + setter + listener fan-out to every listener; Tuning toolbar dirty/offline label wording across all 4 offline×dirty combos (#9730, 2026-07-10) |
| OpenBLT job choreography | `ui` `OpenBltManualJobTest`, `OpenBltSwitchJobTest` | manual flash: suspend-before-flash / invalidate+resume-after (also on failure and flasher crash), restore-only-after-resume, ensure-firmware abort before touching the scanner; switch: reboot-then-`close()` ordering, never `disconnect()` (renumber-follow invariant), port released even with no `BinaryProtocol`; Tier 3 item 4, added 2026-07-10 |
| ECU-follow-across-reboot rule | `ui` `ConsoleUiEcuPortToFollowTest` | `ConsoleUI.ecuPortToFollow`: follows the single new ECU (incl. `EcuWithOpenblt`) once the old port vanished; stays put when disconnected-by-user, current port still present, never-connected, zero/multiple candidates, or candidate == current; Tier 3 item 6, added 2026-07-10 |

Not covered anywhere: `EcuHardwareProbes.inspect()`
classification/retry, `MainFrame` title composition, `StartupFrame` port-list
presentation and splash connection state machine.

The offline-tune UI glue added with #9730 (2026-07-10) is only unit-tested at
its pure edges (the two rows above). The Swing/IO-bound parts remain
sandbox/manual-only, consistent with the tiering: `PinoutPane.showOfflineBoard`
(needs the `pinouts_raw` board data + image render), `TuneManagementTab`'s
import-pair visibility gating, and the splash→offline-console frame-reuse
handoff (`StartupFrame.openOfflineConsole`, which the "not recommended" list
already excludes as splash-machine territory). If any of these regress, the
cheap seam is the same pattern used above — lift the decision (which board
signature, whether to show the import pair) out of the Swing wiring into a pure
helper and test that.

## Test backlog

Ordered by production-refactoring cost. Tier 1 items are pure additions; write
them first — they also lock in the behavior the recent bugfixes established
before further refactoring moves this code again.

### Tier 1 — no production change at all (IMPLEMENTED 2026-07-08)

Implementation notes: items 1–8 and 10 landed as the `connectivity/src/test`
suite plus `MaintenanceUtilTest` and extensions to `ProgramSelectorTest` /
`DeviceSessionManagerTest`; the missing `PortResult.hashCode` (item 3) was
added. Items 9, 11 and 12 turned out to be already covered —
`MainFrameUpdateCheckTest` covers `needsFirmwareUpdate` end to end,
`ConnectionStatusIconTest` already asserts bootloader-over-connected priority,
and `ConnectedEcuTargetTest.effectiveTarget_usesPersistedWhenNoLive` is exactly
the recovery-after-restart scenario.

Connectivity module value types and policy helpers (new tests can create
`connectivity/src/test`, or sit in `ui/src/test` next to the fakes; same
`com.rusefi` package gives package-private access):

1. **`SerialPortCache`** — `put`/`get` present-vs-absent, `retainAll` evicts
   exactly the not-retained keys, `invalidate`, overwrite of same port name.
   Currently only exercised through the scanner.
2. **`AvailableHardware`** — `getKnownPorts(type)` / `getKnownPorts(Set)`
   filtering, `isPortAvailable`, `isEmpty`, `equals`.
3. **`PortResult`** — `equals` compares only port+type (same port, different
   calibrations ⇒ equal), `isEcu()` for each `SerialPortType`. Note: custom
   `equals` with **no `hashCode` override** — a real finding to fix while
   writing the test.
4. **`SerialPortType` sort order** — assert the ordering the scanner's
   comparator relies on: OpenBlt(10) < Dfu(15) < Ecu/EcuWithOpenblt(20) <
   CAN(30) < Unknown(100).
5. **`SerialPortScanner.inspectPorts(...)` (static)** — inject a
   `Function<String, PortResult>` inspector, pass `null` probe-threads ref:
   null-returning inspector drops the port; throwing inspector yields
   `Unknown`; normal results collected. (Skip the deliberate-timeout case — it
   costs the hardwired 5 s sleep; that path needs the Tier 3 clock seam.)
6. **`RecurringStep.suspend()` latch state machine** (without `start()`):
   after `stop()` the returned latch is already at 0; before stop it is at 1
   and repeated `suspend()` returns the same latch until `resume()`.

Maintenance / flashing decision logic (`ui` and `shared_io`):

7. **`MaintenanceUtil.containsPattern`** — the `:`→`=` + whitespace
   normalization that DFU driver detection depends on:
   `"ConfigManagerErrorCode: 0"` must match pattern
   `"ConfigManagerErrorCode=0"`; plus plain substring and no-match cases.
   Package-private, zero deps, untested.
8. **`ProgramSelector.mainButtonModeFor` / `resolveFlashPort` cross-combos**
   — the corrupted-board + renumbering compounds the existing tests skip:
   offline with a stale ECU combo selection *and* a freshly appeared
   renumbered OpenBLT port ⇒ resolves to the OpenBLT port ⇒ `OPENBLT_MANUAL`;
   live connection while the combo selects a bootloader port.
9. **`MainFrame.needsFirmwareUpdate(RusEfiSignature, String)`** — already
   `static` and pure (hash/date comparison feeding the "update available" menu
   state). ~~No test exists~~ — correction: `MainFrameUpdateCheckTest` already
   covers it fully.
10. **`DeviceSessionManager` matrix rows** via existing `FakePortScanner`:
    an `EcuWithOpenblt` port that is a *real* port (not bootloader) while
    NOT_CONNECTED stays DISCONNECTED; stLink/PCAN flags alone don't change
    state.
11. **`ConnectionStatusIcon` additions** — any new color/tooltip/priority case
    uses the existing paint-to-image pattern as-is.
12. **`ConnectedEcuTarget`** — one explicit recovery-after-restart assertion:
    no live target this session (board stuck in bootloader) ⇒
    `isLiveTargetKnown()==false` while `effectiveTarget()` returns the
    persisted board.

### Tier 2 — test-fixture tricks or visibility-only changes (IMPLEMENTED 2026-07-08)

Implementation notes: item 13 landed in `MaintenanceUtilTest` (safe branches
only — the genuine-mismatch dialog still needs the Tier 3 confirm seam), item
14 as `FindFileHelperTargetSelectionTest` (a `@TempDir` input dir plus a
suite-unique target token so the real `older-fw` archive can be exercised and
cleaned safely), item 15 as `DevicePaneTest` after widening the three helpers
to package-private. The `ConnectedEcuTarget` DI migration (2026-07-08) made
item 13 simpler than described: the guard now takes the instance as a
parameter, so tests just construct one and use `setConnectedTargetForUnitTest`.


13. **`MaintenanceUtil.confirmFirmwareMatchesBoard` safe branches** — the
    brick guard added this week. Reachable today using the
    `ConnectedEcuTargetTest` recipe (construct a `ConnectedEcuTarget`, use
    `setConnectedTargetForUnitTest`, write `last_connected_board.txt`) and pass
    it with `UpdateOperationCallbacks.DUMMY`:
    matching target ⇒ true without dialog; unparseable firmware name
    (`extractTargetFromFirmwareName` ⇒ null) ⇒ true, no false alarm;
    compatible board via `BoardCompatibility` allowlist ⇒ true; no target
    known ⇒ true. Only the genuine-mismatch branch is Swing-blocked (Tier 3).
14. **`FindFileHelper.findSrecFileForTarget` / `newestMatchingTarget`** —
    `INPUT_FILES_PATH` is a mutable `public static String`; point it at a
    JUnit `@TempDir`. Scenarios: newest-by-mtime wins; a `uaefi` target must
    NOT pick a `uaefi_pro` file sitting in the same dir (file-selection layer
    of the mismatched-board guard); firmware only present in the `older-fw`
    archive is still found (foreign-download recovery); null/blank/no-match ⇒
    null.
15. **`DevicePane.bootloaderGuidance` / `isBootloaderState` /
    `isOfflineCapableTab`** — pure static helpers, currently `private`; make
    package-private and test directly (`bootloaderGuidance` branches on
    `FileLog.isWindows()`, so assert the platform-independent parts or both
    strings).

### Tier 3 — one small seam each, ranked by value

Each of these follows pattern #2: extract the narrow interface the consumer
already uses, keep a production impl wiring the old statics, change no call
sites.

1. **`EcuHardwareProbes.inspect()`** — DONE 2026-07-08. The three probes plus
   the retry backoff became the nested `PortProbe` interface (production
   `REAL_PROBE` wires the original private statics; 1-arg `inspect()` kept, no
   call sites changed). `EcuHardwareProbesInspectTest` (8 tests, scripted
   fake probe) covers: OpenBLT-first without ECU probing, stale-node ⇒ null,
   vanish-mid-detection ⇒ null, ECU with/without OpenBLT (calibrations ride
   along), found-on-third-retry, Unknown after max attempts with backoff
   between attempts only, and interrupt-during-backoff (stops retrying,
   restores the interrupt flag).
2. **`DeviceSessionManager` FLASHING state + watchdog choreography** — DONE
   2026-07-08. The nested `JobExecutor` interface (implemented by
   `SingleAsyncJobExecutor`, no call-site changes) plus constructor-injected
   watchdog pause/resume hooks (production overload wires
   `ConnectionWatchdog::pause/resume`). `DeviceSessionManagerTest` +4: job
   start ⇒ FLASHING (beats CONNECTED) + watchdog paused; job finish ⇒ scanner
   resume + forced rescan + watchdog resume + post-flash bootloader detection;
   already-running executor reports FLASHING immediately; null executor
   tolerated.
3. **`AbstractAutoFlashJob.awaitEcuPort`** — DONE 2026-07-08. Seam: nested
   `Clock` interface (`millis()` + `sleep()`); production wires `SYSTEM_CLOCK`,
   `awaitEcuPort` widened to package-private taking the clock. Test drives a
   `FakePortScanner`-backed `ConnectivityContext` whose hardware snapshot mutates
   per virtual `sleep`. `AbstractAutoFlashJobAwaitEcuPortTest` (8 tests): ECU
   already present returns without a poll; follows the ECU onto a renumbered port
   appearing after several polls (never the ghost pre-flash port); bootloader
   flicker within the 5 s grace then ECU ⇒ followed not abandoned; persistent
   OpenBLT/DFU past grace ⇒ null before timeout; nothing on the bus ⇒ null at
   timeout; interrupt mid-wait ⇒ null + interrupt flag restored; `EcuWithOpenblt`
   counts as connectable.
4. **OpenBLT job choreography** (`OpenBltManualJob` / `OpenBltSwitchJob`) — DONE
   2026-07-10. `OpenBltManualJob` grew a nested `FlashSteps` seam (ensure-firmware /
   flash / restore-calibrations; production `PRODUCTION_STEPS` wires the original
   `MaintenanceUtil` / `ProgramSelector` / `CalibrationsHelper` statics, no call
   sites changed) — the suspend/invalidate/resume choreography itself stays in
   `doJob` under test. `OpenBltSwitchJob` grew a nested `Rebooter` seam around the
   two `ProgramSelector.rebootToOpenblt` overloads. `OpenBltManualJobTest` (4,
   scripted steps record the fake scanner's state at each step) covers
   suspend-before-flash, invalidate+resume after success/failure/crash,
   restore-only-after-resume-and-only-on-success, and the ensure-firmware abort
   touching nothing. `OpenBltSwitchJobTest` (3, Mockito `InOrder` across the
   rebooter and a mocked `LinkManager`) locks reboot-before-`close()` and the
   `close()`-not-`disconnect()` invariant that keeps `isDisconnectedByUser()`
   false so reconnect can follow the renumbered port.
5. **`MainFrame.setTitle`** — extract a pure
   `composeTitle(isUpdating, bootloaderMode, connected, firmwareVersion, port,
   signature, offline, iniSignature)`; the priority ladder
   (UPDATING > BOOTLOADER > connected > OFFLINE > DISCONNECTED) is pure string
   logic currently welded to `ConsoleUI`/`FrameHelper`.
6. **`ConsoleUI` ECU-follow-across-reboot rule** — DONE 2026-07-10. Extracted
   exactly as sketched: package-private static
   `ConsoleUI.ecuPortToFollow(currentPort, commPorts, ecuPorts, disconnectedByUser)`
   returning `Optional<String>`; the scanner listener in the constructor now just
   gathers the four inputs and `ifPresent`-reconnects, and the
   not-gated-on-isConnected rationale moved to the predicate's javadoc.
   `ConsoleUiEcuPortToFollowTest` (8) covers: follows the single new ECU,
   `EcuWithOpenblt` counts as connectable; empty on disconnected-by-user,
   current-port-still-present (watchdog owns that reconnect), null current port,
   zero candidates, multiple (ambiguous) candidates, and candidate == current
   port (inconsistent scanner-vs-OS snapshot).
7. **`MaintenanceUtil` confirm seam** — DONE 2026-07-08. The nested
   `UserConfirm` interface with package-private overloads (public methods
   delegate to the `confirmOnEdt` JOptionPane production impl).
   `MaintenanceUtilTest` +5 covers the previously Swing-blocked branches:
   mismatch declined ⇒ flash blocked (message names both targets), mismatch
   explicitly accepted ⇒ proceeds, verified match never asks, unverified
   target declined ⇒ fail closed, live-verified bundle-matching target skips
   confirmation. Note: tests pick a board target outside the bundled
   `board_compatibility` allowlist to reach the mismatch branch
   deterministically.
8. **`LinkManager.restart()` gate** — the renumber-follow invariant
   (`disconnect()` blocks restart, port must be present in `getCommPorts()`).
   A `SerialPortSource` interface exists but is not injectable per instance;
   add a settable source. Cheapest immediate assertion (no seam):
   `disconnect()` then `restart()` is a no-op.
9. **`DfuFlasher` H7-vs-F4/F7 driver decision** — extract the one-line
   `isH7(effectiveTarget, is_h7Property)` predicate out of the
   Windows/process-exec method.
10. **`OpenbltDetectorStrategy`** protocol decoders — pure byte logic blocked
    on the concrete `IncomingDataBuffer`; needs a constructible/fake buffer in
    the `io` module. Moderate effort; do after the above.

### Explicitly not recommended

- **`PlainSerialPortScanner.findEcu`** — spins a real `LinkManager` per port;
  no realistic seam short of rewriting it. Cover via sandbox/manual testing.
- **`ConnectionWatchdog`** — static singleton on a Swing `Timer`; needs
  de-singletoning first (fold into Tier 3 item 2's seam when touched).
- **`SerialPortScanner.suspend()` drain and `RecurringStep` 300 ms loop
  timing** — real-thread choreography; only testable after routing the wait
  through `probes.now()` / an injected sleeper. Don't write sleep-based tests
  for these.
- **Adding a DI framework or `mockito-inline`** — rejected during the
  refactoring for good reasons (small hand-wired graph; fakes are cheaper and
  more readable). Still holds.
- **`StartupFrame` splash connection state machine** — deeply tied to a live
  `JFrame`, `LinkManager` and `ConnectionStatusLogic`; extract pieces
  opportunistically (e.g. `updateConnectButtonState`'s port-type predicate)
  rather than attempting the whole machine.

## Conventions for new tests

- **Reflection is prohibited in unit tests.** No `setAccessible`,
  `getDeclaredField`, `getDeclaredMethod` or any other reflective access to
  production internals — it silently breaks on rename/refactor and hides the
  real coupling. Instead, give the production class an explicit test seam:
  a `...ForUnitTest` / `...ForTests` method (existing precedent:
  `ConnectedEcuTarget.setConnectedTargetForUnitTest`,
  `LinkManager.setBinaryProtocolForTests`, `RateCounter.getSizeForUnitTest`),
  or widen a `private` member to package-private with a comment naming the
  test (as `CalibrationsHelper.isUiContext`). (`InvocationTargetException`
  from `SwingUtilities.invokeAndWait` is not reflection — it's fine.)
- JUnit 5 (`org.junit.jupiter`), one behavior per test, names describing the
  scenario (`unpluggedPortIsEvictedAndReinspectedOnReplug`).
- No `Thread.sleep`, no wall-clock timing margins, no real serial ports, no
  network, no display. If a test wants any of those, the production code needs
  a seam (Tier 3) or the scenario belongs in a `*Sandbox`.
- Reset any global you touch (`ConnectionStatusLogic.INSTANCE`,
  `FindFileHelper.INPUT_FILES_PATH`, the `last_connected_board.txt` file) in
  `@BeforeEach`/`@AfterEach`, restoring the prior value.
- Prefer extending `FakePortScanner`/`FakeProbes` over new mocks; if a fake
  grows useful across suites, promote it out of the test class (as
  `FakePortScanner` already was).
- Swing assertions go through off-screen painting and component getters only;
  flush the EDT with `SwingUtilities.invokeAndWait(() -> {})` before asserting
  listener-driven state.
