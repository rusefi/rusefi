package com.rusefi.ui.livedocs.controls;

import javax.swing.*;
import java.awt.*;

/**
 * Swing UI utilities
 */
public class Toolbox {
    public static void setTransparentLineBorder(JComponent reference) {
        reference.setBorder(BorderFactory.createEmptyBorder(1, 1, 1, 1));
    }

    public static void setEnabledRecursive(Container panel, boolean value) {
        panel.setEnabled(value);
        Component[] com = panel.getComponents();
        for (int a = 0; a < com.length; a++) {
            Component element = com[a];
            if (element instanceof Container) {
                setEnabledRecursive((Container) element, value);
            } else {
                element.setEnabled(value);
            }
        }
    }
}
