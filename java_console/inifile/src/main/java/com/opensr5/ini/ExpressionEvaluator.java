package com.opensr5.ini;

import com.devexperts.logging.Logging;
import com.opensr5.ConfigurationImage;
import com.opensr5.ini.field.IniField;
import net.objecthunter.exp4j.Expression;
import net.objecthunter.exp4j.ExpressionBuilder;
import org.jetbrains.annotations.Nullable;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Evaluates mathematical expressions using exp4j.
 * This provides a unified way to evaluate expressions without depending on IniField.parseDouble.
 * <p>
 * Supports:
 * - Basic arithmetic: +, -, *, /
 * - Parentheses for grouping
 * - Numeric literals
 * - Variable references with provided context (e.g., {coolant * 1.8 + 32})
 * - Ternary operator (e.g., {useMetric ? coolant : coolant * 1.8 + 32})
 * <p>
 * Does NOT support:
 * - Function calls (e.g., stringValue(), bitStringValue())
 * TODO:
 * we should considering moving this to a factory-thing, since we are passing and evaluating too many times the same ini file/configuration image
 */
public class ExpressionEvaluator {
    private static final Logging log = Logging.getLogging(ExpressionEvaluator.class);

    private static final class CachedExpression {
        final Expression expression;
        final Set<String> variables;
        CachedExpression(Expression expression, Set<String> variables) {
            this.expression = expression;
            this.variables = variables;
        }
    }

    /** Per-thread cache of built exp4j Expression objects, keyed by expression string. */
    private static final ThreadLocal<Map<String, CachedExpression>> EXPRESSION_CACHE =
        ThreadLocal.withInitial(HashMap::new);

    /**
     * Global cache of variable sets per expression string.
     * INI expressions are static after file load, so this never needs invalidation.
     */
    private static final ConcurrentHashMap<String, Set<String>> VARIABLE_SET_CACHE = new ConcurrentHashMap<>();

    // Pattern to match variable names (identifiers) in expressions
    private static final Pattern VARIABLE_PATTERN = Pattern.compile("\\b([a-zA-Z_][a-zA-Z0-9_]*)\\b");
    // Pre-compiled patterns reused across calls — never use String.matches/replaceAll with these literals
    private static final Pattern CONTAINS_FUNCTION_CALL = Pattern.compile(".*(stringValue|bitStringValue|getValue)\\s*\\(.*");
    private static final Pattern HAS_ARITHMETIC_OPERATORS = Pattern.compile(".*[+*/()].*");
    private static final Pattern IS_PLAIN_NUMBER       = Pattern.compile("^-?\\d+(\\.\\d+)?$");
    static final Pattern IS_SIMPLE_IDENTIFIER  = Pattern.compile("^[a-zA-Z_][a-zA-Z0-9_]*$");

    /**
     * Strip leading {@code {} and trailing {@code }} (with optional surrounding whitespace).
     * Implemented as a manual char scan to avoid allocating Matcher objects on every ECU frame.
     */
    static String stripBraces(String s) {
        int start = 0, end = s.length();
        // trim leading whitespace
        while (start < end && s.charAt(start) <= ' ') start++;
        // trim trailing whitespace
        while (end > start && s.charAt(end - 1) <= ' ') end--;
        // strip leading '{'
        if (start < end && s.charAt(start) == '{') {
            start++;
            while (start < end && s.charAt(start) <= ' ') start++;
        }
        // strip trailing '}'
        if (end > start && s.charAt(end - 1) == '}') {
            end--;
            while (end > start && s.charAt(end - 1) <= ' ') end--;
        }
        // final trim (handles e.g. "{ { inner } }" after outer braces removed)
        while (start < end && s.charAt(start) <= ' ') start++;
        while (end > start && s.charAt(end - 1) <= ' ') end--;
        return (start == 0 && end == s.length()) ? s : s.substring(start, end);
    }

    /**
     * Represents a parsed ternary expression: condition ? trueExpr : falseExpr
     */
    public static class TernaryExpression {
        public final String condition;
        public final String trueExpr;
        public final String falseExpr;

