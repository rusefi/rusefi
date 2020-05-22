package com.rusefi;

import com.rusefi.util.LazyFile;
import com.rusefi.util.SystemOut;

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
    private static final String _16_HEX_SUFFIX = "_16_hex";
    private static final String _HEX_SUFFIX = "_hex";
    private TreeMap<String, String> data = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    public static final VariableRegistry INSTANCE = new VariableRegistry();

    private final Pattern VAR = Pattern.compile("(@@(.*?)@@)");

    public Map<String, Integer> intValues = new HashMap<>();

    private final Map<String, String> cAllDefinitions = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final Map<String, String> javaDefinitions = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    private VariableRegistry() {
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
            String key = m.group(2);


            //     key =

            if (!data.containsKey(key))
                throw new IllegalStateException("No such variable: " + key);
            String s = data.get(key);
            line = m.replaceFirst(s);
        }
        return line;
    }

    public void register(String var, String value) {
        if (data.containsKey(var)) {
            SystemOut.println("Not redefining " + var);
            return;
        }
        value = applyVariables(value);
        int multPosition = value.indexOf(MULT_TOKEN);
        if (multPosition != -1) {
            Integer first = Integer.valueOf(value.substring(0, multPosition));
            Integer second = Integer.valueOf(value.substring(multPosition + 1));
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
        tryToRegisterAsInteger(var, value);
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
        try {
            int intValue = Integer.parseInt(value);
            SystemOut.println("key [" + var + "] value: " + intValue);
            intValues.put(var, intValue);
            if (!var.endsWith(_HEX_SUFFIX)) {
                javaDefinitions.put(var, "\tpublic static final int " + var + " = " + intValue + ";" + EOL);
            }
        } catch (NumberFormatException e) {
            SystemOut.println("Not an integer: " + value);

            if (isQuoted(value) && !var.trim().endsWith(ConfigField.ENUM_SUFFIX)) {
                // quoted and not with enum suffix means plain string define statement
                javaDefinitions.put(var, "\tpublic static final String " + var + " = " + value + ";" + EOL);
            }
        }
    }

    private boolean isQuoted(String value) {
        if (value == null)
            return false;
        value = value.trim();
        if (value.isEmpty())
            return false;
        return value.charAt(0) == '"' && value.charAt(value.length() - 1) == '"';
    }

    /**
     * Registers decimal and hex versions of this integer variable
     */
    public void register(String name, int value) {
        register(name, Integer.toString(value));
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
        StringBuffer result = new StringBuffer();
        for (String value : javaDefinitions.values())
            result.append(value);
        return result.toString();
    }

    public void clear() {
        data.clear();
        cAllDefinitions.clear();
        javaDefinitions.clear();
    }

    public void put(String key, String value) {
        data.put(key, value);
    }

    public String get(Object key) {
        return data.get(key);
    }
}
