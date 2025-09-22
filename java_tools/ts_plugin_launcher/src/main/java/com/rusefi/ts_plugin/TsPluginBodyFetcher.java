package com.rusefi.ts_plugin;

import com.devexperts.logging.Logging;
import com.rusefi.core.FileUtil;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.core.ui.AutoupdateUtil;

import java.util.Date;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Download fresh copy of {@link TsPluginBodyFetcher#PLUGIN_BODY_JAR}
 */
public class TsPluginBodyFetcher {
    private static final String PLUGIN_BODY_JAR = "rusefi_plugin_body.jar";
    static final String LOCAL_JAR_FILE_NAME = FileUtil.RUSEFI_SETTINGS_FOLDER + PLUGIN_BODY_JAR;
    private static final Logging log = getLogging(TsPluginBodyFetcher.class);

    public static void downloadLatestIfNeeded() {
        ConnectionAndMeta connectionAndMeta;
        try {
            connectionAndMeta = new ConnectionAndMeta(PLUGIN_BODY_JAR).invoke(ConnectionAndMeta.getDefaultAutoUpdateUrl());
        } catch (Exception e) {
            log.warn("Error grabbing latest plugin: " + e, e);
            return;
        }
        log.info("Server has " + connectionAndMeta.getCompleteFileSize() + " from " + new Date(connectionAndMeta.getLastModified()));

        if (AutoupdateUtil.hasExistingFile(LOCAL_JAR_FILE_NAME, connectionAndMeta.getCompleteFileSize(), connectionAndMeta.getLastModified())) {
            log.info("We already have latest update " + new Date(connectionAndMeta.getLastModified()));
        } else {
            startBodyDownload();
        }
    }

    private static void startBodyDownload() {
        log.info("startDownload");

        try {
            ConnectionAndMeta connectionAndMeta = new ConnectionAndMeta(PLUGIN_BODY_JAR).invoke(ConnectionAndMeta.getDefaultAutoUpdateUrl());

            ConnectionAndMeta.downloadFile(LOCAL_JAR_FILE_NAME, connectionAndMeta, currentPercentage -> {
            });
            log.info("Downloaded!");

        } catch (Exception e) {
            log.warn("Error downloading " + e, e);
        }
    }
}
