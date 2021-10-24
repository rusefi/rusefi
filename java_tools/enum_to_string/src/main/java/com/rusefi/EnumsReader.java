package com.rusefi;

import com.rusefi.enum_reader.Value;
import com.rusefi.util.SystemOut;

import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.util.*;

public class EnumsReader {
    private static final String ENUMCLASS_PREFIX = "enumclass";

    protected final Map<String, EnumState> enums = new TreeMap<>();

    /**
     * core implementation sorts by name, we need special considerations to sort by value
     */
    @NotNull
    static List<Value> getSortedByOrder(VariableRegistry registry, Map<String, Value> brain_pin_e) {
        Set<Value> byOrdinal = new TreeSet<>(Comparator.comparingInt(value -> value.getIntValueMaybeResolve(registry)));
        byOrdinal.addAll(brain_pin_e.values());
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
                SystemOut.println("  EnumsReader: Entering legacy enum");
                currentValues.clear();
                withAutoValue = false;
                isInsideEnum = true;
                enumName = null;
                isEnumClass = false;
            } else if (line.startsWith(ENUMCLASS_PREFIX)) {
                SystemOut.println("  EnumsReader: Entering fancy enum class");
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
                SystemOut.println("  EnumsReader: Ending enum " + enumName + " found " + currentValues.size() + " values");
                if (withAutoValue)
                    validateValues(currentValues);

                enums.put(enumName, new EnumState(currentValues, enumName, isEnumClass));
            } else {
                if (isInsideEnum) {
                    if (isKeyValueLine(line)) {
                        line = line.replace(",", "");
                        String value = "";
                        int index = line.indexOf('=');
                        if (index != -1) {
                            value = line.substring(index + 1);
                            line = line.substring(0, index);
                        } else {
                            value = Integer.toString(currentValues.size());
                            withAutoValue = true;
                        }
                        SystemOut.println("    EnumsReader: Line " + line);
                        currentValues.put(line, new Value(line, value));
                    } else {
                        SystemOut.println("    EnumsReader: Skipping Line " + line);
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
        public Map<String, Value> values;
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
