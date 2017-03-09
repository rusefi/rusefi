package com.rusefi.ui.fsio;

import com.fathzer.soft.javaluator.DoubleEvaluator;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * (c) Andrey Belomutskiy
 * 10/10/14
 */
public class FlexibleControls {
    private final JPanel panel = new JPanel(new GridLayout(5, 1));

    private final JTextField normalForm = new JTextField();

    private final JTextField rpnForm = new JTextField();

    public FlexibleControls() {

        JButton human2rpm = new JButton("Convert");
        human2rpm.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                process(FlexibleControls.this.normalForm.getText());
            }
        });

        panel.add(new JLabel("Human-readable"));
        panel.add(normalForm);
        panel.add(human2rpm);
        panel.add(new JLabel("RPN form"));
        panel.add(rpnForm);

        normalForm.setText("(time_since_boot < 4) | (rpm > 0)");

        process(normalForm.getText());
    }

    private void process(String text) {
        rpnForm.setText(DoubleEvaluator.process(text).getPosftfixExpression());
    }

    public JPanel getPanel() {
        return panel;
    }
}
