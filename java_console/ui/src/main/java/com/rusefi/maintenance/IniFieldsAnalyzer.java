package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.*;
import com.rusefi.CompatibilitySet;
import com.rusefi.core.Pair;
import com.rusefi.io.UpdateOperationCallbacks;
import com.rusefi.tune.xml.Constant;

import java.util.*;

import static com.devexperts.logging.Logging.getLogging;

public class IniFieldsAnalyzer {
    private static final Logging log = getLogging(IniFieldsAnalyzer.class);

    private static final Set<String> INI_FIELDS_TO_IGNORE = CompatibilitySet.of(
        "byFirmwareVersion",
        // todo INI_FIELDS_TO_IGNORE should be dynamic https://github.com/rusefi/rusefi/issues/7454
        "hash3",
        // todo: looks like a defect? there is no 'signature' field, do we maybe have a defect in IniFileModel?
        "signature"
    );

    public static List<Pair<IniField, Constant>> findValuesToUpdate(
        final IniFileModel prevIni,
        final Map<String, Constant> prevValues,
        final IniFileModel newIni,
        final Map<String, Constant> newValues,
        final UpdateOperationCallbacks callbacks
    ) {
        final List<Pair<IniField, Constant>> result = new ArrayList<>();
        final Map<String, IniField> prevIniFields = prevIni.getAllIniFields();
        final Map<String, IniField> newIniFields = newIni.getAllIniFields();
        for (final Map.Entry<String, IniField> prevFieldEntry: prevIniFields.entrySet()) {
            final String prevFieldName = prevFieldEntry.getKey();
            final IniField newField = newIniFields.get(prevFieldName);
            if (newField != null) {
                final Constant prevValue = prevValues.get(prevFieldName);
                final Constant newValue = newValues.get(prevFieldName);
                if (prevValue != null && newValue != null) {
                    if (!Objects.equals(prevValue.getValue(), newValue.getValue())) {
                        if (!Objects.equals(prevValue.getName(), newValue.getName())) {
                            callbacks.logLine(String.format(
                                "WARNING! Field `%s` cannot be updated because its name is updated: `%s` -> `%s`",
                                prevFieldName,
                                prevValue.getName(),
                                newValue.getName()
                            ));
                        } else if (!Objects.equals(prevValue.getUnits(), newValue.getUnits())) {
                            callbacks.logLine(String.format(
                                "WARNING! Field `%s` cannot be updated because its units are updated: `%s` -> `%s`",
                                prevFieldName,
                                prevValue.getUnits(),
                                newValue.getUnits()
                            ));
                        } else if (!Objects.equals(prevValue.getDigits(), newValue.getDigits())) {
                            callbacks.logLine(String.format(
                                "WARNING! Field `%s` cannot be updated because its digits are updated: `%s` -> `%s`",
                                prevFieldName,
                                prevValue.getDigits(),
                                newValue.getDigits()
                            ));
                        } else if (!Objects.equals(prevValue.getCols(), newValue.getCols())) {
                            callbacks.logLine(String.format(
                                "WARNING! Field `%s` cannot be updated because its column count is updated: `%s` -> `%s`",
                                prevFieldName,
                                prevValue.getCols(),
                                newValue.getCols()
                            ));
                        } else if (!Objects.equals(prevValue.getRows(), newValue.getRows())) {
                            callbacks.logLine(String.format(
                                "WARNING! Field `%s` cannot be updated because its row count is updated: `%s` -> `%s`",
                                prevFieldName,
                                prevValue.getRows(),
                                newValue.getRows()
                            ));
                        } else {
                            final Optional<String> migratedValue = tryMigrateValue(
                                prevFieldEntry.getValue(),
                                newField,
                                prevValue.getValue(),
                                callbacks
                            );
                            if (migratedValue.isPresent()) {
                                log.info(String.format(
                                    "Field `%s` is going to be restored: `%s` -> `%s`",
                                    prevFieldName,
                                    prevValue.getValue(),
                                    newValue.getValue()
                                ));

                                result.add(new Pair<>(newField, newValue.cloneWithValue(migratedValue.get())));
                            } else {
                                log.warn(String.format(
                                    "Field `%s` cannot be updated: `%s` -> `%s`",
                                    prevFieldName,
                                    prevFieldEntry.getValue(),
                                    newField
                                ));
                            }
                        }
                    }
                } else {
                    log.info(String.format("Field `%s` is missed in new .ini file", prevFieldName));
                }
            }
        }
        return result;
    }

