package com.rusefi.core;

import com.devexperts.logging.Logging;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.nio.charset.StandardCharsets;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Names of firmware ObdCode values, as reported by the lastErrorCode and recentErrorCode[] output channels.
 * <p>
 * The firmware header obd_error_codes.h is bundled as a resource by models/build.gradle and parsed at
 * runtime, so the names cannot drift from the firmware enum.
 */
public class ObdCodeDictionary {
    private static final Logging log = getLogging(ObdCodeDictionary.class);
    static final String RESOURCE = "/obd/obd_error_codes.h";
    /**
     * Values below this are standard SAE P-codes, rusEFI-specific codes start here
     */
    public static final int FIRST_CUSTOM_CODE = 6000;

    private static final Pattern ENTRY = Pattern.compile("^\\s*(\\w+)\\s*=\\s*(\\d+)\\s*,?\\s*(?://\\s*(.*))?$");

    private static volatile ObdCodeDictionary instance;

    private final Map<Integer, Entry> entries;

    public static class Entry {
        public final int code;
        public final String name;
        @Nullable
        public final String comment;

        Entry(int code, String name, @Nullable String comment) {
            this.code = code;
            this.name = name;
            this.comment = comment;
        }
    }

    ObdCodeDictionary(Map<Integer, Entry> entries) {
        this.entries = Collections.unmodifiableMap(entries);
    }

    @NotNull
    public static ObdCodeDictionary getInstance() {
        ObdCodeDictionary result = instance;
        if (result == null) {
            result = loadResource();
            instance = result;
        }
        return result;
    }

    private static ObdCodeDictionary loadResource() {
        InputStream stream = ObdCodeDictionary.class.getResourceAsStream(RESOURCE);
        if (stream == null) {
            log.warn("ObdCode names not available: missing " + RESOURCE);
            return new ObdCodeDictionary(new HashMap<>());
        }
        try (Reader reader = new InputStreamReader(stream, StandardCharsets.UTF_8)) {
            return parse(reader);
        } catch (IOException e) {
            log.error("Error reading " + RESOURCE, e);
            return new ObdCodeDictionary(new HashMap<>());
        }
    }

    /**
     * Reads "Name = value," entries of the enum. The header has several names for some values,
     * the first one wins.
     */
    static ObdCodeDictionary parse(Reader reader) throws IOException {
        Map<Integer, Entry> entries = new HashMap<>();
        BufferedReader br = new BufferedReader(reader);
        boolean insideEnum = false;
        String line;
        while ((line = br.readLine()) != null) {
            if (!insideEnum) {
                insideEnum = line.contains("enum class ObdCode");
                continue;
            }
            if (line.trim().startsWith("}")) {
                break;
            }
            Matcher m = ENTRY.matcher(line);
            if (!m.matches()) {
                continue;
            }
            int code = Integer.parseInt(m.group(2));
            String comment = m.group(3) == null ? null : m.group(3).trim();
            entries.putIfAbsent(code, new Entry(code, m.group(1), comment));
        }
        return new ObdCodeDictionary(entries);
    }

    @Nullable
    public Entry get(int code) {
        return entries.get(code);
    }

    public int size() {
        return entries.size();
    }

    public static boolean isCustom(int code) {
        return code >= FIRST_CUSTOM_CODE;
    }

    /**
     * @return "P0335" for standard codes, plain number for rusEFI-specific ones
     */
    public static String formatCode(int code) {
        if (isCustom(code)) {
            return Integer.toString(code);
        }
        return String.format("P%04d", code);
    }

    /**
     * @return human readable name, for instance "Crankshaft Position Sensor A Circuit Malfunction"
     */
    public String getDescription(int code) {
        Entry entry = get(code);
        if (entry == null) {
            return "Unknown code " + code;
        }
        String name = entry.name;
        if (name.startsWith("OBD_")) {
            name = name.substring("OBD_".length());
        } else if (name.startsWith("CUSTOM_")) {
            name = name.substring("CUSTOM_".length());
        }
        return name.replace('_', ' ');
    }
}
