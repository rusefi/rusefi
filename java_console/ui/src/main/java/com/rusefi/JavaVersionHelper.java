package com.rusefi;

public class JavaVersionHelper {
    /**
     * java 8 still exists and humans love finding a way to launch console outside of .exe wrapper, for instance while on OSX
     */
    static boolean isAtLeastJava11() {
        String version = System.getProperty("java.version");
        if (version.startsWith("1.")) {
            // Version starts with 1.x, e.g., 1.8.0_202
            String[] parts = version.split("\\.");
            if (parts.length > 1) {
                try {
                    int major = Integer.parseInt(parts[1]);
                    return major >= 11;
                } catch (NumberFormatException e) {
                    return false;
                }
            }
            return false;
        } else {
            // Version is 9, 10, 11, etc.
            int dotIndex = version.indexOf('.');
            String majorStr = dotIndex == -1 ? version : version.substring(0, dotIndex);
            try {
                int major = Integer.parseInt(majorStr);
                return major >= 11;
            } catch (NumberFormatException e) {
                // Handle cases like "11-ea"
                if (majorStr.contains("-")) {
                    try {
                        int major = Integer.parseInt(majorStr.substring(0, majorStr.indexOf('-')));
                        return major >= 11;
                    } catch (NumberFormatException e2) {
                        return false;
                    }
                }
                return false;
            }
        }
    }
}
