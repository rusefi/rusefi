package com.rusefi.maintenance.jobs;

import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;

import static com.rusefi.maintenance.JobType.DFU_SWITCH;

public class DfuSwitchJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    public DfuSwitchJob(final SerialPortScanner.PortResult port, final JComponent parent) {
        super(DFU_SWITCH.jobName, new SerialPortWithParentComponentJobContext(port, parent));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks) {
        ProgramSelector.rebootToDfu(context.getParent(), context.getPort().port, callbacks);
    }
}
