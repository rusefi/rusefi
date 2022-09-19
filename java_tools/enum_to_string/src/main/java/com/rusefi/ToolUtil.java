package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.util.LazyFile;

import java.net.URISyntaxException;

import static com.devexperts.logging.Logging.getLogging;

public class ToolUtil {
    private static final Logging log = getLogging(ToolUtil.class);
    public static final String EOL = "\n";
    public static String TOOL = "(unknown script)";

    static String getJarFileName() {
        try {

            // Get path of the JAR file
            String jarPath = VariableRegistry.class
                    .getProtectionDomain()
                    .getCodeSource()
                    .getLocation()
                    .toURI()
                    .getPath();
            if (log.debugEnabled())
                log.debug("JAR Path : " + jarPath);

            // Get name of the JAR file
            return jarPath.substring(jarPath.lastIndexOf("/") + 1);
        } catch (URISyntaxException e) {
            return "(unknown jar)";
        }
    }

    public static String getGeneratedAutomaticallyTag() {
        return LazyFile.LAZY_FILE_TAG + getJarFileName() + " based on " + TOOL + " ";
    }

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
