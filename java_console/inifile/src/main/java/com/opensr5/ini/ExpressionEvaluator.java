package com.opensr5.ini;

import com.devexperts.logging.Logging;
import net.objecthunter.exp4j.Expression;
import net.objecthunter.exp4j.ExpressionBuilder;
import org.jetbrains.annotations.Nullable;

/**
 * Evaluates mathematical expressions using exp4j.
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
 */
public class ExpressionEvaluator {
    private static final Logging log = Logging.getLogging(ExpressionEvaluator.class);

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
        // These are not standard math functions
        if (expression.matches(".*(stringValue|bitStringValue|getValue)\\s*\\(.*")) {
            return true;
        }

        // Check for identifiers that look like variable names (not just operators and numbers)
        // Allow: digits, operators (+, -, *, /, ^), parentheses, decimal points, whitespace, e/E for scientific notation

        // Remove known exp4j function names to check for variables
        String withoutFunctions = expression
            .replaceAll("\\b(sin|cos|tan|asin|acos|atan|sinh|cosh|tanh|abs|log|log10|log2|sqrt|cbrt|ceil|floor|exp)\\b", "");

        // Now check if there are any remaining alphabetic characters (variables)
        if (withoutFunctions.matches(".*[a-zA-Z_].*")) {
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
