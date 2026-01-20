package com.opensr5.ini;

import com.rusefi.ini.reader.IniFileReaderUtil;
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
    final String msg;

    /**
     * Often we want to look-up line by first token.
     * Each line in this map has at least two tokens
     */
    private final Map<String, Line> asSet = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    public RawIniFile(List<Line> lines, String msg) {
        this.lines = lines;
        this.msg = msg;

        for (Line line : lines) {
            if (line.tokens.length > 1) {
                String key = line.tokens[0];
                if (!asSet.containsKey(key)) {
                    // odd API: we want to use *first* occurrence of specific key (that's important at least for signature)
                    asSet.put(key, line);
                }
            }
        }
    }

    public static RawIniFile read(String fileName) throws FileNotFoundException {
        return IniFileReaderUtil.read(new FileInputStream(fileName));
    }

    @NotNull
    public Line getMandatoryLine(String key) throws MandatoryLineMissing {
        Line result = getByKey(key);
        if (result == null)
            throw new MandatoryLineMissing("Line not found: " + key);
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

    public int getSimpleIntegerProperty(String key) throws MandatoryLineMissing {
        Line line = asSet.get(key);
        if (line == null)
            throw new MandatoryLineMissing("Line not found: " + key);
        String value = line.getTokens()[1];
        return Integer.parseInt(value);
    }

    public int getSimpleIntegerProperty(String key, int defaultValue) throws MandatoryLineMissing {
        if (!asSet.containsKey(key))
            return defaultValue;
        return getSimpleIntegerProperty(key);
    }

    public List<String> getValues(String key) throws MandatoryLineMissing {
        RawIniFile.Line line = getMandatoryLine(key);
        return Arrays.asList(line.getTokens()).subList(1, line.getTokens().length);
    }

    /**
     * Immutable representation of since ini file line
     */
    public static class Line {
        private String rawText;
        private String[] tokens;

        private static final String namePattern = "[[\\w\\d\\s_]]+";
        private static final Pattern DIRECTIVE = Pattern.compile("#(if\\s" + namePattern + "|else|elif\\s\" + namePattern + \"|endif)");

        public Line(String rawText) {
            this.rawText = rawText;
            tokens = IniFileReaderUtil.splitTokens(rawText);
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
