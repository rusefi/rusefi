package com.rusefi.maintenance;

import com.rusefi.SerialPortScanner;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;

import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

public class BinaryProtocolExecutor {
    @FunctionalInterface
    public interface BinaryProtocolAction<T> {
        T doWithBinaryProtocol(BinaryProtocol binaryProtocol);
    }
    public static <T> T execute(
        final String port,
        final UpdateOperationCallbacks callbacks,
        final BinaryProtocolAction<T> bpAction,
        final T failureResult,
        final boolean validateConfigVersionOnConnect,
        boolean isScanningForEcu) {
        final AtomicReference<T> executionResult = new AtomicReference<>(failureResult);
        try (LinkManager linkManager = new LinkManager(validateConfigVersionOnConnect)
            .setNeedPullText(false)
            .setNeedPullLiveData(true)
        ) {
            callbacks.logLine(String.format("Connecting to port `%s`...", port));
            try {
                if (linkManager.connect(port, isScanningForEcu).await(1, TimeUnit.MINUTES)) {
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

    public static <T> T executeWithSuspendedPortScanner(
        final String port,
        final UpdateOperationCallbacks callbacks,
        final BinaryProtocolAction<T> bpAction,
        final T failureResult,
        final boolean validateConfigVersionOnConnect
    ) {
        try {
            callbacks.logLine("Suspending port scanning...");
            try {
                SerialPortScanner.INSTANCE.suspend().await(1, TimeUnit.MINUTES);
                callbacks.logLine("Port scanning is suspended.");
            } catch (final InterruptedException e) {
                callbacks.logLine("Failed to  suspend port scanning in a minute.");
                return failureResult;
            }
            return execute(port, callbacks, bpAction, failureResult, validateConfigVersionOnConnect, false);
        } finally {
            callbacks.logLine("Resuming port scanning...");
            SerialPortScanner.INSTANCE.resume();
            callbacks.logLine("Port scanning is resumed.");
        }
    }
}
