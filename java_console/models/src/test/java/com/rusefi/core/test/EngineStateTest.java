package com.rusefi.core.test;

import com.rusefi.core.EngineState;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

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
