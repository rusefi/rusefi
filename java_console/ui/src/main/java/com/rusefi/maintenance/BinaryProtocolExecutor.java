package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.fazecast.jSerialComm.SerialPort;
import com.rusefi.ConnectivityContext;
import com.rusefi.Timeouts;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.io.UpdateOperationCallbacks;

import java.util.Arrays;
import java.util.Objects;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import static com.fazecast.jSerialComm.SerialPort.getCommPorts;
import static com.rusefi.binaryprotocol.BinaryProtocol.sleep;

public class BinaryProtocolExecutor {
    private static final long JUST_IN_CASE = Integer.getInteger("BinaryProtocolExecutor.serial_jic", 300);
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
            callbacks.logLine(msg + ": Suspending port scanning...");
            try {
                long start = System.currentTimeMillis();
                connectivityContext.getSerialPortScanner().suspend().await(1, TimeUnit.MINUTES);
                callbacks.logLine(String.format("Port scanning is suspended in %dms.", System.currentTimeMillis() - start));
            } catch (final InterruptedException e) {
                callbacks.logLine("Failed to suspend port scanning in a minute.");
                return failureResult;
            }
            log.info(msg + ": Executing " + callbacks);
            if (LinkManager.isSpecialNotSerial(port)) {
                log.info("Special " + port);
            } else {
                waitForPort(port);
            }
            return execute(port, callbacks, bpAction, failureResult, false, msg);
        } finally {
            callbacks.logLine(msg + ": Resuming port scanning...");
            connectivityContext.getSerialPortScanner().resume();
            callbacks.logLine("Port scanning is resumed.");
        }
    }

    private static void waitForPort(String port) {
        boolean portAvailable;
        {
            SerialPort[] commPorts = getCommPorts();
            portAvailable = contains(commPorts, port);
            log.info(port + ": Available right away? " + Arrays.toString(commPorts) + "; " + portAvailable);
            if (portAvailable) {
                log.info("Giving it " + JUST_IN_CASE + "ms just in case...");
                sleep(JUST_IN_CASE);
                return; // bail without extra logging
            }
        }
        // sametimes we need to wait for the port to re-appear
        long start = System.currentTimeMillis();
        while (!portAvailable && (System.currentTimeMillis() - start) < Timeouts.MINUTE) {
            sleep(200);
            portAvailable = contains(getCommPorts(), port);
        }
        log.info(port + ": Appeared: " + portAvailable + " in " + (System.currentTimeMillis() - start) + "ms");
        log.info("Giving it " + JUST_IN_CASE + "ms just in case...");
        sleep(JUST_IN_CASE);
    }

    private static boolean contains(SerialPort[] commPorts, String port) {
        Objects.requireNonNull(port, "port");
        for (SerialPort p : commPorts) {
            if (port.equals(p.getSystemPortName())) {
                return true;
            }
        }
        return false;
    }
}
