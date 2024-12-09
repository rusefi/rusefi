package com.rusefi;

import com.rusefi.common.MiscTest;
import com.rusefi.f4discovery.PTraceTest;
import com.rusefi.f4discovery.CompositeLoggerTest;
import com.rusefi.f4discovery.HighRevTest;
import com.rusefi.f4discovery.CommonFunctionalTest;
import com.rusefi.f4discovery.BurnCommandTest;

import com.rusefi.f7nucleo.PwmHardwareTest;
import com.rusefi.f7nucleo.VssHardwareLoopTest;

public class HwCiNucleoF7 {
    public static void main(String[] args) {
        CmdJUnitRunner.runHardwareTestAndExit(new Class[]{
            PTraceTest.class,
            CompositeLoggerTest.class,
            HighRevTest.class,
           	MiscTest.class,
            BurnCommandTest.class,
            CommonFunctionalTest.class,
            VssHardwareLoopTest.class,
			//not ok
            //PwmHardwareTest.class,
        });
    }
}
