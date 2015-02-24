package com.rusefi;

import static com.rusefi.AutoTest.setEngineType;
import static com.rusefi.RealHwTest.startRealHardwareTest;

public class EnduranceTest {
    public static void main(String[] args) {

        try {
            String port = startRealHardwareTest(args);

            if (port == null)
                return;

            IoUtil.realHardwareConnect(port);
            for (int i = 0; i < 1000; i++) {
                setEngineType(3);
                IoUtil.changeRpm(1200);
                setEngineType(28);
                IoUtil.changeRpm(2400);
                FileLog.MAIN.logLine("++++++++++++++++++++++++++++++++++++  " + i + "   +++++++++++++++");
            }

        } catch (Throwable e) {
            e.printStackTrace();
            System.exit(-1);
        }
    }
}