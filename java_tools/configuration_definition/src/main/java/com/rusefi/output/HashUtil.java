package com.rusefi.output;

public class HashUtil {
    public static int hash(String string) {
        return djb2lowerCase(string);
    }

    public static int djb2lowerCase(String string) {
        int hash = 5381;

        for (int i = 0; i < string.length(); i++) {
            char c = Character.toLowerCase(string.charAt(i));
            hash = ((hash << 5) + hash) + (byte) c/* hash * 33 + c */;
        }
        return hash;
    }
}
