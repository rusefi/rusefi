package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ini.CurveModel;
import com.opensr5.ini.DialogModel;
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
import java.util.List;
import java.util.Optional;

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
        for (DialogModel.Field field : dialogModel.getFields()) {
            Optional<IniField> iniField = iniFileModel.findIniField(field.getKey());
            if (iniField.isPresent()) {
                container.add(CalibrationFieldFactory.createFieldRow(field, iniField.get(), ci, workingImage));
            } else {
                container.add(CalibrationFieldFactory.createLabelRow(field));
            }
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

    public JPanel getContentPane() {
        return contentPane;
    }
}
