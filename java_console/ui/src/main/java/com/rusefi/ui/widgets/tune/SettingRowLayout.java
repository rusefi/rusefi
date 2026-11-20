package com.rusefi.ui.widgets.tune;

import javax.swing.*;
import java.awt.*;

/**
 * Lays a setting row out on one line while it fits. In a narrower window the editor and what
 * follows it move to a second line, under the label, and the label shortens to the width there,
 * so no editor is squeezed or pushed out of view.
 * <p>
 * The component marked with {@link #WRAP_AFTER} (the label) ends the first line; the spacer
 * right after it is dropped from the wrapped layout.
 */
final class SettingRowLayout implements LayoutManager2 {
    static final String WRAP_AFTER = "SettingRowLayout.wrapAfter";
    private static final int LINE_GAP = 2;

    private final Container target;
    private final BoxLayout line;
    /** The height last reported to the parent, which sized this row from it. */
    private int reportedHeight = -1;

    SettingRowLayout(Container target) {
        this.target = target;
        line = new BoxLayout(target, BoxLayout.X_AXIS);
    }

    static void markWrapAfter(JComponent component) {
        component.putClientProperty(WRAP_AFTER, Boolean.TRUE);
    }

    private int splitIndex() {
        for (int i = 0; i < target.getComponentCount(); i++) {
            Component c = target.getComponent(i);
            if (c instanceof JComponent && Boolean.TRUE.equals(((JComponent) c).getClientProperty(WRAP_AFTER))) {
                return i;
            }
        }
        return -1;
    }

    private boolean wraps() {
        return target.getWidth() > 0 && splitIndex() >= 0
            && target.getWidth() < line.preferredLayoutSize(target).width;
    }

    @Override
    public Dimension preferredLayoutSize(Container parent) {
        Dimension size = wraps() ? twoLines(false) : line.preferredLayoutSize(parent);
        reportedHeight = size.height;
        return size;
    }

    @Override
    public Dimension minimumLayoutSize(Container parent) {
        Dimension size = line.minimumLayoutSize(parent);
        if (splitIndex() >= 0) {
            // A row can always wrap, so it never needs to be as wide as its one-line layout.
            size.width = Math.min(size.width, twoLines(false).width);
        }
        return size;
    }

    @Override
    public Dimension maximumLayoutSize(Container parent) {
        return line.maximumLayoutSize(parent);
    }

    @Override
    public void layoutContainer(Container parent) {
        boolean wrapped = wraps();
        int height;
        if (wrapped) {
            height = twoLines(true).height;
        } else {
            line.layoutContainer(parent);
            height = line.preferredLayoutSize(parent).height;
        }
        if (reportedHeight >= 0 && height != reportedHeight) {
            // The parent sized this row before it had this width; it must ask again.
            reportedHeight = height;
            SwingUtilities.invokeLater(parent::revalidate);
        }
    }

    /** The two-line layout at the row's current width; positions the components when {@code apply}. */
    private Dimension twoLines(boolean apply) {
        Insets insets = target.getInsets();
        int split = splitIndex();
        int right = target.getWidth() - insets.right;
        int firstHeight = 0;
        int secondHeight = 0;
        for (int i = 0; i < target.getComponentCount(); i++) {
            Component c = target.getComponent(i);
            if (c.isVisible()) {
                if (i <= split) {
                    firstHeight = Math.max(firstHeight, c.getPreferredSize().height);
                } else {
                    secondHeight = Math.max(secondHeight, c.getPreferredSize().height);
                }
            }
        }
        int x = insets.left;
        int labelX = insets.left;
        int firstWidth = 0;
        for (int i = 0; i <= split; i++) {
            Component c = target.getComponent(i);
            Dimension d = c.getPreferredSize();
            if (!c.isVisible()) {
                place(apply, c, 0, 0, 0, 0);
                continue;
            }
            int width = d.width;
            if (i == split) {
                labelX = x;
                width = Math.max(0, Math.min(d.width, right - x));
            }
            place(apply, c, x, insets.top + (firstHeight - d.height) / 2, width, d.height);
            x += width;
            firstWidth = x - insets.left + (i == split ? d.width - width : 0);
        }
        x = labelX;
        int top = insets.top + firstHeight + LINE_GAP;
        for (int i = split + 1; i < target.getComponentCount(); i++) {
            Component c = target.getComponent(i);
            Dimension d = c.getPreferredSize();
            if (!c.isVisible() || i == split + 1 && c instanceof Box.Filler) {
                place(apply, c, x, top, 0, 0);
                continue;
            }
            int width = Math.max(0, Math.min(d.width, right - x));
            place(apply, c, x, top + (secondHeight - d.height) / 2, width, d.height);
            x += d.width;
        }
        int secondWidth = x - insets.left;
        return new Dimension(Math.max(firstWidth, secondWidth) + insets.left + insets.right,
            top + secondHeight + insets.bottom);
    }

    private static void place(boolean apply, Component c, int x, int y, int width, int height) {
        if (apply) {
            c.setBounds(x, y, width, height);
        }
    }

    @Override
    public void addLayoutComponent(Component comp, Object constraints) {
        line.addLayoutComponent(comp, constraints);
    }

    @Override
    public void addLayoutComponent(String name, Component comp) {
        line.addLayoutComponent(name, comp);
    }

    @Override
    public void removeLayoutComponent(Component comp) {
        line.removeLayoutComponent(comp);
    }

    @Override
    public float getLayoutAlignmentX(Container parent) {
        return line.getLayoutAlignmentX(parent);
    }

    @Override
    public float getLayoutAlignmentY(Container parent) {
        return line.getLayoutAlignmentY(parent);
    }

    @Override
    public void invalidateLayout(Container parent) {
        line.invalidateLayout(parent);
    }
}
