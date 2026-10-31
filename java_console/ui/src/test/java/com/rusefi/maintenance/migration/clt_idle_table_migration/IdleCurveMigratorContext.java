package com.rusefi.maintenance.migration.clt_idle_table_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;

import jakarta.xml.bind.JAXBException;

import java.io.FileNotFoundException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

public class IdleCurveMigratorContext {

    public static TestTuneMigrationContext loadReducedCltBins() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/clt_idle_table_migration/test_data/reducedCltBins"
        );
        return result;
    }
     public static TestTuneMigrationContext loadIncreasedCltBins() throws JAXBException {
            final TestTuneMigrationContext result = TestTuneMigrationContext.load(
                "src/test/java/com/rusefi/maintenance/migration/clt_idle_table_migration/test_data/increasedCltBins"
            );
            return result;
        }
         public static TestTuneMigrationContext loadSameSizeCltBins() throws JAXBException {
                final TestTuneMigrationContext result = TestTuneMigrationContext.load(
                    "src/test/java/com/rusefi/maintenance/migration/clt_idle_table_migration/test_data/sameSizeCltBins"
                );
                return result;
            }
}

