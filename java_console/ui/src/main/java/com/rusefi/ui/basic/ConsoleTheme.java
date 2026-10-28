package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.rusefi.core.preferences.storage.Node;
import com.rusefi.core.preferences.storage.PersistentConfiguration;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.Locale;
import java.util.concurrent.TimeUnit;

import static com.devexperts.logging.Logging.getLogging;

/**
 * [tag:console_dark_mode] The console appearance chosen in File > Appearance: light, dark, or the operating
 * system's setting. It is read once at start-up by {@link UiHelper#configureLaf()}, so a change applies on the
 * next start. The -Dconsole.theme=dark|light JVM option overrides the saved choice (development, tests).
 */
public final class ConsoleTheme {
    private static final Logging log = getLogging(ConsoleTheme.class);
    static final String KEY = "console_theme";
    private static final long OS_QUERY_TIMEOUT_MS = 2000;

    public enum Choice {
        LIGHT("Light"),
        DARK("Dark"),
        SYSTEM("Follow system");

        public final String label;

        Choice(String label) {
            this.label = label;
        }
    }

    private ConsoleTheme() {
    }

    /** The saved choice; light when nothing valid is saved. */
    public static Choice saved() {
        return saved(PersistentConfiguration.getConfig().getRoot());
    }

    static Choice saved(Node root) {
        String value = root.getProperty(KEY, Choice.LIGHT.name());
        try {
            return Choice.valueOf(value.trim().toUpperCase(Locale.ROOT));
        } catch (IllegalArgumentException e) {
            return Choice.LIGHT;
        }
    }

    /** Saves the choice for the next start. */
    public static void save(Choice choice) {
        PersistentConfiguration.getConfig().getRoot().setProperty(KEY, choice.name());
    }

    /** Whether to install the dark theme now: the JVM option if given, else the saved choice. */
    static boolean isDarkRequested() {
        String override;
        try {
            override = System.getProperty(UiHelper.THEME_PROPERTY, "").trim();
        } catch (SecurityException e) {
            override = "";
        }
        if (!override.isEmpty()) {
            return "dark".equalsIgnoreCase(override);
        }
        return isDark(saved(), ConsoleTheme::isOsDark);
    }

    interface OsDarkQuery {
        boolean isDark();
    }

    static boolean isDark(Choice choice, OsDarkQuery os) {
        switch (choice) {
            case DARK:
                return true;
            case SYSTEM:
                return os.isDark();
            default:
                return false;
        }
    }

    /**
     * The operating system's dark mode: Windows "apps use dark mode", macOS dark appearance, GNOME's dark colour
     * scheme. False when it cannot be read.
     */
    static boolean isOsDark() {
        String os = System.getProperty("os.name", "").toLowerCase(Locale.ROOT);
        try {
            if (os.contains("win")) {
                String out = run("reg", "query", "HKCU\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                    "/v", "AppsUseLightTheme");
                return out.matches("(?s).*AppsUseLightTheme\\s+REG_DWORD\\s+0x0\\b.*");
            }
            if (os.contains("mac")) {
                return run("defaults", "read", "-g", "AppleInterfaceStyle").toLowerCase(Locale.ROOT).contains("dark");
            }
            return run("gsettings", "get", "org.gnome.desktop.interface", "color-scheme")
                .toLowerCase(Locale.ROOT).contains("dark");
        } catch (Exception e) {
            log.info("Could not read the system dark mode: " + e);
            return false;
        }
    }

    private static String run(String... command) throws Exception {
        Process process = new ProcessBuilder(command).redirectErrorStream(true).start();
        // wait first: the answer is one short line, and a hung query must not hold up the start-up
        if (!process.waitFor(OS_QUERY_TIMEOUT_MS, TimeUnit.MILLISECONDS)) {
            process.destroyForcibly();
            return "";
        }
        StringBuilder out = new StringBuilder();
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream(), StandardCharsets.UTF_8))) {
            String line;
            while ((line = reader.readLine()) != null) {
                out.append(line).append('\n');
            }
        }
        return out.toString();
    }
}
