package com.rusefi.ui;

import java.awt.*;
import java.awt.geom.Rectangle2D;
import java.util.HashMap;
import java.util.Map;

/**
 * Places each child at a rectangle given as fractions (0..1) of the container,
 * so a free gauge layout scales with the window. The constraint passed to
 * {@link Container#add(Component, Object)} is a {@link Rectangle2D.Double}.
 * Swing paints the child at index 0 on top.
 */
final class FreeGaugeLayout implements LayoutManager2 {
    private static final Dimension PREFERRED = new Dimension(600, 400);

    private final Map<Component, Rectangle2D.Double> bounds = new HashMap<>();

    /** Moves or resizes a child; the caller revalidates the container. */
    void setBounds(Component component, Rectangle2D.Double fraction) {
        if (bounds.containsKey(component)) {
            bounds.put(component, (Rectangle2D.Double) fraction.clone());
        }
    }

    Rectangle2D.Double getBounds(Component component) {
        Rectangle2D.Double fraction = bounds.get(component);
        return fraction == null ? null : (Rectangle2D.Double) fraction.clone();
    }

    @Override
    public void addLayoutComponent(Component component, Object constraints) {
        if (!(constraints instanceof Rectangle2D.Double)) {
            throw new IllegalArgumentException("Free gauge layout needs fractional Rectangle2D.Double bounds");
        }
        bounds.put(component, (Rectangle2D.Double) ((Rectangle2D.Double) constraints).clone());
    }

    @Override
    public void addLayoutComponent(String name, Component component) {
        throw new IllegalArgumentException("Free gauge layout needs fractional Rectangle2D.Double bounds");
    }

    @Override
    public void removeLayoutComponent(Component component) {
        bounds.remove(component);
    }

    @Override
    public void layoutContainer(Container parent) {
        Insets insets = parent.getInsets();
        int width = parent.getWidth() - insets.left - insets.right;
        int height = parent.getHeight() - insets.top - insets.bottom;
        for (Component child : parent.getComponents()) {
            Rectangle2D.Double fraction = bounds.get(child);
            if (fraction == null) {
                continue;
            }
            // Round both edges, not the size, so gauges that touch keep touching.
            int left = (int) Math.round(fraction.x * width);
            int top = (int) Math.round(fraction.y * height);
            int right = (int) Math.round((fraction.x + fraction.width) * width);
            int bottom = (int) Math.round((fraction.y + fraction.height) * height);
            child.setBounds(insets.left + left, insets.top + top, right - left, bottom - top);
        }
    }

    @Override
    public Dimension preferredLayoutSize(Container parent) {
        return new Dimension(PREFERRED);
    }

    @Override
    public Dimension minimumLayoutSize(Container parent) {
        return new Dimension(0, 0);
    }

    @Override
    public Dimension maximumLayoutSize(Container target) {
        return new Dimension(Integer.MAX_VALUE, Integer.MAX_VALUE);
    }

    @Override
    public float getLayoutAlignmentX(Container target) {
        return 0.5f;
    }

    @Override
    public float getLayoutAlignmentY(Container target) {
        return 0.5f;
    }

    @Override
    public void invalidateLayout(Container target) {
    }
}
