package com.rusefi.ui;

import com.rusefi.ui.storage.Node;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

public class LogSizeControl {
    private static final String BYTES = "SCREEN_LOG_BYTES";

    private final JPanel content = new JPanel(new FlowLayout());

    public LogSizeControl(Node config) {
        content.add(new JLabel("On screen bytes: "));
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
                config.setProperty(BYTES, Integer.toString(value));
            }
        });
    }

    public static int getValue(Node config) {
        return config.getIntProperty(BYTES, 50000);
    }

    public Component getContent() {
        return content;
    }
}
