package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.formdev.flatlaf.FlatLightLaf;
import com.rusefi.FileLog;
import com.rusefi.ui.util.DefaultExceptionHandler;

import javax.swing.*;

import static com.devexperts.logging.Logging.getLogging;

public class UiHelper {
    private static final Logging log = getLogging(UiHelper.class);

    public static void commonUiStartup() {
        log.info("OS name: " + FileLog.getOsName());
        log.info("OS version: " + System.getProperty(FileLog.OS_VERSION));

        try {
            UIManager.setLookAndFeel(new FlatLightLaf());
        } catch (Exception ex) {
            System.err.println("Failed to initialize LaF");
        }

        DefaultExceptionHandler.install();
    }
}
