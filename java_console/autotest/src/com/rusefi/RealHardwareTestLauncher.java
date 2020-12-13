package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.io.LinkManager;

import java.io.PrintWriter;
import java.io.StringWriter;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.Timeouts.SECOND;

/**
 * The following jumper wires are used to test some subsystems as realistically as possible:
 * PD1 <=> PC6
 * PD2 <=> PA5
 * <p>
 * <p>
 * <p>
 * this test connects to real hardware via serial port
 * Andrey Belomutskiy, (c) 2013-2020
 * 2/22/2015
 */
public class RealHardwareTestLauncher {
    private static final Logging log = getLogging(RealHardwareTestLauncher.class);
    public static final String HW_TESTS_START_UP_SLEEP = "hw_tests_start_up_sleep";
    private static final int STARTUP_SLEEP = Integer.getInteger(HW_TESTS_START_UP_SLEEP, 0);

    public static void main(String[] args) throws InterruptedException {
        if (STARTUP_SLEEP != 0) {
            log.info("Sleeping " + STARTUP_SLEEP + " seconds to give OS time to connect VCP driver");
            Thread.sleep(STARTUP_SLEEP * SECOND);
        } else {
            log.info(HW_TESTS_START_UP_SLEEP + " VM option not specified, no start-up sleep in java code");
        }

        boolean isSuccess = runHardwareTest();
        if (!isSuccess)
            System.exit(-1);
        log.info("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        log.info("++++++++++++++++++++++++++++++++++++  Real Hardware Test Passed +++++++++++++++");
        log.info("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        System.exit(0); // this is a safer method eliminating the issue of non-daemon threads
    }

    /**
     * @return true if test is a SUCCESS, false if a FAILURE
     */
    public static boolean runHardwareTest() {
        long start = System.currentTimeMillis();
        try {
            runRealHardwareTest(ControllerConnectorState.getLinkManager());
        } catch (Throwable e) {
            StringWriter sw = new StringWriter();
            PrintWriter pw = new PrintWriter(sw);
            e.printStackTrace(pw);
            log.info("Test failed: " + e + " at " + sw);
            e.printStackTrace();
            return false;
        }
        long time = (System.currentTimeMillis() - start) / 1000;
        log.info("Done in " + time + "secs");
        return true;
    }

    private static void runRealHardwareTest(LinkManager linkManager) {
        // first run tests which require real hardware
        new HardwareTests(linkManager.getCommandQueue()).runRealHardwareTests();
        if (ControllerConnectorState.firmwareVersion == null)
            throw new IllegalStateException("firmwareVersion has not arrived");

        // now run common part of the test which should be same on real hardware and simulator
        new FunctionalTestsSuite(linkManager, linkManager.getCommandQueue()).mainTestBody();
    }
}
