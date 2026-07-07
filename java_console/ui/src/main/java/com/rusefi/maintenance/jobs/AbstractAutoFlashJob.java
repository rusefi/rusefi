package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.util.concurrent.TimeUnit;

abstract class AbstractAutoFlashJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    protected final ConnectivityContext connectivityContext;
    private final @Nullable LinkManager linkManager;

    protected AbstractAutoFlashJob(final String name, final PortResult port, final JComponent parent,
                                   final ConnectivityContext connectivityContext,
                                   final @Nullable LinkManager linkManager) {
        super(name, new SerialPortWithParentComponentJobContext(port, parent));
        this.connectivityContext = connectivityContext;
        this.linkManager = linkManager;
    }

    protected abstract boolean flash(LinkManager lm, BinaryProtocol bp, UpdateOperationCallbacks callbacks);

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        final LinkManager lm = linkManager;
        try {
            if (lm == null) {
                callbacks.logLine("ERROR: No live LinkManager available — auto-connect to an ECU first.");
                callbacks.error();
                return;
            }
            final BinaryProtocol bp = JobHelper.awaitBinaryProtocol(lm, callbacks);
            if (bp == null) {
                callbacks.logLine("Timed out waiting for connection.");
                callbacks.error();
                return;
            }
            if (flash(lm, bp, callbacks)) {
                callbacks.done();
            } else {
                callbacks.error();
            }
        } finally {
            // Notify the UI that the job is complete so tabs are re-enabled and the
            // "Updating" overlay is cleared before we begin the reconnect sequence.
            // TODO: maybe add some delay/progress bar/something to indicate reconnect progress
            onJobFinished.run();
        }

        // lm is non-null here — null path returned inside try. Recover the live connection even when the
        // calibration restore reported failure (the firmware itself may have flashed fine).
        lm.getCommandQueue().clearPendingCommands();
        // The board re-enumerates after reboot, often onto a *different* port
        // and sometimes after a delay. Let the always-on scanner detect + cache it, then reconnect to that
        // port. We poll the scanner's snapshot rather than running a competing PortDetector probe: that
        // probe races the scanner for the same port, fails to open it, and falls back to the stale
        // pre-flash port. Do NOT fall back to the pre-flash port — after renumbering it's a ghost. (#9771)
        connectivityContext.getSerialPortScanner().resume();
        final String detectedPort = awaitEcuPort(TimeUnit.SECONDS.toMillis(60));
        if (detectedPort != null) {
            connectivityContext.getSerialPortScanner().cachePort(new PortResult(detectedPort, context.getPort().type));
            lm.reconnect(detectedPort);
        } else {
            callbacks.logLine("ECU did not re-appear after flashing — reconnect manually once it enumerates.");
        }
    }

    /**
     * Poll the scanner's detected hardware for a connectable ECU port (the rebooted board, possibly on a
     * renumbered port), up to {@code timeoutMs}. Returns the port name, or null on timeout. Uses the
     * scanner's own detection (which caches the port, so the subsequent reconnect doesn't collide with a
     * probe) instead of a competing autodetect. (#9771)
     */
    private String awaitEcuPort(final long timeoutMs) {
        final long deadline = System.currentTimeMillis() + timeoutMs;
        while (System.currentTimeMillis() < deadline) {
            for (final PortResult p : connectivityContext.getCurrentHardware().getKnownPorts()) {
                if (p.isEcu()) {
                    return p.port;
                }
            }
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            }
        }
        return null;
    }

}
