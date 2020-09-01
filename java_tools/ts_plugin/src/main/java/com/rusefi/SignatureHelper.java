package com.rusefi;

import com.rusefi.core.Pair;

public class SignatureHelper {

    public static final String PREFIX = "rusEFI ";
    public static final char SLASH = '/';

    public static Pair<String, String> getUrl(String signature) {
        if (!signature.startsWith(PREFIX))
            return null;
        signature = signature.substring(PREFIX.length()).trim();
        String[] elements = signature.split("\\.");
        if (elements.length != 5)
            return null;

        String year = elements[0];
        String month = elements[1];
        String day = elements[2];
        String bundle = elements[3];
        String hash = elements[4];

        String fileName = hash + ".ini";
        return new Pair("https://rusefi.com/online/ini/rusefi/" + year + SLASH + month + SLASH + day + SLASH + bundle + SLASH + fileName, fileName);
    }
}
