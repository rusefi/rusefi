package com.rusefi.ui.livedocs.controls;

import com.rusefi.ui.livedocs.LiveDocsSandbox;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import static com.rusefi.ui.livedocs.controls.ConfigView.markLiveElement;

public class LogicReference {
    private final JLabel content = new JLabel();

    public LogicReference(double value, JComponent reference) {
        content.setText(Double.toString(value));
        markLiveElement(content);
        content.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseEntered(MouseEvent e) {
                reference.setBorder(BorderFactory.createLineBorder(Color.red));
                LiveDocsSandbox.comment.setText("Mouse action highlights referenced element");
            }

            @Override
            public void mouseExited(MouseEvent e) {
                Toolbox.setTransparentLineBorder(reference);
            }
        });
    }

    public JLabel getContent() {
        return content;
    }
}
