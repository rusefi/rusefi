package com.rusefi.ui.util;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Insets;
import java.util.ArrayList;
import java.util.List;

/**
 * A {@link BorderLayout} that shrinks its horizontal regions when their preferred widths do not
 * fit. Standard BorderLayout always gives West and East their preferred widths, which can make
 * them overlap in a viewport that tracks its parent's width.
 */
public class ConstrainedBorderLayout extends BorderLayout {
    public ConstrainedBorderLayout(int hgap, int vgap) {
        super(hgap, vgap);
    }

    @Override
    public void layoutContainer(Container target) {
        synchronized (target.getTreeLock()) {
            super.layoutContainer(target);

            List<Component> horizontal = getVisibleHorizontalComponents();
            if (horizontal.size() < 2) {
                return;
            }

            Insets insets = target.getInsets();
            int totalWidth = Math.max(0, target.getWidth() - insets.left - insets.right);
            int availableWidth = Math.max(0, totalWidth - getHgap() * (horizontal.size() - 1));
            long preferredWidth = horizontal.stream()
                .map(Component::getPreferredSize)
                .mapToLong(size -> Math.max(0, size.width))
                .sum();
            if (preferredWidth <= availableWidth) {
                return;
            }

            int[] widths = fitWidths(horizontal, availableWidth);
            Component reference = horizontal.get(0);
            int x = insets.left;
            for (int i = 0; i < horizontal.size(); i++) {
                Component component = horizontal.get(i);
                component.setBounds(x, reference.getY(), widths[i], reference.getHeight());
                x += widths[i] + getHgap();
            }
        }
    }

    private List<Component> getVisibleHorizontalComponents() {
        List<Component> components = new ArrayList<>(3);
        addIfVisible(components, getLayoutComponent(WEST));
        addIfVisible(components, getLayoutComponent(CENTER));
        addIfVisible(components, getLayoutComponent(EAST));
        return components;
    }

    private static void addIfVisible(List<Component> components, Component component) {
        if (component != null && component.isVisible()) {
            components.add(component);
        }
    }

    private static int[] fitWidths(List<Component> components, int availableWidth) {
        int count = components.size();
        int[] preferred = new int[count];
        int[] minimum = new int[count];
        long minimumTotal = 0;
        for (int i = 0; i < count; i++) {
            Dimension preferredSize = components.get(i).getPreferredSize();
            Dimension minimumSize = components.get(i).getMinimumSize();
            preferred[i] = Math.max(0, preferredSize.width);
            minimum[i] = Math.min(preferred[i], Math.max(0, minimumSize.width));
            minimumTotal += minimum[i];
        }

        if (minimumTotal >= availableWidth) {
            return allocateProportionally(minimum, availableWidth);
        }

        int[] growth = new int[count];
        for (int i = 0; i < count; i++) {
            growth[i] = preferred[i] - minimum[i];
        }
        int[] allocatedGrowth = allocateProportionally(growth, availableWidth - (int) minimumTotal);
        for (int i = 0; i < count; i++) {
            minimum[i] += allocatedGrowth[i];
        }
        return minimum;
    }

    private static int[] allocateProportionally(int[] capacities, int available) {
        int[] allocated = new int[capacities.length];
        long capacityTotal = 0;
        for (int capacity : capacities) {
            capacityTotal += capacity;
        }
        if (capacityTotal == 0 || available == 0) {
            return allocated;
        }

        int allocatedTotal = 0;
        for (int i = 0; i < capacities.length; i++) {
            allocated[i] = (int) ((long) available * capacities[i] / capacityTotal);
            allocatedTotal += allocated[i];
        }
        for (int i = 0; allocatedTotal < available; i = (i + 1) % capacities.length) {
            if (allocated[i] < capacities[i]) {
                allocated[i]++;
                allocatedTotal++;
            }
        }
        return allocated;
    }
}
