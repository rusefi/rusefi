package com.rusefi.ui.widgets.tune;

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

    public void update(DialogModel dialogModel, IniFileModel iniFileModel) {
        contentPane.removeAll();
        if (dialogModel != null) {
            fillPanel(contentPane, dialogModel, iniFileModel);
        }
        contentPane.revalidate();
        contentPane.repaint();
    }

    private void fillPanel(JPanel container, DialogModel dialogModel, IniFileModel iniFileModel) {
        for (DialogModel.Field field : dialogModel.getFields()) {
            Optional<IniField> iniField = iniFileModel.findIniField(field.getKey());
            if (iniField.isPresent() && iniField.get() instanceof EnumIniField) {
                EnumIniField enumField = (EnumIniField) iniField.get();
                JPanel row = new JPanel();
                row.setLayout(new BoxLayout(row, BoxLayout.X_AXIS));
                row.add(new JLabel(field.getUiName()));

                boolean isCheckBox = isCheckboxEnum(enumField);

                if (isCheckBox) {
                    JCheckBox checkBox = new JCheckBox();
                    row.add(checkBox);
                } else {
                    JComboBox<String> comboBox = new JComboBox<>(enumField.getEnums().values().toArray(new String[0]));
                    comboBox.setMaximumSize(comboBox.getPreferredSize());
                    row.add(comboBox);
                }
                container.add(row);
            } else {
                container.add(new JLabel(field.getUiName()));
            }
        }

        for (PanelModel panel : dialogModel.getPanels()) {
            JPanel panelWidget = new JPanel();
            panelWidget.setLayout(new BoxLayout(panelWidget, BoxLayout.Y_AXIS));
            panelWidget.setBorder(BorderFactory.createTitledBorder(panel.getPanelName()));
            container.add(panelWidget);

            DialogModel subDialog = panel.resolveDialog(iniFileModel);
            if (subDialog != null) {
                fillPanel(panelWidget, subDialog, iniFileModel);
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
