package com.opensr5.ini.test;

import com.opensr5.ini.ExpressionEvaluator;
import org.junit.jupiter.api.Test;

import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.*;

public class ExpressionEvaluatorTest {
    private static final double EPS = 0.001;

    // ========== tryEvaluate() - Simple Numbers ==========

    @Test
    public void testTryEvaluate_SimpleInteger() {
        Double result = ExpressionEvaluator.tryEvaluate("100");
        assertNotNull(result);
        assertEquals(100.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_SimpleDecimal() {
        Double result = ExpressionEvaluator.tryEvaluate("42.5");
        assertNotNull(result);
        assertEquals(42.5, result, EPS);
    }

    @Test
    public void testTryEvaluate_NegativeNumber() {
        Double result = ExpressionEvaluator.tryEvaluate("-10");
        assertNotNull(result);
        assertEquals(-10.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_Zero() {
        Double result = ExpressionEvaluator.tryEvaluate("0");
        assertNotNull(result);
        assertEquals(0.0, result, EPS);
    }

    // ========== tryEvaluate() - With Braces ==========

    @Test
    public void testTryEvaluate_NumberWithBraces() {
        Double result = ExpressionEvaluator.tryEvaluate("{100}");
        assertNotNull(result);
        assertEquals(100.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_BracesWithSpaces() {
        Double result = ExpressionEvaluator.tryEvaluate("{ 42 }");
        assertNotNull(result);
        assertEquals(42.0, result, EPS);
    }

    // ========== tryEvaluate() - Basic Arithmetic ==========

    @Test
    public void testTryEvaluate_Addition() {
        Double result = ExpressionEvaluator.tryEvaluate("5 + 3");
        assertNotNull(result);
        assertEquals(8.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_AdditionWithBraces() {
        Double result = ExpressionEvaluator.tryEvaluate("{5 + 3}");
        assertNotNull(result);
        assertEquals(8.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_Subtraction() {
        Double result = ExpressionEvaluator.tryEvaluate("10 - 3");
        assertNotNull(result);
        assertEquals(7.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_Multiplication() {
        Double result = ExpressionEvaluator.tryEvaluate("4 * 5");
        assertNotNull(result);
        assertEquals(20.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_Division() {
        Double result = ExpressionEvaluator.tryEvaluate("10 / 2");
        assertNotNull(result);
        assertEquals(5.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_DivisionWithBraces() {
        Double result = ExpressionEvaluator.tryEvaluate("{10/2}");
        assertNotNull(result);
        assertEquals(5.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_DecimalDivision() {
        Double result = ExpressionEvaluator.tryEvaluate("{1/10}");
        assertNotNull(result);
        assertEquals(0.1, result, EPS);
    }

    @Test
    public void testTryEvaluate_FractionalDivision() {
        Double result = ExpressionEvaluator.tryEvaluate("{100/10}");
        assertNotNull(result);
        assertEquals(10.0, result, EPS);
    }

    // ========== tryEvaluate() - Complex Expressions ==========

    @Test
    public void testTryEvaluate_MultipleOperations() {
        Double result = ExpressionEvaluator.tryEvaluate("5 + 3 * 2");
        assertNotNull(result);
        assertEquals(11.0, result, EPS); // Respects operator precedence
    }

    @Test
    public void testTryEvaluate_WithParentheses() {
        Double result = ExpressionEvaluator.tryEvaluate("(5 + 3) * 2");
        assertNotNull(result);
        assertEquals(16.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_ComplexExpression() {
        Double result = ExpressionEvaluator.tryEvaluate("{(10 + 5) * 2}");
        assertNotNull(result);
        assertEquals(30.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_NestedParentheses() {
        Double result = ExpressionEvaluator.tryEvaluate("((10 + 5) * 2) - 5");
        assertNotNull(result);
        assertEquals(25.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_NegativeInExpression() {
        Double result = ExpressionEvaluator.tryEvaluate("{-5 + 10}");
        assertNotNull(result);
        assertEquals(5.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_MultipleDecimals() {
        Double result = ExpressionEvaluator.tryEvaluate("1.5 * 2.5");
        assertNotNull(result);
        assertEquals(3.75, result, EPS);
    }

    // ========== tryEvaluate() - Variables (Should Return Null) ==========

    @Test
    public void testTryEvaluate_SingleVariable() {
        Double result = ExpressionEvaluator.tryEvaluate("{rpmHardLimit}");
        assertNull(result);
    }

    @Test
    public void testTryEvaluate_VariableWithAddition() {
        Double result = ExpressionEvaluator.tryEvaluate("{rpmHardLimit + 2000}");
        assertNull(result);
    }

    @Test
    public void testTryEvaluate_VariableWithSubtraction() {
        Double result = ExpressionEvaluator.tryEvaluate("{rpmHardLimit - 500}");
        assertNull(result);
    }

    @Test
    public void testTryEvaluate_VariableWithMultiplication() {
        Double result = ExpressionEvaluator.tryEvaluate("{benchTestCount * 2}");
        assertNull(result);
    }

    @Test
    public void testTryEvaluate_UnderscoreVariable() {
        Double result = ExpressionEvaluator.tryEvaluate("{cranking_rpm}");
        assertNull(result);
    }

    @Test
    public void testTryEvaluate_CamelCaseVariable() {
        Double result = ExpressionEvaluator.tryEvaluate("{testBenchCount}");
        assertNull(result);
    }

    // ========== tryEvaluate() - Function Calls (Should Return Null) ==========

    @Test
    public void testTryEvaluate_FunctionCall() {
        Double result = ExpressionEvaluator.tryEvaluate("{ stringValue(gpPwmNote1) }");
        assertNull(result);
    }

    @Test
    public void testTryEvaluate_FunctionWithMultipleArgs() {
        Double result = ExpressionEvaluator.tryEvaluate("bitStringValue(debugFieldF1List, debugMode)");
        assertNull(result);
    }

    @Test
    public void testTryEvaluate_FunctionWithNoArgs() {
        Double result = ExpressionEvaluator.tryEvaluate("getValue()");
        assertNull(result);
    }

    // ========== tryEvaluate() - Edge Cases ==========

    @Test
    public void testTryEvaluate_Null() {
        Double result = ExpressionEvaluator.tryEvaluate(null);
        assertNull(result);
    }

    @Test
    public void testTryEvaluate_EmptyString() {
        Double result = ExpressionEvaluator.tryEvaluate("");
        assertNull(result);
    }

    @Test
    public void testTryEvaluate_WhitespaceOnly() {
        Double result = ExpressionEvaluator.tryEvaluate("   ");
        assertNull(result);
    }

    @Test
    public void testTryEvaluate_WithWhitespace() {
        Double result = ExpressionEvaluator.tryEvaluate("  42  ");
        assertNotNull(result);
        assertEquals(42.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_InvalidSyntax() {
        // JavaScript engine is lenient: "5 + + 3" is valid (unary plus)
        // This evaluates to 5 + (+3) = 8.0
        Double result = ExpressionEvaluator.tryEvaluate("5 + + 3");
        assertNotNull(result);
        assertEquals(8.0, result, EPS);
    }

    @Test
    public void testTryEvaluate_DivisionByZero() {
        Double result = ExpressionEvaluator.tryEvaluate("10 / 0");
        // exp4j throws an exception for division by zero, so we get null
        assertNull(result);
    }

    @Test
    public void testTryEvaluate_UnbalancedBraces() {
        Double result = ExpressionEvaluator.tryEvaluate("{10 + 5");
        // Should still evaluate because braces are stripped
        assertNotNull(result);
        assertEquals(15.0, result, EPS);
    }

    // ========== looksLikeExpression() - With Braces ==========

    @Test
    public void testLooksLikeExpression_WithBraces() {
        assertTrue(ExpressionEvaluator.looksLikeExpression("{10/2}"));
    }

    @Test
    public void testLooksLikeExpression_WithOpeningBrace() {
        assertTrue(ExpressionEvaluator.looksLikeExpression("{rpmHardLimit"));
    }

    @Test
    public void testLooksLikeExpression_WithClosingBrace() {
        assertTrue(ExpressionEvaluator.looksLikeExpression("rpmHardLimit}"));
    }

    @Test
    public void testLooksLikeExpression_EmptyBraces() {
        assertTrue(ExpressionEvaluator.looksLikeExpression("{}"));
    }

    // ========== looksLikeExpression() - With Operators ==========

    @Test
    public void testLooksLikeExpression_WithPlus() {
        assertTrue(ExpressionEvaluator.looksLikeExpression("5 + 3"));
    }

    @Test
    public void testLooksLikeExpression_WithMultiply() {
        assertTrue(ExpressionEvaluator.looksLikeExpression("5 * 3"));
    }

    @Test
    public void testLooksLikeExpression_WithDivide() {
        assertTrue(ExpressionEvaluator.looksLikeExpression("10/2"));
    }

    @Test
    public void testLooksLikeExpression_WithParentheses() {
        assertTrue(ExpressionEvaluator.looksLikeExpression("(10 + 5)"));
    }

    @Test
    public void testLooksLikeExpression_WithMinusInMiddle() {
        assertTrue(ExpressionEvaluator.looksLikeExpression("10 - 5"));
    }

    // ========== looksLikeExpression() - Plain Values (Should Be False) ==========

    @Test
    public void testLooksLikeExpression_PlainNumber() {
        assertFalse(ExpressionEvaluator.looksLikeExpression("100"));
    }

    @Test
    public void testLooksLikeExpression_NegativeNumber() {
        assertFalse(ExpressionEvaluator.looksLikeExpression("-10"));
    }

    @Test
    public void testLooksLikeExpression_DecimalNumber() {
        assertFalse(ExpressionEvaluator.looksLikeExpression("42.5"));
    }

    @Test
    public void testLooksLikeExpression_NegativeDecimal() {
        assertFalse(ExpressionEvaluator.looksLikeExpression("-42.5"));
    }

    @Test
    public void testLooksLikeExpression_PlainString() {
        assertFalse(ExpressionEvaluator.looksLikeExpression("RPM"));
    }

    @Test
    public void testLooksLikeExpression_StringWithSpaces() {
        assertFalse(ExpressionEvaluator.looksLikeExpression("Engine Speed"));
    }

    @Test
    public void testLooksLikeExpression_SpecialChars() {
        assertFalse(ExpressionEvaluator.looksLikeExpression("°C"));
    }

    @Test
    public void testLooksLikeExpression_PercentSign() {
        assertFalse(ExpressionEvaluator.looksLikeExpression("%"));
    }

    // ========== looksLikeExpression() - Edge Cases ==========

    @Test
    public void testLooksLikeExpression_Null() {
        assertFalse(ExpressionEvaluator.looksLikeExpression(null));
    }

    @Test
    public void testLooksLikeExpression_EmptyString() {
        assertFalse(ExpressionEvaluator.looksLikeExpression(""));
    }

    @Test
    public void testLooksLikeExpression_WhitespaceOnly() {
        assertFalse(ExpressionEvaluator.looksLikeExpression("   "));
    }

    @Test
    public void testLooksLikeExpression_Zero() {
        assertFalse(ExpressionEvaluator.looksLikeExpression("0"));
    }

    // ========== extractSimpleVariableName() ==========

    @Test
    public void testExtractSimpleVariableName_WrappedVariable() {
        assertEquals("intake", ExpressionEvaluator.extractSimpleVariableName("{ intake }"));
    }

    @Test
    public void testExtractSimpleVariableName_WrappedVariableNoBraces() {
        // Without braces, just a plain variable name
        assertEquals("coolant", ExpressionEvaluator.extractSimpleVariableName("coolant"));
    }

    @Test
    public void testExtractSimpleVariableName_WithOperator() {
        // Expression with operators should return null
        assertNull(ExpressionEvaluator.extractSimpleVariableName("{ coolant * 1.8 }"));
    }

    @Test
    public void testExtractSimpleVariableName_WithAddition() {
        assertNull(ExpressionEvaluator.extractSimpleVariableName("{ coolant + 32 }"));
    }

    @Test
    public void testExtractSimpleVariableName_Null() {
        assertNull(ExpressionEvaluator.extractSimpleVariableName(null));
    }

    @Test
    public void testExtractSimpleVariableName_Empty() {
        assertNull(ExpressionEvaluator.extractSimpleVariableName(""));
    }

    @Test
    public void testExtractSimpleVariableName_UnderscoreVariable() {
        assertEquals("cranking_rpm", ExpressionEvaluator.extractSimpleVariableName("{ cranking_rpm }"));
    }

    // ========== extractVariables() ==========

    @Test
    public void testExtractVariables_SingleVariable() {
        Set<String> vars = ExpressionEvaluator.extractVariables("{ coolant }");
        assertEquals(1, vars.size());
        assertTrue(vars.contains("coolant"));
    }

    @Test
    public void testExtractVariables_MultipleVariables() {
        Set<String> vars = ExpressionEvaluator.extractVariables("{ coolant * multiplier + offset }");
        assertEquals(3, vars.size());
        assertTrue(vars.contains("coolant"));
        assertTrue(vars.contains("multiplier"));
        assertTrue(vars.contains("offset"));
    }

    @Test
    public void testExtractVariables_NoVariables() {
        Set<String> vars = ExpressionEvaluator.extractVariables("{ 1.8 + 32 }");
        assertTrue(vars.isEmpty());
    }

    @Test
    public void testExtractVariables_TernarySupported() {
        // Ternary expressions are now supported
        Set<String> vars = ExpressionEvaluator.extractVariables("{ useMetric ? coolant : coolant * 1.8 }");
        assertEquals(2, vars.size());
        assertTrue(vars.contains("useMetric"));
        assertTrue(vars.contains("coolant"));
    }

    @Test
    public void testExtractVariables_FunctionCallReturnsEmpty() {
        Set<String> vars = ExpressionEvaluator.extractVariables("{ stringValue(myField) }");
        assertTrue(vars.isEmpty());
    }

    // ========== tryEvaluateWithContext() ==========

    @Test
    public void testTryEvaluateWithContext_SimpleVariable() {
        Map<String, Double> context = new HashMap<>();
        context.put("coolant", 85.0);

        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ coolant }", context);
        assertNotNull(result);
        assertEquals(85.0, result, EPS);
    }

    @Test
    public void testTryEvaluateWithContext_TemperatureConversion() {
        // Formula: coolant * 1.8 + 32 (Celsius to Fahrenheit)
        Map<String, Double> context = new HashMap<>();
        context.put("coolant", 100.0);  // 100°C = 212°F

        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ coolant * 1.8 + 32 }", context);
        assertNotNull(result);
        assertEquals(212.0, result, EPS);
    }

    @Test
    public void testTryEvaluateWithContext_MultipleVariables() {
        Map<String, Double> context = new HashMap<>();
        context.put("base", 100.0);
        context.put("multiplier", 1.5);
        context.put("offset", 10.0);

        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ base * multiplier + offset }", context);
        assertNotNull(result);
        assertEquals(160.0, result, EPS);  // 100 * 1.5 + 10 = 160
    }

    @Test
    public void testTryEvaluateWithContext_MissingVariable() {
        Map<String, Double> context = new HashMap<>();
        context.put("coolant", 85.0);
        // Missing 'intake' variable

        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ coolant + intake }", context);
        assertNull(result);
    }

    @Test
    public void testTryEvaluateWithContext_NoVariablesNeeded() {
        Map<String, Double> context = new HashMap<>();

        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ 10 + 5 }", context);
        assertNotNull(result);
        assertEquals(15.0, result, EPS);
    }

    @Test
    public void testTryEvaluateWithContext_TernarySupported() {
        Map<String, Double> context = new HashMap<>();
        context.put("useMetric", 1.0);
        context.put("coolant", 100.0);

        // Ternary operators are now supported
        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ useMetric ? coolant : coolant * 1.8 }", context);
        assertNotNull(result);
        assertEquals(100.0, result, EPS);  // useMetric is true, so returns coolant
    }

    @Test
    public void testTryEvaluateWithContext_FunctionCallUnsupported() {
        Map<String, Double> context = new HashMap<>();

        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ stringValue(myField) }", context);
        assertNull(result);
    }

    @Test
    public void testTryEvaluateWithContext_EmptyContext() {
        Map<String, Double> context = new HashMap<>();

        // Expression with variable but empty context
        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ rpm }", context);
        assertNull(result);
    }

    @Test
    public void testTryEvaluateWithContext_ComplexExpression() {
        Map<String, Double> context = new HashMap<>();
        context.put("rpm", 3000.0);
        context.put("correction", 1.1);

        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ (rpm / 1000) * correction }", context);
        assertNotNull(result);
        assertEquals(3.3, result, EPS);  // (3000/1000) * 1.1 = 3.3
    }

    // ========== containsTernary() ==========

    @Test
    public void testContainsTernary_True() {
        assertTrue(ExpressionEvaluator.containsTernary("{ useMetric ? coolant : coolant * 1.8 + 32 }"));
    }

    @Test
    public void testContainsTernary_False() {
        assertFalse(ExpressionEvaluator.containsTernary("{ coolant * 1.8 + 32 }"));
    }

    @Test
    public void testContainsTernary_OnlyQuestion() {
        assertFalse(ExpressionEvaluator.containsTernary("{ someVar ? }"));
    }

    @Test
    public void testContainsTernary_OnlyColon() {
        assertFalse(ExpressionEvaluator.containsTernary("{ a : b }"));
    }

    // ========== parseTernary() ==========

    @Test
    public void testParseTernary_Simple() {
        ExpressionEvaluator.TernaryExpression result = ExpressionEvaluator.parseTernary("{ useMetric ? coolant : fahrenheit }");
        assertNotNull(result);
        assertEquals("useMetric", result.condition);
        assertEquals("coolant", result.trueExpr);
        assertEquals("fahrenheit", result.falseExpr);
    }

    @Test
    public void testParseTernary_ComplexFalseExpr() {
        ExpressionEvaluator.TernaryExpression result = ExpressionEvaluator.parseTernary("{ useMetric ? coolant : coolant * 1.8 + 32 }");
        assertNotNull(result);
        assertEquals("useMetric", result.condition);
        assertEquals("coolant", result.trueExpr);
        assertEquals("coolant * 1.8 + 32", result.falseExpr);
    }

    @Test
    public void testParseTernary_WithParentheses() {
        ExpressionEvaluator.TernaryExpression result = ExpressionEvaluator.parseTernary("{ useMetric ? coolant : (coolant * 1.8 + 32) }");
        assertNotNull(result);
        assertEquals("useMetric", result.condition);
        assertEquals("coolant", result.trueExpr);
        assertEquals("(coolant * 1.8 + 32)", result.falseExpr);
    }

    @Test
    public void testParseTernary_NegatedCondition() {
        ExpressionEvaluator.TernaryExpression result = ExpressionEvaluator.parseTernary("{ !useMetric ? fahrenheit : celsius }");
        assertNotNull(result);
        assertEquals("!useMetric", result.condition);
        assertEquals("fahrenheit", result.trueExpr);
        assertEquals("celsius", result.falseExpr);
    }

    @Test
    public void testParseTernary_NotTernary() {
        ExpressionEvaluator.TernaryExpression result = ExpressionEvaluator.parseTernary("{ coolant * 1.8 }");
        assertNull(result);
    }

    // ========== tryEvaluateWithContext() - Ternary Expressions ==========

    @Test
    public void testTryEvaluateWithContext_TernaryTrue() {
        Map<String, Double> context = new HashMap<>();
        context.put("useMetric", 1.0);  // true
        context.put("coolant", 100.0);

        // useMetric is true (1.0), so return coolant (100)
        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ useMetric ? coolant : coolant * 1.8 + 32 }", context);
        assertNotNull(result);
        assertEquals(100.0, result, EPS);
    }

    @Test
    public void testTryEvaluateWithContext_TernaryFalse() {
        Map<String, Double> context = new HashMap<>();
        context.put("useMetric", 0.0);  // false
        context.put("coolant", 100.0);

        // useMetric is false (0.0), so return coolant * 1.8 + 32 = 212 (Fahrenheit)
        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ useMetric ? coolant : coolant * 1.8 + 32 }", context);
        assertNotNull(result);
        assertEquals(212.0, result, EPS);
    }

    @Test
    public void testTryEvaluateWithContext_TernaryNegatedCondition() {
        Map<String, Double> context = new HashMap<>();
        context.put("useMetric", 1.0);  // true, but negated so condition is false
        context.put("celsius", 100.0);
        context.put("fahrenheit", 212.0);

        // !useMetric = !true = false, so return celsius
        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ !useMetric ? fahrenheit : celsius }", context);
        assertNotNull(result);
        assertEquals(100.0, result, EPS);
    }

    @Test
    public void testTryEvaluateWithContext_TernaryNegatedConditionTrue() {
        Map<String, Double> context = new HashMap<>();
        context.put("useMetric", 0.0);  // false, negated so condition is true
        context.put("celsius", 100.0);
        context.put("fahrenheit", 212.0);

        // !useMetric = !false = true, so return fahrenheit
        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ !useMetric ? fahrenheit : celsius }", context);
        assertNotNull(result);
        assertEquals(212.0, result, EPS);
    }

    @Test
    public void testTryEvaluateWithContext_TernaryComplexBranches() {
        Map<String, Double> context = new HashMap<>();
        context.put("useLambda", 1.0);
        context.put("lambdaValue", 1.0);
        context.put("AFRValue", 14.7);

        // useLambda is true, so return lambdaValue
        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ useLambda ? lambdaValue : AFRValue }", context);
        assertNotNull(result);
        assertEquals(1.0, result, EPS);
    }

    @Test
    public void testTryEvaluateWithContext_TernaryMissingConditionVar() {
        Map<String, Double> context = new HashMap<>();
        context.put("coolant", 100.0);
        // Missing useMetric

        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ useMetric ? coolant : coolant * 1.8 + 32 }", context);
        assertNull(result);
    }

    @Test
    public void testTryEvaluateWithContext_TernaryMissingBranchVar() {
        Map<String, Double> context = new HashMap<>();
        context.put("useMetric", 0.0);  // false, will evaluate false branch
        // Missing coolant for the false branch

        Double result = ExpressionEvaluator.tryEvaluateWithContext("{ useMetric ? celsius : coolant * 1.8 + 32 }", context);
        assertNull(result);
    }

    // ========== extractVariables() - Ternary ==========

    @Test
    public void testExtractVariables_Ternary() {
        Set<String> vars = ExpressionEvaluator.extractVariables("{ useMetric ? coolant : coolant * 1.8 + 32 }");
        assertEquals(2, vars.size());
        assertTrue(vars.contains("useMetric"));
        assertTrue(vars.contains("coolant"));
    }

    @Test
    public void testExtractVariables_TernaryDifferentBranches() {
        Set<String> vars = ExpressionEvaluator.extractVariables("{ useLambda ? lambdaValue : AFRValue }");
        assertEquals(3, vars.size());
        assertTrue(vars.contains("useLambda"));
        assertTrue(vars.contains("lambdaValue"));
        assertTrue(vars.contains("AFRValue"));
    }

    @Test
    public void testExtractVariables_TernaryNegated() {
        Set<String> vars = ExpressionEvaluator.extractVariables("{ !useMetric ? fahrenheit : celsius }");
        assertEquals(3, vars.size());
        assertTrue(vars.contains("useMetric"));
        assertTrue(vars.contains("fahrenheit"));
        assertTrue(vars.contains("celsius"));
    }

    // ========== Real-world Examples ==========

    @Test
    public void testRealWorld_CoolantTemperature_Metric() {
        // From tunerstudio.template.ini: coolantTemperature = { useMetricOnInterface ? coolant : (coolant * 1.8 + 32) }
        Map<String, Double> context = new HashMap<>();
        context.put("useMetricOnInterface", 1.0);  // Metric
        context.put("coolant", 85.0);  // 85°C

        Double result = ExpressionEvaluator.tryEvaluateWithContext(
            "{ useMetricOnInterface ? coolant : (coolant * 1.8 + 32) }", context);
        assertNotNull(result);
        assertEquals(85.0, result, EPS);  // Returns Celsius
    }

    @Test
    public void testRealWorld_CoolantTemperature_Imperial() {
        Map<String, Double> context = new HashMap<>();
        context.put("useMetricOnInterface", 0.0);  // Imperial
        context.put("coolant", 85.0);  // 85°C

        Double result = ExpressionEvaluator.tryEvaluateWithContext(
            "{ useMetricOnInterface ? coolant : (coolant * 1.8 + 32) }", context);
        assertNotNull(result);
        assertEquals(185.0, result, EPS);  // 85 * 1.8 + 32 = 185°F
    }

    @Test
    public void testRealWorld_LambdaOrAFR_Lambda() {
        // From tunerstudio: {useLambdaOnInterface ? lambdaValue : AFRValue}
        Map<String, Double> context = new HashMap<>();
        context.put("useLambdaOnInterface", 1.0);
        context.put("lambdaValue", 0.98);
        context.put("AFRValue", 14.4);

        Double result = ExpressionEvaluator.tryEvaluateWithContext(
            "{ useLambdaOnInterface ? lambdaValue : AFRValue }", context);
        assertNotNull(result);
        assertEquals(0.98, result, EPS);
    }

    @Test
    public void testRealWorld_LambdaOrAFR_AFR() {
        Map<String, Double> context = new HashMap<>();
        context.put("useLambdaOnInterface", 0.0);
        context.put("lambdaValue", 0.98);
        context.put("AFRValue", 14.4);

        Double result = ExpressionEvaluator.tryEvaluateWithContext(
            "{ useLambdaOnInterface ? lambdaValue : AFRValue }", context);
        assertNotNull(result);
        assertEquals(14.4, result, EPS);
    }
}
