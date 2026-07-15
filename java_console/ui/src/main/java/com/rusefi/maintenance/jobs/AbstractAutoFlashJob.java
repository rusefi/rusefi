package com.rusefi.maintenance.jobs;

import com.rusefi.AvailableHardware;
import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.SerialPortType;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.util.concurrent.TimeUnit;

abstract class AbstractAutoFlashJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    /** Wall-clock + sleep seam so {@link #awaitEcuPort} is testable without real time. */
    interface Clock {
        long millis();
        void sleep(long ms) throws InterruptedException;
    }

    static final Clock SYSTEM_CLOCK = new Clock() {
        @Override
        public long millis() {
            return System.currentTimeMillis();
        }

        @Override
        public void sleep(final long ms) throws InterruptedException {
            Thread.sleep(ms);
        }
    };

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

            // Recover the live connection even when calibration restore failed: the firmware itself may
            // have flashed successfully. Keep the job busy until recovery finishes so no other action can
            // take the port between programming and reconnect.
            lm.getCommandQueue().clearPendingCommands();
            connectivityContext.getPortScanner().resume();
            final String detectedPort = awaitEcuPort(TimeUnit.SECONDS.toMillis(60), SYSTEM_CLOCK);
            if (detectedPort != null) {
                connectivityContext.getPortScanner().cachePort(new PortResult(detectedPort, context.getPort().type));
                lm.reconnect(detectedPort);
            } else {
                callbacks.logLine("ECU did not re-appear after flashing — reconnect manually once it enumerates.");
            }
        } finally {
            onJobFinished.run();
        }
    }

    /**
     * Poll the scanner's detected hardware for a connectable ECU port (the rebooted board, possibly on a
     * renumbered port), up to {@code timeoutMs}. Returns the port name, or null on timeout. Uses the
     * scanner's own detection (which caches the port, so the subsequent reconnect doesn't collide with a
     * probe) instead of a competing autodetect. [tag:better_ux_for_flashing]
     */
    // package-private + injected Clock so AbstractAutoFlashJobAwaitEcuPortTest can drive the
    // renumber-follow / bootloader-grace decisions without real time. Production passes SYSTEM_CLOCK.
    String awaitEcuPort(final long timeoutMs, final Clock clock) {
        final long start = clock.millis();
        final long deadline = start + timeoutMs;
        // A *successful* flash reboots the board through the bootloader back to firmware, so a brief
        // bootloader/Unknown flicker while it re-enumerates is normal — give the ECU time to come up
        // before concluding (only on a *persistent* bootloader) that the flash was interrupted and no ECU
        // is coming. This avoids bailing before the ECU is ready. [tag:better_ux_for_flashing]
        final long bootloaderGraceMs = 5_000;
        while (clock.millis() < deadline) {
            final AvailableHardware hw = connectivityContext.getCurrentHardware();
            for (final PortResult p : hw.getKnownPorts()) {
                if (p.isEcu()) {
                    return p.port;
                }
            }
            final boolean inBootloader = !hw.getKnownPorts(SerialPortType.OpenBlt).isEmpty() || hw.isDfuFound();
            if (inBootloader && (clock.millis() - start) > bootloaderGraceMs) {
                // Persistently a bootloader (interrupted flash) — stop waiting so we don't hold the job
                // executor and block the user's next flash attempt.
                return null;
            }
            try {
                clock.sleep(500);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            }
        }
        return null;
    }

}
