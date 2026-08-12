package com.rusefi.maintenance.migration.default_migration;

import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import java.util.Map;

import static org.junit.jupiter.api.Assertions.assertEquals;

public final class CalibrationsTestHelpers {
    private CalibrationsTestHelpers() {}

    public static void checkField(
            final TestTuneMigrationContext testContext,
            final CalibrationsInfo mergedCalibrations,
            final String fieldName,
            final String expectedPrevValue,
            final String expectedUpdatedValue,
            final String expectedMergedValue
    ) {
        assertEquals(
                expectedPrevValue,
                testContext.getPrevValue(fieldName).getValue(),
                String.format("Unexpected prev `%s` field value", fieldName)
        );
        assertEquals(
                expectedUpdatedValue,
                testContext.getUpdatedValue(fieldName).getValue(),
                String.format("Unexpected updated `%s` field value", fieldName)
        );
        final Map<String, Constant> mergedConstants = mergedCalibrations.generateMsq().getConstantsAsMap();
        assertEquals(
                expectedMergedValue,
                mergedConstants.get(fieldName).getValue(),
                String.format("Unexpected merged `%s` field value", fieldName)
        );
    }

    public static void checkField(
            final TestTuneMigrationContext testContext,
            final CalibrationsInfo mergedCalibrations,
            final String fieldName,
            final String expectedPrevValue,
            final String expectedUpdatedValue
    ) {
        checkField(testContext, mergedCalibrations, fieldName, expectedPrevValue, expectedUpdatedValue, expectedPrevValue);
    }
}
