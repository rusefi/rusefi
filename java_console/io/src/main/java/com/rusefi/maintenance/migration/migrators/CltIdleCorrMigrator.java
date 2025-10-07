package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Map;
import java.util.Optional;

public enum CltIdleCorrMigrator implements TuneMigrator {
    INSTANCE;

    public static final String MAN_IDLE_POSITION_FIELD_NAME = "manIdlePosition";
    public static final String CLT_IDLE_CORR_BINS_FIELD_NAME = "cltIdleCorrBins";
    public static final String CLT_IDLE_CORR_FIELD_NAME = "cltIdleCorr";

    public static final String OVERRIDE_CRANKING_IAC_SETTING_FIELD_NAME = "overrideCrankingIacSetting";

    @Override
    public void migrateTune(final TuneMigrationContext context) {
        checkIfOverrideCrankingIacSettingHasDisappeared(context);
        final IniFileModel updatedIniFile = context.getUpdatedIniFile();
        if (!updatedIniFile.findIniField(MAN_IDLE_POSITION_FIELD_NAME).isPresent()) {
            final Map<String, Constant> prevTune = context.getPrevTune().getConstantsAsMap();
            final Constant prevManIdlePositionValue = prevTune.get(MAN_IDLE_POSITION_FIELD_NAME);
            if (prevManIdlePositionValue == null) {
                return;
            }
            final Constant prevCltIdleCorrValue = prevTune.get(CLT_IDLE_CORR_FIELD_NAME);
            if (prevCltIdleCorrValue == null) {
                return;
            }
            final IniFileModel prevIniFile = context.getPrevIniFile();
            final Optional<IniField> prevCltIdleCorrIniField = prevIniFile.findIniField(CLT_IDLE_CORR_FIELD_NAME);
            if (!prevCltIdleCorrIniField.isPresent()) {
                return;
            }
            final Optional<ArrayIniField> prevCltIdleCorrArrayIniField = getValidatedCltIdleCorrIniField(
                prevCltIdleCorrIniField.get(),
                context.getCallbacks()
            );
            if (!prevCltIdleCorrArrayIniField.isPresent()) {
                return;
            }
            final Optional<IniField> updatedCltIdleCorrIniField = updatedIniFile.findIniField(CLT_IDLE_CORR_FIELD_NAME);
            if (!updatedCltIdleCorrIniField.isPresent()) {
                return;
            }
            final Optional<ArrayIniField> updatedCltIdleCorrArrayIniField = getValidatedCltIdleCorrIniField(
                updatedCltIdleCorrIniField.get(),
                context.getCallbacks()
            );
            if (!updatedCltIdleCorrArrayIniField.isPresent()) {
                return;
            }
            final Optional<IniField> prevManIdlePositionIniField = prevIniFile.findIniField(
                MAN_IDLE_POSITION_FIELD_NAME
            );
            if (!prevManIdlePositionIniField.isPresent()) {
                return;
            }
            final Optional<ScalarIniField> prevManIdlePositionScalarIniField = getValidatedManIdlePositionIniField(
                prevManIdlePositionIniField.get(),
                context.getCallbacks()
            );
            if (!prevManIdlePositionScalarIniField.isPresent()) {
                return;
            }
            final String[][] prevValues = prevCltIdleCorrArrayIniField.get().getValues(prevCltIdleCorrValue.getValue());
            final String[][] migratedValues = migrateCltIdleCorrValue(prevManIdlePositionValue.getValue(), prevValues);
            final ArrayIniField updatedCltIdleCorrField = updatedCltIdleCorrArrayIniField.get();
            context.addMigration(
                CLT_IDLE_CORR_FIELD_NAME,
                new Constant(
                    CLT_IDLE_CORR_FIELD_NAME,
                    updatedCltIdleCorrField.getUnits(),
                    updatedCltIdleCorrField.formatValue(migratedValues),
                    updatedCltIdleCorrField.getDigits(),
                    Integer.toString(updatedCltIdleCorrField.getRows()),
                    Integer.toString(updatedCltIdleCorrField.getCols())
                )
            );
            // Add null migration to prevent processing of disappeared `ManIdlePosition` ini-field by
            // `DefaultTuneMigrator`:
            context.addMigration(MAN_IDLE_POSITION_FIELD_NAME, null);
        }
    }

