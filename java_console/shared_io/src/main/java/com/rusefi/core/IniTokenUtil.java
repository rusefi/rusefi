package com.rusefi.core;

import java.util.ArrayList;

/** Tokenization shared by INI signature validation and the INI model parser. */
public final class IniTokenUtil {
    private IniTokenUtil() {
    }

    public static String[] splitTokens(String str) {
        ArrayList<String> strings = new ArrayList<>();
        boolean inQuote = false;
        boolean hadQuote = false;
        int openedBraceCount = 0;
        StringBuilder sb = new StringBuilder();
        for (final char c: str.toCharArray()) {
            // When inside braces (expressions), treat quotes as regular characters
            // and don't use them for tokenization - expressions handle their own quotes
            if ((c == '\"' && openedBraceCount == 0) || (isTokenSeparator(c) && !inQuote && (openedBraceCount == 0))) {
                if (c == '\"') {
                    inQuote = !inQuote;
                    if (!inQuote) {
                        // we are here when we close quotes
                        hadQuote = true;
                    }
                }
                if (!inQuote && sb.length() > 0) {
                    strings.add(sb.toString());
                    sb.delete(0, sb.length());
                    hadQuote = false;
                } else if (hadQuote) {
                    strings.add("");
                    hadQuote = false;
                }
            } else {
                sb.append(c);
                switch (c) {
                    case '{': {
                        openedBraceCount++;
                        break;
                    }
                    case '}': {
                        if (0 < openedBraceCount) {
                            openedBraceCount--;
                        } else {
                            throw new IllegalArgumentException(String.format("Unexpected closing brace: %s", str));
                        }
                        break;
                    }
                }
            }
        }
        if (sb.length() > 0) {
            strings.add(sb.toString());
        }

        return strings.toArray(new String[strings.size()]);
    }

    private static boolean isTokenSeparator(int c) {
        return c == ' ' || c == '\t' || c == '=' || c == ',';
    }
}
