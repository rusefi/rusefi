package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

public class OpenBltSwitchJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    public OpenBltSwitchJob(final SerialPortScanner.PortResult port, final JComponent parent) {
        super("OpenBLT switch", new SerialPortWithParentComponentJobContext(port, parent));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        ProgramSelector.rebootToOpenblt(context.getParent(), context.getPort().port, callbacks);
    }
}
