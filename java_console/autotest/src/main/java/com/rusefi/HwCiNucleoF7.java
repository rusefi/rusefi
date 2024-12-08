package com.rusefi;

import com.rusefi.common.MiscTest;
import com.rusefi.f4discovery.*;

public class HwCiNucleoF7 {
    public static void main(String[] args) {
        CmdJUnitRunner.runHardwareTestAndExit(new Class[]{
            PTraceTest.class,
            CompositeLoggerTest.class,
            HighRevTest.class,
			// removed due to extremely low temperatures on the HWCI node (or we are incorrectly reading the CPU temperature on F7)
           	// MiscTest.class, 
        });
    }
}
