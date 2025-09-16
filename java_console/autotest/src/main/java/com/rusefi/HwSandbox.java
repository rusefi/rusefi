package com.rusefi;

import com.rusefi.f4discovery.BurnCommandTest;

public class HwSandbox {
    public static void main(String[] args) {
        CmdJUnitRunner.runHardwareTestAndExit(new Class[]{
            BurnCommandTest.class,
        });
    }
}
