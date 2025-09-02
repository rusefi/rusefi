package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.rusefi.ConnectivityContext;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;

import java.util.Arrays;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import static com.fazecast.jSerialComm.SerialPort.getCommPorts;

public class BinaryProtocolExecutor {
    private static final Logging log = Logging.getLogging(BinaryProtocolExecutor.class);

    @FunctionalInterface
    public interface BinaryProtocolAction<T> {
        T doWithBinaryProtocol(BinaryProtocol binaryProtocol);
    }

    public static <T> T execute(
        final String port,
        final UpdateOperationCallbacks callbacks,
        final BinaryProtocolAction<T> bpAction,
        final T failureResult,
        boolean isScanningForEcu, String msg) {
        final AtomicReference<T> executionResult = new AtomicReference<>(failureResult);
        try (LinkManager linkManager = new LinkManager()
            .setNeedPullText(false)
            .setNeedPullLiveData(true)
        ) {
            callbacks.logLine(String.format(msg + ": Connecting to port `%s`...", port));
            try {
                if (linkManager.connect(port, isScanningForEcu).await(1, TimeUnit.MINUTES)) {
                    final CountDownLatch latch = new CountDownLatch(1);
                    callbacks.logLine(String.format(msg + ": Performing action on port %s...", port));
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
        final T failureResult, ConnectivityContext connectivityContext,
        String msg) {
        try {
            callbacks.logLine("Suspending port scanning...");
            try {
                long start = System.currentTimeMillis();
                connectivityContext.getSerialPortScanner().suspend().await(1, TimeUnit.MINUTES);
                callbacks.logLine(String.format("Port scanning is suspended in %dms.", System.currentTimeMillis() - start));
            } catch (final InterruptedException e) {
                callbacks.logLine("Failed to suspend port scanning in a minute.");
                return failureResult;
            }
            log.info(msg + ": Executing " + callbacks);
            if (!LinkManager.isSpecialNotSerial(port)) {
                log.info("Special " + port);
            } else {
                log.info("Currently available: " + Arrays.toString(getCommPorts()));
            }
            return execute(port, callbacks, bpAction, failureResult, false, msg);
        } finally {
            callbacks.logLine("Resuming port scanning...");
            connectivityContext.getSerialPortScanner().resume();
            callbacks.logLine("Port scanning is resumed.");
        }
    }
}
