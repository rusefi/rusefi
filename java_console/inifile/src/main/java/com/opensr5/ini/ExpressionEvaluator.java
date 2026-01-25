package com.opensr5.ini;

import com.devexperts.logging.Logging;
import org.jetbrains.annotations.Nullable;

import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;

/**
 * Evaluates mathematical expressions using ScriptEngine.
 * This provides a unified way to evaluate expressions without depending on IniField.parseDouble.
 *
 * Supports:
 * - Basic arithmetic: +, -, *, /
 * - Parentheses for grouping
 * - Numeric literals
 *
 * Does NOT support (these require runtime context):
 * - Variable references (e.g., rpmHardLimit)
 * - Function calls (e.g., stringValue(), bitStringValue())
 * TODO: move to exp4j?, we can use it for variable references and function calls
 */
public class ExpressionEvaluator {
    private static final Logging log = Logging.getLogging(ExpressionEvaluator.class);
    private static final ScriptEngine engine = new ScriptEngineManager().getEngineByName("JavaScript");

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
        String cleaned = expression.trim().replaceAll("^\\{\\s*", "").replaceAll("\\s*\\}$", "").trim();

        // Check if it contains variable names or function calls - these can't be evaluated without context
        if (containsVariableOrFunction(cleaned)) {
            return null;
        }

        try {
            Object result = engine.eval(cleaned);
            if (result instanceof Number) {
                return ((Number) result).doubleValue();
            }
        } catch (ScriptException e) {
            log.debug("Failed to evaluate expression: " + expression + " - " + e.getMessage());
        }

        return null;
    }

    /**
     * Check if the expression contains variable names or function calls that require runtime context.
     *
     * @param expression the cleaned expression (without braces)
     * @return true if it contains variables or functions
     */
    private static boolean containsVariableOrFunction(String expression) {
        // Check for function calls like stringValue(), bitStringValue()
        if (expression.matches(".*\\w+\\s*\\(.*")) {
            return true;
        }

        // Check for identifiers that look like variable names (not just operators and numbers)
        // Allow: digits, operators (+, -, *, /), parentheses, decimal points, whitespace
        // Disallow: alphabetic characters (which indicate variable names)
        if (expression.matches(".*[a-zA-Z_].*")) {
            return true;
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
        if (s.matches(".*[+*/()].*")) {
            return true;
        }

        // Has minus not at the start or with multiple parts
        if (s.contains("-") && !s.matches("^-?\\d+(\\.\\d+)?$")) {
            return true;
        }

        return false;
    }
}
