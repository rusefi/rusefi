package com.rusefi.maintenance.jobs;

import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionStatusValue;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;
import org.jetbrains.annotations.Nullable;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

public class JobHelper {
    public static void doJob(final Runnable job, final Runnable onJobFinished) {
        try {
            job.run();
        } finally {
            onJobFinished.run();
        }
    }

    public static @Nullable BinaryProtocol awaitBinaryProtocol(final LinkManager lm,
                                                                final UpdateOperationCallbacks callbacks) {
        BinaryProtocol bp = lm.getBinaryProtocol();
        if (bp != null) {
            return bp;
        }
        callbacks.logLine("Waiting for connection to stabilize...");
        final CountDownLatch latch = new CountDownLatch(1);
        final ConnectionStatusLogic.Listener listener = new ConnectionStatusLogic.Listener() {
            @Override
            public void onConnectionStatus(boolean isConnected) {
                if (!isConnected || ConnectionStatusLogic.INSTANCE.getValue() == ConnectionStatusValue.CONNECTED) {
                    latch.countDown();
                }
            }
        };
        ConnectionStatusLogic.INSTANCE.addListener(listener);
        final ConnectionStatusValue currentValue = ConnectionStatusLogic.INSTANCE.getValue();
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
        return lm.getBinaryProtocol();
    }
}
