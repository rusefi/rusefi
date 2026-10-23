# Java frontend setup wizard

The rusEFI Console setup wizard configures a connected ECU through a sequence
of Swing panels. Each completed step uploads and burns its settings together
with an ECU-backed progress flag. The Console can resume at the first
unfinished step, revisit completed steps, or show a standalone prompt such as
VIN entry. See [Java frontend app](frontend-app.md) for the surrounding
connection, INI, and tune-editing architecture.

The implementation lives in
[`com.rusefi.ui.wizard`](../../java_console/ui/src/main/java/com/rusefi/ui/wizard).
It uses the live `BinaryProtocol` configuration image and matching INI. Normal
offline tune editing does not provide a live wizard session.

## Entry points and launch controls

[`ConsoleUI`](../../java_console/ui/src/main/java/com/rusefi/ConsoleUI.java)
hosts the full wizard in a `CardLayout` alongside the Console tabs. The
**Launch Wizard** button requires a connected ECU. Its primary highlight
indicates unfinished supported steps; launching manually resumes those steps
without clearing their flags. If every supported step is complete, the
completion card appears and completed steps remain accessible in the strip.

On connection, ConsoleUI first checks standalone prompts. If none opens, it
can call `WizardContainer.startWizard(true)` for unfinished setup. It checks
that the connection still exists, the controller image is loaded, and a
wizard is not already showing before opening it.

| Control | Scope |
|---|---|
| `show_launch_wizard` | `UiProperties` setting controlling visibility of the manual launch button; defaults to true. |
| `auto_launch_wizard` in `UiProperties` | Enables automatic launch of the full setup wizard; defaults to true. |
| `auto_launch_wizard` in the persistent Console configuration root | Additional full-wizard auto-launch gate; defaults to true. **Don't Show Again** saves false here. |
| ECU `wizard*` fields | Persist completion of individual steps independently of the Console preferences. |

An automatically launched full wizard offers **Cancel** and **Don't Show
Again** at the top right of the header. Manual and standalone launches offer
**Exit Wizard** there. Exiting does
not undo completed uploads. The full-wizard preferences do not suppress
standalone prompts, which have their own catalog predicates.

Every step advances from the footer. Its right side shows the step's own Skip,
only on steps that can be skipped, and a blue primary **Continue**. The steps
keep their listeners and enabled rules (`WizardStep.getContinueButton`,
`getSkipButton`). Cylinders and Firing Order save on a single click of a
choice, so they show a footer hint instead of Continue. Those choices are
outlined in the accent colour (`wizardChoice` style) because a click saves.
Cylinder counts 7, 9 and 16 only show a message, so they are greyed out
(`wizardChoiceMuted`). Exit actions are neutral. The firmware dialogs below
keep green confirmation and red cancel or destructive buttons.

[`StartupFrame`](../../java_console/ui/src/main/java/com/rusefi/StartupFrame.java)
hosts a compact `WizardContainer` for standalone prompts after connecting,
before handing off to the main Console. It does not run the full numbered
wizard at this point.

## Catalog, progress, and navigation

[`WizardCatalog`](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/WizardCatalog.java)
declares the ordered metadata. A
[`WizardStepDescriptor`](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/WizardStepDescriptor.java)
has one of two shapes:

- A flagged step has a progress-field name, title, and board applicability
  predicate. `WizardContainer.startWizard` constructs its panel explicitly.
- A standalone step has no progress-field name. Its applicability,
  `needsAttention`, `autoLaunch`, and factory determine whether to create it.

The flagged catalog and the container's indexed panel construction must stay
in the same order. The catalog does not instantiate full-wizard panels.

[`WizardContainer`](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/WizardContainer.java)
includes a flagged step only when it applies to the board and its field is
an `EnumIniField` in the active INI. Missing or unsupported flags do not block
setup. Completion means the flag's raw ordinal is `1`. These flags are
defined in [rusefi_config.txt](../../firmware/integration/rusefi_config.txt)
and reach the frontend through the generated INI; they are not local UI
preferences.

