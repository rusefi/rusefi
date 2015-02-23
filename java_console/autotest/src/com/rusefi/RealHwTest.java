package com.rusefi;

import static com.rusefi.AutoTest.*;

/**
 * this test connects to real hardware via serial port
 * (c) Andrey Belomutskiy 2013-2015
 * 2/22/2015
 */
public class RealHwTest {
    public static void main(String[] args) {
        FileLog.MAIN.start();
        String port;
        if (args.length == 1) {
            port = args[0];
        } else if (args.length == 0) {
            port = IoUtil.getDefaultPort();
            if (port == null)
                return;

        } else {
            System.out.println("Only one optional argument expected: port number");
            return;
        }
        runRealHardwareTest(port);
    }

    private static void runRealHardwareTest(String port) {
        IoUtil.realHardwareConnect(port);
        mainTestBody();
    }
}
