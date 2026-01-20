package com.rusefi.ts;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.IniFileReader;
import com.rusefi.maintenance.migration.TuneMigrationContext;

import java.io.File;
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

        String projectIniFile = lastProjectPath + File.separator + TsHelper.MAIN_CONTROLLER_PATH;
        IniFileModel tsProjectIni = IniFileReader.readIniFile(projectIniFile);
        String tsProjectSignature = tsProjectIni.getSignature();
        if (!tsProjectSignature.equals(context.getPrevIniFile().getSignature())) {
            log.info(tsProjectSignature + " does not match " + context.getPrevIniFile().getSignature());
        }
    }
}
