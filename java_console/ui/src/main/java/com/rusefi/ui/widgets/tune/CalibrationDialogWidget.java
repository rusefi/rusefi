package com.rusefi.ui.widgets.tune;

import com.opensr5.ConfigurationImage;
import com.opensr5.ConfigurationImageGetterSetter;
import com.opensr5.ini.CurveModel;
import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PanelModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;

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
            fillPanel(contentPane, dialogModel, iniFileModel, ci);
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

        for (PanelModel panel : dialogModel.getPanels()) {
            CurveModel curve = iniFileModel.getCurves().get(panel.getPanelName());
            if (curve != null) {
                CurveWidget curveWidget = new CurveWidget(curve, iniFileModel, ci);
                container.add(curveWidget.getContentPane());
                continue;
            }

            JPanel panelWidget = new JPanel();
            panelWidget.setLayout(new BoxLayout(panelWidget, BoxLayout.Y_AXIS));
            panelWidget.setBorder(BorderFactory.createTitledBorder(panel.getPanelName()));
            container.add(panelWidget);

            DialogModel subDialog = panel.resolveDialog(iniFileModel);
            if (subDialog != null) {
                fillPanel(panelWidget, subDialog, iniFileModel, ci);
            }
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
