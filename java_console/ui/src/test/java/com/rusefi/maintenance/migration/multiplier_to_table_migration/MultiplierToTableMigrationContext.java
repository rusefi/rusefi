package com.rusefi.maintenance.migration.multiplier_to_table_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;

import javax.xml.bind.JAXBException;


public class MultiplierToTableMigrationContext {

    public static TestTuneMigrationContext loadSameSize() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/multiplier_to_table_migration/test_data/same_size"
        );
        return result;
    }

    public static TestTuneMigrationContext loadCrankingFuelMultiplier() throws JAXBException{
        return TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/multiplier_to_table_migration/test_data/cranking_fuel_multiplier"
        );    }
}
