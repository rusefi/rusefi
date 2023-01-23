package com.rusefi;

import com.rusefi.common.MiscTest;
import com.rusefi.proteus.ProteusAnalogTest;

public class HwCiF4Proteus {
    public static void main(String[] args) {
        boolean isOk = CmdJUnitRunner.runHardwareTest(new Class[]{
                MiscTest.class,
                ProteusAnalogTest.class,
        });
        if (!isOk)
            System.exit(-1);
    }
}
