package com.rusefi.maintenance.migration.migrators;

import static com.rusefi.maintenance.migration.IniFieldMigrationUtils.generateConstant;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Optional;


public enum MultiplierToTableMigrator implements TuneMigrator {
    INSTANCE;

    private static final class Mapping {
        private final String multiplierFieldName; // scalar on prev tune
        private final String tableFieldName;      // array exists on both prev & updated
        private final String updatedFieldName;

        private Mapping(String multiplierFieldName, String tableFieldName, String updatedFieldName) {
            this.multiplierFieldName = multiplierFieldName;
            this.tableFieldName = tableFieldName;
            this.updatedFieldName = updatedFieldName;
        }
    }

    // List all multiplier->table migrations here
    private static final Mapping[] mappings = new Mapping[] {
        new Mapping("crankingIACposition", "cltCrankingCorr", "cltCrankingCorr"),
        new Mapping("cranking_baseFuel", "crankingCycleFuelCoef", "crankingCycleBaseFuel")
    };

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        for (Mapping m : mappings) {
            migrate(context, m);
        }
    }

    private static void migrate(final TuneMigrationContext context, final Mapping mapping) {
        final Optional<IniField> prevMultiplier = context.getPrevIniFile().findIniField(mapping.multiplierFieldName);
        final Optional<IniField> updatedMultiplier = context.getUpdatedIniFile().findIniField(mapping.multiplierFieldName);

        final Optional<IniField> prevTable = context.getPrevIniFile().findIniField(mapping.tableFieldName);
        final Optional<IniField> updatedTable = context.getUpdatedIniFile().findIniField(mapping.updatedFieldName);

        // Migration is only valid when:
        // - previous tune had the multiplier field, but updated tune does NOT (field removed)
        // - table exists both in prev and updated tunes
        if (!prevMultiplier.isPresent() || updatedMultiplier.isPresent()
                || !prevTable.isPresent() || !updatedTable.isPresent()) {
            return;
        }

        final Constant prevTableConst = context.getPrevTune().getConstantsAsMap().get(mapping.tableFieldName);
        final Constant updatedTableConst = context.getUpdatedTune().getConstantsAsMap().get(mapping.updatedFieldName);
        final Constant prevMultiplierConst = context.getPrevTune().getConstantsAsMap().get(mapping.multiplierFieldName);

        if (prevTableConst == null || updatedTableConst == null || prevMultiplierConst == null) {
            return;
        }

        final ArrayIniField updatedTableField = (ArrayIniField) updatedTable.get();

        final String[][] migratedValues = updatedTableField.getValues(prevTableConst.getValue());
        final double multiplier = Double.parseDouble(prevMultiplierConst.getValue());

        for (int row = 0; row < migratedValues.length; row++) {
            for (int column = 0; column < migratedValues[0].length; column++) {
                final double value = Double.parseDouble(migratedValues[row][column]) * multiplier;
                migratedValues[row][column] = formatArrayValue(value, updatedTableField.getDigits());
            }
        }

        context.addMigration(
                mapping.updatedFieldName,
                generateConstant(updatedTableField, updatedTableField.formatValue(migratedValues))
        );
    }

    private static String formatArrayValue(final double value, final String digits) {
        return IdleCurveMigrator.formatArrayValue(value, digits);
    }
}
