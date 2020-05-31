package com.rusefi.core.test;

import com.rusefi.core.EngineState;
import org.junit.Test;

import static junit.framework.Assert.assertFalse;
import static junit.framework.Assert.assertTrue;

/**
 * @author Andrey Belomutskiy
 *         12/26/12
 */
public class EngineStateTest {
    @Test
    public void startsWithIgnoreCase() {
        assertTrue(EngineState.startWithIgnoreCase("HELLO", "he"));
        assertFalse(EngineState.startWithIgnoreCase("HELLO", "hellllll"));
        assertFalse(EngineState.startWithIgnoreCase("HELLO", "ha"));
    }
}
