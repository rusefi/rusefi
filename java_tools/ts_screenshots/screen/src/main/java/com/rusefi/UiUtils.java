package com.rusefi;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;

public class UiUtils {
    public static void visitComponents(Component cmp, String prefix, Callback callback) {
        visitComponents(cmp, null, prefix, callback);
    }

    public static void visitComponents(Component component, Component parent, String prefix, Callback callback) {
        if (component == null)
            throw new NullPointerException("component");
        if (component instanceof AbstractButton) {
            AbstractButton ab = (AbstractButton) component;
            System.out.println("[button " + ab.getText() + "]");
        } else if (component instanceof JLabel) {
            JLabel ab = (JLabel) component;
            System.out.println("[label " + ab.getText() + "]");
        } else if (component instanceof JComboBox) {
            JComboBox ab = (JComboBox) component;
            System.out.println("[combo " + ab.getSelectedItem() + "]");
        } else if (component instanceof JPanel) {
            JPanel p = (JPanel) component;
            System.out.println("[panel " + p.getLayout() + "] children=" + p.getComponents().length);
            System.out.println("[panel " + p.getLayout() + "] " + p.getLocation() + " size = " + p.getSize());
        }

        System.out.println(prefix + " I see " + component.getClass());
        callback.onComponent(parent, component);
        Container container = (Container) component;
        if (container == null) {
            // Not a container, return
            return;
        }
        // Go visit and add all children
        for (Component subComponent : container.getComponents()) {
            if (subComponent == null)
                continue;
            visitComponents(subComponent, component, prefix + " " + subComponent.getClass().getSimpleName(), callback);
        }
    }

    public static BufferedImage getScreenShot(Component component) {
        BufferedImage image = new BufferedImage(
                component.getWidth(),
                component.getHeight(),
                BufferedImage.TYPE_INT_RGB
        );
        // call the Component's paint method, using
        // the Graphics object of the image.
        component.paint(image.getGraphics()); // alternately use .printAll(..)
        return image;
    }
}
