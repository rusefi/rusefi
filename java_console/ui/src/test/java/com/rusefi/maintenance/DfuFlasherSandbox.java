package com.rusefi.maintenance;

import com.rusefi.ui.StatusConsumer;

import static com.rusefi.maintenance.DfuFlasher.detectPcan;

public class DfuFlasherSandbox {
    public static void main(String[] args) {
        System.out.println("detectPcan " + detectPcan(StatusConsumer.VOID));
    }
}
