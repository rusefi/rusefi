package com.rusefi.maintenance.migration.afr_msq_import_migration;

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


public class AfrMsqImportMigrationTest {

	@Test
	void checkAfrMsqMigrationAfrToLambda() throws JAXBException {
		final TestTuneMigrationContext testContext = AfrMsqImportMigrationContext.loadAfrToLambda();
		ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
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
		ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
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
    		ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
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

            /* FIXME: broken!
    		final Optional<CalibrationsInfo> result = CalibrationsHelper.mergeCalibrations(testContext.getPrevIniFile(),
    				testContext.getPrevTune(), testContext.getUpdatedCalibrationsInfo(), testContext.getCallbacks(),
    				emptySet());
    		assertTrue(result.isPresent());
    		*/
    	}
}