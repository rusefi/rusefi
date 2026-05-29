package com.rusefi.maintenance.migration.target_afr_blends_scale_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ArrayFieldScaleMigrator;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import jakarta.xml.bind.JAXBException;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;

public class TargetAfrBlendsScaleMigratorTest {

    @Test
    void checkTargetAfrBlendsScaleMigration() throws JAXBException {
        final TestTuneMigrationContext testContext = TargetAfrBlendsScaleMigrationContext.load();
        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
        final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

        // Verify targetAfrBlends1_table migration (values multiplied by 10)
        {
            final Constant migratedTable = migratedConstants.get("targetAfrBlends1_table");
            assertNotNull(migratedTable);
            // Check that the migration changed the values (original 0.0-7.0 should become 0.00-70.00)
            String value = migratedTable.getValue();
            assertTrue(value.contains("10.00") || value.contains("20.00") || value.contains("30.00"),
                "Table values should be multiplied by 10, got: " + value);
        }

        // Verify targetAfrBlends2_table migration
        {
            final Constant migratedTable = migratedConstants.get("targetAfrBlends2_table");
            assertNotNull(migratedTable);
            String value = migratedTable.getValue();
            assertTrue(value.contains("10.00") || value.contains("20.00") || value.contains("30.00"),
                "Table values should be multiplied by 10, got: " + value);
        }
    }

    @Test
    void checkArrayFieldScaleMigratorDirectly() throws JAXBException {
        final TestTuneMigrationContext testContext = TargetAfrBlendsScaleMigrationContext.load();
        ArrayFieldScaleMigrator.INSTANCE.migrateTune(testContext);
        final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

        // Verify that ArrayFieldScaleMigrator handles the migration directly
        assertNotNull(migratedConstants.get("targetAfrBlends1_table"));
        assertNotNull(migratedConstants.get("targetAfrBlends2_table"));
    }
}
