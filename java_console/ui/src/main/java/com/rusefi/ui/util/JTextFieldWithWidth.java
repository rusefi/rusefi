package com.rusefi.ui.util;

import javax.swing.*;
import java.awt.*;

/**
 * 7/18/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class JTextFieldWithWidth extends JTextField {
    private final int width;

    public JTextFieldWithWidth(int width) {
        this("", width);
    }

    public JTextFieldWithWidth() {
        this("", 200);
    }

    public JTextFieldWithWidth(String text) {
        this(text, 200);
    }

    public JTextFieldWithWidth(String text, int width) {
        super(text);
        this.width = width;
    }

    @Override
    public Dimension getPreferredSize() {
        Dimension size = super.getPreferredSize();
        return new Dimension(width, size.height);
    }
}
