package com.rusefi.ui.widgets.tune;

import com.devexperts.logging.Logging;
import com.formdev.flatlaf.FlatClientProperties;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.OfflineTuneLoader;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;
import com.rusefi.maintenance.jobs.JobHelper;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.MsqFactory;
import com.rusefi.util.RestorePointStore;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.LoadTuneHelper;
import com.rusefi.ui.widgets.JoinedButtons;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.io.IOException;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.concurrent.Callable;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.FutureTask;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.BiPredicate;
import java.util.function.Consumer;
import java.util.function.Function;

/**
 * Toolbar for the Tune tab: Burn, Discard, Undo, and Redo buttons.
 * Also owns the undo/redo history stacks and the upload/debounce timers
 * so they are kept out of TuningPane.
 */
public class TuningToolbarWidget {
    private static final Logging log = Logging.getLogging(TuningToolbarWidget.class);
    private static final int MAX_UNDO = 15;
    private static final int IDLE_TIMEOUT_MS = 300;
    private static final int UPLOAD_DELAY_MS = 100;
    static final String BURN_NOTE = "Burned. Previous tune kept as a restore point";
    /** Long enough to read the Burn note and click its link. */
    private static final int NOTE_MS = 8000;

    private final JPanel panel;
    private Window dialogOwner;
    private BiPredicate<String, Boolean> startTuneLoad;
    private Function<JComponent, Runnable> reviewHost = content -> null;
    private final JButton burnButton;
    private final JButton discardButton;

    /** [tag:offline_tune] Shows offline / unburned-edit state. Empty when connected and clean. */
    private final JLabel stateLabel = new JLabel();
    private final UIContext uiContext;
    private final Consumer<Runnable> submit;
    private final AtomicReference<ConfigurationImage> sessionImage;

    private final ArrayDeque<TuneSnapshot> undoStack = new ArrayDeque<>();
    private final ArrayDeque<TuneSnapshot> redoStack = new ArrayDeque<>();
    private final AtomicReference<ConfigurationImage> undoBaseline = new AtomicReference<>();

    /** [tag:offline_tune] Baseline image for discard — set when a tune is loaded (from ECU or MSQ file). */
    private ConfigurationImage baselineImage;
    private Map<Integer, ConfigurationImage> secondaryBaseline = Map.of();
    // Saved-to-file is independent of loaded/burned. Before the first Save this tracks the opened tune
    // or the starting ECU image, so an untouched ECU tune does not prompt to be saved.
    private Map<Integer, ConfigurationImage> savedPages = Map.of();
    private String savedLayoutSignature;

    private static final class TuneSnapshot {
        final ConfigurationImage main;
        final Map<Integer, ConfigurationImage> secondary;

        TuneSnapshot(ConfigurationImage main, Map<Integer, ConfigurationImage> secondary) {
            this.main = main;
            this.secondary = secondary;
        }
    }

    private final JButton undoButton = new JButton("Undo");
    private final JButton redoButton = new JButton("Redo");

    private AbstractAction loadTuneAction;
    /** Where the tune is kept before each Burn and Load tune. */
    private volatile RestorePointStore restorePoints = RestorePointStore.shared();
    private AbstractAction saveTuneAction;
    private AbstractAction saveTuneAsAction;
    private AbstractAction createRestorePointAction;
    /** Writes restore points made by hand off the EDT. */
    private java.util.concurrent.Executor fileWork = work -> new Thread(work, "restore-point").start();
    /** Ends a {@link #showNote} message: the toolbar state comes back. */
    private final Timer noteTimer = new Timer(NOTE_MS, e -> refreshState());
    /** The link of the note shown, null for none. */
    private Runnable noteAction;
    private Runnable showRestorePoints = () -> { };
    private final java.awt.event.MouseAdapter noteClick = new java.awt.event.MouseAdapter() {
        @Override
        public void mouseClicked(java.awt.event.MouseEvent e) {
            Runnable action = noteAction;
            if (action != null && SwingUtilities.isLeftMouseButton(e)) {
                action.run();
            }
        }
    };
    private final JFileChooser saveTuneChooser = createMsqFileChooser();

    private final Timer undoCommitTimer;
    private final Timer uploadTimer;
    private volatile boolean firmwareUpdateInProgress;
    private volatile boolean loadInProgress;
    private boolean burnInProgress;
    private boolean saveInProgress;
    private Runnable onBaselineChanged = () -> { };
    private volatile long editGeneration;
    private volatile long connectionGeneration;
    private Consumer<String> errorHandler = message -> JOptionPane.showMessageDialog(null, message, "Error", JOptionPane.ERROR_MESSAGE);

    /**
     * @param uiContext      live context (BinaryProtocol, LinkManager)
     * @param right          the dialog widget — needed for get/update working image
     * @param currentKey     single-element array holding the currently displayed menu key
     * @param sessionImage   single-element array holding the accumulated session image
     * @param baselineImage  baseline image for discard (from ECU or loaded MSQ); may be null initially
     */
    public TuningToolbarWidget(UIContext uiContext,
                                CalibrationDialogWidget right,
                                 AtomicReference<String> currentKey,
                                 AtomicReference<ConfigurationImage> sessionImage,
                                 ConfigurationImage baselineImage,
                                 UpdateOperationCallbacks loadCallbacks,
                                 Runnable onLoadStarted,
                                 Consumer<Boolean> onLoadFinished) {
        this(uiContext, right, currentKey, sessionImage, baselineImage, loadCallbacks,
            onLoadStarted, onLoadFinished, operation -> uiContext.getLinkManager().submit(operation));
    }

    TuningToolbarWidget(UIContext uiContext,
                        CalibrationDialogWidget right,
                        AtomicReference<String> currentKey,
                        AtomicReference<ConfigurationImage> sessionImage,
                        ConfigurationImage baselineImage,
                        UpdateOperationCallbacks loadCallbacks,
                        Runnable onLoadStarted,
                        Consumer<Boolean> onLoadFinished,
                        Consumer<Runnable> submit) {
        this.uiContext = uiContext;
        this.submit = submit;
        this.sessionImage = sessionImage;
        this.baselineImage = baselineImage;
        this.savedPages = baselineImage == null ? Map.of() : Map.of(0, baselineImage.clone());
        IniFileModel initialIni = uiContext.iniFileState.getIniFileModel();
        this.savedLayoutSignature = initialIni == null ? null : initialIni.getSignature();
        uiContext.setSecondaryTuneListeners((id, before) -> {
            secondaryBaseline = copyPages(secondaryBaseline);
            secondaryBaseline.putIfAbsent(id, before.clone());
            savedPages = copyPages(savedPages);
            savedPages.putIfAbsent(id, before.clone());
            ++editGeneration;
            refreshState();
        }, id -> {
            ConfigurationImage burned = workingSecondary().get(id);
            if (burned != null) {
                secondaryBaseline = copyPages(secondaryBaseline);
                secondaryBaseline.put(id, burned.clone());
                refreshState();
            }
        });
        noteTimer.setRepeats(false);
        stateLabel.addMouseListener(noteClick);
        undoButton.setEnabled(false);
        redoButton.setEnabled(false);
        undoButton.setToolTipText("Undo the last tune change");
        redoButton.setToolTipText("Redo the last undone tune change");

        Runnable updateButtons = () -> {
            undoButton.setEnabled(!undoStack.isEmpty());
            redoButton.setEnabled(!redoStack.isEmpty());
        };

        Runnable flushUndoBaseline = () -> {
            ConfigurationImage baseline = undoBaseline.getAndSet(null);
            if (baseline != null) {
                undoStack.push(new TuneSnapshot(baseline, workingSecondary()));
                if (undoStack.size() > MAX_UNDO) {
                    undoStack.removeLast();
                }
                redoStack.clear();
                updateButtons.run();
            }
        };

        undoCommitTimer = new Timer(IDLE_TIMEOUT_MS, e -> flushUndoBaseline.run());
        undoCommitTimer.setRepeats(false);

        uploadTimer = new Timer(UPLOAD_DELAY_MS, e -> uploadPendingChanges());
        uploadTimer.setRepeats(false);

        burnButton = getBurnToEcuButton(uiContext, right, sessionImage);
        burnButton.setEnabled(uiContext.getBinaryProtocol() != null);

        discardButton = getDiscardButton(uiContext, right, sessionImage, currentKey, updateButtons);
        discardButton.setEnabled(baselineImage != null);

        buildLoadTuneAction(uiContext, right, currentKey, sessionImage,
            loadCallbacks, onLoadStarted, onLoadFinished);
        buildSaveTuneAction(uiContext, right, sessionImage);

        undoButton.addActionListener(e -> {
            if (loadInProgress || undoStack.isEmpty()) {
                return;
            }
            undoCommitTimer.stop();
            undoBaseline.set(null);
            ConfigurationImage image = sessionImage.get();
            if (image != null) {
                redoStack.push(new TuneSnapshot(image, workingSecondary()));
            }
            restoreHistory(undoStack.pop());
            String key = currentKey.get();
            // [tag:detached_calibration_sync] Restored windows may be open before a primary page
            // is selected. Publish replacements even with a null key (also redo/discard/load below).
            right.update(key, uiContext.iniFileState.getIniFileModel(), sessionImage.get());
            publishReplacement();
            updateButtons.run();
            // the replaced tune can be clean again, or differ in another number of values
            refreshState();
        });

        redoButton.addActionListener(e -> {
            if (loadInProgress || redoStack.isEmpty()) {
                return;
            }
            undoCommitTimer.stop();
            undoBaseline.set(null);
            ConfigurationImage image = sessionImage.get();
            if (image != null) {
                undoStack.push(new TuneSnapshot(image, workingSecondary()));
            }
            restoreHistory(redoStack.pop());
            String key = currentKey.get();
            right.update(key, uiContext.iniFileState.getIniFileModel(), sessionImage.get());
            publishReplacement();
            updateButtons.run();
            refreshState();
        });

        // sits on the Tuning tab's toolbar strip (TuningPane), which supplies the colour
        panel = layOut(CONTROLS_TOP, stateLabel, burnButton, discardButton, undoButton, redoButton);
        panel.add(new JoinedButtons(new JButton(loadTuneAction)), 2);

        // [tag:offline_tune] Refresh the state label whenever offline mode toggles (e.g. Load Tune
        // while disconnected). Edit/discard/burn/connect/disconnect paths call refreshState() directly.
        uiContext.addOfflineModeListener(offline -> SwingUtilities.invokeLater(this::refreshState));
        refreshState();
    }

