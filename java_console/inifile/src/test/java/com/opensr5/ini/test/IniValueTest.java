package com.opensr5.ini.test;

import com.opensr5.ini.IniValue;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

public class IniValueTest {
    private static final double EPS = 0.001;

    // ========== Factory Method Tests ==========

    @Test
    public void testOfNumeric() {
        IniValue value = IniValue.ofNumeric(42.5);

        assertEquals(IniValue.Type.NUMERIC, value.getType());
        assertTrue(value.isNumeric());
        assertFalse(value.isString());
        assertFalse(value.isExpression());
        assertEquals(42.5, value.getNumericValue(), EPS);
        assertNull(value.getStringValue());
        assertEquals("42.5", value.getRawString());
    }

    @Test
    public void testOfString() {
        IniValue value = IniValue.ofString("RPM");

        assertEquals(IniValue.Type.STRING, value.getType());
        assertTrue(value.isString());
        assertFalse(value.isNumeric());
        assertFalse(value.isExpression());
        assertEquals(0.0, value.getNumericValue(), EPS);
        assertEquals("RPM", value.getStringValue());
        assertEquals("RPM", value.getRawString());
    }

    @Test
    public void testOfExpression() {
        IniValue value = IniValue.ofExpression("{rpmHardLimit + 2000}");

        assertEquals(IniValue.Type.EXPRESSION, value.getType());
        assertTrue(value.isExpression());
        assertFalse(value.isNumeric());
        assertFalse(value.isString());
        assertEquals(0.0, value.getNumericValue(), EPS);
        assertEquals("{rpmHardLimit + 2000}", value.getStringValue());
        assertEquals("{rpmHardLimit + 2000}", value.getRawString());
    }

    // ========== parseString Tests ==========

    @Test
    public void testParseString_PlainString() {
        IniValue value = IniValue.parseString("Engine Speed");

        assertTrue(value.isString());
        assertEquals("Engine Speed", value.getStringValue());
    }

    @Test
    public void testParseString_SpecialCharacters() {
        IniValue value = IniValue.parseString("°C");

        assertTrue(value.isString());
        assertEquals("°C", value.getStringValue());
    }

    @Test
    public void testParseString_PercentSign() {
        IniValue value = IniValue.parseString("%");

        assertTrue(value.isString());
        assertEquals("%", value.getStringValue());
    }

    @Test
    public void testParseString_WithSpaces() {
        IniValue value = IniValue.parseString("Test Bench Iterations");

        assertTrue(value.isString());
        assertEquals("Test Bench Iterations", value.getStringValue());
    }

    @Test
    public void testParseString_ExpressionWithBraces() {
        IniValue value = IniValue.parseString("{ stringValue(gpPwmNote1) }");

        assertTrue(value.isExpression());
        assertEquals("{ stringValue(gpPwmNote1) }", value.getStringValue());
    }

    @Test
    public void testParseString_ExpressionWithFunction() {
        IniValue value = IniValue.parseString("bitStringValue(debugFieldF1List, debugMode)");

        assertTrue(value.isExpression());
        assertEquals("bitStringValue(debugFieldF1List, debugMode)", value.getStringValue());
    }

    // ========== parseNumeric Tests ==========

    @Test
    public void testParseNumeric_SimpleInteger() {
        IniValue value = IniValue.parseNumeric("100");

        assertTrue(value.isNumeric());
        assertEquals(100.0, value.getNumericValue(), EPS);
    }

    @Test
    public void testParseNumeric_SimpleDecimal() {
        IniValue value = IniValue.parseNumeric("42.5");

        assertTrue(value.isNumeric());
        assertEquals(42.5, value.getNumericValue(), EPS);
    }

    @Test
    public void testParseNumeric_NegativeNumber() {
        IniValue value = IniValue.parseNumeric("-10");

        assertTrue(value.isNumeric());
        assertEquals(-10.0, value.getNumericValue(), EPS);
    }

    @Test
    public void testParseNumeric_Zero() {
        IniValue value = IniValue.parseNumeric("0");

        assertTrue(value.isNumeric());
        assertEquals(0.0, value.getNumericValue(), EPS);
    }

    @Test
    public void testParseNumeric_SimpleDivision() {
        IniValue value = IniValue.parseNumeric("{10/2}");

        assertTrue(value.isNumeric());
        assertEquals(5.0, value.getNumericValue(), EPS);
    }

