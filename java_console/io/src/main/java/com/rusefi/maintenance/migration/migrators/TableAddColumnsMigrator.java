package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.FieldType;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Optional;

public class TableAddColumnsMigrator implements TuneMigrator {
    public static final String VE_TABLE_FIELD_NAME = "veTable";
    public static final String VE_RPM_BINS_FIELD_NAME = "veRpmBins";
    public static final String LAMBDA_TABLE_FIELD_NAME = "lambdaTable";
    public static final String LAMBDA_RPM_BINS_FIELD_NAME = "lambdaRpmBins";
    public static final String IGNITION_TABLE_FIELD_NAME = "ignitionTable";
    public static final String IGNITION_RPM_BINS_FIELD_NAME = "ignitionRpmBins";
    public static final String INJECTION_PHASE_FIELD_NAME = "injectionPhase";
    public static final String INJECTION_PHASE_RPM_BINS_FIELD_NAME = "injPhaseRpmBins";
    public static final String MAP_SAMPLING_FIELD_NAME = "map_samplingAngle";
    public static final String MAP_SAMPLING_RPM_BINS_NAME = "map_samplingAngleBins";

    public static final TableAddColumnsMigrator VE_TABLE_MIGRATOR = new TableAddColumnsMigrator(
        VE_TABLE_FIELD_NAME,
        FieldType.UINT16,
        VE_RPM_BINS_FIELD_NAME,
        RetainTableValuesConverter.INSTANCE
    );

    public static final TableAddColumnsMigrator LAMBDA_TABLE_MIGRATOR = new TableAddColumnsMigrator(
        LAMBDA_TABLE_FIELD_NAME,
        FieldType.UINT8,
        LAMBDA_RPM_BINS_FIELD_NAME,
        AfrLambdaTableValuesConverter.INSTANCE
    );

    public static final TableAddColumnsMigrator IGNITION_TABLE_MIGRATOR = new TableAddColumnsMigrator(
        IGNITION_TABLE_FIELD_NAME,
        FieldType.INT16,
        IGNITION_RPM_BINS_FIELD_NAME,
        RetainTableValuesConverter.INSTANCE
    );

    public static final TableAddColumnsMigrator INJECTION_PHASE_MIGRATOR = new TableAddColumnsMigrator(
        INJECTION_PHASE_FIELD_NAME ,
        FieldType.INT16,
        INJECTION_PHASE_RPM_BINS_FIELD_NAME,
        RetainTableValuesConverter.INSTANCE
    );

    public static final TableAddColumnsMigrator MAP_SAMPLING_MIGRATOR = new TableAddColumnsMigrator(
        MAP_SAMPLING_FIELD_NAME ,
        FieldType.FLOAT,
        MAP_SAMPLING_RPM_BINS_NAME,
        MapSamplingValuesConverter.INSTANCE
    );

    private final String tableFieldName;
    private final FieldType tableFieldType;
    private final String columnsBinFieldName;
    private final TableValuesConverter prevTableValueConverter;

    private static class RetainTableValuesConverter implements TableValuesConverter {
        static RetainTableValuesConverter INSTANCE = new RetainTableValuesConverter();

        private RetainTableValuesConverter() {}

        @Override
        public Optional<String[][]> convertTableValues(
            final String[][] prevValues,
            final TuneMigrationContext context
        ) {
            // default implementation does not modify previous table values:
            return Optional.of(prevValues);
        }
    }

