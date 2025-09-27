package com.rusefi.maintenance.migration;

import com.rusefi.config.FieldType;
import com.rusefi.maintenance.DefaultTuneMigrator;
import com.rusefi.ts.TsProjectUpdater;

import java.util.Arrays;
import java.util.List;

import static com.rusefi.maintenance.migration.TableAddColumnsMigrator.VE_RPM_BINS_FIELD_NAME;
import static com.rusefi.maintenance.migration.TableAddColumnsMigrator.VE_TABLE_FIELD_NAME;

public enum ComposedTuneMigrator implements TuneMigrator {
    INSTANCE;

    private final List<TuneMigrator> tuneMigrators = Arrays.asList(
        new TableAddColumnsMigrator(VE_TABLE_FIELD_NAME, FieldType.UINT16, VE_RPM_BINS_FIELD_NAME),
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