    @Test
    public void testParseNumeric_DecimalDivision() {
        IniValue value = IniValue.parseNumeric("{1/10}");

        assertTrue(value.isNumeric());
        assertEquals(0.1, value.getNumericValue(), EPS);
    }

    @Test
    public void testParseNumeric_Addition() {
        IniValue value = IniValue.parseNumeric("{5 + 3}");

        assertTrue(value.isNumeric());
        assertEquals(8.0, value.getNumericValue(), EPS);
    }

    @Test
    public void testParseNumeric_Subtraction() {
        IniValue value = IniValue.parseNumeric("{10 - 3}");

        assertTrue(value.isNumeric());
        assertEquals(7.0, value.getNumericValue(), EPS);
    }

    @Test
    public void testParseNumeric_Multiplication() {
        IniValue value = IniValue.parseNumeric("{4 * 5}");

        assertTrue(value.isNumeric());
        assertEquals(20.0, value.getNumericValue(), EPS);
    }

    @Test
    public void testParseNumeric_ComplexExpression() {
        IniValue value = IniValue.parseNumeric("{(10 + 5) * 2}");

        assertTrue(value.isNumeric());
        assertEquals(30.0, value.getNumericValue(), EPS);
    }

    @Test
    public void testParseNumeric_ExpressionWithVariable() {
        IniValue value = IniValue.parseNumeric("{rpmHardLimit + 2000}");

        assertTrue(value.isExpression());
        assertEquals("{rpmHardLimit + 2000}", value.getStringValue());
        assertEquals(0.0, value.getNumericValue(), EPS); // Returns 0 for expressions
    }

    @Test
    public void testParseNumeric_ExpressionWithSubtraction() {
        IniValue value = IniValue.parseNumeric("{rpmHardLimit - 500}");

        assertTrue(value.isExpression());
        assertEquals("{rpmHardLimit - 500}", value.getStringValue());
    }

    @Test
    public void testParseNumeric_SingleVariable() {
        IniValue value = IniValue.parseNumeric("{cranking_rpm}");

        assertTrue(value.isExpression());
        assertEquals("{cranking_rpm}", value.getStringValue());
    }

    @Test
    public void testParseNumeric_VariableWithMultiplication() {
        IniValue value = IniValue.parseNumeric("{benchTestCount * 2}");

        assertTrue(value.isExpression());
        assertEquals("{benchTestCount * 2}", value.getStringValue());
    }

    // ========== Edge Cases ==========

    @Test
    public void testParseNumeric_NegativeInExpression() {
        IniValue value = IniValue.parseNumeric("{-5 + 10}");

        assertTrue(value.isNumeric());
        assertEquals(5.0, value.getNumericValue(), EPS);
    }

    @Test
    public void testParseNumeric_ScientificNotation() {
        IniValue value = IniValue.parseNumeric("1.5e2");

        assertTrue(value.isNumeric());
        assertEquals(150.0, value.getNumericValue(), EPS);
    }

    @Test
    public void testParseString_EmptyBraces() {
        IniValue value = IniValue.parseString("{}");

        // Empty braces should be treated as expression (malformed but expression-like)
        assertTrue(value.isExpression());
    }

    @Test
    public void testParseNumeric_WithWhitespace() {
        IniValue value = IniValue.parseNumeric("  42  ");

        assertTrue(value.isNumeric());
        assertEquals(42.0, value.getNumericValue(), EPS);
    }

    // ========== toString Tests ==========

    @Test
    public void testToString_Numeric() {
        IniValue value = IniValue.ofNumeric(42.0);
        assertEquals("Numeric{42.0}", value.toString());
    }

    @Test
    public void testToString_String() {
        IniValue value = IniValue.ofString("RPM");
        assertEquals("String{RPM}", value.toString());
    }

    @Test
    public void testToString_Expression() {
        IniValue value = IniValue.ofExpression("{x + 1}");
        assertEquals("Expression{{x + 1}}", value.toString());
    }

    // ========== getRawString Tests ==========

    @Test
    public void testGetRawString_Numeric() {
        IniValue value = IniValue.ofNumeric(123.45);
        assertEquals("123.45", value.getRawString());
    }

    @Test
    public void testGetRawString_String() {
        IniValue value = IniValue.ofString("Test String");
        assertEquals("Test String", value.getRawString());
    }

    @Test
    public void testGetRawString_Expression() {
        IniValue value = IniValue.ofExpression("{var + 1}");
        assertEquals("{var + 1}", value.getRawString());
    }
}
