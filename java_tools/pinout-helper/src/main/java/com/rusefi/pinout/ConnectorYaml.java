package com.rusefi.pinout;

import org.yaml.snakeyaml.Yaml;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

/**
 * One connector .yaml file: electrical pins plus the interactive-pinout 'info' section
 * (connector photo file and per-pin x/y image coordinates).
 */
public class ConnectorYaml {
    public static class PinEntry {
        public final String pin;
        public final String function;
        public final String type;
        public final String color;
        public final String tsName;

        PinEntry(String pin, String function, String type, String color, String tsName) {
            this.pin = pin;
            this.function = function;
            this.type = type;
            this.color = color;
            this.tsName = tsName;
        }
    }

    public static class Coord {
        public final int x;
        public final int y;

        Coord(int x, int y) {
            this.x = x;
            this.y = y;
        }
    }

    public final String name;
    public final File imageFile;
    public final List<PinEntry> pins = new ArrayList<>();
    public final Map<String, Coord> coords = new TreeMap<>();

    private ConnectorYaml(String name, File imageFile) {
        this.name = name;
        this.imageFile = imageFile;
    }

    private static String str(Map<String, Object> map, String key) {
        Object value = map.get(key);
        return value == null ? null : String.valueOf(value).trim();
    }

    @SuppressWarnings("unchecked")
    public static ConnectorYaml read(File yamlFile) throws IOException {
        Map<String, Object> doc;
        try (Reader reader = new FileReader(yamlFile)) {
            doc = new Yaml().load(reader);
        }
        if (doc == null || !(doc.get("pins") instanceof List)) {
            return null;
        }

        String name = yamlFile.getName().replaceFirst("\\.yaml$", "");
        File imageFile = null;
        Map<String, Object> info = doc.get("info") instanceof Map ? (Map<String, Object>) doc.get("info") : new HashMap<String, Object>();
        Map<String, Object> image = info.get("image") instanceof Map ? (Map<String, Object>) info.get("image") : null;
        if (image != null && image.get("file") != null) {
            imageFile = new File(yamlFile.getParentFile(), String.valueOf(image.get("file")));
        }

        ConnectorYaml result = new ConnectorYaml(name, imageFile);

        for (Object o : (List<Object>) doc.get("pins")) {
            if (!(o instanceof Map)) {
                continue;
            }
            Map<String, Object> pin = (Map<String, Object>) o;
            String id = str(pin, "pin");
            if (id == null) {
                continue;
            }
            result.pins.add(new PinEntry(id, str(pin, "function"), str(pin, "type"), str(pin, "color"), str(pin, "ts_name")));
        }

        if (info.get("pins") instanceof List) {
            for (Object o : (List<Object>) info.get("pins")) {
                if (!(o instanceof Map)) {
                    continue;
                }
                Map<String, Object> pin = (Map<String, Object>) o;
                String id = str(pin, "pin");
                Object x = pin.get("x");
                Object y = pin.get("y");
                if (id != null && x instanceof Number && y instanceof Number) {
                    result.coords.put(id, new Coord(((Number) x).intValue(), ((Number) y).intValue()));
                }
            }
        }
        return result;
    }

    public static List<ConnectorYaml> readFolder(File connectorsDir) throws IOException {
        File[] files = connectorsDir.listFiles((dir, fileName) -> fileName.endsWith(".yaml"));
        if (files == null) {
            throw new IOException("Not a folder: " + connectorsDir.getAbsolutePath());
        }
        java.util.Arrays.sort(files);
        List<ConnectorYaml> result = new ArrayList<>();
        for (File file : files) {
            ConnectorYaml connector = read(file);
            if (connector != null && !connector.pins.isEmpty()) {
                result.add(connector);
            }
        }
        return result;
    }
}
