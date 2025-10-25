package com.rusefi.maintenance.migration.migrators;

import com.devexperts.logging.Logging;
import com.opensr5.ini.field.*;
import com.rusefi.io.UpdateOperationCallbacks;

import java.util.Objects;
import java.util.Optional;

import static com.devexperts.logging.Logging.getLogging;
import static com.rusefi.maintenance.migration.IniFieldMigrationUtils.*;

enum DefaultIniFieldMigrationStrategy {
    INSTANCE;
    private static final Logging log = getLogging(DefaultIniFieldMigrationStrategy.class);

    Optional<String> tryMigrateValue(
        final IniField prevField,
        final IniField newField,
        final String prevValue,
        final UpdateOperationCallbacks callbacks
    ) {
        Optional<String> result = Optional.empty();
        final String prevFieldName = prevField.getName();
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
                result = DefaultEnumIniFieldMigrator.INSTANCE.tryMigrateValue(
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
        return result;
    }

    private static boolean canScalarValueBeMigrated(
        final ScalarIniField prevField,
        final ScalarIniField newField,
        final UpdateOperationCallbacks callbacks
    ) {
        boolean result = false;
        if (!checkIfUnitsCanBeMigrated(prevField.getUnits(), newField.getUnits())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be migrated because units are updated: `%s` -> `%s`",
                prevField.getName(),
                prevField,
                newField
            ));
        } else if (!checkIfTypeCanBeMigrated(prevField.getType(), newField.getType())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be migrated because type is updated: `%s` -> `%s`",
                prevField.getName(),
                prevField,
                newField
            ));
        } else {
            result = true;
        }
        return result;
    }

    private static boolean canArrayValueBeMigrated(
        final ArrayIniField prevField,
        final ArrayIniField newField,
        final UpdateOperationCallbacks callbacks
    ) {
        boolean result = false;
        if (!checkIfTypeCanBeMigrated(prevField.getType(), newField.getType())) {
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
        } else {
            result = true;
        }
        return result;
    }
}
