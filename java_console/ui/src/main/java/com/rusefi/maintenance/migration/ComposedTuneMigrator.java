package com.rusefi.maintenance.migration;

import com.rusefi.maintenance.DefaultTuneMigrator;
import com.rusefi.ts.TsProjectUpdater;

import java.util.Arrays;
import java.util.List;

public enum ComposedTuneMigrator implements TuneMigrator {
    INSTANCE;

    private final List<TuneMigrator> tuneMigrators = Arrays.asList(
        VeTableExtensionMigrator.INSTANCE,
        BattLagCorrExtensionMigrator.INSTANCE,
        CltIdleCorrMigrator.INSTANCE,
        DisplacementIniFieldMigrator.INSTANCE,
        VeBlends1BlendParameterMigrator.INSTANCE,
        BoostLoadBinsMigrator.INSTANCE,
        BooleanIniFieldMigrator.INSTANCE,
        DigitsIniFieldMigrator.INSTANCE,

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
