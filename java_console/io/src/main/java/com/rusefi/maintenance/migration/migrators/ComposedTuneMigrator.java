package com.rusefi.maintenance.migration.migrators;

import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.ts.TsProjectUpdater;

import java.util.Arrays;
import java.util.List;

public enum ComposedTuneMigrator implements TuneMigrator {
    INSTANCE;

    private final List<TuneMigrator> tuneMigrators = Arrays.asList(
        TableAddColumnsMigrator.VE_TABLE_MIGRATOR,
        TableAddColumnsMigrator.LAMBDA_TABLE_MIGRATOR,
        TableAddColumnsMigrator.IGNITION_TABLE_MIGRATOR,
        TableAddColumnsMigrator.INJECTION_PHASE_MIGRATOR,
        BattLagCorrExtensionMigrator.INSTANCE,
        CltIdleCorrMigrator.INSTANCE,
        DisplacementIniFieldMigrator.INSTANCE,
        VeBlends1BlendParameterMigrator.INSTANCE,
        BoostLoadBinsMigrator.INSTANCE,
        BooleanIniFieldMigrator.INSTANCE,
        DigitsIniFieldMigrator.INSTANCE,
        STFTFieldMigrator.INSTANCE,
        IdleCurveMigrator.INSTANCE,
        // Disabled!, see #8718
        // AfrMsqImportMigrator.INSTANCE,

        // leave the default as the final migrator!
        DefaultTuneMigrator.INSTANCE
    );

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        for (final TuneMigrator tuneMigrator: tuneMigrators) {
            tuneMigrator.migrateTune(context);
        }
        TsProjectUpdater.INSTANCE.afterTuneMigration(context);
    }
}
