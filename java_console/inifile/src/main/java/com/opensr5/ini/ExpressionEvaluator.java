package com.opensr5.ini;

import com.devexperts.logging.Logging;
import net.objecthunter.exp4j.Expression;
import net.objecthunter.exp4j.ExpressionBuilder;
import org.jetbrains.annotations.Nullable;

import java.util.HashSet;
import java.util.Map;
import java.util.Set;
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
 */
public class ExpressionEvaluator {
    private static final Logging log = Logging.getLogging(ExpressionEvaluator.class);

    // Pattern to match variable names (identifiers) in expressions
    private static final Pattern VARIABLE_PATTERN = Pattern.compile("\\b([a-zA-Z_][a-zA-Z0-9_]*)\\b");

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
        if (expression.matches(".*(stringValue|bitStringValue|getValue)\\s*\\(.*")) {
            return true;
        }

        // Check for identifiers that look like variable names (not just operators and numbers)
        return expression.matches(".*[a-zA-Z_].*");
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
        return s.contains("-") && !s.matches("^-?\\d+(\\.\\d+)?$");
    }

    /**
     * Extract variable names from an expression, including ternary expressions.
     *
     * @param expression the expression to analyze (e.g., "{coolant * 1.8 + 32}" or "{useMetric ? coolant : coolant * 1.8}")
     * @return a set of variable names found in the expression
     */
    public static Set<String> extractVariables(String expression) {
        Set<String> variables = new HashSet<>();
        if (expression == null || expression.trim().isEmpty()) {
            return variables;
        }

        // Remove braces if present
        String cleaned = expression.trim().replaceAll("^\\{\\s*", "").replaceAll("\\s*\\}$", "").trim();

        // Check for unsupported constructs (function calls)
        if (containsUnsupportedConstruct(cleaned)) {
            return variables;
        }

        if (containsTernary(cleaned)) {
            TernaryExpression ternary = parseTernary(cleaned);
            if (ternary != null) {
                // Extract from condition (remove ! prefix if present)
                String condition = ternary.condition.startsWith("!") ?
                    ternary.condition.substring(1).trim() : ternary.condition;
                variables.addAll(extractVariablesFromSimpleExpr(condition));
                // Extract from both branches
                variables.addAll(extractVariablesFromSimpleExpr(ternary.trueExpr));
                variables.addAll(extractVariablesFromSimpleExpr(ternary.falseExpr));
                return variables;
            }
        }

        return extractVariablesFromSimpleExpr(cleaned);
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
     *
     * @param expression the cleaned expression (without braces)
     * @return true if it contains unsupported constructs
     */
    private static boolean containsUnsupportedConstruct(String expression) {
        // Check for custom function calls like stringValue(), bitStringValue()
        return expression.matches(".*(stringValue|bitStringValue|getValue)\\s*\\(.*");
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
        String cleaned = expression.trim().replaceAll("^\\{\\s*", "").replaceAll("\\s*\\}$", "").trim();
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
        String cleaned = expression.trim().replaceAll("^\\{\\s*", "").replaceAll("\\s*\\}$", "").trim();

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

        // Extract variable names from the expression
        Set<String> requiredVars = new HashSet<>();
        Matcher matcher = VARIABLE_PATTERN.matcher(expression);
        while (matcher.find()) {
            String varName = matcher.group(1);
            requiredVars.add(varName);
        }

        // Check if all required variables are provided
        for (String var : requiredVars) {
            if (!variables.containsKey(var)) {
                log.debug("Missing variable '" + var + "' for expression: " + expression);
                return null;
            }
        }

        try {
            ExpressionBuilder builder = new ExpressionBuilder(expression);
            if (!requiredVars.isEmpty()) {
                builder.variables(requiredVars);
            }
            Expression e = builder.build();

            // Set variable values
            for (String var : requiredVars) {
                e.setVariable(var, variables.get(var));
            }

            return e.evaluate();
        } catch (Exception e) {
            log.debug("Failed to evaluate expression: " + expression + " - " + e.getMessage());
        }

        return null;
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
        String cleaned = expression.trim().replaceAll("^\\{\\s*", "").replaceAll("\\s*}$", "").trim();

        // Check if it's just a simple identifier (no operators)
        if (cleaned.matches("^[a-zA-Z_][a-zA-Z0-9_]*$")) {
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
        String cleaned = expression.trim().replaceAll("^\\{\\s*", "").replaceAll("\\s*}$", "").trim();

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