    /**
     * Same controls as the Tuning toolbar [tag:console_theme]: 36 px tall, primary Burn, outlined
     * groups, Undo and Redo joined as one control, then the state label.
     * [tag:tune_controls_position] Burn sits {@link #CONTROLS_LEFT} px from the screen edge and
     * {@link #CONTROLS_TOP} px below the tab row on both Tuning and Gauges, so switching tabs never moves it.
     */
    private static JPanel layOut(int vgap, JLabel label, JButton burn, JButton discard, JButton undo, JButton redo) {
        JPanel row = new JPanel(new FlowLayout(FlowLayout.LEFT, JoinedButtons.ROW_GAP, vgap));
        row.setOpaque(false);
        burn.setPreferredSize(new Dimension(burn.getPreferredSize().width + 24, JoinedButtons.HEIGHT));
        row.add(burn);
        row.add(new JoinedButtons(discard));
        row.add(new JoinedButtons(undo, redo));
        label.setBorder(BorderFactory.createEmptyBorder(0, 6, 0, 0));
        row.add(label);
        return row;
    }

    /** [tag:tune_controls_position] Left inset of the Tuning strip (TuningPane) plus the row gap. */
    public static final int CONTROLS_LEFT = 4 + JoinedButtons.ROW_GAP;
    /** [tag:tune_controls_position] The Tuning toolbar row's vertical gap. */
    public static final int CONTROLS_TOP = 6;

    /**
     * Disabled Burn / Discard / Undo / Redo laid out exactly like {@link #createMirrorControls()}, holding
     * their place on another screen until the Tuning pane (built on first use) supplies the live ones.
     */
    public static JComponent createPlaceholderControls() {
        JButton burn = new JButton("Burn to ECU");
        burn.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        JButton[] buttons = {burn, new JButton("Discard changes"), new JButton("Undo"), new JButton("Redo")};
        for (JButton button : buttons) {
            button.setEnabled(false);
        }
        return layOut(0, new JLabel(), buttons[0], buttons[1], buttons[2], buttons[3]);
    }

    /**
     * Burn / Discard / Undo / Redo for another screen, e.g. the Gauges toolbar, where settings windows edit
     * the same tune. Each button runs this toolbar's own handler and follows its enabled state, so both
     * places always agree; the label repeats the unburned-changes text.
     */
    public JComponent createMirrorControls() {
        JLabel label = new JLabel();
        // a note's link works here too
        label.addMouseListener(noteClick);
        Runnable syncLabel = () -> {
            label.setCursor(stateLabel.getCursor());
            label.setToolTipText(stateLabel.getToolTipText());
            label.setText(stateLabel.getText());
            label.setForeground(stateLabel.getForeground());
        };
        stateLabel.addPropertyChangeListener("text", e -> syncLabel.run());
        stateLabel.addPropertyChangeListener("foreground", e -> syncLabel.run());
        syncLabel.run();
        return layOut(0, label, mirrorOf(burnButton), mirrorOf(discardButton), mirrorOf(undoButton),
            mirrorOf(redoButton));
    }

    private static JButton mirrorOf(JButton original) {
        JButton mirror = new JButton(original.getText());
        mirror.setToolTipText(original.getToolTipText());
        mirror.putClientProperty(FlatClientProperties.STYLE_CLASS,
            original.getClientProperty(FlatClientProperties.STYLE_CLASS));
        mirror.setEnabled(original.isEnabled());
        original.addPropertyChangeListener("enabled", e -> mirror.setEnabled(original.isEnabled()));
        mirror.addActionListener(e -> {
            if (original.isEnabled()) {
                for (java.awt.event.ActionListener listener : original.getActionListeners()) {
                    listener.actionPerformed(e);
                }
            }
        });
        return mirror;
    }

    /**
     * [tag:offline_tune] Updates the toolbar state label from current offline mode + dirty state.
     * Empty (invisible) when connected with no pending edits.
     */
    public void refreshState() {
        ConfigurationImage current = sessionImage.get();
        boolean dirty = hasUnsavedChanges(current);
        IniFileModel ini = uiContext.iniFileState == null ? null : uiContext.iniFileState.getIniFileModel();
        int changedValues = dirty ? changedValueCount(ini, current, baselineImage) : 0;
        // the state replaces any note, and its link
        noteAction = null;
        stateLabel.setCursor(Cursor.getDefaultCursor());
        stateLabel.setToolTipText(null);
        stateLabel.setText(stateLabelText(uiContext.isOfflineMode(), dirty, changedValues));
        stateLabel.setForeground(dirty ? TuningStyle.dirty() : TuningStyle.muted());
    }

    /**
     * [tag:unburned_marks] How many settings and table/curve cells of {@code current} differ from the
     * last burned tune; 0 without an .ini or a burned tune.
     */
    static int changedValueCount(IniFileModel ini, ConfigurationImage current, ConfigurationImage burned) {
        if (ini == null) {
            return 0;
        }
        return UnburnedChanges.of(current, burned).countChanged(ini.getAllIniFields().values());
    }

    static String stateLabelText(boolean offline, boolean dirty) {
        return stateLabelText(offline, dirty, 0);
    }

    /**
     * Pure label-text logic for the offline/dirty toolbar indicator. Package-private and static so it
     * can be unit-tested without Swing (see {@code TuningToolbarStateLabelTest}).
     *
     * @param changedValues settings and cells changed since the last burn; 0 shows no count, e.g. when
     *                      only bytes no setting covers differ
     */
    static String stateLabelText(boolean offline, boolean dirty, int changedValues) {
        if (dirty) {
            String text = offline ? "Local changes not burned to ECU" : "Pending changes not burned";
            if (changedValues > 0) {
                text += " (" + changedValues + (changedValues == 1 ? " value)" : " values)");
            }
            return text;
        }
        return offline ? "Offline tune" : "";
    }

    private @NotNull JButton getBurnToEcuButton(UIContext uiContext,
                                                CalibrationDialogWidget right,
                                                AtomicReference<ConfigurationImage> sessionImage) {
        JButton burnButton = new JButton("Burn to ECU");
        burnButton.setToolTipText("Save the current tune to persistent ECU memory");
        burnButton.putClientProperty(FlatClientProperties.STYLE_CLASS, "primary");
        burnButton.addActionListener(e -> burnToEcuAndThen(right, null));
        return burnButton;
    }

