package com.rusefi.maintenance;

import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.ui.StatusConsumer;

import static com.rusefi.maintenance.DfuFlasher.detectPcan;
import static com.rusefi.maintenance.DfuFlasher.detectStLink;

public class DfuFlasherSandbox {
    public static void main(String[] args) {
        System.out.println("detectStLink " + detectStLink(UpdateOperationCallbacks.DUMMY));
        System.out.println("detectPcan " + detectPcan(UpdateOperationCallbacks.DUMMY));
    }
}
