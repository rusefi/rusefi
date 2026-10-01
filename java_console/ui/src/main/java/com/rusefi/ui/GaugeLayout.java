package com.rusefi.ui;

import com.rusefi.core.preferences.storage.Node;

import java.io.IOException;
import java.io.Reader;
import java.io.Writer;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Properties;

/** Portable dashboard settings, independent of the rest of Console preferences. */
final class GaugeLayout {
    static final String ROWS = "gauges_rows";
    static final String COLUMNS = "gauges_cols";
    private static final String[] GRAPH_PROPERTIES = {"sensor", "period", "auto_scale", "lower", "upper"};

    private GaugeLayout() {
    }

    static void save(Path file, Node config) throws IOException {
        Properties properties = new Properties();
        properties.setProperty("format", "rusefi-gauges");
        properties.setProperty("version", "1");
        copyToFile(properties, "", config, ROWS, COLUMNS);
        int count = config.getIntProperty(ROWS, 0) * config.getIntProperty(COLUMNS, 0);
        for (int i = 0; i < count; i++) {
            String name = "element_" + i;
            Node element = config.getChild(name);
            copyToFile(properties, name + ".", element, "gauge", "type");
            for (String graph : new String[]{"top", "bottom"}) {
                Object node = element.getConfig().get(graph);
                if (node instanceof Node) {
                    copyToFile(properties, name + "." + graph + ".", (Node) node, GRAPH_PROPERTIES);
                }
            }
        }
        // Validate before opening (and potentially truncating) the destination.
        decode(properties);
        try (Writer writer = Files.newBufferedWriter(file, StandardCharsets.UTF_8)) {
            properties.store(writer, "rusEFI gauge layout");
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
        if (!"rusefi-gauges".equals(properties.getProperty("format"))
            || !"1".equals(properties.getProperty("version"))) {
            throw new IOException("Unsupported gauge layout format or version");
        }
        int rows = dimension(properties, ROWS, SizeSelectorPanel.HEIGHT);
        int columns = dimension(properties, COLUMNS, SizeSelectorPanel.WIDTH);
        Node layout = new Node();
        layout.setProperty(ROWS, rows);
        layout.setProperty(COLUMNS, columns);
        for (int i = 0; i < rows * columns; i++) {
            String prefix = "element_" + i + ".";
            Node element = layout.getChild("element_" + i);
            String gauge = properties.getProperty(prefix + "gauge");
            if (gauge == null || gauge.trim().isEmpty()) {
                throw new IOException("Missing gauge selection for element " + (i + 1));
            }
            element.setProperty("gauge", gauge);
            copyFromFile(properties, prefix, element, "type");
            for (String graph : new String[]{"top", "bottom"}) {
                copyFromFile(properties, prefix + graph + ".", element.getChild(graph), GRAPH_PROPERTIES);
            }
        }
        return layout;
    }

    private static int dimension(Properties properties, String key, int max) throws IOException {
        try {
            int value = Integer.parseInt(properties.getProperty(key));
            if (value >= 1 && value <= max) {
                return value;
            }
        } catch (NumberFormatException ignored) {
            // Report missing, non-integer and out-of-range dimensions consistently.
        }
        throw new IOException("Invalid " + key + ": expected 1 to " + max);
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
