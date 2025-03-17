package com.rusefi.maintenance.migration;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.FieldType;
import com.rusefi.io.UpdateOperationCallbacks;

import java.util.Optional;

public enum VeTableExtensionMigrator implements IniFieldMigrator {
    INSTANCE;

    private static String VE_TABLE_FIELD_NAME = "veTable";
    private static String VE_RPM_BINS_FIELD_NAME = "veRpmBins";

    private static final FieldType VE_TABLE_TYPE = FieldType.UINT16;

    static final int VE_TABLE_ROWS = 16;
    static final int OLD_VE_TABLE_COLS = 16;
    static final int NEW_VE_TABLE_COLS = 24;

    @Override
    public Optional<String> tryMigrateValue(
        final IniField prevField,
        final IniField newField,
        final String prevValue,
        final UpdateOperationCallbacks callbacks
    ) {
        Optional<String> result = Optional.empty();
        final String prevFieldName = prevField.getName();
        final String newFieldName = newField.getName();
        if (VE_TABLE_FIELD_NAME.equals(prevFieldName) && VE_TABLE_FIELD_NAME.equals(newFieldName)) {
            result = tryMigrateVeTable(prevField, newField, prevValue, callbacks);
        } else if (VE_RPM_BINS_FIELD_NAME.equals(prevFieldName) && VE_RPM_BINS_FIELD_NAME.equals(newFieldName)) {
            result = VeRpmBinsIniFieldMigrator.INSTANCE.tryMigrateVeRpmBins(prevField, newField, prevValue, callbacks);
        }
        return result;
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

    private static Optional<ArrayIniField> getValidatedVeTableArrayIniField(
        final IniField field,
        final UpdateOperationCallbacks callbacks
    ) {
        if (!(field instanceof ArrayIniField)) {
            callbacks.logLine(String.format(
                "WARNING! `veTable` ini-field is expected to be `ArrayIniField` instead of %s",
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final ArrayIniField arrayField = (ArrayIniField) field;
        final FieldType arrayFieldType = arrayField.getType();
        if (arrayFieldType != VE_TABLE_TYPE) {
            callbacks.logLine(String.format(
                "WARNING! `veTable` ini-field is expected to be `ArrayIniField` instead of %s",
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final int arrayFieldRows = arrayField.getRows();
        if (arrayFieldRows != VE_TABLE_ROWS) {
            callbacks.logLine(String.format(
                "WARNING! `veTable` ini-field is expected to contain %d rows instead of %d",
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
                    "WARNING! `veTable` ini-field is expected to contain %d or %d columns " +
                        "instead of %d",
                    OLD_VE_TABLE_COLS,
                    NEW_VE_TABLE_COLS,
                    arrayFieldRows
                ));
                return Optional.empty();
            }
        }
    }
}
