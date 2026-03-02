package com.rusefi.ui;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.FrontPageModel;
import com.opensr5.ini.IndicatorModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.TsStringFunction;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.core.SensorCentral;
import com.rusefi.ui.widgets.tune.CalibrationDialogWidget;
import com.rusefi.ui.widgets.tune.MainMenuTreeWidget;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayDeque;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

/**
 * Andrey Belomutskiy, (c) 2013-2026
 */
public class TuningPane {
    private static final int MAX_UNDO = 15;
    private static final int IDLE_TIMEOUT_MS = 300;
    private static final int UPLOAD_DELAY_MS = 100;

    private final JPanel content = new JPanel(new BorderLayout());

    public TuningPane(UIContext uiContext) {
        MainMenuTreeWidget left = new MainMenuTreeWidget(uiContext);

        CalibrationDialogWidget right = new CalibrationDialogWidget(uiContext);
        JScrollPane rightScrollPane = new JScrollPane(right.getContentPane());

        final String[] currentKey = {null};

        // Accumulated tune edits across all dialogs for this session.
        final ConfigurationImage[] sessionImage = {null};

        // Undo/redo history — each entry is a snapshot of sessionImage before an edit.
        final ArrayDeque<ConfigurationImage> undoStack = new ArrayDeque<>();
        final ArrayDeque<ConfigurationImage> redoStack = new ArrayDeque<>();
        final ConfigurationImage[] undoBaseline = {null};

        JButton undoButton = new JButton("Undo");
        JButton redoButton = new JButton("Redo");
        undoButton.setEnabled(false);
        redoButton.setEnabled(false);

        Runnable updateUndoRedoButtons = () -> {
            undoButton.setEnabled(!undoStack.isEmpty());
            redoButton.setEnabled(!redoStack.isEmpty());
        };

        // Commits any captured baseline to the undo stack (called by timer and on navigation).
        Runnable flushUndoBaseline = () -> {
            if (undoBaseline[0] != null) {
                undoStack.push(undoBaseline[0]);
                if (undoStack.size() > MAX_UNDO) undoStack.removeLast();
                redoStack.clear();
                undoBaseline[0] = null;
                updateUndoRedoButtons.run();
            }
        };

        // Debounce timer: coalesces rapid edits (e.g., per-keystroke text field events) into a single undo point
        Timer undoCommitTimer = new Timer(IDLE_TIMEOUT_MS, e -> flushUndoBaseline.run());
        undoCommitTimer.setRepeats(false);

        // The actual burn to flash is deferred until the user explicitly clicks "Burn to ECU".
        Timer uploadTimer = new Timer(UPLOAD_DELAY_MS, e -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null || sessionImage[0] == null) return;
            final ConfigurationImage snapshot = sessionImage[0].clone();
            uiContext.getLinkManager().submit(() -> bp.uploadChangesWithoutBurn(snapshot));
        });
        uploadTimer.setRepeats(false);

        Runnable onDiscardExtra = () -> {
            undoCommitTimer.stop();
            undoStack.clear();
            redoStack.clear();
            undoBaseline[0] = null;
            updateUndoRedoButtons.run();
        };

        JPanel toolbar = getToolbar(uiContext, right, currentKey, sessionImage, onDiscardExtra);
        toolbar.add(undoButton);
        toolbar.add(redoButton);

        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, left.getContentPane(), rightScrollPane);
        splitPane.setResizeWeight(0.3);

        left.setOnSelect(subMenu -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null || bp.getControllerConfiguration() == null) {
                return;
            }

            // Flush any in-progress debounce window before loading a new section.
            if (undoCommitTimer.isRunning()) {
                undoCommitTimer.stop();
                flushUndoBaseline.run();
            }

            // On first navigation, seed the session image from the live ECU state.
            // On subsequent navigations, carry forward whatever the user has edited so far
            // so that changes made in dialogs, tables, or curves are not lost when opening another.
            ConfigurationImage pending = right.getWorkingImage();
            if (pending != null) {
                sessionImage[0] = pending;
            } else if (sessionImage[0] == null) {
                sessionImage[0] = bp.getControllerConfiguration().clone();
            }

            currentKey[0] = subMenu.getKey();
            right.update(subMenu.getKey(), uiContext.iniFileState.getIniFileModel(), sessionImage[0]);
        });

        // All edit events (dialog fields, table cells, curve drags) flow through onConfigChange.
        // Text fields fire per-keystroke, so we use a debounce timer to coalesce them into
        // a single undo point per editing pause.
        right.setOnConfigChange(image -> {
            // Capture the pre-edit state at the start of each debounce window.
            if (undoBaseline[0] == null && sessionImage[0] != null) {
                undoBaseline[0] = sessionImage[0];
            }
            // Clone because workingImage is mutated in-place by further edits.
            sessionImage[0] = image.clone();
            left.refreshExpressions(image);
            undoCommitTimer.restart();
            uploadTimer.restart();
        });

        undoButton.addActionListener(e -> {
            if (undoStack.isEmpty()) return;
            // Discard any uncommitted edit sequence so we don't accidentally push it.
            undoCommitTimer.stop();
            undoBaseline[0] = null;
            if (sessionImage[0] != null) redoStack.push(sessionImage[0]);
            sessionImage[0] = undoStack.pop();
            if (currentKey[0] != null) {
                right.update(currentKey[0], uiContext.iniFileState.getIniFileModel(), sessionImage[0]);
            }
            updateUndoRedoButtons.run();
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
            updateUndoRedoButtons.run();
        });

        JPanel northPanel = new JPanel();
        northPanel.setLayout(new BoxLayout(northPanel, BoxLayout.Y_AXIS));
        northPanel.add(toolbar);
        JPanel indicatorPanel = buildFrontendIndicatorPanel(uiContext);
        if (indicatorPanel != null) {
            northPanel.add(indicatorPanel);
        }

        content.add(northPanel, BorderLayout.NORTH);
        content.add(splitPane, BorderLayout.CENTER);
    }


    // TODO: move buttons to a factory!, also the undo/redo
    private static @NotNull JPanel getToolbar(UIContext uiContext, CalibrationDialogWidget right,
                                              String[] currentKey, ConfigurationImage[] sessionImage,
                                              Runnable onDiscardExtra) {
        JButton burnButton = new JButton("Burn to ECU");
        burnButton.addActionListener(e -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            // Use the working image
            // fall back to the session image if no content has been opened yet.
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

        JButton discardButton = new JButton("Discard Changes");
        discardButton.addActionListener(e -> {
            BinaryProtocol bp = uiContext.getBinaryProtocol();
            if (bp == null) return;
            // Reset session to the snapshot captured when we connected to this ECU.
            ConfigurationImage baseline = bp.getCachedImage();
            if (baseline == null) baseline = bp.getControllerConfiguration();
            sessionImage[0] = baseline.clone();
            if (currentKey[0] != null) {
                right.update(currentKey[0], uiContext.iniFileState.getIniFileModel(), sessionImage[0]);
            }
            onDiscardExtra.run();
        });

        JPanel toolbar = new JPanel(new FlowLayout(FlowLayout.LEFT));
        toolbar.add(burnButton);
        toolbar.add(discardButton);
        return toolbar;
    }

    private static JPanel buildFrontendIndicatorPanel(UIContext uiContext) {
        IniFileModel ini = uiContext.iniFileState.getIniFileModel();
        if (ini == null) return null;
        FrontPageModel frontPage = ini.getFrontPage();
        if (frontPage == null) return null;
        List<IndicatorModel> indicators = frontPage.getIndicators();
        if (indicators.isEmpty()) return null;

        JPanel panel = new JPanel(new FlowLayout(FlowLayout.LEFT, 4, 2));
        for (IndicatorModel model : indicators) {
            JLabel label = new JLabel(model.getOffLabel().isEmpty() ? " " : model.getOffLabel());
            label.setOpaque(true);
            label.setBorder(BorderFactory.createCompoundBorder(
                    BorderFactory.createLineBorder(Color.GRAY, 1),
                BorderFactory.createEmptyBorder(4, 8, 4, 8)));
            label.setBackground(parseIndicatorColor(model.getOffBg()));
            label.setForeground(parseIndicatorColor(model.getOffFg()));
            panel.add(label);

            SensorCentral.getInstance().addListener(() -> {
                // Gather variables from the condition and both labels (e.g. bitStringValue index args).
                Set<String> vars = new java.util.HashSet<>();
                vars.addAll(ExpressionEvaluator.extractVariables(model.getExpression()));
                vars.addAll(ExpressionEvaluator.extractVariables(model.getOnLabel()));
                vars.addAll(ExpressionEvaluator.extractVariables(model.getOffLabel()));
                Map<String, Double> context = new HashMap<>();
                for (String var : vars) {
                    double val = SensorCentral.getInstance().getValue(var);
                    if (!Double.isNaN(val)) context.put(var, val);
                }
                Boolean active = ExpressionEvaluator.evaluateBooleanExpression(model.getExpression(), context);
                final String text;
                final Color bg, fg;
                if (Boolean.TRUE.equals(active)) {
                    text = resolveIndicatorLabel(model.getOnLabel(), ini, context);
                    bg = parseIndicatorColor(model.getOnBg());
                    fg = parseIndicatorColor(model.getOnFg());
                } else {
                    text = resolveIndicatorLabel(model.getOffLabel(), ini, context);
                    bg = parseIndicatorColor(model.getOffBg());
                    fg = parseIndicatorColor(model.getOffFg());
                }
                SwingUtilities.invokeLater(() -> {
                    label.setText(text.isEmpty() ? " " : text);
                    label.setBackground(bg);
                    label.setForeground(fg);
                });
            });
        }
        return panel;
    }

    private static String resolveIndicatorLabel(String label, IniFileModel ini, Map<String, Double> context) {
        if (label == null) return "";
        if (TsStringFunction.containsStringFunction(label)) {
            String resolved = TsStringFunction.resolve(label, ini, null, context);
            return resolved != null ? resolved : "";
        }
        String t = label.trim();
        if (t.startsWith("{")) {
            return t.replaceAll("^\\{\\s*", "").replaceAll("\\s*}$", "").trim();
        }
        return t;
    }

    private static Color parseIndicatorColor(String name) {
        if (name == null) return Color.LIGHT_GRAY;
        switch (name.toLowerCase().trim()) {
            case "white":  return Color.WHITE;
            case "black":  return Color.BLACK;
            case "red":    return Color.RED;
            case "yellow": return Color.YELLOW;
            case "green":  return Color.GREEN;
            case "blue":   return Color.BLUE;
            default:       return Color.LIGHT_GRAY;
        }
    }

    public JPanel getContent() {
        return content;
    }
}
