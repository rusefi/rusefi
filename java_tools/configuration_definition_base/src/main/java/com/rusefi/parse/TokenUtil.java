package com.rusefi.parse;

import com.rusefi.core.Pair;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class TokenUtil {
    private static final Pattern pattern = Pattern.compile("\\s");

    public static Pair<String, String> grabFirstTokenAndTheRest(String line) {
        Matcher matcher = pattern.matcher(line);
        if (!matcher.find())
            throw new IllegalStateException("whitespace expected in [" + line + "]");
        int index = matcher.start();
        String first = line.substring(0, index);

        String second = line.substring(index).trim();
        return new Pair<>(first, second);
    }
}
