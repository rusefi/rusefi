package com.rusefi.maintenance.migration;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.*;
import com.rusefi.CompatibilitySet;
import com.rusefi.core.net.ConnectionAndMeta;
import com.rusefi.io.UpdateOperationCallbacks;

import java.util.Objects;
import java.util.Optional;
import java.util.Set;

import static com.devexperts.logging.Logging.getLogging;
import static javax.management.ObjectName.quote;

public enum DefaultIniFieldMigrator implements IniFieldMigrator {
    INSTANCE;
    private static final Logging log = getLogging(DefaultIniFieldMigrator.class);

    // ini fields to ignore on all boards
    private static final Set<String> INI_FIELDS_TO_IGNORE = CompatibilitySet.of("byFirmwareVersion");

    private static final Set<String> boardSpecificIniFieldsToIgnore = ConnectionAndMeta.getNonMigratableIniFields();

    @Override
    public Optional<String> tryMigrateValue(
        final IniField prevField,
        final IniField newField,
        final String prevValue,
        final UpdateOperationCallbacks callbacks
    ) {
        Optional<String> result = Optional.empty();
        final String prevFieldName = prevField.getName();
        if (!INI_FIELDS_TO_IGNORE.contains(prevFieldName) && !boardSpecificIniFieldsToIgnore.contains(prevFieldName)) {
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
        } else {
            final EnumIniField.EnumKeyValueMap newFieldKeyValues = newField.getEnums();
            if (newFieldKeyValues.indexOf(prevValue) == -1) {
                if (newFieldKeyValues.isBitField()) {
                    final int prevValueOrdinal = prevField.getEnums().indexOf(prevValue);
                    final String migratedValue = newFieldKeyValues.get(prevValueOrdinal);
                    if (migratedValue != null) {
                        result = Optional.of(quote(migratedValue));
                    } else {
                        callbacks.logLine(String.format(
                            "WARNING! Field `%s` cannot be migrated because of internal error: " +
                                "new enum doesn't value with ordinal %d",
                            prevField.getName(),
                            prevValueOrdinal
                        ));
                    }
                } else {
                    callbacks.logLine(String.format(
                        "WARNING! Field `%s` cannot be migrated because value `%s` has disappeared",
                        prevField.getName(),
                        prevValue
                    ));
                }
            } else {
                result = Optional.of(prevValue);
            }
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
