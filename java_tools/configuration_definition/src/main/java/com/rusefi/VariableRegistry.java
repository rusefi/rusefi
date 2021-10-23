package com.rusefi;

import com.rusefi.enum_reader.Value;
import com.rusefi.util.LazyFile;
import com.rusefi.util.SystemOut;
import org.jetbrains.annotations.Nullable;

import java.io.IOException;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static com.rusefi.ConfigDefinition.EOL;
import static com.rusefi.ReaderState.MULT_TOKEN;

/**
 * 3/30/2015
 */
public class VariableRegistry  {
    public static final String _16_HEX_SUFFIX = "_16_hex";
    public static final String _HEX_SUFFIX = "_hex";
    public static final String CHAR_SUFFIX = "_char";
    private static final String HEX_PREFIX = "0x";
    private final TreeMap<String, String> data = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    public static final VariableRegistry INSTANCE = new VariableRegistry();

    // todo: smarter regex! See TsWriter.VAR which is a bit better but still not perfect
    // todo: https://github.com/rusefi/rusefi/issues/3053 ?
    private final Pattern VAR = Pattern.compile("(@@(.*?)@@)");

    public Map<String, Integer> intValues = new HashMap<>();

    private final Map<String, String> cAllDefinitions = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final Map<String, String> javaDefinitions = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    public VariableRegistry() {
    }

    /**
     * @return value>name map for specified enum name.
     */
    @Nullable
    public TreeMap<Integer, String> resolveEnumValues(EnumsReader enumsReader, String enumName) {
        TreeMap<Integer, String> valueNameById = new TreeMap<>();

        EnumsReader.EnumState stringValueMap = enumsReader.getEnums().get(enumName);
        if (stringValueMap == null)
            return null;
        for (Value value : stringValueMap.values()) {
            if (value.getValue().contains("ENUM_32_BITS"))
                continue;

            if (isNumeric(value.getValue())) {
                valueNameById.put(value.getIntValue(), value.getName());
            } else {
                String valueFromRegistry = get(value.getValue());
                if (valueFromRegistry == null)
                    throw new IllegalStateException("No value for " + value);
                int intValue = Integer.parseInt(valueFromRegistry);
                valueNameById.put(intValue, value.getName());
            }
        }
        return valueNameById;
    }

    public String getEnumOptionsForTunerStudio(EnumsReader enumsReader, String enumName) {
        TreeMap<Integer, String> valueNameById = resolveEnumValues(enumsReader, enumName);
        if (valueNameById == null)
            return null;

        int maxValue = valueNameById.lastKey();

        StringBuilder sb = new StringBuilder();
        for (int i = 0; i <= maxValue; i++) {
            if (sb.length() > 0)
                sb.append(", ");

            String value = valueNameById.getOrDefault(i, "INVALID");
            sb.append("\"" + value + "\"");
        }
        return sb.toString();
    }

    /**
     * This method replaces variables references like @@var@@ with actual values
     * An exception is thrown if we do not have such variable
     * @return string with variable values inlined
     */
    public String applyVariables(String line) {
        if (line == null)
            return null;
        Matcher m;
        while ((m = VAR.matcher(line)).find()) {
            if (m.groupCount() < 2)
                throw new IllegalStateException("Something broken in: [" + line + "]");
            String key = m.group(2);
            if (!data.containsKey(key))
                throw new IllegalStateException("No such variable: [" + key + "]");
            String s = data.get(key);
            line = m.replaceFirst(s);
        }
        return line;
    }

    public void register(String var, String param) {
        String value = doRegister(var, param);
        if (value == null)
            return;
        tryToRegisterAsInteger(var, value);
    }

