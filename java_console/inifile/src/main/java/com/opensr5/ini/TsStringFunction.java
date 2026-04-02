package com.opensr5.ini;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.field.EnumIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.StringIniField;
import org.jetbrains.annotations.Nullable;

import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Evaluates TunerStudio string functions: bitStringValue() and stringValue().
 *
 * - bitStringValue(enumConstantName, expression) — looks up the enum constant by name,
 *   evaluates the expression to get an integer index, returns the corresponding option string.
 * - stringValue(stringConstantName) — reads the ASCII string value from the configuration image.
 */
public class TsStringFunction {
    private static final Logging log = Logging.getLogging(TsStringFunction.class);

    private static final Pattern BIT_STRING_VALUE_PATTERN =
        Pattern.compile("bitStringValue\\s*\\(\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*,\\s*(.+?)\\s*\\)");

    private static final Pattern STRING_VALUE_PATTERN =
        Pattern.compile("stringValue\\s*\\(\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\)");

    /**
     * Check if an expression contains a bitStringValue() or stringValue() call.
     */
    public static boolean containsStringFunction(String expression) {
        if (expression == null)
            return false;
        String cleaned = stripBraces(expression);
        return BIT_STRING_VALUE_PATTERN.matcher(cleaned).find()
            || STRING_VALUE_PATTERN.matcher(cleaned).find();
    }

    /**
     * Resolve all bitStringValue() and stringValue() calls within the given expression string.
     * Surrounding text is preserved — only the function calls are replaced with their resolved values.
     *
     * @param expression the expression string (may include braces)
     * @param ini the INI file model for field lookups
     * @param configImage the configuration image (needed for stringValue and config-based enum indices)
     * @param outputChannelValues accumulated output channel values (for resolving bitStringValue's expression arg)
     * @return the resolved string, or null if resolution fails
     */
    @Nullable
    public static String resolve(String expression, IniFileModel ini,
                                 @Nullable ConfigurationImage configImage,
                                 @Nullable Map<String, Double> outputChannelValues) {
        if (expression == null)
            return null;

        String working = stripBraces(expression);

        // Resolve bitStringValue() calls
        working = resolveBitStringValueCalls(working, ini, configImage, outputChannelValues);
        if (working == null)
            return null;

        // Resolve stringValue() calls
        working = resolveStringValueCalls(working, ini, configImage);
        if (working == null)
            return null;

        return working.trim();
    }

    @Nullable
    private static String resolveBitStringValueCalls(String text, IniFileModel ini,
                                                     @Nullable ConfigurationImage configImage,
                                                     @Nullable Map<String, Double> outputChannelValues) {
        Matcher m = BIT_STRING_VALUE_PATTERN.matcher(text);
        StringBuffer sb = new StringBuffer();
        while (m.find()) {
            String enumName = m.group(1);
            String indexExpr = m.group(2).trim();

            String resolved = resolveBitStringValue(enumName, indexExpr, ini, configImage, outputChannelValues);
            if (resolved == null) {
                log.debug("Failed to resolve bitStringValue(" + enumName + ", " + indexExpr + ")");
                return null;
            }
            m.appendReplacement(sb, Matcher.quoteReplacement(resolved));
        }
        m.appendTail(sb);
        return sb.toString();
    }

    @Nullable
    private static String resolveBitStringValue(String enumName, String indexExpr, IniFileModel ini,
                                                @Nullable ConfigurationImage configImage,
                                                @Nullable Map<String, Double> outputChannelValues) {
        // Look up the enum field
        Optional<IniField> optField = ini.findIniField(enumName);
        if (!optField.isPresent()) {
            log.debug("bitStringValue: field not found: " + enumName);
            return null;
        }
        IniField field = optField.get();
        if (!(field instanceof EnumIniField)) {
            log.debug("bitStringValue: field is not an EnumIniField: " + enumName);
            return null;
        }
        EnumIniField enumField = (EnumIniField) field;

        // Evaluate the index expression
        Double indexValue = resolveNumericExpression(indexExpr, ini, configImage, outputChannelValues);
        if (indexValue == null) {
            log.debug("bitStringValue: could not resolve index expression: " + indexExpr);
            return null;
        }

        int index = indexValue.intValue();
        String result = enumField.getEnums().get(index);
        if (result == null) {
            log.debug("bitStringValue: no enum value at index " + index + " for " + enumName);
            return null;
        }
        return result;
    }

    @Nullable
    private static String resolveStringValueCalls(String text, IniFileModel ini,
                                                  @Nullable ConfigurationImage configImage) {
        Matcher m = STRING_VALUE_PATTERN.matcher(text);
        StringBuffer sb = new StringBuffer();
        while (m.find()) {
            String fieldName = m.group(1);

            String resolved = resolveStringValue(fieldName, ini, configImage);
            if (resolved == null) {
                log.debug("Failed to resolve stringValue(" + fieldName + ")");
                return null;
            }
            m.appendReplacement(sb, Matcher.quoteReplacement(resolved));
        }
        m.appendTail(sb);
        return sb.toString();
    }

