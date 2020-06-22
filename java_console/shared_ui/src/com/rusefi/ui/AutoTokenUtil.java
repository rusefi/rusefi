package com.rusefi.ui;

public class AutoTokenUtil {
    public static boolean isToken(String content) {
        content = content.trim();
        if (content.length() != 8 + 1 + 4 + 1 + 4 + 1 + 4 + 1 + 12)
            return false;
        return content.charAt(8) == '-' && content.charAt(8 + 1 + 4) == '-';
    }
}
