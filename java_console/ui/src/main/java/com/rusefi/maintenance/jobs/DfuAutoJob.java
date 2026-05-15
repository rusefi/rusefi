package com.rusefi.maintenance.jobs;

import com.rusefi.ConnectivityContext;
import com.rusefi.PortResult;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.DfuFlasher;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class DfuAutoJob extends AsyncJobWithContext<SerialPortWithParentComponentJobContext> {
    private final ConnectivityContext connectivityContext;
    private final @Nullable LinkManager linkManager;

    public DfuAutoJob(final PortResult port, final JComponent parent, ConnectivityContext connectivityContext,
                      final @Nullable LinkManager linkManager) {
        super("DFU update", new SerialPortWithParentComponentJobContext(port, parent));
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
                ok = DfuFlasher.doAutoDfu(context.getParent(), context.getPort(), bp, linkManager, callbacks, connectivityContext);
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
            onJobFinished.run();
        }
        try {
            connectivityContext.getSerialPortScanner().suspend().await(30, java.util.concurrent.TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        linkManager.getCommandQueue().clearPendingCommands();
        try {
            linkManager.reconnect();
        } finally {
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
