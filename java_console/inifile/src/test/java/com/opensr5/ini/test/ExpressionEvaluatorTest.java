package com.opensr5.ini.test;

import com.opensr5.ini.ExpressionEvaluator;
import org.junit.jupiter.api.Test;

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
}