[`WizardProgressPanel`](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/WizardProgressPanel.java)
numbers only visible steps and marks each as current, completed, or upcoming.
Completed entries can be selected to revisit them; upcoming entries cannot
be used to jump ahead. After a save, the container finds the earliest
unfinished visible step, including when the user revisits an earlier step.
The strip scrolls when its entries do not fit.

## Full setup sequence

Rows follow catalog order; displayed numbers depend on the applicable steps.
The board predicate parses the connected firmware signature's bundle target
and matches `HdConstants.HD_81` case-insensitively. Cylinders, Firing Order,
MAP Sensor, and Crank Trigger are excluded for that board family. All other
rows still require their progress flag to exist in the active INI.

| Step and implementation | Progress flag | Settings and behavior |
|---|---|---|
| [Cylinders](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/NumberOfCylindersPanel.java) | `wizardNumberOfCylinders` | Writes `cylindersCount` and `twoStroke`. The 7, 9, and 16 buttons display an informational overlay instead of completing the step. |
| [Firing Order](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/FiringOrderPanel.java) | `wizardFiringOrder` | Filters the INI's `firingOrder` choices by cylinder count. One- and two-cylinder selections use implicit orders and hide this step. |
| [Base VE Table](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/VeTableWizardStep.java) | `wizardVeTable` | Embeds `VeTableGeneratorPanel`; Continue applies the generated preview as a modified image, while Skip keeps the current table. The wizard moves the panel's apply and close buttons into its footer. Both complete the step. |
| [Engine Displacement](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/DisplacementPanel.java) | `wizardDisplacement` | Requires confirmation of total `displacement` in litres, greater than 0 and no more than 65. Checks that the value remains valid after INI encoding. |
| [MAP Sensor](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/MapSensorTypePanel.java) | `wizardMapSensorType` | Selects `map_sensor_hwChannel` and `map_sensor_type`; Custom also edits the two voltage/pressure calibration points. |
| [TPS](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/TpsPanel.java) | `wizardTps` | Selects `tps1_1AdcChannel` and closed/WOT endpoints (`tpsMin`, `tpsMax`), entered manually or captured from live voltage. |
| [CLT Sensor](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/CltSensorPanel.java) | `wizardCltSensor` | Selects `clt_adcChannel` and an INI thermistor preset or three custom temperature/resistance points, with optional pull-up and linear-mode controls. |
| [Crank Trigger](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/CrankTriggerPanel.java) | `wizardCrankTrigger` | Embeds the INI `trigger_primary` dialog through `CalibrationDialogWidget`; saves edits on Continue, or keeps the current settings on Skip. |
| [Cam Trigger](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/CamTriggerPanel.java) | `wizardCamTrigger` | Embeds the INI `trigger_cams` dialog with the same save/skip behavior. |
| [Ignition Outputs](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/OutputAssignmentPanel.java) | `wizardIgnitionOutputs` | Edits `ignitionPins1`, `ignitionPins2`, etc. for the configured cylinder count; offers per-output bench tests. |
| Injector Outputs (`OutputAssignmentPanel`) | `wizardInjectorOutputs` | Edits `injectionPins1`, `injectionPins2`, etc. with the same assignment and test workflow. |
| [Injector Flow](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/InjectorFlowPanel.java) | `wizardInjectorFlow` | Uses `hardcoded_injectors` presets when that enum exists; otherwise edits `injector_flow` and selects cc/min through `injectorFlowAsMassFlow` when available. |

Saving the Cylinders step clears `wizardFiringOrder`,
`wizardIgnitionOutputs`, and `wizardInjectorOutputs`, even when confirming
the same count. For one or two cylinders, the same save assigns the implicit
`firingOrder` ordinal (`0` or `8`) and sets `wizardFiringOrder` again, provided
both INI fields exist as enums. Larger counts require explicit firing-order
confirmation.

