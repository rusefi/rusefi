package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.CurveModel;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PanelModel;
import com.opensr5.ini.TableModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.ui.laf.GradientTitleBorder;

import javax.swing.*;
import java.util.List;
import java.util.Optional;

public class CalibrationDialogWidget {
    private final JPanel contentPane = new JPanel();

    public CalibrationDialogWidget() {
        contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
    }

    public void update(DialogModel dialogModel, IniFileModel iniFileModel, ConfigurationImage ci) {
        contentPane.removeAll();
        if (dialogModel != null) {
            String layoutHint = dialogModel.getLayoutHint();
            if ("xAxis".equalsIgnoreCase(layoutHint)) {
                contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.X_AXIS));
            } else {
                contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
            }
            fillPanel(contentPane, dialogModel, iniFileModel, ci);
        }
        contentPane.revalidate();
        contentPane.repaint();
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
                IniField f = iniField.get();
                JPanel row = new JPanel();
                row.setLayout(new BoxLayout(row, BoxLayout.X_AXIS));
                row.add(new JLabel(field.getUiName()));

                if (f instanceof EnumIniField) {
                    EnumIniField enumField = (EnumIniField) f;

                    boolean isCheckBox = isCheckboxEnum(enumField);

                    String currentValue = ci == null ? "" : ConfigurationImageGetterSetter.getStringValue(f, ci);

                    if (isCheckBox) {
                        JCheckBox checkBox = new JCheckBox();
                        checkBox.setSelected(currentValue.equalsIgnoreCase("\"Enabled\"") || currentValue.equalsIgnoreCase("\"Yes\""));
                        row.add(checkBox);
                    } else {
                        JComboBox<String> comboBox = new JComboBox<>(enumField.getEnums().values().toArray(new String[0]));
                        comboBox.setSelectedItem(currentValue.replace("\"", ""));
                        comboBox.setMaximumSize(comboBox.getPreferredSize());
                        row.add(comboBox);
                    }
                } else {
                    String currentValue = ci == null ? "" : ConfigurationImageGetterSetter.getStringValue(f, ci);
                    JTextField textField = new JTextField(currentValue);
                    textField.setMaximumSize(textField.getPreferredSize());
                    row.add(textField);
                }
                container.add(row);
            } else {
                container.add(new JLabel(field.getUiName()));
            }
        }

        for (DialogModel.Command command : dialogModel.getCommandsOfCurrentDialog()) {
            JButton button = new JButton(command.getUiName());
            button.addActionListener(e -> {
                // TODO: implement command execution
                System.out.println("Executing command: " + command.getCommand());
            });
            container.add(button);
        }

        List<PanelModel> panels = dialogModel.getPanels();
        JPanel horizontalPanel = null;
        for (PanelModel panel : panels) {
            String placement = panel.getPlacement();
            boolean isHorizontal = "west".equalsIgnoreCase(placement) || "center".equalsIgnoreCase(placement) || "east".equalsIgnoreCase(placement);

            if (isHorizontal) {
                if (horizontalPanel == null) {
                    horizontalPanel = new JPanel();
                    horizontalPanel.setLayout(new BoxLayout(horizontalPanel, BoxLayout.X_AXIS));
                    container.add(horizontalPanel);
                }
            } else {
                horizontalPanel = null;
            }

            JPanel targetContainer = isHorizontal ? horizontalPanel : container;

            CurveModel curve = iniFileModel.getCurves().get(panel.getPanelName());
            if (curve != null) {
                CurveWidget curveWidget = new CurveWidget(curve, iniFileModel, ci);
                targetContainer.add(curveWidget.getContentPane());
                continue;
            }

            TableModel table = iniFileModel.getTable(panel.getPanelName());
            if (table != null) {
                TuningTableView tuningTableView = new TuningTableView(table.getTitle());
                tuningTableView.displayTable(iniFileModel, table.getTableId(), ci);
                targetContainer.add(tuningTableView.getContent());
                continue;
            }

            JPanel panelWidget = new JPanel();
            DialogModel subDialog = panel.resolveDialog(iniFileModel);
            String subLayoutHint = subDialog != null ? subDialog.getLayoutHint() : null;
            if ("xAxis".equalsIgnoreCase(subLayoutHint)) {
                panelWidget.setLayout(new BoxLayout(panelWidget, BoxLayout.X_AXIS));
            } else {
                panelWidget.setLayout(new BoxLayout(panelWidget, BoxLayout.Y_AXIS));
            }

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

    private static boolean isCheckboxEnum(EnumIniField enumField) {
        boolean isCheckBox = false;
        if (enumField.getEnums().size() == 2) {
            List<String> values = new java.util.ArrayList<>(enumField.getEnums().values());
            String v1 = values.get(0).toLowerCase();
            String v2 = values.get(1).toLowerCase();

            if ((v1.equals("yes") && v2.equals("no")) || (v1.equals("no") && v2.equals("yes")) ||
                    (v1.equals("enabled") && v2.equals("disabled")) || (v1.equals("disabled") && v2.equals("enabled"))) {
                isCheckBox = true;
            }
        }
        return isCheckBox;
    }

    public JPanel getContentPane() {
        return contentPane;
    }
}
