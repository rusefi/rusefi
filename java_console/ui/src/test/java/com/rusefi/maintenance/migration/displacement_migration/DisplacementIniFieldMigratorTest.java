package com.rusefi.maintenance.migration.displacement_migration;

import com.opensr5.ini.field.IniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import static com.rusefi.maintenance.migration.TestTuneMigrationContextFactory.createTestMigrationContext;
import static com.rusefi.maintenance.migration.migrators.DisplacementIniFieldMigrator.*;
import static org.junit.jupiter.api.Assertions.*;

public class DisplacementIniFieldMigratorTest {
    private static final IniField TEST_DISPLACEMENT_INI_FIELD_IN_LITERS = new ScalarIniField(
        DISPLACEMENT_FIELD_NAME,
        432,
        L_UNITS,
        FieldType.UINT16,
        0.001,
        "3",
        0
    );
    private static final IniField TEST_DISPLACEMENT_INI_FIELD_IN_CUBIC_INCHES = new ScalarIniField(
        DISPLACEMENT_FIELD_NAME,
        412,
        CUBIC_INCHES_UNITS,
        FieldType.FLOAT,
        61.0236100347,
        "1",
        0
    );

    private static final String TEST_DISPLACEMENT_IN_LITERS = "1.917";
    private static final String DISPLACEMENT_IN_CUBIC_INCHES = "120.9997";

    private static final Constant DISPLACEMENT_VALUE_IN_LITERS = new Constant(
        DISPLACEMENT_FIELD_NAME,
        L_UNITS,
        TEST_DISPLACEMENT_IN_LITERS,
        "3"
    );
    private static final Constant DISPLACEMENT_VALUE_IN_CUBIC_INCHES = new Constant(
        DISPLACEMENT_FIELD_NAME,
        CUBIC_INCHES_UNITS,
        DISPLACEMENT_IN_CUBIC_INCHES,
        "1"
    );

    @Test
    void testMigrationFromLitersToCubicInches() {
        TestTuneMigrationContext testContext = createTestMigrationContext(
            DISPLACEMENT_VALUE_IN_LITERS,
            TEST_DISPLACEMENT_INI_FIELD_IN_LITERS,
            DISPLACEMENT_VALUE_IN_CUBIC_INCHES,
            TEST_DISPLACEMENT_INI_FIELD_IN_CUBIC_INCHES
        );

        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);

        final Constant migratedDisplacementValue = testContext.getMigratedConstants().get(DISPLACEMENT_FIELD_NAME);
        assertNotNull(migratedDisplacementValue);
        assertEquals(
            117.0, // = 1.917 liters in cubic inches
            Double.parseDouble(migratedDisplacementValue.getValue()),
            0.1
        );
        assertEquals(DISPLACEMENT_VALUE_IN_CUBIC_INCHES.getName(), migratedDisplacementValue.getName());
        assertEquals(DISPLACEMENT_VALUE_IN_CUBIC_INCHES.getUnits(), migratedDisplacementValue.getUnits());
        assertEquals(DISPLACEMENT_VALUE_IN_CUBIC_INCHES.getDigits(), migratedDisplacementValue.getDigits());
    }

    @Test
    void testMigrationInLiters() {
        final Constant testUpdatedDisplacementValue = DISPLACEMENT_VALUE_IN_LITERS.cloneWithValue("1.983");

        final TestTuneMigrationContext testContext = createTestMigrationContext(
            DISPLACEMENT_VALUE_IN_LITERS,
            TEST_DISPLACEMENT_INI_FIELD_IN_LITERS,
            testUpdatedDisplacementValue,
            TEST_DISPLACEMENT_INI_FIELD_IN_LITERS
        );

        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);

        final Constant migratedDisplacementValue = testContext.getMigratedConstants().get(DISPLACEMENT_FIELD_NAME);
        assertNotNull(migratedDisplacementValue);
        assertEquals(DISPLACEMENT_VALUE_IN_LITERS.getValue(), migratedDisplacementValue.getValue());
        assertEquals(DISPLACEMENT_VALUE_IN_LITERS.getName(), migratedDisplacementValue.getName());
        assertEquals(DISPLACEMENT_VALUE_IN_LITERS.getUnits(), migratedDisplacementValue.getUnits());
        assertEquals(DISPLACEMENT_VALUE_IN_LITERS.getDigits(), migratedDisplacementValue.getDigits());
    }

    @Test
    void testMigrationInCubicInches() {
        final Constant testUpdatedDisplacementValue = DISPLACEMENT_VALUE_IN_CUBIC_INCHES.cloneWithValue("117");

        final TestTuneMigrationContext testContext = createTestMigrationContext(
            DISPLACEMENT_VALUE_IN_CUBIC_INCHES,
            TEST_DISPLACEMENT_INI_FIELD_IN_CUBIC_INCHES,
            testUpdatedDisplacementValue,
            TEST_DISPLACEMENT_INI_FIELD_IN_CUBIC_INCHES
        );

        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);

        final Constant migratedDisplacementValue = testContext.getMigratedConstants().get(DISPLACEMENT_FIELD_NAME);
        assertNotNull(migratedDisplacementValue);
        assertEquals(DISPLACEMENT_VALUE_IN_CUBIC_INCHES.getValue(), migratedDisplacementValue.getValue());
        assertEquals(DISPLACEMENT_VALUE_IN_CUBIC_INCHES.getName(), migratedDisplacementValue.getName());
        assertEquals(DISPLACEMENT_VALUE_IN_CUBIC_INCHES.getUnits(), migratedDisplacementValue.getUnits());
        assertEquals(DISPLACEMENT_VALUE_IN_CUBIC_INCHES.getDigits(), migratedDisplacementValue.getDigits());
    }

    @Test
    void testMigrationWithoutChanges() {
        final TestTuneMigrationContext testContext = createTestMigrationContext(
            DISPLACEMENT_VALUE_IN_LITERS,
            TEST_DISPLACEMENT_INI_FIELD_IN_LITERS,
            DISPLACEMENT_VALUE_IN_LITERS,
            TEST_DISPLACEMENT_INI_FIELD_IN_LITERS
        );

        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);

        final Constant migratedDisplacementValue = testContext.getMigratedConstants().get(DISPLACEMENT_FIELD_NAME);
        assertNull(migratedDisplacementValue);
    }
}
