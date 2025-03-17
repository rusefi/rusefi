package com.rusefi.maintenance.migration;

import com.opensr5.ini.field.IniField;
import com.rusefi.io.UpdateOperationCallbacks;

import java.util.Arrays;
import java.util.List;
import java.util.Optional;

public enum ComposedIniFieldMigrator implements IniFieldMigrator {
    INSTANCE;

    private final List<IniFieldMigrator> iniFieldMigrators = Arrays.asList(
        DefaultIniFieldMigrator.INSTANCE
    );

    @Override
    public Optional<String> tryMigrateValue(
        final IniField prevField,
        final IniField newField,
        final String prevValue,
        final UpdateOperationCallbacks callbacks
    ) {
        Optional<String> result = Optional.empty();
        for (final IniFieldMigrator migrator: iniFieldMigrators) {
            result = migrator.tryMigrateValue(prevField, newField, prevValue, callbacks);
            if (result.isPresent()) {
                break;
            }
        }
        return result;
    }
}
