package com.rusefi.core.test;

import com.rusefi.core.SensorCentral;
import com.rusefi.core.EngineState;
import com.rusefi.io.LinkDecoder;
import org.junit.Ignore;
import org.junit.Test;

import java.util.concurrent.atomic.AtomicInteger;

import static com.rusefi.core.EngineState.SEPARATOR;
import static junit.framework.Assert.*;

/**
 * @author Andrey Belomutskiy
 *         12/26/12
 */
public class EngineStateTest {
    @Test
    public void packUnpack() {
        String a = "rpm,100,";
        String packed = EngineState.packString(a);
        assertEquals("line:8:rpm,100,", packed);
        assertEquals(a, LinkDecoder.TEXT_PROTOCOL_DECODER.unpack(packed));
    }

    @Test
    public void startsWithIgnoreCase() {
        assertTrue(EngineState.startWithIgnoreCase("HELLO", "he"));
        assertFalse(EngineState.startWithIgnoreCase("HELLO", "hellllll"));
        assertFalse(EngineState.startWithIgnoreCase("HELLO", "ha"));
    }
}
