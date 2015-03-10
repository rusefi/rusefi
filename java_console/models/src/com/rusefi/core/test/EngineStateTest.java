package com.rusefi.core.test;

import com.rusefi.core.SensorCentral;
import com.rusefi.core.EngineState;
import com.rusefi.io.LinkManager;
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
        assertEquals(a, EngineState.unpackString(packed));
    }

    @Test
    public void startsWithIgnoreCase() {
        assertTrue(EngineState.startWithIgnoreCase("HELLO", "he"));
        assertFalse(EngineState.startWithIgnoreCase("HELLO", "hellllll"));
        assertFalse(EngineState.startWithIgnoreCase("HELLO", "ha"));
    }

    @Test
    public void testRpm() {
        final AtomicInteger rpmResult = new AtomicInteger();
        EngineState es = new EngineState(new EngineState.EngineStateListenerImpl() {
            public void onKeyValue(String key, String value) {
                if ("rpm".equals(key))
                    rpmResult.set(Integer.parseInt(value));
            }
        });
        es.processNewData("line:7:");
        es.processNewData(SensorCentral.RPM_KEY + SEPARATOR);
        assertEquals(0, rpmResult.get());
        LinkManager.connector = LinkManager.VOID;
        es.processNewData("600\r");
        assertEquals(600, rpmResult.get());
    }

    @Test
    public void testAdcRepresentation() {
        assertEquals("1025.00 (0.83v)", SensorCentral.getInternalAdcRepresentation(1025));
    }
}
