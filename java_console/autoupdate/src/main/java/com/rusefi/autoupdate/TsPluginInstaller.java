package com.rusefi.autoupdate;

import com.devexperts.logging.Logging;
import com.rusefi.core.FileUtil;
import com.rusefi.ts.TsHelper;

import java.io.File;
import java.io.IOException;

import static com.devexperts.logging.Logging.getLogging;

public class TsPluginInstaller {
    private static final Logging log = getLogging(TsPluginInstaller.class);

    private static final String PLUGIN_FOLDER = TsHelper.TS_ROOT +
        "TunerStudio" +
        File.separator + "plugins" +
        File.separator;

    private static final String RUSEFI_TS_PLUGIN_LAUNCHER_JAR = "rusefi_ts_plugin_launcher.jar";

    public static void installTsPlugin() {
        File tsLauncher = new File(RUSEFI_TS_PLUGIN_LAUNCHER_JAR);
        if (!tsLauncher.exists()) {
            log.warn(RUSEFI_TS_PLUGIN_LAUNCHER_JAR + " not found");
            return;
        }
        try {
            new File(PLUGIN_FOLDER).mkdirs();
            FileUtil.copyFile(RUSEFI_TS_PLUGIN_LAUNCHER_JAR, PLUGIN_FOLDER + RUSEFI_TS_PLUGIN_LAUNCHER_JAR);
            log.info("Just installed fresh " + RUSEFI_TS_PLUGIN_LAUNCHER_JAR + " into " + PLUGIN_FOLDER);
            String legacyPluginFileName = PLUGIN_FOLDER + "rusefi_plugin_launcher.jar";
            File legacyPluginFile = new File(legacyPluginFileName);
            if (legacyPluginFile.exists()) {
                log.info("Removing legacy plugin file " + legacyPluginFileName);
                legacyPluginFile.delete();
            }
        } catch (IOException e) {
            log.warn("Error installing " + RUSEFI_TS_PLUGIN_LAUNCHER_JAR + e, e);
        }
    }
}