        public TernaryExpression(String condition, String trueExpr, String falseExpr) {
            this.condition = condition;
            this.trueExpr = trueExpr;
            this.falseExpr = falseExpr;
        }
    }

    /**
     * Try to evaluate an expression as a numeric value.
     * Strips braces {} before evaluation.
     *
     * @param expression the expression to evaluate (e.g., "10/2", "{5 + 3}", "100")
     * @return the evaluated numeric value, or null if the expression cannot be evaluated
     */
    @Nullable
    public static Double tryEvaluate(String expression) {
        if (expression == null || expression.trim().isEmpty()) {
            return null;
        }

        // Remove braces if present
        String cleaned = stripBraces(expression);

        // Check if it contains variable names or custom function calls - these can't be evaluated without context
        if (containsVariableOrFunction(cleaned)) {
            return null;
        }

        try {
            Expression e = new ExpressionBuilder(cleaned).build();
            return e.evaluate();
        } catch (Exception e) {
            log.debug("Failed to evaluate expression: " + expression + " - " + e.getMessage());
        }

        return null;
    }

    /**
     * Check if the expression contains variable names or custom function calls that require runtime context.
     *
     * @param expression the cleaned expression (without braces)
     * @return true if it contains variables or custom functions
     */
    private static boolean containsVariableOrFunction(String expression) {
        // Check for custom function calls like stringValue(), bitStringValue()
        if (containsUnsupportedConstruct(expression)) {
            return true;
        }

        // Check for identifiers that look like variable names (not just operators and numbers)
        // yes, this can be a regex, but too many calls on different threads == too much ram used evaluating this (100~mb!)
        for (int i = 0; i < expression.length(); i++) {
            char c = expression.charAt(i);
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') return true;
        }
        return false;
    }

    /**
     * Check if a string looks like an expression that might need evaluation.
     *
     * @param s the string to check
     * @return true if it looks like an expression (contains braces, operators, or parentheses)
     */
    public static boolean looksLikeExpression(String s) {
        if (s == null || s.trim().isEmpty()) {
            return false;
        }

        // Has braces - likely an expression
        if (s.contains("{") || s.contains("}")) {
            return true;
        }

        // Has arithmetic operators or parentheses (and is not just a negative number)
        if (HAS_ARITHMETIC_OPERATORS.matcher(s).matches()) {
            return true;
        }

        // Has minus not at the start or with multiple parts
        return s.contains("-") && !IS_PLAIN_NUMBER.matcher(s).matches();
    }

    /**
     * Extract variable names from an expression, including ternary expressions.
     * Results are cached per expression string (INI expressions are static after file load).
     *
     * @param expression the expression to analyze (e.g., "{coolant * 1.8 + 32}" or "{useMetric ? coolant : coolant * 1.8}")
     * @return an unmodifiable set of variable names found in the expression
     */
    public static Set<String> extractVariables(String expression) {
        if (expression == null || expression.trim().isEmpty()) {
            return Collections.emptySet();
        }
        return VARIABLE_SET_CACHE.computeIfAbsent(expression, ExpressionEvaluator::computeExtractVariables);
    }

    private static Set<String> computeExtractVariables(String expression) {
        // Remove braces if present
        String cleaned = stripBraces(expression);

        // For string function calls, extract variables from the expression argument
        if (containsUnsupportedConstruct(cleaned)) {
            String innerExpr = TsStringFunction.extractBitStringValueExpression(cleaned);
            if (innerExpr != null) {
                return Collections.unmodifiableSet(extractVariablesFromSimpleExpr(innerExpr));
            }
            return Collections.emptySet();
        }

        Set<String> variables = new HashSet<>();
        if (containsTernary(cleaned)) {
            TernaryExpression ternary = parseTernary(cleaned);
            if (ternary != null) {
                String condition = ternary.condition.startsWith("!") ?
                    ternary.condition.substring(1).trim() : ternary.condition;
                variables.addAll(extractVariablesFromSimpleExpr(condition));
                variables.addAll(extractVariablesFromSimpleExpr(ternary.trueExpr));
                variables.addAll(extractVariablesFromSimpleExpr(ternary.falseExpr));
                return Collections.unmodifiableSet(variables);
            }
        }

        return Collections.unmodifiableSet(extractVariablesFromSimpleExpr(cleaned));
    }

