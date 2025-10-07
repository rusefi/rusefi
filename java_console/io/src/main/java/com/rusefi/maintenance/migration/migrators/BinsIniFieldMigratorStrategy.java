package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.CompatibilitySet;
import com.rusefi.config.FieldType;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.tune.xml.Msq;
import org.jetbrains.annotations.NotNull;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

class BinsIniFieldMigratorStrategy {
    private static final int BINS_INI_FIELD_COLS = 1;
    private static final double BINS_INI_FIELD_MULTIPLIER = 1;
    private static final String BINS_INI_FIELD_DIGITS = "0";
    private static final FieldType BINS_INI_FIELD_TYPE = FieldType.UINT16;

    private final String iniFieldName;
    private final int prevCount;
    private final int newCount;

    BinsIniFieldMigratorStrategy(final String binsIniFieldName, final int prevBinsCount, final int newBinsCount) {
        iniFieldName = binsIniFieldName;
        prevCount = prevBinsCount;
        newCount = newBinsCount;
    }

    Optional<String> tryMigrateBins(
        final IniField prevField,
        final IniField newField,
        final String prevValue,
        final UpdateOperationCallbacks callbacks
    ) {
        Optional<String> result = Optional.empty();
        final Optional<ArrayIniField> prevValidatedBinsIniField = getValidatedBinsArrayIniField(
            prevField,
            callbacks
        );
        final Optional<ArrayIniField> newValidatedBinsIniField = getValidatedBinsArrayIniField(
            newField,
            callbacks
        );
        if (prevValidatedBinsIniField.isPresent() && newValidatedBinsIniField.isPresent()) {
            final ArrayIniField prevBinsField = prevValidatedBinsIniField.get();
            final ArrayIniField newBinsField = newValidatedBinsIniField.get();
            final int binsToAddCount = newBinsField.getRows() - prevBinsField.getRows();
            if (0 < binsToAddCount) {
                final List<String> prevValues = Arrays.stream(prevBinsField.getValues(prevValue))
                    .map(e -> e[0])
                    .collect(Collectors.toList());
                final List<Long> prevLongValues = prevValues.stream()
                    .map(Double::parseDouble)
                    .map(Math::round)
                    .collect(Collectors.toList());
                final long lastValue = prevLongValues.get(prevLongValues.size() - 1);
                final String max = newBinsField.getMax();

                Optional<Long> recommendedStep;
                if (max != null) {
                    final long maxPossibleStep = (long)(
                        (Double.parseDouble(max) - lastValue) / binsToAddCount
                    );
                    if (1 <= maxPossibleStep) {
                        recommendedStep = Optional.of(chooseStep(prevLongValues, maxPossibleStep));
                    } else {
                        callbacks.logLine(String.format(
                            "WARNING! `%s` ini-field cannot be propagated with increasing values, because max value is %s",
                            iniFieldName,
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
                    result = Optional.of(newBinsField.formatValue(newValues));
                }
            }
        }
        return result;
    }

    private String[] @NotNull [] expandValues(List<String> prevValues, long lastValue, Optional<Long> recommendedStep) {
        final String[][] newValues = new String[newCount][1];
        // copy prev values:
        for (int i = 0; i < prevCount; i++) {
            newValues[i] = new String[] { prevValues.get(i) };
        }
        long lastBin = lastValue;
        // add missed bins with recommended step:
        for (int i = prevCount; i < newCount; i++) {
            lastBin += recommendedStep.get();
            newValues[i] = new String[] { String.format(Msq.TS_INTEGRATION_LOCALE, "%.1f", (double)lastBin) };
        }
        return newValues;
    }


    private Optional<ArrayIniField> getValidatedBinsArrayIniField(
        final IniField field,
        final UpdateOperationCallbacks callbacks
    ) {
        if (!(field instanceof ArrayIniField)) {
            callbacks.logLine(String.format(
                "WARNING! `%s` ini-field is expected to be `ArrayIniField` instead of %s",
                iniFieldName,
                field.getClass().getName()
            ));
            return Optional.empty();
        }
        final ArrayIniField arrayField = (ArrayIniField) field;
        final FieldType arrayFieldType = arrayField.getType();
        if (arrayFieldType != BINS_INI_FIELD_TYPE) {
            callbacks.logLine(String.format(
                "WARNING! Type of `%s` ini-field is expected to be `%s` instead of `%s`",
                iniFieldName,
                BINS_INI_FIELD_TYPE,
                arrayFieldType
            ));
            return Optional.empty();
        }
        final int arrayFieldCols = arrayField.getCols();
        if (arrayFieldCols != BINS_INI_FIELD_COLS) {
            callbacks.logLine(String.format(
                "WARNING! `%s` ini-field is expected to contain %d columns instead of %d",
                iniFieldName,
                BINS_INI_FIELD_COLS,
                arrayFieldCols
            ));
            return Optional.empty();
        }
        final double arrayFieldMultiplier = arrayField.getMultiplier();
        if (arrayFieldMultiplier != BINS_INI_FIELD_MULTIPLIER) {
            callbacks.logLine(String.format(
                "WARNING! Multiplier of `%s` ini-field is expected to be %f instead of %f",
                iniFieldName,
                BINS_INI_FIELD_MULTIPLIER,
                arrayFieldMultiplier
            ));
            return Optional.empty();
        }
        final String arrayFieldDigits = arrayField.getDigits();
        if (!BINS_INI_FIELD_DIGITS.equals(arrayFieldDigits)) {
            callbacks.logLine(String.format(
                "WARNING! Digits of `%s` ini-field is expected to be `%s` instead of `%s`",
                iniFieldName,
                BINS_INI_FIELD_DIGITS,
                arrayFieldDigits
            ));
            return Optional.empty();
        }
        final int arrayFieldRows = arrayField.getRows();
        if (CompatibilitySet.of(prevCount, newCount).contains(arrayFieldRows)) {
            return Optional.of(arrayField);
        } else {
            callbacks.logLine(String.format(
                "WARNING! `%s` ini-field is expected to contain %d or %d rows instead of %d",
                iniFieldName,
                prevCount,
                newCount,
                arrayFieldRows
            ));
            return Optional.empty();
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
