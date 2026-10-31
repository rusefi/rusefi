package com.rusefi;

import com.devexperts.logging.FileLogger;
import com.opensr5.Logger;
import org.jetbrains.annotations.Nullable;

import java.io.*;

public enum AutotestLogging {
    INSTANCE;

    public static String currentLogName;

    @Nullable
    private OutputStream fileLog; // null if not opened yet or already closed
    public static boolean suspendLogging;


    public synchronized void logLine(String fullLine) {
        String withDate = FileLogger.getDate() + Logger.END_OF_TIMESTAND_TAG + fullLine;
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


    private FileOutputStream openLog() throws FileNotFoundException {
        FileLogger.createFolderIfNeeded();
        String shortFileName = name() + "_rfi_report_" + FileLogger.date;
        currentLogName = shortFileName + ".csv";
        String fullFileName = FileLogger.DIR + currentLogName;
        rlog("Writing to " + fullFileName);
        return new FileOutputStream(fullFileName, true);
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
