package com.rusefi.ui.widgets;

import javax.swing.*;
import java.awt.*;

/**
 * 7/18/13
 * (c) Andrey Belomutskiy
 */
public class JTextFieldWithWidth extends JTextField {
    private int width;

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