    /**
     * Extract variable names from a simple (non-ternary) expression.
     */
    private static Set<String> extractVariablesFromSimpleExpr(String expression) {
        Set<String> variables = new HashSet<>();
        if (expression == null || expression.trim().isEmpty()) {
            return variables;
        }

        Matcher matcher = VARIABLE_PATTERN.matcher(expression);
        while (matcher.find()) {
            String varName = matcher.group(1);
            variables.add(varName);
        }

        return variables;
    }

    /**
     * Check if the expression contains unsupported constructs like function calls.
     * Uses plain string search to avoid Matcher allocation on every ECU frame.
     *
     * @param expression the cleaned expression (without braces)
     * @return true if it contains unsupported constructs
     */
    private static boolean containsUnsupportedConstruct(String expression) {
        return expression.contains("stringValue") || expression.contains("bitStringValue") || expression.contains("getValue");
    }

    /**
     * Check if an expression contains a ternary operator.
     *
     * @param expression the expression to check
     * @return true if it contains a ternary operator (? and :)
     */
    public static boolean containsTernary(String expression) {
        if (expression == null) {
            return false;
        }
        String cleaned = stripBraces(expression);
        return cleaned.contains("?") && cleaned.contains(":");
    }

    /**
     * Parse a ternary expression into its three components.
     * Handles nested parentheses correctly.
     *
     * @param expression the ternary expression (e.g., "useMetric ? coolant : coolant * 1.8 + 32")
     * @return the parsed TernaryExpression, or null if not a valid ternary
     */
    @Nullable
    public static TernaryExpression parseTernary(String expression) {
        if (expression == null || expression.trim().isEmpty()) {
            return null;
        }

        // Remove braces if present
        String cleaned = stripBraces(expression);

        // Find the ? operator
        int questionMark = findOperatorOutsideParens(cleaned, '?');
        if (questionMark == -1) {
            return null;
        }

        String condition = cleaned.substring(0, questionMark).trim();

        // Find the : operator after the ?
        String afterQuestion = cleaned.substring(questionMark + 1);
        int colon = findOperatorOutsideParens(afterQuestion, ':');
        if (colon == -1) {
            return null;
        }

        String trueExpr = afterQuestion.substring(0, colon).trim();
        String falseExpr = afterQuestion.substring(colon + 1).trim();

        return new TernaryExpression(condition, trueExpr, falseExpr);
    }

