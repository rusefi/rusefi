package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.FieldType;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Optional;

/**
 * Handles non-shrinking resize of 2D tables and their axes.
 *
 * <h3>Testing and Coverage Examples:</h3>
 * This migrator is tested in {@code TableAddColumnsMigratorTest} in the {@code java_console/ui} module.
 * Examples of tables covered by this migrator include:
 * <ul>
 *   <li>{@code veTable} (VE table migration)</li>
 *   <li>{@code ignitionTable} (Ignition table migration)</li>
 * </ul>
 */
public class TableAddColumnsMigrator implements TuneMigrator {
    public static final String VE_TABLE_FIELD_NAME = "veTable";
    public static final String VE_RPM_BINS_FIELD_NAME = "veRpmBins";
    public static final String VE_LOAD_BINS_FIELD_NAME = "veLoadBins";
    public static final String LAMBDA_TABLE_FIELD_NAME = "lambdaTable";
    public static final String LAMBDA_RPM_BINS_FIELD_NAME = "lambdaRpmBins";
    public static final String LAMBDA_LOAD_BINS_FIELD_NAME = "lambdaLoadBins";
    public static final String IGNITION_TABLE_FIELD_NAME = "ignitionTable";
    public static final String IGNITION_RPM_BINS_FIELD_NAME = "ignitionRpmBins";
    public static final String IGNITION_LOAD_BINS_FIELD_NAME = "ignitionLoadBins";
    public static final String INJECTION_PHASE_FIELD_NAME = "injectionPhase";
    public static final String INJECTION_PHASE_RPM_BINS_FIELD_NAME = "injPhaseRpmBins";
    public static final String INJECTION_PHASE_LOAD_BINS_FIELD_NAME = "injPhaseLoadBins";
    public static final String MAP_SAMPLING_FIELD_NAME = "map_samplingAngle";
    public static final String MAP_SAMPLING_RPM_BINS_NAME = "map_samplingAngleBins";
    public static final String PEDAL_TO_TPS_TABLE_FIELD_NAME = "pedalToTpsTable";
    public static final String PEDAL_TO_TPS_RPM_BINS_FIELD_NAME = "pedalToTpsRpmBins";
    public static final String PEDAL_TO_TPS_PEDAL_BINS_FIELD_NAME = "pedalToTpsPedalBins";

    public static final TableAddColumnsMigrator VE_TABLE_MIGRATOR = new TableAddColumnsMigrator(
        VE_TABLE_FIELD_NAME,
        FieldType.UINT16,
        VE_RPM_BINS_FIELD_NAME,
        VE_LOAD_BINS_FIELD_NAME,
        RetainTableValuesConverter.INSTANCE
    );

    public static final TableAddColumnsMigrator LAMBDA_TABLE_MIGRATOR = new TableAddColumnsMigrator(
        LAMBDA_TABLE_FIELD_NAME,
        FieldType.UINT8,
        LAMBDA_RPM_BINS_FIELD_NAME,
        LAMBDA_LOAD_BINS_FIELD_NAME,
        AfrLambdaTableValuesConverter.INSTANCE
    );

    public static final TableAddColumnsMigrator IGNITION_TABLE_MIGRATOR = new TableAddColumnsMigrator(
        IGNITION_TABLE_FIELD_NAME,
        FieldType.INT16,
        IGNITION_RPM_BINS_FIELD_NAME,
        IGNITION_LOAD_BINS_FIELD_NAME,
        RetainTableValuesConverter.INSTANCE
    );

    public static final TableAddColumnsMigrator INJECTION_PHASE_MIGRATOR = new TableAddColumnsMigrator(
        INJECTION_PHASE_FIELD_NAME ,
        FieldType.INT16,
        INJECTION_PHASE_RPM_BINS_FIELD_NAME,
        INJECTION_PHASE_LOAD_BINS_FIELD_NAME,
        RetainTableValuesConverter.INSTANCE
    );

    public static final TableAddColumnsMigrator MAP_SAMPLING_MIGRATOR = new TableAddColumnsMigrator(
        MAP_SAMPLING_FIELD_NAME ,
        FieldType.FLOAT,
        null,
        MAP_SAMPLING_RPM_BINS_NAME,
        MapSamplingValuesConverter.INSTANCE
    );

    public static final TableAddColumnsMigrator PEDAL_TO_TPS_MIGRATOR = new TableAddColumnsMigrator(
        PEDAL_TO_TPS_TABLE_FIELD_NAME,
        FieldType.UINT8,
        PEDAL_TO_TPS_RPM_BINS_FIELD_NAME,
        PEDAL_TO_TPS_PEDAL_BINS_FIELD_NAME,
        RetainTableValuesConverter.INSTANCE
    );

