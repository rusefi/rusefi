package com.rusefi;

import com.opensr5.ini.IniFileModelImpl;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.LinkManager;
import com.rusefi.simulator.SimulatorFunctionalTest;

import java.io.File;
import java.io.IOException;

/**
 * this class runs rusEFI functional tests against rusEFI simulator
 */
public class SimulatorFunctionalTestLauncher {
    static volatile boolean isHappy;
    public static void main(String[] args) {
        Thread.setDefaultUncaughtExceptionHandler((t, e) -> {
            e.printStackTrace();
            System.exit(66);
        });
        String iniFileName = args[0];
        BinaryProtocol.iniFileProvider = signature -> new IniFileModelImpl().readIniFile(iniFileName);
        boolean startSimulator = args.length > 1 && args[1].equalsIgnoreCase("start");

//        if (startSimulator) {
//            buildSimulator();
//        }

        long start = System.currentTimeMillis();
        AutotestLogging.INSTANCE.start();

        boolean failed = false;
        try {
            LinkManager linkManager = new LinkManager();
            IoUtil.connectToSimulator(linkManager, startSimulator);
            TestingUtils.installVoidEngineSnifferAction(linkManager.getCommandQueue());
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
        AutotestLogging.INSTANCE.logLine("*******************************************************************************");
        AutotestLogging.INSTANCE.logLine("**** SimulatorFunctionalTestLauncher  Looks good! *****************************");
        AutotestLogging.INSTANCE.logLine("*******************************************************************************");
        long time = (System.currentTimeMillis() - start) / 1000;
        AutotestLogging.INSTANCE.logLine("Done in " + time + "secs");
        System.exit(0); // this is a safer method eliminating the issue of non-daemon threads
    }

    private static void buildSimulator() throws IOException, InterruptedException {
        Process makeProcess = Runtime.getRuntime().exec("make -j8", null, new File("../simulator"));
        SimulatorExecHelper.dumpProcessOutput(makeProcess, null);
        makeProcess.waitFor();
    }
}
