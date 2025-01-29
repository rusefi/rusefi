package com.rusefi.maintenance;

import com.rusefi.SerialPortScanner;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicBoolean;

public class BinaryProtocolExecutor {
    @FunctionalInterface
    public interface BinaryProtocolAction {
        boolean doWithBinaryProtocol(BinaryProtocol binaryProtocol);
    }
    private static boolean execute(
        final String port,
        final UpdateOperationCallbacks callbacks,
        final BinaryProtocolAction bpAction
    ) {
        final AtomicBoolean executionResult = new AtomicBoolean(false);
        try (LinkManager linkManager = new LinkManager()
            .setNeedPullText(false)
            .setNeedPullLiveData(true)
        ) {
            callbacks.logLine(String.format("Connecting to port `%s`...", port));
            try {
                if (linkManager.connect(port).await(1, TimeUnit.MINUTES)) {
                    final CountDownLatch latch = new CountDownLatch(1);
                    callbacks.logLine(String.format("Performing action on port %s...", port));
                    linkManager.execute(() -> {
                        try {
                            executionResult.set(bpAction.doWithBinaryProtocol(linkManager.getBinaryProtocol()));
                        } finally {
                            latch.countDown();
                        }
                    });
                    try {
                        if (!latch.await(1, TimeUnit.MINUTES)) {
                            callbacks.logLine(String.format("Failed perform action on port %s in a minute", port));
                        }
                    } catch (final InterruptedException e) {
                        callbacks.logLine(String.format("Action on port %s was interrupted", port));
                    }
                } else {
                    callbacks.logLine(String.format("Failed to connect to port %s in a minute", port));
                }
            } catch (final InterruptedException e) {
                callbacks.logLine(String.format("Connection to port %s was interrupted", port));
            }
        }
        return executionResult.get();
    }

    public static boolean executeWithSuspendedPortScanner(
        final String port,
        final UpdateOperationCallbacks callbacks,
        final BinaryProtocolAction bpAction
    ) {
        try {
            callbacks.logLine("Suspending port scanning...");
            try {
                SerialPortScanner.INSTANCE.suspend().await(1, TimeUnit.MINUTES);
                callbacks.logLine("Port scanning is suspended.");
            } catch (final InterruptedException e) {
                callbacks.logLine("Failed to  suspend port scanning in a minute.");
                return false;
            }
            return execute(port, callbacks, bpAction);
        } finally {
            callbacks.logLine("Resuming port scanning...");
            SerialPortScanner.INSTANCE.resume();
            callbacks.logLine("Port scanning is resumed.");
        }
    }
}
