package com.rusefi;

import com.rusefi.common.MiscTest;
import com.rusefi.proteus.ProteusAnalogTest;

public class HwCiProteus {
    public static void main(String[] args) {
        CmdJUnitRunner.runHardwareTestAndExit(new Class[]{
                MiscTest.class,
                ProteusAnalogTest.class,
        });
    }
}
