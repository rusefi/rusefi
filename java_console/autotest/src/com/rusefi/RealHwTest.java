package com.rusefi;

import static com.rusefi.AutoTest.*;

/**
 * this test connects to real hardware via serial port
 * (c) Andrey Belomutskiy 2013-2015
 * 2/22/2015
 */
public class RealHwTest {
    public static void main(String[] args) {
        long start = System.currentTimeMillis();
        /**
         * with real hardware we have noise on all analog inputs which gives us random sensor data, we cannot really
         * test exact numbers yet
         */
        TestingUtils.isRealHardware = true;
        FileLog.MAIN.start();
        String port;
        if (args.length == 1) {
            port = args[0];
        } else if (args.length == 0) {
            port = IoUtil.getDefaultPort();
            if (port == null)
                return;

        } else {
            System.out.println("Only one optional argument expected: port number");
            return;
        }
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

    private static void runRealHardwareTest(String port) {
        IoUtil.realHardwareConnect(port);
        mainTestBody();
    }
}
