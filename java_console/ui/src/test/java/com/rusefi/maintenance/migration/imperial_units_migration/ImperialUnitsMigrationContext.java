package com.rusefi.maintenance.migration.imperial_units_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;

import jakarta.xml.bind.JAXBException;

import static com.rusefi.maintenance.migration.migrators.ImperialUnitsMigrator.*;
import static org.junit.jupiter.api.Assertions.*;

public class ImperialUnitsMigrationContext {
    private static final String PREV_VEHICLE_NAME_VALUE = "\"Test Imperial Vehicle\"";
    private static final String PREV_LAUNCH_CONTROL_VALUE = "\"disabled\"";

    public static TestTuneMigrationContext load() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/imperial_units_migration/test_data"
        );

        // Verify updated tune HAS useMetricOnInterface field with default "Imperial" value
        assertNotNull(result.getUpdatedValue(USE_METRIC_FIELD_NAME));
        assertEquals("\"Imperial\"", result.getUpdatedValue(USE_METRIC_FIELD_NAME).getValue());

        // Verify other fields exist as expected
        assertEquals(PREV_VEHICLE_NAME_VALUE, result.getPrevValue("vehicleName").getValue());
        assertEquals(PREV_LAUNCH_CONTROL_VALUE, result.getPrevValue("launchControlEnabled").getValue());

        return result;
    }
}

