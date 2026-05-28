package com.rusefi.maintenance;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import jakarta.xml.bind.JAXBException;
import java.util.HashSet;
import java.util.Set;

import static com.rusefi.maintenance.migration.default_migration.DefaultTestTuneMigrationContext.*;
import static com.rusefi.maintenance.migration.migrators.TableAddColumnsMigrator.VE_TABLE_FIELD_NAME;
import static java.util.Collections.emptySet;
import static org.junit.jupiter.api.Assertions.*;

public class CalibrationsHelperPartialMergeTest {
    private PartialMergeTestContext testContext;

    @BeforeEach
    public void setUp() throws JAXBException {
        testContext = PartialMergeTestContext.load();
    }

    @Test
    public void testMergeCalibrationsWithPartialFailure_returnsNonNullResult() {
        CalibrationsHelper.MergeResult result = CalibrationsHelper.mergeCalibrationsWithPartialFailure(
            testContext.getPrevIniFile(),
            testContext.getPrevTune(),
            testContext.getUpdatedCalibrationsInfo(),
            testContext.getCallbacks(),
            emptySet()
        );

        assertNotNull(result);
        assertNotNull(result.failedFields);
    }

    @Test
    public void testMergeCalibrationsWithPartialFailure_tracksFailedFields() {
        Set<String> simulatedFailures = new HashSet<>();
        simulatedFailures.add(IGNITION_TABLE_FIELD_NAME);

        CalibrationsHelper.MergeResult result = testContext.mergeWithSimulatedFailures(simulatedFailures);

        assertNotNull(result);
        assertTrue(result.mergedCalibrations.isPresent());
        assertEquals(1, result.failedFields.size());
        assertTrue(result.failedFields.contains(IGNITION_TABLE_FIELD_NAME));

        String logText = testContext.getTestCallbacks().getContent();
        assertTrue(logText.contains("Failed to migrate field `" + IGNITION_TABLE_FIELD_NAME + "`"));
    }

    @Test
    public void testMergeCalibrationsWithPartialFailure_multipleFieldsFail() {
        Set<String> simulatedFailures = new HashSet<>();
        simulatedFailures.add(IGNITION_TABLE_FIELD_NAME);
        simulatedFailures.add(VE_TABLE_FIELD_NAME);

        CalibrationsHelper.MergeResult result = testContext.mergeWithSimulatedFailures(simulatedFailures);

        assertNotNull(result);
        assertTrue(result.mergedCalibrations.isPresent());
        assertEquals(2, result.failedFields.size());
        assertTrue(result.failedFields.contains(IGNITION_TABLE_FIELD_NAME));
        assertTrue(result.failedFields.contains(VE_TABLE_FIELD_NAME));

        String logText = testContext.getTestCallbacks().getContent();
        assertTrue(logText.contains("Failed to migrate field `" + IGNITION_TABLE_FIELD_NAME + "`"));
        assertTrue(logText.contains("Failed to migrate field `" + VE_TABLE_FIELD_NAME + "`"));
    }
}
