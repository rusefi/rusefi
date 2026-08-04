package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.IniField;
import com.rusefi.config.FieldType;
import com.rusefi.io.UpdateOperationCallbacks;
import org.jetbrains.annotations.NotNull;

import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.Arrays;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

class BinsIniFieldMigratorStrategy {
    private static final int BINS_INI_FIELD_COLS = 1;

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
            prevCount,
            callbacks
        );
        final Optional<ArrayIniField> newValidatedBinsIniField = getValidatedBinsArrayIniField(
            newField,
            newCount,
            callbacks
        );
        if (prevValidatedBinsIniField.isPresent() && newValidatedBinsIniField.isPresent()) {
            final ArrayIniField prevBinsField = prevValidatedBinsIniField.get();
            final ArrayIniField newBinsField = newValidatedBinsIniField.get();
            if (prevBinsField.getType() != newBinsField.getType()) {
                callbacks.logLine(String.format(
                    "WARNING! Type of `%s` ini-field is changed: `%s` -> `%s`",
                    iniFieldName,
                    prevBinsField.getType(),
                    newBinsField.getType()
                ));
                return Optional.empty();
            }
            if (Double.compare(prevBinsField.getMultiplier(), newBinsField.getMultiplier()) != 0) {
                callbacks.logLine(String.format(
                    "WARNING! Multiplier of `%s` ini-field is changed: `%s` -> `%s`",
                    iniFieldName,
                    prevBinsField.getMultiplier(),
                    newBinsField.getMultiplier()
                ));
                return Optional.empty();
            }

            final int binsToAddCount = newBinsField.getRows() - prevBinsField.getRows();
            if (0 < binsToAddCount) {
                final List<String> prevValues = Arrays.stream(prevBinsField.getValues(prevValue))
                    .map(e -> e[0])
                    .collect(Collectors.toList());
                try {
                    final BigDecimal quantum = getQuantum(newBinsField);
                    final List<Long> prevRawValues = prevValues.stream()
                        .map(BigDecimal::new)
                        .map(value -> value.divide(quantum, 0, RoundingMode.HALF_UP).longValueExact())
                        .collect(Collectors.toList());
                    final long lastValue = prevRawValues.get(prevRawValues.size() - 1);
                    final String max = newBinsField.getMax();
                    final Optional<Long> maximumValue = getMaximumValue(newBinsField, quantum);
                    final long recommendedStep;
                    if (maximumValue.isPresent()) {
                        final long maxPossibleStep = (maximumValue.get() - lastValue) / binsToAddCount;
                        if (1 <= maxPossibleStep) {
                            recommendedStep = chooseStep(prevRawValues, maxPossibleStep);
                        } else {
                            callbacks.logLine(String.format(
                                "WARNING! `%s` ini-field cannot be propagated with increasing values, because max value is %s",
                                iniFieldName,
                                max == null ? newBinsField.getType() + " storage limit" : max
                            ));
                            return Optional.empty();
                        }
                    } else if (prevRawValues.size() > 1) {
                        recommendedStep = lastValue - prevRawValues.get(prevRawValues.size() - 2);
                    } else {
                        callbacks.logLine(String.format(
                            "WARNING! `%s` ini-field cannot be propagated without a maximum or two previous values",
                            iniFieldName
                        ));
                        return Optional.empty();
                    }

                    if (recommendedStep <= 0) {
                        callbacks.logLine(String.format(
                            "WARNING! `%s` ini-field cannot be propagated with increasing values",
                            iniFieldName
                        ));
                        return Optional.empty();
                    }

                    final String[][] newValues = expandValues(
                        prevValues,
                        lastValue,
                        recommendedStep,
                        quantum,
                        Math.max(1, (int)IniField.parseDouble(newBinsField.getDigits()))
                    );
                    result = Optional.of(newBinsField.formatValue(newValues));
                } catch (ArithmeticException | NumberFormatException e) {
                    callbacks.logLine(String.format(
                        "WARNING! `%s` ini-field contains an invalid numeric value",
                        iniFieldName
                    ));
                }
            }
        }
        return result;
    }

    private BigDecimal getQuantum(final ArrayIniField field) {
        if (field.getType() == FieldType.FLOAT) {
            return BigDecimal.ONE.scaleByPowerOfTen(-(int)IniField.parseDouble(field.getDigits()));
        }

        return BigDecimal.valueOf(Math.abs(field.getMultiplier()));
    }

    private Optional<Long> getMaximumValue(final ArrayIniField field, final BigDecimal quantum) {
        final String max = field.getMax();
        final Long storageMax = getStorageMax(field.getType());
        if (max == null) {
            return Optional.ofNullable(storageMax);
        }

        final long configuredMax = BigDecimal.valueOf(IniField.parseDouble(max))
            .divide(quantum, 0, RoundingMode.FLOOR)
            .longValueExact();
        return Optional.of(storageMax == null ? configuredMax : Math.min(configuredMax, storageMax));
    }

    private Long getStorageMax(final FieldType type) {
        switch (type) {
            case UINT8:
                return 0xFFL;
            case UINT16:
                return 0xFFFFL;
            case INT8:
                return (long)Byte.MAX_VALUE;
            case INT16:
                return (long)Short.MAX_VALUE;
            case INT:
                return (long)Integer.MAX_VALUE;
            default:
                return null;
        }
    }

    private String[] @NotNull [] expandValues(
        final List<String> prevValues,
        final long lastValue,
        final long recommendedStep,
        final BigDecimal quantum,
        final int digits
    ) {
        final String[][] newValues = new String[newCount][1];
        // copy prev values:
        for (int i = 0; i < prevCount; i++) {
            newValues[i] = new String[] { prevValues.get(i) };
        }
        long lastBin = lastValue;
        // add missed bins with recommended step:
        for (int i = prevCount; i < newCount; i++) {
            lastBin = Math.addExact(lastBin, recommendedStep);
            newValues[i] = new String[] {
                quantum.multiply(BigDecimal.valueOf(lastBin)).setScale(digits, RoundingMode.HALF_UP).toPlainString()
            };
        }
        return newValues;
    }


    private Optional<ArrayIniField> getValidatedBinsArrayIniField(
        final IniField field,
        final int expectedRows,
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
        if (!arrayFieldType.isNumeric()) {
            callbacks.logLine(String.format(
                "WARNING! Type of `%s` ini-field is expected to be numeric instead of `%s`",
                iniFieldName,
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
        final int arrayFieldRows = arrayField.getRows();
        if (arrayFieldRows == expectedRows) {
            return Optional.of(arrayField);
        } else {
            callbacks.logLine(String.format(
                "WARNING! `%s` ini-field is expected to contain %d rows instead of %d",
                iniFieldName,
                expectedRows,
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
