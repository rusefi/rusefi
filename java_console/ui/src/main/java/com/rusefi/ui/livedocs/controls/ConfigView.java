package com.rusefi.ui.livedocs.controls;

import com.rusefi.ui.livedocs.LiveDocsSandbox;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class ConfigView {
    private final JLabel content;

    public ConfigView(ConfigReference e) {
        content = new JLabel(Integer.toString(e.value));
        markLiveElement(content);
        content.setToolTipText("Configuration XXX");

        content.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseEntered(MouseEvent e) {
                LiveDocsSandbox.comment.setText("Mouse action shows tooltip");
            }
        });

    }

    public static void markLiveElement(JComponent content) {
        content.setBackground(Color.lightGray);
        content.setOpaque(true);
    }

    public JLabel getContent() {
        return content;
    }
}
