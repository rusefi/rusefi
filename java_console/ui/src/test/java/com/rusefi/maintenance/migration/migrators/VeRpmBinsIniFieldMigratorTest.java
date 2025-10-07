package com.rusefi.maintenance.migration.migrators;

import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestCallbacks;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.Optional;

import static com.rusefi.maintenance.migration.migrators.TableAddColumnsMigrator.*;
import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.assertEquals;

public class VeRpmBinsIniFieldMigratorTest {
    private TestCallbacks testCallbacks;
    private BinsIniFieldMigratorStrategy testMigrator;

    private static final int OLD_VE_TABLE_COLS = 16;
    private static final int NEW_VE_TABLE_COLS = 24;

    @BeforeEach
    void setUp() {
        testCallbacks = new TestCallbacks();
        testMigrator = new BinsIniFieldMigratorStrategy(VE_RPM_BINS_FIELD_NAME, OLD_VE_TABLE_COLS, NEW_VE_TABLE_COLS);
    }

    @Test
    void checkVeRpmBinsMigrationWithMaximumPossibleStep() {
        final ArrayIniField oldVeTableField = new ArrayIniField(
            "veRpmBins",
            0,
            FieldType.UINT16,
            1,
            OLD_VE_TABLE_COLS,
            "RPM",
            1,
            "0",
            "9000",
            "0"
        );
        final ArrayIniField newVeTableField = new ArrayIniField(
            "veRpmBins",
            0,
            FieldType.UINT16,
            1,
            NEW_VE_TABLE_COLS,
            "RPM",
            1,
            "0",
            "9000", // max is too small to use even the minimum of existing steps
            "0"
        );

        final Optional<String> migratedValue = testMigrator.tryMigrateBins(
            oldVeTableField,
            newVeTableField,
            "\n" +
                "         800.0\n" +
                "         1100.0\n" +
                "         1300.0\n" +
                "         1600.0\n" +
                "         1900.0\n" +
                "         2300.0\n" +
                "         2700.0\n" +
                "         3100.0\n" +
                "         3500.0\n" +
                "         4000.0\n" +
                "         4500.0\n" +
                "         5000.0\n" +
                "         5500.0\n" +
                "         6000.0\n" +
                "         6800.0\n" +
                "         8500.0\n",
            testCallbacks
        );
        assertTrue(migratedValue.isPresent());
        assertEquals(
            "\n" +
                "         800.0\n" +
                "         1100.0\n" +
                "         1300.0\n" +
                "         1600.0\n" +
                "         1900.0\n" +
                "         2300.0\n" +
                "         2700.0\n" +
                "         3100.0\n" +
                "         3500.0\n" +
                "         4000.0\n" +
                "         4500.0\n" +
                "         5000.0\n" +
                "         5500.0\n" +
                "         6000.0\n" +
                "         6800.0\n" +
                "         8500.0\n" +
                "         8562.0\n" +
                "         8624.0\n" +
                "         8686.0\n" +
                "         8748.0\n" +
                "         8810.0\n" +
                "         8872.0\n" +
                "         8934.0\n" +
                "         8996.0\n",
            migratedValue.get()
        );
        assertEquals("", testCallbacks.getContent());
    }

    @Test
    void checkVeRpmBinsMigrationWithMaximumStep() {
        final ArrayIniField oldVeTableField = new ArrayIniField(
            "veRpmBins",
            0,
            FieldType.UINT16,
            1,
            OLD_VE_TABLE_COLS,
            "RPM",
            1,
            "0",
            "18000",
            "0"
        );
        final ArrayIniField newVeTableField = new ArrayIniField(
            "veRpmBins",
            0,
            FieldType.UINT16,
            1,
            NEW_VE_TABLE_COLS,
            "RPM",
            1,
            "0",
            "18000",
            "0"
        );

        final Optional<String> migratedValue = testMigrator.tryMigrateBins(
            oldVeTableField,
            newVeTableField,
            "\n" +
                "         800.0\n" +
                "         1100.0\n" +
                "         1300.0\n" +
                "         1600.0\n" +
                "         1900.0\n" +
                "         2300.0\n" +
                "         2700.0\n" +
                "         3100.0\n" +
                "         3500.0\n" +
                "         4000.0\n" +
                "         4500.0\n" +
                "         5000.0\n" +
                "         5500.0\n" +
                "         6000.0\n" +
                "         6800.0\n" +
                "         8500.0\n", // max is NOT enough for propagation with last step
            testCallbacks
        );
        assertTrue(migratedValue.isPresent());
        assertEquals(
            "\n" +
                "         800.0\n" +
                "         1100.0\n" +
                "         1300.0\n" +
                "         1600.0\n" +
                "         1900.0\n" +
                "         2300.0\n" +
                "         2700.0\n" +
                "         3100.0\n" +
                "         3500.0\n" +
                "         4000.0\n" +
                "         4500.0\n" +
                "         5000.0\n" +
                "         5500.0\n" +
                "         6000.0\n" +
                "         6800.0\n" +
                "         8500.0\n" +
                "         9300.0\n" +
                "         10100.0\n" +
                "         10900.0\n" +
                "         11700.0\n" +
                "         12500.0\n" +
                "         13300.0\n" +
                "         14100.0\n" +
                "         14900.0\n",
            migratedValue.get()
        );
        assertEquals("", testCallbacks.getContent());
    }

