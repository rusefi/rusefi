package com.rusefi.maintenance;

import com.rusefi.io.UpdateOperationCallbacks;

import static com.rusefi.maintenance.DfuFlasher.detectSTM32BootloaderDriverState;
import static com.rusefi.maintenance.MaintenanceUtil.detectPcan;
import static com.rusefi.maintenance.StLinkFlasher.detectStLink;

/**
 * Manual harness that probes for DFU/ST-Link flashing hardware.
 */
public class DfuFlasherSandbox {
    public static void main(String[] args) {
        System.out.println("DFU " + detectSTM32BootloaderDriverState(UpdateOperationCallbacks.DUMMY));
        System.out.println("detectStLink " + detectStLink(UpdateOperationCallbacks.DUMMY));
        System.out.println("detectPcan " + detectPcan(UpdateOperationCallbacks.DUMMY));
    }
}
