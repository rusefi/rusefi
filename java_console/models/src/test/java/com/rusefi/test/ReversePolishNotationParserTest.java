package com.rusefi.test;

import com.fathzer.soft.javaluator.DoubleEvaluator;
import com.fathzer.soft.javaluator.Operator;
import com.rusefi.InfixConverter;
import com.rusefi.config.generated.Fields;
import org.junit.Test;

import java.text.ParseException;

import static org.junit.Assert.*;

/**
 * @see DoubleEvaluator
 * @see Operator
 */
public class ReversePolishNotationParserTest {
    @Test
    public void testFunctionParameters() {
        assertParseB("3 log", "log(3)");
        assertParseB("1 2 3 if", "if(1, 2, 3)");

        assertParseB("1 3 max", "max(1, 3)");
        try {
            assertHumanToRPN("max(1 3)", "3 1 max");
            fail("Error expected");
        } catch (Throwable ignored) {
            // expected
        }

        assertHumanToRPN("log 3 ", "log 3"); // todo: better handling?
        assertParseB("0 fsio_setting", "fsio_setting(0)");

        assertParseB("rpm 2 fsio_setting >", "rpm > fsio_setting(2)");
    }

    @Test
    public void testBooleanConversion() throws ParseException {
        assertParseB("2 1 >", "2 > 1");
        assertParseB("1 2 + 3 -", "1 + 2 - 3");
        assertParseB("1 2 | 3 |", "1 | 2 | 3");

        assertParseB("1 2 + 3 4 5 + / +", "1 + 2 + 3 / (4 +5 )");

        assertHumanToRPN("rpm > false", "rpm 0 >");
        assertHumanToRPN("(rpm > false)", "rpm 0 >");
        assertParseB("rpm user0 > clt user2 > or", "(rpm > user0) or (clt > user2)");
        assertParseB("1 2 | 3 |", "1 | 2 | 3");
        assertParseB("rpm user0 > clt user2 > or vbatt user1 > or", "(rpm > user0) or (clt > user2) or (vbatt > user1)");
    }

    private void assertParseB(String rpn, String expression) {
        assertHumanToRPN(expression, rpn);

        // humans line infix notation for some reason
        String humanInfixForm = InfixConverter.getHumanInfixForm(rpn);
        System.out.println(humanInfixForm);

        assertEquals("infix recovery", getReplace(expression).toLowerCase(), getReplace(humanInfixForm).toLowerCase());
    }

    private String getReplace(String h) {
        return h.replaceAll("[\\(\\)]", "").replace(" ", "");
    }

    private void assertHumanToRPN(String inputExpression, String expectedRPN) {
        DoubleEvaluator evaluator = new DoubleEvaluator();
        evaluator.evaluate(inputExpression.toLowerCase());

        assertEquals(expectedRPN, evaluator.getPosftfixExpression());
    }

    @Test
    public void testRpnToHuman() {
        assertEquals("if((fsio_analog_input(0) > 20), 0, 10)", InfixConverter.getHumanInfixForm("0 fsio_analog_input 20 > 0 10 if"));

        assertTrue(InfixConverter.getHumanInfixFormOrError("0 fsoi_input 20 > 0 10 if").contains(Fields.FSIO_METHOD_FSIO_TABLE));
    }

    @Test
    public void testListOfFunctions() {
        String readableListOfFunctions = InfixConverter.getReadableListOfFunctions();
        assertTrue(readableListOfFunctions.contains(Fields.FSIO_METHOD_FSIO_DIGITAL_INPUT));
        assertTrue(readableListOfFunctions.contains(Fields.FSIO_METHOD_FSIO_TABLE));
    }

    @Test
    public void testUnaryMinus() {
        /**
         * do we even need to and/or should to support unary minus?
         *
         * http://stackoverflow.com/questions/20246787/handling-unary-minus-for-shunting-yard-algorithm
         */
        assertValue("3 negate", "negate3", -3);
        assertValue("2 3 6 ^ negate +", "2+negate 3^6", -727.0);
    }

    @Test
    public void testBooleanNot2() throws Exception {
        assertValue("0 !", "! 0", 1);
        assertValue("0 not", "not 0", 1);
        assertValue("1 not", "not(1)", 0);
        assertValue("0 not", "not(0)", 1);
//        assertValue("1 0 | not 0 & 1 0 | |", "(((true | false) & not(false)) | (true | false))", 1);
    }

    private void assertValue(String expectedRpn, String expression, double expectedValue) {
        DoubleEvaluator evaluator = new DoubleEvaluator();
        assertEquals(expectedValue, evaluator.evaluate(expression), 0.001);
        assertParseB(expectedRpn, expression);
    }

    @Test
    public void testRusEfi() {
        assertParseB("1 4 or", "1 or 4");
        assertParseB("1 4 or", "1 OR 4");

        assertParseB("time_since_boot 4 <", "(time_since_boot < 4)");
        assertParseB("1 4 |", "1 | 4");
        assertParseB("time_since_boot 4 < rpm 0 > |", "(time_since_boot < 4) | (rpm > 0)");

        assertParseB("rpm 4500 > trottle 50 > and", "(rpm > 4500) and (trottle> 50)");

        assertParseB("1 4 and", "1 and 4");
        assertParseB("1 4 and", "1 AND 4");

        assertParseB("1 4 &", "1 & 4");

        assertParseB("coolant fan_off_setting >", "(coolant > fan_off_setting)");

        assertParseB("1 coolant fan_on_setting > or", "1 OR (coolant > fan_on_setting)");
        assertParseB("fan coolant fan_off_setting > and coolant fan_on_setting > or", "(fan and (coolant > fan_off_setting)) OR (coolant > fan_on_setting)");

        assertParseB("time_since_boot 4 <= rpm 0 > |", "(time_since_boot <= 4) | (rpm > 0)");

        assertParseB("time_since_boot 4 <= rpm 0 > |", "(time_since_boot <= 4) | (rpm > 0)");

        assertParseB("time_since_boot 4 <= rpm 0 > or", "(time_since_boot <= 4) OR (rpm > 0)");

        assertParseB("self rpm 4800 >= and rpm 5000 > or", "(self and (rpm >= 4800)) OR (rpm > 5000)");
    }
}