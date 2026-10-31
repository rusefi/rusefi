package com.rusefi.config;

/** Constants used by Java console code but not present in every generated firmware set. */
public final class ConsoleConstants {
    public static final String CMD_LUA_RESET = "luareset";
    public static final char TS_CRC32_CHECK_COMMAND = 'K';
    public static final char TS_READ32_COMMAND = '(';

    public static final String OUTPUT_CHANNEL_SD_LOGGING_INTERNAL = "sd_logging_internal";
    public static final String OUTPUT_CHANNEL_SD_MSD = "sd_msd";
    public static final String OUTPUT_CHANNEL_SD_PRESENT = "sd_present";

    public static final int trigger_type_e_TT_VVT_MITSUBISHI_6G75 = 97;
    public static final int vvt_mode_e_VVT_BMW_N63TU = 17;

    private ConsoleConstants() {
    }
}
