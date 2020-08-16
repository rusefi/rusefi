package com.rusefi.auth;

import org.jetbrains.annotations.NotNull;

import static com.rusefi.ui.storage.PersistentConfiguration.getConfig;

public class AutoTokenUtil {

    public static final int TOKEN_LENGTH = 8 + 1 + 4 + 1 + 4 + 1 + 4 + 1 + 12;
    public static final String TOKEN_PROFILE_URL = "https://rusefi.com/forum/ucp.php?i=254";
    public static final String TOKEN_WARNING = "Please copy token from your forum profile";
    private static final String AUTH_TOKEN = "auth_token";

    public static boolean isToken(String content) {
        if (content == null)
            return false;
        content = content.trim();
        if (content.length() != TOKEN_LENGTH)
            return false;
        return content.charAt(8) == '-' && content.charAt(8 + 1 + 4) == '-';
    }

    public static void setAuthToken(String value) {
        getConfig().getRoot().setProperty(AUTH_TOKEN, value);
    }

    @NotNull
    public static String getAuthToken() {
        return getConfig().getRoot().getProperty(AUTH_TOKEN);
    }
}
