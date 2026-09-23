package com.rusefi.common;

import com.rusefi.RusefiTestBase;
import org.junit.Test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

/**
 * Checks slow and fast MAP voltage. See {@link AdcHardwareTest} for wiring.
 *
 * Set HARDWARE_CI_MAP_VOLTAGE to the fixture's expected scaled voltage:
 * F407 Discovery: 3.0 V. F767 Nucleo: 3.3 V (the default).
 * UAEFI Rev E: set HARDWARE_CI_MAP_VOLTAGE=5.0 for the IAT-to-MAP jumper.
 * Allows 10% voltage tolerance, like the Proteus supply test.
 */
public class MapAnalogHardwareTest extends RusefiTestBase {
    @Test
    public void testSlowMapAnalogVoltage() {
        assertMapVoltage("rawMap");
    }

    @Test
    public void testFastMapAnalogVoltage() {
        assertMapVoltage("rawMapFast");
    }

    private void assertMapVoltage(String channel) {
        String configuredVoltage = System.getenv("HARDWARE_CI_MAP_VOLTAGE");
        double expectedVoltage = configuredVoltage == null ? 3.3 : Double.parseDouble(configuredVoltage);
        assertTrue("Expected fixture voltage must be finite and positive",
                Double.isFinite(expectedVoltage) && expectedVoltage > 0);

        // Check each fresh sample.
        for (double voltage : AdcHardwareTest.awaitFreshSamples(channel, 4)) {
            assertEquals(channel + " fixture voltage", expectedVoltage, voltage, expectedVoltage * 0.1);
        }
    }
}
