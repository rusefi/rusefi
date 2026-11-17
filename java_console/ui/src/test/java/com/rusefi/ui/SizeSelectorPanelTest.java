package com.rusefi.ui;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;

import static org.junit.jupiter.api.Assertions.assertArrayEquals;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class SizeSelectorPanelTest {
    @Test
    public void hoverPreviewsAndClickPicksRowsByColumns() throws Exception {
        int[] picked = new int[2];
        SwingUtilities.invokeAndWait(() -> {
            SizeSelectorPanel panel = new SizeSelectorPanel((row, column) -> {
                picked[0] = row;
                picked[1] = column;
            });
            assertEquals("1 × 1", panel.getCaptionText());
            // cells are laid out row by row: row 2, column 3 is index (2 - 1) * WIDTH + (3 - 1)
            Component cell = cells(panel).getComponent(SizeSelectorPanel.WIDTH + 2);
            cell.dispatchEvent(new MouseEvent(cell, MouseEvent.MOUSE_ENTERED, 0, 0, 5, 5, 0, false));
            assertEquals("2 × 3", panel.getCaptionText());
            cell.dispatchEvent(new MouseEvent(cell, MouseEvent.MOUSE_CLICKED, 0, 0, 5, 5, 1, false));
            assertArrayEquals(new int[]{2, 3}, picked);
        });
    }

    static Container cells(SizeSelectorPanel panel) {
        return (Container) ((BorderLayout) panel.getLayout()).getLayoutComponent(BorderLayout.CENTER);
    }
}
