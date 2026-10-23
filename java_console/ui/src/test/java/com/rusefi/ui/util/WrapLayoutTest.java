package com.rusefi.ui.util;

import org.junit.jupiter.api.Test;

import javax.swing.JPanel;
import java.awt.Dimension;
import java.awt.FlowLayout;

import static org.junit.jupiter.api.Assertions.assertEquals;

class WrapLayoutTest {
    private static JPanel box(int width, int height) {
        JPanel box = new JPanel();
        box.setPreferredSize(new Dimension(width, height));
        return box;
    }

    @Test
    void preferredSizeIncludesTheGapsAroundTheRows() {
        JPanel panel = new JPanel(new WrapLayout(FlowLayout.LEFT, 6, 4));
        panel.add(box(100, 36));
        panel.add(box(50, 30));
        // FlowLayout puts vgap above the first row and below the last, hgap left and right
        assertEquals(new Dimension(6 + 100 + 6 + 50 + 6, 4 + 36 + 4), panel.getPreferredSize());
    }

    @Test
    void wrapsWhereFlowLayoutDoesAndCountsTheGapBetweenRows() {
        JPanel panel = new JPanel(new WrapLayout(FlowLayout.LEFT, 6, 4));
        panel.add(box(100, 36));
        panel.add(box(50, 30));
        // exactly room for both on one row
        panel.setSize(6 + 100 + 6 + 50 + 6, 100);
        assertEquals(4 + 36 + 4, panel.getPreferredSize().height);
        panel.doLayout();
        assertEquals(panel.getComponent(0).getY(), panel.getComponent(1).getY() - 3, "one row, centred vertically");

        // too narrow for the second box even without the gap before it, so FlowLayout wraps too
        panel.setSize(6 + 100 + 50 + 6 - 1, 100);
        assertEquals(4 + 36 + 4 + 30 + 4, panel.getPreferredSize().height);
        panel.doLayout();
        assertEquals(4 + 36 + 4, panel.getComponent(1).getY(), "the second box starts the second row");
        int bottom = panel.getComponent(1).getY() + panel.getComponent(1).getHeight();
        assertEquals(panel.getPreferredSize().height - 4, bottom, "nothing is cut off at the bottom");
    }
}
