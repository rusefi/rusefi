package com.rusefi.common;

import com.rusefi.RusefiTestBase;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.functional_tests.EcuTestHelper;
import org.junit.Ignore;
import org.junit.Test;

import static org.junit.Assert.assertTrue;

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
        String message = "mcuTemp is " + mcuTemp;
        assertTrue(message, mcuTemp > 10);
        assertTrue(message, mcuTemp < 50);
    }
}
