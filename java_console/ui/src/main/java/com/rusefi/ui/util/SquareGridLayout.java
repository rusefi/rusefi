package com.rusefi.ui.util;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.LayoutManager2;

/**
 * A grid of square cells, such as round gauges, that scales with the width it is given:
 * the preferred width is fixed at {@code preferredCell}, while the preferred height follows the
 * cell size the current width allows, between {@code minimumCell} and {@code maximumCell}.
 * <p>
 * Because the preferred height depends on the current width, the owner must revalidate after a
 * width change (see {@link #revalidateOnWidthChange}). The preferred width never depends on the
 * current width, so this cannot feed back into an ever-growing layout.
 */
public class SquareGridLayout implements LayoutManager2 {
    private final int columns;
    private final int gap;
    private final int minimumCell;
    private final int preferredCell;
    private final int maximumCell;

    public SquareGridLayout(int columns, int gap, int minimumCell, int preferredCell, int maximumCell) {
        this.columns = Math.max(1, columns);
        this.gap = Math.max(0, gap);
        this.minimumCell = minimumCell;
        this.preferredCell = Math.max(minimumCell, preferredCell);
        this.maximumCell = Math.max(this.preferredCell, maximumCell);
    }

    /** Revalidates {@code parent} whenever its width changes, so ancestors pick up the new height. */
    public static void revalidateOnWidthChange(Container parent) {
        parent.addComponentListener(new java.awt.event.ComponentAdapter() {
            private int lastWidth = -1;

            @Override
            public void componentResized(java.awt.event.ComponentEvent e) {
                int width = parent.getWidth();
                if (width != lastWidth) {
                    lastWidth = width;
                    javax.swing.SwingUtilities.invokeLater(() -> {
                        parent.invalidate();
                        parent.revalidate();
                    });
                }
            }
        });
    }

    private static int visibleCount(Container parent) {
        int count = 0;
        for (Component component : parent.getComponents()) {
            if (component.isVisible()) {
                count++;
            }
        }
        return count;
    }

    private int effectiveColumns(int count) {
        return Math.max(1, Math.min(columns, count));
    }

    private static int rows(int count, int cols) {
        return (count + cols - 1) / cols;
    }

    private Dimension size(Container parent, int cell) {
        Insets insets = parent.getInsets();
        int count = visibleCount(parent);
        if (count == 0) {
            return new Dimension(insets.left + insets.right, insets.top + insets.bottom);
        }
        int cols = effectiveColumns(count);
        int rows = rows(count, cols);
        return new Dimension(insets.left + insets.right + cols * cell + (cols - 1) * gap,
            insets.top + insets.bottom + rows * cell + (rows - 1) * gap);
    }

    /** The cell size the parent's current width allows, before the height limit. */
    int cellForWidth(Container parent) {
        int width = parent.getWidth();
        if (width <= 0) {
            return preferredCell;
        }
        Insets insets = parent.getInsets();
        int cols = effectiveColumns(visibleCount(parent));
        int cell = (width - insets.left - insets.right - (cols - 1) * gap) / cols;
        return Math.max(minimumCell, Math.min(maximumCell, cell));
    }

    @Override
    public Dimension preferredLayoutSize(Container parent) {
        synchronized (parent.getTreeLock()) {
            Dimension preferred = size(parent, preferredCell);
            preferred.height = size(parent, cellForWidth(parent)).height;
            return preferred;
        }
    }

    @Override
    public Dimension minimumLayoutSize(Container parent) {
        synchronized (parent.getTreeLock()) {
            return size(parent, minimumCell);
        }
    }

    @Override
    public Dimension maximumLayoutSize(Container parent) {
        synchronized (parent.getTreeLock()) {
            return size(parent, maximumCell);
        }
    }

    @Override
    public void layoutContainer(Container parent) {
        synchronized (parent.getTreeLock()) {
            Insets insets = parent.getInsets();
            int count = visibleCount(parent);
            if (count == 0) {
                return;
            }
            int cols = effectiveColumns(count);
            int rows = rows(count, cols);
            int heightCell = (parent.getHeight() - insets.top - insets.bottom - (rows - 1) * gap) / rows;
            int cell = Math.max(0, Math.min(cellForWidth(parent), heightCell));
            int index = 0;
            for (Component component : parent.getComponents()) {
                if (!component.isVisible()) {
                    continue;
                }
                int column = index % cols;
                int row = index / cols;
                component.setBounds(insets.left + column * (cell + gap), insets.top + row * (cell + gap), cell, cell);
                index++;
            }
        }
    }

    @Override
    public void addLayoutComponent(String name, Component comp) {
    }

    @Override
    public void addLayoutComponent(Component comp, Object constraints) {
    }

    @Override
    public void removeLayoutComponent(Component comp) {
    }

    @Override
    public float getLayoutAlignmentX(Container target) {
        return 0;
    }

    @Override
    public float getLayoutAlignmentY(Container target) {
        return 0;
    }

    @Override
    public void invalidateLayout(Container target) {
    }
}
