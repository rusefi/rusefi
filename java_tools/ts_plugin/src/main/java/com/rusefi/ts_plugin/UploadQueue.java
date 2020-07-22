package com.rusefi.ts_plugin;

import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.shared.FileUtil;
import com.rusefi.tools.online.Online;
import com.rusefi.tools.online.UploadResult;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ui.AuthTokenPanel;

import javax.xml.bind.JAXBException;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.concurrent.LinkedBlockingDeque;

public class UploadQueue {
    public static final String OUTBOX_FOLDER = FileUtil.RUSEFI_SETTINGS_FOLDER + File.separator + "outbox";
    private static LinkedBlockingDeque<String> queue = new LinkedBlockingDeque<>(128);

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
        File folder = new File(OUTBOX_FOLDER);
        if (!folder.exists())
            return;
        String[] files = folder.list(new FilenameFilter() {
            @Override
            public boolean accept(File dir, String name) {
                return name.endsWith(".msq");
            }
        });
        if (files == null)
            return;

        for (String file : files) {
            if (queue.size() > 90)
                return;
            System.out.println(UploadQueue.class.getSimpleName() + " readOutbox " + file);
            try {
                String fileName = OUTBOX_FOLDER + File.separator + file;
                queue.put(fileName);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        System.out.println(UploadQueue.class.getSimpleName() + " readOutbox got " + queue.size());
    }

    private static void uploadLoop() throws InterruptedException {
        while (true) {
            String fileName = queue.take();

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
                System.out.println(UploadQueue.class.getSimpleName() + " Re-queueing " + fileName);
                queue.put(fileName);
                continue;
            }
            delete(fileName);
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
        msq.bibliography.setTuneComment("Auto-saved");
        try {
            new File(OUTBOX_FOLDER).mkdirs();
            String fileName = OUTBOX_FOLDER + File.separator + System.currentTimeMillis() + ".msq";
            msq.writeXmlFile(fileName);
            queue.put(fileName);
        } catch (InterruptedException | JAXBException | IOException e) {
            throw new IllegalStateException(e);
        }
    }
}