    public void burnToEcuAndThen(CalibrationDialogWidget right, Runnable onSuccess) {
        if (firmwareUpdateInProgress || burnInProgress || loadInProgress || uiContext.isOfflineMode()) {
            return;
        }
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        ConfigurationImage toBurn = right.getWorkingImage();
        if (toBurn == null) {
            toBurn = sessionImage.get();
        }
        if (bp == null || toBurn == null) {
            return;
        }
        uploadTimer.stop();
        final ConfigurationImage image = toBurn.clone();
        final Map<Integer, ConfigurationImage> secondary = copyPages(workingSecondary());
        final Map<Integer, ConfigurationImage> previous = copyPages(secondaryBaseline);
        final Set<Integer> changedSecondary = changedPages(previous, secondary);
        final Map<Integer, ConfigurationImage> target = snapshotPages(image, secondary);
        final IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        final RestorePointStore store = restorePoints;
        final long savedEditGeneration = editGeneration;
        final long savedConnectionGeneration = connectionGeneration;
        burnInProgress = true;
        burnButton.setEnabled(false);
        discardButton.setEnabled(false);
        loadTuneAction.setEnabled(false);
        try {
            submit.accept(() -> {
                Set<Integer> acknowledged = new TreeSet<>();
                boolean previousKept;
                RuntimeException failure = null;
                try {
                    if (!sameLiveConnection(bp, savedConnectionGeneration)) {
                        throw new IllegalStateException("The ECU session changed.");
                    }
                    requireSecondaryRam(bp, secondary, changedSecondary);
                    previousKept = saveRestorePoint(store, snapshotPages(bp.getControllerConfiguration(), previous), ini,
                        RestorePointStore.Reason.BEFORE_BURN, RestorePointsWindow.vehicleNameOf(image, ini));
                    bp.uploadChangesWithoutBurn(image);
                    bp.burn();
                    bp.setConfigurationImage(image);
                    acknowledged.add(0);
                    burnSecondaryPages(bp, changedSecondary, acknowledged, () -> {
                        if (!sameLiveConnection(bp, savedConnectionGeneration)) {
                            throw new IllegalStateException("The ECU session changed.");
                        }
                    });
                } catch (RuntimeException ex) {
                    previousKept = false;
                    failure = ex;
                }
                boolean restorePointKept = previousKept;
                RuntimeException operationFailure = failure;
                SwingUtilities.invokeLater(() -> {
                    boolean sameConnection = savedConnectionGeneration == connectionGeneration
                        && uiContext.getBinaryProtocol() == bp;
                    ConfigurationImage currentImage = sessionImage.get();
                    boolean sameTune = currentImage != null
                        && java.util.Arrays.equals(currentImage.getContent(), image.getContent());
                    boolean noNewEdits = savedEditGeneration == editGeneration && sameTune;
                    burnInProgress = false;
                    burnButton.setEnabled(!firmwareUpdateInProgress && uiContext.getBinaryProtocol() != null);
                    discardButton.setEnabled(baselineImage != null);
                    loadTuneAction.setEnabled(!firmwareUpdateInProgress && !loadInProgress);
                    if (!sameConnection) {
                        return;
                    }
                    if (!acknowledged.isEmpty()) {
                        adoptAcknowledgedBurns(acknowledged, target);
                    }
                    if (operationFailure != null) {
                        refreshState();
                        errorHandler.accept("Failed to burn tune (some pages may already be persisted): "
                            + operationFailure.getMessage());
                        return;
                    }
                    if (restorePointKept) {
                        showNote(BURN_NOTE, "Restore points", showRestorePoints);
                    }
                    if (noNewEdits && onSuccess != null) {
                        onSuccess.run();
                    }
                });
            });
        } catch (RuntimeException ex) {
            burnInProgress = false;
            burnButton.setEnabled(uiContext.getBinaryProtocol() != null);
            discardButton.setEnabled(baselineImage != null);
            loadTuneAction.setEnabled(!firmwareUpdateInProgress);
            errorHandler.accept("Failed to burn tune: " + ex.getMessage());
        }
    }

    /** Call when ECU connects to enable the burn button. */
    public void onEcuConnected() {
        connectionGeneration++;
        burnButton.setEnabled(!burnInProgress && !loadInProgress);
        refreshState();
    }

    public void resetTuneFileBaseline(ConfigurationImage image) {
        secondaryBaseline = copyPages(workingSecondary());
        savedPages = snapshotPages(image, secondaryBaseline);
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        savedLayoutSignature = ini == null ? null : ini.getSignature();
    }

    public void adoptConnectedBaseline(ConfigurationImage image, boolean replacingOfflineTune) {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        String signature = ini == null ? null : ini.getSignature();
        secondaryBaseline = Map.of();
        if (replacingOfflineTune || savedPages.isEmpty() || !Objects.equals(signature, savedLayoutSignature)) {
            uiContext.setTuneFilePath(null);
            resetTuneFileBaseline(image);
        }
        setBaselineImage(image);
    }

    private @NotNull JButton getDiscardButton(UIContext uiContext,
                                              CalibrationDialogWidget right,
                                              AtomicReference<ConfigurationImage> sessionImage,
                                              AtomicReference<String> currentKey,
                                              Runnable updateButtons) {
        JButton discardButton = new JButton("Discard changes");
        discardButton.setToolTipText("Restore the tune from the last load or burn");

        discardButton.addActionListener(e -> {
            if (burnInProgress || loadInProgress || baselineImage == null) {
                return;
            }
            uploadTimer.stop();
            ConfigurationImage restoration = baselineImage.clone();
            Map<Integer, ConfigurationImage> secondary = copyPages(secondaryBaseline);
            Set<Integer> changedSecondary = changedPages(secondary, workingSecondary());
            ++editGeneration;
            uiContext.setWorkingSecondaryTunePages(secondary);
            sessionImage.set(restoration);
            String key = currentKey.get();
            right.update(key, uiContext.iniFileState.getIniFileModel(), restoration);
            publishReplacement();
            undoCommitTimer.stop();
            undoStack.clear();
            redoStack.clear();
            undoBaseline.set(null);
            updateButtons.run();
            refreshState();

            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null || firmwareUpdateInProgress || uiContext.isOfflineMode()) {
                return;
            }
            ConfigurationImage snapshot = restoration.clone();
            long savedConnectionGeneration = connectionGeneration;
            try {
                submit.accept(() -> {
                    if (savedConnectionGeneration != connectionGeneration
                            || uiContext.getBinaryProtocol() != bp
                            || firmwareUpdateInProgress || uiContext.isOfflineMode()) {
                        return;
                    }
                     try {
                         bp.uploadChangesWithoutBurn(snapshot);
                        restoreSecondaryToRam(bp, secondary, changedSecondary);
                    } catch (RuntimeException ex) {
                        SwingUtilities.invokeLater(() ->
                            errorHandler.accept("Failed to restore tune to ECU: " + ex.getMessage()));
                    }
                });
            } catch (RuntimeException ex) {
                SwingUtilities.invokeLater(() ->
                    errorHandler.accept("Failed to restore tune to ECU: " + ex.getMessage()));
            }
        });

