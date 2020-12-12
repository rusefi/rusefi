package com.rusefi;

import com.rusefi.config.generated.Fields;
import com.rusefi.functional_tests.BaseTest;
import com.rusefi.io.CommandQueue;
import com.rusefi.io.LinkManager;

import static com.rusefi.IoUtil.*;
import static com.rusefi.RealHwTest.startRealHardwareTest;

/**
 * this command utility confirms that rusEFI hardware stays alive for long periods of time
 */
public class EnduranceTestUtility {

    private static final int DEFAULT_COUNT = 2000;

    public static void main(String[] args) {
        LinkManager linkManager = new LinkManager();
        CommandQueue commandQueue = linkManager.getCommandQueue();
        long start = System.currentTimeMillis();
        int count = parseCount(args);
        try {
            String port = startRealHardwareTest(args);

            if (port == null) {
                System.out.println("EnduranceTest [SERIAL] [COUNT]");
                return;
            }

            FileLog.MAIN.logLine("Running " + count + " cycles");

            IoUtil.realHardwareConnect(linkManager, port);
            for (int i = 0; i < count; i++) {
                BaseTest.currentEngineType = Fields.ET_FORD_ASPIRE;
                sendCommand("set " + Fields.CMD_ENGINE_TYPE + " " + 3, BaseTest.COMPLEX_COMMAND_RETRY, Timeouts.SET_ENGINE_TIMEOUT, commandQueue);
                sleepSeconds(2);
                sendCommand(getEnableCommand("self_stimulation"), commandQueue);
//                IoUtil.changeRpm(1200);
                BaseTest.currentEngineType = Fields.ET_DEFAULT_FRANKENSO;
                sendCommand("set " + Fields.CMD_ENGINE_TYPE + " " + 28, BaseTest.COMPLEX_COMMAND_RETRY, Timeouts.SET_ENGINE_TIMEOUT, commandQueue);
                sleepSeconds(2);
                FileLog.MAIN.logLine("++++++++++++++++++++++++++++++++++++  " + i + "   +++++++++++++++");
            }

        } catch (Throwable e) {
            e.printStackTrace();
            System.exit(-1);
        }
        FileLog.MAIN.logLine("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        FileLog.MAIN.logLine("++++++++++++++++++++++++++++++++++++  YES YES YES " + count + "   +++++++++++++++");
        FileLog.MAIN.logLine("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
        long totalTime = System.currentTimeMillis() - start;
        long minutes = totalTime / 1000 / 60;
        FileLog.MAIN.logLine("In " + minutes + " minutes");
    }

    private static int parseCount(String[] args) {
        if (args.length == 2) {
            return Integer.parseInt(args[1]);
        }
        return DEFAULT_COUNT;
    }
}