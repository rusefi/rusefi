package com.rusefi;

import com.rusefi.common.MiscTest;
import com.rusefi.f4discovery.*;
import com.rusefi.io.CommandQueue;

/**
 * dead?
 * <p>
 * The following jumper wires are used to test some digital subsystems.
 * PD1 (output) <=> PC6 (PAL/ICU input)
 * PD2 (output) <=> PA5 (PAL/ICU input)
 *
 * Proteus has more advanced jumpers allowing for some analog domain coverage see ProteusAnalogTest
 *
 * <p>
 * <p>
 * <p>
 * this test connects to real hardware via serial port
 * Andrey Belomutskiy, (c) 2013-2020
 * 2/22/2015
 */
public class HwCiF4Discovery {
    private final static Class[] tests = {
// huh? why does this not work for discovery?        PTraceTest.class,
            CompositeLoggerTest.class,
            BurnCommandTest.class,
            MiscTest.class,
            CommonFunctionalTest.class,
            DiscoveryPwmHardwareTest.class,
            VssHardwareLoopTest.class,
            HighRevTest.class,
    };

    public static void main(String[] args) {
        /**
         * trying a random hack https://github.com/rusefi/rusefi/issues/4772
         */
        CommandQueue.DEFAULT_TIMEOUT = 4950;
        CmdJUnitRunner.runHardwareTestAndExit(tests);
    }

    static boolean runHardwareTest() {
        return CmdJUnitRunner.runHardwareTest(tests);
    }
}
