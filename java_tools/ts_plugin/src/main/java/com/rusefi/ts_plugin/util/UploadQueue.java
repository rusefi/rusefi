package com.rusefi.ts_plugin.util;

import com.devexperts.logging.Logging;
import com.efiAnalytics.plugin.ecu.ControllerAccess;
import com.rusefi.core.FileUtil;
import com.rusefi.tools.online.Online;
import com.rusefi.tools.online.UploadResult;
import com.rusefi.ts_plugin.ui.TuneUploder;
import com.rusefi.tune.xml.Msq;
import com.rusefi.ts_plugin.ui.AuthTokenPanel;

import jakarta.xml.bind.JAXBException;
import java.io.File;
import java.io.IOException;
import java.util.concurrent.LinkedBlockingDeque;

public class UploadQueue {
    private final static Logging log = Logging.getLogging(UploadQueue.class);

    public static final String OUTBOX_FOLDER = FileUtil.RUSEFI_SETTINGS_FOLDER + "outbox";
    private static final LinkedBlockingDeque<FileAndFolder> queue = new LinkedBlockingDeque<>(128);

    private static boolean isStarted;

    public synchronized static void start() {
        if (isStarted)
            return;
        isStarted = true;
        readOutbox();
        Thread t = new Thread(() -> {
            try {
                uploadLoop();
            } catch (InterruptedException e) {
                throw new IllegalStateException(e);
            }
        }, "Posting Thread");
        t.setDaemon(true);
        t.start();
    }

    private static void readOutbox() {
        File folder = new File(OUTBOX_FOLDER);
        if (!folder.exists())
            return;
        String[] files = folder.list((dir, name) -> name.endsWith(".msq"));
        if (files == null)
            return;

        for (String file : files) {
            if (queue.size() > 90)
                return;
            System.out.println(UploadQueue.class.getSimpleName() + " readOutbox " + file);
            try {
                queue.put(new FileAndFolder(OUTBOX_FOLDER, file));
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        System.out.println(UploadQueue.class.getSimpleName() + " readOutbox got " + queue.size());
    }

    @SuppressWarnings("InfiniteLoopStatement")
    private static void uploadLoop() throws InterruptedException {
        while (true) {
            FileAndFolder file = queue.take();

            UploadResult result = Online.upload(new File(file.getFullName()), AuthTokenPanel.getAuthToken());
            System.out.println("isError " + result.isError());
            System.out.println("first " + result.getFirstMessage());
            if (result.isError() && result.getFirstMessage().contains("This file already exists")) {
                System.out.println(UploadQueue.class.getSimpleName() + " No need to re-try this one");
                file.postUpload();
                // do not retry this error
                continue;
            }
            if (result.isError()) {
                System.out.println(UploadQueue.class.getSimpleName() + " Re-queueing " + file.getFullName());
                queue.put(file);
                continue;
            }
            file.postUpload();
        }
    }

    @SuppressWarnings("ResultOfMethodCallIgnored")
    private static void delete(String fileName) {
        System.out.println(UploadQueue.class.getSimpleName() + " Deleting " + fileName);
        new File(fileName).delete();
    }

    @SuppressWarnings("ResultOfMethodCallIgnored")
    public static void enqueue(ControllerAccess controllerAccess, String configurationName) {
        start();
        if (queue.size() > 100) {
            // too much pending drama
            return;
        }
        Msq msq = TuneUploder.grabTune(controllerAccess, configurationName);
        if (msq == null) {
            log.error("Error saving tune");
            return;
        }
        msq.bibliography.setTuneComment("Auto-saved");
        try {
            new File(OUTBOX_FOLDER).mkdirs();
            String fileName = System.currentTimeMillis() + ".msq";
            String fullFileName = OUTBOX_FOLDER + File.separator + fileName;
            msq.writeXmlFile(fullFileName);
            queue.put(new FileAndFolder(OUTBOX_FOLDER, fileName));
        } catch (InterruptedException | JAXBException | IOException e) {
            throw new IllegalStateException(e);
        }
    }

    static class FileAndFolder {
        private static final boolean DEBUG_SAVE_UPLOADED = false;
        private final String folder;
        private final String file;

        public FileAndFolder(String folder, String file) {
            this.folder = folder;
            this.file = file;
        }

        public String getFullName() {
            return folder + File.separator + file;
        }

        @SuppressWarnings("ResultOfMethodCallIgnored")
        public void postUpload() {
            if (DEBUG_SAVE_UPLOADED) {
                log.info("Renaming file " + file);
                String uploadedDir = folder + File.separator + "uploaded";
                new File(uploadedDir).mkdirs();
                new File(getFullName()).renameTo(new File(uploadedDir + File.separator + file));
            } else {
                delete(getFullName());
            }
        }
    }
}

