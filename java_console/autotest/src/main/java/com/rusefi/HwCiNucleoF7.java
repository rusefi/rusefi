package com.rusefi;

import com.rusefi.common.MiscTest;
import com.rusefi.common.AdcHardwareTest;
import com.rusefi.common.MapAnalogHardwareTest;
import com.rusefi.f4discovery.*;
import com.rusefi.nucleo.NucleoPwmHardwareTest;
import com.rusefi.nucleo.NucleoVssHardwareTest;

/** Analog MAP fixture: 3V3 - 10 kOhm - PC3 - 10 kOhm - GND (see AdcHardwareTest). */
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
            AdcHardwareTest.class,
            MapAnalogHardwareTest.class,
//            CommonFunctionalTest.class,
        });
    }
}
