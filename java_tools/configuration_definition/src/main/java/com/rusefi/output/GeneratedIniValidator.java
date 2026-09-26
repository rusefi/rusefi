package com.rusefi.output;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.regex.Pattern;

import static com.rusefi.util.LazyFile.CHARSET;

/** Checks generated INIs independently of the permissive runtime INI reader. */
public final class GeneratedIniValidator {
    private static final Pattern TEMPLATE_MARKER = Pattern.compile("@@|@#|#@|@(?:if_|endif_block)|@OFFSET@");

    private GeneratedIniValidator() {
    }

    public static void main(String[] args) throws IOException {
        if (args.length == 0)
            throw new IllegalArgumentException("Usage: GeneratedIniValidator <generated.ini> [...]");
        for (String file : args)
            validate(Path.of(file));
    }

    public static void validate(Path file) throws IOException {
        try (BufferedReader reader = Files.newBufferedReader(file, CHARSET)) {
            String line;
            int lineNumber = 0;
            while ((line = reader.readLine()) != null) {
                lineNumber++;
                try {
                    validateLine(line);
                } catch (IllegalStateException e) {
                    throw new IllegalStateException(file + ":" + lineNumber + ": " + e.getMessage(), e);
                }
            }
        }
    }

    public static void validateLine(String line) {
        if (TEMPLATE_MARKER.matcher(withoutComment(line)).find())
            throw new IllegalStateException("Unresolved or malformed template marker in [" + line.trim() + "]");
    }

    // Semicolons inside strings are data, not comments (including escaped quotes).
    static String withoutComment(String line) {
        boolean quoted = false;
        boolean escaped = false;
        for (int i = 0; i < line.length(); i++) {
            char c = line.charAt(i);
            if (c == '"' && !escaped)
                quoted = !quoted;
            if (c == ';' && !quoted)
                return line.substring(0, i);
            escaped = c == '\\' && !escaped;
        }
        return line;
    }
}
