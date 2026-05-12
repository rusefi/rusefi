package com.rusefi.util;

import com.devexperts.logging.Logging;

import static com.devexperts.logging.Logging.getLogging;

public class ExitUtil {
    private static final Logging log = getLogging(ExitUtil.class);

    public static void exit(String s, int exitCode) {
        log.error(s, new Exception());
        System.exit(exitCode);
    }
}
