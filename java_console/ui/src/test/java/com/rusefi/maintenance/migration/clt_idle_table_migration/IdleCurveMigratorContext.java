package com.rusefi.maintenance.migration.clt_idle_table_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;

import javax.xml.bind.JAXBException;

import java.io.FileNotFoundException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

public class IdleCurveMigratorContext {

    public static TestTuneMigrationContext load() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/clt_idle_table_migration/test_data"
        );
        return result;
    }
}
