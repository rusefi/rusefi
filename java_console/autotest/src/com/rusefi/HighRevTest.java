package com.rusefi;

import com.rusefi.functional_tests.EcuTestHelper;
import org.junit.Ignore;
import org.junit.Test;

import static com.rusefi.functional_tests.EcuTestHelper.FAIL;
import static com.rusefi.config.generated.Fields.*;

public class HighRevTest {
    @Test
    public void testVW() {
        EcuTestHelper ecu = EcuTestHelper.createInstance();

        ecu.setEngineType(ET_VW_ABA);
        // trying to disable engine sniffer to help https://github.com/rusefi/rusefi/issues/1849
        ecu.sendCommand("set " + CMD_ENGINESNIFFERRPMTHRESHOLD + " 100");
        ecu.changeRpm(900);
        // first let's get to expected RPM
        EcuTestHelper.assertRpmDoesNotJump(16000, 5, 40, FAIL, ecu.commandQueue);
    }

    @Test
    public void testV12() {
        EcuTestHelper ecu = EcuTestHelper.createInstance();
        ecu.setEngineType(ET_BMW_M73_F);
        ecu.changeRpm(700);
        // first let's get to expected RPM
        EcuTestHelper.assertRpmDoesNotJump(16000, 5, 40, FAIL, ecu.commandQueue);
        testCaseBug1873(ecu);
    }

    private void testCaseBug1873(EcuTestHelper ecu) {
        EcuTestHelper.assertRpmDoesNotJump(60, 5, 110, FAIL, ecu.commandQueue);
    }
}
