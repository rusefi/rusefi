package com.rusefi;

import com.rusefi.core.SensorCentral;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

public class CmdJUnitRunnerTest {
    public static class HardwareTest {
        @org.junit.Test
        public void observesOutputDemand() {
            assertTrue(SensorCentral.getInstance().getOutputChannelDemand().isFull());
        }
    }

    @org.junit.jupiter.api.Test
    public void hardwareTestRunScopesFullOutputDemand() {
        SensorCentral sensorCentral = SensorCentral.getInstance();
        SensorCentral.ListenerToken selectiveDemand = sensorCentral.addListener("seconds", value -> { });
        try {
            assertTrue(CmdJUnitRunner.runHardwareTest(new Class[]{HardwareTest.class}));
            assertFalse(sensorCentral.getOutputChannelDemand().isFull());
        } finally {
            selectiveDemand.remove();
        }
    }
}
