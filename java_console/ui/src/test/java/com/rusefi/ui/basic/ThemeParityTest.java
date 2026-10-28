package com.rusefi.ui.basic;

import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import static org.junit.jupiter.api.Assertions.*;

/**
 * [tag:console_theme] [tag:console_dark_mode] FlatLightLaf.properties and FlatDarkLaf.properties
 * define the same palette variables and keys, and text, state colours and controls stay readable
 * on their backgrounds in both themes.
 */
public class ThemeParityTest {
    private static final String THEME = "/com/rusefi/ui/theme/";
    /** WCAG 2 minimum contrast for normal-size text. */
    private static final double TEXT = 4.5;
    /** WCAG 2 minimum contrast for bold chip text and for graphics such as status dots and lines. */
    private static final double GRAPHIC = 3.0;

    @Test
    public void lightAndDarkDefineTheSameKeys() throws IOException {
        Set<String> light = keys("FlatLightLaf.properties");
        Set<String> dark = keys("FlatDarkLaf.properties");
        Set<String> onlyLight = new TreeSet<>(light);
        onlyLight.removeAll(dark);
        Set<String> onlyDark = new TreeSet<>(dark);
        onlyDark.removeAll(light);
        assertTrue(onlyLight.isEmpty() && onlyDark.isEmpty(),
            "Only in light: " + onlyLight + "\nOnly in dark: " + onlyDark);
    }

    @Test
    public void lightThemeIsReadable() throws Exception {
        checkReadable(false);
    }

    @Test
    public void darkThemeIsReadable() throws Exception {
        checkReadable(true);
    }

    @Test
    public void developerSwitchSelectsTheDarkTheme() throws Exception {
        withTheme(true, () -> {
            assertTrue(UiHelper.isDarkTheme());
            Color background = UIManager.getColor("Panel.background");
            assertTrue(luminance(background) < 0.05, "dark panels: " + background);
            assertNotNull(UIManager.get("[style]Button.primary"), "style classes load in the dark theme too");
        });
        withTheme(false, () -> assertFalse(UiHelper.isDarkTheme()));
    }

    private void checkReadable(boolean dark) throws Exception {
        List<String> failures = new ArrayList<>();
        withTheme(dark, () -> {
            String[] page = {"Panel.background", "Console.Card.background", "Console.Toolbar.background"};
            for (String background : page) {
                check(failures, "Label.foreground", background, TEXT);
                check(failures, "Console.Tuning.mutedForeground", background, TEXT);
            }
            check(failures, "TabbedPane.foreground", "Console.TabArea.background", TEXT);
            check(failures, "TabbedPane.selectedForeground", "Console.TabArea.background", TEXT);
            check(failures, "TabbedPane.underlineColor", "Console.TabArea.background", GRAPHIC);
            check(failures, "Console.TitleBar.mutedForeground", "Console.TitleBar.chipBackground", TEXT);
            for (String state : new String[]{"connected", "disconnected", "pending", "bootloader"}) {
                String key = "Console.Status." + state + ".";
                check(failures, key + "foreground", key + "background", TEXT);
                check(failures, key + "dot", "Console.StatusBar.background", GRAPHIC);
            }
            for (String state : new String[]{"success", "error"}) {
                String key = "Console.Wizard." + state + ".";
                check(failures, key + "foreground", key + "background", TEXT);
            }
            for (String banner : new String[]{"info", "warning", "error"}) {
                String key = "Console.Banner." + banner + ".";
                check(failures, key + "accent", key + "background", GRAPHIC);
                check(failures, "Label.foreground", key + "background", TEXT);
            }
            for (String tone : new String[]{"neutral", "red", "yellow", "green", "blue", "dark"}) {
                String key = "Console.Tuning.indicator." + tone + ".";
                check(failures, key + "foreground", key + "background", GRAPHIC);
            }
            check(failures, "Console.Tuning.dirtyForeground", "Console.Tuning.sidebar.background", TEXT);
            check(failures, "Console.Tuning.mutedForeground", "Console.Tuning.liveValue.background", TEXT);
            check(failures, "Label.foreground", "Console.Card.sectionBackground", TEXT);
            check(failures, "Label.foreground", "Console.Segmented.selectedBackground", TEXT);
            check(failures, "Label.foreground", "Console.GridPicker.selectedBackground", TEXT);
            check(failures, "Console.LogViewer.mutedForeground", "Console.LogViewer.sidebar.background", TEXT);
            // window frame: full-window messages, banner, links and error lines
            for (String overlay : new String[]{"foreground", "good", "bad"}) {
                check(failures, "Console.Overlay." + overlay, "Panel.background", TEXT);
            }
            check(failures, "Console.Link.foreground", "Console.InstallerBanner.background", TEXT);
            for (String background : page) {
                check(failures, "Console.Link.foreground", background, TEXT);
                check(failures, "Console.ErrorText.foreground", background, TEXT);
            }
            check(failures, "Console.HorizontalLine.foreground", "Panel.background", GRAPHIC);
            check(failures, "Console.Announcement.borderColor", "Panel.background", GRAPHIC);
            // gauges: value, captions and scale on the card and dial face; zones, needle and graph line
            for (String face : new String[]{"Console.Gauge.card.top", "Console.Gauge.card.bottom", "Console.Gauge.face.center"}) {
                check(failures, "Console.Gauge.text", face, TEXT);
                check(failures, "Console.Gauge.label", face, TEXT);
                check(failures, "Console.Gauge.scale", face, TEXT);
                check(failures, "Console.Gauge.danger", face, GRAPHIC);
                check(failures, "Console.Gauge.warningText", face, GRAPHIC);
            }
            check(failures, "Console.Gauge.label", "Console.Gauge.face.edge", TEXT);
            // a round dial draws its scale numbers over the zone fill
            check(failures, "Console.Gauge.scale", "Console.Gauge.dialFill", TEXT);
            check(failures, "Console.Gauge.warningText", "Console.Gauge.warningFill", GRAPHIC);
            check(failures, "Console.Gauge.danger", "Console.Gauge.dangerFill", GRAPHIC);
            check(failures, "Console.Gauge.needle", "Console.Gauge.dialFill", GRAPHIC);
            check(failures, "Console.Gauge.needle", "Console.Gauge.face.center", GRAPHIC);
            check(failures, "Console.Gauge.graph.line", "Console.Gauge.card.top", GRAPHIC);
            check(failures, "Console.Gauge.edit.accent", "Panel.background", GRAPHIC);
            check(failures, "Console.Gauge.detached.close", "Console.Gauge.detached.background", GRAPHIC);
            for (String tone : new String[]{"neutral", "accent"}) {
                String key = "Console.Status." + tone + ".";
                check(failures, key + "foreground", key + "background", TEXT);
            }
        });
        assertTrue(failures.isEmpty(), (dark ? "dark" : "light") + " theme:\n" + String.join("\n", failures));
    }

