package com.rusefi.maintenance.migration;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.FieldType;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.tune.xml.Constant;

import java.util.Optional;

public class VeTableExtensionMigrator implements TuneMigrator {
    static final String VE_TABLE_FIELD_NAME = "veTable";
    static final String VE_RPM_BINS_FIELD_NAME = "veRpmBins";

    static final int VE_TABLE_ROWS = 16;
    // todo: can this be more dynamic?
    static final int OLD_VE_TABLE_COLS = 16;
    static final int NEW_VE_TABLE_COLS = 24;

    private final String tableFieldName;
    private final FieldType tableFieldType;
    private final String columnsBinFieldName;

    VeTableExtensionMigrator(
        final String tableIniFieldName,
        final FieldType tableIniFieldType,
        final String columnsIniBinFieldName
    ) {
        tableFieldName = tableIniFieldName;
        tableFieldType = tableIniFieldType;
        columnsBinFieldName = columnsIniBinFieldName;
    }

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        migrateVeTable(context);
        migrateVeRpmBinsTable(context);
    }

    private void migrateVeTable(final TuneMigrationContext context) {
        final Constant prevValue = context.getPrevTune().getConstantsAsMap().get(tableFieldName);
        if (prevValue != null) {
            final Optional<IniField> prevField = context.getPrevIniFile().findIniField(tableFieldName);
            if (!prevField.isPresent()) {
                context.getCallbacks().logLine(String.format(
                    "WARNING!!! Missed `%s` ini field in previous .ini file.",
                    tableFieldName
                ));
                return;
            }
            final Optional<IniField> updatedField = context.getUpdatedIniFile().findIniField(tableFieldName);
            if (!updatedField.isPresent()) {
                context.getCallbacks().logLine(String.format(
                    "WARNING!!! Missed `%s` ini field in updated .ini file.",
                    tableFieldName
                ));
                return;
            }
            final Optional<ArrayIniField> prevArrayIniField = getValidatedVeTableArrayIniField(
                prevField.get(),
                context.getCallbacks()
            );
            final Optional<ArrayIniField> updatedArrayIniField = getValidatedVeTableArrayIniField(
                updatedField.get(),
                context.getCallbacks()
            );
            if (prevArrayIniField.isPresent() && updatedArrayIniField.isPresent()) {
                final ArrayIniField prevVeTableField = prevArrayIniField.get();
                final ArrayIniField updatedVeTableField = updatedArrayIniField.get();
                if ((prevVeTableField.getCols() == OLD_VE_TABLE_COLS)
                    && (updatedVeTableField.getCols() == NEW_VE_TABLE_COLS)
                ) {
                    final Optional<String> migratedValue = tryMigrateVeTable(
                        prevVeTableField,
                        updatedVeTableField,
                        prevValue.getValue(),
                        context.getCallbacks()
                    );
                    if (migratedValue.isPresent()) {
                        context.addMigration(
                            tableFieldName,
                            new Constant(
                                tableFieldName,
                                updatedVeTableField.getUnits(),
                                migratedValue.get(),
                                updatedVeTableField.getDigits(),
                                Integer.toString(updatedVeTableField.getRows()),
                                Integer.toString(updatedVeTableField.getCols())
                            )
                        );
                    }
                }
            }
        }
    }

    private void migrateVeRpmBinsTable(final TuneMigrationContext context) {
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
            final ArrayIniField updatedVeRpmBinsField = (ArrayIniField) updatedField.get();
            final Optional<String> migratedValue = new BinsIniFieldMigrator(
                columnsBinFieldName,
                OLD_VE_TABLE_COLS,
                NEW_VE_TABLE_COLS
            ).tryMigrateBins(
                prevField.get(),
                updatedVeRpmBinsField,
                prevValue.getValue(),
                context.getCallbacks()
            );
            if (migratedValue.isPresent()) {
                context.addMigration(
                    columnsBinFieldName,
                    new Constant(
                        columnsBinFieldName,
                        updatedVeRpmBinsField.getUnits(),
                        migratedValue.get(),
                        updatedVeRpmBinsField.getDigits(),
                        Integer.toString(updatedVeRpmBinsField.getRows()),
                        Integer.toString(updatedVeRpmBinsField.getCols())
                    )
                );
            }
        }
    }

    private Optional<String> tryMigrateVeTable(
        final IniField prevField,
        final IniField newField,
        final String prevValue,
        final UpdateOperationCallbacks callbacks
    ) {
        Optional<String> result = Optional.empty();
        final Optional<ArrayIniField> prevVeTableValidatedField = getValidatedVeTableArrayIniField(prevField, callbacks);
        final Optional<ArrayIniField> newVeTableValidatedField = getValidatedVeTableArrayIniField(newField, callbacks);
        if (prevVeTableValidatedField.isPresent() && newVeTableValidatedField.isPresent()) {
            final ArrayIniField prevVeTableField = prevVeTableValidatedField.get();
            final ArrayIniField newVeTableField = newVeTableValidatedField.get();
            final int prevVeTableFieldCols = prevVeTableField.getCols();
            final int newVeTableFieldCols = newVeTableField.getCols();
            if ((prevVeTableFieldCols == OLD_VE_TABLE_COLS) && (newVeTableFieldCols == NEW_VE_TABLE_COLS)) {
                final String[][] prevValues = prevVeTableField.getValues(prevValue);
                final String[][] newValues = new String[VE_TABLE_ROWS][NEW_VE_TABLE_COLS];
                for (int rowIdx = 0; rowIdx < VE_TABLE_ROWS; rowIdx++) {
                    // copy prev values:
                    for (int colIdx = 0; colIdx < OLD_VE_TABLE_COLS; colIdx++) {
                        newValues[rowIdx][colIdx] = prevValues[rowIdx][colIdx];
                    }
                    // propagate value from a last column to new columns:
                    for (int colIdx = OLD_VE_TABLE_COLS; colIdx < NEW_VE_TABLE_COLS; colIdx++) {
                        newValues[rowIdx][colIdx] = prevValues[rowIdx][OLD_VE_TABLE_COLS - 1];
                    }
                }
                result = Optional.of(newVeTableField.formatValue(newValues));
            }
        }
        return result;
    }

    private Optional<ArrayIniField> getValidatedVeTableArrayIniField(
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
                "WARNING! `%s` ini-field is expected to be `ArrayIniField` instead of %s",
                tableFieldName,
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final int arrayFieldRows = arrayField.getRows();
        if (arrayFieldRows != VE_TABLE_ROWS) {
            callbacks.logLine(String.format(
                "WARNING! `%s` ini-field is expected to contain %d rows instead of %d",
                tableFieldName,
                VE_TABLE_ROWS,
                arrayFieldRows
            ));
            return Optional.empty();
        }
        final int arrayFieldCols = arrayField.getCols();
        switch (arrayFieldCols) {
            case OLD_VE_TABLE_COLS:
            case NEW_VE_TABLE_COLS: {
                return Optional.of(arrayField);
            }
            default: {
                callbacks.logLine(String.format(
                    "WARNING! `%s` ini-field is expected to contain %d or %d columns instead of %d",
                    tableFieldName,
                    OLD_VE_TABLE_COLS,
                    NEW_VE_TABLE_COLS,
                    arrayFieldRows
                ));
                return Optional.empty();
            }
        }
    }
}
