package com.rusefi.ts;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModelImpl;
import com.rusefi.maintenance.migration.TuneMigrationContext;

import java.io.File;
import java.io.IOException;
import java.util.Properties;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Post-processing step of tune migration that looks at the user's local TunerStudio project
 * (as opposed to the tune contents, which is what the {@link com.rusefi.maintenance.migration.migrators.TuneMigrator}s handle).
 * <p>
 * Motivation (see #8357 "Adjust TS project files to reduce confusion"): after the console migrates a tune to a new
 * firmware/.ini, the TunerStudio project the user last had open still references the old .ini signature.
 * That leaves the user with a project whose 'mainController.ini' no longer matches the ECU, which is confusing:
 * TS complains about a signature mismatch and the user has to figure out that they need to re-import the .ini.
 * Long term this class is meant to reconcile that project state (point it at the new .ini) so that TS simply
 * opens correctly against the freshly-updated ECU.
 * <p>
 * How it locates the project: TS keeps '~/.efiAnalytics/tsUser.properties' (see {@link TsHelper#readTsProperties()})
 * with 'lastProjectPath' and 'lastConnectedFirmwareSignature'; the project's active .ini is
 * '&lt;lastProjectPath&gt;/projectCfg/mainController.ini' ({@link TsHelper#MAIN_CONTROLLER_PATH}).
 * <p>
 * Current state: this is still a skeleton. It only reads the project .ini and logs whether its signature
 * matches the pre-migration .ini from the {@link TuneMigrationContext}; nothing is written back yet.
 * Any failure (no TS install, no last project, unreadable files) is logged and swallowed since this step must
 * never break the migration itself.
 * <p>
 * Invoked as the last step of {@link com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator#migrateTune};
 * see also TsProjectUpdaterSandbox in the ui test sources for a manual harness.
 */
public enum TsProjectUpdater {
    INSTANCE;

    private static final Logging log = getLogging(TsProjectUpdater.class);

    public void afterTuneMigration(TuneMigrationContext context) {
        try {
            adjustTsProjectState(context);
        } catch (Throwable e) {
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
        IniFileModelImpl tsProjectIni = IniFileModelImpl.readIniFile(projectIniFile);
        String tsProjectSignature = tsProjectIni.getSignature();
        if (!tsProjectSignature.equals(context.getPrevIniFile().getSignature())) {
            log.info(tsProjectSignature + " does not match " + context.getPrevIniFile().getSignature());
            return;
        }
        // todo: actually do something useful?!
    }
}
