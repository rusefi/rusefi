package com.rusefi.maintenance.migration.afr_msq_import_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;

import jakarta.xml.bind.JAXBException;


public class AfrMsqImportMigrationContext {

    public static TestTuneMigrationContext loadAfrToLambda() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/afr_msq_import_migration/test_data/AFRtoLambda"
        );
        return result;
    }
    public static TestTuneMigrationContext loadLambdaToAfr() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/afr_msq_import_migration/test_data/LambdaToAFR"
        );
        return result;
    }
    public static TestTuneMigrationContext loadSmallAfrToBigLambda() throws JAXBException {
            final TestTuneMigrationContext result = TestTuneMigrationContext.load(
                "src/test/java/com/rusefi/maintenance/migration/afr_msq_import_migration/test_data/SmallAfrToBigLambda"
            );
            return result;
        }
}

