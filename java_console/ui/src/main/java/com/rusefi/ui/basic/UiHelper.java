package com.rusefi.ui.basic;

import com.rusefi.core.OsUtil;

import com.devexperts.logging.Logging;
import com.formdev.flatlaf.FlatDarkLaf;
import com.formdev.flatlaf.FlatLaf;
import com.formdev.flatlaf.FlatLightLaf;
import com.rusefi.FileLog;
import com.rusefi.ui.util.DefaultExceptionHandler;

import javax.swing.*;

import static com.devexperts.logging.Logging.getLogging;

public class UiHelper {
    private static final Logging log = getLogging(UiHelper.class);
    static final String THEME_PACKAGE = "com.rusefi.ui.theme";
    static final String THEME_PROPERTY = "console.theme";
    private static boolean themeRegistered;

    public static void commonUiStartup() {
        log.info("OS name: " + OsUtil.getOsName());
        log.info("OS version: " + System.getProperty(FileLog.OS_VERSION));

        try {
            configureLaf();
        } catch (Exception ex) {
            System.err.println("Failed to initialize LaF");
        }

        DefaultExceptionHandler.install();
    }

    /** The appearance chosen in File > Appearance, or the -Dconsole.theme option [tag:console_dark_mode]. */
    public static void configureLaf() {
        configureLaf(ConsoleTheme.isDarkRequested());
    }

    /** True while the dark console theme is installed. */
    public static boolean isDarkTheme() {
        return FlatLaf.isLafDark();
    }

    /**
     * [tag:console_dark_mode] Settings key for a remembered Light/Dark plot choice (Knock Analyzer, Engine
     * Sniffer). The dark console theme keeps its own choice, so picking a light plot in the light theme does
     * not make plots light in the dark theme; the light theme keeps using {@code key} as before.
     */
    public static String plotThemeKey(String key) {
        return isDarkTheme() ? key + "_in_dark_theme" : key;
    }

    public static void configureLaf(boolean dark) {
        // [tag:console_theme] Sizes, shapes and per-theme colours live in the .properties
        // files of this package: FlatLaf.properties for both themes, then FlatLightLaf.properties
        // or FlatDarkLaf.properties for the one installed.
        if (!themeRegistered) {
            FlatLaf.registerCustomDefaultsSource(THEME_PACKAGE, UiHelper.class.getClassLoader());
            themeRegistered = true;
        }

        // Enable custom window decorations for a more modern look
        JFrame.setDefaultLookAndFeelDecorated(true);
        JDialog.setDefaultLookAndFeelDecorated(true);

        // a failed dark setup falls back to the light theme rather than to stock Swing
        if (!dark || !FlatDarkLaf.setup()) {
            FlatLightLaf.setup();
        }
    }

    // Lets tests remove the console theme so it does not leak into other tests in the same JVM.
    public static void unregisterThemeForUnitTest() {
        if (themeRegistered) {
            FlatLaf.unregisterCustomDefaultsSource(THEME_PACKAGE, UiHelper.class.getClassLoader());
            themeRegistered = false;
        }
    }
}