    static private String[][] migrateCltIdleCorrValue(
        final String prevManIdlePositionValue,
        final String[][] prevCltIdleCorrValues
    ) {
        String[][] result = new String[prevCltIdleCorrValues.length][];

        final double manIdlePositionRatio = Double.parseDouble(prevManIdlePositionValue);
        for (int row = 0; row < prevCltIdleCorrValues.length; row++) {
            final String[] prevCltIdleCorrRow = prevCltIdleCorrValues[row];
            final String[] updatedCltIdleCorrRow = new String[prevCltIdleCorrRow.length];
            for (int column = 0; column < prevCltIdleCorrRow.length; column++) {
                final double value = Double.parseDouble(prevCltIdleCorrValues[row][column]);
                updatedCltIdleCorrRow[column] = Double.toString(manIdlePositionRatio * value);
            }
            result[row] = updatedCltIdleCorrRow;
        }
        return result;
    }

    private static Optional<ScalarIniField> getValidatedManIdlePositionIniField(
        final IniField field,
        final UpdateOperationCallbacks callbacks
    ) {
        if (!(field instanceof ScalarIniField)) {
            callbacks.logLine(String.format(
                "WARNING! `manIdlePosition` ini-field is expected to be `ScalarIniField` instead of `%s`",
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final ScalarIniField arrayField = (ScalarIniField) field;
        final FieldType arrayFieldType = arrayField.getType();
        if (arrayFieldType != FieldType.FLOAT) {
            callbacks.logLine(String.format(
                "WARNING! `manIdlePosition` ini-field is expected to be `FLOAT` instead of `%s`",
                arrayFieldType
            ));
            return Optional.empty();
        }
        return Optional.of(arrayField);
    }

    private static Optional<ArrayIniField> getValidatedCltIdleCorrIniField(
        final IniField field,
        final UpdateOperationCallbacks callbacks
    ) {
        if (!(field instanceof ArrayIniField)) {
            callbacks.logLine(String.format(
                "WARNING! `cltIdleCorr` ini-field is expected to be `ArrayIniField` instead of `%s`",
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final ArrayIniField arrayField = (ArrayIniField) field;
        final FieldType arrayFieldType = arrayField.getType();
        if (arrayFieldType != FieldType.FLOAT) {
            callbacks.logLine(String.format(
                "WARNING! `cltIdleCorr` ini-field is expected to be `FLOAT` instead of `%s`",
                arrayFieldType
            ));
            return Optional.empty();
        }
        final int arrayFieldRows = arrayField.getRows();
        if (arrayFieldRows != 16) {
            callbacks.logLine(String.format(
                "WARNING! `cltIdleCorr` ini-field is expected to contain 16 rows instead of %d",
                arrayFieldRows
            ));
            return Optional.empty();
        }
        final int arrayFieldCols = arrayField.getCols();
        if (arrayFieldCols != 1) {
            callbacks.logLine(String.format(
                "WARNING! `cltIdleCorr` ini-field is expected to contain 1 cols instead of %d",
                arrayFieldCols
            ));
            return Optional.empty();
        }
        return Optional.of(arrayField);
    }

    private static void checkIfOverrideCrankingIacSettingHasDisappeared(final TuneMigrationContext context) {
        final IniFileModel prevIniFile = context.getPrevIniFile();
        final IniFileModel updatedIniFile = context.getUpdatedIniFile();
        if (prevIniFile.findIniField(OVERRIDE_CRANKING_IAC_SETTING_FIELD_NAME).isPresent()
            && !updatedIniFile.findIniField(OVERRIDE_CRANKING_IAC_SETTING_FIELD_NAME).isPresent()
        ) {
            final Map<String, Constant> prevTune = context.getPrevTune().getConstantsAsMap();
            final Constant prevOverrideCrankingIacSettingConst = prevTune.get(OVERRIDE_CRANKING_IAC_SETTING_FIELD_NAME);
            if (prevOverrideCrankingIacSettingConst != null) {
                final String prevOverrideCrankingIacSettingValue = prevOverrideCrankingIacSettingConst.getValue();
                if (!("\"true\"".equals(prevOverrideCrankingIacSettingValue))) {
                    context.getCallbacks().logLine(String.format(
                        "WARNING! `%s` ini-field with value `%s` cannot be migrated.",
                        OVERRIDE_CRANKING_IAC_SETTING_FIELD_NAME,
                        prevOverrideCrankingIacSettingValue
                    ));
                }
                context.addMigration(OVERRIDE_CRANKING_IAC_SETTING_FIELD_NAME, null);
            }
        }
    }
}
