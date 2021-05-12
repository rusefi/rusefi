package com.rusefi.ui;

import javax.swing.*;
import java.awt.*;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 2/9/2016
 */
public class GaugesGrid {
    public final JPanel panel;
    public GaugesGrid(int rows, int cols) {
        panel = new JPanel(new GridLayout(rows, cols));
    }

    public void setLayout(int rows, int columns) {
        panel.setLayout(new GridLayout(rows, columns));
        panel.removeAll();
    }
}
