package com.rusefi.ui.etb;

import org.putgemin.VerticalFlowLayout;

import javax.swing.*;

public class CalibrationPanel {
    private final JPanel content = new JPanel(new VerticalFlowLayout());

    public CalibrationPanel() {
        content.setBorder(BorderFactory.createTitledBorder("Calibration"));
        content.add(new JButton("Grab TPS#1 fully closed"));
        content.add(new JButton("Grab TPS#1 Wide Open"));

        content.add(new JButton("Grab Pedal Up"));
        content.add(new JButton("Grab Pedal Down"));
    }

    public JPanel getContent() {
        return content;
    }
}
