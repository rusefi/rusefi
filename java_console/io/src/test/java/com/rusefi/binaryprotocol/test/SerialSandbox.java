package com.rusefi.binaryprotocol.test;

import com.rusefi.autodetect.PortDetector;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.HeartBeatListeners;
import com.rusefi.io.LinkManager;

import java.util.Date;
import java.util.concurrent.CountDownLatch;

public class SerialSandbox {
    public static void main(String[] args) {
        BinaryProtocol.tsOutputSize = 100;

        long startTime = System.currentTimeMillis();

        String port = PortDetector.autoDetectSerial(callbackContext -> null).getSerialPort();
        System.out.println("Serial detected on " + port);

        HeartBeatListeners.INSTANCE.addListener(() -> {
            int seconds = (int) ((System.currentTimeMillis() - startTime) / 1000);
            System.out.println(new Date() + ": onDataArrival alive for " + seconds + " second(s)");
        });

        LinkManager linkManager = new LinkManager()
                .setNeedPullText(false)
                .setNeedPullLiveData(false);
        CountDownLatch connected = linkManager.connect(port);
        if (connected.getCount() > 0)
            throw new IllegalStateException("Not connected in time");
    }
}
