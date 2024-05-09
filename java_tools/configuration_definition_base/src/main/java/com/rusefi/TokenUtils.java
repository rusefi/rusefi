package com.rusefi;

import java.util.ArrayList;
import java.util.List;

public class TokenUtils {
    public static String[] tokenizeWithBraces(String stringToTokenize) {
        final List<String> tokens = new ArrayList<>();
        if (stringToTokenize != null) {
            StringBuilder currentToken = new StringBuilder();
            int openedBracesCount = 0;
            for (char ch : stringToTokenize.toCharArray()) {
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
                                throw new IllegalStateException("Unexpected closing brace");
                            }
                            break;
                        }
                    }
                }
            }
            if (openedBracesCount != 0) {
                throw new IllegalStateException("Unclosed opening brace");
            }
            tokens.add(currentToken.toString());
        }
        return tokens.toArray(String[]::new);
    }
}