    @Nullable
    private String doRegister(String var, String value) {
        if (data.containsKey(var)) {
            SystemOut.println("Not redefining " + var);
            return null;
        }
        value = applyVariables(value);
        int multPosition = value.indexOf(MULT_TOKEN);
        if (!isQuoted(value, '"') && multPosition != -1) {
            Integer first = Integer.valueOf(value.substring(0, multPosition).trim());
            Integer second = Integer.valueOf(value.substring(multPosition + 1).trim());
            value = String.valueOf(first * second);
        }

        SystemOut.println("Registering " + var + " as " + value);
        data.put(var, value);

        if (!value.contains("\n")) {
            // multi-lines are not supported in C headers
            if (!var.endsWith(_16_HEX_SUFFIX) && !var.endsWith(_HEX_SUFFIX)) {
                cAllDefinitions.put(var, "#define " + var + " " + value + EOL);
            }
        }
        return value;
    }

    public static boolean isNumeric(String str) {
        try {
            Integer.parseInt(str);
            return true;
        } catch (NumberFormatException e) {
            return false;
        }
    }

    @SuppressWarnings("StringConcatenationInsideStringBufferAppend")
    private void tryToRegisterAsInteger(String var, String value) {
        if (value.trim().startsWith(HEX_PREFIX)) {
            int intValue = Integer.parseInt(value.trim().substring(HEX_PREFIX.length()), 16);
            intValues.put(var, intValue);
            javaDefinitions.put(var, "\tpublic static final int " + var + " = " + value + ";" + EOL);
            return;
        }

        try {
            int intValue = Integer.parseInt(value);
            SystemOut.println("key [" + var + "] value: " + intValue);
            intValues.put(var, intValue);
            if (!var.endsWith(_HEX_SUFFIX)) {
                javaDefinitions.put(var, "\tpublic static final int " + var + " = " + intValue + ";" + EOL);
            }
        } catch (NumberFormatException e) {
            SystemOut.println("Not an integer: " + value);

            if (!var.trim().endsWith(ConfigField.ENUM_SUFFIX)) {
                if (isQuoted(value, '"')) {
                    // quoted and not with enum suffix means plain string define statement
                    javaDefinitions.put(var, "\tpublic static final String " + var + " = " + value + ";" + EOL);
                } else if (isQuoted(value, '\'')) {
                    // quoted and not with enum suffix means plain string define statement
                    javaDefinitions.put(var, "\tpublic static final char " + var + " = " + value + ";" + EOL);
                    char charValue = value.charAt(1);
                    registerHex(var + CHAR_SUFFIX, charValue);
                    doRegister(var + CHAR_SUFFIX, Character.toString(charValue));
                }
            }
        }
    }

    public static boolean isQuoted(String value, char quote) {
        if (value == null)
            return false;
        value = value.trim();
        if (value.isEmpty())
            return false;
        return isQ(value, quote);
    }

    private static boolean isQ(String value, char quote) {
        return value.charAt(0) == quote && value.charAt(value.length() - 1) == quote;
    }

    /**
     * Registers decimal and hex versions of this integer variable
     */
    public void register(String name, int value) {
        register(name, Integer.toString(value));
        registerHex(name, value);
    }

    private void registerHex(String name, int value) {
        register(name + _HEX_SUFFIX, Integer.toString(value, 16));
        String _16_hex = String.format("\\\\x%02x\\\\x%02x", (value >> 8) & 0xFF, value & 0xFF);
        register(name + _16_HEX_SUFFIX, _16_hex);
    }

    public void writeDefinesToFile(String fileName) throws IOException {

        SystemOut.println("Writing to " + fileName);
        LazyFile cHeader = new LazyFile(fileName);

        cHeader.write("//\n// " + ConfigDefinition.getGeneratedAutomaticallyTag() + ConfigDefinition.definitionInputFile + "\n//\n\n");
        cHeader.write(getDefinesSection());
        cHeader.close();
    }

    public String getDefinesSection() {
        StringBuilder sb = new StringBuilder();
        for (String value : cAllDefinitions.values())
            sb.append(value);
        return sb.toString();
    }

    public String getJavaConstants() {
        StringBuilder result = new StringBuilder();
        for (String value : javaDefinitions.values())
            result.append(value);
        return result.toString();
    }

    public int size() {
        return data.size();
    }

    public void put(String key, String value) {
        data.put(key, value);
    }

    public String get(Object key) {
        return data.get(key);
    }
}
