package com.rusefi.maintenance.migration;

import com.opensr5.ini.field.IniField;
import com.rusefi.io.UpdateOperationCallbacks;

import java.util.Optional;

public interface IniFieldMigrator {
    Optional<String> tryMigrateValue(
        IniField prevField,
        IniField newField,
        String prevValue,
        UpdateOperationCallbacks callbacks
    );
}
