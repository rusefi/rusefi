package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;
import com.rusefi.ui.widgets.GaugeDisplayKind;
import com.rusefi.ui.widgets.GaugeScaleOverrides;

import java.awt.geom.Rectangle2D;
import java.io.IOException;
import java.io.Reader;
import java.io.Writer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.Map;
import java.util.Properties;

/**
 * Portable dashboard settings, independent of the rest of Console preferences.
 * <p>
 * The grid layout ({@code element_N}) is always kept. The free layout ({@code free_N})
 * exists only in Free mode: every switch to Free starts it from the current grid, and
 * switching back to Grid discards it. Free gauges store their position and size as
 * fractions of the gauge area; a higher index is drawn on top. Grid-mode files are
 * written as version 1, so they stay readable by older consoles.
 */
final class GaugeLayout {
    static final String ROWS = "gauges_rows";
    static final String COLUMNS = "gauges_cols";
    static final String MODE = "gauges_mode";
    static final String MODE_GRID = "grid";
    static final String MODE_FREE = "free";
    static final String FREE_COUNT = "free_count";
    static final String GRID_ELEMENT = "element_";
    static final String FREE_ELEMENT = "free_";
    static final String X = "x";
    static final String Y = "y";
    static final String WIDTH = "w";
    static final String HEIGHT = "h";
    static final int MAX_FREE_GAUGES = 64;
    /** Smallest free gauge, as a fraction of the gauge area. */
    static final double MIN_FREE_SIZE = 0.02;
    // Tolerates rounding in fractions written by Double.toString.
    private static final double EPSILON = 1e-9;
    // Older consoles read only the keys they know, so the scale settings need no version bump.
    private static final String[] ELEMENT_PROPERTIES = {"gauge", "type", "display",
        GaugeScaleOverrides.MIN, GaugeScaleOverrides.MAX, GaugeScaleOverrides.DANGER, GaugeScaleOverrides.DECIMALS};
    private static final String[] BOUNDS_PROPERTIES = {X, Y, WIDTH, HEIGHT};
    private static final String[] GRAPH_PROPERTIES = {"sensor", "period", "auto_scale", "lower", "upper"};

    private GaugeLayout() {
    }

    static boolean isFreeMode(Node config) {
        return MODE_FREE.equals(config.getProperty(MODE, MODE_GRID));
    }

    static boolean hasFreeLayout(Node config) {
        return config.getProperty(FREE_COUNT, null) != null;
    }

    static int freeCount(Node config) {
        return Math.max(0, Math.min(MAX_FREE_GAUGES, config.getIntProperty(FREE_COUNT, 0)));
    }

    /**
     * Replaces the free layout with a copy of the current grid: each cell becomes a
     * free gauge with the same content, position and size.
     */
    static void seedFreeFromGrid(Node config) {
        clearFreeLayout(config);
        int rows = Math.max(1, config.getIntProperty(ROWS, 1));
        int columns = Math.max(1, config.getIntProperty(COLUMNS, 1));
        int count = Math.min(MAX_FREE_GAUGES, rows * columns);
        for (int i = 0; i < count; i++) {
            Node free = config.getChild(FREE_ELEMENT + i);
            copy(config.getChild(GRID_ELEMENT + i), free);
            setBounds(free, new Rectangle2D.Double((double) (i % columns) / columns,
                (double) (i / columns) / rows, 1.0 / columns, 1.0 / rows));
        }
        config.setProperty(FREE_COUNT, count);
    }

    static void clearFreeLayout(Node config) {
        config.getConfig().keySet().removeIf(key -> key.startsWith(FREE_ELEMENT)
            && key.substring(FREE_ELEMENT.length()).matches("\\d+") || key.equals(FREE_COUNT));
    }

    static void setBounds(Node element, Rectangle2D.Double bounds) {
        element.setProperty(X, bounds.x);
        element.setProperty(Y, bounds.y);
        element.setProperty(WIDTH, bounds.width);
        element.setProperty(HEIGHT, bounds.height);
    }

