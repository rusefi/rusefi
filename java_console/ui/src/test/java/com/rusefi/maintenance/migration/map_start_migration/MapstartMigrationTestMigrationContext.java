package com.rusefi.maintenance.migration.map_start_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;

import jakarta.xml.bind.JAXBException;

public class MapstartMigrationTestMigrationContext {

    public static TestTuneMigrationContext loadValidMigration() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/map_start_migration/test_data/valid_migration"
        );
        return result;
    }

    public static TestTuneMigrationContext loadInvalidMigration() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/map_start_migration/test_data/invalid_migration"
        );
        return result;
    }
}