    /**
     * Find an operator character outside of parentheses.
     *
     * @param s the string to search
     * @param op the operator character to find
     * @return the index of the operator, or -1 if not found
     */
    private static int findOperatorOutsideParens(String s, char op) {
        int depth = 0;
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (c == '(') {
                depth++;
            } else if (c == ')') {
                depth--;
            } else if (c == op && depth == 0) {
                return i;
            }
        }
        return -1;
    }

    /**
     * Evaluate a condition expression. Returns true if the value is non-zero.
     * Handles negation (! prefix).
     *
     * @param condition the condition to evaluate
     * @param variables the variable context
     * @return true if condition is truthy, false otherwise, or null if cannot evaluate
     */
    @Nullable
    private static Boolean evaluateCondition(String condition, Map<String, Double> variables) {
        condition = condition.trim();

        // Handle negation
        boolean negate = false;
        if (condition.startsWith("!")) {
            negate = true;
            condition = condition.substring(1).trim();
        }

        // Try to evaluate as an expression
        Double result = evaluateSimpleExpression(condition, variables);
        if (result == null) {
            return null;
        }

        // Non-zero is truthy
        boolean truthy = result != 0.0;
        return negate != truthy;
    }

    /**
     * Evaluate a simple (non-ternary) expression with variables.
     *
     * @param expression the expression to evaluate
     * @param variables the variable context
     * @return the result, or null if cannot evaluate
     */
    @Nullable
    private static Double evaluateSimpleExpression(String expression, Map<String, Double> variables) {
        if (expression == null || expression.trim().isEmpty()) {
            return null;
        }

        expression = expression.trim();

        // Check for unsupported constructs
        if (containsUnsupportedConstruct(expression)) {
            return null;
        }

        // Look up or build the cached Expression AST for this expression string.
        Map<String, CachedExpression> cache = EXPRESSION_CACHE.get();
        CachedExpression cached = cache.get(expression);
        if (cached == null) {
            Set<String> requiredVars = new HashSet<>();
            Matcher matcher = VARIABLE_PATTERN.matcher(expression);
            while (matcher.find()) {
                requiredVars.add(matcher.group(1));
            }
            try {
                ExpressionBuilder builder = new ExpressionBuilder(expression);
                if (!requiredVars.isEmpty()) {
                    builder.variables(requiredVars);
                }
                cached = new CachedExpression(builder.build(), requiredVars);
                cache.put(expression, cached);
            } catch (Exception e) {
                log.debug("Failed to build expression: " + expression + " - " + e.getMessage());
                return null;
            }
        }

        // Check if all required variables are provided
        for (String var : cached.variables) {
            if (!variables.containsKey(var)) {
                log.debug("Missing variable '" + var + "' for expression: " + expression);
                return null;
            }
        }

        try {
            // Set variable values and evaluate
            for (String var : cached.variables) {
                cached.expression.setVariable(var, variables.get(var));
            }
            double result = cached.expression.evaluate();
            return Double.isNaN(result) ? null : result;
        } catch (Exception e) {
            log.debug("Failed to evaluate expression: " + expression + " - " + e.getMessage());
        }

        return null;
    }

    /**
     * Convenience overload: resolves variable values from an INI model and config image, then evaluates.
     *
     * @param expression the expression to evaluate
     * @param ini the INI file model used to look up field offsets
     * @param ci the configuration image to read field values from
     * @return true/false based on the expression, or null if it cannot be evaluated
     */
    @Nullable
    public static Boolean evaluateBooleanExpression(String expression, IniFileModel ini, ConfigurationImage ci) {
        return evaluateBooleanExpression(expression, buildVariableContext(expression, ini, ci));
    }

    /**
     * Evaluate a numeric expression against INI fields read from the given configuration image.
     * Supports ternary expressions (e.g. "{useMetricOnInterface ? 300 : 43.5}").
     */
    @Nullable
    public static Double evaluateNumericExpression(String expression, IniFileModel ini, ConfigurationImage ci) {
        if (expression == null || ini == null || ci == null) return null;
        return tryEvaluateWithContext(expression, buildVariableContext(expression, ini, ci));
    }

    private static Map<String, Double> buildVariableContext(String expression, IniFileModel ini, ConfigurationImage ci) {
        Set<String> varNames = extractVariables(expression);
        Map<String, Double> context = new HashMap<>();
        for (String varName : varNames) {
            Optional<IniField> field = ini.findIniField(varName);
            if (field.isPresent()) {
                Double value = ci.readNumericValue(field.get());
                if (value != null) context.put(varName, value);
            }
        }
        return context;
    }

    /**
     * Evaluate a boolean expression like those used in TunerStudio INI files for field visibility/enabled.
     * Supports: ==, !=, >=, <=, >, <, &&, ||, !, parentheses, and bare variables (truthy if non-zero).
     *
     * @param expression the expression to evaluate (e.g., "{trigger_type == 0}" or "{field1 > 50 && field2 < 100}")
     * @param variables a map of variable names to their values
     * @return true/false based on the expression, or null if the expression cannot be evaluated
     */
    @Nullable
    public static Boolean evaluateBooleanExpression(String expression, Map<String, Double> variables) {
        if (expression == null || expression.trim().isEmpty()) {
            return null;
        }

        // Remove braces if present
        String cleaned = stripBraces(expression);

        if (cleaned.isEmpty()) {
            return null;
        }

        try {
            return parseOrExpr(cleaned, variables);
        } catch (Exception e) {
            log.debug("Failed to evaluate boolean expression: " + expression + " - " + e.getMessage());
            return null;
        }
    }

    /**
     * Parse an OR expression: terms separated by || outside parentheses.
     */
    @Nullable
    private static Boolean parseOrExpr(String expr, Map<String, Double> variables) {
        List<String> parts = splitOutsideParens(expr, "||");
        if (parts.size() > 1) {
            for (String part : parts) {
                Boolean result = parseAndExpr(part.trim(), variables);
                if (result == null) return null;
                if (result) return true;
            }
            return false;
        }
        return parseAndExpr(expr, variables);
    }

    /**
     * Parse an AND expression: terms separated by && outside parentheses.
     */
    @Nullable
    private static Boolean parseAndExpr(String expr, Map<String, Double> variables) {
        List<String> parts = splitOutsideParens(expr, "&&");
        if (parts.size() > 1) {
            for (String part : parts) {
                Boolean result = parseComparison(part.trim(), variables);
                if (result == null) return null;
                if (!result) return false;
            }
            return true;
        }
        return parseComparison(expr, variables);
    }

    /**
     * Parse a comparison expression: two sides separated by ==, !=, >=, <=, >, < outside parentheses.
     */
    @Nullable
    private static Boolean parseComparison(String expr, Map<String, Double> variables) {
        // Try each comparison operator (check 2-char operators first)
        String[] ops = {"==", "!=", ">=", "<=", ">", "<"};
        for (String op : ops) {
            int idx = findMultiCharOperatorOutsideParens(expr, op);
            if (idx >= 0) {
                String left = expr.substring(0, idx).trim();
                String right = expr.substring(idx + op.length()).trim();
                Double leftVal = evaluateSimpleExpression(left, variables);
                Double rightVal = evaluateSimpleExpression(right, variables);
                if (leftVal == null || rightVal == null) return null;
                switch (op) {
                    case "==": return Math.abs(leftVal - rightVal) < 1e-9;
                    case "!=": return Math.abs(leftVal - rightVal) >= 1e-9;
                    case ">=": return leftVal >= rightVal;
                    case "<=": return leftVal <= rightVal;
                    case ">": return leftVal > rightVal;
                    case "<": return leftVal < rightVal;
                }
            }
        }
        return parseAtom(expr, variables);
    }

    /**
     * Parse an atomic boolean expression: negation, parenthesized sub-expression, or bare variable/expression (truthy if non-zero).
     */
    @Nullable
    private static Boolean parseAtom(String expr, Map<String, Double> variables) {
        expr = expr.trim();
        if (expr.isEmpty()) return null;

        // Handle negation
        if (expr.startsWith("!")) {
            Boolean inner = parseAtom(expr.substring(1).trim(), variables);
            return inner == null ? null : !inner;
        }

        // Handle parenthesized sub-expression
        if (expr.startsWith("(") && findMatchingParen(expr) == expr.length() - 1) {
            return parseOrExpr(expr.substring(1, expr.length() - 1).trim(), variables);
        }

        // Bare variable or arithmetic expression — truthy if non-zero
        Double val = evaluateSimpleExpression(expr, variables);
        if (val == null) return null;
        return val != 0.0;
    }

    /**
     * Split a string by a delimiter, but only when the delimiter is outside parentheses.
     */
    private static List<String> splitOutsideParens(String s, String delimiter) {
        List<String> parts = new ArrayList<>();
        int depth = 0;
        int start = 0;
        for (int i = 0; i <= s.length() - delimiter.length(); i++) {
            char c = s.charAt(i);
            if (c == '(') {
                depth++;
            } else if (c == ')') {
                depth--;
            } else if (depth == 0 && s.startsWith(delimiter, i)) {
                parts.add(s.substring(start, i));
                start = i + delimiter.length();
                i += delimiter.length() - 1; // skip past delimiter
            }
        }
        parts.add(s.substring(start));
        return parts;
    }

    /**
     * Find a multi-character operator (like ==, !=, >=, <=) outside parentheses.
     * For single-char operators (>, <), avoids matching as part of multi-char operators.
     *
     * @return the index of the operator, or -1 if not found
     */
    private static int findMultiCharOperatorOutsideParens(String s, String op) {
        int depth = 0;
        for (int i = 0; i <= s.length() - op.length(); i++) {
            char c = s.charAt(i);
            if (c == '(') {
                depth++;
            } else if (c == ')') {
                depth--;
            } else if (depth == 0 && s.startsWith(op, i)) {
                // For single-char operators, make sure we're not part of a 2-char operator
                if (op.length() == 1) {
                    // Check if this is part of ==, !=, >=, <=
                    if (i > 0 && (s.charAt(i - 1) == '!' || s.charAt(i - 1) == '>' || s.charAt(i - 1) == '<')) {
                        continue;
                    }
                    if (i + 1 < s.length() && s.charAt(i + 1) == '=') {
                        continue;
                    }
                }
                return i;
            }
        }
        return -1;
    }

    /**
     * Find the matching closing parenthesis for an opening one.
     */
    private static int findMatchingParen(String s) {
        int depth = 0;
        for (int i = 0; i < s.length(); i++) {
            if (s.charAt(i) == '(') depth++;
            else if (s.charAt(i) == ')') {
                depth--;
                if (depth == 0) return i;
            }
        }
        return -1;
    }

    /**
     * Try to extract a simple variable name from an expression like "{ varName }".
     * Returns null if the expression contains operators or multiple variables.
     *
     * @param expression the expression to check
     * @return the simple variable name if this is just a wrapped variable, null otherwise
     */
    @Nullable
    public static String extractSimpleVariableName(String expression) {
        if (expression == null || expression.trim().isEmpty()) {
            return null;
        }

        // Remove braces and whitespace
        String cleaned = stripBraces(expression);

        // Check if it's just a simple identifier (no operators)
        if (IS_SIMPLE_IDENTIFIER.matcher(cleaned).matches()) {
            return cleaned;
        }

        return null;
    }

    /**
     * Try to evaluate an expression with a provided variable context.
     * Strips braces {} before evaluation. Supports ternary expressions.
     *
     * @param expression the expression to evaluate (e.g., "{coolant * 1.8 + 32}" or "{useMetric ? coolant : coolant * 1.8 + 32}")
     * @param variables a map of variable names to their values
     * @return the evaluated numeric value, or null if the expression cannot be evaluated
     */
    @Nullable
    public static Double tryEvaluateWithContext(String expression, Map<String, Double> variables) {
        if (expression == null || expression.trim().isEmpty()) {
            return null;
        }

        // Remove braces if present
        String cleaned = stripBraces(expression);

        // Check for unsupported constructs
        if (containsUnsupportedConstruct(cleaned)) {
            log.debug("Expression contains unsupported construct: " + expression);
            return null;
        }

        // Handle ternary expressions
        if (containsTernary(cleaned)) {
            return evaluateTernary(cleaned, variables);
        }

        return evaluateSimpleExpression(cleaned, variables);
    }

    /**
     * Evaluate a ternary expression: condition ? trueExpr : falseExpr
     *
     * @param expression the ternary expression (without braces)
     * @param variables the variable context
     * @return the result, or null if cannot evaluate
     */
    @Nullable
    private static Double evaluateTernary(String expression, Map<String, Double> variables) {
        TernaryExpression ternary = parseTernary(expression);
        if (ternary == null) {
            log.debug("Failed to parse ternary expression: " + expression);
            return null;
        }

        // Evaluate the condition
        Boolean conditionResult = evaluateCondition(ternary.condition, variables);
        if (conditionResult == null) {
            log.debug("Failed to evaluate ternary condition: " + ternary.condition);
            return null;
        }

        // Evaluate the appropriate branch
        String branchExpr = conditionResult ? ternary.trueExpr : ternary.falseExpr;

        // The branch itself could be a ternary (nested)
        if (containsTernary(branchExpr)) {
            return evaluateTernary(branchExpr, variables);
        }

        return evaluateSimpleExpression(branchExpr, variables);
    }
}
