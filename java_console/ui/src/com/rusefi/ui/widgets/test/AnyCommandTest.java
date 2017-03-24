package com.rusefi.ui.widgets.test;

import com.rusefi.ui.widgets.AnyCommand;
import org.junit.Test;

import static org.junit.Assert.assertEquals;

/**
 * (c) Andrey Belomutskiy
 * 3/9/2017.
 */
public class AnyCommandTest {
    @Test
    public void testUnquote() {
        assertEquals("1 2 3", AnyCommand.unquote("  \"1 2 3\"  "));
    }

    @Test
    public void testPrepareEvalCommand() {
        assertEquals("rpn_eval \"2 3 +\"", AnyCommand.prepareCommand("eval   \"2 + 3\" "));
    }

    @Test
    public void testSetFSIOexpression() {
        // todo: parameter order needs to be in postfix form
        assertEquals("set_rpn_expression 1 \"rpm fsio_setting 0 >\"", AnyCommand.prepareCommand("set_fsio_expression 1 \"rpm > fsio_setting 0\""));
    }
}
