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
import java.util.function.Function;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Evaluates TunerStudio string functions: bitStringValue() and stringValue().
 *
 * - bitStringValue(enumConstantName, expression) — looks up the enum constant by name,
 *   evaluates the expression to get an integer index, returns the corresponding option string.
 * - stringValue(stringConstantName) — reads the ASCII string value from the configuration image.
 * - condition ? "a" : "b" — a ternary whose branch is a quoted literal or one of the functions above.
 */
public class TsStringFunction {
    private static final Logging log = Logging.getLogging(TsStringFunction.class);

    private static final Pattern BIT_STRING_VALUE_PATTERN =
        Pattern.compile("bitStringValue\\s*\\(\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*,\\s*(.+?)\\s*\\)");

    private static final Pattern STRING_VALUE_PATTERN =
        Pattern.compile("stringValue\\s*\\(\\s*([a-zA-Z_][a-zA-Z0-9_]*)\\s*\\)");

    private static final Pattern STRING_LITERAL = Pattern.compile("\"[^\"]*\"");

    /**
     * Check if an expression is a string computed from the tune or live data: a bitStringValue()
     * or stringValue() call, or a ternary choosing between strings ({ useMetricOnInterface ? "kPa" : "psi" }).
     */
    public static boolean containsStringFunction(String expression) {
        if (expression == null)
            return false;
        String cleaned = stripBraces(expression);
        return BIT_STRING_VALUE_PATTERN.matcher(cleaned).find()
            || STRING_VALUE_PATTERN.matcher(cleaned).find()
            || isStringTernary(cleaned);
    }

    private static boolean isStringTernary(String cleaned) {
        if (cleaned.indexOf('?') < 0) {
            return false;
        }
        ExpressionEvaluator.TernaryExpression ternary = ExpressionEvaluator.parseTernary(cleaned);
        return ternary != null && (isStringLiteral(ternary.trueExpr) || isStringLiteral(ternary.falseExpr)
            || containsStringFunction(ternary.trueExpr) || containsStringFunction(ternary.falseExpr));
    }

    private static boolean isStringLiteral(String expression) {
        return STRING_LITERAL.matcher(stripBraces(expression)).matches();
    }

    /** Text of a quoted literal; anything else unchanged. */
    private static String unquote(String expression) {
        String trimmed = stripBraces(expression);
        return isStringLiteral(trimmed) ? trimmed.substring(1, trimmed.length() - 1) : trimmed;
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
        return resolveWithVariables(expression, ini, configImage,
            expr -> ExpressionEvaluator.resolveVariables(expr, ini, configImage,
                outputChannelValues == null ? Collections.emptyMap() : outputChannelValues));
    }

    @Nullable
    private static String resolveWithVariables(String expression, IniFileModel ini, @Nullable ConfigurationImage configImage,
                                               Function<String, Map<String, Double>> variables) {
        if (expression == null) {
            return null;
        }

        String working = stripBraces(expression);
        ExpressionEvaluator.TernaryExpression conditional = ExpressionEvaluator.parseTernary(working);
        if (conditional != null) {
            Boolean condition = ExpressionEvaluator.evaluateBooleanExpression(conditional.condition, variables.apply(conditional.condition));
            if (condition == null) {
                return null;
            }
            return resolveWithVariables(condition ? conditional.trueExpr : conditional.falseExpr, ini, configImage, variables);
        }

        if (isStringLiteral(working)) {
            return unquote(working);
        }

        // Resolve bitStringValue() calls
        working = resolveBitStringValueCalls(working, ini, variables);
        if (working == null)
            return null;

        // Resolve stringValue() calls
        working = resolveStringValueCalls(working, ini, configImage);
        if (working == null)
            return null;

        return working.trim();
    }

    /** Resolve a UI label without exposing TunerStudio function syntax on failure. */
    public static String resolveDisplayLabel(String label, IniFileModel ini,
                                              @Nullable ConfigurationImage configImage,
                                              @Nullable Map<String, Double> outputChannelValues) {
        return displayLabel(label, text -> resolve(text, ini, configImage, outputChannelValues));
    }

    /** Resolve label expressions against a caller's page-aware value lookup. */
    public static String resolveDisplayLabelWithVariables(String label, IniFileModel ini, @Nullable ConfigurationImage configImage,
                                                           Function<String, Map<String, Double>> variables) {
        return displayLabel(label, text -> resolveWithVariables(text, ini, configImage, variables));
    }

    private static String displayLabel(String label, Function<String, String> resolve) {
        if (label == null) {
            return "";
        }
        if (!containsStringFunction(label)) {
            return label;
        }
        String resolved = resolve.apply(label);
        return resolved != null ? resolved : "";
    }

    @Nullable
    private static String resolveBitStringValueCalls(String text, IniFileModel ini,
                                                      Function<String, Map<String, Double>> variables) {
        Matcher m = BIT_STRING_VALUE_PATTERN.matcher(text);
        StringBuffer sb = new StringBuffer();
        while (m.find()) {
            String enumName = m.group(1);
            String indexExpr = m.group(2).trim();

            String resolved = resolveBitStringValue(enumName, indexExpr, ini, variables);
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
                                                 Function<String, Map<String, Double>> variables) {
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
        Double indexValue = ExpressionEvaluator.tryEvaluateWithContext(indexExpr, variables.apply(indexExpr));
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
        if (!(field instanceof StringIniField) || !ExpressionEvaluator.containsMainPageField(configImage, field)) {
            log.debug("stringValue: field is not a readable main-page string: " + fieldName);
            return null;
        }
        StringIniField stringField = (StringIniField) field;

        byte[] bytes = configImage.getRange(stringField.getOffset(), stringField.getSize());
        return trimNulls(new String(bytes, StandardCharsets.US_ASCII));
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
        if (isStringTernary(cleaned)) {
            ExpressionEvaluator.TernaryExpression ternary = ExpressionEvaluator.parseTernary(cleaned);
            List<String> results = new ArrayList<>(allPossibleResolutions(ternary.trueExpr, ini));
            results.addAll(allPossibleResolutions(ternary.falseExpr, ini));
            return results;
        }
        if (isStringLiteral(cleaned)) {
            return Collections.singletonList(unquote(cleaned));
        }
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
