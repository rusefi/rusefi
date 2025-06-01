package com.rusefi.binaryprotocol.test;

import com.opensr5.ini.field.IniField;
import com.rusefi.autodetect.PortDetector;
import com.rusefi.autodetect.SerialAutoChecker;
import com.rusefi.core.SensorCentral;
import com.rusefi.io.HeartBeatListeners;
import com.rusefi.io.LinkManager;
import com.rusefi.panama.PanamaHelper;

import java.util.Date;
import java.util.Objects;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

/**
 * basic protocol client
 *
 * @see BinaryProtocolServerSandbox
 */
public class SerialSandbox {
    public static void main(String[] args) throws Exception {
        boolean textPull = false;
        long startTime = System.currentTimeMillis();

        SerialAutoChecker.AutoDetectResult autoDetectResult = PortDetector.autoDetectSerial(callbackContext -> null);
        String port = autoDetectResult.getSerialPort();
        System.out.println("Serial detected on " + port);

        HeartBeatListeners.INSTANCE.addListener(() -> {
            int seconds = (int) ((System.currentTimeMillis() - startTime) / 1000);
            System.out.println(new Date() + ": onDataArrival alive for " + seconds + " second(s) textPull=" + textPull + " " + autoDetectResult.getSignature());
        });

        LinkManager linkManager = new LinkManager()
            .setNeedPullText(textPull) // todo: open issue #2
            .setNeedPullLiveData(true);

        try {
            linkManager.connect(port, false).await(60, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new IllegalStateException("Not connected in time");
        }

        IniField mcuSerialField = PanamaHelper.getIniField(linkManager);
        if (mcuSerialField == null) {
            throw new IllegalStateException("Older unit without MCUSERIAL?");
        }
        Objects.requireNonNull(mcuSerialField);

        SensorCentral.getInstance().addListener(() -> {
            int mcuSerial = PanamaHelper.getMcuSerial(mcuSerialField);
            System.out.println("mcuSerial " + mcuSerial);
        });

        CountDownLatch latch = new CountDownLatch(1);
        linkManager.execute(new Runnable() {
            @Override
            public void run() {
                boolean result = linkManager.getBinaryProtocol().requestOutputChannels();
                System.out.println("requestOutputChannels=" + result);
                latch.countDown();
            }
        });
        latch.await(1, TimeUnit.MINUTES);
    }
}
