package com.rusefi.maintenance.migration.imperial_units_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;

import javax.xml.bind.JAXBException;

import static com.rusefi.maintenance.migration.migrators.ImperialUnitsMigrator.*;
import static org.junit.jupiter.api.Assertions.*;

public class ImperialUnitsMigrationContextWithExistingField {
    private static final String PREV_VEHICLE_NAME_VALUE = "\"Test Vehicle With Metric\"";
    private static final String PREV_USE_METRIC_VALUE = "\"Imperial\"";

    public static TestTuneMigrationContext load() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/imperial_units_migration/test_data_with_existing_field"
        );

        // Verify prev tune ALREADY HAS useMetricOnInterface field (user preference exists)
        assertNotNull(result.getPrevValue(USE_METRIC_FIELD_NAME));
        assertEquals(PREV_USE_METRIC_VALUE, result.getPrevValue(USE_METRIC_FIELD_NAME).getValue());

        // Verify updated tune also has the field
        assertNotNull(result.getUpdatedValue(USE_METRIC_FIELD_NAME));
        assertEquals(PREV_USE_METRIC_VALUE, result.getUpdatedValue(USE_METRIC_FIELD_NAME).getValue());

        // Verify other fields exist as expected
        assertEquals(PREV_VEHICLE_NAME_VALUE, result.getPrevValue("vehicleName").getValue());

        return result;
    }
}
