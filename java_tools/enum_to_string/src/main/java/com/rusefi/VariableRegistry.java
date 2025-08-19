package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.enum_reader.Value;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import java.io.*;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.function.Function;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static com.devexperts.logging.Logging.getLogging;

/**
 * 3/30/2015
 */
public class VariableRegistry {
    public static final String AUTO_ENUM_SUFFIX = "_auto_enum";
    private static final Logging log = getLogging(VariableRegistry.class);

    public static final String _16_HEX_SUFFIX = "_16_hex";
    public static final String _HEX_SUFFIX = "_hex";
    public static final String CHAR_SUFFIX = "_char";
    public static final String KEY_VALUE_FORMAT_ENUM = "_enum";
    public static final String ARRAY_FORMAT_ENUM = "_fullenum";
    public static final char MULT_TOKEN = '*';
    public static final String DEFINE = "#define";
    private static final String HEX_PREFIX = "0x";
    private static final String TEMPLATE_QUITE_OPEN_TAG = "@#";
    private final TreeMap<String, String> data = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    // todo: smarter regex! See TsWriter.VAR which is a bit better but still not perfect
    // todo: https://github.com/rusefi/rusefi/issues/3053 ?
    public final static String TEMPLATE_TAG = "@@";
    private final Pattern VAR = Pattern.compile("(" + TEMPLATE_TAG + "(.*?)" + TEMPLATE_TAG + ")");
    private final Pattern VAR_REMOVE_QUOTE = Pattern.compile("(" + TEMPLATE_QUITE_OPEN_TAG + "(.*?)#@)");

    public final Map<String, Integer> intValues = new HashMap<>();

