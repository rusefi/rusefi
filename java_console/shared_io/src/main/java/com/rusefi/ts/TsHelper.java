package com.rusefi.ts;

import org.jetbrains.annotations.NotNull;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Properties;

public class TsHelper {
    public static final String TS_ROOT = System.getProperty("user.home") + File.separator + ".efiAnalytics" + File.separator;
    public static final String CURRENT_TUNE_MSQ = "CurrentTune.msq";
    public static final String MAIN_CONTROLLER_PATH = "projectCfg" + File.separator + "mainController.ini";
    static final String TS_USER_FILE = TS_ROOT + "tsUser.properties";

    public static @NotNull Properties readTsProperties() throws IOException {
        Properties properties = new Properties();
        properties.load(Files.newInputStream(Paths.get(TS_USER_FILE)));
        return properties;
    }
}
