package com.rusefi;

import com.devexperts.logging.FileLogger;
import com.devexperts.logging.Logging;
import com.opensr5.Logger;
import com.rusefi.util.LazyFile;
import com.rusefi.util.LazyFileImpl;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.io.*;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * what the hell is this anyway? todo: migrate to log4j2
 * 6/30/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
@Deprecated
public enum FileLog {
    MAIN,
    SIMULATOR_CONSOLE;

    public static final String LOG_INFO_TEXT = "Writing logs to '" + FileLogger.DIR + "'";
    public static final String OS_VERSION = "os.version";
    public static String currentLogName;

    @Nullable
    private OutputStream fileLog; // null if not opened yet or already closed
    public static boolean suspendLogging;

    FileLog() {
    }

    public static String getDate() {
        return FileLogger.getDate();
    }

    public void start() {
        if (fileLog != null)
            return;
        try {
            fileLog = openLog();
        } catch (FileNotFoundException e) {
            // Access is denied would be an example of a legit exception to happen here
            return;
        }
    }

    public static boolean isLinux() {
        return getOsName().equalsIgnoreCase("Linux");
    }

    public static String getOsName() {
        return System.getProperty("os.name");
    }

    public static boolean is32bitJava() {
        return System.getProperty("os.arch").contains("86");
    }

    public static boolean isWindows() {
        return getOsName().contains("Windows");
    }

    private FileOutputStream openLog() throws FileNotFoundException {
        FileLogger.createFolderIfNeeded();
        String shortFileName = name() + "_rfi_report_" + FileLogger.date;
        currentLogName = shortFileName + ".csv";
        String fullFileName = FileLogger.DIR + currentLogName;
        rlog("Writing to " + fullFileName);
        return new FileOutputStream(fullFileName, true);
    }

    public synchronized void logLine(String fullLine) {
        String withDate = getDate() + Logger.END_OF_TIMESTAND_TAG + fullLine;
        System.out.println(withDate);
        if (suspendLogging)
            return;
        if (fileLog == null)
            return;
        try {
            fileLog.write((withDate + "\r\n").getBytes());
            fileLog.flush();
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

    private static void rlog(String msg) {
        System.out.println("rlog " + msg);
    }

    public void log(Throwable exception) {
        if (fileLog == null)
            throw new NullPointerException("fileLog while " + exception);
        OutputStreamWriter os = new OutputStreamWriter(fileLog);
        exception.printStackTrace(new PrintWriter(os));
    }
}
