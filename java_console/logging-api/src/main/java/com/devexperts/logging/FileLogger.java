package com.devexperts.logging;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;

public class FileLogger {
    public static final String DIR = "logs/";

    public static final String date = getDate();
    public static final String DATE_PATTERN = "yyyy-MM-dd_HH_mm_ss_SSS";

    static {
        Logging.configureLogFile(FileLogger.DIR + "efi_log_" + date + ".log");
    }

    public static String getDate() {
        return new SimpleDateFormat(FileLogger.DATE_PATTERN).format(new Date());
    }

    public static void createFolderIfNeeded() {
        File dir = new File(DIR);
        if (dir.exists())
            return;
        boolean created = dir.mkdirs();
        if (!created)
            throw new IllegalStateException("Failed to create " + DIR + " folder");
    }
}
