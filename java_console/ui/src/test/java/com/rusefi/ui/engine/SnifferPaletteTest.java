package com.rusefi.ui.engine;

import org.junit.jupiter.api.Test;

import java.awt.*;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

import static org.junit.jupiter.api.Assertions.*;

/**
 * The Engine Sniffer plot palettes: theme file and code fallbacks agree, and channel outlines and texts
 * stay readable on their background in both plots. [tag:console_theme]
 */
class SnifferPaletteTest {
    /** WCAG 2 minimum contrast for graphics such as pulse outlines and lines. */
    private static final double MIN_GRAPHIC_CONTRAST = 3.0;
    /** WCAG 2 minimum contrast for normal-size text. */
    private static final double MIN_TEXT_CONTRAST = 4.5;

    @Test
    void themeFileMatchesConstants() throws IOException {
        Properties theme = new Properties();
        try (InputStream stream = SnifferPalette.class.getResourceAsStream("/com/rusefi/ui/theme/FlatLightLaf.properties")) {
            assertNotNull(stream);
            theme.load(stream);
        }
        for (boolean dark : new boolean[]{false, true}) {
            SnifferPalette palette = dark ? SnifferPalette.DARK : SnifferPalette.LIGHT;
            String own = "Console.EngineSniffer." + (dark ? "dark" : "light") + ".";
            String plot = "Console.LogChart." + (dark ? "dark" : "light") + ".";
            assertColor(theme, plot + "background", palette.background);
            assertColor(theme, plot + "grid", palette.divider);
            assertColor(theme, own + "text", palette.text);
            assertColor(theme, own + "mutedText", palette.mutedText);
            assertColor(theme, own + "durationText", palette.durationText);
            assertColor(theme, own + "engineCycle", palette.engineCycle);
            assertColor(theme, own + "timeScale", palette.timeScale);
            for (SnifferPalette.Role role : SnifferPalette.Role.values()) {
                assertColor(theme, own + role.key(), palette.outline(role));
            }
        }
    }

    @Test
    void outlinesAndTextsAreReadable() {
        for (SnifferPalette palette : new SnifferPalette[]{SnifferPalette.LIGHT, SnifferPalette.DARK}) {
            String mode = palette.dark ? "dark" : "light";
            for (SnifferPalette.Role role : SnifferPalette.Role.values()) {
                double contrast = contrast(palette.outline(role), palette.background);
                assertTrue(contrast >= MIN_GRAPHIC_CONTRAST, mode + " " + role + " outline contrast " + contrast);
            }
            assertTrue(contrast(palette.engineCycle, palette.background) >= MIN_GRAPHIC_CONTRAST, mode + " engine cycle");
            assertTrue(contrast(palette.text, palette.background) >= MIN_TEXT_CONTRAST, mode + " text");
            assertTrue(contrast(palette.mutedText, palette.background) >= MIN_TEXT_CONTRAST, mode + " muted text");
            assertTrue(contrast(palette.durationText, palette.background) >= MIN_TEXT_CONTRAST, mode + " duration text");
        }
    }

    @Test
    void channelFamiliesFollowTheChannelNames() {
        assertEquals(SnifferPalette.Role.TRIGGER, SnifferPalette.Role.of("t1"));
        assertEquals(SnifferPalette.Role.TACH, SnifferPalette.Role.of("tach"));
        assertEquals(SnifferPalette.Role.TACH, SnifferPalette.Role.of("dizzy"));
        assertEquals(SnifferPalette.Role.OTHER, SnifferPalette.Role.of("hip"));
    }

    private static void assertColor(Properties theme, String key, Color expected) {
        String value = theme.getProperty(key);
        assertNotNull(value, key);
        assertEquals(expected, Color.decode(value.trim()), key);
    }

    private static double contrast(Color a, Color b) {
        double la = luminance(a);
        double lb = luminance(b);
        return (Math.max(la, lb) + 0.05) / (Math.min(la, lb) + 0.05);
    }

    private static double luminance(Color c) {
        return 0.2126 * channel(c.getRed()) + 0.7152 * channel(c.getGreen()) + 0.0722 * channel(c.getBlue());
    }

    private static double channel(int value) {
        double v = value / 255.0;
        return v <= 0.03928 ? v / 12.92 : Math.pow((v + 0.055) / 1.055, 2.4);
    }
}