Completing or skipping Base VE Table clears `wizardDisplacement`. Displacement
therefore requires its own confirmation after the VE decision. An INI that
exposes displacement setup without `wizardVeTable` still offers the
displacement step.

## Step results and ECU writes

[`WizardStep`](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/WizardStep.java)
provides a title, panel, optional progress flag, completion callback, and
`onShow`/`onHide` lifecycle hooks.
[`AbstractWizardStep`](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/AbstractWizardStep.java)
supplies common styling, callback dispatch, and enum-value helpers.
[`WizardConfig.snapshot`](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/WizardConfig.java)
returns the current protocol, INI, and controller image only when all three
are available. The image is a reference to current state; callers clone it
before applying edits.

[`WizardStepResult`](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/WizardStepResult.java)
can carry a field/value pair, a modified image, both, or neither. A result
with no edits still completes a flagged step; this is how Skip works.
The container handles completion as follows:

1. Require an available protocol, INI, and controller image.
2. Clone the supplied modified image, or clone the current controller image.
3. Apply any field/value pair through `ConfigurationImageGetterSetter`.
4. Clear dependent progress flags, set this step's flag, and apply an implicit
   firing order when appropriate.
5. Submit `BinaryProtocol.uploadChanges` to the LinkManager I/O executor.
6. Return to the Swing event dispatch thread to refresh progress and advance.

