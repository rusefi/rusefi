package com.rusefi.maintenance;

import com.rusefi.io.UpdateOperationCallbacks;

import static com.rusefi.maintenance.MaintenanceUtil.detectPcan;
import static com.rusefi.maintenance.StLinkFlasher.detectStLink;

public class DfuFlasherSandbox {
    public static void main(String[] args) {
        System.out.println("detectStLink " + detectStLink(UpdateOperationCallbacks.DUMMY));
        System.out.println("detectPcan " + detectPcan(UpdateOperationCallbacks.DUMMY));
    }
}
