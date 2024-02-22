package com.rusefi;

import com.rusefi.common.MiscTest;
import com.rusefi.f4discovery.CompositeLoggerTest;
import com.rusefi.f4discovery.HighRevTest;
import com.rusefi.f4discovery.PTraceTest;
import com.rusefi.proteus.ProteusAnalogTest;

/**
 * See ProteusAnalogTest for jumper configuration documentation
 */
public class HwCiProteus {
    public static void main(String[] args) {
        CmdJUnitRunner.runHardwareTestAndExit(new Class[]{
            PTraceTest.class,
                CompositeLoggerTest.class,
                HighRevTest.class,
                MiscTest.class,
                ProteusAnalogTest.class,
        });
    }
}
