package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.shared.FileUtil;
import com.rusefi.tools.online.Online;
import com.rusefi.tools.online.UploadResult;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.AuthTokenPanel;

import javax.xml.bind.JAXBException;
import java.io.File;
import java.io.IOException;
import java.util.Objects;
import java.util.concurrent.LinkedBlockingDeque;

public class UploadQueue {
    public static final String OUTBOX_FOLDER = FileUtil.RUSEFI_SETTINGS_FOLDER + File.separator + "outbox";
    private static LinkedBlockingDeque<Msq> queue = new LinkedBlockingDeque<>(128);

    private static boolean isStarted;

    public synchronized static void start() {
        if (isStarted)
            return;
        isStarted = true;
        readOutbox();
        new Thread(() -> {
            try {
                uploadLoop();
            } catch (InterruptedException e) {
                throw new IllegalStateException(e);
            }
        }, "Positing Thread").start();
    }

    private static void readOutbox() {
        for (String file : Objects.requireNonNull(new File(OUTBOX_FOLDER).list((dir, name) -> name.endsWith(".msq")))) {
            if (queue.size() > 90)
                return;
            System.out.println(UploadQueue.class.getSimpleName() + " readOutbox " + file);
            try {
                Msq msg = Msq.readTune(OUTBOX_FOLDER + File.separator + file);
                queue.put(msg);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        System.out.println(UploadQueue.class.getSimpleName() + " readOutbox got " + queue.size());
    }

    private static void uploadLoop() throws InterruptedException {
        while (true) {
            Msq msq = queue.take();

            new File(OUTBOX_FOLDER).mkdirs();
            String fileName = OUTBOX_FOLDER + File.separator + System.currentTimeMillis() + ".msq";
            try {
                msq.writeXmlFile(fileName);
                UploadResult result = Online.upload(new File(fileName), AuthTokenPanel.getAuthToken());
                System.out.println("isError " + result.isError());
                System.out.println("first " + result.getFirstMessage());
                if (result.isError() && result.getFirstMessage().contains("This file already exists")) {
                    System.out.println(UploadQueue.class.getSimpleName() + " No need to re-try this one");
                    delete(fileName);
                    // do not retry this error
                    continue;
                }
                if (result.isError()) {
                    System.out.println(UploadQueue.class.getSimpleName() + " Re-queueing " + msq);
                    queue.put(msq);
                    continue;
                }
                delete(fileName);
            } catch (JAXBException | IOException e) {
                e.printStackTrace();
            }
        }
    }

    private static void delete(String fileName) {
        System.out.println(UploadQueue.class.getSimpleName() + " Deleting " + fileName);
        new File(fileName).delete();
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
