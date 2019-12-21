package com.rusefi;

import com.rusefi.io.LinkManager;
import org.jetbrains.annotations.NotNull;

import static com.rusefi.AutoTest.mainTestBody;
import static com.rusefi.Timeouts.SECOND;

/**
 * this test connects to real hardware via serial port
 * (c) Andrey Belomutskiy 2013-2018
 * 2/22/2015
 */
public class RealHwTest {
    private static final int STARTUP_SLEEP = 45;

    public static void main(String[] args) throws InterruptedException {
        System.out.println("Sleeping " + STARTUP_SLEEP + " seconds to give OS time to connect VCP driver");
        Thread.sleep(STARTUP_SLEEP * SECOND);
        long start = System.currentTimeMillis();

        boolean isSuccess = runHardwareTest(args, start);
        if (!isSuccess)
            System.exit(-1);
        FileLog.MAIN.logLine("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        FileLog.MAIN.logLine("++++++++++++++++++++++++++++++++++++  Real Hardware Test Passed +++++++++++++++");
        FileLog.MAIN.logLine("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        System.exit(0); // this is a safer method eliminating the issue of non-daemon threads
    }

    /**
     * @return true if test is a SUCCESS, false if a FAILURE
     */
    public static boolean runHardwareTest(String[] args, long start) {
        String port = startRealHardwareTest(args);
        if (port == null) {
            return false;
        } else {
            try {
                runRealHardwareTest(port);
            } catch (Throwable e) {
                e.printStackTrace();
                return false;
            }
            long time = (System.currentTimeMillis() - start) / 1000;
            FileLog.MAIN.logLine("Done in " + time + "secs");
        }
        return true;
    }

    static String startRealHardwareTest(@NotNull String[] args) {
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

    private static void runRealHardwareTest(String port) throws Exception {
        IoUtil.realHardwareConnect(port);
        mainTestBody();
    }
}
