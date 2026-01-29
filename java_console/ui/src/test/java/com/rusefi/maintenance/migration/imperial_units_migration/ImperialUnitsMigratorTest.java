package com.rusefi.maintenance.migration.imperial_units_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;
import java.util.Map;

import static com.rusefi.maintenance.migration.migrators.ImperialUnitsMigrator.*;
import static org.junit.jupiter.api.Assertions.*;

public class ImperialUnitsMigratorTest {

    @Test
    void checkImperialUnitsMigration() throws JAXBException {
        final TestTuneMigrationContext testContext = ImperialUnitsMigrationContext.load();
        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
        final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

        final Constant migratedUseMetric = migratedConstants.get(USE_METRIC_FIELD_NAME);
        assertNotNull(migratedUseMetric);
        assertEquals(USE_METRIC_FIELD_VALUE, migratedUseMetric.getValue());
    }

    @Test
    void checkNoMigrationWhenFieldExistsInPrevTune() throws JAXBException {
        final TestTuneMigrationContext testContext = ImperialUnitsMigrationContextWithExistingField.load();
        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
        final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

        // Should not migrate when field already exists in prev tune
        assertNull(migratedConstants.get(USE_METRIC_FIELD_NAME));
    }
}
