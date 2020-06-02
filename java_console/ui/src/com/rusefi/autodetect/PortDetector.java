package com.rusefi.autodetect;

import com.rusefi.FileLog;
import com.rusefi.io.IoStream;
import jssc.SerialPortList;
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
 * (c) Andrey Belomutskiy 2013-2019
 */
public class PortDetector {
    /**
     * Connect to all serial ports and find out which one respond first
     * @param callback
     */
    public static String autoDetectSerial(Function<IoStream, Void> callback) {
        String[] serialPorts = getPortNames();
        if (serialPorts.length == 0) {
            System.err.println("No serial ports detected");
            return null;
        }
        FileLog.MAIN.logLine("Trying " + Arrays.toString(serialPorts));
        List<Thread> serialFinder = new ArrayList<>();
        CountDownLatch portFound = new CountDownLatch(1);
        AtomicReference<String> result = new AtomicReference<>();
        for (String serialPort : serialPorts) {
            Thread thread = new Thread(new SerialAutoChecker(serialPort, portFound, result, callback));
            serialFinder.add(thread);
            thread.start();
        }
        try {
            portFound.await(5, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
//        FileLog.MAIN.logLine("Found " + result.get());
        for (Thread thread : serialFinder)
            thread.interrupt();
//        FileLog.MAIN.logLine("Returning " + result.get());
        return result.get();
    }

    private static String[] getPortNames() {
//        long now = System.currentTimeMillis();
        String[] portNames = SerialPortList.getPortNames();
//        FileLog.MAIN.logLine("Took " + (System.currentTimeMillis() - now));
        return portNames;
    }

    @Nullable
    public static String autoDetectPort(JFrame parent) {
        String autoDetectedPort = autoDetectSerial(null);
        if (autoDetectedPort == null) {
            JOptionPane.showMessageDialog(parent, "Failed to located device");
            return null;
        }
        return autoDetectedPort;
    }

    public static String autoDetectSerialIfNeeded(String port) {
        if (!isAutoPort(port))
            return port;
        return autoDetectSerial(null);
    }

    public static boolean isAutoPort(String port) {
        return port.toLowerCase().startsWith("auto");
    }
}
