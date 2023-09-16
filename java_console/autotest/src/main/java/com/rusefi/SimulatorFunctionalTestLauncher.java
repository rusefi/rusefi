package com.rusefi;

import com.rusefi.io.LinkManager;
import com.rusefi.simulator.SimulatorFunctionalTest;

/**
 * this class runs rusEFI functional tests against rusEFI simulator
 * As of Dec 2020 this seems very broken?
 */
public class SimulatorFunctionalTestLauncher {
    static boolean isHappy;
    public static void main(String[] args) {
        Thread.setDefaultUncaughtExceptionHandler((t, e) -> {
            e.printStackTrace();
            System.exit(-1);
        });
        boolean startSimulator = args.length == 1 && args[0].equalsIgnoreCase("start");

        long start = System.currentTimeMillis();
        FileLog.SIMULATOR_CONSOLE.start();
        FileLog.MAIN.start();

        boolean failed = false;
        try {
            LinkManager linkManager = new LinkManager();
            IoUtil.connectToSimulator(linkManager, startSimulator);
            new SimulatorFunctionalTest(linkManager).mainTestBody();
        } catch (Throwable e) {
            e.printStackTrace();
            failed = true;
        } finally {
            SimulatorExecHelper.destroy();
        }
        if (failed)
            System.exit(-1);
        isHappy = true;
        FileLog.MAIN.logLine("*******************************************************************************");
        FileLog.MAIN.logLine("**** SimulatorFunctionalTestLauncher  Looks good! *****************************");
        FileLog.MAIN.logLine("*******************************************************************************");
        long time = (System.currentTimeMillis() - start) / 1000;
        FileLog.MAIN.logLine("Done in " + time + "secs");
        System.exit(0); // this is a safer method eliminating the issue of non-daemon threads
    }
}
