package com.rusefi.maintenance.migration.cranking_idle_position_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;

import javax.xml.bind.JAXBException;


public class CrankingIdleMigrationContext {

    public static TestTuneMigrationContext loadSameSize() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/cranking_idle_position_migration/test_data/same_size"
        );
        return result;
    }
}
