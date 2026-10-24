package com.rusefi.ui.util;

import javax.swing.*;
import javax.swing.plaf.UIResource;
import java.awt.*;
import java.awt.event.ContainerAdapter;
import java.awt.event.ContainerEvent;

/**
 * Lets a card's colour show through the plain panels laid out on it, such as the setup wizard's
 * step cards and the Tuning dialog card. Panels with an explicitly assigned colour keep it.
 */
public final class TransparentPanels {
    private static final String INSTALLED = "rusefi.transparentPanels";

    /**
     * Clears the theme background of plain panels and scroll viewports under {@code root}, now and
     * whenever children are added later.
     */
    public static void install(JComponent root) {
        for (Component child : root.getComponents()) {
            makeTransparent(child);
        }
        if (root.getClientProperty(INSTALLED) == null) {
            root.putClientProperty(INSTALLED, Boolean.TRUE);
            root.addContainerListener(new ContainerAdapter() {
                @Override
                public void componentAdded(ContainerEvent e) {
                    makeTransparent(e.getChild());
                }
            });
        }
    }

    private static void makeTransparent(Component component) {
        if (component instanceof JPanel || component instanceof JViewport || component instanceof JScrollPane) {
            JComponent c = (JComponent) component;
            if (c.getBackground() instanceof UIResource) {
                c.setOpaque(false);
            }
            install(c);
        }
    }

    private TransparentPanels() {
    }
}
