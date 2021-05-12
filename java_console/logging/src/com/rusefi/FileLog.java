package com.rusefi;

import com.opensr5.Logger;
import com.rusefi.util.LazyFile;
import org.jetbrains.annotations.Nullable;

import javax.swing.*;
import java.io.*;

/**
 * what the hell is this anyway? todo: migrate to log4j2
 * 6/30/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public enum FileLog {
    MAIN,
    SIMULATOR_CONSOLE;

    public static final String LOG_INFO_TEXT = "Writing logs to '" + Logger.DIR + "'";
    public static final String OS_VERSION = "os.version";
    private static final String WIKI_URL = "https://github.com/rusefi/rusefi/wiki/rusEFI-logs-folder";
    public static String currentLogName;
    public static final Logger LOGGER = new Logger() {
        @Override
        public void trace(String msg) {
        }

        @Override
        public boolean isTradeEnabled() {
            return false;
        }

        @Override
        public void info(String msg) {
            MAIN.logLine(msg);
        }

        @Override
        public void error(String msg) {
            MAIN.logLine(msg);
        }
    };

    @Nullable
    private OutputStream fileLog; // null if not opened yet or already closed
    public static boolean suspendLogging;

    FileLog() {
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
        new Thread(FileLog::writeReadmeFile).start();
        // a bit strange spot for this invocation for sure
        printOsInfo();
    }

    private static void writeReadmeFile() {
        LazyFile file = new LazyFile(Logger.DIR + "README.html");
        file.write("<center>" + "<a href='" + WIKI_URL + "'>More info online<br/><img src=https://raw.githubusercontent.com/wiki/rusefi/rusefi/logo.gif></a>");
        try {
            file.close();
        } catch (IOException e) {
            // ignoring this one
        }
    }

    private static void printOsInfo() {
        MAIN.logLine("OS name: " + getOsName());
        MAIN.logLine("OS version: " + System.getProperty(OS_VERSION));
    }

    private static String getOsName() {
        return System.getProperty("os.name");
    }

    public static boolean isWindows() {
        return getOsName().contains("Windows");
    }

    private FileOutputStream openLog() throws FileNotFoundException {
        String date = Logger.getDate();
        createFolderIfNeeded();
        currentLogName = name() + "_rfi_report_" + date + ".csv";
        String fileName = Logger.DIR + currentLogName;
        rlog("Writing to " + fileName);
        return new FileOutputStream(fileName, true);
    }

    public static void createFolderIfNeeded() {
        File dir = new File(Logger.DIR);
        if (dir.exists())
            return;
        boolean created = dir.mkdirs();
        if (!created)
            throw new IllegalStateException("Failed to create " + Logger.DIR + " folder");
    }

    public synchronized void logLine(String fullLine) {
        String withDate = Logger.getDate() + Logger.END_OF_TIMESTAND_TAG + fullLine;
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

    public void logException(String msg, Throwable e) {
        logLine(msg + e);
        e.printStackTrace();
        if (fileLog == null) {
            JOptionPane.showConfirmDialog(null, msg + " " + e, "no log and Exception Occurred", JOptionPane.DEFAULT_OPTION);
            return;
        }
        log(e);
    }
}