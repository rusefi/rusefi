package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

import static com.rusefi.maintenance.JobType.OPENBLT_SWITCH;

public class OpenBltSwitchJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    public OpenBltSwitchJob(final SerialPortScanner.PortResult port, final JComponent parent) {
        super(OPENBLT_SWITCH.jobName, new SerialPortWithParentComponentJobContext(port, parent));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        ProgramSelector.rebootToOpenblt(context.getParent(), context.getPort().port, callbacks);
    }
}
