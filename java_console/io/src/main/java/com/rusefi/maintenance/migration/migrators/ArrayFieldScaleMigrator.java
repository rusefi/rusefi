package com.rusefi.maintenance.migration.migrators;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Locale;
import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;

/**
 * Generic migrator for array fields that change scale between firmware versions.
 * Follows the same pattern as BooleanIniFieldMigrator with a static array of fields to migrate.
 *
 * <h3>Testing and Coverage Examples:</h3>
 * This migrator is tested in {@code TargetAfrBlendsScaleMigratorTest} in the {@code java_console/ui} module.
 * Examples of fields covered by this migrator include:
 * <ul>
 *   <li>{@code targetAfrBlends1_table} (Scale change from 0.1 to 0.01)</li>
 *   <li>{@code targetAfrBlends2_table} (Scale change from 0.1 to 0.01)</li>
 * </ul>
 */
public enum ArrayFieldScaleMigrator implements TuneMigrator {
    INSTANCE;

    private static final Logging log = getLogging(ArrayFieldScaleMigrator.class);

    private static final ArrayFieldScaleMigration[] fieldsToMigrate = {
        // targetAfrBlends scale change: 0.1 -> 0.01 (values multiplied by 10)
        new ArrayFieldScaleMigration("targetAfrBlends1_table", 0.1, 0.01),
        new ArrayFieldScaleMigration("targetAfrBlends2_table", 0.1, 0.01),
    };

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        for (ArrayFieldScaleMigration field : fieldsToMigrate) {
            migrateField(context, field);
        }
    }

    private void migrateField(final TuneMigrationContext context, final ArrayFieldScaleMigration fieldConfig) {
        final String fieldName = fieldConfig.getFieldName();

        final Optional<IniField> prevIniField = context.getPrevIniFile().findIniField(fieldName);
        final Optional<IniField> updatedIniField = context.getUpdatedIniFile().findIniField(fieldName);

        if (!prevIniField.isPresent() || !updatedIniField.isPresent()) {
            return;
        }

        final IniField prevField = prevIniField.get();
        final IniField updatedField = updatedIniField.get();

        if (!(prevField instanceof ArrayIniField) || !(updatedField instanceof ArrayIniField)) {
            return;
        }

        final ArrayIniField prevArrayField = (ArrayIniField) prevField;
        final ArrayIniField updatedArrayField = (ArrayIniField) updatedField;

        final Constant prevConstant = context.getPrevTune().getConstantsAsMap().get(fieldName);
        final Constant updatedConstant = context.getUpdatedTune().getConstantsAsMap().get(fieldName);

        if (prevConstant == null || updatedConstant == null) {
            return;
        }

        // Check if this is the expected scale change migration
        final double prevScale = prevArrayField.getMultiplier();
        final double updatedScale = updatedArrayField.getMultiplier();

        if (!fieldConfig.shouldMigrate(prevScale, updatedScale)) {
            return;
        }

        // Migrate values
        final String[][] prevValues = prevArrayField.getValues(prevConstant.getValue());
        final String[][] migratedValues = new String[prevValues.length][prevValues[0].length];

        for (int i = 0; i < prevValues.length; i++) {
            for (int j = 0; j < prevValues[i].length; j++) {
                try {
                    final double prevValue = Double.parseDouble(prevValues[i][j]);
                    final double migratedValue = fieldConfig.migrateValue(prevValue);
                    migratedValues[i][j] = String.format(Locale.US, "%." + updatedArrayField.getDigits() + "f", migratedValue);
                } catch (NumberFormatException e) {
                    log.warn(String.format(
                        "Invalid value in `%s`: `%s`, skipping migration for this cell",
                        fieldName, prevValues[i][j]
                    ));
                    migratedValues[i][j] = prevValues[i][j];
                }
            }
        }

        final Constant migratedConstant = new Constant(
            fieldName,
            updatedArrayField.getUnits(),
            updatedArrayField.formatValue(migratedValues),
            updatedArrayField.getDigits(),
            Integer.toString(updatedArrayField.getRows()),
            Integer.toString(updatedArrayField.getCols())
        );

        context.addMigration(fieldName, migratedConstant);
        log.info(String.format("Migrated `%s` from scale %.4f to %.4f (multiplied values by %.4f)",
            fieldName, fieldConfig.getOldScale(), fieldConfig.getNewScale(), fieldConfig.getScaleFactor()));
    }
}
