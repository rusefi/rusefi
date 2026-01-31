package com.rusefi.ui.widgets;

import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PanelModel;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;

import javax.swing.*;
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
                JComboBox<String> comboBox = new JComboBox<>(enumField.getEnums().values().toArray(new String[0]));
                comboBox.setMaximumSize(comboBox.getPreferredSize());
                row.add(comboBox);
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

    public JPanel getContentPane() {
        return contentPane;
    }
}
