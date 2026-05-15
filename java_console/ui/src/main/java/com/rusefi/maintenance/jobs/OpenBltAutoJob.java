package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.ProgramSelector;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class OpenBltAutoJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    private final ConnectivityContext connectivityContext;
    private final @Nullable LinkManager linkManager;

    public OpenBltAutoJob(final PortResult port, final JComponent parent, ConnectivityContext connectivityContext, final @Nullable LinkManager linkManager) {
        super("OpenBLT via Serial", new SerialPortWithParentComponentJobContext(port, parent));
        this.connectivityContext = connectivityContext;
        this.linkManager = linkManager;
    }

    @Override
    public void doJob(final UpdateOperationCallbacks callbacks, final Runnable onJobFinished) {
        try {
            final boolean ok;
            if (linkManager != null) {
                BinaryProtocol bp = awaitBinaryProtocol(callbacks);
                if (bp == null) {
                    callbacks.logLine("Timed out waiting for connection.");
                    callbacks.error();
                    return;
                }
                ok = ProgramSelector.flashOpenbltSerialAutomatic(
                    context.getParent(), context.getPort(), bp, linkManager, callbacks, connectivityContext);
            } else {
                callbacks.logLine("ERROR: No live LinkManager available — auto-connect to an ECU first.");
                callbacks.error();
                return;
            }
            if (ok) {
                callbacks.done();
            } else {
                callbacks.error();
            }
        } finally {
            // Notify the UI that the job is complete so tabs are re-enabled and the
            // "Updating" overlay is cleared before we begin the reconnect sequence.
            // TODO: maybe add some delay/progress bar/something to indicate the user that we are reconnecting to the ecu after the update
            onJobFinished.run();
        }

        // Reconnect the linkManager after the flash — this happens after onJobFinished
        // so the UI is already responsive while we wait.
        // Suspend the scanner and wait for any in-progress port inspection to
        // finish before reconnecting. SerialPortScanner.inspectPort opens the
        // same serial port; if it runs concurrently with reconnection, the open
        // fails with "no permissions" because the port is already held.
        try {
            connectivityContext.getSerialPortScanner().suspend().await(30, java.util.concurrent.TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        // Discard commands queued in the previous session so they are not
        // replayed after reconnection (ej: a stale "set date" would mismatch the
        // new session's confirmation and crash the connection).
        linkManager.getCommandQueue().clearPendingCommands();
        try {
            linkManager.reconnect();
        } finally {
            // Cache the reconnected port so the scanner does not re-inspect it on
            // the next scan cycle and compete with the newly established connection.
            connectivityContext.getSerialPortScanner().cachePort(
                new PortResult(context.getPort().port, context.getPort().type)
            );
            connectivityContext.getSerialPortScanner().resume();
        }
    }

    private BinaryProtocol awaitBinaryProtocol(UpdateOperationCallbacks callbacks) {
        BinaryProtocol bp = linkManager.getBinaryProtocol();
        if (bp != null) {
            return bp;
        }
        callbacks.logLine("Waiting for connection to stabilize...");
        CountDownLatch latch = new CountDownLatch(1);
        ConnectionStatusLogic.Listener listener = new ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                if (!isConnected || ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED) {
                    latch.countDown();
                }
            }
        };
        ConnectionStatusLogic.INSTANCE.addListener(listener);
        ConnectionStatusValue currentValue = ConnectionStatusLogic.INSTANCE.getValue();
        if (currentValue == ConnectionStatusValue.CONNECTED || currentValue == ConnectionStatusValue.NOT_CONNECTED) {
            latch.countDown();
        }
        try {
            latch.await(30, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            ConnectionStatusLogic.INSTANCE.removeListener(listener);
        }
        return linkManager.getBinaryProtocol();
    }
}
