package com.rusefi;

import com.devexperts.logging.Logging;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.config.generated.Fields;
import com.rusefi.core.EngineState;
import com.rusefi.io.LinkManager;
import org.jetbrains.annotations.NotNull;

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
public class RealHwTest {
    private static final Logging log = getLogging(RealHwTest.class);
    private static final int STARTUP_SLEEP = 20;
    private volatile static String firmwareVersion;

    public static void main(String[] args) throws InterruptedException {
        log.info("Sleeping " + STARTUP_SLEEP + " seconds to give OS time to connect VCP driver");
        Thread.sleep(STARTUP_SLEEP * SECOND);

        boolean isSuccess = runHardwareTest(args);
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
    public static boolean runHardwareTest(String[] args) {
        String port = startRealHardwareTest(args);
        if (port == null) {
            return false;
        } else {
            return runHardwareTest(port);
        }
    }

    /**
     * @return true if test is a SUCCESS, false if a FAILURE
     */
    public static boolean runHardwareTest(String port) {
        long start = System.currentTimeMillis();
        try {
            runRealHardwareTest(port);
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
            port = PortDetector.autoDetectPort(null);
        } else {
            System.out.println("Only one optional argument expected: port number");
            port = null;
        }
        return port;
    }

    private static void runRealHardwareTest(String port) {
        LinkManager linkManager = new LinkManager().setCompositeLogicEnabled(false);
        linkManager.getEngineState().registerStringValueAction(Fields.PROTOCOL_VERSION_TAG, new EngineState.ValueCallback<String>() {
            @Override
            public void onUpdate(String firmwareVersion) {
                RealHwTest.firmwareVersion = firmwareVersion;
            }
        });

        IoUtil.realHardwareConnect(linkManager, port);
        // first run tests which require real hardware
        new HardwareTests(linkManager.getCommandQueue()).runRealHardwareTests();
        if (firmwareVersion == null)
            throw new IllegalStateException("firmwareVersion has not arrived");

        // now run common part of the test which should be same on real hardware and simulator
        new AutoTest(linkManager, linkManager.getCommandQueue()).mainTestBody();
    }
}
