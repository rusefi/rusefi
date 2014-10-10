package com.rusefi.ui;

import com.autsia.bracer.BracerParser;

import javax.swing.*;
import java.awt.*;
import java.text.ParseException;

/**
 * (c) Andrey Belomutskiy
 * 10/10/14
 */
public class FlexibleControls {
    private final JPanel panel = new JPanel(new BorderLayout());

    private final JTextField normalForm = new JTextField();

    private final JTextField rpnForm = new JTextField();

    public FlexibleControls() {

        panel.add(normalForm, BorderLayout.NORTH);
        panel.add(rpnForm, BorderLayout.SOUTH);

        normalForm.setText("(time_since_boot < 4) | (rpm > 0)");

        process();

    }

    private void process() {
        BracerParser bp = new BracerParser();
        try {
            bp.parse(normalForm.getText());
        } catch (ParseException e) {
           throw new IllegalStateException(e);
        }
        rpnForm.setText(bp.getStackRPN().toString());
    }

    public JPanel getPanel() {
        return panel;
    }


}
