package com.rusefi.f4discovery;

import com.rusefi.RusefiTestBase;
import com.rusefi.functional_tests.EcuTestHelper;
import org.junit.Ignore;
import org.junit.Test;

import static com.rusefi.functional_tests.EcuTestHelper.FAIL;
import static com.rusefi.config.generated.Fields.*;

public class HighRevTest extends RusefiTestBase {
    @Test
    public void testVW() {
        ecu.setEngineType(ET_VW_ABA);
        // trying to disable engine sniffer to help https://github.com/rusefi/rusefi/issues/1849
        ecu.sendCommand("set " + CMD_ENGINESNIFFERRPMTHRESHOLD + " 100");
        ecu.changeRpm(900);
        // first let's get to expected RPM
        EcuTestHelper.assertRpmDoesNotJump(6000, 5, 40, FAIL, ecu.commandQueue);
    }

    @Test
    public void testV12() {
        ecu.setEngineType(ET_BMW_M73_F);
        ecu.changeRpm(700);
        // first let's get to expected RPM
        EcuTestHelper.assertRpmDoesNotJump(6000, 5, 40, FAIL, ecu.commandQueue);

        // tests bug 1873
        EcuTestHelper.assertRpmDoesNotJump(60, 5, 110, FAIL, ecu.commandQueue);
    }
}
