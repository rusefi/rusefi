package com.rusefi.ui.basic;

import com.devexperts.logging.Logging;
import com.rusefi.FileLog;
import com.rusefi.ui.util.DefaultExceptionHandler;

import javax.swing.*;

import static com.devexperts.logging.Logging.getLogging;

public class UiHelper {
    private static final Logging log = getLogging(UiHelper.class);

    public static void commonUiStartup() {
        log.info("OS name: " + FileLog.getOsName());
        log.info("OS version: " + System.getProperty(FileLog.OS_VERSION));


        DefaultExceptionHandler.install();
    }
}
