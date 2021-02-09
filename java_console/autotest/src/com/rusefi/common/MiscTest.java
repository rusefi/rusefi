package com.rusefi.common;

import com.rusefi.RusefiTestBase;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.functional_tests.EcuTestHelper;
import org.junit.Ignore;
import org.junit.Test;

public class MiscTest extends RusefiTestBase {
    @Test
    public void burn() {
        BinaryProtocol bp = ecu.getLinkManager().getCurrentStreamState();
        // let's make sure 'burn' command works since sometimes it does not
        bp.burn();
    }

    @Test
    public void testMcuTemperature() {
        double mcuTemp = SensorCentral.getInstance().getValue(Sensor.INT_TEMP);

        System.out.println("MCU temperature is " + mcuTemp + " deg C");

        // You are probably indoors and not on fire
        assertTrue(mcuTemp > 10);
        assertTrue(mcuTemp < 50);
    }
}
