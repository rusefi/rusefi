package com.rusefi.ui.livedocs;

import javax.swing.*;
import java.awt.*;

/**
 * (c) Andrey Belomutskiy 2013-2019
 */
public class LessJumpyJLabel extends JLabel {
    private int maxPreferredWidth = 0;

    public LessJumpyJLabel(String text) {
        super(text);
    }

    @Override
    public Dimension getPreferredSize() {
        Dimension preferredSize = super.getPreferredSize();
        // let's occupy maximum width we ever wanted to reduce layout jumpiness
        maxPreferredWidth = Math.max(maxPreferredWidth, preferredSize.width);
        return new Dimension(maxPreferredWidth, preferredSize.height);
    }
}
