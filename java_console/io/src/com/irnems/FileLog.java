package com.irnems;

import com.rusefi.io.LinkManager;
import org.jetbrains.annotations.Nullable;

import java.io.*;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * 6/30/13
 * (c) Andrey Belomutskiy
 */
public enum FileLog {
    MAIN,
    SIMULATOR_CONSOLE;

    private static final String DIR = "out/";

    @Nullable
    private OutputStream fileLog; // null if not opened yet or already closed

    private FileLog() {
    }

    public void start() {
        try {
            fileLog = openLog();
        } catch (FileNotFoundException e) {
            throw new IllegalStateException(e);
        }
    }

    private FileOutputStream openLog() throws FileNotFoundException {
        if (LinkManager.onlyUI)
            return null;
        String date = getDate();
        createFolderIfNeeded();
        String fileName = DIR + name() + "_rfi_report_" + date + ".csv";
        rlog("Writing to " + fileName);
        return new FileOutputStream(fileName, true);
    }

    private static void createFolderIfNeeded() {
        File dir = new File(DIR);
        if (dir.exists())
            return;
        boolean created = dir.mkdirs();
        if (!created)
            throw new IllegalStateException("Failed to create " + DIR + " folder");
    }

    public static String getDate() {
        return new SimpleDateFormat("yyyy-MM-dd HH_mm").format(new Date());
    }

    public synchronized void logLine(String fullLine) {
        if (fileLog == null)
            return;
        try {
            fileLog.write((fullLine + "\r\n").getBytes());
            fileLog.flush();
            System.out.println(fullLine);
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }

    }

    public synchronized void close() {
        if (fileLog == null)
            return; // already closed
        try {
            rlog("Closing file...");
            fileLog.close();
            fileLog = null;
        } catch (IOException e) {
            throw new IllegalStateException(e);
        }
    }

    public static void rlog(String msg) {
        System.out.println("r " + msg);
    }
}