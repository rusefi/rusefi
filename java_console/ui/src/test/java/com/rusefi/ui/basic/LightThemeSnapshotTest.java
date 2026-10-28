package com.rusefi.ui.basic;

import com.formdev.flatlaf.FlatLightLaf;
import org.junit.jupiter.api.Test;

import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;

import static org.junit.jupiter.api.Assertions.fail;

/**
 * [tag:console_theme] [tag:light_theme_snapshot] Every colour the console theme sets in the light
 * look, recorded in light-theme-snapshot.txt: the console's own Console.* keys and every FlatLaf key
 * whose value differs from stock FlatLightLaf (for example through @background). Refactoring the
 * theme files (palette variables, the dark theme) must leave the light look exactly as it was.
 * <p>
 * An intended light change: copy build/console-theme/light-theme-snapshot.txt, which a failing run
 * writes, over the snapshot in src/test/resources and review the diff.
 */
public class LightThemeSnapshotTest {
    static final String SNAPSHOT = "/com/rusefi/ui/theme/light-theme-snapshot.txt";

    @Test
    public void lightThemeColoursAreUnchanged() throws Exception {
        String actual = render(snapshot());
        String expected = read();
        if (actual.equals(expected)) {
            return;
        }
        Path written = Paths.get("build", "console-theme", "light-theme-snapshot.txt");
        Files.createDirectories(written.getParent());
        Files.write(written, actual.getBytes(StandardCharsets.UTF_8));
        fail("Light theme colours changed:\n" + diff(expected, actual)
            + "\nIf intended, copy " + written.toAbsolutePath() + " to src/test/resources" + SNAPSHOT);
    }

    /** Key -> colour or style text of the console light theme, where it differs from stock FlatLightLaf. */
    static SortedMap<String, String> snapshot() throws Exception {
        LookAndFeel previous = UIManager.getLookAndFeel();
        boolean previousFrameDecorated = JFrame.isDefaultLookAndFeelDecorated();
        boolean previousDialogDecorated = JDialog.isDefaultLookAndFeelDecorated();
        Map<String, String> stock = new HashMap<>();
        SortedMap<String, String> console = new TreeMap<>();
        try {
            SwingUtilities.invokeAndWait(() -> {
                UiHelper.unregisterThemeForUnitTest();
                FlatLightLaf.setup();
                stock.putAll(values());
                UiHelper.configureLaf(false);
                console.putAll(values());
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
        console.entrySet().removeIf(e -> !e.getKey().startsWith("Console.") && e.getValue().equals(stock.get(e.getKey())));
        return console;
    }

    /** Colours as #aarrggbb and [style] classes as text, by key, of the installed look and feel. */
    private static Map<String, String> values() {
        UIDefaults defaults = UIManager.getLookAndFeelDefaults();
        Map<String, String> result = new HashMap<>();
        for (Object key : new ArrayList<>(defaults.keySet())) {
            if (!(key instanceof String)) {
                continue;
            }
            Object value = defaults.get(key);
            if (value instanceof Color) {
                result.put((String) key, String.format("#%08x", ((Color) value).getRGB()));
            } else if (((String) key).startsWith("[style]")) {
                result.put((String) key, String.valueOf(value instanceof Map ? new TreeMap<>((Map<?, ?>) value) : value));
            }
        }
        return result;
    }

    private static String render(SortedMap<String, String> values) {
        StringBuilder sb = new StringBuilder();
        values.forEach((key, value) -> sb.append(key).append('=').append(value).append('\n'));
        return sb.toString();
    }

    private static String read() throws IOException {
        try (InputStream stream = LightThemeSnapshotTest.class.getResourceAsStream(SNAPSHOT)) {
            if (stream == null) {
                return "";
            }
            java.io.ByteArrayOutputStream out = new java.io.ByteArrayOutputStream();
            byte[] buffer = new byte[8192];
            int n;
            while ((n = stream.read(buffer)) > 0) {
                out.write(buffer, 0, n);
            }
            return new String(out.toByteArray(), StandardCharsets.UTF_8).replace("\r\n", "\n");
        }
    }

    private static String diff(String expected, String actual) {
        Map<String, String> before = parse(expected);
        Map<String, String> after = parse(actual);
        StringBuilder sb = new StringBuilder();
        int shown = 0;
        SortedSet<String> keys = new TreeSet<>(before.keySet());
        keys.addAll(after.keySet());
        for (String key : keys) {
            String a = before.get(key);
            String b = after.get(key);
            if (!Objects.equals(a, b) && shown++ < 40) {
                sb.append("  ").append(key).append(": ").append(a).append(" -> ").append(b).append('\n');
            }
        }
        return sb.toString();
    }

    private static Map<String, String> parse(String text) {
        Map<String, String> map = new HashMap<>();
        for (String line : text.split("\n")) {
            int eq = line.indexOf('=');
            if (eq > 0) {
                map.put(line.substring(0, eq), line.substring(eq + 1));
            }
        }
        return map;
    }
}
