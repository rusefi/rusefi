package com.rusefi.output;

import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static com.rusefi.ToolUtil.EOL;

/**
 * Loaded from an optional per-board file named "ignore_gauges.txt".
 * Each non-blank, non-comment line is a pattern. Lines starting with {@code ;} or {@code #} are comments.
 * <p>
 * Pattern syntax:
 * <ul>
 *   <li>{@code name} — exact gauge name match</li>
 *   <li>{@code prefix*}, {@code *suffix}, {@code *} — wildcard in name</li>
 *   <li>{@code name@Category Name} — match gauge by name only inside a specific category</li>
 *   <li>{@code *@Category Name} — hide every gauge inside a category</li>
 *   <li>Both the name part and the category part support {@code *} wildcards independently.</li>
 * </ul>
 *
 * <p>Example file:
 * <pre>
 * ; hide all raw FFT spectrum gauges
 * knockSpectrum*
 *
 * ; hide the whole alternator PID category
 * *@Alternator PID
 *
 * ; hide only vvt outputs 3 and 4 from this board
 * vvtOutput3Gauge@VVT
 * vvtOutput4Gauge@VVT
 * </pre>
 */
public class GaugeIgnoreList {
    private static final Pattern GAUGE_NAME_PATTERN = Pattern.compile("^\\s*(\\w+)\\s*=");
    private static final Pattern GAUGE_CATEGORY_PATTERN = Pattern.compile("^\\s*gaugeCategory\\s*=\\s*(.+)$");
    private static final String GAUGE_CATEGORY_KEYWORD = "gaugeCategory";

    private final List<String> patterns = new ArrayList<>();

    /** Tracks the category that the most recently seen {@code gaugeCategory} line declared. */
    private String currentCategory = "";

    /**
     * Load from the given file path. Returns an empty list if the file does not exist.
     */
    public static GaugeIgnoreList load(String filePath) throws IOException {
        GaugeIgnoreList list = new GaugeIgnoreList();
        list.addPatternsFrom(filePath);
        return list;
    }

    /**
     * Append patterns from the given file into this list. No-op if the file does not exist.
     * Allows multiple files (e.g. global + per-board) to be merged into one list.
     */
    public void addPatternsFrom(String filePath) throws IOException {
        File file = new File(filePath);
        if (!file.exists())
            return;
        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            String line;
            while ((line = reader.readLine()) != null) {
                line = line.trim();
                if (line.isEmpty() || line.startsWith(";") || line.startsWith("#"))
                    continue;
                patterns.add(line);
            }
        }
    }

    /** Construct a list directly from patterns — useful in tests. */
    public static GaugeIgnoreList forPatterns(String... patterns) {
        GaugeIgnoreList list = new GaugeIgnoreList();
        for (String p : patterns)
            list.patterns.add(p);
        return list;
    }

    public boolean isEmpty() {
        return patterns.isEmpty();
    }

    /**
     * Returns true if {@code gaugeName} should be excluded given the currently tracked category.
     * Uses the category last seen via {@link #filterGauges}.
     */
    public boolean shouldIgnore(String gaugeName) {
        return shouldIgnore(gaugeName, currentCategory);
    }

    /**
     * Returns true if {@code gaugeName} inside {@code category} matches any pattern.
     */
    public boolean shouldIgnore(String gaugeName, String category) {
        for (String pattern : patterns) {
            if (matchesEntry(pattern, gaugeName, category))
                return true;
        }
        return false;
    }

    /**
     * Returns the gauge name from a TunerStudio gauge definition line, or {@code null} if the
     * line is not a gauge definition ({@code gaugeCategory}, comments, blank lines, etc.).
     */
    public static String extractGaugeName(String line) {
        Matcher m = GAUGE_NAME_PATTERN.matcher(line);
        if (!m.find())
            return null;
        String name = m.group(1);
        return GAUGE_CATEGORY_KEYWORD.equals(name) ? null : name;
    }

    /**
     * Returns the category name from a {@code gaugeCategory = Name} line, or {@code null} if
     * the line is not a category declaration.
     */
    public static String extractCategoryName(String line) {
        Matcher m = GAUGE_CATEGORY_PATTERN.matcher(line);
        return m.find() ? m.group(1).trim() : null;
    }

    /**
     * Filter gauge definitions from {@code content}, updating the internal category state as
     * {@code gaugeCategory} lines are encountered.
     * <ul>
     *   <li>Single-line: returns {@code null} if the gauge should be ignored, or the line unchanged.</li>
     *   <li>Multi-line (from variable substitution like {@code @@LIVE_DATA_GAUGES_FROM_FILE@@}):
     *       removes matching gauge definition lines and returns the rest joined with EOL.</li>
     * </ul>
     */
    public String filterGauges(String content) {
        if (isEmpty())
            return content;

        if (!content.contains("\n")) {
            String cat = extractCategoryName(content);
            if (cat != null) {
                currentCategory = cat;
                return shouldHideCategory(cat) ? null : content;
            }
            String name = extractGaugeName(content);
            return (name != null && shouldIgnore(name)) ? null : content;
        }

        // Multi-line content: split, filter, rejoin
        String[] lines = content.split("\n");
        StringBuilder result = new StringBuilder();
        boolean first = true;
        for (String subLine : lines) {
            String cleanLine = subLine.endsWith("\r") ? subLine.substring(0, subLine.length() - 1) : subLine;
            String cat = extractCategoryName(cleanLine);
            if (cat != null) {
                currentCategory = cat;
                if (shouldHideCategory(cat))
                    continue;
            } else {
                String name = extractGaugeName(cleanLine);
                if (name != null && shouldIgnore(name))
                    continue;
            }
            if (!first)
                result.append(EOL);
            result.append(cleanLine);
            first = false;
        }
        return result.toString();
    }

    /**
     * Returns true if a {@code *@category} pattern covers {@code category}, meaning every
     * gauge inside it would be hidden and the category header line should also be dropped.
     * Name-only patterns (no {@code @}) never trigger category-line removal.
     */
    private boolean shouldHideCategory(String category) {
        for (String pattern : patterns) {
            int atIdx = pattern.indexOf('@');
            if (atIdx < 0)
                continue;
            String namePart = pattern.substring(0, atIdx);
            String categoryPart = pattern.substring(atIdx + 1);
            if ("*".equals(namePart) && matchesWildcard(categoryPart, category))
                return true;
        }
        return false;
    }

    private static boolean matchesEntry(String pattern, String gaugeName, String category) {
        int atIdx = pattern.indexOf('@');
        if (atIdx < 0) {
            // No category constraint — match by name only
            return matchesWildcard(pattern, gaugeName);
        }
        String namePart = pattern.substring(0, atIdx);
        String categoryPart = pattern.substring(atIdx + 1);
        return matchesWildcard(namePart, gaugeName) && matchesWildcard(categoryPart, category);
    }

    private static boolean matchesWildcard(String pattern, String name) {
        if (!pattern.contains("*"))
            return pattern.equals(name);
        // Escape regex metacharacters, then replace * with .*
        String regex = pattern
                .replace("\\", "\\\\")
                .replace(".", "\\.")
                .replace("*", ".*");
        return name.matches(regex);
    }
}
