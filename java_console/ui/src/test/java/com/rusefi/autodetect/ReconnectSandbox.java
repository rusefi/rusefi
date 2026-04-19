package com.rusefi.autodetect;

import com.rusefi.IoUtil;
import com.rusefi.io.ConnectionStatusLogic;
import com.rusefi.io.ConnectionWatchdog;
import com.rusefi.io.LinkManager;

import java.util.concurrent.atomic.AtomicBoolean;

public class ReconnectSandbox {
    public static void main(String[] args) throws InterruptedException {

        LinkManager linkManager = new LinkManager();

        waitForDeviceAndStart(linkManager);

        AtomicBoolean status = new AtomicBoolean();

        ConnectionStatusLogic.INSTANCE.addListener(isConnected -> status.set(isConnected));

        while (true) {
            System.out.println("Hello " + status);
            Thread.sleep(1000);
        }
    }

    private static String detectPortUntilDetected() {
        while (true) {
            String port = PortDetector.autoDetectSerial(null).getSerialPort();
            System.out.println("detectPortUntilDetected " + port);
            if (port != null)
                return port;
            IoUtil.sleepSeconds(1);
        }
    }

    public static void waitForDeviceAndStart(LinkManager linkManager) {
        String autoDetectedPort = detectPortUntilDetected();
        System.out.println("First time port detected: " + autoDetectedPort);

        linkManager.startAndConnect(autoDetectedPort, ConnectionStatusLogic.Listener.VOID);

        ConnectionWatchdog.init(linkManager);
    }
}
