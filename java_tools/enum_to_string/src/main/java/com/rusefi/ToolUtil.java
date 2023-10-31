package com.rusefi;

import com.devexperts.logging.Logging;

import static com.devexperts.logging.Logging.getLogging;

public class ToolUtil {
    private static final Logging log = getLogging(ToolUtil.class);
    public static final String EOL = "\n";

    static boolean isEmptyDefinitionLine(String line) {
        /**
         * historically somehow '!' was the start of comment line
         * '//' is the later added alternative.
         */
        return line.length() == 0 || line.startsWith("!") || line.startsWith("//");
    }

    static boolean startsWithToken(String line, String token) {
        return line.startsWith(token + " ") || line.startsWith(token + "\t");
    }

    static String trimLine(String line) {
        line = line.trim();
        line = line.replaceAll("\\s+", " ");
        return line;
    }
}
