package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.CurveModel;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.ExpressionEvaluator;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PanelModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.IniField;
import com.rusefi.ui.UIContext;
import com.rusefi.ui.laf.GradientTitleBorder;
import com.rusefi.ui.util.ScrollablePanel;
import com.rusefi.ui.util.SwingUtil;
import com.rusefi.ui.util.WrapLayout;

import javax.swing.*;
import java.awt.*;
import java.util.*;
import java.util.List;

/**
 * Orchestrates layout of calibration dialogs by composing UI widgets
 * created by {@link CalibrationFieldFactory}.
 *
 * @see TuningTableView
 */
public class CalibrationDialogWidget {
    private final JPanel contentPane = new ScrollablePanel();
    private final UIContext uiContext;
    private ConfigurationImage workingImage;
    private IniFileModel currentIniFileModel;
    private final List<ExpressionRow> expressionRows = new ArrayList<>();

    /**
     * Tracks a field row that has visibility or enabled expressions to re-evaluate on config changes.
     */
    private static class ExpressionRow {
        final JPanel row;
        final String enableExpression;
        final String visibleExpression;

        ExpressionRow(JPanel row, String enableExpression, String visibleExpression) {
            this.row = row;
            this.enableExpression = enableExpression;
            this.visibleExpression = visibleExpression;
        }
    }

    public CalibrationDialogWidget(UIContext uiContext) {
        this.uiContext = uiContext;
        contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
        contentPane.setAlignmentX(Component.LEFT_ALIGNMENT);
    }

    private static void applyLayout(JPanel panel, String layoutHint) {
        if ("border".equalsIgnoreCase(layoutHint)) {
            // Equal-width columns keep the .ini West/East intent;
            // children wrap inside their allocated width via WrapLayout.
            panel.setLayout(new GridLayout(1, 0));
        } else if ("xAxis".equalsIgnoreCase(layoutHint)) {
            panel.setLayout(new BoxLayout(panel, BoxLayout.X_AXIS));
        } else {
            panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
        }
    }

    public ConfigurationImage getWorkingImage() {
        return workingImage;
    }

    public void update(DialogModel dialogModel, IniFileModel iniFileModel, ConfigurationImage ci) {
        workingImage = ci != null ? ci.clone() : null;
        currentIniFileModel = iniFileModel;
        expressionRows.clear();
        contentPane.removeAll();
        if (dialogModel != null) {
            applyLayout(contentPane, dialogModel.getLayoutHint());
            contentPane.setAlignmentX(Component.LEFT_ALIGNMENT);
            fillPanel(contentPane, dialogModel, iniFileModel, ci);
        }
        contentPane.revalidate();
        contentPane.repaint();
        // After the initial layout gives children their actual widths,
        // WrapLayout can compute correct wrapped heights on the second pass.
        // invalidateTree is needed because revalidate() only invalidates the
        // component and its ancestors, not children — so children return
        // stale cached preferred sizes computed before wrapping.
        SwingUtilities.invokeLater(() -> {
            SwingUtil.invalidateTree(contentPane);
            contentPane.revalidate();
            contentPane.repaint();
        });
    }

    public void update(String key) {
        IniFileModel iniFileModel = uiContext.iniFileState.getIniFileModel();
        ConfigurationImage ci = uiContext.getBinaryProtocol().getControllerConfiguration();
        update(key, iniFileModel, ci);
    }

    public void update(String key, IniFileModel iniFileModel, ConfigurationImage ci) {
        contentPane.removeAll();
        if (key != null) {
            DialogModel dialog = iniFileModel.getDialogs().get(key);
            if (dialog != null) {
                update(dialog, iniFileModel, ci);
                return;
            }

            TableModel table = iniFileModel.getTable(key);
            if (table != null) {
                contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
                TuningTableView tuningTableView = new TuningTableView(table.getTitle());
                tuningTableView.displayTable(iniFileModel, table.getTableId(), ci);
                contentPane.add(tuningTableView.getContent());
            } else {
                CurveModel curve = iniFileModel.getCurves().get(key);
                if (curve != null) {
                    contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
                    CurveWidget curveWidget = new CurveWidget(curve, iniFileModel, ci);
                    contentPane.add(curveWidget.getContentPane());
                }
            }
        }
        contentPane.revalidate();
        contentPane.repaint();
    }

