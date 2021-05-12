package com.rusefi.ui.widgets.test;

import com.rusefi.ui.widgets.AnyCommand;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 * 3/9/2017.
 */
public class AnyCommandTest {
    private static String prepareCommand(String rawCommand) {
        return AnyCommand.prepareCommand(rawCommand, null);
    }

    @Test
    public void testNoExceptionIsThrown() {
        assertEquals("eval \"2 2 +\"", AnyCommand.prepareEvalCommand("eval \"2 2 +\""));
    }

    @Test
    public void testUnquote() {
        assertEquals("1 2 3", AnyCommand.unquote("  \"1 2 3\"  "));
    }

    @Test
    public void testPrepareEvalCommand() {
        assertEquals("rpn_eval \"2 3 +\"", prepareCommand("eval   \"2 + 3\" "));
    }

    @Test
    public void testSetFSIOexpression() {
        // todo: parameter order needs to be in postfix form
        assertEquals("set_rpn_expression 1 \"rpm fsio_setting 0 >\"", prepareCommand("set_fsio_expression 1 \"rpm > fsio_setting 0\""));
    }

    @Test
    public void testSetFSIOexpressionWithFunnyQuotes() {
        assertEquals("tps > 10", AnyCommand.unquote("\"tps > 10\""));
        assertEquals("tps > 10", AnyCommand.unquote("\u201Ctps > 10\u201D"));


        assertEquals("set_rpn_expression 1 \"tps 10 >\"", prepareCommand("Set_fsio_expression 1 \u201Ctps > 10\u201D"));
    }
}
