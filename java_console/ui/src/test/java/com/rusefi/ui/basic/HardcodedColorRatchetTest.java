package com.rusefi.ui.basic;

import org.junit.jupiter.api.Test;

import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.regex.Pattern;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.fail;

/**
 * [tag:console_dark_mode] [tag:fixed_color] Colours written into console UI code instead of coming from
 * the theme (com/rusefi/ui/theme/*.properties) stay light in the dark theme. This test counts them per
 * source file and compares with hardcoded-colors-baseline.txt: a new hard-coded colour fails, and so
 * does a file that got fewer until the baseline is lowered with it, so the count only goes down.
 * <p>
 * Not counted: a fallback on the same line as a theme lookup (UIManager.getColor, themed(...),
 * {@code c != null ? c : ...}); the palette classes in {@link #PALETTES}, whose constants tests keep
 * equal to the theme files; and a colour that must look the same in every theme, marked with
 * {@code [tag:fixed_color]} and a reason on the same or the previous line.
 */
public class HardcodedColorRatchetTest {
    static final String BASELINE = "/com/rusefi/ui/theme/hardcoded-colors-baseline.txt";
    private static final Path SOURCES = Paths.get("src", "main", "java");
    private static final Pattern COLOR = Pattern.compile(
        "new Color\\(|Color\\.decode\\(|\\bColor\\.(white|black|red|green|blue|gray|lightGray|darkGray|yellow|orange|pink|cyan|magenta"
            + "|WHITE|BLACK|RED|GREEN|BLUE|GRAY|LIGHT_GRAY|DARK_GRAY|YELLOW|ORANGE|PINK|CYAN|MAGENTA)\\b");
    private static final Pattern THEME_LOOKUP = Pattern.compile("UIManager\\.getColor|themed\\(|\\bcolor\\(\"|!= null \\?");
    /**
     * Not a colour written into the code: a translucent copy of another colour, e.g. a theme colour, and
     * parsing a variable (the hex fallbacks of a theme key, e.g. StatusChip.Tone).
     */
    private static final Pattern DERIVED = Pattern.compile("new Color\\(\\w+\\.getRed\\(\\)|Color\\.decode\\([A-Za-z_]\\w*\\)");
    /** Light and dark plot palettes kept equal to the theme files by PlotPaletteTest and SnifferPaletteTest */
    private static final Set<String> PALETTES = new HashSet<>(Arrays.asList(
        "com/rusefi/ui/logview/PlotPalette.java",
        "com/rusefi/ui/engine/SnifferPalette.java"));

    @Test
    public void hardcodedColoursOnlyGoDown() throws IOException {
        SortedMap<String, Integer> actual = count();
        SortedMap<String, Integer> baseline = baseline();
        List<String> more = new ArrayList<>();
        List<String> fewer = new ArrayList<>();
        SortedSet<String> files = new TreeSet<>(actual.keySet());
        files.addAll(baseline.keySet());
        for (String file : files) {
            int now = actual.getOrDefault(file, 0);
            int before = baseline.getOrDefault(file, 0);
            if (now > before) {
                more.add(file + ": " + before + " -> " + now);
            } else if (now < before) {
                fewer.add(file + ": " + before + " -> " + now);
            }
        }
        if (more.isEmpty() && fewer.isEmpty()) {
            return;
        }
        Path written = Paths.get("build", "console-theme", "hardcoded-colors-baseline.txt");
        Files.createDirectories(written.getParent());
        Files.write(written, render(actual).getBytes(StandardCharsets.UTF_8));
        StringBuilder message = new StringBuilder();
        if (!more.isEmpty()) {
            message.append("New hard-coded colours; take them from the theme (UIManager / Console.* keys), or mark a colour that is"
                + " the same in every theme with [tag:fixed_color] and a reason:\n  ").append(String.join("\n  ", more)).append('\n');
        }
        if (!fewer.isEmpty()) {
            message.append("Fewer hard-coded colours, lower the baseline:\n  ").append(String.join("\n  ", fewer)).append('\n');
        }
        message.append("Updated counts: ").append(written.toAbsolutePath()).append(" -> src/test/resources").append(BASELINE);
        fail(message.toString());
    }

    static SortedMap<String, Integer> count() throws IOException {
        SortedMap<String, Integer> counts = new TreeMap<>();
        List<Path> sources;
        try (Stream<Path> walk = Files.walk(SOURCES)) {
            sources = walk.filter(p -> p.toString().endsWith(".java")).collect(Collectors.toList());
        }
        for (Path source : sources) {
            String file = SOURCES.relativize(source).toString().replace('\\', '/');
            if (PALETTES.contains(file)) {
                continue;
            }
            List<String> lines = Files.readAllLines(source, StandardCharsets.UTF_8);
            int n = 0;
            for (int i = 0; i < lines.size(); i++) {
                String line = DERIVED.matcher(lines.get(i)).replaceAll("");
                if (!COLOR.matcher(line).find() || THEME_LOOKUP.matcher(line).find()) {
                    continue;
                }
                boolean marked = line.contains("[tag:fixed_color]") || (i > 0 && lines.get(i - 1).contains("[tag:fixed_color]"));
                if (!marked) {
                    n++;
                }
            }
            if (n > 0) {
                counts.put(file, n);
            }
        }
        return counts;
    }

    private static SortedMap<String, Integer> baseline() throws IOException {
        SortedMap<String, Integer> counts = new TreeMap<>();
        try (InputStream stream = HardcodedColorRatchetTest.class.getResourceAsStream(BASELINE)) {
            assertNotNull(stream, BASELINE);
            String text = new String(readAll(stream), StandardCharsets.UTF_8);
            for (String line : text.split("\r?\n")) {
                int eq = line.lastIndexOf('=');
                if (!line.startsWith("#") && eq > 0) {
                    counts.put(line.substring(0, eq), Integer.parseInt(line.substring(eq + 1).trim()));
                }
            }
        }
        return counts;
    }

    private static String render(SortedMap<String, Integer> counts) {
        StringBuilder sb = new StringBuilder("# Hard-coded colours per console source file, see HardcodedColorRatchetTest. Only ever lower these.\n");
        sb.append("# total=").append(counts.values().stream().mapToInt(Integer::intValue).sum()).append('\n');
        counts.forEach((file, n) -> sb.append(file).append('=').append(n).append('\n'));
        return sb.toString();
    }

    private static byte[] readAll(InputStream stream) throws IOException {
        java.io.ByteArrayOutputStream out = new java.io.ByteArrayOutputStream();
        byte[] buffer = new byte[8192];
        int n;
        while ((n = stream.read(buffer)) > 0) {
            out.write(buffer, 0, n);
        }
        return out.toByteArray();
    }
}
