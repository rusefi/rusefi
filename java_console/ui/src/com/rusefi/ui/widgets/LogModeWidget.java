package com.rusefi.ui.widgets;

import com.rusefi.io.CommandQueue;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Date: 3/29/13
 * (c) Andrey Belomutskiy
 */
public class LogModeWidget {
    private final JPanel panel = new JPanel(new FlowLayout(FlowLayout.LEFT, 0, 0));

    private final JCheckBox mode = new JCheckBox("full logging");

    public LogModeWidget() {
        panel.setBorder(BorderFactory.createLineBorder(Color.black));
        panel.add(mode);

        mode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int code = mode.isSelected() ? 1 : 0;
                CommandQueue.getInstance().write("fl " + code);
            }
        });
    }

    public JPanel getPanel() {
        return panel;
    }
}