    private TableAddColumnsMigrator(
        final String tableIniFieldName,
        final FieldType tableIniFieldType,
        final String columnsIniBinFieldName,
        final TableValuesConverter tableValuesConverter
    ) {
        tableFieldName = tableIniFieldName;
        tableFieldType = tableIniFieldType;
        columnsBinFieldName = columnsIniBinFieldName;
        prevTableValueConverter = tableValuesConverter;
    }

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        final Optional<IniField> prevField = context.getPrevIniFile().findIniField(tableFieldName);
        if (!prevField.isPresent()) {
            return;
        }
        final Optional<IniField> updatedField = context.getUpdatedIniFile().findIniField(tableFieldName);
        if (!updatedField.isPresent()) {
            return;
        }
        final Optional<ArrayIniField> prevArrayIniField = getValidatedTableArrayIniField(
            prevField.get(),
            context.getCallbacks()
        );
        final Optional<ArrayIniField> updatedArrayIniField = getValidatedTableArrayIniField(
            updatedField.get(),
            context.getCallbacks()
        );
        if (prevArrayIniField.isPresent() && updatedArrayIniField.isPresent()) {
            final ArrayIniField prevTableField = prevArrayIniField.get();
            final int prevTableFieldCols = prevTableField.getCols();
            final ArrayIniField updatedTableField = updatedArrayIniField.get();
            final int updatedTableFieldCols = updatedTableField.getCols();
            final Constant prevValue = context.getPrevTune().getConstantsAsMap().get(tableFieldName);
            if (prevValue != null) {
                final Optional<String[][]> convertedPrevValues = prevTableValueConverter.convertTableValues(
                    prevTableField.getValues(prevValue.getValue()),
                    context
                );
                if (convertedPrevValues.isPresent()) {
                    final Optional<String[][]> migratedValues = tryMigrateTable(
                        prevTableField,
                        updatedTableField,
                        convertedPrevValues.get(),
                        context.getCallbacks()
                    );
                    if (migratedValues.isPresent()) {
                        context.addMigration(
                            tableFieldName,
                            new Constant(
                                tableFieldName,
                                updatedTableField.getUnits(),
                                updatedTableField.formatValue(migratedValues.get()),
                                updatedTableField.getDigits(),
                                Integer.toString(updatedTableField.getRows()),
                                Integer.toString(updatedTableFieldCols)
                            )
                        );
                    }
                }
            }
            migrateBins(context, prevTableFieldCols, updatedTableFieldCols);
        }
    }

    private void migrateBins(final TuneMigrationContext context, final int prevBinsCount, final int updatedBinsCount) {
        final Constant prevValue = context.getPrevTune().getConstantsAsMap().get(columnsBinFieldName);
        if (prevValue != null) {
            final Optional<IniField> prevField = context.getPrevIniFile().findIniField(columnsBinFieldName);
            if (!prevField.isPresent()) {
                context.getCallbacks().logLine(String.format(
                    "WARNING!!! Missed `%s` ini field in previous .ini file.",
                    columnsBinFieldName
                ));
                return;
            }
            final Optional<IniField> updatedField = context.getUpdatedIniFile().findIniField(columnsBinFieldName);
            if (!updatedField.isPresent()) {
                context.getCallbacks().logLine(String.format(
                    "WARNING!!! Missed `%s` ini field in updated .ini file.",
                    columnsBinFieldName
                ));
                return;
            }
            final ArrayIniField updatedBinsField = (ArrayIniField) updatedField.get();
            final Optional<String> migratedValue = new BinsIniFieldMigratorStrategy(
                columnsBinFieldName,
                prevBinsCount,
                updatedBinsCount
            ).tryMigrateBins(
                prevField.get(),
                updatedBinsField,
                prevValue.getValue(),
                context.getCallbacks()
            );
            if (migratedValue.isPresent()) {
                context.addMigration(
                    columnsBinFieldName,
                    new Constant(
                        columnsBinFieldName,
                        updatedBinsField.getUnits(),
                        migratedValue.get(),
                        updatedBinsField.getDigits(),
                        Integer.toString(updatedBinsField.getRows()),
                        Integer.toString(updatedBinsField.getCols())
                    )
                );
            }
        }
    }

    private Optional<String[][]> tryMigrateTable(
        final ArrayIniField prevField,
        final ArrayIniField newField,
        final String[][] prevValues,
        final UpdateOperationCallbacks callbacks
    ) {
        Optional<String[][]> result = Optional.empty();
        final int tableFieldRows = prevField.getRows();
        if (newField.getRows() == tableFieldRows) {
            final int prevTableFieldCols = prevField.getCols();
            final int newTableFieldCols = newField.getCols();
            if (prevTableFieldCols < newTableFieldCols) {
                final String[][] newValues = new String[tableFieldRows][newTableFieldCols];
                for (int rowIdx = 0; rowIdx < tableFieldRows; rowIdx++) {
                    // copy prev values:
                    for (int colIdx = 0; colIdx < prevTableFieldCols; colIdx++) {
                        newValues[rowIdx][colIdx] = prevValues[rowIdx][colIdx];
                    }
                    // propagate value from a last column to new columns:
                    for (int colIdx = prevTableFieldCols; colIdx < newTableFieldCols; colIdx++) {
                        newValues[rowIdx][colIdx] = prevValues[rowIdx][prevTableFieldCols - 1];
                    }
                }
                result = Optional.of(newValues);
            }
        } else {
            callbacks.logLine(String.format(
                "WARNING! New `%s` ini-field is expected to contain %d rows as prev ini-field does",
                tableFieldName,
                tableFieldRows
            ));
        }
        return result;
    }

    private Optional<ArrayIniField> getValidatedTableArrayIniField(
        final IniField field,
        final UpdateOperationCallbacks callbacks
    ) {
        if (!(field instanceof ArrayIniField)) {
            callbacks.logLine(String.format(
                "WARNING! `%s` ini-field is expected to be `ArrayIniField` instead of %s",
                tableFieldName,
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final ArrayIniField arrayField = (ArrayIniField) field;
        final FieldType arrayFieldType = arrayField.getType();
        if (arrayFieldType != tableFieldType) {
            callbacks.logLine(String.format(
                "WARNING! `%s` ini-field is expected to be `%s` instead of `%s`",
                tableFieldName,
                tableFieldType,
                arrayFieldType
            ));
            return Optional.empty();
        }
        return Optional.of(arrayField);
    }
}
