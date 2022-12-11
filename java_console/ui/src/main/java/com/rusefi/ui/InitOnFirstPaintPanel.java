package com.rusefi.ui;

import com.rusefi.ui.util.UiUtils;

import javax.swing.*;
import java.awt.*;

public abstract class InitOnFirstPaintPanel {
    private final JPanel content = new JPanel(new BorderLayout()) {
        boolean isFirstPaint = true;

        @Override
        public void paint(Graphics g) {
            if (isFirstPaint) {
                content.removeAll();
                content.add(createContent(), BorderLayout.CENTER);
                UiUtils.trueRepaint(content);
                isFirstPaint = false;
            }
            super.paint(g);
        }
    };

    protected abstract JPanel createContent();

    public InitOnFirstPaintPanel() {
        content.add(new JLabel("Initializing..."), BorderLayout.CENTER);
    }

    public JComponent getContent() {
        return content;
    }
}
