package com.rusefi.maintenance.migration.digit_field_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;

import jakarta.xml.bind.JAXBException;

import java.io.FileNotFoundException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

public class DigitIniFieldMigrationContext {
    public static final String PPS_EXP_AVERAGE_ALPHA_FIELD_NAME = "ppsExpAverageAlpha";


    public static TestTuneMigrationContext load() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/digit_field_migration/test_data"
        );

        assertEquals(
            "1.0",
            result.getPrevValue(PPS_EXP_AVERAGE_ALPHA_FIELD_NAME).getValue()
        );
        assertEquals(
            "100.0",
            result.getUpdatedValue(PPS_EXP_AVERAGE_ALPHA_FIELD_NAME).getValue()
        );
        return result;
    }
}

