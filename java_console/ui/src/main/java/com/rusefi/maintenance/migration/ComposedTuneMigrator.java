package com.rusefi.maintenance.migration;

import com.rusefi.maintenance.DefaultTuneMigrator;

import java.util.Arrays;
import java.util.List;

public enum ComposedTuneMigrator implements TuneMigrator {
    INSTANCE;

    private final List<TuneMigrator> tuneMigrators = Arrays.asList(
        VeTableExtensionMigrator.INSTANCE,
        BattLagCorrExtensionMigrator.INSTANCE,
        CltIdleCorrMigrator.INSTANCE,
        DisplacementIniFieldMigrator.INSTANCE,
        DefaultTuneMigrator.INSTANCE
    );

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        for (final TuneMigrator tuneMigrator: tuneMigrators) {
            tuneMigrator.migrateTune(context);
        }
    }
}
