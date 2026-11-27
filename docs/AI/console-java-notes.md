# Console (Java/Swing) Notes

Swing invariants, headless tests, keyboard shortcuts, themes and Java version constants.

### Console Swing invariant: one glass pane per frame

A `JFrame` has exactly one glass pane (`JFrame.setGlassPane` delegates to `getRootPane().setGlassPane`). In the console TWO subsystems both grab the main frame's glass pane: `MainFrame`'s `FrameOverlay`s (config error, unsaved-tune, firmware-update prompts) and `TabbedPanel.installGlassPane()` (the loading/updating/critical-error `statusGlassPane`). They only coexist because `installGlassPane` runs once early (on the tabbedPane's `SHOWING_CHANGED` `invokeLater`) and `showOverlay` save/restores `previousGlassPane`. Any `FrameOverlay` shown *during* start-up races that install and gets silently replaced by the invisible status pane while `activeOverlay` still points at the orphan - the overlay is logically "up" but never on screen (issue #10219, config-error overlay shown at connect time). If you add or move a start-up overlay, make it displacement-aware: re-assert when `frame.getGlassPane() != yourOverlay` (see `ConfigErrorOverlayController`'s `displaced` predicate). A single glass-pane arbiter would remove the whole class of bug.

### Swing startup readiness in tests

An enabled control can be a transient state between startup operations. In
`LLMTab`, opening a saved account enables controls before an `invokeLater`
starts model loading and disables them again. Separate EDT calls to observe
enabled and `doClick()` can therefore lose the click. The cancellation test
hit this in CI check 113887619777; wait for the fake transport's initial model
request plus enabled controls before clicking. That report was attached to
the Windows check suite, but its failing XML and logs came from the Linux
console workflow (run 37950058647); inspect the producing workflow rather
than inferring the host from the report association.

### Java tests run headless on CI

The console workflow's `./gradlew test` step runs on Ubuntu without
`xvfb-run`, so Java reports `GraphicsEnvironment.isHeadless()`; a Windows
desktop run is not headless. Drag and drop setup differs: headless Swing
installs no `DropTarget` (`getDropTarget()` returns null after
`setTransferHandler`), and `JList.setDragEnabled(true)` throws
`HeadlessException`. A synthetic mouse press on a `JTree` throws too: its
look and feel reads the toolkit's menu shortcut key, so send test clicks to a
plain component when only the press itself matters. Guard such calls, and
reproduce CI locally with `JAVA_TOOL_OPTIONS=-Djava.awt.headless=true ./gradlew :ui:test`.
`Component.dispatchEvent` does not deliver key or mouse events to a component that is not
displayed (key events are retargeted to the absent focus owner), so a test can silently exercise
nothing. Invoke the component's own listeners or bound actions directly (see
`SettingsSearchPopupTest.theTabBarBoxOpensTheSearchWithWhatWasTyped`).

### Option-pane dialogs keep their first size

`JOptionPane.showConfirmDialog` sizes its dialog once and does not grow it. If the content's
preferred size then increases (a row shown, a taller preview), `GridBagLayout` is short of space and
drops every row to its minimum size: a `JScrollPane` collapses to about one row. Keep such content's
preferred size constant while it is edited: give scroll panes a minimum equal to their preferred
size, reserve fixed-size slots for changing content, and switch alternatives with `CardLayout`
rather than `setVisible` (see `CustomIndicatorDialog`, `contentKeepsItsSizeWhileEditing`).

### Swing clicks on drag-enabled components

AWT sends no `MOUSE_CLICKED` once the pointer moved between press and
release, and `DragSource.getDragThreshold()` can be as small as 2 px
(measured on a Windows desktop), so on a drag-enabled `JList` a slightly
moving click starts drag and drop and never reaches the component as a
click. Act on release within a few pixels of the press, and treat a drag
dropped back on its origin row as a click (see `LogViewerPanel.rowMouse`
and `dropOnList`).

