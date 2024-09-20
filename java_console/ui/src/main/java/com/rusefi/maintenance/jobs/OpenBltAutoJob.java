package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

public class OpenBltAutoJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    public OpenBltAutoJob(final SerialPortScanner.PortResult port, final JComponent parent) {
        super("OpenBLT via Serial", new SerialPortWithParentComponentJobContext(port, parent));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        ProgramSelector.flashOpenbltSerialAutomatic(context.getParent(), context.getPort(), callbacks);
    }
}
