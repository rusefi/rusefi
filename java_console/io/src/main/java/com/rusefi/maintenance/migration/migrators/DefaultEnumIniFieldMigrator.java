package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.EnumIniField;
import com.rusefi.io.UpdateOperationCallbacks;

import java.util.Optional;

import static com.rusefi.maintenance.migration.IniFieldMigrationUtils.checkIfTypeCanBeMigrated;
import static javax.management.ObjectName.quote;
import static javax.management.ObjectName.unquote;

public enum DefaultEnumIniFieldMigrator {
    INSTANCE;

    public Optional<String> tryMigrateValue(
        final EnumIniField prevField,
        final EnumIniField newField,
        final String prevValue,
        final UpdateOperationCallbacks callbacks
    ) {
        Optional<String> result = Optional.empty();
        if (EnumIniField.isUnknownOrdinalValue(prevValue)) {
            callbacks.logLine(String.format(
                "Skipped `%s`: this tune has an unknown pin. Use the original tune with the INI used to save it, "
                    + "or choose a supported pin.",
                prevField.getName()
            ));
        } else if (!checkIfTypeCanBeMigrated(prevField.getType(), newField.getType())) {
            callbacks.logLine(String.format(
                "WARNING! Field `%s` cannot be migrated because type is updated: `%s` -> `%s`",
                prevField.getName(),
                prevField.getType(),
                newField.getType()
            ));
        } else {
            final EnumIniField.EnumKeyValueMap newFieldKeyValues = newField.getEnums();
            final int matchingOrdinal = newFieldKeyValues.indexOf(prevValue);
            if (matchingOrdinal != -1) {
                final String label = EnumIniField.isQuoted(prevValue) ? unquote(prevValue) : prevValue;
                final long matchingLabels = newFieldKeyValues.values().stream()
                    .filter(label::equals)
                    .count();
                if (matchingLabels == 1) {
                    result = Optional.of(prevValue);
                } else {
                    callbacks.logLine(String.format(
                        "WARNING! Field `%s` cannot be migrated because value `%s` is ambiguous in the new enum",
                        prevField.getName(),
                        prevValue
                    ));
                }
            } else if (newFieldKeyValues.isBitField()) {
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
        }
        return result;
    }
}
