package com.opensr5.ini;

import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

/**
 * Variant-like class to hold either a numeric value, a plain string, or an expression string.
 * This represents an INI value that can be:
 * - An evaluation-time numeric constant (e.g., 100, 5.0)
 * - A plain string (e.g., "RPM", "°C")
 * - A runtime-evaluated expression (e.g., {rpmHardLimit + 2000}, { stringValue(gpPwmNote1) })
 *
 * This class can be used for various INI elements, including gauges, curves, tables,
 * and any other element that may contain expressions or numeric values.
 */
public class IniValue {
    public enum Type {
        NUMERIC,    // A numeric value that has been evaluated
        STRING,     // A plain string literal
        EXPRESSION  // An expression that needs runtime evaluation
    }

    private final Type type;
    private final double numericValue;
    private final String stringValue;

    private IniValue(Type type, double numericValue, String stringValue) {
        this.type = type;
        this.numericValue = numericValue;
        this.stringValue = stringValue;
    }

    /**
     * Create an IniValue from a numeric value
     */
    public static IniValue ofNumeric(double value) {
        return new IniValue(Type.NUMERIC, value, null);
    }

    /**
     * Create an IniValue from a plain string (not an expression)
     */
    public static IniValue ofString(@NotNull String value) {
        return new IniValue(Type.STRING, 0.0, value);
    }

    /**
     * Create an IniValue from an expression string that needs runtime evaluation
     */
    public static IniValue ofExpression(@NotNull String expression) {
        return new IniValue(Type.EXPRESSION, 0.0, expression);
    }

    /**
     * Parse a string that's expected to be a string field (like title or units).
     * Can be either a plain string or an expression.
     *
     * @param s the string to parse
     * @return an IniValue that's either STRING or EXPRESSION type
     */
    public static IniValue parseString(@NotNull String s) {
        // Check if it contains expression markers
        if (ExpressionEvaluator.looksLikeExpression(s)) {
            return ofExpression(s);
        }
        // Otherwise it's a plain string
        return ofString(s);
    }

    /**
     * Parse a string that's expected to be numeric, but could also be an expression.
     * Try to evaluate it first. If evaluation fails, treat it as a runtime expression.
     *
     * @param s the string to parse
     * @return an IniValue that's either NUMERIC or EXPRESSION type
     */
    public static IniValue parseNumeric(@NotNull String s) {
        // First, try to evaluate as a number or simple math expression
        Double evaluated = ExpressionEvaluator.tryEvaluate(s);
        if (evaluated != null) {
            return ofNumeric(evaluated);
        }

        // If it looks like an expression but couldn't be evaluated, store as expression
        if (ExpressionEvaluator.looksLikeExpression(s)) {
            return ofExpression(s);
        }

        // try to parse as plain number
        try {
            double value = Double.parseDouble(s);
            return ofNumeric(value);
        } catch (NumberFormatException e) {
            // Give up and treat as expression
            return ofExpression(s);
        }
    }

    /**
     * @return the type of this value
     */
    public Type getType() {
        return type;
    }

    /**
     * @return true if this value is an expression that needs runtime evaluation
     */
    public boolean isExpression() {
        return type == Type.EXPRESSION;
    }

    /**
     * @return true if this value is a plain string
     */
    public boolean isString() {
        return type == Type.STRING;
    }

    /**
     * @return true if this value is numeric
     */
    public boolean isNumeric() {
        return type == Type.NUMERIC;
    }

    /**
     * @return the numeric value (or 0.0 if this is not numeric)
     */
    public double getNumericValue() {
        return numericValue;
    }

    /**
     * @return the string value (for STRING or EXPRESSION types), or null if this is NUMERIC
     */
    @Nullable
    public String getStringValue() {
        return stringValue;
    }

    /**
     * @return the raw string representation of this value
     */
    public String getRawString() {
        switch (type) {
            case NUMERIC:
                return String.valueOf(numericValue);
            case STRING:
            case EXPRESSION:
                return stringValue;
            default:
                return "";
        }
    }

    @Override
    public String toString() {
        switch (type) {
            case NUMERIC:
                return "Numeric{" + numericValue + "}";
            case STRING:
                return "String{" + stringValue + "}";
            case EXPRESSION:
                return "Expression{" + stringValue + "}";
            default:
                return "Unknown{}";
        }
    }
}
