package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.formdev.flatlaf.FlatDarkLaf;
import com.formdev.flatlaf.FlatLightLaf;
import com.rusefi.FileLog;
import com.rusefi.ui.util.DefaultExceptionHandler;

import javax.swing.*;

import java.awt.*;

import static com.devexperts.logging.Logging.getLogging;

public class UiHelper {
    private static final Logging log = getLogging(UiHelper.class);

    public static void commonUiStartup() {
        log.info("OS name: " + FileLog.getOsName());
        log.info("OS version: " + System.getProperty(FileLog.OS_VERSION));

        try {
            configureLaf();
        } catch (Exception ex) {
            System.err.println("Failed to initialize LaF");
        }

        DefaultExceptionHandler.install();
    }

    public static void configureLaf() {
/*
todo: re-apply only for dark L&F
        // Use a more modern dark color scheme
        UIManager.put("TitlePane.background", new Color(40, 44, 52));
        UIManager.put("TitlePane.foreground", Color.WHITE);
        UIManager.put("TitlePane.buttonHoverBackground", new Color(60, 64, 72));
        UIManager.put("TitlePane.closeHoverBackground", new Color(232, 17, 35));
        UIManager.put("TitlePane.closeHoverForeground", Color.WHITE);
 */

        // Custom icons for window controls can be configured via UIManager if needed,
        // but FlatLaf's default vector icons are already quite modern.
        // We can however customize their style.
        UIManager.put("TitlePane.buttonSize", new Dimension(45, 30));
        UIManager.put("TitlePane.iconSize", new Dimension(18, 18));

        // Use more modern symbols for minimize/maximize/close if desired
        // FlatLaf uses vector icons, we can customize some properties

        // Modern rounded corners for components
        UIManager.put("Button.arc", 10);
        UIManager.put("Component.arc", 10);
        UIManager.put("ProgressBar.arc", 10);
        UIManager.put("TextComponent.arc", 10);

        // Enable custom window decorations for a more modern look
        JFrame.setDefaultLookAndFeelDecorated(true);
        JDialog.setDefaultLookAndFeelDecorated(true);

        // console messages/status areas are not ready for FlatDarkLaf - colors are weird
        FlatLightLaf.setup();
    }
}
