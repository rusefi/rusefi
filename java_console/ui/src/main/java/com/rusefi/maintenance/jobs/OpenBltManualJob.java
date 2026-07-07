package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.MaintenanceUtil;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;
import java.util.concurrent.TimeUnit;

public class OpenBltManualJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    public OpenBltManualJob(final PortResult port, final JComponent parent) {
        super("OpenBLT via Serial", new SerialPortWithParentComponentJobContext(port, parent));
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(
            () -> {
                if (!MaintenanceUtil.ensureFirmwareForConnectedTarget(callbacks)) {
                    callbacks.error();
                    return;
                }
                // Take exclusive access to the port before flashing. The always-on scanner probes OpenBLT
                // ports with XCP every cycle; if a probe overlaps the flash it corrupts the firmware image.
                // suspend().await(...) blocks until the in-flight scan cycle has finished, mirroring the
                // auto path's bltUpdateFirmware. (#9771)
                final SerialPortScanner scanner = ConnectivityContext.INSTANCE.getSerialPortScanner();
                try {
                    scanner.suspend().await(30, TimeUnit.SECONDS);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
                try {
                    if (ProgramSelector.flashOpenbltSerial(context.getParent(), context.getPort().port, callbacks)) {
                        callbacks.done();
                    } else {
                        callbacks.error();
                    }
                } finally {
                    // Board reboots to fresh firmware on a possibly-different port — re-inspect it.
                    scanner.invalidatePort(context.getPort().port);
                    scanner.resume();
                }
            },
            onJobFinished
        );
    }
}
