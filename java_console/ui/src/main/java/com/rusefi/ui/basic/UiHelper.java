package com.rusefi.ui.basic;

import com.rusefi.core.OsUtil;

import com.devexperts.logging.Logging;
import com.formdev.flatlaf.FlatLaf;
import com.formdev.flatlaf.FlatLightLaf;
import com.rusefi.FileLog;
import com.rusefi.ui.util.DefaultExceptionHandler;

import javax.swing.*;

import static com.devexperts.logging.Logging.getLogging;

public class UiHelper {
    private static final Logging log = getLogging(UiHelper.class);
    static final String THEME_PACKAGE = "com.rusefi.ui.theme";
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

    public static void configureLaf() {
        // [tag:console_theme] Sizes, shapes and per-theme colours live in the .properties
        // files of this package, so a dark theme only needs its own FlatDarkLaf.properties.
        if (!themeRegistered) {
            FlatLaf.registerCustomDefaultsSource(THEME_PACKAGE, UiHelper.class.getClassLoader());
            themeRegistered = true;
        }

        // Enable custom window decorations for a more modern look
        JFrame.setDefaultLookAndFeelDecorated(true);
        JDialog.setDefaultLookAndFeelDecorated(true);

        // console messages/status areas are not ready for FlatDarkLaf - colors are weird
        FlatLightLaf.setup();
    }

    // Lets tests remove the console theme so it does not leak into other tests in the same JVM.
    public static void unregisterThemeForUnitTest() {
        if (themeRegistered) {
            FlatLaf.unregisterCustomDefaultsSource(THEME_PACKAGE, UiHelper.class.getClassLoader());
            themeRegistered = false;
        }
    }
}
