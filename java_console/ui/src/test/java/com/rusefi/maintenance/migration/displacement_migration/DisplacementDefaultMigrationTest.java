package com.rusefi.maintenance.migration.displacement_migration;

import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import static com.rusefi.maintenance.migration.TestTuneMigrationContextFactory.createTestMigrationContext;
import static com.rusefi.maintenance.migration.migrators.DisplacementIniFieldMigrator.CUBIC_INCHES_UNITS;
import static com.rusefi.maintenance.migration.migrators.DisplacementIniFieldMigrator.DISPLACEMENT_FIELD_NAME;
import static org.junit.jupiter.api.Assertions.*;

public class DisplacementDefaultMigrationTest {
    private static final String TEST_PREV_DISPLACEMENT = "117.0";
    private static final String TEST_NEW_DISPLACEMENT = "121.0";
    private static final String TEST_PREV_DIGITS = "1";
    private static final String TEST_NEW_DIGITS = "0";

    @Test
    void testMigrationWithModifiedDigits() {
        final TestTuneMigrationContext testContext = createTestMigrationContext(
            new Constant(DISPLACEMENT_FIELD_NAME, CUBIC_INCHES_UNITS, TEST_PREV_DISPLACEMENT, TEST_PREV_DIGITS),
            new ScalarIniField(
                DISPLACEMENT_FIELD_NAME,
                412,
                CUBIC_INCHES_UNITS,
                FieldType.FLOAT,
                61.0236100347,
                "1",
                0
            ),
            new Constant(DISPLACEMENT_FIELD_NAME, CUBIC_INCHES_UNITS, TEST_NEW_DISPLACEMENT, TEST_NEW_DIGITS),
            new ScalarIniField(
                DISPLACEMENT_FIELD_NAME,
                432,
                CUBIC_INCHES_UNITS,
                FieldType.FLOAT,
                61.0236100347,
                "0",
                0
            )
        );

        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);

        assertEquals(
            new Constant(DISPLACEMENT_FIELD_NAME, CUBIC_INCHES_UNITS, TEST_PREV_DISPLACEMENT, TEST_NEW_DIGITS),
            testContext.getMigratedValue(DISPLACEMENT_FIELD_NAME)
        );
        assertEquals("", testContext.getTestCallbacks().getContent());
    }
}
