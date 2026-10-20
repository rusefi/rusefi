package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.widgets.CommandBox;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;

public class LogSizeControl {
    private static final String BYTES = "SCREEN_LOG_BYTES";

    private final JPanel content = new JPanel(new FlowLayout(FlowLayout.LEFT, 8, 0));

    public LogSizeControl(Node config) {
        JLabel caption = new JLabel("On-screen log buffer");
        content.add(caption);
        JTextField lines = new JTextField(8);
        caption.setLabelFor(lines);
        lines.setToolTipText("Bytes of console messages kept on screen; press Enter to apply");
        // same rounded frame as the console command box, with the unit in its shaded end
        content.add(CommandBox.withSuffix(lines, "bytes"));
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