### Native Swing screenshots under WSLg

Xvfb without a window manager verifies shaped-window input but cannot reveal
compositor decorations. Under WSLg, Java Robot screen capture may return black
even while the windows are visible on Windows; capture the Windows desktop to
inspect those artifacts. Use the console's FlatLaf theme as well as the default
theme when testing detached windows.

### .msq values are rounded to the setting's digits

`Msq.valueOf` writes each scalar and table cell with `StringFormatter.niceToString`
at the INI `digits`, so a tune saved as `.msq` and read back is not byte-identical
to the ECU image (689.4757 becomes 689.48). Byte comparisons between an `.msq` and
a live image report such values as changed. Restore points compare and restore
only values that differ as shown (`RestorePointDiff.asShown`,
`[tag:restore_point_rounding]`).

## Console keyboard shortcuts

Console calibration dialogs use ScrollablePanel, which tracks viewport width
but not height. A table can therefore retain its preferred height inside a
much taller viewport. Space-dependent table UI must consult the outer
JViewport extent (and table offset within its view), not just the table's own
height; re-evaluate after ancestor resize/layout as well as local resize.

- Whenever Console keyboard shortcuts are added, changed, or removed, update the user-facing legend in `java_console/ui/src/main/java/com/rusefi/ui/console/ShortcutsDialog.java` in the same change. Describe the action, focus/context restrictions, and relevant modifiers; keep the corresponding button/menu tooltips and accelerators consistent with the legend.

## Console theme files

The console's FlatLaf look is defined in `java_console/ui/src/main/resources/com/rusefi/ui/theme/`
(`[tag:console_theme]`, registered in `UiHelper.configureLaf()`): `FlatLaf.properties` for
sizes/shapes shared by all themes, `FlatLightLaf.properties` and `FlatDarkLaf.properties` for
palette variables and colour keys. Do not put colour keys in the shared file: FlatLaf's built-in
`FlatLightLaf.properties` assigns most of them and wins. Add new look settings there rather than
as `UIManager.put` calls, so they follow the active theme.
Users pick the theme in File > Appearance (Light, Dark, Follow system; `ConsoleTheme`,
`[tag:console_dark_mode]`). It is read once in `UiHelper.configureLaf()` and applies on the next
start, because many panels read their colours when they are built; `-Dconsole.theme=dark|light`
overrides the saved choice. Every new colour key goes into both theme files (`ThemeParityTest` checks
equal key sets and contrast). `LightThemeSnapshotTest` pins every resolved light colour, so a
refactor of the theme files must leave it green; an intended light change updates
`light-theme-snapshot.txt` from the copy the failing run writes to `ui/build/console-theme/`.
FlatLaf's Linux defaults make `TitlePane.closeHoverForeground` and
`TitlePane.closePressedForeground` aliases of `TitlePane.foreground`; on other
platforms they remain stock white. `LightThemeSnapshotTest` derives the Linux
expectations from the pinned title colour and omits these aliases from its
replacement snapshot, so the same baseline works across platforms.
`tint()` mixes with white and glows on dark backgrounds: the dark file uses `shade()` instead.
Hard-coded `Color` values in `ui/src/main/java` are counted by `HardcodedColorRatchetTest`
(`[tag:fixed_color]`): take colours from the theme, or mark one that must look the same in every
theme with that tag and a reason; lower `hardcoded-colors-baseline.txt` when removing them.
The value colour scale of tables, curves and 3D views (`CalibrationColorPalette`) comes from the
`Console.Calibration.low/middle/high` theme keys: light cells in the light theme, dark cells in the dark
theme, the same cool-to-warm hue order in both. Each scale keeps one text colour across all its cells;
the live-point marker drawn over it stays `[tag:fixed_color]`. The table's axis values and selection use `Console.Calibration.axisBackground/axisForeground/selection`; a selected cell keeps its value colour under a tint, with an outline only on the block's outer edges (`CalibrationColorPalette.selectionEdges`, shared with curve cells); 3D facets and selected curve points use the same selection colour. The curve plot is themed (`Console.Tuning.curve.*`, background `Console.Tuning.surface.background`), not a black scope. While a curve is edited its plot scale only grows (`CurveWidget.growAxis`); a tight refit after each edit made the unedited points jump. A dragged point keeps its grab offset. The table has 2 px intercell gaps and no grid lines (re-applied in `updateUI`), and `changeSelection` scrolls to the cell including its gap.
Colours inside images are invisible to that test: load the `icons/tuning/*48.png` icons through
`TuningIcons.load`, which lightens them in the dark theme (`TuningIconsTest` checks their contrast).
Java's logical `Font.MONOSPACED` renders as Courier New on Windows; use `ConsoleFonts.mono()` for
monospaced console text. Toolbar controls share `JoinedButtons.HEIGHT` so a row lines up.
For a rounded panel, set a `FlatLineBorder` with an arc: `FlatPanelUI` then paints the background
rounded too. Do not also set `FlatClientProperties.STYLE` (`arc`) on the panel, because applying a
style drops a border set earlier (see `WizardStyle.styleRounded`).
On Windows 10+ FlatLaf resizes custom-decorated windows through its native window border,
which it skips for `setUndecorated(true)` windows; its Swing resizer is only installed when
the native border is unsupported. An undecorated FlatLaf dialog is therefore a fixed size on
Windows: leave it decorated and let FlatLaf apply the decorations (`DetachedPanelTitleBar`).

