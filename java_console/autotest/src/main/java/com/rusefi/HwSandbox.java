package com.rusefi;

import com.rusefi.f4discovery.BurnCommandTest;

/**
 * Runs hardware-in-the-loop tests against a real ECU (e.g. {@link BurnCommandTest}).
 */
public class HwSandbox {
    public static void main(String[] args) {
        CmdJUnitRunner.runHardwareTestAndExit(new Class[]{
            BurnCommandTest.class,
        });
    }
}
