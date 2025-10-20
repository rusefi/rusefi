package com.rusefi.maintenance.migration.migrators;

import com.rusefi.maintenance.migration.TuneMigrationContext;

import java.util.Optional;

public interface TableValuesConverter {
    Optional<String[][]> convertTableValues(String[][] prevValues, TuneMigrationContext context);
}
