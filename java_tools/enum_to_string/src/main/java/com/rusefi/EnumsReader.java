package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.enum_reader.Value;

import org.jetbrains.annotations.NotNull;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

import static com.devexperts.logging.Logging.getLogging;

public class EnumsReader {
    private static final Logging log = getLogging(EnumsReader.class);
    private static final String ENUMCLASS_PREFIX = "enumclass";

    protected final Map<String, EnumState> enums = new TreeMap<>();

    /**
     * core implementation sorts by name, we need special considerations to sort by value
     */
    @NotNull
    static List<Value> getSortedByOrder(VariableRegistry registry, Map<String, Value> enumValues) {
        Set<Integer> ids = new TreeSet<>();
        for (Value value : enumValues.values()) {
            boolean isUniqueId = ids.add(value.getIntValueMaybeResolve(registry));
            if (!isUniqueId)
                throw new IllegalArgumentException("Ordinal duplication? " + value);
        }

        Set<Value> byOrdinal = new TreeSet<>(Comparator.comparingInt(value -> value.getIntValueMaybeResolve(registry)));
        byOrdinal.addAll(enumValues.values());
        return new ArrayList<>(byOrdinal);
    }

    public Map<String /*enum name*/, EnumState> getEnums() {
        return enums;
    }

    public EnumsReader read(Reader in) throws IOException {
        enums.putAll(readStatic(in));
        return this;
    }

    public static Map<String, EnumState> readStatic(Reader in) throws IOException {
        boolean isInsideEnum = false;
        BufferedReader reader = new BufferedReader(in);
        String line;
        String enumName = null;
        boolean isEnumClass = false;
        Map<String, Value> currentValues = new TreeMap<>();
        Map<String, EnumState> enums = new TreeMap<>();

        boolean withAutoValue = false;

        while ((line = reader.readLine()) != null) {
            line = removeSpaces(line);

            line = line.replaceAll("//.+", "");
            if (line.startsWith("typedefenum{") || line.startsWith("typedefenum__attribute__")) {
                if (log.debugEnabled())
                    log.debug("  EnumsReader: Entering legacy enum");
                currentValues.clear();
                withAutoValue = false;
                isInsideEnum = true;
                enumName = null;
                isEnumClass = false;
            } else if (line.startsWith(ENUMCLASS_PREFIX)) {
                if (log.debugEnabled())
                    log.debug("  EnumsReader: Entering fancy enum class");
                currentValues.clear();
                withAutoValue = false;
                isInsideEnum = true;
                isEnumClass = true;
                int colonIndex = line.indexOf(":");
                if (colonIndex == -1)
                    throw new IllegalStateException("color and Type not located in " + line);
                enumName = line.substring(ENUMCLASS_PREFIX.length(), colonIndex);
            } else if (line.startsWith("}") && line.endsWith(";")) {
                isInsideEnum = false;
                if (enumName == null)
                    enumName = line.substring(1, line.length() - 1);
                if (log.debugEnabled())
                    log.debug("  EnumsReader: Ending enum " + enumName + " found " + currentValues.size() + " values");
                if (withAutoValue)
                    validateValues(currentValues);

                enums.put(enumName, new EnumState(currentValues, enumName, isEnumClass));
            } else {
                if (isInsideEnum) {
                    if (isKeyValueLine(line)) {
                        line = line.replace(",", "");
                        String value;
                        int index = line.indexOf('=');
                        if (index != -1) {
                            value = line.substring(index + 1);
                            line = line.substring(0, index);
                        } else {
                            value = Integer.toString(currentValues.size());
                            withAutoValue = true;
                        }
                        if (log.debugEnabled())
                            log.debug("    EnumsReader: Line " + line);
                        currentValues.put(line, new Value(line, value));
                    } else {
                        if (log.debugEnabled())
                            log.debug("    EnumsReader: Skipping Line " + line);
                    }
                }
            }
        }
        return enums;
    }

    private static void validateValues(Map<String, Value> currentValues) {
        for (Map.Entry<String, Value> entry : currentValues.entrySet()) {
            int v = entry.getValue().getIntValue();
            if (v < 0 || v >= currentValues.size())
                throw new IllegalStateException("Unexpected " + entry);
        }
    }

    private static String removeSpaces(String line) {
        return line.replaceAll("\\s+", "");
    }

    static boolean isKeyValueLine(String line) {
        return removeSpaces(line).matches("[a-zA-Z_$][a-zA-Z\\d_$]*(=(0x[0-9a-fA-F]+|(-)?[0-9]+|([-a-zA-Z\\d_])*))*,?");
    }

    public static class EnumState {
        public final Map<String, Value> values;
        public final String enumName;
        public final boolean isEnumClass;

        public EnumState(Map<String, Value> currentValues, String enumName, boolean isEnumClass) {
            values = new TreeMap<>(currentValues);
            this.enumName = enumName;
            this.isEnumClass = isEnumClass;
        }

        public Collection<Value> values() {
            return values.values();
        }

        public Iterable<? extends Map.Entry<String, Value>> entrySet() {
            return values.entrySet();
        }
    }
}
