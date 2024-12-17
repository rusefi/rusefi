package com.rusefi.f4discovery;

import com.rusefi.RusefiTestBase;
import com.rusefi.common.VssHardwareTestLogic;
import com.rusefi.Timeouts;
import com.rusefi.core.Sensor;
import com.rusefi.core.SensorCentral;
import com.rusefi.enums.engine_type_e;
import com.rusefi.functional_tests.EcuTestHelper;
import org.junit.Test;

import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;
import static com.rusefi.config.generated.Fields.*;

/**
 * This test relies on jumpers connecting physical pins on Discovery:
 * PD1<>PC6
 * PD2<>PA5
 */
public class VssHardwareLoopTest extends RusefiTestBase {
    @Override
    protected boolean needsHardwareTriggerInput() {
        // This test uses hardware trigger input!
        return true;
    }

    @Test
    public void test() {
        VssHardwareTestLogic.runIdleVssTest(ecu, "PD2", "PA5");
    }
}
