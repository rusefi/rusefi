package com.rusefi;

import com.rusefi.common.MiscTest;
import com.rusefi.f4discovery.CommonFunctionalTest;
import com.rusefi.f4discovery.HighRevTest;
import com.rusefi.f4discovery.PwmHardwareTest;
import com.rusefi.f4discovery.VssHardwareLoopTest;

/**
 * dead?
 * 
 * The following jumper wires are used to test some subsystems as realistically as possible:
 * PD1 <=> PC6
 * PD2 <=> PA5
 * <p>
 * <p>
 * <p>
 * this test connects to real hardware via serial port
 * Andrey Belomutskiy, (c) 2013-2020
 * 2/22/2015
 */
public class HwCiF4Discovery {

    public static void main(String[] args) {
        boolean isOk = runHardwareTest();
        if (!isOk)
            System.exit(-1);
    }

    static boolean runHardwareTest() {
        return CmdJUnitRunner.runHardwareTest(
                new Class[]{MiscTest.class,
                        CommonFunctionalTest.class,
                        PwmHardwareTest.class,
                        VssHardwareLoopTest.class,
                        HighRevTest.class});
    }
}
