package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.ui.UIContext;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayDeque;

/**
 * Toolbar for the Tune tab: Burn, Discard, Undo, and Redo buttons.
 * Also owns the undo/redo history stacks and the upload/debounce timers
 * so they are kept out of TuningPane.
 */
public class TuningToolbarWidget {
    private static final int MAX_UNDO = 15;
    private static final int IDLE_TIMEOUT_MS = 300;
    private static final int UPLOAD_DELAY_MS = 100;

    private final JPanel panel;

    private final ArrayDeque<ConfigurationImage> undoStack = new ArrayDeque<>();
    private final ArrayDeque<ConfigurationImage> redoStack = new ArrayDeque<>();
    private final ConfigurationImage[] undoBaseline = {null};

    private final JButton undoButton = new JButton("Undo");
    private final JButton redoButton = new JButton("Redo");

    private final Timer undoCommitTimer;
    private final Timer uploadTimer;

    /**
     * @param uiContext    live context (BinaryProtocol, LinkManager)
     * @param right        the dialog widget — needed for get/update working image
     * @param currentKey   single-element array holding the currently displayed menu key
     * @param sessionImage single-element array holding the accumulated session image
     */
    public TuningToolbarWidget(UIContext uiContext,
                                CalibrationDialogWidget right,
                                String[] currentKey,
                                ConfigurationImage[] sessionImage) {
        undoButton.setEnabled(false);
        redoButton.setEnabled(false);

        Runnable updateButtons = () -> {
            undoButton.setEnabled(!undoStack.isEmpty());
            redoButton.setEnabled(!redoStack.isEmpty());
        };

        Runnable flushUndoBaseline = () -> {
            if (undoBaseline[0] != null) {
                undoStack.push(undoBaseline[0]);
                if (undoStack.size() > MAX_UNDO) undoStack.removeLast();
                redoStack.clear();
                undoBaseline[0] = null;
                updateButtons.run();
            }
        };

        undoCommitTimer = new Timer(IDLE_TIMEOUT_MS, e -> flushUndoBaseline.run());
        undoCommitTimer.setRepeats(false);

        uploadTimer = new Timer(UPLOAD_DELAY_MS, e -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null || sessionImage[0] == null) return;
            final ConfigurationImage snapshot = sessionImage[0].clone();
            uiContext.getLinkManager().submit(() -> bp.uploadChangesWithoutBurn(snapshot));
        });
        uploadTimer.setRepeats(false);

        JButton burnButton = getBurnToEcuButton(uiContext, right, sessionImage);

        JButton discardButton = getDiscardButton(uiContext, right, sessionImage, currentKey, updateButtons);

        undoButton.addActionListener(e -> {
            if (undoStack.isEmpty()) return;
            undoCommitTimer.stop();
            undoBaseline[0] = null;
            if (sessionImage[0] != null) redoStack.push(sessionImage[0]);
            sessionImage[0] = undoStack.pop();
            if (currentKey[0] != null) {
                right.update(currentKey[0], uiContext.iniFileState.getIniFileModel(), sessionImage[0]);
            }
            updateButtons.run();
        });

        redoButton.addActionListener(e -> {
            if (redoStack.isEmpty()) return;
            undoCommitTimer.stop();
            undoBaseline[0] = null;
            if (sessionImage[0] != null) undoStack.push(sessionImage[0]);
            sessionImage[0] = redoStack.pop();
            if (currentKey[0] != null) {
                right.update(currentKey[0], uiContext.iniFileState.getIniFileModel(), sessionImage[0]);
            }
            updateButtons.run();
        });

        panel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        panel.add(burnButton);
        panel.add(discardButton);
        panel.add(undoButton);
        panel.add(redoButton);
    }

    private @NotNull JButton getBurnToEcuButton(UIContext uiContext,
                                                CalibrationDialogWidget right,
                                                ConfigurationImage[] sessionImage) {
        JButton burnButton = new JButton("Burn to ECU");
        burnButton.addActionListener(e -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            ConfigurationImage toBurn = right.getWorkingImage();
            if (toBurn == null) toBurn = sessionImage[0];
            if (bp == null || toBurn == null) return;
            final ConfigurationImage image = toBurn;
            sessionImage[0] = image;
            uiContext.getLinkManager().submit(() -> {
                bp.burn();
                bp.setConfigurationImage(image);
            });
        });
        return burnButton;
    }

    private @NotNull JButton getDiscardButton(UIContext uiContext,
                                              CalibrationDialogWidget right,
                                              ConfigurationImage[] sessionImage,
                                              String[] currentKey,
                                              Runnable updateButtons) {
        JButton discardButton = new JButton("Discard changes");

        discardButton.addActionListener(e -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null) return;
            ConfigurationImage baseline = bp.getCachedImage();
            if (baseline == null) baseline = bp.getControllerConfiguration();
            sessionImage[0] = baseline.clone();
            if (currentKey[0] != null) {
                right.update(currentKey[0], uiContext.iniFileState.getIniFileModel(), sessionImage[0]);
            }
            undoCommitTimer.stop();
            undoStack.clear();
            redoStack.clear();
            undoBaseline[0] = null;
            updateButtons.run();
        });

        return discardButton;
    }

    public JPanel getPanel() {
        return panel;
    }

    /**
     * Must be called on every edit event (from {@code right.setOnConfigChange}).
     * Captures the pre-edit baseline for undo and restarts the debounce + upload timers.
     */
    public void onEdit(ConfigurationImage previousSessionImage) {
        if (undoBaseline[0] == null && previousSessionImage != null) {
            undoBaseline[0] = previousSessionImage;
        }
        undoCommitTimer.restart();
        uploadTimer.restart();
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
            if (undoBaseline[0] != null) {
                undoStack.push(undoBaseline[0]);
                if (undoStack.size() > MAX_UNDO) undoStack.removeLast();
                redoStack.clear();
                undoBaseline[0] = null;
                undoButton.setEnabled(!undoStack.isEmpty());
                redoButton.setEnabled(!redoStack.isEmpty());
            }
        }
    }

    /**
     * Must be called when the ECU disconnects so stale state is dropped.
     */
    public void onDisconnect() {
        undoCommitTimer.stop();
        uploadTimer.stop();
        undoStack.clear();
        redoStack.clear();
        undoBaseline[0] = null;
        undoButton.setEnabled(false);
        redoButton.setEnabled(false);
    }
}
