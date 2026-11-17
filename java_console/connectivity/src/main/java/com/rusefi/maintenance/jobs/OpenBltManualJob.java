package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.PortScanner;
import com.rusefi.core.io.ConnectedEcuTarget;
import com.rusefi.io.UpdateOperationCallbacks;

import javax.swing.*;
import java.util.concurrent.TimeUnit;

public class OpenBltManualJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    /** Result of the settings step, separate from whether programming succeeded. */
    public enum RestoreResult {
        /** Settings processing finished without known migration/write failures; not a readiness check. */
        COMPLETE,
        /** Defaults, partial migration, no backup, or an unreported legacy restoration outcome. */
        WARNING,
        FAILED,
        /** An operation such as emergency wipe deliberately does not restore settings. */
        SKIPPED
    }

    /**
     * Seam for unit tests: the three side-effecting steps wrapped by the scanner suspend/resume
     * choreography, so OpenBltManualJobTest can assert the ordering without hardware. Production
     * wires the original statics; the choreography itself stays in {@link #doJob}.
     */
    public interface FlashSteps {
        boolean ensureFirmware(UpdateOperationCallbacks callbacks, ConnectedEcuTarget target);

        boolean flash(JComponent parent, String port, UpdateOperationCallbacks callbacks, ConnectedEcuTarget target);

        void restoreCalibrations(UpdateOperationCallbacks callbacks, ConnectivityContext connectivityContext);

        /** Retain legacy implementations, but do not infer verified restoration from a void return. */
        default RestoreResult restoreCalibrationsWithResult(UpdateOperationCallbacks callbacks,
                                                           ConnectivityContext connectivityContext) {
            restoreCalibrations(callbacks, connectivityContext);
            callbacks.logLine("Settings restoration outcome was not reported. Verify settings before using the ECU.");
            return RestoreResult.WARNING;
        }
    }

    private final ConnectivityContext connectivityContext;
    private final FlashSteps steps;

    OpenBltManualJob(final PortResult port, final JComponent parent, final ConnectivityContext connectivityContext,
                     final FlashSteps steps) {
        this("Update ECU firmware", port, parent, connectivityContext, steps);
    }

    OpenBltManualJob(final String title, final PortResult port, final JComponent parent,
                     final ConnectivityContext connectivityContext, final FlashSteps steps) {
        super(title, new SerialPortWithParentComponentJobContext(port, parent));
        this.connectivityContext = connectivityContext;
        this.steps = steps;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        JobHelper.doJob(
            () -> {
                if (context.getPort().bootloaderInfo != null && context.getPort().bootloaderInfo.board != null) {
                    connectivityContext.getConnectedEcuTarget().set(context.getPort().bootloaderInfo.board);
                }
                if (!steps.ensureFirmware(callbacks, connectivityContext.getConnectedEcuTarget())) {
                    callbacks.error();
                    return;
                }
                // Take exclusive access to the port before flashing. The always-on scanner probes OpenBLT
                // ports with XCP every cycle; if a probe overlaps the flash it corrupts the firmware image.
                // suspend().await(...) blocks until the in-flight scan cycle has finished, mirroring the
                // auto path's bltUpdateFirmware. [tag:better_ux_for_flashing]
                final PortScanner scanner = connectivityContext.getPortScanner();
                try {
                    if (!scanner.suspend().await(30, TimeUnit.SECONDS)) {
                        callbacks.logLine("Timed out waiting for the serial port scanner to stop.");
                        scanner.resume();
                        callbacks.error();
                        return;
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    callbacks.logLine("Interrupted while waiting for the serial port scanner to stop.");
                    scanner.resume();
                    callbacks.error();
                    return;
                }
                final boolean flashed;
                try {
                    flashed = steps.flash(context.getParent(), context.getPort().port, callbacks,
                        connectivityContext.getConnectedEcuTarget());
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
                // off an ECU this session. Programming success must not hide failed restoration.
                // [tag:better_ux_for_flashing]
                final RestoreResult restoreResult = steps.restoreCalibrationsWithResult(callbacks, connectivityContext);
                if (restoreResult == RestoreResult.FAILED) {
                    callbacks.logLine("Firmware programming finished, but settings restoration failed. "
                        + "Review the report and verify settings before using the ECU.");
                    callbacks.error();
                } else if (restoreResult == RestoreResult.WARNING) {
                    callbacks.logLine("Firmware programming finished with settings warnings. "
                        + "Review the report and verify settings before using the ECU.");
                    callbacks.warning();
                } else {
                    callbacks.logLine(restoreResult == RestoreResult.SKIPPED
                        ? "Programming operation finished; settings restoration was intentionally skipped."
                        : "Firmware programming and settings steps finished. "
                            + "Verify the installed firmware version and settings before using the ECU.");
                    callbacks.done();
                }
            },
            onJobFinished
        );
    }
}
