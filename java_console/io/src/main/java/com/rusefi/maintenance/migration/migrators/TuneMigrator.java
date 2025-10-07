package com.rusefi.maintenance.migration.migrators;

import com.rusefi.maintenance.migration.TuneMigrationContext;

public interface TuneMigrator {
    void migrateTune(TuneMigrationContext context);
}
