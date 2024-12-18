package com.rusefi;

import com.rusefi.common.MiscTest;
import com.rusefi.f4discovery.*;
import com.rusefi.nucleo.NucleoPwmHardwareTest;
import com.rusefi.nucleo.NucleoVssHardwareTest;

public class HwCiNucleoF7 {
    public static void main(String[] args) {
        CmdJUnitRunner.runHardwareTestAndExit(new Class[]{
            PTraceTest.class,
            CompositeLoggerTest.class,
            HighRevTest.class,
            NucleoPwmHardwareTest.class,
			NucleoVssHardwareTest.class,
//            MiscTest.class,
            BurnCommandTest.class,
//            CommonFunctionalTest.class,
        });
    }
}
