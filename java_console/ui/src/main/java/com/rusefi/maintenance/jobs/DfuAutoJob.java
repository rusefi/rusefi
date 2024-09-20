package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;

import javax.swing.*;

public class DfuAutoJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    public DfuAutoJob(final SerialPortScanner.PortResult port, final JComponent parent) {
        super("DFU update", new SerialPortWithParentComponentJobContext(port, parent));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        DfuFlasher.doAutoDfu(context.getParent(), context.getPort().port, callbacks);
    }
}
