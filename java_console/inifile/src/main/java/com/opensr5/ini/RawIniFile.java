package com.opensr5.ini;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/1/2017
 */
public class RawIniFile {
    /**
     * A list of lines. Lines which are only a comment were filtered out already.
     */
    private final List<Line> lines;

    /**
     * Often we want to look-up line by first token.
     * Each line in this map has at least two tokens
     */
    private final Map<String, Line> asSet = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    public RawIniFile(List<Line> lines) {
        this.lines = lines;

        for (Line line : lines) {
            if (line.tokens.length > 1)
                asSet.put(line.tokens[0], line);
        }
    }

    public static RawIniFile read(String fileName) throws FileNotFoundException {
        return IniFileReader.read(new FileInputStream(fileName));
    }

    @NotNull
    public Line getMandatoryLine(String key) {
        Line result = getByKey(key);
        if (result == null)
            throw new IllegalStateException("Line not found: " + key);
        assert result.tokens.length > 1;
        return result;
    }

    @Nullable
    public Line getByKey(String key) {
        return asSet.get(key);
    }

    public List<Line> getLines() {
        return lines;
    }

    public int getSimpleIntegerProperty(String key) {
        Line line = asSet.get(key);
        if (line == null)
            throw new IllegalStateException("Line not found: " + key);
        String value = line.getTokens()[1];
        return Integer.parseInt(value);
    }

    public int getSimpleIntegerProperty(String key, int defaultValue) {
        if (!asSet.containsKey(key))
            return defaultValue;
        return getSimpleIntegerProperty(key);
    }

    public List<String> getValues(String key) {
        RawIniFile.Line line = getMandatoryLine(key);
        return Arrays.asList(line.getTokens()).subList(1, line.getTokens().length);
    }

    /**
     * Immutable representation of since ini file line
     */
    public static class Line {
        private final String rawText;
        private final String[] tokens;

        private static final String namePattern = "[[\\w\\d\\s_]]+";
        private static final Pattern DIRECTIVE = Pattern.compile("#(if\\s" + namePattern + "|else|elif\\s\" + namePattern + \"|endif)");

        public Line(String rawText) {
            this.rawText = rawText;
            tokens = IniFileReader.splitTokens(rawText);
        }

        public String[] getTokens() {
            return tokens;
        }

        public static boolean isCommentLine(String rawText) {
            return rawText.trim().startsWith(";");
        }


        public static boolean isPreprocessorDirective(String rawText) {
            Matcher matcher = DIRECTIVE.matcher(rawText);
            return matcher.matches();
        }

        public String getRawText() {
            return rawText;
        }

        @Override
        public String toString() {
            return "Line{" +
                    "rawText='" + rawText + '\'' +
                    ", tokens=" + Arrays.toString(tokens) +
                    '}';
        }
    }
}
