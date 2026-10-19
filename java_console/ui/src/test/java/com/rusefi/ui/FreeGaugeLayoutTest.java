package com.rusefi.ui;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Rectangle2D;

import static org.junit.jupiter.api.Assertions.*;

class FreeGaugeLayoutTest {
    @Test
    void placesChildrenByFractionAndScalesWithTheContainer() {
        JPanel panel = new JPanel(new FreeGaugeLayout());
        JLabel left = new JLabel("left");
        JLabel right = new JLabel("right");
        panel.add(left, new Rectangle2D.Double(0, 0, 1.0 / 3, 1));
        panel.add(right, new Rectangle2D.Double(1.0 / 3, 0.5, 2.0 / 3, 0.5));

        panel.setSize(300, 200);
        panel.doLayout();
        assertEquals(new Rectangle(0, 0, 100, 200), left.getBounds());
        assertEquals(new Rectangle(100, 100, 200, 100), right.getBounds());

        panel.setSize(1000, 400);
        panel.doLayout();
        assertEquals(new Rectangle(0, 0, 333, 400), left.getBounds());
        assertEquals(new Rectangle(333, 200, 667, 200), right.getBounds(), "Touching gauges keep touching");
    }

    @Test
    void rejectsChildrenWithoutFractionalBounds() {
        JPanel panel = new JPanel(new FreeGaugeLayout());
        assertThrows(IllegalArgumentException.class, () -> panel.add(new JLabel()));
    }
}