[`BinaryProtocol.uploadChanges`](../../java_console/io/src/main/java/com/rusefi/binaryprotocol/BinaryProtocol.java)
writes changed bytes, calls `burn()`, and updates its cached image. There is
no final wizard-wide commit: each completed step issues its own burn. This
also applies to a Skip that only changes flags. Exiting the wizard does not
roll back those writes. Completion flags record accepted steps, not a
hardware validation result. Hardware reconfiguration and reboot requirements
still follow [the firmware's normal burn behavior](../hardware-reinit-and-power-cycle.md).

Two panels perform additional I/O:

- Output **Test** uploads and burns all visible assignments before sending
  `TS_IO_TEST_COMMAND`. Its payload contains a big-endian 16-bit command
  category and one-based output index. The test action requires a live
  connection, all selections, and the bench-test acknowledgement checkbox.
  The panel tells the user to stop the engine; its button-enable logic does
  not check RPM. Testing alone does not set the step's completion flag.
- Injector preset selection queues `BOARD_ACTION_INJECTOR_PRESET` after the
  normal step upload so firmware reads the selected `hardcoded_injectors`
  value. It then rereads the controller image because the command changes
  settings on the ECU; a stale cached image could overwrite those settings
  during a later save.

## Sensor panels and live data

MAP, TPS, and CLT panels refresh their configuration on `onShow`, subscribe
to `SensorCentral`, and release subscriptions on `onHide`. The container
calls the hide hook when switching steps, completing, exiting, or starting
another wizard. Preserve this lifecycle when adding panels so hidden steps
do not retain live-output demand.

| Panel | Output-channel subscriptions |
|---|---|
| MAP | `rawMap`, `MapPressure`, `isMapValid` |
| TPS | `rawTps1Primary`, `TPSValue` |
| CLT | `rawClt`, `coolantTemperature` |

Live readings describe the ECU's applied configuration. Editing a local
selection or calibration does not apply it until the step saves. TPS accepts
either endpoint direction, requires voltages in 0–5 V and a span of at least
0.1 V. Custom MAP calibration requires finite, distinct voltage and pressure
points with voltages in 0–5 V. CLT presets come from the active INI's
`clt_thermistor` dialog; its panel also validates custom calibration data.

Enum choices come from the board's INI. Blank and `INVALID` options are
filtered, and unavailable persisted enum ordinals can require a new
selection. A supported progress flag alone does not guarantee that a panel's
required configuration fields or dialog are present.

## Standalone VIN prompt and startup handoff

The catalog's standalone entry applies to the HD81 board family when
`GenericFieldsPanel.anyFieldEmpty` finds `vinNumber` blank, whitespace-only,
or `INVALID`. Missing fields are ignored. The prompt has no progress flag
and `startSingleStep` hides the numbered strip and exits after saving.

[`GenericFieldsPanel`](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/GenericFieldsPanel.java)
edits scalar/string fields and enums; arrays are unsupported. Field-specific
behavior belongs to `WizardFieldPolicy`.
[`VinFieldPolicy`](../../java_console/ui/src/main/java/com/rusefi/ui/wizard/VinFieldPolicy.java)
prefills a blank VIN from `vin_number_prefix`, removes whitespace during
entry, caps input at 17 characters, and requires exactly 17 characters to
save. This is length validation, not a VIN checksum check. Save is disabled
while disconnected.

[`UIContext.shouldSkipStandaloneWizard`](../../java_console/ui/src/main/java/com/rusefi/ui/UIContext.java)
records offered descriptors per `BinaryProtocol` instance. The shared context
prevents an exited startup prompt from immediately appearing again during
the main-window handoff. A new protocol connection can offer it again; this
dismissal is not persisted.

`StartupFrame.continueAfterSplashConnection` resumes the interrupted handoff
after either exit or a completed save. It verifies that the original protocol
is still connected and still has its image; otherwise it returns to startup.
It clears the exit callback before invoking the continuation, so an exit
followed by a queued save completion cannot hand off twice.

## Tests and extension points

The [wizard test package](../../java_console/ui/src/test/java/com/rusefi/ui/wizard)
covers catalog order, progress filtering and navigation, dependent flags,
implicit firing order, panel validation, subscription cleanup, and output
test ordering. `CltSensorPanelTest.containerStepOrderMatchesCatalog` checks
the explicitly assembled panels against catalog order.
[StartupWizardHandoffTest](../../java_console/ui/src/test/java/com/rusefi/StartupWizardHandoffTest.java)
covers save/exit continuation, prompt deduplication, disconnection, and queued
completion. [LaunchWizardHighlightTest](../../java_console/ui/src/test/java/com/rusefi/LaunchWizardHighlightTest.java)
covers the manual launch button's presentation.
`WizardButtonStyleTest` checks the footer Continue and Skip of every setup and
standalone step, the one-click choice styles, the fixed header and footer
layout across launch modes, and the firmware dialogs' colours using the
Console theme.

Run focused tests from the repository root:

```bash
JAVA_TOOL_OPTIONS=-Djava.awt.headless=true ./gradlew :ui:test \
  --tests 'com.rusefi.ui.wizard.*' \
  --tests 'com.rusefi.StartupWizardHandoffTest' \
  --tests 'com.rusefi.LaunchWizardHighlightTest'
```

[`WizardSandbox`](../../java_console/ui/src/test/java/com/rusefi/ui/wizard/WizardSandbox.java)
is a visual test main using the real container, a generated
`firmware/tunerstudio/generated/rusefi_f407-discovery.ini`, and an in-memory
protocol whose uploads replace its cached image. Launch it with the UI test
runtime classpath from the repository root or `java_console`; optional
arguments select width and height. It does not simulate live sensor values
or hardware behavior. An INI without wizard flags skips the full flow.

When adding a full-wizard step, keep its firmware flag, catalog descriptor,
container construction/indexes, and order tests aligned. Use INI field
accessors instead of hardcoded offsets, clone images before editing, and
pair live subscriptions with cleanup. Standalone prompts instead use a
catalog predicate and factory and share the per-connection suppression path.
See [Java connectivity and UI testing](../java-connectivity-ui-unit-testing.md)
for headless Swing fixtures and explicit test seams.

`DfuBoardSelectionPanel`, `FirmwareRollbackPanel`, `EmergencyWipePanel`, and
`FirmwareUpdateBlockedPanel` share this package and styling but are hosted
by maintenance workflows. They are not numbered setup steps or progress
flags. The TunerStudio plugin's `BackgroundWizard` is also a separate path.
