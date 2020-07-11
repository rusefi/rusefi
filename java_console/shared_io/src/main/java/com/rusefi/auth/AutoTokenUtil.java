package com.rusefi.auth;

public class AutoTokenUtil {

    public static final int TOKEN_LENGTH = 8 + 1 + 4 + 1 + 4 + 1 + 4 + 1 + 12;

    public static boolean isToken(String content) {
        if (content == null)
            return false;
        content = content.trim();
        if (content.length() != TOKEN_LENGTH)
            return false;
        return content.charAt(8) == '-' && content.charAt(8 + 1 + 4) == '-';
    }
}
