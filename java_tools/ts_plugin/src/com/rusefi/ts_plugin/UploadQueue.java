package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.shared.FileUtil;
import com.rusefi.tools.online.Online;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.AuthTokenPanel;

import javax.xml.bind.JAXBException;
import java.io.File;
import java.io.IOException;
import java.util.concurrent.LinkedBlockingDeque;

public class UploadQueue {
    public static final String OUTBOX_FOLDER = FileUtil.RUSEFI_SETTINGS_FOLDER + File.separator + "outbox";
    private static LinkedBlockingDeque<Msq> queue = new LinkedBlockingDeque<>(128);

    private static boolean isStarted;

    private synchronized static void start() {
        if (isStarted)
            return;
        isStarted = true;
        new Thread(() -> {
            try {
                postingLoop();
            } catch (InterruptedException e) {
                throw new IllegalStateException(e);
            }
        }, "Positing Thread").start();
    }

    private static void postingLoop() throws InterruptedException {
        while (true) {
            Msq msq = queue.take();

            new File(OUTBOX_FOLDER).mkdirs();
            String fileName = OUTBOX_FOLDER + File.separator + System.currentTimeMillis() + ".msq";
            try {
                msq.writeXmlFile(fileName);
                Online.upload(new File(fileName), AuthTokenPanel.getAuthToken());
            } catch (JAXBException | IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static void enqueue(ControllerAccess controllerAccess, String configurationName) {
        start();
        if (queue.size() > 100) {
            // too much pending drama
            return;
        }
        Msq msq = TuneUploder.grabTune(controllerAccess, configurationName);
        try {
            queue.put(msq);
        } catch (InterruptedException e) {
            throw new IllegalStateException(e);
        }
    }
}
