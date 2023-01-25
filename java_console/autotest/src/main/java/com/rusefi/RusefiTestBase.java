package com.rusefi;

import com.rusefi.functional_tests.EcuTestHelper;
import com.rusefi.waves.EngineChart;
import org.junit.*;

public class RusefiTestBase {
    protected EcuTestHelper ecu;

    protected boolean needsHardwareTriggerInput() {
        // Most tests do not, but some may need it
        return false;
    }

    @Before
    public void startUp() {
        ecu = EcuTestHelper.createInstance(needsHardwareTriggerInput());
    }

    @After
    public void checkStackUsage() {
        if (ecu != null)
            ecu.sendCommand("threadsinfo");
    }

    protected EngineChart nextChart() {
        return TestingUtils.nextChart(ecu.commandQueue);
    }
}
