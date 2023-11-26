package com.rusefi;

import com.devexperts.logging.Logging;
import com.opensr5.Logger;
import com.rusefi.util.LazyFile;
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

    public static final String OS_VERSION = "os.version";
    public static final String DATE_PATTERN = "yyyy-MM-dd_HH_mm_ss_SSS";
    private static final String WIKI_URL = "https://github.com/rusefi/rusefi/wiki/rusEFI-logs-folder";
    public static String currentLogName;

    @Nullable
    private OutputStream fileLog; // null if not opened yet or already closed

    FileLog() {
    }

    public static String getDate() {
        return new SimpleDateFormat(DATE_PATTERN).format(new Date());
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

    public static boolean isLinux() {
        return getOsName().equalsIgnoreCase("Linux");
    }

    public static String getOsName() {
        return System.getProperty("os.name");
    }

    public static boolean isWindows() {
        return getOsName().contains("Windows");
    }

    private FileOutputStream openLog() throws FileNotFoundException {
        String date = getDate();
        createFolderIfNeeded();
        String shortFileName = name() + "_rfi_report_" + date;
        Logging.configureLogFile(Logger.DIR + shortFileName + ".log");
        currentLogName = shortFileName + ".csv";
        String fullFileName = Logger.DIR + currentLogName;
        rlog("Writing to " + fullFileName);
        return new FileOutputStream(fullFileName, true);
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
        String withDate = getDate() + Logger.END_OF_TIMESTAND_TAG + fullLine;
        System.out.println(withDate);

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