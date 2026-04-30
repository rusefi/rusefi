package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.IniFileModel;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.jobs.AsyncJob;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;
import com.rusefi.maintenance.jobs.JobHelper;
import com.rusefi.tune.xml.Msq;
import com.rusefi.tune.xml.MsqFactory;
import com.rusefi.ui.StatusWindow;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.basic.LoadTuneHelper;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.File;
import java.util.ArrayDeque;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicReference;

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
    private final AtomicReference<ConfigurationImage> undoBaseline = new AtomicReference<>();

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
                                AtomicReference<String> currentKey,
                                AtomicReference<ConfigurationImage> sessionImage) {
        undoButton.setEnabled(false);
        redoButton.setEnabled(false);

        Runnable updateButtons = () -> {
            undoButton.setEnabled(!undoStack.isEmpty());
            redoButton.setEnabled(!redoStack.isEmpty());
        };

        Runnable flushUndoBaseline = () -> {
            ConfigurationImage baseline = undoBaseline.getAndSet(null);
            if (baseline != null) {
                undoStack.push(baseline);
                if (undoStack.size() > MAX_UNDO) {
                    undoStack.removeLast();
                }
                redoStack.clear();
                updateButtons.run();
            }
        };

        undoCommitTimer = new Timer(IDLE_TIMEOUT_MS, e -> flushUndoBaseline.run());
        undoCommitTimer.setRepeats(false);

        uploadTimer = new Timer(UPLOAD_DELAY_MS, e -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            ConfigurationImage image = sessionImage.get();
            if (bp == null || image == null) {
                return;
            }
            final ConfigurationImage snapshot = image.clone();
            uiContext.getLinkManager().submit(() -> bp.uploadChangesWithoutBurn(snapshot));
        });
        uploadTimer.setRepeats(false);

        JButton burnButton = getBurnToEcuButton(uiContext, right, sessionImage);

        JButton discardButton = getDiscardButton(uiContext, right, sessionImage, currentKey, updateButtons);

        JButton loadTuneButton = getLoadTuneButton(uiContext, right, currentKey, sessionImage);
        JButton saveTuneButton = getSaveTuneButton(uiContext, right, sessionImage);

        undoButton.addActionListener(e -> {
            if (undoStack.isEmpty()) {
                return;
            }
            undoCommitTimer.stop();
            undoBaseline.set(null);
            ConfigurationImage image = sessionImage.get();
            if (image != null) {
                redoStack.push(image);
            }
            sessionImage.set(undoStack.pop());
            String key = currentKey.get();
            if (key != null) {
                right.update(key, uiContext.iniFileState.getIniFileModel(), sessionImage.get());
            }
            updateButtons.run();
        });

        redoButton.addActionListener(e -> {
            if (redoStack.isEmpty()) {
                return;
            }
            undoCommitTimer.stop();
            undoBaseline.set(null);
            ConfigurationImage image = sessionImage.get();
            if (image != null) {
                undoStack.push(image);
            }
            sessionImage.set(redoStack.pop());
            String key = currentKey.get();
            if (key != null) {
                right.update(key, uiContext.iniFileState.getIniFileModel(), sessionImage.get());
            }
            updateButtons.run();
        });

        panel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        panel.add(burnButton);
        panel.add(discardButton);
        panel.add(undoButton);
        panel.add(redoButton);
        panel.add(loadTuneButton);
        panel.add(saveTuneButton);
    }

    private @NotNull JButton getBurnToEcuButton(UIContext uiContext,
                                                CalibrationDialogWidget right,
                                                AtomicReference<ConfigurationImage> sessionImage) {
        JButton burnButton = new JButton("Burn to ECU");
        burnButton.addActionListener(e -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            ConfigurationImage toBurn = right.getWorkingImage();
            if (toBurn == null) {
                toBurn = sessionImage.get();
            }
            if (bp == null || toBurn == null) {
                return;
            }
            final ConfigurationImage image = toBurn;
            sessionImage.set(image);
            uiContext.getLinkManager().submit(() -> {
                bp.burn();
                bp.setConfigurationImage(image);
            });
        });
        return burnButton;
    }

    private @NotNull JButton getDiscardButton(UIContext uiContext,
                                              CalibrationDialogWidget right,
                                              AtomicReference<ConfigurationImage> sessionImage,
                                              AtomicReference<String> currentKey,
                                              Runnable updateButtons) {
        JButton discardButton = new JButton("Discard changes");

        discardButton.addActionListener(e -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null) {
                return;
            }
            ConfigurationImage baseline = bp.getCachedImage();
            if (baseline == null) {
                baseline = bp.getControllerConfiguration();
            }
            sessionImage.set(baseline.clone());
            String key = currentKey.get();
            if (key != null) {
                right.update(key, uiContext.iniFileState.getIniFileModel(), sessionImage.get());
            }
            undoCommitTimer.stop();
            undoStack.clear();
            redoStack.clear();
            undoBaseline.set(null);
            updateButtons.run();
        });

        return discardButton;
    }

    private @NotNull JButton getLoadTuneButton(UIContext uiContext,
                                                CalibrationDialogWidget right,
                                                AtomicReference<String> currentKey,
                                                AtomicReference<ConfigurationImage> sessionImage) {
        JFileChooser chooser = createMsqFileChooser();
        JButton button = new JButton(LoadTuneHelper.LOAD_TUNE_TEXT);
        button.addActionListener(e -> {
            if (chooser.showOpenDialog(button) != JFileChooser.APPROVE_OPTION) {
                return;
            }
            IniFileModel ini = uiContext.iniFileState.getIniFileModel();
            if (ini == null) {
                JOptionPane.showMessageDialog(button, "No INI file loaded", "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }
            final String path = chooser.getSelectedFile().getAbsolutePath();
            final String fileName = chooser.getSelectedFile().getName();
            ConfigurationImage sessionImg = sessionImage.get();
            final ConfigurationImage base = sessionImg != null
                    ? sessionImg
                    : new ConfigurationImage(ini.getMetaInfo().getPageSize(0));
            final AtomicReference<ConfigurationImage> result = new AtomicReference<>();
            final StatusWindow statusWindow = new StatusWindow();
            statusWindow.showFrame("Load Tune");
            final UpdateOperationCallbacks callbacks = statusWindow.getContent();

            AsyncJobExecutor.INSTANCE.executeJob(
                new AsyncJob("Load Tune") {
                    @Override
                    public void doJob(UpdateOperationCallbacks cb, Runnable onJobFinished) {
                        JobHelper.doJob(() -> {
                            try {
                                callbacks.logLine("Reading " + fileName + "...");
                                Msq msq = Msq.readTune(path);
                                callbacks.logLine("Applying tune fields...");
                                ConfigurationImage newImage = msq.applyOnto(base, ini);
                                BinaryProtocol bp = uiContext.getBinaryProtocol();
                                if (bp != null) {
                                    callbacks.logLine("Uploading and burning to ECU...");
                                    CountDownLatch latch = new CountDownLatch(1);
                                    uiContext.getLinkManager().submit(() -> {
                                        try {
                                            bp.uploadChanges(newImage);
                                        } finally {
                                            latch.countDown();
                                        }
                                    });
                                    latch.await();
                                }
                                result.set(newImage);
                                callbacks.done();
                            } catch (Exception ex) {
                                callbacks.logLine("Error: " + ex.getMessage());
                                callbacks.error();
                            }
                        }, onJobFinished);
                    }
                },
                callbacks,
                () -> SwingUtilities.invokeLater(() -> {
                    ConfigurationImage res = result.get();
                    if (res != null) {
                        statusWindow.getFrame().dispose();
                        sessionImage.set(res);
                        String key = currentKey.get();
                        if (key != null) {
                            right.update(key, ini, res);
                        }
                        uiContext.fireConfigImageChanged(res);
                    }
                })
            );
        });
        return button;
    }

    private @NotNull JButton getSaveTuneButton(UIContext uiContext,
                                               CalibrationDialogWidget right,
                                               AtomicReference<ConfigurationImage> sessionImage) {
        JFileChooser chooser = createMsqFileChooser();
        JButton button = new JButton("Save Tune To File");
        button.addActionListener(e -> {
            IniFileModel ini = uiContext.iniFileState.getIniFileModel();
            ConfigurationImage image = right.getWorkingImage();
            if (image == null) image = sessionImage.get();
            if (image == null) {
                BinaryProtocol bp = uiContext.getBinaryProtocol();
                if (bp != null) {
                    image = bp.getControllerConfiguration();
                }
            }
            if (ini == null || image == null) {
                JOptionPane.showMessageDialog(button, "No configuration loaded", "Error", JOptionPane.ERROR_MESSAGE);
                return;
            }
            if (chooser.showSaveDialog(button) != JFileChooser.APPROVE_OPTION) {
                return;
            }
            File selected = chooser.getSelectedFile();
            String path = selected.getAbsolutePath();
            if (!path.toLowerCase().endsWith(".msq")) {
                path += ".msq";
            }
            final String finalPath = path;
            final ConfigurationImage finalImage = image;
            new Thread(() -> {
                try {
                    MsqFactory.valueOf(finalImage, ini).writeXmlFile(finalPath);
                } catch (Exception ex) {
                    SwingUtilities.invokeLater(() -> JOptionPane.showMessageDialog(
                            button, "Failed to save tune: " + ex.getMessage(), "Error", JOptionPane.ERROR_MESSAGE));
                }
            }, "save-tune").start();
        });
        return button;
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

    /**
     * Must be called on every edit event (from {@code right.setOnConfigChange}).
     * Captures the pre-edit baseline for undo and restarts the debounce + upload timers.
     */
    public void onEdit(ConfigurationImage previousSessionImage) {
        undoBaseline.compareAndSet(null, previousSessionImage);
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
            ConfigurationImage baseline = undoBaseline.getAndSet(null);
            if (baseline != null) {
                undoStack.push(baseline);
                if (undoStack.size() > MAX_UNDO) undoStack.removeLast();
                redoStack.clear();
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
        undoBaseline.set(null);
        undoButton.setEnabled(false);
        redoButton.setEnabled(false);
    }
}
