package com.rusefi.maintenance.migration.batt_lag_corr_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import jakarta.xml.bind.JAXBException;
import java.util.Map;

import static com.rusefi.maintenance.migration.migrators.BattLagCorrExtensionMigrator.INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME;
import static com.rusefi.maintenance.migration.migrators.BattLagCorrExtensionMigrator.INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME;
import static org.junit.jupiter.api.Assertions.*;

public class BattLagCorrMigratorTest {
    @Test
    void checkBattLagCorrMigration() throws JAXBException {
        final TestTuneMigrationContext testContext = BattLagCorrMigrationTestTuneMigrationContext.load();
        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
        final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

        assertEquals(2, migratedConstants.size());

        final Constant migratedInjectorBattLagCorrBattBins = migratedConstants.get(
            INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME
        );
        assertNotNull(migratedInjectorBattLagCorrBattBins);
        assertEquals(INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME, migratedInjectorBattLagCorrBattBins.getName());
        assertEquals("volts", migratedInjectorBattLagCorrBattBins.getUnits());
        Assertions.assertEquals(BattLagCorrMigrationTestTuneMigrationContext.INJECTOR_BATT_LAG_CORR_BINS_TEST_VALUE, migratedInjectorBattLagCorrBattBins.getValue());
        assertEquals("2", migratedInjectorBattLagCorrBattBins.getDigits());
        assertEquals("8", migratedInjectorBattLagCorrBattBins.getRows());
        assertEquals("1", migratedInjectorBattLagCorrBattBins.getCols());

        final Constant migratedInjectorBattLagCorrTable = migratedConstants.get(
            INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME
        );
        assertNotNull(migratedInjectorBattLagCorrTable);
        assertEquals(INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME, migratedInjectorBattLagCorrTable.getName());
        assertEquals("ms", migratedInjectorBattLagCorrTable.getUnits());
        assertEquals(
            "\n" +
            "         3.371 1.974 1.383 1.194 1.04 0.914 0.797 0.726\n" +
            "         3.371 1.974 1.383 1.194 1.04 0.914 0.797 0.726\n",
            migratedInjectorBattLagCorrTable.getValue()
        );
        assertEquals("2", migratedInjectorBattLagCorrTable.getDigits());
        assertEquals("2", migratedInjectorBattLagCorrTable.getRows());
        assertEquals("8", migratedInjectorBattLagCorrTable.getCols());

        assertEquals("", testContext.getTestCallbacks().getContent());
    }
}

