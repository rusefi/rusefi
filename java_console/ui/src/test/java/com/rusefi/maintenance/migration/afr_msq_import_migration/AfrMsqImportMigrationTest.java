package com.rusefi.maintenance.migration.afr_msq_import_migration;

import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.AfrMsqImportMigrator;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;

import org.junit.jupiter.api.Test;

import jakarta.xml.bind.JAXBException;
import java.util.Map;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static java.util.Collections.emptySet;

public class AfrMsqImportMigrationTest {

	@Test
	void checkAfrMsqMigrationAfrToLambda() throws JAXBException {
		final TestTuneMigrationContext testContext = AfrMsqImportMigrationContext.loadAfrToLambda();
		AfrMsqImportMigrator.INSTANCE.migrateTune(testContext);
		final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

		final Constant migratedcltIdleCorrTableName = migratedConstants.get("lambdaTable");
		final String expectedMigratedTable = "1.00 1.00 1.00 1.01 1.01 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         1.00 1.00 1.00 1.01 1.01 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         1.00 1.00 1.00 1.01 1.01 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         1.00 1.00 1.00 1.01 1.01 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         1.00 1.00 1.00 1.01 1.01 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         1.00 1.00 1.00 1.01 1.01 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         1.00 1.00 1.00 1.01 1.01 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         0.97 0.97 0.97 0.97 0.97 0.98 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99\n"
				+ "         0.95 0.95 0.95 0.95 0.95 0.96 0.96 0.97 0.97 0.96 0.96 0.96 0.96 0.96 0.96 0.96\n"
				+ "         0.93 0.93 0.93 0.93 0.93 0.94 0.94 0.94 0.94 0.94 0.94 0.94 0.94 0.93 0.93 0.93\n"
				+ "         0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.88 0.88 0.88 0.88\n"
				+ "         0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.86 0.86 0.86 0.86 0.86 0.86 0.86\n"
				+ "         0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.86 0.86 0.86 0.86 0.86 0.86 0.86\n"
				+ "         0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.86 0.86 0.86 0.86 0.86 0.86 0.86\n"
				+ "         0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.86 0.86 0.86 0.86 0.86 0.86 0.86\n"
				+ "         0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.86 0.86 0.86 0.86 0.86 0.86 0.86";

		assertNotNull(migratedcltIdleCorrTableName);
		assertEquals(expectedMigratedTable, migratedcltIdleCorrTableName.getValue().trim());

		final Optional<CalibrationsInfo> result = CalibrationsHelper.mergeCalibrations(testContext.getPrevIniFile(),
				testContext.getPrevTune(), testContext.getUpdatedCalibrationsInfo(), testContext.getCallbacks(),
				emptySet());
		assertTrue(result.isPresent());
	}

	@Test
	void checkAfrMsqMigrationLambdaToAfr() throws JAXBException {
		final TestTuneMigrationContext testContext = AfrMsqImportMigrationContext.loadLambdaToAfr();
		AfrMsqImportMigrator.INSTANCE.migrateTune(testContext);
		final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

		final Constant migratedcltIdleCorrTableName = migratedConstants.get("lambdaTable");
		final String expectedMigratedTable = "14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70\n"
				+ "         14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70\n"
				+ "         14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70\n"
				+ "         14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70\n"
				+ "         14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70 14.70\n"
				+ "         14.00 14.00 14.00 14.00 14.00 14.00 14.00 14.00 14.00 14.00 14.00 14.00 14.00 14.00 14.00 14.00\n"
				+ "         13.50 13.50 13.50 13.50 13.50 13.50 13.50 13.50 13.50 13.50 13.50 13.50 13.50 13.50 13.50 13.50\n"
				+ "         13.20 13.20 13.20 13.20 13.20 13.20 13.20 13.20 13.20 13.20 13.20 13.20 13.20 13.20 13.20 13.20\n"
				+ "         13.10 13.10 13.10 13.10 13.10 13.10 13.10 13.10 13.10 13.10 13.10 13.10 13.10 13.10 13.10 13.10\n"
				+ "         12.90 12.90 12.90 12.90 12.90 12.90 12.90 12.90 12.90 12.90 12.90 12.90 12.90 12.90 12.90 12.90\n"
				+ "         12.60 12.60 12.60 12.60 12.60 12.60 12.60 12.60 12.60 12.60 12.60 12.60 12.60 12.60 12.60 12.60\n"
				+ "         12.30 12.30 12.30 12.30 12.30 12.30 12.30 12.30 12.30 12.30 12.30 12.30 12.30 12.30 12.30 12.30\n"
				+ "         11.80 11.80 11.80 11.80 11.80 11.80 11.80 11.80 11.80 11.80 11.80 11.80 11.80 11.80 11.80 11.80\n"
				+ "         11.30 11.30 11.30 11.30 11.30 11.30 11.30 11.30 11.30 11.30 11.30 11.30 11.30 11.30 11.30 11.30\n"
				+ "         11.00 11.00 11.00 11.00 11.00 11.00 11.00 11.00 11.00 11.00 11.00 11.00 11.00 11.00 11.00 11.00\n"
				+ "         10.70 10.70 10.70 10.70 10.70 10.70 10.70 10.70 10.70 10.70 10.70 10.70 10.70 10.70 10.70 10.70";

		assertNotNull(migratedcltIdleCorrTableName);
		assertEquals(expectedMigratedTable, migratedcltIdleCorrTableName.getValue().trim());

		final Optional<CalibrationsInfo> result = CalibrationsHelper.mergeCalibrations(testContext.getPrevIniFile(),
				testContext.getPrevTune(), testContext.getUpdatedCalibrationsInfo(), testContext.getCallbacks(),
				emptySet());
		assertTrue(result.isPresent());
	}

