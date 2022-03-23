package com.rusefi.util;

import com.devexperts.logging.Logging;

import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;

import static com.devexperts.logging.Logging.getLogging;

public class IoUtils {
    private static final Logging log = getLogging(IoUtils.class);

    public static final Charset CHARSET = StandardCharsets.ISO_8859_1;

    public static void exit(String s, int exitCode) {
        log.error(s, new Exception());
        System.exit(exitCode);
    }
}