    @Nullable
    private static String resolveStringValue(String fieldName, IniFileModel ini,
                                             @Nullable ConfigurationImage configImage) {
        if (configImage == null) {
            log.debug("stringValue: no config image available for " + fieldName);
            return null;
        }

        Optional<IniField> optField = ini.findIniField(fieldName);
        if (!optField.isPresent()) {
            log.debug("stringValue: field not found: " + fieldName);
            return null;
        }
        IniField field = optField.get();
        if (!(field instanceof StringIniField)) {
            log.debug("stringValue: field is not a StringIniField: " + fieldName);
            return null;
        }
        StringIniField stringField = (StringIniField) field;

        byte[] bytes = configImage.getRange(stringField.getOffset(), stringField.getSize());
        return trimNulls(new String(bytes, StandardCharsets.US_ASCII));
    }

    /**
     * Resolve a numeric expression using output channel values, config values, or static evaluation.
     */
    @Nullable
    static Double resolveNumericExpression(String expr, IniFileModel ini,
                                           @Nullable ConfigurationImage configImage,
                                           @Nullable Map<String, Double> outputChannelValues) {
        // Try as a simple variable name first
        String simpleVar = expr.trim();
        if (ExpressionEvaluator.IS_SIMPLE_IDENTIFIER.matcher(simpleVar).matches()) {
            // Try output channel values first
            if (outputChannelValues != null && outputChannelValues.containsKey(simpleVar)) {
                return outputChannelValues.get(simpleVar);
            }
            // Fall back to config image
            if (configImage != null) {
                Double configVal = readConfigValue(simpleVar, ini, configImage);
                if (configVal != null) {
                    return configVal;
                }
            }
            log.info("resolveNumericExpression(" + simpleVar + "): NOT FOUND (outputChannels="
                + (outputChannelValues != null) + " configImage=" + (configImage != null)
                + " iniField=" + ini.findIniField(simpleVar).isPresent() + ")");
            return null;
        }

        // Try static evaluation (numeric literal or arithmetic)
        Double staticResult = ExpressionEvaluator.tryEvaluate(expr);
        if (staticResult != null) {
            return staticResult;
        }

        // Try evaluation with context
        if (outputChannelValues != null) {
            return ExpressionEvaluator.tryEvaluateWithContext(expr, outputChannelValues);
        }

        return null;
    }

    /**
     * Read a numeric value from the config image for the given field name.
     * Handles ScalarIniField and EnumIniField (bits).
     */
    @Nullable
    private static Double readConfigValue(String fieldName, IniFileModel ini, ConfigurationImage configImage) {
        Optional<IniField> optField = ini.findIniField(fieldName);
        return optField.map(configImage::readNumericValue).orElse(null);
    }

    /**
     * Extract the expression argument from a bitStringValue() call.
     * Used by ExpressionEvaluator to extract variables that need resolving.
     *
     * @return the expression string (second argument), or null if not a bitStringValue call
     */
    @Nullable
    public static String extractBitStringValueExpression(String expression) {
        if (expression == null)
            return null;
        String cleaned = stripBraces(expression);
        Matcher m = BIT_STRING_VALUE_PATTERN.matcher(cleaned);
        if (m.find()) {
            return m.group(2).trim();
        }
        return null;
    }

    private static String stripBraces(String s) {
        return ExpressionEvaluator.stripBraces(s);
    }

    private static String trimNulls(String s) {
        int end = s.indexOf('\0');
        return end >= 0 ? s.substring(0, end) : s;
    }

    /**
     * Returns all possible resolved strings for a label expression.
     * For labels containing {@code bitStringValue(enumName, varExpr)} where {@code varExpr} is not a
     * numeric literal, every enum option is substituted to produce the full candidate list.
     * For static labels or literal-indexed calls, returns a single-element list.
     * Used to pre-compute the maximum display width of an indicator label so its size stays stable.
     */
    public static List<String> allPossibleResolutions(String expression, IniFileModel ini) {
        if (expression == null || expression.isEmpty()) return Collections.singletonList(" ");
        String cleaned = stripBraces(expression);
        Matcher m = BIT_STRING_VALUE_PATTERN.matcher(cleaned);
        if (!m.find()) {
            return Collections.singletonList(cleaned.trim());
        }
        String enumName = m.group(1);
        String indexExpr = m.group(2).trim();
        int matchStart = m.start();
        int matchEnd = m.end();
        String prefix = cleaned.substring(0, matchStart);
        String suffix = cleaned.substring(matchEnd);

        // If the index is a numeric literal, only one value is possible
        try {
            int index = Integer.parseInt(indexExpr);
            Optional<IniField> optField = ini.findIniField(enumName);
            if (optField.isPresent() && optField.get() instanceof EnumIniField) {
                String val = ((EnumIniField) optField.get()).getEnums().get(index);
                if (val != null) return Collections.singletonList((prefix + val + suffix).trim());
            }
            return Collections.singletonList(cleaned.trim());
        } catch (NumberFormatException ignored) {
            // Variable index — list all options
        }

        Optional<IniField> optField = ini.findIniField(enumName);
        if (!optField.isPresent() || !(optField.get() instanceof EnumIniField)) {
            return Collections.singletonList(cleaned.trim());
        }
        Collection<String> enumVals = ((EnumIniField) optField.get()).getEnums().values();
        List<String> results = new ArrayList<>(enumVals.size());
        for (String val : enumVals) {
            results.add((prefix + val + suffix).trim());
        }
        return results;
    }
}