    private final String tableFieldName;
    private final FieldType tableFieldType;
    private final String columnsBinFieldName;
    private final String rowsBinFieldName;
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
        this(tableIniFieldName, tableIniFieldType, columnsIniBinFieldName, null, tableValuesConverter);
    }

    private TableAddColumnsMigrator(
        final String tableIniFieldName,
        final FieldType tableIniFieldType,
        final String columnsIniBinFieldName,
        final String rowsIniBinFieldName,
        final TableValuesConverter tableValuesConverter
    ) {
        tableFieldName = tableIniFieldName;
        tableFieldType = tableIniFieldType;
        columnsBinFieldName = columnsIniBinFieldName;
        rowsBinFieldName = rowsIniBinFieldName;
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
            final int prevTableFieldRows = prevTableField.getRows();
            final ArrayIniField updatedTableField = updatedArrayIniField.get();
            final int updatedTableFieldCols = updatedTableField.getCols();
            final int updatedTableFieldRows = updatedTableField.getRows();
            final boolean columnsGrow = prevTableFieldCols < updatedTableFieldCols;
            final boolean rowsGrow = prevTableFieldRows < updatedTableFieldRows;
            if (!columnsGrow && !rowsGrow) {
                if (updatedTableFieldCols < prevTableFieldCols || updatedTableFieldRows < prevTableFieldRows) {
                    context.getCallbacks().logLine(String.format(
                        "WARNING! `%s` ini-field cannot be migrated from %dx%d to smaller %dx%d dimensions",
                        tableFieldName,
                        prevTableFieldCols,
                        prevTableFieldRows,
                        updatedTableFieldCols,
                        updatedTableFieldRows
                    ));
                }
                return;
            }
            if (updatedTableFieldCols < prevTableFieldCols || updatedTableFieldRows < prevTableFieldRows) {
                context.getCallbacks().logLine(String.format(
                    "WARNING! `%s` ini-field cannot be migrated when one dimension shrinks",
                    tableFieldName
                ));
                return;
            }
            if (rowsGrow && rowsBinFieldName == null) {
                context.getCallbacks().logLine(String.format(
                    "WARNING! `%s` ini-field cannot add rows without a row-axis field",
                    tableFieldName
                ));
                return;
            }
            if (columnsGrow && columnsBinFieldName == null) {
                context.getCallbacks().logLine(String.format(
                    "WARNING! `%s` ini-field cannot add columns without a column-axis field",
                    tableFieldName
                ));
                return;
            }

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
                        final Map<String, Constant> migrations = new LinkedHashMap<>();
                        migrations.put(
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

                        if (columnsGrow) {
                            final Optional<Constant> columnsMigration = migrateBins(
                                context,
                                columnsBinFieldName,
                                prevTableFieldCols,
                                updatedTableFieldCols
                            );
                            if (!columnsMigration.isPresent()) {
                                return;
                            }
                            migrations.put(columnsBinFieldName, columnsMigration.get());
                        }
                        if (rowsGrow) {
                            final Optional<Constant> rowsMigration = migrateBins(
                                context,
                                rowsBinFieldName,
                                prevTableFieldRows,
                                updatedTableFieldRows
                            );
                            if (!rowsMigration.isPresent()) {
                                return;
                            }
                            migrations.put(rowsBinFieldName, rowsMigration.get());
                        }

                        migrations.forEach(context::addMigration);
                    }
                }
            }
        }
    }

    private Optional<Constant> migrateBins(
        final TuneMigrationContext context,
        final String binsFieldName,
        final int prevBinsCount,
        final int updatedBinsCount
    ) {
        final Constant prevValue = context.getPrevTune().getConstantsAsMap().get(binsFieldName);
        if (prevValue != null) {
            final Optional<IniField> prevField = context.getPrevIniFile().findIniField(binsFieldName);
            if (!prevField.isPresent()) {
                context.getCallbacks().logLine(String.format(
                    "WARNING!!! Missed `%s` ini field in previous .ini file.",
                    binsFieldName
                ));
                return Optional.empty();
            }
            final Optional<IniField> updatedField = context.getUpdatedIniFile().findIniField(binsFieldName);
            if (!updatedField.isPresent()) {
                context.getCallbacks().logLine(String.format(
                    "WARNING!!! Missed `%s` ini field in updated .ini file.",
                    binsFieldName
                ));
                return Optional.empty();
            }
            final Optional<String> migratedValue = new BinsIniFieldMigratorStrategy(
                binsFieldName,
                prevBinsCount,
                updatedBinsCount
            ).tryMigrateBins(
                prevField.get(),
                updatedField.get(),
                prevValue.getValue(),
                context.getCallbacks()
            );
            if (migratedValue.isPresent()) {
                final ArrayIniField updatedBinsField = (ArrayIniField) updatedField.get();
                return Optional.of(new Constant(
                    binsFieldName,
                    updatedBinsField.getUnits(),
                    migratedValue.get(),
                    updatedBinsField.getDigits(),
                    Integer.toString(updatedBinsField.getRows()),
                    Integer.toString(updatedBinsField.getCols())
                ));
            }
        } else {
            context.getCallbacks().logLine(String.format(
                "WARNING!!! Missed `%s` value in previous tune.",
                binsFieldName
            ));
        }
        return Optional.empty();
    }

    private Optional<String[][]> tryMigrateTable(
        final ArrayIniField prevField,
        final ArrayIniField newField,
        final String[][] prevValues,
        final UpdateOperationCallbacks callbacks
    ) {
        final int prevRows = prevField.getRows();
        final int prevCols = prevField.getCols();
        if (prevValues.length != prevRows) {
            callbacks.logLine(String.format(
                "WARNING! Previous `%s` value has %d rows instead of %d",
                tableFieldName,
                prevValues.length,
                prevRows
            ));
            return Optional.empty();
        }
        for (final String[] row : prevValues) {
            if (row.length != prevCols) {
                callbacks.logLine(String.format(
                    "WARNING! Previous `%s` value has a row with %d columns instead of %d",
                    tableFieldName,
                    row.length,
                    prevCols
                ));
                return Optional.empty();
            }
        }

        final int newRows = newField.getRows();
        final int newCols = newField.getCols();
        if (newRows < prevRows || newCols < prevCols) {
            return Optional.empty();
        }

        final String[][] newValues = new String[newRows][newCols];
        for (int rowIdx = 0; rowIdx < newRows; rowIdx++) {
            for (int colIdx = 0; colIdx < newCols; colIdx++) {
                newValues[rowIdx][colIdx] = prevValues[
                    Math.min(rowIdx, prevRows - 1)
                ][
                    Math.min(colIdx, prevCols - 1)
                ];
            }
        }
        return Optional.of(newValues);
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