## Java Version Constants

TunerStudio's bundled runtime is Java 8. Gradle's Java 11 toolchain with
source/targetCompatibility=8 still permits newer JDK API calls, producing
Java 8 bytecode that crashes at runtime. The INI module uses --release 8 and
has a java8Test task; pass -Pjava8Executable=<TunerStudio runtime8/bin/java.exe>
to test with the installed runtime. The plugin launcher compares both exact
file size and timestamp against the server, so a locally rebuilt cached body
JAR is replaced on the next online plugin launch until the fix is published.

Java sources are compiled as UTF-8 via `options.encoding` in the root `build.gradle`
(`[tag:java_source_encoding]`, guarded by `SourceEncodingTest`). Without it, Windows
builds garble non-ASCII literals. On a Windows checkout, `SensorLoggerTest` (temp
directory still locked at cleanup), `FirmwareSourceCodeDownloaderTest` (two
rollback-failure cases that expect an `IOException` Windows does not raise) and
`CalibrationPanelWorkspaceTest` fail independently of the change being tested.
`ConsoleThemeTest` asserts light-theme colours but `UiHelper.configureLaf()` honours the saved
console theme, so with "Follow system" on a Windows desktop in dark mode it fails; run the Java
tests with `-Dconsole.theme=light` (e.g. in `JAVA_TOOL_OPTIONS`) to pin the theme.

- When modifying Java code (`java_console/`, `java_tools/`), bump `UiVersion.CONSOLE_VERSION` in `java_tools/version/src/main/java/com/rusefi/UiVersion.java` to the current date in `YYYYMMDD` format as part of the same change (no-op if it already shows today's date).
- `Autoupdate.AUTOUPDATE_VERSION` in `java_console/autoupdate/src/main/java/com/rusefi/autoupdate/Autoupdate.java` is a separate, manually managed version for the updater executable. Bump it (same `YYYYMMDD` format) only when the change affects updater behavior, i.e. code that runs inside `rusefi_autoupdate.exe`.

The standalone updater overwrites its own JAR during extraction. Classes first
used after that point can fail to load even though they are packaged correctly.
Installed launchers use `installation.properties` and bypass the default launcher
lookup, so they do not necessarily preload the same classes as extracted bundles
(#10368: `OsUtil`). Resolve launch-time dependencies before extraction; test in a
fresh JVM so earlier tests cannot hide lazy-loading failures.
