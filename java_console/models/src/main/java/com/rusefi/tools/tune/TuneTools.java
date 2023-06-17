package com.rusefi.tools.tune;

import org.jetbrains.annotations.NotNull;

public class TuneTools {
    private static String quote(String string) {
        return "\"" + string + "\"";
    }

    public static int resolveEnumByName(String tsCustomLine, String key) {
        String quotedKey = quote(key);
        String[] tokens = tsCustomLine.split(",");
        int magicOffset = 4; // skipping meta info header
        for (int i = 0; i < tokens.length - magicOffset; i++) {
            String token = tokens[i + magicOffset].trim();
            if (token.equalsIgnoreCase(quotedKey))
                return i;
        }
        throw new IllegalStateException("Enum name [" + key + "] not found in " + tsCustomLine);
    }

    @NotNull
    public static String getAssignmentCode(String name, String value) {
        return "\tengineConfiguration->" + name + " = " + value + ";\n";
    }
}