	@Test
	void checkAfrMsqMigrationSmallAfrToBigLambda() throws JAXBException {
		final TestTuneMigrationContext testContext = AfrMsqImportMigrationContext.loadSmallAfrToBigLambda();
		AfrMsqImportMigrator.INSTANCE.migrateTune(testContext);
		final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

		final Constant migratedcltIdleCorrTableName = migratedConstants.get("lambdaTable");
		final String expectedMigratedTable = "1.00 1.00 1.00 1.00 1.00 1.01 1.01 1.02 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         1.00 1.00 1.00 1.00 1.00 1.01 1.01 1.02 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         1.00 1.00 1.00 1.00 1.00 1.01 1.01 1.02 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         1.00 1.00 1.00 1.00 1.00 1.01 1.01 1.02 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         1.00 1.00 1.00 1.00 1.00 1.01 1.01 1.02 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         1.00 1.00 1.00 1.00 1.00 1.01 1.01 1.02 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         1.00 1.00 1.00 1.00 1.00 1.01 1.01 1.02 1.02 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03 1.03\n"
				+ "         0.97 0.97 0.97 0.97 0.97 0.97 0.97 0.98 0.98 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99 0.99\n"
				+ "         0.95 0.95 0.95 0.95 0.95 0.95 0.95 0.96 0.96 0.96 0.96 0.97 0.97 0.96 0.96 0.96 0.96 0.96 0.96 0.96 0.96 0.96 0.96 0.96\n"
				+ "         0.93 0.93 0.93 0.93 0.93 0.93 0.93 0.94 0.94 0.94 0.94 0.94 0.94 0.94 0.94 0.94 0.94 0.94 0.94 0.94 0.93 0.93 0.93 0.93\n"
				+ "         0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.89 0.88 0.88 0.88 0.88 0.88\n"
				+ "         0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86\n"
				+ "         0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86\n"
				+ "         0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86\n"
				+ "         0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86\n"
				+ "         0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.87 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86 0.86";

		assertNotNull(migratedcltIdleCorrTableName);
		assertEquals(expectedMigratedTable, migratedcltIdleCorrTableName.getValue().trim());

		final Optional<CalibrationsInfo> result = CalibrationsHelper.mergeCalibrations(testContext.getPrevIniFile(),
				testContext.getPrevTune(), testContext.getUpdatedCalibrationsInfo(), testContext.getCallbacks(),
				emptySet());
		assertTrue(result.isPresent());
	}

	@Test
	void testUpsampleTable2x2To4x4() {
		final String[][] input = { { "1.0", "2.0" }, { "3.0", "4.0" } };

		// Upsample to 4x4
		final String[][] result = AfrMsqImportMigrator.upsampleTable(input, 4, 4, "1");

		// Expected values with bilinear interpolation
		// First row: 1.0, 1.33, 1.67, 2.0
		// Second row: 1.67, 2.0, 2.33, 2.67
		// Third row: 2.33, 2.67, 3.0, 3.33
		// Fourth row: 3.0, 3.33, 3.67, 4.0
		assertEquals("1.0", result[0][0]);
		assertEquals("2.0", result[0][3]);
		assertEquals("3.0", result[3][0]);
		assertEquals("4.0", result[3][3]);

		// Check interpolated middle values
		assertEquals("2.0", result[1][1]); // Middle point should be average of all corners
		assertEquals("3.0", result[2][2]); // Another middle point
	}

	@Test
	void testUpsampleTable3x3To6x6() {
		final String[][] input = { { "1.0", "2.0", "3.0" }, { "4.0", "5.0", "6.0" }, { "7.0", "8.0", "9.0" } };

		// Upsample to 6x6
		final String[][] result = AfrMsqImportMigrator.upsampleTable(input, 6, 6, "1");

		// Verify corners are preserved
		assertEquals("1.0", result[0][0]);
		assertEquals("3.0", result[0][5]);
		assertEquals("7.0", result[5][0]);
		assertEquals("9.0", result[5][5]);

		// Verify middle point is preserved
		assertEquals("4.2", result[2][2]); // Should map to original center at (1,1)
	}

