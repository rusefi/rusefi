package com.rusefi.proteus;

import com.rusefi.RusefiTestBase;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import org.junit.Test;

import static org.junit.Assert.assertTrue;

public class ProteusAnalogTest extends RusefiTestBase {
    @Test
    public void testVbatt() {
        double vbatt = SensorCentral.getInstance().getValue(Sensor.VBATT);

        // allow some tolerance for the supply voltage...
        assertTrue(vbatt > 11);
        assertTrue(vbatt < 13);
    }
}
