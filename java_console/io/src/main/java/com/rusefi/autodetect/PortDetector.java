package com.rusefi.autodetect;

import com.devexperts.logging.Logging;
import com.rusefi.NamedThreadFactory;
import com.rusefi.io.LinkManager;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Function;

/**
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class PortDetector {
    private final static Logging log = Logging.getLogging(PortDetector.class);

    private static final NamedThreadFactory AUTO_DETECT_PORT = new NamedThreadFactory("AutoDetectPort");

    /**
     * Connect to all serial ports and find out which one respond first
     * @param callback
     * @return port name on which rusEFI was detected or null if none
     */
    @NotNull
    public static SerialAutoChecker.AutoDetectResult autoDetectSerial(Function<SerialAutoChecker.CallbackContext, Void> callback) {
        String rusEfiAddress = System.getProperty("rusefi.address");
        if (rusEfiAddress != null) {
            return getSignatureFromPorts(callback, new String[] {rusEfiAddress});
        }
        String[] serialPorts = getPortNames();
        if (serialPorts.length == 0) {
            log.error("No serial ports detected");
            return new SerialAutoChecker.AutoDetectResult(null, null);
        }
        log.info("Trying " + Arrays.toString(serialPorts));
        return getSignatureFromPorts(callback, serialPorts);
    }

    @NotNull
    private static SerialAutoChecker.AutoDetectResult getSignatureFromPorts(Function<SerialAutoChecker.CallbackContext, Void> callback, String[] serialPorts) {
        List<Thread> serialFinder = new ArrayList<>();
        CountDownLatch portFound = new CountDownLatch(1);
        AtomicReference<SerialAutoChecker.AutoDetectResult> result = new AtomicReference<>();
        for (String serialPort : serialPorts) {
            Thread thread = AUTO_DETECT_PORT.newThread(new Runnable() {
                @Override
                public void run() {
                    new SerialAutoChecker(serialPort, portFound).openAndCheckResponse(result, callback);
                }

                @Override
                public String toString() {
                    return serialPort + " " + super.toString();
                }
            });
            serialFinder.add(thread);
            thread.start();
        }
        try {
            portFound.await(5, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
        log.info("Now interrupting " + serialFinder);
        try {
            for (Thread thread : serialFinder) {
                log.info("Interrupting " + thread);
                thread.interrupt();
            }
        } catch (RuntimeException e) {
            log.error("Unexpected runtime", e);
        }
        log.info("Done interrupting!");

        SerialAutoChecker.AutoDetectResult autoDetectResult = result.get();
        if (autoDetectResult == null)
            autoDetectResult = new SerialAutoChecker.AutoDetectResult(null, null);
        log.debug("Found " + autoDetectResult + " now stopping threads");
//        FileLog.MAIN.logLine("Returning " + result.get());
        return autoDetectResult;
    }

    private static String[] getPortNames() {
//        long now = System.currentTimeMillis();
        String[] portNames = LinkManager.getCommPorts();
//        FileLog.MAIN.logLine("Took " + (System.currentTimeMillis() - now));
        return portNames;
    }

    @Nullable
    public static SerialAutoChecker.AutoDetectResult autoDetectPort(JFrame parent) {
        SerialAutoChecker.AutoDetectResult autoDetectedPort = autoDetectSerial(null);
        if (autoDetectedPort.getSerialPort() == null) {
            JOptionPane.showMessageDialog(parent, "Failed to located device");
            return null;
        }
        return autoDetectedPort;
    }

    public static String autoDetectSerialIfNeeded(String port) {
        if (!isAutoPort(port))
            return port;
        return autoDetectSerial(null).getSerialPort();
    }

    public static boolean isAutoPort(String port) {
        return port.toLowerCase().startsWith("auto");
    }
}
