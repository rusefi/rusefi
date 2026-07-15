package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageMetaVersion0_0;
import com.opensr5.ConfigurationImageWithMeta;
import com.opensr5.ini.EventTriggerModel;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.FrontPageModel;
import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.SensorCentral;
import com.rusefi.maintenance.OfflineEditMigration;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.widgets.TextGaugeStrip;
import com.rusefi.ui.widgets.StatusPanel;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;
import com.rusefi.ui.widgets.tune.IndicatorPanel;
import com.rusefi.ui.widgets.tune.MainMenuTreeWidget;
import com.rusefi.ui.widgets.tune.TuningToolbarWidget;

import com.devexperts.logging.Logging;
import com.rusefi.ui.basic.TuneOperationStatusPanel;

import javax.swing.*;
import javax.swing.Action;
import java.awt.*;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Consumer;

/**
 * Andrey Belomutskiy, (c) 2013-2026
 */
public class TuningPane {
    private static final Logging log = Logging.getLogging(TuningPane.class);
    private static final String TUNING_CARD = "tuning";
    private static final String LOAD_CARD = "load";

    private final JPanel content = new JPanel(new CardLayout());
    private final JPanel tuningContent = new JPanel(new BorderLayout());
    private final UIContext uiContext;
    private final MainMenuTreeWidget left;
    private final TuningToolbarWidget toolbar;
    private final TuneOperationStatusPanel loadStatusCard;
    private final CalibrationDialogWidget right;
    private TextGaugeStrip gaugeStrip;
    /** Accumulated tune edits across all dialogs for this session. Field so offline seeding can set it. */
    private final AtomicReference<ConfigurationImage> sessionImage = new AtomicReference<>();
    /** Single-element holder for the currently displayed menu key. Field so the connect handler can access it. */
    private final AtomicReference<String> currentKey = new AtomicReference<>();
    /** [tag:offline_tune] The .ini the offline tune was loaded with — used to migrate edits if a different-signature ECU connects. */
    private IniFileModel offlineIni;
    private volatile boolean firmwareUpdateInProgress;
    /** Fired when the user picks "Show in Pinout" on a pin-enum field. Wired from ConsoleUI after construction. */
    private Consumer<String> navigateToPinout;
    private Runnable showTuningTab = () -> { };

    public TuningPane(UIContext uiContext) {
        this(uiContext, null, null);
    }

    public TuningPane(UIContext uiContext, Node config) {
        this(uiContext, null, config);
    }

    /**
     * @param uiContext       live context (BinaryProtocol, LinkManager)
     * @param initialBaseline optional baseline image for discard (used in offline mode when a tune is pre-loaded)
     */
    public TuningPane(UIContext uiContext, ConfigurationImage initialBaseline, Node config) {
        this.uiContext = uiContext;
        left = new MainMenuTreeWidget(uiContext);

        right = new CalibrationDialogWidget(uiContext);
        JScrollPane rightScrollPane = new JScrollPane(right.getContentPane());

        ConfigurationImage baseline = initialBaseline;
        BinaryProtocol initialBp = uiContext.getBinaryProtocol();
        if (baseline == null && initialBp != null && initialBp.getControllerConfiguration() != null) {
            baseline = initialBp.getControllerConfiguration().clone();
        }
        StatusPanel loadStatusPanel = new StatusPanel(250);
        loadStatusCard = new TuneOperationStatusPanel(loadStatusPanel, this::showTuningContent);
        toolbar = new TuningToolbarWidget(
            uiContext,
            right,
            currentKey,
            sessionImage,
            baseline,
            loadStatusPanel,
            () -> {
                showTuningTab.run();
                showLoadProgress();
            },
            failed -> {
                showTuningTab.run();
                showLoadResult(failed);
            }
        );

        if (config != null) {
            gaugeStrip = new TextGaugeStrip(uiContext, config.getChild("gauge_strip"));
        }

        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, left.getContentPane(), rightScrollPane);
        splitPane.setResizeWeight(0.3);

