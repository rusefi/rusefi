package com.rusefi.maintenance.migration.target_afr_blends_scale_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;

import jakarta.xml.bind.JAXBException;

public class TargetAfrBlendsScaleMigrationContext {
    public static TestTuneMigrationContext load() throws JAXBException {
        return TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/target_afr_blends_scale_migration/test_data"
        );
    }
}
