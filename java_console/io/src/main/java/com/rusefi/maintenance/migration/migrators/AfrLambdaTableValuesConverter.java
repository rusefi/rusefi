package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.maintenance.migration.TuneMigrationContext;
import com.rusefi.tune.xml.Constant;
import com.rusefi.tune.xml.Msq;

import java.util.Locale;
import java.util.Objects;
import java.util.Optional;

import static com.rusefi.maintenance.migration.migrators.TableAddColumnsMigrator.*;

public enum AfrLambdaTableValuesConverter implements TableValuesConverter {
    INSTANCE;

    private static final String AFR_UNITS = "afr";
    private static final String LAMBDA_UNITS = "lambda";

    private static final String STOICH_RATIO_PRIMARY_FIELD_NAME = "stoichRatioPrimary";

    @Override
    public Optional<String[][]> convertTableValues(final String[][] prevValues, final TuneMigrationContext context) {
        final Msq prevTune = context.getPrevTune();
        final Constant prevLambdaTableValue = prevTune.getConstantsAsMap().get(LAMBDA_TABLE_FIELD_NAME);
        final Constant updatedLambdaTableValue = context.getUpdatedTune().getConstantsAsMap().get(
            LAMBDA_TABLE_FIELD_NAME
        );
        if ((prevLambdaTableValue != null) && (updatedLambdaTableValue != null)) {
            final String prevLambdaTableValueUnits = prevLambdaTableValue.getUnits();
            final String updatedLambdaTableValueUnits = updatedLambdaTableValue.getUnits();
            if (Objects.equals(prevLambdaTableValueUnits, updatedLambdaTableValueUnits)) {
                return Optional.of(prevValues);
            } else {
                final Optional<ArrayIniField> prevLambdaTableIniField = getPrevLambdaTableIniField(context);
                if (prevLambdaTableIniField.isPresent()) {
                    final Optional<Double> prevStoichRatioPrimary = getPrevStoichRatioPrimary(context);
                    if (prevStoichRatioPrimary.isPresent()) {
                        final int prevValuesRowCount = prevValues.length;
                        final int prevValuesColumnCount = prevValues[0].length;
                        final String[][] convertedValues = new String[prevValuesRowCount][prevValuesColumnCount];
                        for (int i = 0; i < prevValuesRowCount; i++) {
                            for (int j = 0; j < prevValuesColumnCount; j++) {
                                final String prevValue = prevValues[i][j];
                                final Optional<String> convertedValue = convertLambdaTableElementValue(
                                    prevValue,
                                    prevLambdaTableValueUnits,
                                    updatedLambdaTableValueUnits,
                                    updatedLambdaTableValue.getDigits(),
                                    prevStoichRatioPrimary.get(),
                                    context
                                );
                                if (convertedValue.isPresent()) {
                                    convertedValues[i][j] = convertedValue.get();
                                } else {
                                    return Optional.empty();
                                }
                            }
                        }
                        return Optional.of(convertedValues);
                    }
                }
            }
        }
        return Optional.empty();
    }

    private Optional<String> convertLambdaTableElementValue(
        final String prevElementValue,
        final String prevUnits,
        final String updatedUnits,
        final String updatedDigits,
        final double prevStoichRatioPrimary,
        final TuneMigrationContext context
    ) {
        Optional<Double> result = Optional.empty();
        try {
            final double prevElement = Double.parseDouble(prevElementValue);
            if (AFR_UNITS.equalsIgnoreCase(prevUnits) && (LAMBDA_UNITS.equalsIgnoreCase(updatedUnits))) {
                if (prevStoichRatioPrimary != 0.0d) {
                    result = Optional.of(prevElement / prevStoichRatioPrimary);
                } else {
                    context.logWarn(String.format(
                        "We cannot migrate values of `%s` table when units are changed `%s` -> `%s` because of zero " +
                            "`%s` prev value",
                        LAMBDA_TABLE_FIELD_NAME,
                        prevUnits,
                        updatedUnits,
                        STOICH_RATIO_PRIMARY_FIELD_NAME
                    ));
                }
            } else if (LAMBDA_UNITS.equalsIgnoreCase(prevUnits) && AFR_UNITS.equalsIgnoreCase(updatedUnits)) {
                result = Optional.of(prevElement * prevStoichRatioPrimary);
            } else {
                context.logWarn(String.format(
                    "We cannot migrate values of `%s` table when units are changed `%s` -> `%s`",
                    LAMBDA_TABLE_FIELD_NAME,
                    prevUnits,
                    updatedUnits
                ));
            }
        } catch (final NumberFormatException e) {
            context.logWarn(String.format(
                "Invalid element in previous `%s` table field value: `%s`.",
                LAMBDA_TABLE_FIELD_NAME,
                prevElementValue
            ));
        } catch (final NullPointerException e) {
            context.logWarn(String.format(
                "Null element in previous `%s` table field value.",
                LAMBDA_TABLE_FIELD_NAME
            ));
        }
        return result.map(value -> String.format(Locale.US, "%." + updatedDigits + "f", value));
    }

    private Optional<Double> getPrevStoichRatioPrimary(final TuneMigrationContext context) {
        Optional<Double> result = Optional.empty();
        final Constant prevStoichRatioPrimaryConstant = context.getPrevTune().getConstantsAsMap().get(
            STOICH_RATIO_PRIMARY_FIELD_NAME
        );
        if (prevStoichRatioPrimaryConstant != null) {
            final String prevStoichRatioPrimaryValue = prevStoichRatioPrimaryConstant.getValue();
            try {
                result = Optional.of(Double.parseDouble(prevStoichRatioPrimaryValue));
            } catch (final NumberFormatException e) {
                context.logWarn(String.format(
                    "Invalid  previous `%s` field value: `%s`.",
                    STOICH_RATIO_PRIMARY_FIELD_NAME,
                    prevStoichRatioPrimaryValue
                ));
            } catch (final NullPointerException e) {
                context.logWarn(String.format(
                    "To migrate changed `%s` units previous tune should contain non-null `%s` field value.",
                    LAMBDA_RPM_BINS_FIELD_NAME,
                    STOICH_RATIO_PRIMARY_FIELD_NAME
                ));
            }
        } else {
            context.logWarn(String.format(
                "To migrate changed `%s` units prev tune should contain `%s` field value.",
                LAMBDA_RPM_BINS_FIELD_NAME,
                STOICH_RATIO_PRIMARY_FIELD_NAME
            ));
        }
        return result;
    }

    private Optional<ArrayIniField> getPrevLambdaTableIniField(final TuneMigrationContext context) {
        Optional<ArrayIniField> result = Optional.empty();
        final Optional<IniField> optionalPrevIniField = context.getPrevIniFile().findIniField(LAMBDA_TABLE_FIELD_NAME);
        if (optionalPrevIniField.isPresent()) {
            final IniField prevIniField = optionalPrevIniField.get();
            if (prevIniField instanceof ArrayIniField) {
                result = Optional.of((ArrayIniField) prevIniField);
            } else {
                context.logWarn(String.format(
                    "`%s` prev ini-field is expected to be `ArrayIniField` instead of `%s`",
                    LAMBDA_TABLE_FIELD_NAME,
                    prevIniField.getClass().getSimpleName()
                ));
            }
        } else {
            context.logWarn(String.format("`%s` ini-field is missed in prev ini-file", LAMBDA_TABLE_FIELD_NAME));
        }
        return result;
    }
}
