package com.rusefi.ui.util;

import javax.swing.*;
import java.awt.*;

/**
 * A JPanel that implements Scrollable to track viewport width,
 * preventing horizontal scrollbars while allowing vertical scrolling.
 *
 * On width changes, schedules a revalidation so that WrapLayout containers
 * recalculate their preferred heights based on the actual available width.
 */
public class ScrollablePanel extends JPanel implements Scrollable {
    private int lastWidth = -1;

    @Override
    public void setBounds(int x, int y, int width, int height) {
        boolean widthChanged = width != lastWidth;
        lastWidth = width;
        super.setBounds(x, y, width, height);
        if (widthChanged && width > 0) {
            SwingUtilities.invokeLater(this::revalidate);
        }
    }

    @Override
    public Dimension getPreferredScrollableViewportSize() {
        return getPreferredSize();
    }

    @Override
    public int getScrollableUnitIncrement(Rectangle visibleRect, int orientation, int direction) {
        return 16;
    }

    @Override
    public int getScrollableBlockIncrement(Rectangle visibleRect, int orientation, int direction) {
        return orientation == SwingConstants.VERTICAL ? visibleRect.height : visibleRect.width;
    }

    @Override
    public boolean getScrollableTracksViewportWidth() {
        return true;
    }

    @Override
    public boolean getScrollableTracksViewportHeight() {
        return false;
    }
}
