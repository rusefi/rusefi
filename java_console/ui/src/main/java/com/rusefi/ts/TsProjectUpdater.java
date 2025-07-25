package com.rusefi.ts;

import com.devexperts.logging.Logging;
import com.rusefi.maintenance.migration.TuneMigrationContext;

import java.io.IOException;
import java.util.Properties;

import static com.devexperts.logging.Logging.getLogging;

public enum TsProjectUpdater {
    INSTANCE;

    private static final Logging log = getLogging(TsProjectUpdater.class);

    public void afterTuneMigration(TuneMigrationContext context) {
        try {
            adjustTsProjectState(context);
        } catch (IOException e) {
            log.warn("Error updating TS project");
        }
    }

    private void adjustTsProjectState(TuneMigrationContext context) throws IOException {
        Properties properties = TsHelper.readTsProperties();
        String lastProjectPath = properties.getProperty("lastProjectPath");
        String lastConnectedFirmwareSignature = properties.getProperty("lastConnectedFirmwareSignature");
        if (lastProjectPath == null || lastConnectedFirmwareSignature == null) {
            log.warn("no lastProjectPath or lastConnectedFirmwareSignature");
            return;
        }
        log.info("lastProjectPath=" + lastProjectPath + ", lastConnectedFirmwareSignature=" + lastConnectedFirmwareSignature);
    }
}