    /** Bounds of a free gauge, forced inside the gauge area so a damaged preference stays visible. */
    static Rectangle2D.Double getBounds(Node element) {
        double width = clamp(element.getDoubleProperty(WIDTH, 0.25), MIN_FREE_SIZE, 1);
        double height = clamp(element.getDoubleProperty(HEIGHT, 0.25), MIN_FREE_SIZE, 1);
        double x = clamp(element.getDoubleProperty(X, 0), 0, 1 - width);
        double y = clamp(element.getDoubleProperty(Y, 0), 0, 1 - height);
        return new Rectangle2D.Double(x, y, width, height);
    }

    private static double clamp(double value, double min, double max) {
        return Double.isFinite(value) ? Math.max(min, Math.min(max, value)) : min;
    }

    private static void copy(Node source, Node target) {
        for (Map.Entry<String, Object> entry : source.getConfig().entrySet()) {
            if (entry.getValue() instanceof Node) {
                copy((Node) entry.getValue(), target.getChild(entry.getKey()));
            } else {
                target.getConfig().put(entry.getKey(), entry.getValue());
            }
        }
    }

    static void save(Path file, Node config) throws IOException {
        boolean free = isFreeMode(config) && hasFreeLayout(config);
        Properties properties = new Properties();
        properties.setProperty("format", "rusefi-gauges");
        properties.setProperty("version", free ? "2" : "1");
        copyToFile(properties, "", config, ROWS, COLUMNS);
        int count = config.getIntProperty(ROWS, 0) * config.getIntProperty(COLUMNS, 0);
        for (int i = 0; i < count; i++) {
            elementToFile(properties, GRID_ELEMENT + i, config.getChild(GRID_ELEMENT + i));
        }
        if (free) {
            properties.setProperty(MODE, MODE_FREE);
            int freeCount = freeCount(config);
            properties.setProperty(FREE_COUNT, Integer.toString(freeCount));
            for (int i = 0; i < freeCount; i++) {
                Node element = config.getChild(FREE_ELEMENT + i);
                elementToFile(properties, FREE_ELEMENT + i, element);
                copyToFile(properties, FREE_ELEMENT + i + ".", element, BOUNDS_PROPERTIES);
            }
        }
        // Validate before opening (and potentially truncating) the destination.
        decode(properties);
        try (Writer writer = Files.newBufferedWriter(file, StandardCharsets.UTF_8)) {
            properties.store(writer, "rusEFI gauge layout");
        }
    }

    private static void elementToFile(Properties properties, String name, Node element) {
        copyToFile(properties, name + ".", element, ELEMENT_PROPERTIES);
        for (String graph : new String[]{"top", "bottom"}) {
            Object node = element.getConfig().get(graph);
            if (node instanceof Node) {
                copyToFile(properties, name + "." + graph + ".", (Node) node, GRAPH_PROPERTIES);
            }
        }
    }

    static Node load(Path file) throws IOException {
        Properties properties = new Properties();
        try (Reader reader = Files.newBufferedReader(file, StandardCharsets.UTF_8)) {
            properties.load(reader);
        } catch (IllegalArgumentException e) {
            throw new IOException("Invalid gauge layout file", e);
        }
        return decode(properties);
    }

    private static Node decode(Properties properties) throws IOException {
        String version = properties.getProperty("version");
        if (!"rusefi-gauges".equals(properties.getProperty("format"))
            || !("1".equals(version) || "2".equals(version))) {
            throw new IOException("Unsupported gauge layout format or version");
        }
        int rows = integer(properties, ROWS, 1, SizeSelectorPanel.HEIGHT);
        int columns = integer(properties, COLUMNS, 1, SizeSelectorPanel.WIDTH);
        Node layout = new Node();
        layout.setProperty(ROWS, rows);
        layout.setProperty(COLUMNS, columns);
        for (int i = 0; i < rows * columns; i++) {
            elementFromFile(properties, GRID_ELEMENT + i, layout.getChild(GRID_ELEMENT + i));
        }
        if ("2".equals(version)) {
            String mode = properties.getProperty(MODE);
            if (!MODE_GRID.equals(mode) && !MODE_FREE.equals(mode)) {
                throw new IOException("Invalid " + MODE + ": expected " + MODE_GRID + " or " + MODE_FREE);
            }
            layout.setProperty(MODE, mode);
            int count = integer(properties, FREE_COUNT, 0, MAX_FREE_GAUGES);
            layout.setProperty(FREE_COUNT, count);
            for (int i = 0; i < count; i++) {
                Node element = layout.getChild(FREE_ELEMENT + i);
                elementFromFile(properties, FREE_ELEMENT + i, element);
                setBounds(element, boundsFromFile(properties, FREE_ELEMENT + i + "."));
            }
        }
        return layout;
    }

