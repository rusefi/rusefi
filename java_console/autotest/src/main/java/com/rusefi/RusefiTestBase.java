package com.rusefi;

import com.rusefi.functional_tests.EcuTestHelper;
import com.rusefi.waves.EngineChart;
import org.junit.Before;

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

    protected EngineChart nextChart() {
        return TestingUtils.nextChart(ecu.commandQueue);
    }
}
