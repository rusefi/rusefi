package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

public class DfuSwitchJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    public DfuSwitchJob(final SerialPortScanner.PortResult port, final JComponent parent) {
        super("DFU switch", new SerialPortWithParentComponentJobContext(port, parent));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        ProgramSelector.rebootToDfu(context.getParent(), context.getPort().port, callbacks);
    }
}