    private static void elementFromFile(Properties properties, String name, Node element) throws IOException {
        String prefix = name + ".";
        String gauge = properties.getProperty(prefix + "gauge");
        if (gauge == null || gauge.trim().isEmpty()) {
            throw new IOException("Missing gauge selection for " + name);
        }
        element.setProperty("gauge", gauge);
        copyFromFile(properties, prefix, element, "type", "display");
        copyFromFile(properties, prefix, element, GaugeScaleOverrides.KEYS);
        GaugeScaleOverrides scale = GaugeScaleOverrides.read(element);
        if (scale.getMin() != null && scale.getMax() != null && !(scale.getMin() < scale.getMax())) {
            throw new IOException("Invalid scale for " + name + ": min must be below max");
        }
        for (String graph : new String[]{"top", "bottom"}) {
            copyFromFile(properties, prefix + graph + ".", element.getChild(graph), GRAPH_PROPERTIES);
        }
    }

    private static Rectangle2D.Double boundsFromFile(Properties properties, String prefix) throws IOException {
        double x = fraction(properties, prefix + X);
        double y = fraction(properties, prefix + Y);
        double width = fraction(properties, prefix + WIDTH);
        double height = fraction(properties, prefix + HEIGHT);
        if (width < MIN_FREE_SIZE || height < MIN_FREE_SIZE
            || x + width > 1 + EPSILON || y + height > 1 + EPSILON) {
            throw new IOException("Invalid position or size for " + prefix + "gauge");
        }
        return new Rectangle2D.Double(x, y, width, height);
    }

    private static double fraction(Properties properties, String key) throws IOException {
        try {
            double value = Double.parseDouble(properties.getProperty(key));
            if (value >= 0 && value <= 1) {
                return value;
            }
        } catch (NumberFormatException | NullPointerException ignored) {
            // Report missing, non-numeric and out-of-range fractions consistently.
        }
        throw new IOException("Invalid " + key + ": expected 0 to 1");
    }

    private static int integer(Properties properties, String key, int min, int max) throws IOException {
        try {
            int value = Integer.parseInt(properties.getProperty(key));
            if (value >= min && value <= max) {
                return value;
            }
        } catch (NumberFormatException ignored) {
            // Report missing, non-integer and out-of-range values consistently.
        }
        throw new IOException("Invalid " + key + ": expected " + min + " to " + max);
    }

    private static void copyToFile(Properties target, String prefix, Node source, String... keys) {
        for (String key : keys) {
            String value = source.getProperty(key, null);
            if (value != null) {
                target.setProperty(prefix + key, value);
            }
        }
    }

    private static void copyFromFile(Properties source, String prefix, Node target, String... keys) throws IOException {
        for (String key : keys) {
            String value = source.getProperty(prefix + key);
            if (value == null) {
                continue;
            }
            boolean valid = true;
            if (key.equals("type") || key.equals("auto_scale")) {
                valid = value.equals("true") || value.equals("false");
            } else if (key.equals("period")) {
                valid = value.equals(Integer.toString(SensorLiveGraph.ChangePeriod.lookup(value).getMs()));
            } else if (key.equals("lower") || key.equals("upper")) {
                try {
                    valid = Double.isFinite(Double.parseDouble(value));
                } catch (NumberFormatException e) {
                    valid = false;
                }
            } else if (key.equals("display")) {
                valid = GaugeDisplayKind.lookup(value) != null;
            } else if (Arrays.asList(GaugeScaleOverrides.KEYS).contains(key)) {
                valid = GaugeScaleOverrides.isValid(key, value);
            } else if (key.equals("sensor")) {
                valid = !value.trim().isEmpty();
            }
            if (!valid) {
                throw new IOException("Invalid layout setting: " + prefix + key);
            }
            target.setProperty(key, value);
        }
    }
}
