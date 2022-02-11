package com.rusefi.binaryprotocol.test;

import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.binaryprotocol.BinaryProtocol;
import com.rusefi.io.HeartBeatListeners;
import com.rusefi.io.LinkManager;

import java.util.Date;
import java.util.concurrent.CountDownLatch;

public class SerialSandbox {
    public static void main(String[] args) {
        // todo: open issue #1
        BinaryProtocol.tsOutputSize = 100;

        long startTime = System.currentTimeMillis();

        SerialAutoChecker.AutoDetectResult autoDetectResult = PortDetector.autoDetectSerial(callbackContext -> null);
        String port = autoDetectResult.getSerialPort();
        System.out.println("Serial detected on " + port);

        HeartBeatListeners.INSTANCE.addListener(() -> {
            int seconds = (int) ((System.currentTimeMillis() - startTime) / 1000);
            System.out.println(new Date() + ": onDataArrival alive for " + seconds + " second(s) " + autoDetectResult.getSignature());
        });

        LinkManager linkManager = new LinkManager()
                .setNeedPullText(false) // todo: open issue #2
                .setNeedPullLiveData(true);
        CountDownLatch connected = linkManager.connect(port);
        if (connected.getCount() > 0)
            throw new IllegalStateException("Not connected in time");
    }
}
