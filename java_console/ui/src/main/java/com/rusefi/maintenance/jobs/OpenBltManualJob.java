package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

public class OpenBltManualJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    public OpenBltManualJob(final SerialPortScanner.PortResult port, final JComponent parent) {
        super("OpenBLT via Serial", new SerialPortWithParentComponentJobContext(port, parent));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        ProgramSelector.flashOpenbltSerialJni(context.getParent(), context.getPort().port, callbacks);
    }
}