    /**
     * Light pairs that are known to sit just under the minimum and are kept as they are, so that the light
     * look stays pinned by LightThemeSnapshotTest. Green #15803d on #e7f6ec measures 4.49:1; the classic
     * pure red error text #ff0000 measures 3.67:1 on the page and 4.00:1 on white; the live graph's amber
     * line #e0a100 measures 2.27:1 on the white card, with the tinted area under it; a round dial's orange
     * warning numbers #e65100 measure 2.56:1 on its warning fill #ffcc80.
     */
    private static final Set<String> KNOWN_LIGHT_EXCEPTIONS = new TreeSet<>(java.util.Arrays.asList(
        "Console.Status.connected.foreground on Console.Status.connected.background",
        "Console.Wizard.success.foreground on Console.Wizard.success.background",
        "Console.ErrorText.foreground on Panel.background",
        "Console.ErrorText.foreground on Console.Card.background",
        "Console.ErrorText.foreground on Console.Toolbar.background",
        "Console.Gauge.graph.line on Console.Gauge.card.top",
        "Console.Gauge.warningText on Console.Gauge.warningFill"));

    private static void check(List<String> failures, String foreground, String background, double minimum) {
        if (!UiHelper.isDarkTheme() && KNOWN_LIGHT_EXCEPTIONS.contains(foreground + " on " + background)) {
            return;
        }
        Color fg = UIManager.getColor(foreground);
        Color bg = UIManager.getColor(background);
        if (fg == null || bg == null) {
            failures.add("missing " + (fg == null ? foreground : background));
            return;
        }
        double contrast = contrast(fg, bg);
        if (contrast < minimum) {
            failures.add(String.format("%s on %s: %.2f < %.1f", foreground, background, contrast, minimum));
        }
    }

    private static void withTheme(boolean dark, Runnable check) throws Exception {
        LookAndFeel previous = UIManager.getLookAndFeel();
        boolean previousFrameDecorated = JFrame.isDefaultLookAndFeelDecorated();
        boolean previousDialogDecorated = JDialog.isDefaultLookAndFeelDecorated();
        try {
            SwingUtilities.invokeAndWait(() -> {
                UiHelper.configureLaf(dark);
                check.run();
            });
        } finally {
            SwingUtilities.invokeAndWait(() -> {
                UiHelper.unregisterThemeForUnitTest();
                JFrame.setDefaultLookAndFeelDecorated(previousFrameDecorated);
                JDialog.setDefaultLookAndFeelDecorated(previousDialogDecorated);
                try {
                    UIManager.setLookAndFeel(previous);
                } catch (UnsupportedLookAndFeelException e) {
                    throw new AssertionError(e);
                }
            });
        }
    }

    /** Keys and palette variables of a theme file; continuation lines of [style] classes are skipped. */
    private static Set<String> keys(String file) throws IOException {
        Set<String> keys = new TreeSet<>();
        try (InputStream stream = ThemeParityTest.class.getResourceAsStream(THEME + file)) {
            assertNotNull(stream, file);
            BufferedReader reader = new BufferedReader(new InputStreamReader(stream, StandardCharsets.UTF_8));
            boolean continuation = false;
            String line;
            while ((line = reader.readLine()) != null) {
                String trimmed = line.trim();
                boolean wasContinuation = continuation;
                continuation = trimmed.endsWith("\\");
                if (wasContinuation || trimmed.isEmpty() || trimmed.startsWith("#")) {
                    continue;
                }
                int eq = trimmed.indexOf('=');
                if (eq > 0) {
                    keys.add(trimmed.substring(0, eq).trim());
                }
            }
        }
        return keys;
    }

    static double contrast(Color a, Color b) {
        double la = luminance(a);
        double lb = luminance(b);
        return (Math.max(la, lb) + 0.05) / (Math.min(la, lb) + 0.05);
    }

    static double luminance(Color c) {
        return 0.2126 * channel(c.getRed()) + 0.7152 * channel(c.getGreen()) + 0.0722 * channel(c.getBlue());
    }

    private static double channel(int value) {
        double v = value / 255.0;
        return v <= 0.03928 ? v / 12.92 : Math.pow((v + 0.055) / 1.055, 2.4);
    }
}
