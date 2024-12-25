package com.rusefi;

import java.util.Optional;

public class LocalIniFileProvider {
    public static final String INI_FILE_FOR_SIMULATOR_ROOT_PATH = Optional.ofNullable(System.getenv("SIMULATOR_INI_FILE")).orElse("firmware/tunerstudio/generated/rusefi_stm32f767_nucleo.ini");
    // f407-discovery is historically the most inclusive .ini file
    public static final String INI_FILE_FOR_SIMULATOR = "../" + INI_FILE_FOR_SIMULATOR_ROOT_PATH;
}
