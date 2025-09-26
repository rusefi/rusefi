package com.rusefi.maintenance.migration;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.FieldType;
import com.rusefi.io.UpdateOperationCallbacks;
import org.jetbrains.annotations.NotNull;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

import static com.rusefi.maintenance.migration.VeTableExtensionMigrator.*;

class VeRpmBinsIniFieldMigrator {
    private static final int BINS_INI_FIELD_COLS = 1;
    private static final double BINS_INI_FIELD_MULTIPLIER = 1;
    private static final String BINS_INI_FIELD_DIGITS = "0";
    private static final FieldType BINS_INI_FIELD_TYPE = FieldType.UINT16;

    Optional<String> tryMigrateVeRpmBins(
        final IniField prevField,
        final IniField newField,
        final String prevValue,
        final UpdateOperationCallbacks callbacks
    ) {
        Optional<String> result = Optional.empty();
        final Optional<ArrayIniField> prevVeRpmBinsValidatedField = getValidatedVeRpmBinsArrayIniField(
            prevField,
            callbacks
        );
        final Optional<ArrayIniField> newVeRpmBinsValidatedField = getValidatedVeRpmBinsArrayIniField(
            newField,
            callbacks
        );
        if (prevVeRpmBinsValidatedField.isPresent() && newVeRpmBinsValidatedField.isPresent()) {
            final ArrayIniField prevVeRpmBinsField = prevVeRpmBinsValidatedField.get();
            final ArrayIniField newVeRpmBinsField = newVeRpmBinsValidatedField.get();
            final int prevVeRpmBinsFieldRows = prevVeRpmBinsField.getRows();
            final int newVeRpmBinsFieldRows = newVeRpmBinsField.getRows();
            if ((prevVeRpmBinsFieldRows == OLD_VE_TABLE_COLS) && (newVeRpmBinsFieldRows == NEW_VE_TABLE_COLS)) {
                final List<String> prevValues = Arrays.stream(prevVeRpmBinsField.getValues(prevValue))
                    .map(e -> e[0])
                    .collect(Collectors.toList());
                final List<Long> prevLongValues = prevValues.stream()
                    .map(Double::parseDouble)
                    .map(Math::round)
                    .collect(Collectors.toList());
                final long lastValue = prevLongValues.get(prevLongValues.size() - 1);
                final String max = newVeRpmBinsField.getMax();

                Optional<Long> recommendedStep;
                if (max != null) {
                    final long maxPossibleStep = (long)(
                        (Double.parseDouble(max) - lastValue) / (NEW_VE_TABLE_COLS - OLD_VE_TABLE_COLS)
                    );
                    if (1 <= maxPossibleStep) {
                        recommendedStep = Optional.of(chooseStep(prevLongValues, maxPossibleStep));
                    } else {
                        callbacks.logLine(String.format(
                            "WARNING! `veRpmBins` ini-field cannot be propagated with increasing values, because max value is %s",
                            max
                        ));
                        return Optional.empty();
                    }
                } else {
                    final long valueBeforeLast = prevLongValues.get(prevLongValues.size() - 2);
                    recommendedStep = Optional.of(lastValue - valueBeforeLast);
                }
                if (recommendedStep.isPresent()) {
                    final String[][] newValues = expandValues(prevValues, lastValue, recommendedStep);
                    result = Optional.of(newVeRpmBinsField.formatValue(newValues));
                }
            }
        }
        return result;
    }

    private static String[] @NotNull [] expandValues(List<String> prevValues, long lastValue, Optional<Long> recommendedStep) {
        final String[][] newValues = new String[NEW_VE_TABLE_COLS][1];
        // copy prev values:
        for (int i = 0; i < OLD_VE_TABLE_COLS; i++) {
            newValues[i] = new String[] { prevValues.get(i) };
        }
        long lastBin = lastValue;
        // add missed bins with recommended step:
        for (int i = OLD_VE_TABLE_COLS; i < NEW_VE_TABLE_COLS; i++) {
            lastBin += recommendedStep.get();
            newValues[i] = new String[] { String.format("%.1f", (double)lastBin) };
        }
        return newValues;
    }


    private static Optional<ArrayIniField> getValidatedVeRpmBinsArrayIniField(
        final IniField field,
        final UpdateOperationCallbacks callbacks
    ) {
        if (!(field instanceof ArrayIniField)) {
            callbacks.logLine(String.format(
                "WARNING! `veRpmBins` ini-field is expected to be `ArrayIniField` instead of %s",
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final ArrayIniField arrayField = (ArrayIniField) field;
        final FieldType arrayFieldType = arrayField.getType();
        if (arrayFieldType != BINS_INI_FIELD_TYPE) {
            callbacks.logLine(String.format(
                "WARNING! Type of `veRpmBins` ini-field is expected to be `%s` instead of `%s`",
                BINS_INI_FIELD_TYPE,
                arrayFieldType
            ));
            return Optional.empty();
        }
        final int arrayFieldCols = arrayField.getCols();
        if (arrayFieldCols != BINS_INI_FIELD_COLS) {
            callbacks.logLine(String.format(
                "WARNING! `veRpmBins` ini-field is expected to contain %d columns instead of %d",
                VE_TABLE_ROWS,
                arrayFieldCols
            ));
            return Optional.empty();
        }
        final double arrayFieldMultiplier = arrayField.getMultiplier();
        if (arrayFieldMultiplier != BINS_INI_FIELD_MULTIPLIER) {
            callbacks.logLine(String.format(
                "WARNING! Multiplier of `veRpmBins` ini-field is expected to be %f instead of %f",
                BINS_INI_FIELD_MULTIPLIER,
                arrayFieldMultiplier
            ));
            return Optional.empty();
        }
        final String arrayFieldDigits = arrayField.getDigits();
        if (!BINS_INI_FIELD_DIGITS.equals(arrayFieldDigits)) {
            callbacks.logLine(String.format(
                "WARNING! Digits of `veRpmBins` ini-field is expected to be `%s` instead of `%s`",
                BINS_INI_FIELD_DIGITS,
                arrayFieldDigits
            ));
            return Optional.empty();
        }
        final int arrayFieldRows = arrayField.getRows();
        switch (arrayFieldRows) {
            case OLD_VE_TABLE_COLS:
            case NEW_VE_TABLE_COLS: {
                return Optional.of(arrayField);
            }
            default: {
                callbacks.logLine(String.format(
                    "WARNING! `veRpmBins` ini-field is expected to contain %d or %d rows " +
                        "instead of %d",
                    OLD_VE_TABLE_COLS,
                    NEW_VE_TABLE_COLS,
                    arrayFieldRows
                ));
                return Optional.empty();
            }
        }
    }

    private static long chooseStep(final List<Long> bins, final long maxPossibleStep) {
        Long lastBin = null;
        Long lastStep = null;
        Long maxStep = null;
        for (final long bin: bins) {
            if (lastBin != null) {
                lastStep = bin - lastBin;
                if (lastStep <= maxPossibleStep) {
                    if ((maxStep == null) || (maxStep < lastStep)) {
                        maxStep = lastStep;
                    }
                }
            }
            lastBin = bin;
        }
        if (lastStep != null) {
            if (lastStep <= maxPossibleStep) {
                return lastStep;
            } else if (maxStep != null) {
                return maxStep;
            }
        }
        return maxPossibleStep;
    }
}
