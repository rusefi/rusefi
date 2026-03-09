package com.rusefi.compatibility.ini;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.List;

/**
 * Compatibility preprocessor for INI files containing #if/#else/#endif directives.
 * <p>
 * When a #if directive is encountered, only the first (if) branch is kept;
 * the #else branch and any nested directives within it are discarded.
 * <p>
 * This allows reading older INI files that still contain preprocessor directives
 * for conditional compilation (e.g., #if LAMBDA).
 */
public class IniPreprocessor {

    private IniPreprocessor() {
    }

    /**
     * Preprocess raw INI text lines, resolving #if/#else/#endif directives
     * by always selecting the first (#if) branch.
     * <p>
     * Nesting is supported: directives encountered inside a skipped block are
     * tracked so the correct #endif closes the correct #if.
     *
     * @param rawLines list of raw text lines from an INI file (empty lines already stripped)
     * @return new list with preprocessor directives resolved; directive lines are never emitted
     */
    public static List<String> preprocessLines(List<String> rawLines) {
        List<String> result = new ArrayList<>(rawLines.size());

        // Each entry represents one #if nesting level.
        // The value is what `currentlyKeeping` was BEFORE entering this level,
        // so it can be restored on #endif.
        Deque<Boolean> stack = new ArrayDeque<>();

        // true → we are in an active (first) branch and should emit lines
        // false → we are inside a skipped branch (#else or nested inside one)
        boolean currentlyKeeping = true;

        for (String rawLine : rawLines) {
            String trimmed = rawLine.trim();

            if (isIfDirective(trimmed)) {
                // Push the current state; take the #if branch, which inherits parent keeping state.
                stack.push(currentlyKeeping);
                // currentlyKeeping stays the same: if parent was skipping we stay skipping,
                // if parent was keeping we keep the if-branch.
                continue;
            }

            if (isElseOrElifDirective(trimmed)) {
                // We already took the #if branch, so always skip the #else/#elif branch.
                currentlyKeeping = false;
                continue;
            }

            if (isEndifDirective(trimmed)) {
                if (!stack.isEmpty()) {
                    currentlyKeeping = stack.pop();
                }
                continue;
            }

            if (currentlyKeeping) {
                result.add(rawLine);
            }
        }

        return result;
    }

    public static final String DIRECTIVE_TYPE = "directive";

    /**
     * Returns true if the given config-field type name represents a preprocessor directive
     * (as opposed to a real configuration field type like "int" or "boolean").
     */
    public static boolean isDirectiveType(String typeName) {
        return DIRECTIVE_TYPE.equalsIgnoreCase(typeName);
    }

    /**
     * Returns true if the line (already trimmed) is any preprocessor directive:
     * {@code #if}, {@code #elif}, {@code #else}, or {@code #endif}.
     */
    public static boolean isDirectiveLine(String trimmed) {
        return isIfDirective(trimmed) || isElseOrElifDirective(trimmed) || isEndifDirective(trimmed);
    }

    private static boolean isIfDirective(String trimmed) {
        // Require a space after #if to avoid matching #ifdef, #ifndef
        return trimmed.startsWith("#if ") || trimmed.equals("#if");
    }

    private static boolean isElseOrElifDirective(String trimmed) {
        return trimmed.equals("#else") || trimmed.startsWith("#elif ") || trimmed.equals("#elif");
    }

    private static boolean isEndifDirective(String trimmed) {
        return trimmed.equals("#endif");
    }
}
