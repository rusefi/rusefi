package com.irnems.core.test;

import com.irnems.core.SensorCentral;
import com.irnems.core.EngineState;
import org.junit.Test;

import java.util.concurrent.atomic.AtomicInteger;

import static com.irnems.core.EngineState.SEPARATOR;
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
        es.append("line:7:");
        es.append(EngineState.RPM_KEY + SEPARATOR);
        assertEquals(0, rpmResult.get());
        es.append("600\r");
        assertEquals(600, rpmResult.get());
    }

    @Test
    public void testAdcRepresentation() {
        assertEquals("1025.00 (0.83v)", SensorCentral.getInternalAdcRepresentation(1025));
    }
}