        left.setOnSelect(subMenu -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            ConfigurationImage ecuImage = (bp != null) ? bp.getControllerConfiguration() : null;
            // [tag:offline_tune] Offline mode has no ECU image; fall back to the session image seeded
            // from the loaded tune. Bail only if we have neither source — else the right pane stays blank.
            if (ecuImage == null && sessionImage.get() == null) {
                return;
            }

            // Flush any in-progress debounce window before loading a new section.
            toolbar.flushBeforeNavigate();

            // On first navigation, seed the session image from the live ECU state.
            // On subsequent navigations, carry forward whatever the user has edited so far
            // so that changes made in dialogs, tables, or curves are not lost when opening another.
            ConfigurationImage pending = right.getWorkingImage();
            if (pending != null) {
                sessionImage.set(pending);
            } else if (sessionImage.get() == null) {
                sessionImage.set(ecuImage.clone());
            }

            currentKey.set(subMenu.getKey());
            right.update(subMenu.getKey(), uiContext.iniFileState.getIniFileModel(), sessionImage.get());
        });

        // All edit events (dialog fields, table cells, curve drags) flow through onConfigChange.
        // Text fields fire per-keystroke; the toolbar widget coalesces them into one undo point.
        right.setOnConfigChange(image -> {
            toolbar.onEdit(sessionImage.get());
            // Clone because workingImage is mutated in-place by further edits.
            sessionImage.set(image.clone());
            left.refreshExpressions(image);
            uiContext.fireConfigImageChanged(image);
        });

        // Right-click "Show in Pinout" on a pin-enum combo fires the current value upward.
        right.setOnShowInPinout(value -> {
            if (navigateToPinout != null) navigateToPinout.accept(value);
        });

        // Per-trigger edge-detection state: maps trigger index → previous boolean value.
        // ConcurrentHashMap because onSensorUpdate runs on the protocol thread while
        // the disconnect handler clears it from the EDT.
        final ConcurrentHashMap<Integer, Boolean> triggerPrevValues = new ConcurrentHashMap<>();

        SensorCentral.ResponseListener eventTriggerListener = () -> {
            IniFileModel ini = uiContext.iniFileState.getIniFileModel();
            if (ini == null) return;
            List<EventTriggerModel> triggers = ini.getEventTriggers();
            if (triggers.isEmpty()) return;
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null) return;

            Map<String, Double> channels = SensorCentral.getInstance().getOutputChannelMap();

            for (int i = 0; i < triggers.size(); i++) {
                EventTriggerModel trigger = triggers.get(i);
                Double val = ExpressionEvaluator.tryEvaluateWithContext(trigger.getExpression(), channels);
                boolean current = val != null && val != 0.0;
                Boolean prev = triggerPrevValues.put(i, current);
                if (prev == null) {
                    // First observed frame: initialize state without firing.
                    continue;
                }
                if (!prev && current) {
                    final int page = trigger.getPage();
                    log.info("EventTrigger fired: page=" + page + " expr=[" + trigger.getExpression() + "]");
                    if (page == 1) {
                        final int pageSize = ini.getMetaInfo().getPageSize(0);
                        final String signature = ini.getMetaInfo().getSignature();
                        uiContext.getLinkManager().submit(() -> {
                            log.info("EventTrigger: reading page 1 from ECU");
                            ConfigurationImageWithMeta result = bp.readFullImageFromController(
                                    new ConfigurationImageMetaVersion0_0(pageSize, signature));
                            if (result.isEmpty()) {
                                log.error("EventTrigger: page 1 read returned empty — refresh aborted");
                                return;
                            }
                            ConfigurationImage newImage = result.getConfigurationImage();
                            bp.setConfigurationImage(newImage);
                            log.info("EventTrigger: page 1 refreshed successfully");
                            SwingUtilities.invokeLater(() -> {
                                toolbar.onDisconnect();
                                assert newImage != null;
                                sessionImage.set(newImage.clone());
                                String key = currentKey.get();
                                if (key != null) {
                                    right.update(key, ini, sessionImage.get());
                                }
                            });
                        });
                    } else {
                        log.info("EventTrigger: page " + page + " refresh not yet implemented — skipping");
                    }
                }
            }
        };
        SensorCentral.getInstance().addListener(eventTriggerListener);

        // When the ECU disconnects (e.g. after a firmware flash or board swap), drop stale
        // undo/redo state so the next connection starts fresh, but preserve sessionImage
        // and baselineImage so the user can continue editing offline.
        // Without clearing sessionImage, the old board's image would be used as the diff
        // baseline in uploadChangesWithoutBurn — but that's fine because on reconnect
        // we re-seed from the ECU below.
        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> {
            if (!isConnected) {
                SwingUtilities.invokeLater(() -> {
                    toolbar.onDisconnect();
                    right.reset();
                    // Preserve sessionImage for offline editing — do NOT set to null
                    triggerPrevValues.clear();
                });
            } else {
                SwingUtilities.invokeLater(() -> {
                    triggerPrevValues.clear();
                    BinaryProtocol bp = uiContext.getBinaryProtocol();
                    if (bp == null || bp.getControllerConfiguration() == null) {
                        return;
                    }

                    // [tag:offline_tune] If we were editing offline and have pending changes, ask the
                    // user what to do before the freshly-read ECU image overwrites their edits. If the
                    // user chose to write/migrate, the helper takes over and we keep their edits.
                    ConfigurationImage offlineEdits = sessionImage.get();
                    if (uiContext.isOfflineMode() && toolbar.hasUnsavedChanges(offlineEdits)
                            && reconcileOfflineEditsOnConnect(bp, right, offlineEdits)) {
                        return;
                    }

                    // Default: adopt the ECU's current tune.
                    uiContext.setOfflineMode(false);
                    toolbar.onEcuConnected();
                    sessionImage.set(bp.getControllerConfiguration().clone());
                    toolbar.setBaselineImage(sessionImage.get().clone());
                    String key = currentKey.get();
                    if (key != null) {
                        right.update(key, uiContext.iniFileState.getIniFileModel(), sessionImage.get());
                    }
                    // Refresh config-image-driven panes (e.g. PinoutPane "Tune use") to the adopted ECU tune.
                    uiContext.fireConfigImageChanged(sessionImage.get());
                });
            }
        });

        JPanel northPanel = new JPanel();
        northPanel.setLayout(new BoxLayout(northPanel, BoxLayout.Y_AXIS));
        northPanel.add(toolbar.getPanel());
        JPanel indicatorPanel = buildFrontendIndicatorPanel(uiContext);
        if (indicatorPanel != null) {
            northPanel.add(indicatorPanel);
        }
        if (gaugeStrip != null) {
            northPanel.add(gaugeStrip.getContent());
        }

        tuningContent.add(northPanel, BorderLayout.NORTH);
        tuningContent.add(splitPane, BorderLayout.CENTER);
        content.add(tuningContent, TUNING_CARD);
        content.add(loadStatusCard.getContent(), LOAD_CARD);
        showTuningContent();
    }


    private static JPanel buildFrontendIndicatorPanel(UIContext uiContext) {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (ini == null) {
            return null;
        }
        FrontPageModel frontPage = ini.getFrontPage();
        if (frontPage == null) {
            return null;
        }
        List<IndicatorModel> indicators = frontPage.getIndicators();
        if (indicators.isEmpty()) {
            return null;
        }
        return new IndicatorPanel(indicators, ini, 0).getPanel();
    }

    public JPanel getContent() {
        return content;
    }

    public void setNavigateToPinout(Consumer<String> navigateToPinout) {
        this.navigateToPinout = navigateToPinout;
    }

    public void setShowTuningTab(Runnable showTuningTab) {
        this.showTuningTab = showTuningTab;
    }

    private void showLoadProgress() {
        loadStatusCard.showProgress("Loading tune...");
        ((CardLayout) content.getLayout()).show(content, LOAD_CARD);
    }

    private void showLoadResult(boolean failed) {
        loadStatusCard.showResult("Tune loaded successfully", "Tune load failed", failed);
        ((CardLayout) content.getLayout()).show(content, LOAD_CARD);
    }

    private void showTuningContent() {
        ((CardLayout) content.getLayout()).show(content, TUNING_CARD);
    }

    /**
     * Selects the dialog containing {@code fieldKey} in the left tree, which triggers the
     * existing onSelect pipeline to render the dialog on the right.
     * Field-level scrolling within the dialog is deferred (see issue — "scroll into view").
     */
    public void navigateToField(String dialogKey, String fieldKey) {
        if (dialogKey == null) return;
        left.selectSubMenu(dialogKey);
    }

    public Action getLoadTuneAction() {
        return toolbar.getLoadTuneAction();
    }

    public Action getSaveTuneAction() {
        return toolbar.getSaveTuneAction();
    }

    enum ExitPrompt {
        NONE,
        BURN,
        SAVE
    }

    static ExitPrompt exitPrompt(boolean dirty, boolean connected, boolean firmwareUpdateInProgress) {
        if (!dirty) {
            return ExitPrompt.NONE;
        }
        return connected && !firmwareUpdateInProgress ? ExitPrompt.BURN : ExitPrompt.SAVE;
    }

    static void dispatchExitChoice(ExitPrompt prompt, int choice,
                                   Consumer<Runnable> burn, Consumer<Runnable> save, Runnable exit) {
        if (choice == 0) {
            if (prompt == ExitPrompt.BURN) {
                burn.accept(exit);
            } else if (prompt == ExitPrompt.SAVE) {
                save.accept(exit);
            }
        } else if (choice == 1) {
            exit.run();
        }
    }

    public void requestExit(Component parent, Runnable exit, boolean firmwareUpdateInProgress) {
        ExitPrompt prompt = exitPrompt(toolbar.hasUnsavedChanges(sessionImage.get()),
                ConnectionStatusLogic.INSTANCE.isConnected() && uiContext.getBinaryProtocol() != null,
                firmwareUpdateInProgress);
        if (prompt == ExitPrompt.NONE) {
            exit.run();
            return;
        }

        boolean burn = prompt == ExitPrompt.BURN;
        String action = burn ? "Burn to ECU and Exit" : "Save Tune and Exit";
        String without = burn ? "Exit Without Burning" : "Exit Without Saving";
        int choice = JOptionPane.showOptionDialog(parent,
                burn ? "The tune has changes that have not been burned to the ECU."
                        : "The tune has unsaved changes.",
                "Unsaved Tune Changes", JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.WARNING_MESSAGE,
                null, new Object[]{action, without, "Cancel"}, action);
        dispatchExitChoice(prompt, choice,
                onSuccess -> toolbar.burnToEcuAndThen(right, onSuccess),
                onSuccess -> toolbar.saveTuneAndThen(right, onSuccess), exit);
    }

    public void setFirmwareUpdateInProgress(boolean inProgress) {
        firmwareUpdateInProgress = inProgress;
        toolbar.setFirmwareUpdateInProgress(inProgress);
    }

    /**
     * [tag:offline_tune]
     * Seeds the session image and baseline for offline editing (no ECU connection).
     * Called from ConsoleUI when opening with a pre-loaded tune.
     */
    public void seedOfflineImage(ConfigurationImage image, String dialogKey) {
        if (image == null) return;
        // Seed the session image so the onSelect handler has a base image to render against
        // (offline mode has no BinaryProtocol/ECU image to fall back to).
        sessionImage.set(image.clone());
        toolbar.setBaselineImage(image.clone());
        // Remember which .ini these edits belong to so a different-signature ECU can migrate them.
        offlineIni = uiContext.iniFileState.getIniFileModel();
        // Select the first dialog if none is selected yet
        if (dialogKey != null) {
            left.selectSubMenu(dialogKey);
        }
    }

    /** [tag:offline_tune] Reconnect-dialog outcomes. Pure mapping in {@link #reconcileOutcome} is unit-tested. */
    enum ReconcileOutcome { WRITE_LOCAL, MIGRATE, USE_ECU, KEEP_LOCAL }

    /**
     * [tag:offline_tune] Maps the reconnect dialog's choice to an action.
     * Dismissing the dialog (window close / Esc → {@link JOptionPane#CLOSED_OPTION}) must NEVER discard
     * local edits — it keeps them (stays offline). {@code dialogChoice} is the 0-based option index
     * returned by {@code showOptionDialog}: option 0 keeps local (burn or migrate), option 1 uses the ECU tune.
     */
    static ReconcileOutcome reconcileOutcome(boolean sameSignature, int dialogChoice, boolean migratableEmpty) {
        if (dialogChoice == JOptionPane.CLOSED_OPTION) {
            return ReconcileOutcome.KEEP_LOCAL;
        }
        if (sameSignature) {
            return dialogChoice == 0 ? ReconcileOutcome.WRITE_LOCAL : ReconcileOutcome.USE_ECU;
        }
        if (dialogChoice == 0 && !migratableEmpty) {
            return ReconcileOutcome.MIGRATE;
        }
        return ReconcileOutcome.USE_ECU; // "Use ECU Tune", or "Migrate" with nothing migratable
    }

    /**
     * [tag:offline_tune]
     * Called on the EDT when an ECU connects while there are unsaved offline edits.
     * Prompts the user to keep/migrate their edits or use the ECU tune.
     *
     * @return true if the caller must NOT adopt the ECU tune (edits written/migrated, or kept-local on
     *         dialog dismissal); false only if the user explicitly chose the ECU tune.
     */
    private boolean reconcileOfflineEditsOnConnect(BinaryProtocol bp, CalibrationDialogWidget right,
                                                    ConfigurationImage offlineEdits) {
        if (firmwareUpdateInProgress) {
            return true;
        }
        IniFileModel ecuIni = uiContext.iniFileState.getIniFileModel();
        String ecuSig = (ecuIni != null) ? ecuIni.getSignature() : bp.signature;
        String offlineSig = (offlineIni != null) ? offlineIni.getSignature() : null;
        boolean sameSignature = offlineSig != null && offlineSig.equals(ecuSig);

        // Same firmware (or we cannot compare layouts): the edited image is byte-compatible — write it raw.
        if (sameSignature || offlineIni == null || ecuIni == null) {
            int choice = JOptionPane.showOptionDialog(content,
                    "You have unsaved local tune edits.\n" +
                            "Keep them and burn to the connected ECU, or use the ECU's current tune?\n\n" +
                            "Closing this dialog keeps your local edits without burning.",
                    "ECU Connected", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE,
                    null, new Object[]{"Keep Local and Burn to ECU", "Use ECU Tune"}, "Keep Local and Burn to ECU");
            switch (reconcileOutcome(true, choice, false)) {
                case USE_ECU:
                    return false; // caller adopts the ECU's current tune
                case KEEP_LOCAL:
                    // Dialog dismissed — never discard silently. Stay offline with edits intact; the ECU
                    // is connected and the layout matches, so allow burning them later.
                    toolbar.onEcuConnected();
                    return true;
                default: // WRITE_LOCAL
                    break;
            }
            final ConfigurationImage toWrite = offlineEdits.clone();
            uiContext.getLinkManager().submit(() -> {
                bp.uploadChanges(toWrite); // uploads + burns + caches the image
                SwingUtilities.invokeLater(() -> {
                    uiContext.setOfflineMode(false);
                    toolbar.onEcuConnected();
                    toolbar.setBaselineImage(toWrite.clone());
                    uiContext.fireConfigImageChanged(toWrite);
                });
            });
            return true;
        }

        // Different firmware signature: migrate only the edited fields that also exist on the ECU.
        // ponytail: name-based field migration on the EDT; fine for a one-shot connect dialog.
        ConfigurationImage baseline = toolbar.getBaselineImage();
        if (baseline == null) {
            baseline = offlineEdits; // no baseline => nothing detectable as an edit
        }
        OfflineEditMigration.Plan plan = OfflineEditMigration.computePlan(baseline, offlineEdits, offlineIni, ecuIni);

        StringBuilder msg = new StringBuilder();
        msg.append("The connected ECU uses a different firmware than your offline tune.\n\n")
                .append("Offline tune signature:\n    ").append(offlineSig).append("\n")
                .append("Connected ECU signature:\n    ").append(ecuSig).append("\n\n")
                .append(plan.migratable.size()).append(" of your edited field(s) can be migrated to this ECU.\n");
        if (!plan.incompatible.isEmpty()) {
            msg.append(plan.incompatible.size())
                    .append(" edited field(s) do not exist on this ECU and will be skipped:\n")
                    .append(formatFieldList(plan.incompatible)).append("\n");
        }
        msg.append("\nMigrate your edits to the connected ECU, or use the ECU's current tune?\n")
                .append("Closing this dialog keeps your local edits (offline) without changing the ECU.");

        int choice = JOptionPane.showOptionDialog(content, msg.toString(),
                "ECU Signature Mismatch", JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE,
                null, new Object[]{"Migrate to ECU", "Use ECU Tune"}, "Migrate to ECU");
        switch (reconcileOutcome(false, choice, plan.migratable.isEmpty())) {
            case USE_ECU:
                return false; // "Use ECU Tune", or nothing migratable — adopt the ECU tune
            case KEEP_LOCAL:
                // Dialog dismissed — keep local edits, stay offline. No raw burn: the layouts differ,
                // so burning stays disabled until the user reconnects and reconciles deliberately.
                return true;
            default: // MIGRATE
                break;
        }

        final ConfigurationImage merged = OfflineEditMigration.apply(bp.getControllerConfiguration(), ecuIni, plan.migratable);
        final IniFileModel targetIni = ecuIni;
        uiContext.getLinkManager().submit(() -> {
            bp.uploadChanges(merged); // uploads + burns + caches the image
            SwingUtilities.invokeLater(() -> {
                uiContext.setOfflineMode(false);
                toolbar.onEcuConnected();
                sessionImage.set(merged.clone());
                toolbar.setBaselineImage(merged.clone());
                String key = currentKey.get();
                if (key != null) {
                    right.update(key, targetIni, sessionImage.get());
                }
                uiContext.fireConfigImageChanged(merged);
            });
        });
        return true;
    }

    /** Formats a field-name list for a dialog, truncating long lists. */
    private static String formatFieldList(List<String> fields) {
        final int max = 15;
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < Math.min(fields.size(), max); i++) {
            sb.append("    • ").append(fields.get(i)).append("\n");
        }
        if (fields.size() > max) {
            sb.append("    … and ").append(fields.size() - max).append(" more\n");
        }
        return sb.toString();
    }
}
