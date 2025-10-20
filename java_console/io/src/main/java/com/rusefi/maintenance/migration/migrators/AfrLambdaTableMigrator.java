package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.FieldType;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Objects;
import java.util.Optional;

import static com.rusefi.maintenance.migration.migrators.TableAddColumnsMigrator.LAMBDA_TABLE_FIELD_NAME;

public enum AfrLambdaTableMigrator implements TuneMigrator {
    INSTANCE;

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        final Optional<IniField> prevField = context.getPrevIniFile().findIniField(LAMBDA_TABLE_FIELD_NAME);
        if (!prevField.isPresent()) {
            return;
        }
        final Optional<IniField> updatedField = context.getUpdatedIniFile().findIniField(LAMBDA_TABLE_FIELD_NAME);
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
            final ArrayIniField updatedTableField = updatedArrayIniField.get();
            if (Objects.equals(prevTableField.getRows(), updatedTableField.getRows()) &&
                Objects.equals(prevTableField.getCols(), updatedTableField.getCols())
            ) {
                final int updatedTableFieldCols = updatedTableField.getCols();
                final Constant prevValue = context.getPrevTune().getConstantsAsMap().get(LAMBDA_TABLE_FIELD_NAME);
                if (prevValue != null) {
                    final Optional<String[][]> migratedValues = AfrLambdaTableValuesConverter.INSTANCE.convertTableValues(
                        prevTableField.getValues(prevValue.getValue()),
                        context
                    );
                    if (migratedValues.isPresent()) {
                        context.addMigration(
                            LAMBDA_TABLE_FIELD_NAME,
                            new Constant(
                                LAMBDA_TABLE_FIELD_NAME,
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
        }
    }

    private Optional<ArrayIniField> getValidatedTableArrayIniField(
        final IniField field,
        final UpdateOperationCallbacks callbacks
    ) {
        if (!(field instanceof ArrayIniField)) {
            callbacks.logLine(String.format(
                "WARNING! `%s` ini-field is expected to be `ArrayIniField` instead of %s",
                LAMBDA_TABLE_FIELD_NAME,
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final ArrayIniField arrayField = (ArrayIniField) field;
        final FieldType arrayFieldType = arrayField.getType();
        if (arrayFieldType != FieldType.UINT8) {
            callbacks.logLine(String.format(
                "WARNING! `%s` ini-field is expected to be `UINT8` instead of `%s`",
                LAMBDA_TABLE_FIELD_NAME,
                arrayFieldType
            ));
            return Optional.empty();
        }
        return Optional.of(arrayField);
    }
}
