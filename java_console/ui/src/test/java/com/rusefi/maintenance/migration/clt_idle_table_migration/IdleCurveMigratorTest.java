package com.rusefi.maintenance.migration.clt_idle_table_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.maintenance.migration.migrators.IdleCurveMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;
import java.util.Map;

import static org.junit.jupiter.api.Assertions.*;

public class IdleCurveMigratorTest {

	@Test
	void checkIdleCurveMigrations() throws JAXBException {
		final TestTuneMigrationContext testContext = IdleCurveMigratorContext.load();
		ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
		final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

		final Constant migratedPpsExpAverageName = migratedConstants.get("cltIdleCorrTable");
		assertNotNull(migratedPpsExpAverageName);
		assertEquals(
				"73.0 73.0 63.0 63.0 58.0 58.0 48.0 48.0\n" + "         73.0 73.0 63.0 63.0 58.0 58.0 48.0 48.0\n"
						+ "         73.0 73.0 63.0 63.0 58.0 58.0 48.0 48.0\n"
						+ "         73.0 73.0 63.0 63.0 58.0 58.0 48.0 48.0\n"
						+ "         73.0 73.0 63.0 63.0 58.0 58.0 48.0 48.0\n"
						+ "         73.0 73.0 63.0 63.0 58.0 58.0 48.0 48.0\n"
						+ "         73.0 73.0 63.0 63.0 58.0 58.0 48.0 48.0\n"
						+ "         73.0 73.0 63.0 63.0 58.0 58.0 48.0 48.0",
				migratedPpsExpAverageName.getValue().trim());

	}

	@Test
	void testDownsampleCurveToTable_from16to8() {
		String[][] prevValues = { { "100.0" }, { "95.0" }, { "90.0" }, { "85.0" }, { "80.0" }, { "75.0" }, { "70.0" },
				{ "65.0" }, { "60.0" }, { "55.0" }, { "50.0" }, { "45.0" }, { "40.0" }, { "35.0" }, { "30.0" },
				{ "25.0" } };

		// Downsample from 16 to 8 values with 3 rows
		String[][] result = IdleCurveMigrator.downsampleCurveToTable(prevValues, 16, 8, 3);

		// Verify dimensions
		assertEquals(3, result.length, "Should have 3 rows");
		assertEquals(8, result[0].length, "Should have 8 columns");

		// With 16 values downsampled to 8:
		// start: 8/3 = 2 values (indices 0-1)
		// end: 8/3 = 2 values (indices 14-15)
		// middle: 8 - 2 - 2 = 4 values (starting at index (16-4)/2 = 6, so indices 6-9)
		String[] expectedFirstRow = { "100.0", "95.0", "70.0", "65.0", "60.0", "55.0", "30.0", "25.0" };

		// Check first row values
		assertArrayEquals(expectedFirstRow, result[0], "First row should match expected values");

		// Verify replication across columns
		for (int row = 1; row < 3; row++) {
			assertArrayEquals(result[0], result[row], "Row " + row + " should be identical to row 0");
		}
	}

	@Test
	void testUpsampleCurveToTable_from4to8() {
		String[][] prevValues = { { "100.0" }, { "80.0" }, { "60.0" }, { "40.0" } };

		String[][] result = IdleCurveMigrator.upsampleCurveToTable(prevValues, 4, 8, 3, "2" // 2 decimal places
		);

		// Verify dimensions
		assertEquals(3, result.length, "Should have 3 rows");
		assertEquals(8, result[0].length, "Should have 8 columns");

		// Check first and last values (should match originals)
		assertEquals("100.00", result[0][0]);
		assertEquals("40.00", result[0][7]);

		// Check that intermediate values are interpolated (not equal to originals)
		assertNotEquals("100.00", result[0][1]);
		assertNotEquals("80.00", result[0][2]);

		// Verify values are in descending order
		for (int i = 0; i < 7; i++) {
			double current = Double.parseDouble(result[0][i]);
			double next = Double.parseDouble(result[0][i + 1]);
			assertTrue(current >= next, "Values should be in descending order: " + current + " >= " + next);
		}

		// Verify replication across columns
		for (int row = 1; row < 3; row++) {
			assertArrayEquals(result[0], result[row]);
		}
	}

	@Test
	void testFormatValue() {
		assertEquals("3.14", IdleCurveMigrator.formatArrayValue(3.14159, "2"));
		assertEquals("3.1", IdleCurveMigrator.formatArrayValue(3.14159, "1"));
		assertEquals("3", IdleCurveMigrator.formatArrayValue(3.14159, "0"));
		assertEquals("3.142", IdleCurveMigrator.formatArrayValue(3.14159, "3"));
		assertEquals("100.00", IdleCurveMigrator.formatArrayValue(100.0, "2"));
	}

	@Test
	void testUpsampleEdgeCase_sameSize() {
		// When upsampling to the same size, values should be identical
		String[][] prevValues = { { "10.0" }, { "20.0" }, { "30.0" } };

		String[][] result = IdleCurveMigrator.upsampleCurveToTable(prevValues, 3, 3, 2, "1");

		assertEquals("10.0", result[0][0]);
		assertEquals("20.0", result[0][1]);
		assertEquals("30.0", result[0][2]);
	}
}
