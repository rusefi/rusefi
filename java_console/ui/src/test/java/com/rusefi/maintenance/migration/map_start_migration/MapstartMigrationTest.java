package com.rusefi.maintenance.migration.map_start_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import jakarta.xml.bind.JAXBException;
import java.util.Map;

import static com.rusefi.maintenance.migration.migrators.TableAddColumnsMigrator.MAP_SAMPLING_FIELD_NAME;
import static org.junit.jupiter.api.Assertions.*;

public class MapstartMigrationTest {

    @Test
    void checkValidMigration() throws JAXBException {
        final TestTuneMigrationContext testContext = MapstartMigrationTestMigrationContext.loadValidMigration();
        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
        final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

        assertEquals(1, migratedConstants.size());

        final Constant migratedMapStartAngle = migratedConstants.get(
            MAP_SAMPLING_FIELD_NAME
        );
        assertNotNull(migratedMapStartAngle);

        assertEquals(
            "\n" +
                "         55.00\n" +
                "         59.00\n" +
                "         64.00\n" +
                "         68.00\n" +
                "         72.00\n" +
                "         76.00\n" +
                "         81.00\n" +
                "         85.00\n",
            migratedMapStartAngle.getValue()
        );
    }

    @Test
    void checkInvalidMigration() throws JAXBException{
        final TestTuneMigrationContext testContext = MapstartMigrationTestMigrationContext.loadInvalidMigration();
        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
        final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

        assertEquals(0, migratedConstants.size());
    }
}

