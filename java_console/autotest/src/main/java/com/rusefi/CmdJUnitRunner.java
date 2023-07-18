package com.rusefi;

import org.junit.runner.JUnitCore;
import org.junit.runner.Result;

import java.util.Set;

public class CmdJUnitRunner {
    public static void runHardwareTestAndExit(Class[] classes) {
        boolean isOk = runHardwareTest(classes);
        Set<Thread> threads = Thread.getAllStackTraces().keySet();
        System.out.println("Have we left any non-daemon threads?");
        System.out.printf("%-15s \t %-15s \t %-15s \t %s\n", "Name", "State", "Priority", "isDaemon");
        for (Thread t : threads) {
            System.out.printf("%-15s \t %-15s \t %-15d \t %s\n", t.getName(), t.getState(), t.getPriority(), t.isDaemon());
        }
        System.exit(isOk ? 0 : -1);
    }

    /**
     * @param classes
     * @return true if test is a SUCCESS, false if a FAILURE
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
