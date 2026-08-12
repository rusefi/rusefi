package com.rusefi.ui.util;

import java.awt.*;

/**
 * A FlowLayout that wraps components to the next line when they exceed the container width,
 * and correctly reports preferred size based on the wrapped layout.
 */
public class WrapLayout extends FlowLayout {
    public WrapLayout(int align, int hgap, int vgap) {
        super(align, hgap, vgap);
    }

    @Override
    public Dimension preferredLayoutSize(Container target) {
        return computeSize(target, true);
    }

    @Override
    public Dimension minimumLayoutSize(Container target) {
        return computeSize(target, false);
    }

    private Dimension computeSize(Container target, boolean preferred) {
        synchronized (target.getTreeLock()) {
            int targetWidth = target.getWidth();
            if (targetWidth == 0) {
                targetWidth = Integer.MAX_VALUE;
            }

            Insets insets = target.getInsets();
            int maxWidth = targetWidth - insets.left - insets.right;
            int hgap = getHgap();
            int vgap = getVgap();

            int rowWidth = 0;
            int rowHeight = 0;
            int totalHeight = 0;
            int totalWidth = 0;

            for (int i = 0; i < target.getComponentCount(); i++) {
                Component c = target.getComponent(i);
                if (!c.isVisible()) continue;

                Dimension d = preferred ? c.getPreferredSize() : c.getMinimumSize();

                if (rowWidth > 0 && rowWidth + hgap + d.width > maxWidth) {
                    totalHeight += rowHeight + vgap;
                    totalWidth = Math.max(totalWidth, rowWidth);
                    rowWidth = 0;
                    rowHeight = 0;
                }

                if (rowWidth > 0) {
                    rowWidth += hgap;
                }
                rowWidth += d.width;
                rowHeight = Math.max(rowHeight, d.height);
            }

            totalHeight += rowHeight;
            totalWidth = Math.max(totalWidth, rowWidth);

            return new Dimension(
                totalWidth + insets.left + insets.right,
                totalHeight + insets.top + insets.bottom
            );
        }
    }
}
