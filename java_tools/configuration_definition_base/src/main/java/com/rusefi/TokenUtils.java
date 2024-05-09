package com.rusefi;

import java.util.ArrayList;
import java.util.List;

public class TokenUtils {
    public static String[] tokenizeWithBraces(final String stringToTokenize) {
        final List<String> tokens = new ArrayList<>();
        if (stringToTokenize != null) {
            StringBuilder currentToken = new StringBuilder();
            int openedBracesCount = 0;
            for (final char ch : stringToTokenize.toCharArray()) {
                // we finish token parsing only if it contains no unclosed braces
                if ((ch == ',') && (openedBracesCount == 0)) {
                    tokens.add(currentToken.toString());
                    currentToken.setLength(0);
                } else {
                    currentToken.append(ch);
                    switch (ch) {
                        case '{': {
                            openedBracesCount++;
                            break;
                        }
                        case '}': {
                            if (0 < openedBracesCount) {
                                openedBracesCount--;
                            } else {
                                throw new IllegalArgumentException(String.format(
                                    "Unexpected closing brace: %s",
                                    stringToTokenize
                                ));
                            }
                            break;
                        }
                    }
                }
            }
            if (openedBracesCount != 0) {
                throw new IllegalArgumentException(String.format("Unclosed opening brace: %s", stringToTokenize));
            }
            tokens.add(currentToken.toString());
        }
        return tokens.toArray(String[]::new);
    }
}
