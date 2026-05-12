package com.rusefi.ui.util;

import java.awt.*;

public class SwingUtil {
    /**
     * Recursively invalidates a component and all its descendants,
     * clearing cached preferred sizes so they are recomputed on the next layout pass.
     */
    public static void invalidateTree(Component c) {
        c.invalidate();
        if (c instanceof Container) {
            for (Component child : ((Container) c).getComponents()) {
                invalidateTree(child);
            }
        }
    }
}
