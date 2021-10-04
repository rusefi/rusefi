package com.rusefi.ui;

import com.rusefi.ui.storage.Node;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

public class LogSizeControl {
    private static final String LINES = "SCREEN_LOG_LINES";

    private final JPanel content = new JPanel(new FlowLayout());

    public LogSizeControl(Node config) {
        content.add(new JLabel("On screen lines: "));
        JTextField lines = new JTextField(8);
        content.add(lines);
        lines.setText(Integer.toString(getValue(config)));
        lines.addActionListener(new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent ev) {
                int value;
                try {
                    value = Integer.parseInt(lines.getText());
                } catch (NumberFormatException ex) {
                    return;
                }
                config.setProperty(LINES, Integer.toString(value));
            }
        });
    }

    public static int getValue(Node config) {
        return config.getIntProperty(LINES, 1000);
    }

    public Component getContent() {
        return content;
    }
}