    private static Optional<String> tryMigrateValue(
        final IniField prevField,
        final IniField newField,
        final String prevValue,
        final UpdateOperationCallbacks callbacks
    ) {
        Optional<String> result = Optional.empty();
        final String prevFieldName = prevField.getName();
        if (!INI_FIELDS_TO_IGNORE.contains(prevFieldName)) {
            if (prevField instanceof ScalarIniField) {
                if (newField instanceof ScalarIniField) {
                    if (canScalarValueBeMigrated((ScalarIniField) prevField, (ScalarIniField) newField, callbacks)) {
                        result = Optional.of(prevValue);
                    }
                } else {
                    callbacks.logLine(String.format(
                        "WARNING! Field `%s` cannot be migrated because it is no is no longer scalar in new .ini file",
                        prevFieldName
                    ));
                }
            } else if (prevField instanceof StringIniField) {
                if (newField instanceof StringIniField) {
                    result = Optional.of(prevValue);
                } else {
                    callbacks.logLine(String.format(
                        "WARNING! Field `%s` cannot be migrated because it is no longer string in new .ini file",
                        prevFieldName
                    ));
                }
            } else if (prevField instanceof EnumIniField) {
                if (newField instanceof EnumIniField) {
                    result = tryMigrateEnumValue(
                        (EnumIniField) prevField,
                        (EnumIniField) newField,
                        prevValue,
                        callbacks
                    );
                } else {
                    callbacks.logLine(String.format(
                        "WARNING! `%s` cannot be migrated because it is no longer enum in new .ini file",
                        prevFieldName
                    ));
                }
            } else if (prevField instanceof ArrayIniField) {
                if (newField instanceof ArrayIniField) {
                    if (canArrayValueBeMigrated((ArrayIniField) prevField, (ArrayIniField) newField, callbacks)) {
                        result = Optional.of(prevValue);
                    }
                } else {
                    callbacks.logLine(String.format(
                        "WARNING! Field `%s` cannot be migrated because it is no longer array in new .ini file",
                        prevFieldName
                    ));
                }
            } else {
                log.error(String.format("Unexpected field type: %s", prevField.getClass()));
            }
        }
        return result;
    }

    private static boolean canScalarValueBeMigrated(
        final ScalarIniField prevField,
        final ScalarIniField newField,
        final UpdateOperationCallbacks callbacks
    ) {
        boolean result = false;
        if (!Objects.equals(prevField.getUnits(), newField.getUnits())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be migrated because units are updated: `%s` -> `%s`",
                prevField.getName(),
                prevField,
                newField
            ));
        } else if (!Objects.equals(prevField.getType(), newField.getType())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be migrated because type is updated: `%s` -> `%s`",
                prevField.getName(),
                prevField,
                newField
            ));
        } else if (!Objects.equals(prevField.getDigits(), newField.getDigits())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be migrated because digits are updated: `%s` -> `%s`",
                prevField.getName(),
                prevField,
                newField
            ));
        } else {
            result = true;
        }
        return result;
    }

    private static Optional<String> tryMigrateEnumValue(
        final EnumIniField prevField,
        final EnumIniField newField,
        final String prevValue,
        final UpdateOperationCallbacks callbacks
    ) {
        Optional<String> result = Optional.empty();
        if (!Objects.equals(prevField.getType(), newField.getType())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be migrated because type is updated: `%s` -> `%s`",
                prevField.getName(),
                prevField.getType(),
                newField.getType()
            ));
        } else if (!Objects.equals(prevField.getBitSize0(), newField.getBitSize0())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be migrated because bit size 0 is updated: `%s` -> `%s`",
                prevField.getName(),
                prevField.getBitSize0(),
                newField.getBitSize0()
            ));
        } else if (newField.getEnums().indexOf(prevValue) == -1) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be migrated because value `%s` has disappeared",
                prevField.getName(),
                prevValue
            ));
        } else {
            result = Optional.of(prevValue);
        }
        return result;
    }

    private static boolean canArrayValueBeMigrated(
        final ArrayIniField prevField,
        final ArrayIniField newField,
        final UpdateOperationCallbacks callbacks
    ) {
        boolean result = false;
        if (!Objects.equals(prevField.getType(), newField.getType())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be migrated because type is updated: `%s` -> `%s`",
                prevField.getName(),
                prevField.getType(),
                newField.getType()
            ));
        } else if (!Objects.equals(prevField.getCols(), newField.getCols())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be migrated because column count is updated: `%s` -> `%s`",
                prevField.getName(),
                prevField.getCols(),
                newField.getCols()
            ));
        } else if (!Objects.equals(prevField.getRows(), newField.getRows())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be migrated because row count is updated: `%s` -> `%s`",
                prevField.getName(),
                prevField.getRows(),
                newField.getRows()
            ));
        } else if (!Objects.equals(prevField.getDigits(), newField.getDigits())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be migrated because digits are updated: `%s` -> `%s`",
                prevField.getName(),
                prevField.getDigits(),
                newField.getDigits()
            ));
        } else {
            result = true;
        }
        return result;
    }
}