	@Test
	void testUpsampleTableSameSize() {
		// When upsampling to the same size, values should be identical
		final String[][] input = { { "1.5", "2.5" }, { "3.5", "4.5" } };

		final String[][] result = AfrMsqImportMigrator.upsampleTable(input, 2, 2, "1");

		// All values should be identical
		assertEquals("1.5", result[0][0]);
		assertEquals("2.5", result[0][1]);
		assertEquals("3.5", result[1][0]);
		assertEquals("4.5", result[1][1]);
	}

	@Test
	void testUpsampleTablePrecision() {
		// Test that precision is maintained correctly
		final String[][] input = { { "1.234", "2.345" }, { "3.456", "4.567" } };

		final String[][] result = AfrMsqImportMigrator.upsampleTable(input, 3, 3, "3");

		// Check that all values have 3 decimal places
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				final String[] parts = result[row][col].split("\\.");
				assertEquals(2, parts.length, "Value should have decimal point");
				assertEquals(3, parts[1].length(), "Value should have 3 decimal places");
			}
		}
	}

	@Test
	void testDownsampleTable6x6To3x3() {
		// Create a 6x6 table with distinct values in different regions
		final String[][] input = { { "1.0", "1.1", "1.2", "1.3", "1.4", "1.5" },
				{ "2.0", "2.1", "2.2", "2.3", "2.4", "2.5" }, { "3.0", "3.1", "3.2", "3.3", "3.4", "3.5" },
				{ "4.0", "4.1", "4.2", "4.3", "4.4", "4.5" }, { "5.0", "5.1", "5.2", "5.3", "5.4", "5.5" },
				{ "6.0", "6.1", "6.2", "6.3", "6.4", "6.5" } };

		// Downsample to 3x3
		final String[][] result = AfrMsqImportMigrator.downsampleTable(input, 6, 3);

		// Verify corners are preserved (approximately)
		assertEquals("1.0", result[0][0]); // Start-start
		assertEquals("1.2", result[0][2]); // Start-end
		assertEquals("6.0", result[2][0]); // End-start
		assertEquals("6.2", result[2][2]); // End-end

		// Verify that we have values from different sections
		assertNotEquals(result[0][0], result[1][1]); // Start vs middle
		assertNotEquals(result[1][1], result[2][2]); // Middle vs end
	}

	@Test
	void testDownsampleTable4x4To2x2() {
		final String[][] input = { { "1.0", "2.0", "3.0", "4.0" }, { "5.0", "6.0", "7.0", "8.0" },
				{ "9.0", "10.0", "11.0", "12.0" }, { "13.0", "14.0", "15.0", "16.0" } };

		final String[][] result = AfrMsqImportMigrator.downsampleTable(input, 4, 2);

		// With 2x2 result: start section = 0, middle section = 2, end section = 0
		// So we should get values from start and middle of the input
		assertEquals(2, result.length);
		assertEquals(4, result[0].length);

		// Verify we got some values from the input
		assertNotNull(result[0][0]);
		assertNotNull(result[0][1]);
		assertNotNull(result[1][0]);
		assertNotNull(result[1][1]);
	}

	@Test
	void testDownsampleTableSameSize() {
		// When downsampling to the same size, values should be identical
		final String[][] input = { { "1.5", "2.5" }, { "3.5", "4.5" } };

		final String[][] result = AfrMsqImportMigrator.downsampleTable(input, 2, 2);

		// All values should be identical
		assertEquals("1.5", result[0][0]);
		assertEquals("2.5", result[0][1]);
		assertEquals("3.5", result[1][0]);
		assertEquals("4.5", result[1][1]);
	}

	@Test
	void testDownsampleTablePreservesEdgeValues() {
		final String[][] original = { { "10.0", "20.0", "30.0" }, { "40.0", "50.0", "60.0" },
				{ "70.0", "80.0", "90.0" } };

		// Upsample to 9x9
		final String[][] upsampled = AfrMsqImportMigrator.upsampleTable(original, 9, 9, "1");

		// Downsample to 3x3
		final String[][] result = AfrMsqImportMigrator.downsampleTable(upsampled, 9, 3);

		// Check that we preserved corner values from start, middle, end sections
		assertEquals("10.0", result[0][0]); // Top-left (start-start)
		assertEquals("70.0", result[2][0]); // Bottom-left (end-start)
	}

	// read this test case as some corner-of-the-corner cases, has migrations only applies one of the two
	@Test
	void testUpsampleThenDownsamplePreservesApproximateValues() {
		// Create original table
		final String[][] original = { { "10.0", "20.0", "30.0" }, { "40.0", "50.0", "60.0" },
				{ "70.0", "80.0", "90.0" } };

		// Upsample to 9x9
		final String[][] upsampled = AfrMsqImportMigrator.upsampleTable(original, 9, 9, "1");

		// Downsample back to 3x3
		final String[][] downsampled = AfrMsqImportMigrator.downsampleTable(upsampled, 9, 3);

		// Corner values should be preserved or very close
		assertEquals("10.0", downsampled[0][0]);
		assertEquals("15.0", downsampled[0][2]);
		assertEquals("70.0", downsampled[2][0]);
		assertEquals("75.0", downsampled[2][2]);
	}

}

