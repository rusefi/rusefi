package com.rusefi.ui.widgets.tune;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class CalibrationColorPaletteTest {
    private static final String[] KEYS = {"Console.Calibration.low", "Console.Calibration.middle", "Console.Calibration.high"};

    @AfterEach
    public void clearKeys() {
        for (String key : KEYS) {
            UIManager.put(key, null);
        }
    }

    @Test
    public void scaleFollowsTheThemeKeys() {
        Color low = new Color(0x1d3b57);
        Color middle = new Color(0x33383d);
        Color high = new Color(0x8a531f);
        UIManager.put(KEYS[0], low);
        UIManager.put(KEYS[1], middle);
        UIManager.put(KEYS[2], high);

        assertEquals(low, CalibrationColorPalette.background(0, 0, 100));
        assertEquals(middle, CalibrationColorPalette.background(50, 0, 100));
        assertEquals(high, CalibrationColorPalette.background(100, 0, 100));
        assertEquals(middle, CalibrationColorPalette.background(7, 7, 7), "a flat table is the middle colour");
    }

    @Test
    public void eachScaleKeepsOneTextColour() {
        // Light theme: dark text on every cell. Dark theme: light text on every cell.
        assertEquals(Color.BLACK, CalibrationColorPalette.foreground(new Color(0xb7d0ea)));
        assertEquals(Color.BLACK, CalibrationColorPalette.foreground(new Color(0xedeff1)));
        assertEquals(Color.BLACK, CalibrationColorPalette.foreground(new Color(0xf3b36f)));
        assertEquals(Color.WHITE, CalibrationColorPalette.foreground(new Color(0x1d3b57)));
        assertEquals(Color.WHITE, CalibrationColorPalette.foreground(new Color(0x33383d)));
        assertEquals(Color.WHITE, CalibrationColorPalette.foreground(new Color(0x8a531f)));
    }
}
