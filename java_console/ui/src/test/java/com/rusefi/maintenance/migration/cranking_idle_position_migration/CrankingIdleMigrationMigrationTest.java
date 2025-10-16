package com.rusefi.maintenance.migration.cranking_idle_position_migration;

import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;

import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;
import java.util.Map;
import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;
import static java.util.Collections.emptySet;


public class CrankingIdleMigrationMigrationTest {

	@Test
	void checkCrankingIdleMigrationSameSize() throws JAXBException {
		final TestTuneMigrationContext testContext = CrankingIdleMigrationContext.loadSameSize();
		ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
		final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

		final Constant migratedCltCrankingCorrTableName = migratedConstants.get("cltCrankingCorr");
		final String expectedMigratedTable = "65.00\n"
				+ "         66.00\n"
				+ "         66.00\n"
				+ "         66.00\n"
				+ "         64.50\n"
				+ "         63.00\n"
				+ "         61.00\n"
				+ "         60.50";

		assertNotNull(migratedCltCrankingCorrTableName);
		assertEquals(expectedMigratedTable, migratedCltCrankingCorrTableName.getValue().trim());

		final Optional<CalibrationsInfo> result = CalibrationsHelper.mergeCalibrations(testContext.getPrevIniFile(),
				testContext.getPrevTune(), testContext.getUpdatedCalibrationsInfo(), testContext.getCallbacks(),
				emptySet());
		assertTrue(result.isPresent());
	}
	
}
