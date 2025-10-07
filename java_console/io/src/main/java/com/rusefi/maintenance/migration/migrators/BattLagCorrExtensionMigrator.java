package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.FieldType;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Map;
import java.util.Optional;

public enum BattLagCorrExtensionMigrator implements TuneMigrator {
    INSTANCE;

    /* Old fields: */
    public static final String INJECTOR_BATT_LAG_CORR_BINS_FIELD_NAME = "injector_battLagCorrBins";
    public static final String INJECTOR_BATT_LAG_CORR_FIELD_NAME = "injector_battLagCorr";

    /* New fields: */
    public static final String INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME = "injector_battLagCorrBattBins";
    public static final String INJECTOR_BATT_LAG_CORR_PRESS_BINS_FIELD_NAME = "injector_battLagCorrPressBins";
    public static final String INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME = "injector_battLagCorrTable";

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        final IniFileModel prevIniFile = context.getPrevIniFile();
        final Optional<IniField> prevInjectorBattLagCorrBinsField = prevIniFile.findIniField(
            INJECTOR_BATT_LAG_CORR_BINS_FIELD_NAME
        );
        if (!prevInjectorBattLagCorrBinsField.isPresent()) {
            return;
        }
        final Optional<IniField> prevInjectorBattLagCorrField = prevIniFile.findIniField(
            INJECTOR_BATT_LAG_CORR_FIELD_NAME
        );
        if (!prevInjectorBattLagCorrField.isPresent()) {
            return;
        }
        if (prevIniFile.findIniField(INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME).isPresent()) {
            context.logWarn(String.format(
                "Prev ini file contains both `%s` and `%s` ini fields",
                INJECTOR_BATT_LAG_CORR_BINS_FIELD_NAME,
                INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME
            ));
            return;
        }
        if (prevIniFile.findIniField(INJECTOR_BATT_LAG_CORR_PRESS_BINS_FIELD_NAME).isPresent()) {
            context.logWarn(String.format(
                "Prev ini file contains both `%s` and `%s` ini fields",
                INJECTOR_BATT_LAG_CORR_BINS_FIELD_NAME,
                INJECTOR_BATT_LAG_CORR_PRESS_BINS_FIELD_NAME
            ));
            return;
        }
        if (prevIniFile.findIniField(INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME).isPresent()) {
            context.logWarn(String.format(
                "Prev ini file contains both `%s` and `%s` ini fields",
                INJECTOR_BATT_LAG_CORR_FIELD_NAME,
                INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME
            ));
            return;
        }
        final IniFileModel updatedIniFile = context.getUpdatedIniFile();
        final Optional<IniField> updatedInjectorBattLagCorrBattBinsField = updatedIniFile.findIniField(
            INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME
        );
        if (!updatedInjectorBattLagCorrBattBinsField.isPresent()) {
            return;
        }
        final Optional<IniField> updatedInjectorBattLagCorrPressBinsField = updatedIniFile.findIniField(
            INJECTOR_BATT_LAG_CORR_PRESS_BINS_FIELD_NAME
        );
        if (!updatedInjectorBattLagCorrPressBinsField.isPresent()) {
            return;
        }
        final Optional<IniField> updatedInjectorBattLagCorrTableField = updatedIniFile.findIniField(
            INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME
        );
        if (!updatedInjectorBattLagCorrTableField.isPresent()) {
            return;
        }
        if (updatedIniFile.findIniField(INJECTOR_BATT_LAG_CORR_BINS_FIELD_NAME).isPresent()) {
            context.logWarn(String.format(
                "Updated ini file contains both `%s` and `%s` ini fields",
                INJECTOR_BATT_LAG_CORR_BINS_FIELD_NAME,
                INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME
            ));
            return;
        }
        if (updatedIniFile.findIniField(INJECTOR_BATT_LAG_CORR_FIELD_NAME).isPresent()) {
            context.logWarn(String.format(
                "Updated ini file contains both `%s` and `%s` ini fields",
                INJECTOR_BATT_LAG_CORR_FIELD_NAME,
                INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME
            ));
            return;
        }
        final Optional<ArrayIniField> injectorBattLagCorrBinsArrayIniField =
            getValidatedInjectorBattLagCorrBinsArrayIniField(prevInjectorBattLagCorrBinsField.get(), context.getCallbacks());
        if (!injectorBattLagCorrBinsArrayIniField.isPresent()) {
            return;
        }
        final Optional<ArrayIniField> injectorBattLagCorrArrayIniField =
            getValidatedInjectorBattLagCorrArrayIniField(prevInjectorBattLagCorrField.get(), context.getCallbacks());
        if (!injectorBattLagCorrArrayIniField.isPresent()) {
            return;
        }
        final Optional<ArrayIniField> injectorBattLagCorrBattBinsArrayIniField =
            getValidatedInjectorBattLagCorrBattBinsArrayIniField(
                updatedInjectorBattLagCorrBattBinsField.get(),
                context.getCallbacks()
            );
        if (!injectorBattLagCorrBattBinsArrayIniField.isPresent()) {
            return;
        }
        final Optional<ArrayIniField> injectorBattLagCorrPressBinsArrayIniField =
            getValidatedInjectorBattLagCorrPressBinsArrayIniField(
                updatedInjectorBattLagCorrPressBinsField.get(),
                context.getCallbacks()
            );
        if (!injectorBattLagCorrPressBinsArrayIniField.isPresent()) {
            return;
        }
        final Optional<ArrayIniField> injectorBattLagCorrTableArrayIniField =
            getValidatedInjectorBattLagCorrTableArrayIniField(
                updatedInjectorBattLagCorrTableField.get(),
                context.getCallbacks()
            );
        if (!injectorBattLagCorrTableArrayIniField.isPresent()) {
            return;
        }