        return discardButton;
    }

    /**
     * Tells views following the working tune ({@link UIContext#fireConfigImageChanged}: Pinout's Tune use,
     * the context's current image) that Undo, Redo or Discard replaced it. An edit notifies through the
     * dialog's change callback, but these replace the tune directly, so without this those views keep showing
     * the replaced tune (TuningUndoNotificationTest).
     */
    private void publishReplacement() {
        ConfigurationImage image = sessionImage.get();
        if (image != null) {
            uiContext.fireConfigImageChanged(image);
        }
    }

    private void uploadPendingChanges() {
        if (firmwareUpdateInProgress || loadInProgress || uiContext.isOfflineMode()) {
            return;
        }
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        ConfigurationImage image = sessionImage.get();
        if (bp == null || image == null) {
            return;
        }
        final ConfigurationImage snapshot = image.clone();
        long connection = connectionGeneration;
        try {
            submit.accept(() -> {
                if (!sameLiveConnection(bp, connection) || loadInProgress) {
                    return;
                }
                try {
                    bp.uploadChangesWithoutBurn(snapshot);
                } catch (RuntimeException ex) {
                    SwingUtilities.invokeLater(() ->
                        errorHandler.accept("Failed to upload tune: " + ex.getMessage()));
                }
            });
        } catch (RuntimeException ex) {
            SwingUtilities.invokeLater(() ->
                errorHandler.accept("Failed to upload tune: " + ex.getMessage()));
        }
    }

    private boolean sameLiveConnection(BinaryProtocol bp, long connection) {
        return uiContext.getBinaryProtocol() == bp && connection == connectionGeneration
            && !bp.isClosed() && !firmwareUpdateInProgress && !uiContext.isOfflineMode();
    }

    private Map<Integer, ConfigurationImage> workingSecondary() {
        return uiContext.getWorkingSecondaryTunePages();
    }

    private static Map<Integer, ConfigurationImage> copyPages(Map<Integer, ConfigurationImage> pages) {
        Map<Integer, ConfigurationImage> result = new TreeMap<>();
        pages.forEach((id, image) -> result.put(id, image.clone()));
        return result;
    }

    private static Map<Integer, ConfigurationImage> snapshotPages(ConfigurationImage main, Map<Integer, ConfigurationImage> secondary) {
        Map<Integer, ConfigurationImage> result = copyPages(secondary);
        if (main != null) {
            result.put(0, main.clone());
        }
        return result;
    }

    private static boolean sameImage(ConfigurationImage a, ConfigurationImage b) {
        return a == b || a != null && b != null && a.getByteOrder() == b.getByteOrder()
            && Arrays.equals(a.getContent(), b.getContent());
    }

    private static Set<Integer> changedPages(Map<Integer, ConfigurationImage> previous, Map<Integer, ConfigurationImage> working) {
        Set<Integer> changed = new TreeSet<>();
        working.forEach((id, page) -> {
            if (!sameImage(previous.get(id), page)) {
                changed.add(id);
            }
        });
        changed.remove(0);
        return changed;
    }

    private void restoreHistory(TuneSnapshot snapshot) {
        Set<Integer> changed = changedPages(workingSecondary(), snapshot.secondary);
        Map<Integer, ConfigurationImage> restored = copyPages(workingSecondary());
        restored.putAll(snapshot.secondary);
        uiContext.setWorkingSecondaryTunePages(restored);
        sessionImage.set(snapshot.main);
        ++editGeneration;
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        long connection = connectionGeneration;
        if (bp != null && sameLiveConnection(bp, connection) && !burnInProgress) {
            uploadTimer.stop();
            submit.accept(() -> {
                if (!sameLiveConnection(bp, connection) || loadInProgress) {
                    return;
                }
                try {
                    bp.uploadChangesWithoutBurn(snapshot.main);
                    restoreSecondaryToRam(bp, snapshot.secondary, changed);
                } catch (RuntimeException ex) {
                    SwingUtilities.invokeLater(() -> errorHandler.accept("Failed to restore ECU RAM: " + ex.getMessage()));
                }
            });
        }
    }

    private static void restoreSecondaryToRam(BinaryProtocol bp, Map<Integer, ConfigurationImage> pages, Set<Integer> changed) {
        if (changed.isEmpty()) {
            return;
        }
        Map<Integer, ConfigurationImage> wanted = new TreeMap<>();
        Map<Integer, ConfigurationImage> actual = new TreeMap<>();
        for (int page : changed) {
            ConfigurationImage image = pages.get(page);
            if (image == null) {
                throw new IllegalStateException("Missing snapshot for calibration page " + page);
            }
            byte[] bytes = bp.readFromPage(page, 0, image.getSize());
            if (bytes == null || bytes.length != image.getSize()) {
                throw new IllegalStateException("Cannot verify current calibration page " + page);
            }
            wanted.put(page, image);
            actual.put(page, new ConfigurationImage(bytes, image.getByteOrder()));
        }
        TunePageIo.uploadSecondary(bp, actual, wanted);
    }

    private static void requireSecondaryRam(BinaryProtocol bp, Map<Integer, ConfigurationImage> snapshots, Set<Integer> pages) {
        for (int page : pages) {
            ConfigurationImage expected = snapshots.get(page);
            if (!Arrays.equals(bp.readFromPage(page, 0, expected.getSize()), expected.getContent())) {
                throw new IllegalStateException(String.format("RAM on calibration page 0x%04X changed. Reopen the tune review.", page));
            }
        }
    }

    /** Fires the pending debounce upload deterministically in unit tests. */
    void flushUploadTimerForUnitTest() {
        uploadTimer.stop();
        uploadPendingChanges();
    }

    boolean isUploadTimerRunningForUnitTest() {
        return uploadTimer.isRunning();
    }

    private void buildLoadTuneAction(UIContext uiContext,
                                     CalibrationDialogWidget right,
                                     AtomicReference<String> currentKey,
                                     AtomicReference<ConfigurationImage> sessionImage,
                                     UpdateOperationCallbacks callbacks,
                                     Runnable onLoadStarted,
                                     Consumer<Boolean> onLoadFinished) {
        JFileChooser chooser = createMsqFileChooser();
        loadTuneAction = new AbstractAction(LoadTuneHelper.LOAD_TUNE_TEXT) {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (firmwareUpdateInProgress || burnInProgress || loadInProgress || saveInProgress) {
                    return;
                }
                ConfigurationImage current = currentImage(right);
                if (hasUnsavedFileChanges(current)) {
                    String[] options = {"Save", "Continue Without Saving", "Cancel"};
                    int choice = JOptionPane.showOptionDialog(getDialogParent(),
                        "The current tune has changes that have not been saved to an .msq file.",
                        "Unsaved Tune", JOptionPane.DEFAULT_OPTION, JOptionPane.WARNING_MESSAGE,
                        null, options, options[2]);
                    if (choice == 0) {
                        long generation = editGeneration;
                        long connection = connectionGeneration;
                        saveTuneAndThen(right, () -> {
                            if (generation == editGeneration && connection == connectionGeneration) {
                                chooseTune();
                            }
                        });
                    } else if (choice == 1) {
                        chooseTune();
                    }
                    return;
                }
                chooseTune();
            }

            private void chooseTune() {
                if (firmwareUpdateInProgress || burnInProgress || loadInProgress || saveInProgress) {
                    return;
                }
                TuneFileFolder.openIn(chooser);
                if (chooser.showOpenDialog(getDialogParent()) != JFileChooser.APPROVE_OPTION) {
                    return;
                }
                TuneFileFolder.remember(chooser.getSelectedFile());
                reviewTuneFile(chooser.getSelectedFile().getAbsolutePath());
            }
        };
        startTuneLoad = (path, restoring) -> {
                if (firmwareUpdateInProgress || burnInProgress || loadInProgress || saveInProgress) {
                    return false;
                }
                final String fileName = new File(path).getName();

                final BinaryProtocol bp = uiContext.isOfflineMode() ? null : uiContext.getBinaryProtocol();
                final IniFileModel originalIni = uiContext.iniFileState.getIniFileModel();
                final ConfigurationImage current = currentImage(right);
                if (restoring && (current == null || originalIni == null)) {
                    return false;
                }
                final Map<Integer, ConfigurationImage> working = snapshotPages(current, workingSecondary());
                final long connection = connectionGeneration;
                final long edit = editGeneration;
                final boolean pendingUpload = uploadTimer.isRunning();
                uploadTimer.stop();
                loadInProgress = true;
                uiContext.setTuneReviewInProgress(true);
                updateLoadControls();

                callbacks.clear();
                onLoadStarted.run();

                AsyncJobExecutor.INSTANCE.executeJob(
                    new AsyncJob(restoring ? "Restore Point" : "Load Tune") {
                        @Override
                        public void doJob(UpdateOperationCallbacks cb, Runnable onJobFinished) {
                            JobHelper.doJob(() -> {
                                boolean[] applyAttempted = {false};
                                AtomicReference<Map<Integer, ConfigurationImage>> burnTarget = new AtomicReference<>();
                                Set<Integer> burnedPages = new TreeSet<>();
                                try {
                                    cb.logLine((restoring ? "Reading restore point " : "Loading ") + fileName + "...");
                                    boolean opening = current == null;
                                    if (bp != null && (opening || originalIni == null)) {
                                        throw new IllegalStateException("Wait for the ECU tune to finish loading.");
                                    }
                                    OfflineTuneLoader.Result result = opening
                                        ? OfflineTuneLoader.loadTuneFromFile(path, null, cb::logLine)
                                        : OfflineTuneLoader.loadConnectedTuneFromFile(
                                            path, originalIni, cb::logLine);
                                    if (result == null) {
                                        cb.error();
                                        SwingUtilities.invokeLater(() -> onLoadFinished.accept(true));
                                        return;
                                    }
                                    if (opening) {
                                        Map<Integer, ConfigurationImage> opened = TuneLoadPreparation.openOffline(result.msq, result.ini);
                                        SwingUtilities.invokeAndWait(() -> {
                                            requireCurrentReview(bp, originalIni, connection, edit);
                                            uiContext.setIniFileModelForOffline(result.ini);
                                            publishLoadedPages(right, currentKey, result.ini, opened);
                                            uiContext.setOfflineMode(true);
                                            uiContext.setTuneFilePath(path);
                                            resetTuneFileBaseline(opened.get(0));
                                            setBaselineImage(opened.get(0));
                                        });
                                        cb.done();
                                        SwingUtilities.invokeLater(() -> onLoadFinished.accept(false));
                                        return;
                                    }

                                    List<String> warnings = new ArrayList<>();
                                    Map<Integer, ConfigurationImage> base = new TreeMap<>(working);
                                    Map<Integer, ConfigurationImage> devicePages = new TreeMap<>();
                                    if (bp != null) {
                                        devicePages.putAll(onCommunicationThread(() -> {
                                            requireCurrentReview(bp, originalIni, connection, edit);
                                            Map<Integer, ConfigurationImage> pages = TunePageIo.readSecondary(bp, originalIni, warnings);
                                            pages.put(0, bp.getControllerConfiguration().clone());
                                            return pages;
                                        }));
                                        // An unread page cannot be reviewed against a stale working copy.
                                        base.keySet().removeIf(id -> id != 0);
                                        devicePages.forEach((id, page) -> {
                                            if (id != 0 && TunePageIo.isBurnable(originalIni, id)) {
                                                base.put(id, page);
                                            }
                                        });
                                    }

                                    TuneLoadPreparation prepared = TuneLoadPreparation.prepare(
                                        result.msq, result.ini, originalIni, base, cb);
                                    warnings.addAll(prepared.warnings);
                                    Map<Integer, ConfigurationImage> incoming = restoring
                                        ? RestorePointDiff.asShown(prepared.incoming, base, originalIni) : prepared.incoming;
                                    TuneLoadPlan plan = TuneLoadPlan.create(originalIni, base, incoming, warnings);
                                    TuneReviewDialog.Decision decision = reviewTune(fileName, plan, bp, originalIni, connection, edit,
                                        restoring);
                                    if (decision == null || decision.selected.isEmpty()) {
                                        SwingUtilities.invokeLater(() -> onLoadFinished.accept(null));
                                        return;
                                    }

                                    Map<Integer, ConfigurationImage> chosen = plan.select(decision.selected);
                                    Set<Integer> changed = changedPages(base, chosen);
                                    boolean reviewedMain = !sameImage(base.get(0), chosen.get(0));
                                    Map<Integer, ConfigurationImage> replacement = new TreeMap<>(working);
                                    Map<Integer, ConfigurationImage> before = copyPages(working);
                                    replacement.put(0, chosen.get(0));
                                    for (int page : changed) {
                                        replacement.put(page, chosen.get(page));
                                        before.put(page, base.get(page));
                                    }
                                    SwingUtilities.invokeAndWait(() -> requireCurrentReview(bp, originalIni, connection, edit));
                                    if (!restoring) {
                                        saveRestorePoint(restorePoints, base, originalIni, RestorePointStore.Reason.BEFORE_LOAD, null);
                                    }
                                    if (bp != null) {
                                        cb.logLine("Applying reviewed changes to ECU RAM...");
                                        onCommunicationThread(() -> {
                                            requireCurrentReview(bp, originalIni, connection, edit);
                                            if (decision.burn) {
                                                saveRestorePoint(restorePoints, devicePages, originalIni,
                                                    RestorePointStore.Reason.BEFORE_BURN,
                                                    RestorePointsWindow.vehicleNameOf(chosen.get(0), originalIni));
                                            }
                                             Map<Integer, ConfigurationImage> target = applyReviewedTuneToRam(bp, plan,
                                                 decision.selected, devicePages,
                                                 () -> {
                                                     requireCurrentReview(bp, originalIni, connection, edit);
                                                     applyAttempted[0] = true;
                                                 });
                                            if (decision.burn) {
                                                requireCurrentReview(bp, originalIni, connection, edit);
                                                burnTarget.set(target);
                                                try {
                                                    burnReviewedPages(bp, devicePages, target, burnedPages, reviewedMain,
                                                        () -> requireCurrentReview(bp, originalIni, connection, edit));
                                                } catch (RuntimeException ex) {
                                                    // RAM accepted the whole selection. Publish it if this is still the
                                                    // same session, even if some of the requested burns were rejected.
                                                    SwingUtilities.invokeLater(() -> {
                                                        if (sameLiveConnection(bp, connection) && edit == editGeneration
                                                                && uiContext.iniFileState.getIniFileModel() == originalIni) {
                                                            adoptReviewedTune(right, currentKey, originalIni, before, replacement);
                                                        }
                                                    });
                                                    throw ex;
                                                }
                                            }
                                            return null;
                                        });
                                    }
                                    SwingUtilities.invokeAndWait(() -> {
                                        requireCurrentReview(bp, originalIni, connection, edit);
                                        adoptReviewedTune(right, currentKey, originalIni, before, replacement, !restoring);
                                        if (restoring) {
                                            showNote("Restore point put back as unburned changes");
                                        }
                                    });
                                    cb.done();
                                    SwingUtilities.invokeLater(() -> onLoadFinished.accept(false));
                                 } catch (Exception ex) {
                                    log.error("Failed to load tune " + path, ex);
                                    if (ex instanceof InterruptedException) {
                                        Thread.currentThread().interrupt();
                                    }
                                    Throwable problem = ex instanceof java.lang.reflect.InvocationTargetException && ex.getCause() != null
                                        ? ex.getCause() : ex;
                                    cb.logLine("Tune load stopped: " + problem.getMessage());
                                     if (burnTarget.get() != null) {
                                        cb.logLine("ECU RAM was applied; flash may be only partially persisted. "
                                            + "Check the ECU before continuing.");
                                    } else if (applyAttempted[0]) {
                                        cb.logLine("ECU RAM may have changed. Nothing was burned. Check the ECU before continuing.");
                                    }
                                    cb.error();
                                    SwingUtilities.invokeLater(() -> onLoadFinished.accept(true));
                                } finally {
                                    SwingUtilities.invokeLater(() -> {
                                         if (!burnedPages.isEmpty() && sameLiveConnection(bp, connection)
                                                 && uiContext.iniFileState.getIniFileModel() == originalIni) {
                                             adoptAcknowledgedBurns(burnedPages, burnTarget.get());
                                        }
                                        loadInProgress = false;
                                        uiContext.setTuneReviewInProgress(false);
                                         // A detached editor can invalidate the review while its upload is paused.
                                         if (!applyAttempted[0] && (pendingUpload || edit != editGeneration)
                                                 && bp != null && sameLiveConnection(bp, connection)
                                                 && uiContext.iniFileState.getIniFileModel() == originalIni) {
                                             uploadTimer.restart();
                                        }
                                        updateLoadControls();
                                    });
                                }
                            }, onJobFinished);
                        }
                    },
                    callbacks,
                    () -> { }
                );
                return true;
        };
    }

    public boolean reviewRestorePoint(RestorePointStore.RestorePoint point) {
        return startTuneLoad.test(point.file.toString(), true);
    }

    boolean reviewTuneFile(String path) {
        return startTuneLoad.test(path, false);
    }

    static void uploadLoadedTune(BinaryProtocol bp, ConfigurationImage image) {
        bp.uploadChangesWithoutBurn(image);
    }

    static Map<Integer, ConfigurationImage> applyReviewedTuneToRam(BinaryProtocol bp, TuneLoadPlan plan,
                                       Set<TuneLoadPlan.Change> selected,
                                       Map<Integer, ConfigurationImage> devicePages, Runnable beforeWrite) {
        if (selected == null || selected.isEmpty()) {
            return devicePages;
        }
        Map<Integer, ConfigurationImage> target = plan.select(selected, devicePages);
        boolean mainChanged = !sameImage(devicePages.get(0), target.get(0));
        if (!sameImage(devicePages.get(0), bp.getControllerConfiguration())) {
            throw new IllegalStateException("The ECU tune changed. Start a new review.");
        }
        Set<Integer> changed = changedPages(devicePages, target);
        if (!mainChanged && changed.isEmpty()) {
            return target;
        }
        requireSecondaryRam(bp, devicePages, changed);
        if (mainChanged) {
            beforeWrite.run();
            uploadLoadedTune(bp, target.get(0));
        }
        for (int page : changed) {
            beforeWrite.run();
            TunePageIo.uploadSecondary(bp, devicePages, Map.of(page, target.get(page)));
        }
        return target;
    }

    /** Persist only pages changed by the accepted review, recording each acknowledged burn. */
    static void burnReviewedPages(BinaryProtocol bp, Map<Integer, ConfigurationImage> before,
                                  Map<Integer, ConfigurationImage> target, Set<Integer> acknowledged) {
        burnReviewedPages(bp, before, target, acknowledged, !sameImage(before.get(0), target.get(0)), () -> { });
    }

    static void burnReviewedPages(BinaryProtocol bp, Map<Integer, ConfigurationImage> before,
                                  Map<Integer, ConfigurationImage> target, Set<Integer> acknowledged,
                                  boolean reviewedMain, Runnable beforeEachBurn) {
        boolean mainChanged = !sameImage(before.get(0), target.get(0));
        Set<Integer> secondary = changedPages(before, target);
        if (reviewedMain) {
            beforeEachBurn.run();
            if (mainChanged) {
                bp.burn();
            } else if (!bp.burnPage(0)) {
                // The selected values may already be in RAM, but Apply and Burn explicitly persists them.
                throw new IllegalStateException("Burn failed for the main calibration page.");
            }
            bp.cacheCurrentImage();
            acknowledged.add(0);
        }
        burnSecondaryPages(bp, secondary, acknowledged, beforeEachBurn);
    }

    private static void burnSecondaryPages(BinaryProtocol bp, Set<Integer> pages, Set<Integer> acknowledged,
                                           Runnable beforeEachBurn) {
        for (int page : pages) {
            beforeEachBurn.run();
            TunePageIo.burnSecondary(bp, Set.of(page));
            acknowledged.add(page);
        }
    }

    private void adoptAcknowledgedBurns(Set<Integer> pages, Map<Integer, ConfigurationImage> target) {
        if (pages.contains(0)) {
            setBaselineImage(target.get(0));
        }
        if (pages.stream().anyMatch(page -> page != 0)) {
            secondaryBaseline = copyPages(secondaryBaseline);
            for (int page : pages) {
                if (page != 0 && target.get(page) != null) {
                    secondaryBaseline.put(page, target.get(page).clone());
                }
            }
            refreshState();
        }
    }

    public boolean isReviewInProgress() {
        return loadInProgress;
    }

    void adoptReviewedTune(CalibrationDialogWidget right, AtomicReference<String> currentKey, IniFileModel ini,
                           Map<Integer, ConfigurationImage> previous, Map<Integer, ConfigurationImage> replacement) {
        adoptReviewedTune(right, currentKey, ini, previous, replacement, true);
    }

    private void adoptReviewedTune(CalibrationDialogWidget right, AtomicReference<String> currentKey, IniFileModel ini,
                                    Map<Integer, ConfigurationImage> previous, Map<Integer, ConfigurationImage> replacement,
                                    boolean clearFilePath) {
        Map<Integer, ConfigurationImage> before = copyPages(previous);
        ConfigurationImage main = before.remove(0);
        // Newly reviewed pages get a reference snapshot, not an implicit burn or save of the result.
        secondaryBaseline = copyPages(secondaryBaseline);
        before.forEach(secondaryBaseline::putIfAbsent);
        savedPages = copyPages(savedPages);
        before.forEach(savedPages::putIfAbsent);
        flushBeforeNavigate();
        undoStack.push(new TuneSnapshot(main, before));
        if (undoStack.size() > MAX_UNDO) {
            undoStack.removeLast();
        }
        redoStack.clear();
        undoButton.setEnabled(true);
        redoButton.setEnabled(false);
        ++editGeneration;
        if (clearFilePath) {
            uiContext.setTuneFilePath(null);
        }
        publishLoadedPages(right, currentKey, ini, replacement);
    }

    private ConfigurationImage currentImage(CalibrationDialogWidget right) {
        BinaryProtocol bp = uiContext.isOfflineMode() ? null : uiContext.getBinaryProtocol();
        return imageToSave(right.getWorkingImage(), sessionImage.get(),
            bp == null ? null : bp.getControllerConfiguration(), baselineImage);
    }

    private void updateLoadControls() {
        boolean available = !burnInProgress && !loadInProgress && !firmwareUpdateInProgress;
        loadTuneAction.setEnabled(available && !saveInProgress);
        discardButton.setEnabled(available && baselineImage != null);
        burnButton.setEnabled(available && uiContext.getBinaryProtocol() != null && !uiContext.isOfflineMode());
    }

    private void requireCurrentReview(BinaryProtocol bp, IniFileModel ini, long connection, long edit) {
        BinaryProtocol active = uiContext.isOfflineMode() ? null : uiContext.getBinaryProtocol();
        if (firmwareUpdateInProgress || connectionGeneration != connection || editGeneration != edit
                || active != bp || bp != null && bp.isClosed() || uiContext.iniFileState.getIniFileModel() != ini) {
            throw new IllegalStateException("The tune or ECU connection changed. Start a new review.");
        }
    }

    private <T> T onCommunicationThread(Callable<T> action) throws Exception {
        FutureTask<T> task = new FutureTask<>(action);
        submit.accept(task);
        try {
            return task.get();
        } catch (java.util.concurrent.ExecutionException e) {
            if (e.getCause() instanceof Exception) {
                throw (Exception) e.getCause();
            }
            throw e;
        }
    }

    private void publishLoadedPages(CalibrationDialogWidget right, AtomicReference<String> currentKey,
                                    IniFileModel ini, Map<Integer, ConfigurationImage> pages) {
        Map<Integer, ConfigurationImage> secondary = copyPages(pages);
        ConfigurationImage image = secondary.remove(0);
        uiContext.setWorkingSecondaryTunePages(secondary);
        sessionImage.set(image);
        right.update(currentKey.get(), ini, image);
        publishReplacement();
        refreshState();
    }

    static ConfigurationImage applyLoadedTune(Msq tune, IniFileModel sourceIni,
                                                ConfigurationImage base, IniFileModel targetIni,
                                                UpdateOperationCallbacks callbacks) {
        return TuneLoadPreparation.prepare(tune, sourceIni, targetIni, Map.of(0, base), callbacks).incoming.get(0);
    }

    public void setFirmwareUpdateInProgress(boolean inProgress) {
        firmwareUpdateInProgress = inProgress;
        if (inProgress) {
            uploadTimer.stop();
        }
        updateLoadControls();
    }

    private void buildSaveTuneAction(UIContext uiContext,
                                     CalibrationDialogWidget right,
                                     AtomicReference<ConfigurationImage> sessionImage) {
        saveTuneAction = new AbstractAction(LoadTuneHelper.SAVE_TUNE_TEXT) {
            @Override
            public void actionPerformed(ActionEvent e) {
                saveTuneAndThen(right, null);
            }
        };
        saveTuneAsAction = new AbstractAction(LoadTuneHelper.SAVE_TUNE_AS_TEXT) {
            @Override
            public void actionPerformed(ActionEvent e) {
                saveTuneAsAndThen(right, null);
            }
        };
        createRestorePointAction = new AbstractAction(LoadTuneHelper.CREATE_RESTORE_POINT_TEXT) {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (firmwareUpdateInProgress) {
                    return;
                }
                String note = askRestorePointNote();
                if (note != null) {
                    createRestorePoint(right, note);
                }
            }
        };
        createRestorePointAction.putValue(Action.SHORT_DESCRIPTION,
            "Keep the tune open now, unburned changes too, to come back to later");
    }

    /** The note for a restore point made by hand: "" for none, null when cancelled. */
    private String askRestorePointNote() {
        JTextField note = new JTextField(28);
        JLabel noteLabel = new JLabel("Note (optional)");
        noteLabel.setLabelFor(note);
        JLabel hint = new JLabel("Saves the tune open now, including unburned changes.");
        hint.setForeground(TuningStyle.muted());
        JPanel form = new JPanel(new GridLayout(0, 1, 0, 6));
        form.add(noteLabel);
        form.add(note);
        form.add(hint);
        int choice = JOptionPane.showOptionDialog(getDialogParent(), form, "Create restore point",
            JOptionPane.OK_CANCEL_OPTION, JOptionPane.PLAIN_MESSAGE, null, new Object[]{"Create", "Cancel"}, "Create");
        return choice == 0 ? note.getText() : null;
    }

    /**
     * [tag:restore_points] Keeps the tune open now, unburned changes included, as a restore point with a note. Made
     * by hand, it is saved even when the newest point holds the same tune. Says so in the toolbar when done.
     */
    void createRestorePoint(CalibrationDialogWidget right, String note) {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        ConfigurationImage image = imageToSave(
            right.getWorkingImage(),
            sessionImage.get(),
            bp == null ? null : bp.getControllerConfiguration(),
            baselineImage
        );
        if (ini == null || image == null) {
            errorHandler.accept("No tune open to keep as a restore point");
            return;
        }
        Map<Integer, ConfigurationImage> pages = snapshotPages(image, workingSecondary());
        RestorePointStore store = restorePoints;
        fileWork.execute(() -> {
            try {
                RestorePointStore.RestorePoint point = store.saveAlways(MsqFactory.valueOf(pages, ini),
                    RestorePointStore.Reason.MANUAL, note);
                log.info("Restore point " + point.file);
                SwingUtilities.invokeLater(() -> showNote("Restore point created"));
            } catch (IOException | RuntimeException ex) {
                log.error("Failed to create restore point", ex);
                SwingUtilities.invokeLater(() -> errorHandler.accept("Failed to create restore point: " + ex.getMessage()));
            }
        });
    }

    /** A short message in place of the toolbar state for a few seconds, e.g. "Restore point created". */
    public void showNote(String text) {
        showNote(text, null, null);
    }

    /**
     * A note with a link after it, e.g. "Burned. Previous tune kept as a restore point · Restore points": a click
     * on the label (here or on its Gauges mirror) runs {@code action} while the note shows.
     */
    public void showNote(String text, String linkText, Runnable action) {
        noteAction = action;
        stateLabel.setCursor(action == null ? Cursor.getDefaultCursor() : Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
        stateLabel.setToolTipText(action == null ? null : "Open " + linkText);
        stateLabel.setForeground(TuningStyle.muted());
        stateLabel.setText(action == null ? text : noteHtml(text, linkText));
        noteTimer.restart();
    }

    static String noteHtml(String text, String linkText) {
        Color link = UIManager.getColor("Component.linkColor");
        String color = link == null ? "#1d4ed8" : String.format("#%06x", link.getRGB() & 0xffffff);
        return "<html>" + escapeHtml(text) + " · <u><font color=\"" + color + "\">" + escapeHtml(linkText)
            + "</font></u></html>";
    }

    private static String escapeHtml(String text) {
        return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;");
    }

    /** What the Burn note's link opens: the Restore Points window. */
    public void setShowRestorePoints(Runnable showRestorePoints) {
        this.showRestorePoints = showRestorePoints;
    }

    /** The note's link for tests: what a click on the label would run, or null. */
    Runnable getNoteActionForTests() {
        return noteAction;
    }

    public Action getCreateRestorePointAction() {
        return createRestorePointAction;
    }

    void setFileWorkForTests(java.util.concurrent.Executor fileWork) {
        this.fileWork = fileWork;
    }

    /** Saves to the current tune file without asking; asks for a file when there is none yet. */
    public void saveTuneAndThen(CalibrationDialogWidget right, Runnable onSuccess) {
        String current = uiContext.getTuneFilePath();
        if (current != null) {
            saveTuneToPathAndThen(right, current, onSuccess);
        } else {
            saveTuneAsAndThen(right, onSuccess);
        }
    }

    /** Asks for a file, starting at the current tune file, and makes the chosen file the current one. */
    public void saveTuneAsAndThen(CalibrationDialogWidget right, Runnable onSuccess) {
        if (loadInProgress || saveInProgress || firmwareUpdateInProgress) {
            return;
        }
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        ConfigurationImage image = imageToSave(
            right.getWorkingImage(),
            sessionImage.get(),
            bp == null ? null : bp.getControllerConfiguration(),
            baselineImage
        );
        if (ini == null || image == null) {
            errorHandler.accept("No configuration loaded");
            return;
        }
        String current = uiContext.getTuneFilePath();
        if (current != null) {
            saveTuneChooser.setSelectedFile(new File(current));
        } else {
            saveTuneChooser.setSelectedFile(null);
            TuneFileFolder.openIn(saveTuneChooser);
        }
        if (saveTuneChooser.showSaveDialog(getDialogParent()) != JFileChooser.APPROVE_OPTION) {
            return;
        }
        File selected = saveTuneChooser.getSelectedFile();
        TuneFileFolder.remember(selected);
        String path = selected.getAbsolutePath();
        if (!path.toLowerCase().endsWith(".msq")) {
            path += ".msq";
        }
        // JFileChooser does not ask before replacing a file; the current file is replaced by plain Save anyway
        if (new File(path).exists() && !new File(path).equals(current == null ? null : new File(current))
                && JOptionPane.showConfirmDialog(getDialogParent(), new File(path).getName() + " already exists. Replace it?",
                LoadTuneHelper.SAVE_TUNE_AS_TEXT, JOptionPane.YES_NO_OPTION, JOptionPane.WARNING_MESSAGE)
                != JOptionPane.YES_OPTION) {
            return;
        }
        saveTuneToPathAndThen(ini, image, path, onSuccess);
    }

    public void saveTuneToPathAndThen(CalibrationDialogWidget right, String path, Runnable onSuccess) {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        BinaryProtocol bp = uiContext.getBinaryProtocol();
        ConfigurationImage image = imageToSave(
            right.getWorkingImage(),
            sessionImage.get(),
            bp == null ? null : bp.getControllerConfiguration(),
            baselineImage
        );
        if (ini == null || image == null || path == null) {
            errorHandler.accept("No configuration loaded");
            return;
        }
        saveTuneToPathAndThen(ini, image, path, onSuccess);
    }

    private void saveTuneToPathAndThen(IniFileModel ini, ConfigurationImage image, String path, Runnable onSuccess) {
        if (loadInProgress || saveInProgress || firmwareUpdateInProgress) {
            return;
        }
        saveInProgress = true;
        updateLoadControls();
        final String finalPath = path;
        final Map<Integer, ConfigurationImage> pages = snapshotPages(image, workingSecondary());
        final BinaryProtocol bp = uiContext.isOfflineMode() ? null : uiContext.getBinaryProtocol();
        final long connection = connectionGeneration;
        final long edit = editGeneration;
        new Thread(() -> {
            try {
                if (bp != null) {
                    List<String> warnings = new ArrayList<>();
                    Map<Integer, ConfigurationImage> extra = onCommunicationThread(() -> {
                        if (!sameLiveConnection(bp, connection)) {
                            throw new IllegalStateException("The ECU session changed before the tune could be saved.");
                        }
                        return TunePageIo.readSecondary(bp, ini, warnings);
                    });
                    if (!warnings.isEmpty()) {
                        throw new IllegalStateException("Could not save the complete tune: " + String.join("; ", warnings));
                    }
                    // Online pages are edited on the ECU itself; use their fresh contents.
                    pages.putAll(extra);
                }
                Path destination = Path.of(finalPath).toAbsolutePath();
                Path temporary = Files.createTempFile(destination.getParent(), ".tune-", ".msq");
                try {
                    MsqFactory.valueOf(pages, ini).writeXmlFile(temporary.toString());
                    try {
                        Files.move(temporary, destination, StandardCopyOption.REPLACE_EXISTING, StandardCopyOption.ATOMIC_MOVE);
                    } catch (java.nio.file.AtomicMoveNotSupportedException e) {
                        Files.move(temporary, destination, StandardCopyOption.REPLACE_EXISTING);
                    }
                } finally {
                    Files.deleteIfExists(temporary);
                }
                SwingUtilities.invokeLater(() -> {
                    saveInProgress = false;
                    updateLoadControls();
                    if (connection != connectionGeneration || ini != uiContext.iniFileState.getIniFileModel()) {
                        return;
                    }
                    savedPages = copyPages(pages);
                    savedLayoutSignature = ini.getSignature();
                    uiContext.setTuneFilePath(finalPath);
                    if (edit == editGeneration && onSuccess != null) {
                        onSuccess.run();
                    }
                });
            } catch (Exception ex) {
                if (ex instanceof InterruptedException) {
                    Thread.currentThread().interrupt();
                }
                SwingUtilities.invokeLater(() -> {
                    saveInProgress = false;
                    updateLoadControls();
                    errorHandler.accept("Failed to save tune: " + ex.getMessage());
                });
            }
        }, "save-tune").start();
    }

    /**
     * [tag:restore_points] Keeps the tune about to be replaced as a restore point. Never throws: a point that cannot
     * be made is logged and the Burn or Load goes on.
     */
    static boolean saveRestorePoint(RestorePointStore store, ConfigurationImage image, IniFileModel ini,
                                    RestorePointStore.Reason reason) {
        return saveRestorePoint(store, image, ini, reason, null);
    }

    /**
     * @param vehicleName the car the point belongs to, when not the one named in {@code image}: before a Burn, the
     *                    tune being burned names it
     */
    static boolean saveRestorePoint(RestorePointStore store, ConfigurationImage image, IniFileModel ini,
                                    RestorePointStore.Reason reason, String vehicleName) {
        return image != null && saveRestorePoint(store, Map.of(0, image), ini, reason, vehicleName);
    }

    private static boolean saveRestorePoint(RestorePointStore store, Map<Integer, ConfigurationImage> pages,
                                            IniFileModel ini, RestorePointStore.Reason reason, String vehicleName) {
        if (store == null || !pages.containsKey(0) || ini == null) {
            return false;
        }
        try {
            Msq tune = MsqFactory.valueOf(pages, ini);
            // empty when the newest point already holds this tune: it is kept all the same
            (vehicleName == null ? store.save(tune, reason, null) : store.save(tune, vehicleName, reason, null))
                .ifPresent(point -> log.info("Restore point " + point.file));
            return true;
        } catch (IOException | RuntimeException e) {
            log.error("Failed to make a restore point (" + reason.label + "): " + e, e);
            return false;
        }
    }

    public RestorePointStore getRestorePointStore() {
        return restorePoints;
    }

    public void setRestorePointStoreForTests(RestorePointStore store) {
        restorePoints = store;
    }

    static ConfigurationImage imageToSave(ConfigurationImage workingImage,
                                          ConfigurationImage sessionImage,
                                          ConfigurationImage ecuImage,
                                          ConfigurationImage baselineImage) {
        if (workingImage != null) {
            return workingImage;
        }
        if (sessionImage != null) {
            return sessionImage;
        }
        return ecuImage != null ? ecuImage : baselineImage;
    }

    public AbstractAction getLoadTuneAction() {
        return loadTuneAction;
    }

    public void setErrorHandler(Consumer<String> errorHandler) {
        this.errorHandler = errorHandler;
    }

    public AbstractAction getSaveTuneAction() {
        return saveTuneAction;
    }

    public AbstractAction getSaveTuneAsAction() {
        return saveTuneAsAction;
    }

    /**
     * @return true if {@code current} differs from the loaded baseline (i.e. there are pending edits).
     * If there is no baseline yet, any non-null image counts as a change.
     */
    public boolean hasUnsavedChanges(ConfigurationImage current) {
        if (current == null) {
            return false;
        }
        if (baselineImage == null) {
            return true;
        }
        return !sameImage(current, baselineImage) || hasSecondaryEdits();
    }

    public boolean hasSecondaryEdits() {
        return !changedPages(secondaryBaseline, workingSecondary()).isEmpty()
            || !changedPages(workingSecondary(), secondaryBaseline).isEmpty();
    }

    public boolean hasUnsavedFileChanges(ConfigurationImage current) {
        if (current == null) {
            return false;
        }
        if (!sameImage(savedPages.get(0), current)) {
            return true;
        }
        return workingSecondary().entrySet().stream()
            .anyMatch(page -> !sameImage(savedPages.get(page.getKey()), page.getValue()));
    }

    /** @return the loaded baseline image (pre-edit), or null if none loaded. */
    public ConfigurationImage getBaselineImage() {
        return baselineImage;
    }

    /** Sets the baseline image for discard and enables the discard button. */
    public void setBaselineImage(ConfigurationImage image) {
        this.baselineImage = image == null ? null : image.clone();
        discardButton.setEnabled(!burnInProgress && image != null);
        refreshState();
        onBaselineChanged.run();
    }

    /** [tag:unburned_marks] Called after every burn, load or reconnect replaces the baseline. */
    public void setOnBaselineChanged(Runnable listener) {
        onBaselineChanged = listener == null ? () -> { } : listener;
    }

    private static @NotNull JFileChooser createMsqFileChooser() {
        JFileChooser chooser = new JFileChooser();
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        chooser.setFileFilter(new FileNameExtensionFilter("Tune files (.msq)", "msq"));
        return chooser;
    }

    public JPanel getPanel() {
        return panel;
    }

    private TuneReviewDialog.Decision reviewTune(String fileName, TuneLoadPlan plan, BinaryProtocol bp,
                                                  IniFileModel ini, long connection, long edit, boolean restoring) throws Exception {
        CompletableFuture<TuneReviewDialog.Decision> decision = new CompletableFuture<>();
        try {
            SwingUtilities.invokeAndWait(() -> {
                requireCurrentReview(bp, ini, connection, edit);
                TuneReviewDialog.show(fileName, plan, bp != null, restoring, reviewHost, decision);
            });
            // Only the load worker waits. The Console must stay responsive while the review card is open.
            return decision.get();
        } finally {
            decision.complete(null);
        }
    }

    public void setTuneReviewHost(Function<JComponent, Runnable> reviewHost) {
        this.reviewHost = reviewHost;
    }

    /** File menu actions can run before the lazy Tuning tab is attached to the console. */
    public void setDialogOwner(Window owner) {
        dialogOwner = owner;
    }

    private Component getDialogParent() {
        return dialogOwner != null ? dialogOwner : panel;
    }

    /**
     * Must be called on every edit event (from {@code right.setOnConfigChange}).
     * Captures the pre-edit baseline for undo and restarts the debounce + upload timers.
     */
    public void onEdit(ConfigurationImage previousSessionImage) {
        editGeneration++;
        undoBaseline.compareAndSet(null, previousSessionImage);
        undoCommitTimer.restart();
        if (!loadInProgress) {
            uploadTimer.restart();
        }
        refreshState();
    }

    /**
     * Returns true when a debounce window is open (i.e., the user is mid-edit).
     * Call {@link #flushBeforeNavigate()} before loading a new section.
     */
    public boolean isUndoTimerRunning() {
        return undoCommitTimer.isRunning();
    }

    /**
     * Flushes any pending debounce window to the undo stack.
     * Must be called before navigating to a different section so that the
     * in-progress edit becomes its own undo point.
     */
    public void flushBeforeNavigate() {
        if (undoCommitTimer.isRunning()) {
            undoCommitTimer.stop();
            ConfigurationImage baseline = undoBaseline.getAndSet(null);
            if (baseline != null) {
                undoStack.push(new TuneSnapshot(baseline, workingSecondary()));
                if (undoStack.size() > MAX_UNDO) undoStack.removeLast();
                redoStack.clear();
                undoButton.setEnabled(!undoStack.isEmpty());
                redoButton.setEnabled(!redoStack.isEmpty());
            }
        }
    }

    /**
     * Must be called when the ECU disconnects so stale state is dropped.
     * [tag:offline_tune] Preserves baselineImage so discard still works in offline mode.
     */
    public void onDisconnect() {
        connectionGeneration++;
        undoCommitTimer.stop();
        uploadTimer.stop();
        undoStack.clear();
        redoStack.clear();
        undoBaseline.set(null);
        undoButton.setEnabled(false);
        redoButton.setEnabled(false);
        // Disable burn button when ECU disconnects
        burnButton.setEnabled(false);
        discardButton.setEnabled(!burnInProgress && baselineImage != null);
        loadTuneAction.setEnabled(!firmwareUpdateInProgress && !burnInProgress && !loadInProgress && !saveInProgress);
        refreshState();
    }
}
