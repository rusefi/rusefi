package com.rusefi.ui.util;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.assertEquals;

/**
 * Gauge groups such as the CAN O2, fuel pressure and TPS dialogs' four gauges wrap into a 2x2 grid
 * of large gauges at medium widths instead of one row of small ones.
 */
class SquareGridLayoutTest {
    // The tuning dialogs' gauge grid: gap 4, minimum 60, preferred 90, largest 180.
    private static JPanel grid(int cells, int width) {
        JPanel panel = new JPanel(new SquareGridLayout(2, 4, 60, 90, 180));
        for (int i = 0; i < cells; i++) {
            panel.add(new JPanel());
        }
        panel.setSize(width, 2000);
        panel.doLayout();
        return panel;
    }

    private static int rows(JPanel panel) {
        return (int) java.util.Arrays.stream(panel.getComponents()).mapToInt(Component::getY).distinct().count();
    }

    @Test
    void fourGaugesFormATwoByTwoGridAtMediumWidth() {
        JPanel panel = grid(4, 420);
        assertEquals(2, rows(panel));
        assertEquals(208, panel.getComponent(0).getWidth(), "the width is shared by two columns");
        assertEquals(panel.getComponent(0).getY(), panel.getComponent(1).getY());
        assertEquals(panel.getComponent(0).getX(), panel.getComponent(2).getX());
    }

    @Test
    void wideWindowStillFitsOneRow() {
        JPanel panel = grid(4, 1000);
        assertEquals(1, rows(panel), "cells would exceed the largest size in two columns");
        assertEquals(247, panel.getComponent(0).getWidth());
    }

    @Test
    void narrowWindowNeverGoesBelowWhatFits() {
        JPanel panel = grid(4, 200);
        assertEquals(2, rows(panel));
        assertEquals(98, panel.getComponent(0).getWidth());

        JPanel single = grid(4, 120);
        assertEquals(4, rows(single), "only one preferred-size column fits");
    }

    @Test
    void twoGaugesStayInOneRow() {
        assertEquals(1, rows(grid(2, 420)));
        assertEquals(1, rows(grid(1, 420)));
    }
}
