package com.rusefi;

import com.rusefi.util.LazyFile;
import com.rusefi.util.SystemOut;

import java.io.IOException;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static com.rusefi.ConfigDefinition.EOL;

/**
 * 3/30/2015
 */
public class VariableRegistry extends TreeMap<String, String> {
    public static final VariableRegistry INSTANCE = new VariableRegistry();

    private final Pattern VAR = Pattern.compile("(@@(.*?)@@)");

    public Map<String, Integer> intValues = new HashMap<>();

    private final Map<String, String> cAllDefinitions = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    private final Map<String, String> javaDefinitions = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    private VariableRegistry() {
        super(String.CASE_INSENSITIVE_ORDER);
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

            if (!containsKey(key))
                throw new IllegalStateException("No such variable: " + key);
            String s = get(key);
            line = m.replaceFirst(s);
        }
        return line;
    }

    public void register(String var, String value) {
        if (containsKey(var)) {
            SystemOut.println("Not redefining " + var);
            return;
        }
        SystemOut.println("Registering " + var + " as " + value);
        put(var, value);

        cAllDefinitions.put(var, "#define " + var + " " + value + EOL);
        tryToRegisterAsInteger(var, value);
    }

    @SuppressWarnings("StringConcatenationInsideStringBufferAppend")
    private void tryToRegisterAsInteger(String var, String value) {
        try {
            int intValue = Integer.parseInt(value);
            SystemOut.println("key [" + var + "] value: " + intValue);
            intValues.put(var, intValue);
            javaDefinitions.put(var, "\tpublic static final int " + var + " = " + intValue + ";" + EOL);
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
        register(name + "_hex", Integer.toString(value, 16));
    }

    public void writeDefinesToFile(String fileName) throws IOException {

        SystemOut.println("Writing to " + fileName);
        LazyFile cHeader = new LazyFile(fileName);

        cHeader.write("//\n// " + ConfigDefinition.GENERATED_AUTOMATICALLY_TAG + ConfigDefinition.definitionInputFile + "\n//\n\n");
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
}