    private void fillPanel(JPanel container, DialogModel dialogModel, IniFileModel iniFileModel, ConfigurationImage ci) {
        Runnable onChange = this::refreshExpressions;

        for (DialogModel.Field field : dialogModel.getFields()) {
            JPanel row;
            Optional<IniField> iniField = iniFileModel.findIniField(field.getKey());
            row = iniField.map(value -> CalibrationFieldFactory.createFieldRow(field, value, ci, workingImage, onChange)).orElseGet(() -> CalibrationFieldFactory.createLabelRow(field));

            boolean hasExpressions = field.getEnableExpression() != null || field.getVisibleExpression() != null;
            if (hasExpressions) {
                ExpressionRow exprRow = new ExpressionRow(row, field.getEnableExpression(), field.getVisibleExpression());
                expressionRows.add(exprRow);

                // Initial evaluation against workingImage
                ConfigurationImage evalImage = workingImage != null ? workingImage : ci;
                if (evalImage != null) {
                    applyExpressionState(exprRow, iniFileModel, evalImage);
                }
            }

            container.add(row);
        }

        for (DialogModel.Command command : dialogModel.getCommandsOfCurrentDialog()) {
            container.add(CalibrationFieldFactory.createCommandRow(command));
        }

        boolean isGridLayout = container.getLayout() instanceof GridLayout;
        List<PanelModel> panels = dialogModel.getPanels();
        JPanel horizontalPanel = null;
        for (PanelModel panel : panels) {
            String placement = panel.getPlacement();
            boolean isHorizontal = "west".equalsIgnoreCase(placement) || "center".equalsIgnoreCase(placement) || "east".equalsIgnoreCase(placement);

            JPanel targetContainer;
            if (isGridLayout) {
                targetContainer = container;
            } else if (isHorizontal) {
                if (horizontalPanel == null) {
                    horizontalPanel = new JPanel(new WrapLayout(FlowLayout.LEFT, 0, 0));
                    horizontalPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
                    container.add(horizontalPanel);
                }
                targetContainer = horizontalPanel;
            } else {
                horizontalPanel = null;
                targetContainer = container;
            }

            CurveModel curve = iniFileModel.getCurves().get(panel.getPanelName());
            if (curve != null) {
                CurveWidget curveWidget = new CurveWidget(curve, iniFileModel, ci);
                JComponent content = curveWidget.getContentPane();
                CalibrationFieldFactory.applyStyle(content);
                content.setAlignmentX(Component.LEFT_ALIGNMENT);
                targetContainer.add(content);
                continue;
            }

            TableModel table = iniFileModel.getTable(panel.getPanelName());
            if (table != null) {
                TuningTableView tuningTableView = new TuningTableView(table.getTitle());
                tuningTableView.displayTable(iniFileModel, table.getTableId(), ci);
                JComponent content = tuningTableView.getContent();
                CalibrationFieldFactory.applyStyle(content);
                content.setAlignmentX(Component.LEFT_ALIGNMENT);
                targetContainer.add(content);
                continue;
            }

            JPanel panelWidget = new JPanel();
            panelWidget.setAlignmentX(Component.LEFT_ALIGNMENT);
            DialogModel subDialog = panel.resolveDialog(iniFileModel);
            String subLayoutHint = subDialog != null ? subDialog.getLayoutHint() : null;
            applyLayout(panelWidget, subLayoutHint);

            if (subDialog != null) {
                String uiName = subDialog.getUiName();
                if (uiName == null || uiName.isEmpty()) {
                    uiName = subDialog.getKey();
                }
                panelWidget.setName(uiName);
                GradientTitleBorder.installBorder(uiName, panelWidget);
                fillPanel(panelWidget, subDialog, iniFileModel, ci);
            } else {
                panelWidget.setName(panel.getPanelName());
                GradientTitleBorder.installBorder(panel.getPanelName(), panelWidget);
            }
            targetContainer.add(panelWidget);
        }
    }

    /**
     * Re-evaluates all expression-controlled rows against the current workingImage.
     * Called whenever any field value changes.
     */
    private void refreshExpressions() {
        if (workingImage == null || currentIniFileModel == null) return;
        for (ExpressionRow exprRow : expressionRows) {
            applyExpressionState(exprRow, currentIniFileModel, workingImage);
        }
        contentPane.revalidate();
        contentPane.repaint();
    }

    private void applyExpressionState(ExpressionRow exprRow, IniFileModel iniFileModel, ConfigurationImage ci) {
        // Visibility
        if (exprRow.visibleExpression != null) {
            Boolean visible = evaluateFieldExpression(exprRow.visibleExpression, iniFileModel, ci);
            exprRow.row.setVisible(visible == null || visible);
        }
        // Enabled
        if (exprRow.enableExpression != null) {
            Boolean enabled = evaluateFieldExpression(exprRow.enableExpression, iniFileModel, ci);
            setComponentsEnabled(exprRow.row, enabled == null || enabled);
        }
    }

    private Boolean evaluateFieldExpression(String expression, IniFileModel iniFileModel, ConfigurationImage ci) {
        Set<String> varNames = ExpressionEvaluator.extractVariables(expression);
        Map<String, Double> context = new HashMap<>();
        for (String varName : varNames) {
            Optional<IniField> varField = iniFileModel.findIniField(varName);
            if (varField.isPresent()) {
                Double value = ci.readNumericValue(varField.get());
                if (value != null) {
                    context.put(varName, value);
                }
            }
        }
        return ExpressionEvaluator.evaluateBooleanExpression(expression, context);
    }

    private static void setComponentsEnabled(Container container, boolean enabled) {
        for (Component comp : container.getComponents()) {
            comp.setEnabled(enabled);
            if (comp instanceof Container) {
                setComponentsEnabled((Container) comp, enabled);
            }
        }
    }

    public JPanel getContentPane() {
        return contentPane;
    }
}