    @Test
    void checkVeRpmBinsMigrationWithLastStep() {
        final ArrayIniField oldVeTableField = new ArrayIniField(
            "veRpmBins",
            0,
            FieldType.UINT16,
            1,
            OLD_VE_TABLE_COLS,
            "RPM",
            1,
            "0",
            "18000",
            "0"
        );
        final ArrayIniField newVeTableField = new ArrayIniField(
            "veRpmBins",
            0,
            FieldType.UINT16,
            1,
            NEW_VE_TABLE_COLS,
            "RPM",
            1,
            "0",
            "23000", // max is enough for propagation with last step
            "0"
        );

        final Optional<String> migratedValue = testMigrator.tryMigrateBins(
            oldVeTableField,
            newVeTableField,
            "\n" +
                "         800.0\n" +
                "         1100.0\n" +
                "         1300.0\n" +
                "         1600.0\n" +
                "         1900.0\n" +
                "         2300.0\n" +
                "         2700.0\n" +
                "         3100.0\n" +
                "         3500.0\n" +
                "         4000.0\n" +
                "         4500.0\n" +
                "         5000.0\n" +
                "         5500.0\n" +
                "         6000.0\n" +
                "         6800.0\n" +
                "         8500.0\n",
            testCallbacks
        );
        assertTrue(migratedValue.isPresent());
        assertEquals(
            "\n" +
                "         800.0\n" +
                "         1100.0\n" +
                "         1300.0\n" +
                "         1600.0\n" +
                "         1900.0\n" +
                "         2300.0\n" +
                "         2700.0\n" +
                "         3100.0\n" +
                "         3500.0\n" +
                "         4000.0\n" +
                "         4500.0\n" +
                "         5000.0\n" +
                "         5500.0\n" +
                "         6000.0\n" +
                "         6800.0\n" +
                "         8500.0\n" +
                "         10200.0\n" +
                "         11900.0\n" +
                "         13600.0\n" +
                "         15300.0\n" +
                "         17000.0\n" +
                "         18700.0\n" +
                "         20400.0\n" +
                "         22100.0\n",
            migratedValue.get()
        );
        assertEquals("", testCallbacks.getContent());
    }

    @Test
    void checkVeRpmBinsMigrationWithMinimalStep() {
        final ArrayIniField oldVeTableField = new ArrayIniField(
            "veRpmBins",
            0,
            FieldType.UINT16,
            1,
            OLD_VE_TABLE_COLS,
            "RPM",
            1,
            "0",
            "18000",
            "0"
        );
        final ArrayIniField newVeTableField = new ArrayIniField(
            "veRpmBins",
            0,
            FieldType.UINT16,
            1,
            NEW_VE_TABLE_COLS,
            "RPM",
            1,
            "0",
            "8508", // minimal possible max for propagation with `1` step
            "0"
        );

        final Optional<String> migratedValue = testMigrator.tryMigrateBins(
            oldVeTableField,
            newVeTableField,
            "\n" +
                "         800.0\n" +
                "         1100.0\n" +
                "         1300.0\n" +
                "         1600.0\n" +
                "         1900.0\n" +
                "         2300.0\n" +
                "         2700.0\n" +
                "         3100.0\n" +
                "         3500.0\n" +
                "         4000.0\n" +
                "         4500.0\n" +
                "         5000.0\n" +
                "         5500.0\n" +
                "         6000.0\n" +
                "         6800.0\n" +
                "         8500.0\n",
            testCallbacks
        );
        assertTrue(migratedValue.isPresent());
        assertEquals(
            "\n" +
                "         800.0\n" +
                "         1100.0\n" +
                "         1300.0\n" +
                "         1600.0\n" +
                "         1900.0\n" +
                "         2300.0\n" +
                "         2700.0\n" +
                "         3100.0\n" +
                "         3500.0\n" +
                "         4000.0\n" +
                "         4500.0\n" +
                "         5000.0\n" +
                "         5500.0\n" +
                "         6000.0\n" +
                "         6800.0\n" +
                "         8500.0\n" +
                "         8501.0\n" +
                "         8502.0\n" +
                "         8503.0\n" +
                "         8504.0\n" +
                "         8505.0\n" +
                "         8506.0\n" +
                "         8507.0\n" +
                "         8508.0\n",
            migratedValue.get()
        );
        assertEquals("", testCallbacks.getContent());
    }

    @Test
    void checkImposssibleVeRpmBinsMigration() {
        final ArrayIniField oldVeTableField = new ArrayIniField(
            "veRpmBins",
            0,
            FieldType.UINT16,
            1,
            OLD_VE_TABLE_COLS,
            "RPM",
            1,
            "0",
            "9000",
            "0"
        );
        final ArrayIniField newVeTableField = new ArrayIniField(
            "veRpmBins",
            0,
            FieldType.UINT16,
            1,
            NEW_VE_TABLE_COLS,
            "RPM",
            1,
            "0",
            "8507", // max is too small to fill missed bins with increasing values
            "0"
        );

        final Optional<String> migratedValue = testMigrator.tryMigrateBins(
            oldVeTableField,
            newVeTableField,
            "\n" +
                "         800.0\n" +
                "         1100.0\n" +
                "         1300.0\n" +
                "         1600.0\n" +
                "         1900.0\n" +
                "         2300.0\n" +
                "         2700.0\n" +
                "         3100.0\n" +
                "         3500.0\n" +
                "         4000.0\n" +
                "         4500.0\n" +
                "         5000.0\n" +
                "         5500.0\n" +
                "         6000.0\n" +
                "         6800.0\n" +
                "         8500.0\n",
            testCallbacks
        );
        assertFalse(migratedValue.isPresent());
        assertEquals(
            "WARNING! `veRpmBins` ini-field cannot be propagated with increasing values, because max value is 8507\r\n",
            testCallbacks.getContent()
        );
    }
}
