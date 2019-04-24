package com.rusefi.ui.etb;

import org.jetbrains.annotations.NotNull;
import org.putgemin.VerticalFlowLayout;

import javax.swing.*;

public class CalibrationPanel {
    private final JPanel content = new JPanel(new VerticalFlowLayout());

    public CalibrationPanel() {
        content.setBorder(BorderFactory.createTitledBorder("Calibration"));
        content.add(createCommandButton("Grab TPS#1 fully closed"));
        content.add(createCommandButton("Grab TPS#1 Wide Open"));

        content.add(createCommandButton("Grab Pedal Up"));
        content.add(createCommandButton("Grab Pedal Down"));
    }

    @NotNull
    private JButton createCommandButton(String s) {
        JButton button = new JButton(s);
        button.setEnabled(false);
        return button;
    }

    public JPanel getContent() {
        return content;
    }
}
