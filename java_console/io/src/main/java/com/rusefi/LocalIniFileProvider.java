package com.rusefi;

public class LocalIniFileProvider {
    public static final String INI_FILE_FOR_SIMULATOR_ROOT_PATH = "firmware/tunerstudio/generated/rusefi_f407-discovery.ini";
    // f407-discovery is historically the most inclusive .ini file
    public static final String INI_FILE_FOR_SIMULATOR = "../" + INI_FILE_FOR_SIMULATOR_ROOT_PATH;
}
