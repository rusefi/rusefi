package com.rusefi.binaryprotocol.test;

import com.rusefi.autodetect.PortDetector;
import com.rusefi.io.LinkManager;

import java.util.concurrent.CountDownLatch;

public class SerialSandbox {
    public static void main(String[] args) {
        String port = PortDetector.autoDetectSerial(callbackContext -> null).getSerialPort();
        System.out.println("Serial detected on " + port);

        LinkManager linkManager = new LinkManager();
        CountDownLatch connected = linkManager.connect(port);
        if (connected.getCount() > 0)
            throw new IllegalStateException("Not connected in time");


    }
}
