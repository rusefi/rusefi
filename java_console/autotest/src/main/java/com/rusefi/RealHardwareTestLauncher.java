package com.rusefi;

import com.rusefi.f4discovery.CommonFunctionalTest;
import com.rusefi.f4discovery.HighRevTest;
import com.rusefi.f4discovery.PwmHardwareTest;
import com.rusefi.f4discovery.VssHardwareLoopTest;
import org.junit.runner.JUnitCore;
import org.junit.runner.Result;

/**
 * dead?
 * 
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

    public static void main(String[] args) {
        JUnitCore junit = new JUnitCore();
        Result result = junit.run(
            CommonFunctionalTest.class,
            PwmHardwareTest.class,
            VssHardwareLoopTest.class,
            HighRevTest.class
        );

        // Print a summary of tests run
        System.out.println("Ran " + result.getRunCount() + " tests total.");
        System.out.println("Ignored " + result.getIgnoreCount() + " tests.");
        System.out.println("Failed " + result.getFailureCount() + " tests.");

        result.getFailures().forEach(f -> {
            System.out.println(f.toString());

            System.out.println("Test failed: " + f.getTestHeader() + " because " + f.getMessage());
            System.out.println("Exception:");

            f.getException().printStackTrace();
        });
    }

    /**
     * @return true if test is a SUCCESS, false if a FAILURE
     */
    public static boolean runHardwareTest() {
        return false;
    }
}
