package com.rusefi;

import com.devexperts.logging.Logging;

import java.util.List;

public class ScannerHelper {
    private final static Logging log = Logging.getLogging(ScannerHelper.class);

    public static void interruptThreads(List<Thread> threads) {
        for (Thread t : threads) {
            log.trace(String.format("Interrupting thread `%s`...", t.getName()));
            t.interrupt();
        }
    }
}
