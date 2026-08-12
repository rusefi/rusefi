package com.rusefi.maintenance.migration.clt_idle_corr_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.maintenance.migration.default_migration.DefaultTestTuneMigrationContext;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import jakarta.xml.bind.JAXBException;
import java.util.Map;

import static com.rusefi.maintenance.migration.migrators.CltIdleCorrMigrator.*;
import static com.rusefi.maintenance.migration.clt_idle_corr_migration.CltIdleCorrMigrationTestTuneMigrationContext.TEST_PREV_CLT_IDLE_CORR_BINS_VALUE;
import static com.rusefi.maintenance.migration.default_migration.DefaultTestTuneMigrationContext.*;
import static org.junit.jupiter.api.Assertions.*;

public class CltIdleCorrMigratorTest {
    @Test
    void checkBattLagCorrMigrationWithDisappearedManIdlePosition() throws JAXBException {
        final TestTuneMigrationContext testContext = CltIdleCorrMigrationTestTuneMigrationContext.load();
        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
        final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

        assertEquals(3, migratedConstants.size());

        // migrated `manIdlePosition` is null to prevent processing of disappeared `ManIdlePosition` ini-field by
        // `DefaultTuneMigrator`:
        assertTrue(migratedConstants.containsKey(MAN_IDLE_POSITION_FIELD_NAME));
        assertNull(migratedConstants.get(MAN_IDLE_POSITION_FIELD_NAME));
        assertFalse(testContext.getTestCallbacks().getContent().contains(MAN_IDLE_POSITION_FIELD_NAME));

        {
            final Constant migratedCltIdleCorrBinsValue = migratedConstants.get(CLT_IDLE_CORR_BINS_FIELD_NAME);
            assertNotNull(migratedCltIdleCorrBinsValue);
            assertEquals(CLT_IDLE_CORR_BINS_FIELD_NAME, migratedCltIdleCorrBinsValue.getName());
            assertEquals("C", migratedCltIdleCorrBinsValue.getUnits());
            assertEquals(TEST_PREV_CLT_IDLE_CORR_BINS_VALUE, migratedCltIdleCorrBinsValue.getValue());
            assertEquals("2", migratedCltIdleCorrBinsValue.getDigits());
            assertEquals("16", migratedCltIdleCorrBinsValue.getRows());
            assertEquals("1", migratedCltIdleCorrBinsValue.getCols());
        }
        {
            final Constant migratedCltIdleCorrValue = migratedConstants.get(CLT_IDLE_CORR_FIELD_NAME);
            assertNotNull(migratedCltIdleCorrValue);
            assertEquals(CLT_IDLE_CORR_FIELD_NAME, migratedCltIdleCorrValue.getName());
            assertEquals("%", migratedCltIdleCorrValue.getUnits());
            assertEquals(
                "\n" +
                    "         75.0\n" +
                    "         75.0\n" +
                    "         66.66667\n" +
                    "         66.66667\n" +
                    "         66.66667\n" +
                    "         66.66667\n" +
                    "         66.66667\n" +
                    "         66.66667\n" +
                    "         66.66667\n" +
                    "         61.666664999999995\n" +
                    "         58.333330000000004\n" +
                    "         55.00000000000001\n" +
                    "         50.0\n" +
                    "         50.0\n" +
                    "         50.0\n" +
                    "         50.0\n",
                migratedCltIdleCorrValue.getValue()
            );
            assertEquals("2", migratedCltIdleCorrValue.getDigits());
            assertEquals("16", migratedCltIdleCorrValue.getRows());
            assertEquals("1", migratedCltIdleCorrValue.getCols());
        }
    }

    @Test
    void checkBattLagCorrMigrationWithSurvivivngManIdlePosition() throws JAXBException {
        final TestTuneMigrationContext testContext = DefaultTestTuneMigrationContext.load();
        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
        final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();

        {
            final Constant migratedCltIdleCorrBinsValue = migratedConstants.get(MAN_IDLE_POSITION_FIELD_NAME);
            assertNotNull(migratedCltIdleCorrBinsValue);
            assertEquals(MAN_IDLE_POSITION_FIELD_NAME, migratedCltIdleCorrBinsValue.getName());
            assertEquals("%", migratedCltIdleCorrBinsValue.getUnits());
            assertEquals(PREV_MAN_IDLE_POSITION_VALUE, migratedCltIdleCorrBinsValue.getValue());
            assertEquals("0", migratedCltIdleCorrBinsValue.getDigits());
            assertNull(migratedCltIdleCorrBinsValue.getRows());
            assertNull(migratedCltIdleCorrBinsValue.getCols());
        }
        {
            final Constant migratedCltIdleCorrBinsValue = migratedConstants.get(CLT_IDLE_CORR_BINS_FIELD_NAME);
            assertNotNull(migratedCltIdleCorrBinsValue);
            assertEquals(CLT_IDLE_CORR_BINS_FIELD_NAME, migratedCltIdleCorrBinsValue.getName());
            assertNull(migratedCltIdleCorrBinsValue.getUnits());
            assertEquals(PREV_CLT_IDLE_CORR_BINS_VALUE, migratedCltIdleCorrBinsValue.getValue());
            assertEquals("2", migratedCltIdleCorrBinsValue.getDigits());
            assertEquals("16", migratedCltIdleCorrBinsValue.getRows());
            assertEquals("1", migratedCltIdleCorrBinsValue.getCols());
        }
        {
            final Constant migratedCltIdleCorrValue = migratedConstants.get(CLT_IDLE_CORR_FIELD_NAME);
            assertNotNull(migratedCltIdleCorrValue);
            assertEquals(CLT_IDLE_CORR_FIELD_NAME, migratedCltIdleCorrValue.getName());
            assertNull(migratedCltIdleCorrValue.getUnits());
            assertEquals(PREV_CLT_IDLE_CORR_VALUE, migratedCltIdleCorrValue.getValue());
            assertEquals("2", migratedCltIdleCorrValue.getDigits());
            assertEquals("16", migratedCltIdleCorrValue.getRows());
            assertEquals("1", migratedCltIdleCorrValue.getCols());
        }
    }
}

