package com.rusefi.maintenance.migration.boolean_field_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.maintenance.migration.default_migration.DefaultTestTuneMigrationContext;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import jakarta.xml.bind.JAXBException;
import java.util.Map;

import static com.rusefi.maintenance.migration.migrators.BooleanIniFieldMigrator.*;
import static org.junit.jupiter.api.Assertions.*;
import static javax.management.ObjectName.quote;

public class BooleanIniFieldMigratorTest {

    @Test
    void checkBooleanMigrations() throws JAXBException {
        final TestTuneMigrationContext testContext = DefaultTestTuneMigrationContext.load();
        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
        final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

        // replicate this code block for all the migrated fields on BooleanIniFieldMigrator
        {
            final Constant migratedLaunchControlFieldName = migratedConstants.get(LAUNCH_CONTROL_FIELD_NAME);
            assertNotNull(migratedLaunchControlFieldName);
            assertEquals(quote("disabled"), migratedLaunchControlFieldName.getValue());
        }
    }
}

