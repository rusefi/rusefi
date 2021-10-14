package com.rusefi;

import com.rusefi.enum_reader.Value;
import com.rusefi.util.SystemOut;

import org.jetbrains.annotations.NotNull;

import java.io.*;
import java.util.*;

public class EnumsReader {
    private final Map<String, Value> currentValues = new TreeMap<>();

    private final Map<String, Map<String, Value>> enums = new TreeMap<>();

    @NotNull
    static List<Value> getSortedByOrder(Map<String, Value> brain_pin_e) {
        Set<Value> byOrdinal = new TreeSet<>(Comparator.comparingInt(Value::getIntValue));
        byOrdinal.addAll(brain_pin_e.values());
        return new ArrayList<>(byOrdinal);
    }

    public Map<String /*enum name*/, Map<String/*enum member*/, Value>> getEnums() {
        return enums;
    }

    public EnumsReader read(String path, String fileName) throws IOException {
        return read(new FileReader(path + File.separator + fileName));
    }

    public EnumsReader read(Reader in) throws IOException {
        boolean isInsideEnum = false;
        BufferedReader reader = new BufferedReader(in);
        String line;

        boolean withAutoValue = false;

        while ((line = reader.readLine()) != null) {
            line = removeSpaces(line);

            line = line.replaceAll("//.+", "");
            if (line.startsWith("typedefenum{") || line.startsWith("typedefenum__attribute__")) {
                SystemOut.println("  EnumsReader: Entering enum");
                currentValues.clear();
                withAutoValue = false;
                isInsideEnum = true;
            } else if (line.startsWith("}") && line.endsWith(";")) {
                isInsideEnum = false;
                line = line.substring(1, line.length() - 1);
                SystemOut.println("  EnumsReader: Ending enum " + line + " found " + currentValues.size() + " values");
                if (withAutoValue)
                    validateValues(currentValues);

                enums.put(line, new TreeMap<>(currentValues));
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
        return this;
    }

    private void validateValues(Map<String, Value> currentValues) {
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
}
