package com.rusefi.ui.widgets;

import com.opensr5.ini.DialogModel;
import com.opensr5.ini.PanelModel;

import javax.swing.*;

public class CalibrationDialogWidget {
    private final JPanel contentPane = new JPanel();

    public CalibrationDialogWidget() {
        contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
    }

    public void update(DialogModel dialogModel) {
        contentPane.removeAll();
        if (dialogModel != null) {
            for (DialogModel.Field field : dialogModel.getFields()) {
                contentPane.add(new JLabel(field.getUiName()));
            }

            for (PanelModel panel : dialogModel.getPanels()) {
                JPanel panelWidget = new JPanel();
                panelWidget.setBorder(BorderFactory.createTitledBorder(panel.getPanelName()));
                contentPane.add(panelWidget);
            }
        }
        contentPane.revalidate();
        contentPane.repaint();
    }

    public JPanel getContentPane() {
        return contentPane;
    }
}
