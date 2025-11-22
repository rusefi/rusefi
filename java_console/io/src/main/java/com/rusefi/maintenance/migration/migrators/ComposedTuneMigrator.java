package com.rusefi.maintenance.migration.migrators;

import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.ts.TsProjectUpdater;

import java.util.Arrays;
import java.util.List;

public enum ComposedTuneMigrator implements TuneMigrator {
    INSTANCE;

    private final List<TuneMigrator> tuneMigrators = Arrays.asList(
        // first the migrators that update the table shape
        TableAddColumnsMigrator.VE_TABLE_MIGRATOR,
        TableAddColumnsMigrator.LAMBDA_TABLE_MIGRATOR,
        TableAddColumnsMigrator.IGNITION_TABLE_MIGRATOR,
        TableAddColumnsMigrator.INJECTION_PHASE_MIGRATOR,
        TableAddColumnsMigrator.MAP_SAMPLING_MIGRATOR,
        // rest of migrators
        AfrLambdaTableMigrator.INSTANCE,
        BattLagCorrExtensionMigrator.INSTANCE,
        CltIdleCorrMigrator.INSTANCE,
        DisplacementIniFieldMigrator.INSTANCE,
        VeBlends1BlendParameterMigrator.INSTANCE,
        BoostLoadBinsMigrator.INSTANCE,
        BooleanIniFieldMigrator.INSTANCE,
        DigitsIniFieldMigrator.INSTANCE,
        STFTFieldMigrator.INSTANCE,
        IdleCurveMigrator.INSTANCE,
        MultiplierToTableMigrator.INSTANCE,
        ScalarToArrayMigrator.INSTANCE,
        MapSamplingValuesMigrator.INSTANCE,

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