        final Map<String, Constant> prevTune = context.getPrevTune().getConstantsAsMap();
        final Constant prevInjectorBattLagCorrBins = prevTune.get(INJECTOR_BATT_LAG_CORR_BINS_FIELD_NAME);
        if (prevInjectorBattLagCorrBins == null) {
            return;
        }
        final Constant prevInjectorBattLagCorr = prevTune.get(INJECTOR_BATT_LAG_CORR_FIELD_NAME);
        if (prevInjectorBattLagCorr == null) {
            return;
        }
        final Map<String, Constant> updatedTune = context.getUpdatedTune().getConstantsAsMap();
        final Constant updatedInjectorBattLagCorrTable = updatedTune.get(INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME);
        if ((updatedInjectorBattLagCorrTable == null) ||
            !updatedInjectorBattLagCorrTable.getValue().equals(prevInjectorBattLagCorr.getValue())
        ) {
            context.addMigration(
                INJECTOR_BATT_LAG_CORR_BINS_FIELD_NAME,
                INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME,
                generateConstant(injectorBattLagCorrBattBinsArrayIniField.get(), prevInjectorBattLagCorrBins.getValue())
            );
        }
        context.addMigration(
            INJECTOR_BATT_LAG_CORR_FIELD_NAME,
            INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME,
            generateConstant(
                injectorBattLagCorrTableArrayIniField.get(),
                migrateInjectorBattLagCorrValue(
                    injectorBattLagCorrArrayIniField.get(),
                    injectorBattLagCorrTableArrayIniField.get(),
                    prevInjectorBattLagCorr.getValue()
                )
            )
        );
    }

    private static Constant generateConstant(
        final ArrayIniField iniField,
        final String value
    ) {
        return new Constant(
            iniField.getName(),
            iniField.getUnits(),
            value,
            iniField.getDigits(),
            String.valueOf(iniField.getRows()),
            String.valueOf(iniField.getCols())
        );
    }

    private String migrateInjectorBattLagCorrValue(
        final ArrayIniField prevInjectorBattLagCorrField,
        final ArrayIniField updatedInjectorBattLagCorrTableField,
        final String prevInjectorBattLagCorrValue
    ) {
        final String[][] prevValues = prevInjectorBattLagCorrField.getValues(prevInjectorBattLagCorrValue);
        final String[][] migratedValues = new String[2][prevValues.length];
        for (int i = 0; i < prevValues.length; i++) {
            migratedValues[0][i] = prevValues[i][0];
            migratedValues[1][i] = prevValues[i][0];
        }
        return updatedInjectorBattLagCorrTableField.formatValue(migratedValues);
    }

    private static Optional<ArrayIniField> getValidatedInjectorBattLagCorrBinsArrayIniField(
        final IniField field,
        final UpdateOperationCallbacks callbacks
    ) {
        if (!(field instanceof ArrayIniField)) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrBins` ini-field is expected to be `ArrayIniField` instead of %s",
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final ArrayIniField arrayField = (ArrayIniField) field;
        final FieldType arrayFieldType = arrayField.getType();
        if (arrayFieldType != FieldType.FLOAT) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrBins` ini-field is expected to be FLOAT instead of %s",
                arrayFieldType
            ));
            return Optional.empty();
        }
        final int arrayFieldRows = arrayField.getRows();
        if (arrayFieldRows != 8) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrBins` ini-field is expected to contain 8 rows instead of %d",
                arrayFieldRows
            ));
            return Optional.empty();
        }
        final int arrayFieldCols = arrayField.getCols();
        if (arrayFieldCols != 1) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrBins` ini-field is expected to contain 1 cols instead of %d",
                arrayFieldCols
            ));
            return Optional.empty();
        }
        return Optional.of(arrayField);
    }

    private static Optional<ArrayIniField> getValidatedInjectorBattLagCorrArrayIniField(
        final IniField field,
        final UpdateOperationCallbacks callbacks
    ) {
        if (!(field instanceof ArrayIniField)) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorr` ini-field is expected to be `ArrayIniField` instead of %s",
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final ArrayIniField arrayField = (ArrayIniField) field;
        final FieldType arrayFieldType = arrayField.getType();
        if (arrayFieldType != FieldType.FLOAT) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorr` ini-field is expected to be FLOAT instead of %s",
                arrayFieldType
            ));
            return Optional.empty();
        }
        final int arrayFieldRows = arrayField.getRows();
        if (arrayFieldRows != 8) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorr` ini-field is expected to contain 8 rows instead of %d",
                arrayFieldRows
            ));
            return Optional.empty();
        }
        final int arrayFieldCols = arrayField.getCols();
        if (arrayFieldCols != 1) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorr` ini-field is expected to contain 1 cols instead of %d",
                arrayFieldCols
            ));
            return Optional.empty();
        }
        return Optional.of(arrayField);
    }

    private static Optional<ArrayIniField> getValidatedInjectorBattLagCorrBattBinsArrayIniField(
        final IniField field,
        final UpdateOperationCallbacks callbacks
    ) {
        if (!(field instanceof ArrayIniField)) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrBattBins` ini-field is expected to be `ArrayIniField` instead of %s",
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final ArrayIniField arrayField = (ArrayIniField) field;
        final FieldType arrayFieldType = arrayField.getType();
        if (arrayFieldType != FieldType.INT16) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrBattBins` ini-field is expected to be INT16 instead of %s",
                arrayFieldType
            ));
            return Optional.empty();
        }
        final int arrayFieldRows = arrayField.getRows();
        if (arrayFieldRows != 8) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrBattBins` ini-field is expected to contain 8 rows instead of %d",
                arrayFieldRows
            ));
            return Optional.empty();
        }
        final int arrayFieldCols = arrayField.getCols();
        if (arrayFieldCols != 1) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrBattBins` ini-field is expected to contain 1 cols instead of %d",
                arrayFieldCols
            ));
            return Optional.empty();
        }
        return Optional.of(arrayField);
    }

    private static Optional<ArrayIniField> getValidatedInjectorBattLagCorrPressBinsArrayIniField(
        final IniField field,
        final UpdateOperationCallbacks callbacks
    ) {
        if (!(field instanceof ArrayIniField)) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrPressBins` ini-field is expected to be `ArrayIniField` instead of %s",
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final ArrayIniField arrayField = (ArrayIniField) field;
        final FieldType arrayFieldType = arrayField.getType();
        if (arrayFieldType != FieldType.INT) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrPressBins` ini-field is expected to be INT16 instead of %s",
                arrayFieldType
            ));
            return Optional.empty();
        }
        final int arrayFieldRows = arrayField.getRows();
        if (arrayFieldRows != 2) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrPressBins` ini-field is expected to contain 2 rows instead of %d",
                arrayFieldRows
            ));
            return Optional.empty();
        }
        final int arrayFieldCols = arrayField.getCols();
        if (arrayFieldCols != 1) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrPressBins` ini-field is expected to contain 1 cols instead of %d",
                arrayFieldCols
            ));
            return Optional.empty();
        }
        return Optional.of(arrayField);
    }

    private static Optional<ArrayIniField> getValidatedInjectorBattLagCorrTableArrayIniField(
        final IniField field,
        final UpdateOperationCallbacks callbacks
    ) {
        if (!(field instanceof ArrayIniField)) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrTable` ini-field is expected to be `ArrayIniField` instead of %s",
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final ArrayIniField arrayField = (ArrayIniField) field;
        final FieldType arrayFieldType = arrayField.getType();
        if (arrayFieldType != FieldType.INT16) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrTable` ini-field is expected to be INT16 instead of %s",
                arrayFieldType
            ));
            return Optional.empty();
        }
        final int arrayFieldRows = arrayField.getRows();
        if (arrayFieldRows != 2) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrTable` ini-field is expected to contain 2 rows instead of %d",
                arrayFieldRows
            ));
            return Optional.empty();
        }
        final int arrayFieldCols = arrayField.getCols();
        if (arrayFieldCols != 8) {
            callbacks.logLine(String.format(
                "WARNING! `injector_battLagCorrTable` ini-field is expected to contain 8 cols instead of %d",
                arrayFieldCols
            ));
            return Optional.empty();
        }
        return Optional.of(arrayField);
    }
}
