package com.rusefi.ui.util;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.LayoutManager2;

/**
 * A grid of square cells, such as round gauges, that fills the width it is given: as many cells
 * per row as fit at {@code preferredCell}, rows balanced so the last one is not left nearly
 * empty, then the width shared equally between the columns. There is no upper cell size, so the
 * cells keep growing with the window.
 * <p>
 * The preferred width is fixed at {@code preferredColumns} cells of {@code preferredCell}, while
 * the preferred height follows the cell size the current width allows.
 * <p>
 * Because the preferred height depends on the current width, the owner must revalidate after a
 * width change (see {@link #revalidateOnWidthChange}). The preferred width never depends on the
 * current width, so this cannot feed back into an ever-growing layout.
 */
public class SquareGridLayout implements LayoutManager2 {
    private final int preferredColumns;
    private final int gap;
    private final int minimumCell;
    private final int preferredCell;

    public SquareGridLayout(int preferredColumns, int gap, int minimumCell, int preferredCell) {
        this.preferredColumns = Math.max(1, preferredColumns);
        this.gap = Math.max(0, gap);
        this.minimumCell = Math.max(1, minimumCell);
        this.preferredCell = Math.max(this.minimumCell, preferredCell);
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

    private int preferredColumns(int count) {
        return Math.max(1, Math.min(preferredColumns, count));
    }

    private int innerWidth(Container parent) {
        Insets insets = parent.getInsets();
        return parent.getWidth() - insets.left - insets.right;
    }

    /** As many columns as fit at the preferred cell size, balanced over the resulting rows. */
    int columnsForWidth(Container parent) {
        int count = visibleCount(parent);
        if (count == 0) {
            return 1;
        }
        if (parent.getWidth() <= 0) {
            return preferredColumns(count);
        }
        int fit = (innerWidth(parent) + gap) / (preferredCell + gap);
        int cols = Math.max(1, Math.min(count, fit));
        return (count + rows(count, cols) - 1) / rows(count, cols);
    }

    private static int rows(int count, int cols) {
        return (count + cols - 1) / cols;
    }

    private Dimension size(Container parent, int cols, int cell) {
        Insets insets = parent.getInsets();
        int count = visibleCount(parent);
        if (count == 0) {
            return new Dimension(insets.left + insets.right, insets.top + insets.bottom);
        }
        int rows = rows(count, cols);
        return new Dimension(insets.left + insets.right + cols * cell + (cols - 1) * gap,
            insets.top + insets.bottom + rows * cell + (rows - 1) * gap);
    }

    /** The cell size the parent's current width allows, before the height limit. */
    int cellForWidth(Container parent, int cols) {
        if (parent.getWidth() <= 0) {
            return preferredCell;
        }
        return Math.max(minimumCell, (innerWidth(parent) - (cols - 1) * gap) / cols);
    }

    @Override
    public Dimension preferredLayoutSize(Container parent) {
        synchronized (parent.getTreeLock()) {
            int count = visibleCount(parent);
            Dimension preferred = size(parent, preferredColumns(count), preferredCell);
            int cols = columnsForWidth(parent);
            preferred.height = size(parent, cols, cellForWidth(parent, cols)).height;
            return preferred;
        }
    }

    @Override
    public Dimension minimumLayoutSize(Container parent) {
        synchronized (parent.getTreeLock()) {
            return size(parent, preferredColumns(visibleCount(parent)), minimumCell);
        }
    }

    @Override
    public Dimension maximumLayoutSize(Container parent) {
        return new Dimension(Short.MAX_VALUE, Short.MAX_VALUE);
    }

    @Override
    public void layoutContainer(Container parent) {
        synchronized (parent.getTreeLock()) {
            Insets insets = parent.getInsets();
            int count = visibleCount(parent);
            if (count == 0) {
                return;
            }
            int cols = columnsForWidth(parent);
            int rows = rows(count, cols);
            int heightCell = (parent.getHeight() - insets.top - insets.bottom - (rows - 1) * gap) / rows;
            int cell = Math.max(0, Math.min(cellForWidth(parent, cols), heightCell));
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
