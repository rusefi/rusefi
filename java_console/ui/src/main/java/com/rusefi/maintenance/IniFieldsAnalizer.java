package com.rusefi.maintenance;

import com.devexperts.logging.Logging;
import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.*;
import com.rusefi.core.Pair;
import com.rusefi.tune.xml.Constant;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Objects;

import static com.devexperts.logging.Logging.getLogging;

public class IniFieldsAnalizer {
    private static final Logging log = getLogging(IniFieldsAnalizer.class);

    public static List<Pair<IniField, Constant>> findValuesToUpdate(
        final IniFileModel prevIni,
        final Map<String, Constant> prevValues,
        final IniFileModel newIni,
        final Map<String, Constant> newValues
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
                            log.warn(String.format(
                                "Field `%s` cannot be updated because its name is updated: %s -> %s",
                                prevFieldName,
                                prevValue,
                                newValue
                            ));
                        } else if (!Objects.equals(prevValue.getUnits(), newValue.getUnits())) {
                            log.warn(String.format(
                                "Field `%s` cannot be updated because its units are updated: %s -> %s",
                                prevFieldName,
                                prevValue,
                                newValue
                            ));
                        } else if (!Objects.equals(prevValue.getDigits(), newValue.getDigits())) {
                            log.warn(String.format(
                                "Field `%s` cannot be updated because its digits are updated: %s -> %s",
                                prevFieldName,
                                prevValue,
                                newValue
                            ));
                        } else if (!Objects.equals(prevValue.getCols(), newValue.getCols())) {
                            log.warn(String.format(
                                "Field `%s` cannot be updated because its column count is updated: %s -> %s",
                                prevFieldName,
                                prevValue,
                                newValue
                            ));
                        } else if (!Objects.equals(prevValue.getRows(), newValue.getRows())) {
                            log.warn(String.format(
                                "Field `%s` cannot be updated because its column count is updated: %s -> %s",
                                prevFieldName,
                                prevValue,
                                newValue
                            ));
                        } else if (canValueBeMigrated(prevFieldEntry.getValue(), newField)) {
                            log.info(String.format(
                                "Field `%s` will be updated: %s -> %s",
                                prevFieldName,
                                prevValue,
                                newValue
                            ));
                            result.add(new Pair<>(newField, prevValue));
                        } else {
                            log.warn(String.format(
                                "Field `%s` cannot be updated: %s -> %s",
                                prevFieldName,
                                prevFieldEntry.getValue(),
                                newField
                            ));
                        }
                    }
                } else {
                    log.info(String.format("Field `%s` is missed in new .ini file", prevFieldName));
                }
            }
        }
        return result;
    }

    private static boolean canValueBeMigrated(final IniField prevField, final IniField newField) {
        boolean result = false;
        final String prevFieldName = prevField.getName();
        if (prevField instanceof ScalarIniField) {
            if (newField instanceof ScalarIniField) {
                result = canScalarValueBeMigrated((ScalarIniField) prevField, (ScalarIniField) newField);
            } else {
                log.warn(String.format(
                    "Field `%s` cannot be migrated because it is no is no longer scalar in new .ini file: %s -> %s",
                    prevFieldName,
                    prevField,
                    newField
                ));
            }
        } else if (prevField instanceof StringIniField) {
            if (newField instanceof StringIniField) {
                result = true;
            } else {
                log.warn(String.format(
                    "Field `%s` cannot be migrated because it is no longer string in new .ini file: %s -> %s",
                    prevFieldName,
                    prevField,
                    newField
                ));
            }
        } else if (prevField instanceof EnumIniField) {
            if (newField instanceof EnumIniField) {
                result = canEnumValueBeMigrated((EnumIniField) prevField, (EnumIniField) newField);
            } else {
                log.warn(String.format(
                    "Field `%s` cannot be migrated because it is no longer enum in new .ini file: %s -> %s",
                    prevFieldName,
                    prevField,
                    newField
                ));
            }
        } else if (prevField instanceof ArrayIniField) {
            if (newField instanceof ArrayIniField) {
                result = canArrayValueBeMigrated((ArrayIniField) prevField, (ArrayIniField) newField);
            } else {
                log.warn(String.format(
                    "Field `%s` cannot be migrated because it is no longer array in new .ini file: %s -> %s",
                    prevFieldName,
                    prevField,
                    newField
                ));
            }
        } else {
            log.error(String.format("Unexpected field type: %s", prevField.getClass()));
        }
        return result;
    }

    private static boolean canScalarValueBeMigrated(final ScalarIniField prevField, final ScalarIniField newField) {
        boolean result = false;
        if (!Objects.equals(prevField.getUnits(), newField.getUnits())) {
            log.warn(String.format(
                "Field cannot be migrated because units are updated: %s -> %s",
                prevField,
                newField
            ));
        } else if (!Objects.equals(prevField.getType(), newField.getType())) {
            log.warn(String.format("Field cannot be migrated because type is updated: %s -> %s", prevField, newField));
        } else if (!Objects.equals(prevField.getDigits(), newField.getDigits())) {
            log.warn(String.format(
                "Field cannot be migrated because digits are updated: %s -> %s",
                prevField,
                newField
            ));
        } else {
            result = true;
        }
        return result;
    }

    private static boolean canEnumValueBeMigrated(final EnumIniField prevField, final EnumIniField newField) {
        boolean result = false;
        //TODO: Should we check if `enums` map is updated?
        if (!Objects.equals(prevField.getType(), newField.getType())) {
            log.warn(String.format("Field cannot be migrated because type is updated: %s -> %s", prevField, newField));
        } else if (!Objects.equals(prevField.getBitPosition(), newField.getBitPosition())) {
            log.warn(String.format(
                "Field cannot be migrated because bit position is updated: %s -> %s",
                prevField,
                newField
            ));
        } else if (!Objects.equals(prevField.getBitSize0(), newField.getBitSize0())) {
            log.warn(String.format(
                "Field cannot be migrated because bit size 0 is updated: %s -> %s",
                prevField,
                newField
            ));
        } else {
            result = true;
        }
        return result;
    }

    private static boolean canArrayValueBeMigrated(final ArrayIniField prevField, final ArrayIniField newField) {
        boolean result = false;
        if (!Objects.equals(prevField.getType(), newField.getType())) {
            log.warn(String.format("Field cannot be migrated because type is updated: %s -> %s", prevField, newField));
        } else if (!Objects.equals(prevField.getCols(), newField.getCols())) {
            log.warn(String.format(
                "Field cannot be migrated because column count is updated: %s -> %s",
                prevField,
                newField
            ));
        } else if (!Objects.equals(prevField.getRows(), newField.getRows())) {
            log.warn(String.format(
                "Field cannot be migrated because row count is updated: %s -> %s",
                prevField,
                newField
            ));
        } else if (!Objects.equals(prevField.getDigits(), newField.getDigits())) {
            log.warn(String.format(
                "Field cannot be migrated because digits are updated: %s -> %s",
                prevField,
                newField
            ));
        } else {
            result = true;
        }
        return result;
    }
}