    private final Map<String, String> cAllDefinitions = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);
    // todo: move this logic to JavaFieldsConsumer since that's the consumer?
    private final Map<String, String> javaDefinitions = new TreeMap<>(String.CASE_INSENSITIVE_ORDER);

    public static String unquote(String token) {
        return VariableRegistry.unquote(token, '\"');
    }

    @NotNull
    public static String unquote(String token, char quoteSymbol) {
        int length = token.length();
        if (length < 2)
            return token;
        if (token.charAt(0) == quoteSymbol && token.charAt(token.length() - 1) == quoteSymbol)
            return token.substring(1, length - 1);
        return token;
    }

    static boolean looksLikeDefineLine(String line) {
        return ToolUtil.startsWithToken(line, DEFINE);
    }

    public void readPrependValues(String prependFile, boolean ignoreUnexpectedLined) {
        File file = new File(IoUtil3.prependIfNotAbsolute(RootHolder.ROOT, prependFile));
        try {
            readPrependValues(new FileReader(file), ignoreUnexpectedLined);
        } catch (Throwable e) {
            throw new IllegalStateException("While reading from " + file.getAbsolutePath(), e);
        }
    }

    public void readPrependValues(Reader fileReader, boolean ignoreUnexpectedLined) throws IOException {
        BufferedReader definitionReader = new BufferedReader(fileReader);
        String line;
        while ((line = definitionReader.readLine()) != null) {
            line = ToolUtil.trimLine(line);
            /**
             * we should ignore empty lines and comments
             */
            if (ToolUtil.isEmptyDefinitionLine(line))
                continue;
            if (ToolUtil.startsWithToken(line, DEFINE)) {
                processDefine(line.substring(DEFINE.length()).trim());
            } else if (!ignoreUnexpectedLined) {
                throw new IllegalStateException("Unexpected line while prepending: [" + line + "]");
            }
        }
    }

    void processLine(String line) {
        processDefine(line.substring(VariableRegistry.DEFINE.length()).trim());
    }

    void processDefine(String line) {
        int index = line.indexOf(' ');
        String name;
        if (index == -1) {
            name = line;
            line = "";
        } else {
            name = line.substring(0, index);
            line = line.substring(index).trim();
        }
        if (isNumeric(line)) {
            int v = Integer.parseInt(line);
            register(name, v);
        } else {
            if (line.contains(" ") && !isQuoted(line, '\"') && !isQuoted(line, '\'')) {
                throw new IllegalStateException("Unexpected space in unquoted " + line);
            }

            register(name, line);
        }
    }

    /**
     * @return value>name map for specified enum name.
     */
    @Nullable
    private TreeMap<Integer, String> resolveEnumValues(@NotNull EnumsReader.EnumState stringValueMap) {
        TreeMap<Integer, String> valueNameById = new TreeMap<>();

        for (Value value : stringValueMap.values()) {

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

    private static void appendCommaIfNeeded(StringBuilder sb) {
        if (sb.length() > 0)
            sb.append(",");
    }

    public String getEnumOptionsForTunerStudio(EnumsReader enumsReader, String enumName) {
        EnumsReader.EnumState stringValueMap = enumsReader.getEnums().get(enumName);
        if (stringValueMap == null)
            return null;

        TreeMap<Integer, String> valueNameById = resolveEnumValues(stringValueMap);
        if (valueNameById == null)
            return null;

        return getHumanSortedTsKeyValueString(valueNameById);
    }

    public static String quote(String string) {
        return "\"" + string + "\"";
    }

    @NotNull
    public static String getHumanSortedTsKeyValueString(Map<Integer, String> valueNameById) {
        TreeMap<Integer, String> humanDropDownSorted = new TreeMap<>((o1, o2) -> {
            if (o1.intValue() == o2)
                return 0;
            if (o1.intValue() == 0)
                return -1; // "None" always go first
            if (o2.intValue() == 0)
                return 1;
            return valueNameById.get(o1).compareTo(valueNameById.get(o2));
        });
        humanDropDownSorted.putAll(valueNameById);

        StringBuilder sb = new StringBuilder();
        for (Map.Entry<Integer, String> e : humanDropDownSorted.entrySet()) {
            appendCommaIfNeeded(sb);
            sb.append(e.getKey() + "=" + quote(e.getValue()));
        }
        return sb.toString();
    }

    /**
     * This method replaces variables references like @@var@@ with actual values
     * An exception is thrown if we do not have such variable
     *
     * @return string with variable values inlined
     */
    public String applyVariables(String line) {
        if (line == null)
            return null;
        line = process(line, VAR, key -> {
            if (!data.containsKey(key))
                throw new IllegalStateException("No such variable: [" + key + "]");
            return data.get(key);
        });
        line = process(line, VAR_REMOVE_QUOTE, key -> {
            if (!data.containsKey(key))
                throw new IllegalStateException("No such variable: [" + key + "]");
            String unquotedDouble = unquote(data.get(key));
            return unquote(unquotedDouble, '\'');
        });
        return line;
    }

    @NotNull
    private String process(String line, Pattern pattern, Function<String, String> function) {
        Matcher m;
        while ((m = pattern.matcher(line)).find()) {
            if (m.groupCount() < 2)
                throw new IllegalStateException("Something broken in: [" + line + "]");
            String key = m.group(2);
            String newValue = function.apply(key);
            //log.info("Replacing " + key + " with " + newValue + " in " + line + " using " + pattern.pattern());
            newValue = newValue.replace("\\", "\\\\");
            line = m.replaceFirst(Matcher.quoteReplacement(newValue));
        }
        return line;
    }

    public void register(String var, String rawValue) {
        try {
            String value = doRegister(var, rawValue);
            if (value == null)
                return;
            tryToRegisterAsInteger(var, value);
        } catch (RuntimeException e) {
            throw new IllegalStateException("While [" + var + "][" + rawValue + "]", e);
        }
    }

    /**
     * @return input value with template variables applied
     */
    @Nullable
    private String doRegister(String var, String value) {
        if (data.containsKey(var)) {
            if (log.debugEnabled())
                log.debug("Not redefining " + var);
            return null;
        }
        value = applyVariables(value);
        int multPosition = value.indexOf(MULT_TOKEN);
        if (!value.contains("\n") && !isQuoted(value, '"') && multPosition != -1) {
            Integer first = Integer.valueOf(value.substring(0, multPosition).trim());
            Integer second = Integer.valueOf(value.substring(multPosition + 1).trim());
            value = String.valueOf(first * second);
        }

        if (log.debugEnabled())
            log.debug("Registering " + var + " as " + value);
        data.put(var, value);

        if (!value.contains("\n")) {
            // multi-lines are not supported in C headers
            if (!var.endsWith(_16_HEX_SUFFIX) && !var.endsWith(_HEX_SUFFIX)) {
                cAllDefinitions.put(var, "#define " + var + " " + value + ToolUtil.EOL);
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
            javaDefinitions.put(var, "\tpublic static final int " + var + " = " + value + ";" + ToolUtil.EOL);
            return;
        }

        try {
            int intValue = Integer.parseInt(value);
            if (log.debugEnabled())
                log.debug("key [" + var + "] value: " + intValue);
            intValues.put(var, intValue);
            if (!var.endsWith(_HEX_SUFFIX)) {
                javaDefinitions.put(var, "\tpublic static final int " + var + " = " + intValue + ";" + ToolUtil.EOL);
            }
        } catch (NumberFormatException e) {
            //SystemOut.println("Not an integer: " + value);

            if (!var.trim().endsWith(KEY_VALUE_FORMAT_ENUM) &&
                !var.trim().endsWith(ARRAY_FORMAT_ENUM)) {
                if (isQuoted(value, '"')) {
                    // quoted and not with enum suffix means plain string define statement
                    javaDefinitions.put(var, "\tpublic static final String " + var + " = " + value + ";" + ToolUtil.EOL);
                } else if (isQuoted(value, '\'')) {
                    // quoted and not with enum suffix means plain string define statement
                    javaDefinitions.put(var, "\tpublic static final char " + var + " = " + value + ";" + ToolUtil.EOL);
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
        return simpleIsQuoted(value, quote);
    }

    private static boolean simpleIsQuoted(String value, char quote) {
        return value.charAt(0) == quote && value.charAt(value.length() - 1) == quote;
    }

    /**
     * Registers decimal and hex versions of this integer variable
     */
    public void register(String name, int value) {
        register(name, Integer.toString(value));
        registerHex(name, value);
    }

    public void registerHex(String name, int value) {
        register(name + _HEX_SUFFIX, Integer.toString(value, 16));
        String _16_hex = String.format("\\x%02x\\x%02x", (value >> 8) & 0xFF, value & 0xFF);
        register(name + _16_HEX_SUFFIX, _16_hex);
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

    public Set<String> getKeys() {
        return data.keySet();
    }

    @Nullable
    public String get(Object key) {
        return data.get(key);
    }
}
