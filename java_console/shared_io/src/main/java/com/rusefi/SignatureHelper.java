package com.rusefi;

public class SignatureHelper {

    public static final String PREFIX = "rusEFI ";

    static String getUrl(String signature) {
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

        return "https://rusefi.com/online/ini/rusefi/" + year + "/" + month + "/" + day + "/" + bundle + "/" + hash + ".ini";
    }
}
