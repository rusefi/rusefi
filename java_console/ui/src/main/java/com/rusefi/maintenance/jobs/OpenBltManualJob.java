package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.PortScanner;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.MaintenanceUtil;
import com.rusefi.maintenance.ProgramSelector;

import javax.swing.*;
import java.util.concurrent.TimeUnit;

public class OpenBltManualJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    private final ConnectivityContext connectivityContext;

    public OpenBltManualJob(final PortResult port, final JComponent parent, final ConnectivityContext connectivityContext) {
        super("OpenBLT via Serial", new SerialPortWithParentComponentJobContext(port, parent));
        this.connectivityContext = connectivityContext;
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
                // auto path's bltUpdateFirmware. [tag:better_ux_for_flashing]
                final PortScanner scanner = connectivityContext.getPortScanner();
                try {
                    scanner.suspend().await(30, TimeUnit.SECONDS);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
                final boolean flashed;
                try {
                    flashed = ProgramSelector.flashOpenbltSerial(context.getParent(), context.getPort().port, callbacks);
                } finally {
                    // Board reboots to fresh firmware on a possibly-different port — re-inspect it.
                    scanner.invalidatePort(context.getPort().port);
                    scanner.resume();
                }
                if (!flashed) {
                    callbacks.error();
                    return;
                }
                // Scanner is running again so restore can wait for the rebooted ECU port to reappear.
                // The auto path backs up+restores the tune around the flash; the manual path had no live
                // ECU pre-flash (board was already in the bootloader), so restore the last tune backed up
                // off an ECU this session. A hard failure here is non-fatal: the flash itself succeeded.
                // [tag:better_ux_for_flashing]
                CalibrationsHelper.restorePreviousCalibrationsAfterManualFlash(callbacks, connectivityContext);
                callbacks.done();
            },
            onJobFinished
        );
    }
}
