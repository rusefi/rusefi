package com.rusefi;

import org.junit.runner.JUnitCore;
import org.junit.runner.Result;

public class CmdJUnitRunner {
    /**
     * @return true if test is a SUCCESS, false if a FAILURE
     * @param classes
     */
    public static boolean runHardwareTest(Class[] classes) {
        JUnitCore junit = new JUnitCore();
        Result result = junit.run(
                classes
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
        return result.getRunCount() > 0 && result.getFailureCount() == 0;
    }
}
