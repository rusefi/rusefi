package com.rusefi;

import com.rusefi.common.MiscTest;
import com.rusefi.f4discovery.CommonFunctionalTest;
import com.rusefi.f4discovery.HighRevTest;
import com.rusefi.f4discovery.PwmHardwareTest;
import com.rusefi.f4discovery.VssHardwareLoopTest;
import com.rusefi.io.CommandQueue;

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
    private final static Class[] tests = {MiscTest.class,
            CommonFunctionalTest.class,
            PwmHardwareTest.class,
            VssHardwareLoopTest.class,
            HighRevTest.class};

    public static void main(String[] args) {
        /**
         * trying a random hack https://github.com/rusefi/rusefi/issues/4772
         */
        CommandQueue.DEFAULT_TIMEOUT = 4950;
        CmdJUnitRunner.runHardwareTestAndExit(tests);
    }
}
