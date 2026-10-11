package com.rusefi;

/**
 * what the hell is this anyway? rename this utility class?
 * 6/30/13
 * Andrey Belomutskiy, (c) 2013-2020
 */
public class FileLog {

    public static final String OS_VERSION = "os.version";

    FileLog() {
    }

    public static boolean is32bitJava() {
        return System.getProperty("os.arch").contains("86");
    }

}
