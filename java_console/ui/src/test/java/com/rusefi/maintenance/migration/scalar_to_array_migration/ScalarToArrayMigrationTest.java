package com.rusefi.maintenance.migration.scalar_to_array_migration;

import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;

import org.junit.jupiter.api.Test;

import jakarta.xml.bind.JAXBException;
import java.util.Map;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static java.util.Collections.emptySet;

public class ScalarToArrayMigrationTest {

	@Test
	void checkScalarMigration() throws JAXBException {
		final TestTuneMigrationContext testContext = ScalarToArrayMigrationContext.loadContext();
		ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
		final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

		final Constant migratedScalarConstant = migratedConstants.get("afterCrankingIACtaperDuration");
		final String expectedMigratedTable = "200.0\n"
				+ "         200.0\n"
				+ "         200.0\n"
				+ "         200.0\n"
				+ "         200.0\n"
				+ "         200.0";

		assertNotNull(migratedScalarConstant);
		assertEquals(expectedMigratedTable, migratedScalarConstant.getValue().trim());

		final Optional<CalibrationsInfo> result = CalibrationsHelper.mergeCalibrations(testContext.getPrevIniFile(),
				testContext.getPrevTune(), testContext.getUpdatedCalibrationsInfo(), testContext.getCallbacks(),
				emptySet());
		assertTrue(result.isPresent());
	}
}
