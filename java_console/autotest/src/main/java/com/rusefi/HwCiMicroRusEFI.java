package com.rusefi;

public class HwCiMicroRusEFI {
    public static void main(String[] args) {
        CmdJUnitRunner.runHardwareTestAndExit(new Class[]{
                MreHighRevTest.class,
        });
    }
}
