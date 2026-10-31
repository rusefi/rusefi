package com.rusefi.maintenance;

import com.opensr5.ConfigurationImageWithMeta;
import com.rusefi.binaryprotocol.BinaryProtocol;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import jakarta.xml.bind.JAXBException;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import static com.rusefi.maintenance.migration.default_migration.DefaultTestTuneMigrationContext.*;
import static com.rusefi.maintenance.migration.migrators.TableAddColumnsMigrator.VE_TABLE_FIELD_NAME;
import static java.util.Collections.emptySet;
import static org.junit.jupiter.api.Assertions.*;
import static org.mockito.Mockito.*;

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

    @Test
    public void secondaryPageIsWrittenVerifiedAndBurnedByPageIdentifier() {
        byte[] luaPage = new byte[16];
        Arrays.fill(luaPage, (byte) 42);
        CalibrationsInfo calibrations = new CalibrationsInfo(
            mock(com.opensr5.ini.IniFileModel.class),
            Map.of(
                0, new ConfigurationImageWithMeta(null, new byte[8]),
                0x0400, new ConfigurationImageWithMeta(null, luaPage)
            ),
            Set.of(0x0400)
        );
        BinaryProtocol binaryProtocol = mock(BinaryProtocol.class);
        when(binaryProtocol.readFromPage(0x0400, 0, luaPage.length)).thenReturn(luaPage.clone());
        when(binaryProtocol.burnPage(0x0400)).thenReturn(true);

        assertTrue(CalibrationsUpdater.uploadPages(binaryProtocol, calibrations, new TestCallbacks()));
        verify(binaryProtocol).writeInBlocks(luaPage, 0, 0, luaPage.length, 0x0400);
        verify(binaryProtocol).readFromPage(0x0400, 0, luaPage.length);
        verify(binaryProtocol).burnPage(0x0400);
        verifyNoMoreInteractions(binaryProtocol);
    }
}
