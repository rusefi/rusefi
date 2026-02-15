package com.rusefi.maintenance.migration.scalar_to_array_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;

import jakarta.xml.bind.JAXBException;


public class ScalarToArrayMigrationContext {

    public static TestTuneMigrationContext loadContext() throws JAXBException {
            final TestTuneMigrationContext result = TestTuneMigrationContext.load(
                "src/test/java/com/rusefi/maintenance/migration/scalar_to_array_migration/test_data"
            );
            return result;
        }
}
