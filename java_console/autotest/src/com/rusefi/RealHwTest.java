package com.rusefi;

import com.rusefi.io.LinkManager;

import static com.rusefi.AutoTest.*;

/**
 * this test connects to real hardware via serial port
 * (c) Andrey Belomutskiy 2013-2017
 * 2/22/2015
 */
public class RealHwTest {
    private static final int STARTUP_SLEEP = 45;
    private static final long SECOND = 1000L;

    public static void main(String[] args) throws InterruptedException {
        System.out.println("Sleeping " + STARTUP_SLEEP + " seconds to give OS time to connect VCP driver");
        Thread.sleep(STARTUP_SLEEP * SECOND);
        long start = System.currentTimeMillis();
        String port = startRealHardwareTest(args);

        if (port == null)
            return;
        boolean failed = false;
        try {
            runRealHardwareTest(port);
        } catch (Throwable e) {
            e.printStackTrace();
            failed = true;
        }
        if (failed)
            System.exit(-1);
        FileLog.MAIN.logLine("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        FileLog.MAIN.logLine("++++++++++++++++++++++++++++++++++++  Real Hardware Test Passed +++++++++++++++");
        FileLog.MAIN.logLine("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        long time = (System.currentTimeMillis() - start) / 1000;
        FileLog.MAIN.logLine("Done in " + time + "secs");
        System.exit(0); // this is a safer method eliminating the issue of non-daemon threads
    }

    static String startRealHardwareTest(String[] args) {
        /**
         * with real hardware we have noise on all analog inputs which gives us random sensor data, we cannot really
         * test exact numbers yet
         */
        TestingUtils.isRealHardware = true;
        FileLog.MAIN.start();
        String port;
        if (args.length == 1 || args.length == 2) {
            port = args[0];
        } else if (args.length == 0) {
            port = LinkManager.getDefaultPort();
        } else {
            System.out.println("Only one optional argument expected: port number");
            port = null;
        }
        return port;
    }

    private static void runRealHardwareTest(String port) {
        IoUtil.realHardwareConnect(port);
        mainTestBody();
    }
}
