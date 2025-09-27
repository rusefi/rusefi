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
        migrateTable(context);
        migrateBins(context);
    }

    private void migrateTable(final TuneMigrationContext context) {
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
                final ArrayIniField updatedTableField = updatedArrayIniField.get();
                final Optional<String> migratedValue = tryMigrateTable(
                    prevTableField,
                    updatedTableField,
                    prevValue.getValue()
                );
                if (migratedValue.isPresent()) {
                    context.addMigration(
                        tableFieldName,
                        new Constant(
                            tableFieldName,
                            updatedTableField.getUnits(),
                            migratedValue.get(),
                            updatedTableField.getDigits(),
                            Integer.toString(updatedTableField.getRows()),
                            Integer.toString(updatedTableField.getCols())
                        )
                    );
                }
            }
        }
    }

    private void migrateBins(final TuneMigrationContext context) {
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
            final Optional<String> migratedValue = new BinsIniFieldMigrator(
                columnsBinFieldName,
                OLD_VE_TABLE_COLS,
                NEW_VE_TABLE_COLS
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

    private Optional<String> tryMigrateTable(
        final ArrayIniField prevField,
        final ArrayIniField newField,
        final String prevValue
    ) {
        Optional<String> result = Optional.empty();
        final int prevTableFieldCols = prevField.getCols();
        final int newTableFieldCols = newField.getCols();
        if (prevTableFieldCols < newTableFieldCols) {
            final String[][] prevValues = prevField.getValues(prevValue);
            final String[][] newValues = new String[VE_TABLE_ROWS][newTableFieldCols];
            for (int rowIdx = 0; rowIdx < VE_TABLE_ROWS; rowIdx++) {
                // copy prev values:
                for (int colIdx = 0; colIdx < prevTableFieldCols; colIdx++) {
                    newValues[rowIdx][colIdx] = prevValues[rowIdx][colIdx];
                }
                // propagate value from a last column to new columns:
                for (int colIdx = prevTableFieldCols; colIdx < newTableFieldCols; colIdx++) {
                    newValues[rowIdx][colIdx] = prevValues[rowIdx][prevTableFieldCols - 1];
                }
            }
            result = Optional.of(newField.formatValue(newValues));
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
