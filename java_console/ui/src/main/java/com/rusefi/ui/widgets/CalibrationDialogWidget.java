package com.rusefi.ui.widgets;

import com.opensr5.ini.DialogModel;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.PanelModel;

import javax.swing.*;

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
            container.add(new JLabel(field.getUiName()));
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
