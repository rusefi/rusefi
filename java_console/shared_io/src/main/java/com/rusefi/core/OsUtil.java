package com.rusefi.core;

/**
 * Host OS checks, in the lowest module that needs them.
 * <p>
 * {@code com.rusefi.FileLog} in :ecu_io has had these for years, but :autoupdate and :core_ui sit
 * below :ecu_io in the dependency graph and cannot reach it, so this is where the single
 * implementation lives now and FileLog delegates here.
 */
public class OsUtil {
    public static String getOsName() {
        return System.getProperty("os.name", "");
    }

    /**
     * Note the deliberate "Windows" rather than a shorter "win" - the latter also matches
     * "Darwin", which would tag macOS as Windows.
     */
    public static boolean isWindows() {
        return getOsName().contains("Windows");
    }

    public static boolean isLinux() {
        return getOsName().equalsIgnoreCase("Linux");
    }
}
