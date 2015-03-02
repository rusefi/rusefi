package com.rusefi;

import static com.rusefi.IoUtil.sendCommand;
import static com.rusefi.IoUtil.sleep;
import static com.rusefi.RealHwTest.startRealHardwareTest;

public class EnduranceTest {

    private static final int DEFAULT_COUNT = 2000;

    public static void main(String[] args) {
        long start = System.currentTimeMillis();
        int count = parseCount(args);
        try {
            String port = startRealHardwareTest(args);

            if (port == null) {
                System.out.println("EnduranceTest [SERIAL] [COUNT]");
                return;
            }

            FileLog.MAIN.logLine("Running " + count + " cycles");

            IoUtil.realHardwareConnect(port);
            for (int i = 0; i < count; i++) {
                AutoTest.currentEngineType = 3;
                sendCommand("set_engine_type " + 3, AutoTest.COMPLEX_COMMAND_RETRY, 600);
                sleep(2);
                sendCommand("enable self_stimulation");
//                IoUtil.changeRpm(1200);
                AutoTest.currentEngineType = 28;
                sendCommand("set_engine_type " + 28, AutoTest.COMPLEX_COMMAND_RETRY, 600);
                sleep(2);
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