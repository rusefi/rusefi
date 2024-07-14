package com.rusefi.f4discovery;

import com.rusefi.RusefiTestBase;
import com.rusefi.enums.engine_type_e;
import com.rusefi.functional_tests.EcuTestHelper;
import org.junit.Test;

import static com.rusefi.config.generated.Integration.CMD_ENGINESNIFFERRPMTHRESHOLD;
import static com.rusefi.functional_tests.EcuTestHelper.FAIL;

public class HighRevTest extends RusefiTestBase {
    @Test
    public void testVW() {
        runHighRevTest(engine_type_e.VW_ABA, ecu);
    }

    @Test
    public void testV12() {
        runHighRevTest(engine_type_e.FRANKENSO_BMW_M73_F, ecu);

        // tests bug 1873
        EcuTestHelper.assertRpmDoesNotJump(60, 5, 110, FAIL, ecu.commandQueue);
    }

    public static void runHighRevTest(engine_type_e engine_type_e, EcuTestHelper ecu1) {
        ecu1.setEngineType(engine_type_e);
        // trying to disable engine sniffer to help https://github.com/rusefi/rusefi/issues/1849
        ecu1.sendCommand("set " + CMD_ENGINESNIFFERRPMTHRESHOLD + " 100");
        ecu1.changeRpm(900);
        // first let's get to expected RPM
        EcuTestHelper.assertRpmDoesNotJump(6000, 5, 40, FAIL, ecu1.commandQueue);
    }
}
