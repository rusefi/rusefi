package com.rusefi.maintenance.migration;

import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestCallbacks;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.Optional;

import static org.junit.jupiter.api.Assertions.*;

public class VeTableExtensionMigratorTest {
    private TestCallbacks testCallbacks;

    @BeforeEach
    void setUp() {
        testCallbacks = new TestCallbacks();
    }

    @Test
    void checkVeTableMigration() {
        final ArrayIniField oldVeTableField = new ArrayIniField(
            "veTable",
            0,
            FieldType.UINT16,
            16,
            16,
            "%",
            0.1,
            "0",
            "999",
            "1"
        );
        final ArrayIniField newVeTableField = new ArrayIniField(
            "veTable",
            0,
            FieldType.UINT16,
            24,
            16,
            "%",
            0.1,
            "0",
            "999",
            "1"
        );

        final Optional<String> migratedValue = VeTableExtensionMigrator.INSTANCE.tryMigrateValue(
            oldVeTableField,
            newVeTableField,
            "\n" +
                "         48.5 48.4 51.4 48.9 48.2 46.0 46.0 46.0 51.6 52.3 52.4 51.8 50.8 49.9 48.9 48.0\n" +
                "         45.6 48.9 49.2 51.9 49.0 46.0 46.0 46.0 49.7 50.0 49.5 48.3 47.1 46.0 44.9 43.8\n" +
                "         46.7 47.0 49.0 54.7 49.8 46.0 46.0 46.0 53.2 53.3 52.5 51.3 50.0 48.8 47.7 46.5\n" +
                "         50.0 48.0 50.0 62.9 60.1 47.1 46.7 44.5 45.6 52.7 54.1 54.5 51.9 46.9 45.6 48.3\n" +
                "         54.3 51.0 51.3 71.5 68.3 51.3 50.5 48.0 48.3 49.6 54.6 56.8 55.6 50.7 49.4 51.8\n" +
                "         68.9 70.8 72.0 73.9 72.0 65.6 59.1 53.7 55.3 56.2 53.0 58.0 56.8 55.8 55.5 55.8\n" +
                "         68.9 70.8 72.0 73.9 72.0 65.4 58.8 57.9 59.5 59.3 55.6 59.5 58.4 58.3 59.1 59.5\n" +
                "         68.9 70.8 72.0 73.9 72.0 65.6 59.1 58.8 60.1 62.8 60.7 62.2 62.0 61.7 61.8 61.6\n" +
                "         68.9 70.8 72.0 73.9 72.0 68.1 64.1 63.4 62.0 63.8 65.1 67.5 67.1 66.5 65.2 63.9\n" +
                "         64.3 70.8 72.0 73.9 72.0 68.4 64.8 64.3 63.1 64.6 68.4 72.0 72.1 71.4 69.1 66.8\n" +
                "         64.8 70.8 72.0 73.9 72.0 68.8 65.6 65.1 64.2 65.4 70.1 74.7 77.2 76.7 73.2 69.3\n" +
                "         65.2 70.8 72.0 73.9 72.0 69.2 66.3 66.0 65.2 66.1 69.4 77.1 80.5 80.3 77.9 72.2\n" +
                "         66.1 70.8 72.0 73.9 72.0 69.9 67.7 67.7 67.4 67.7 70.4 78.5 82.8 82.7 80.6 77.8\n" +
                "         66.6 69.7 70.9 72.2 71.0 68.3 68.5 68.5 68.5 68.5 70.8 77.4 80.2 79.8 78.3 77.4\n" +
                "         67.0 68.6 69.7 70.6 70.1 68.9 69.2 69.4 69.5 69.2 71.1 72.8 73.8 73.2 71.9 68.7\n" +
                "         67.5 67.5 68.6 68.9 69.1 69.5 69.9 70.2 70.6 70.0 71.2 70.6 70.0 69.3 68.4 67.0\n",
            testCallbacks
        );
        assertTrue(migratedValue.isPresent());
        assertEquals(
            "\n" +
                "         48.5 48.4 51.4 48.9 48.2 46.0 46.0 46.0 51.6 52.3 52.4 51.8 50.8 49.9 48.9 48.0 48.0 48.0 48.0 48.0 48.0 48.0 48.0 48.0\n" +
                "         45.6 48.9 49.2 51.9 49.0 46.0 46.0 46.0 49.7 50.0 49.5 48.3 47.1 46.0 44.9 43.8 43.8 43.8 43.8 43.8 43.8 43.8 43.8 43.8\n" +
                "         46.7 47.0 49.0 54.7 49.8 46.0 46.0 46.0 53.2 53.3 52.5 51.3 50.0 48.8 47.7 46.5 46.5 46.5 46.5 46.5 46.5 46.5 46.5 46.5\n" +
                "         50.0 48.0 50.0 62.9 60.1 47.1 46.7 44.5 45.6 52.7 54.1 54.5 51.9 46.9 45.6 48.3 48.3 48.3 48.3 48.3 48.3 48.3 48.3 48.3\n" +
                "         54.3 51.0 51.3 71.5 68.3 51.3 50.5 48.0 48.3 49.6 54.6 56.8 55.6 50.7 49.4 51.8 51.8 51.8 51.8 51.8 51.8 51.8 51.8 51.8\n" +
                "         68.9 70.8 72.0 73.9 72.0 65.6 59.1 53.7 55.3 56.2 53.0 58.0 56.8 55.8 55.5 55.8 55.8 55.8 55.8 55.8 55.8 55.8 55.8 55.8\n" +
                "         68.9 70.8 72.0 73.9 72.0 65.4 58.8 57.9 59.5 59.3 55.6 59.5 58.4 58.3 59.1 59.5 59.5 59.5 59.5 59.5 59.5 59.5 59.5 59.5\n" +
                "         68.9 70.8 72.0 73.9 72.0 65.6 59.1 58.8 60.1 62.8 60.7 62.2 62.0 61.7 61.8 61.6 61.6 61.6 61.6 61.6 61.6 61.6 61.6 61.6\n" +
                "         68.9 70.8 72.0 73.9 72.0 68.1 64.1 63.4 62.0 63.8 65.1 67.5 67.1 66.5 65.2 63.9 63.9 63.9 63.9 63.9 63.9 63.9 63.9 63.9\n" +
                "         64.3 70.8 72.0 73.9 72.0 68.4 64.8 64.3 63.1 64.6 68.4 72.0 72.1 71.4 69.1 66.8 66.8 66.8 66.8 66.8 66.8 66.8 66.8 66.8\n" +
                "         64.8 70.8 72.0 73.9 72.0 68.8 65.6 65.1 64.2 65.4 70.1 74.7 77.2 76.7 73.2 69.3 69.3 69.3 69.3 69.3 69.3 69.3 69.3 69.3\n" +
                "         65.2 70.8 72.0 73.9 72.0 69.2 66.3 66.0 65.2 66.1 69.4 77.1 80.5 80.3 77.9 72.2 72.2 72.2 72.2 72.2 72.2 72.2 72.2 72.2\n" +
                "         66.1 70.8 72.0 73.9 72.0 69.9 67.7 67.7 67.4 67.7 70.4 78.5 82.8 82.7 80.6 77.8 77.8 77.8 77.8 77.8 77.8 77.8 77.8 77.8\n" +
                "         66.6 69.7 70.9 72.2 71.0 68.3 68.5 68.5 68.5 68.5 70.8 77.4 80.2 79.8 78.3 77.4 77.4 77.4 77.4 77.4 77.4 77.4 77.4 77.4\n" +
                "         67.0 68.6 69.7 70.6 70.1 68.9 69.2 69.4 69.5 69.2 71.1 72.8 73.8 73.2 71.9 68.7 68.7 68.7 68.7 68.7 68.7 68.7 68.7 68.7\n" +
                "         67.5 67.5 68.6 68.9 69.1 69.5 69.9 70.2 70.6 70.0 71.2 70.6 70.0 69.3 68.4 67.0 67.0 67.0 67.0 67.0 67.0 67.0 67.0 67.0\n",
            migratedValue.get()
        );
        assertEquals("", testCallbacks.getContent());
    }

    @Test
    void checkVeRpmBinsMigrationWithMaximumPossibleStep() {
        final ArrayIniField oldVeTableField = new ArrayIniField(
            "veRpmBins",
            0,
            FieldType.UINT16,
            1,
            16,
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
            24,
            "RPM",
            1,
            "0",
            "9000", // max is too small to use even the minimum of existing steps
            "0"
        );

        final Optional<String> migratedValue = VeTableExtensionMigrator.INSTANCE.tryMigrateValue(
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
            16,
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
            24,
            "RPM",
            1,
            "0",
            "18000",
            "0"
        );

        final Optional<String> migratedValue = VeTableExtensionMigrator.INSTANCE.tryMigrateValue(
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
            16,
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
            24,
            "RPM",
            1,
            "0",
            "23000", // max is enough for propagation with last step
            "0"
        );

        final Optional<String> migratedValue = VeTableExtensionMigrator.INSTANCE.tryMigrateValue(
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
            16,
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
            24,
            "RPM",
            1,
            "0",
            "8508", // minimal possible max for propagation with `1` step
            "0"
        );

        final Optional<String> migratedValue = VeTableExtensionMigrator.INSTANCE.tryMigrateValue(
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
            16,
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
            24,
            "RPM",
            1,
            "0",
            "8507", // max is too small to fill missed bins with increasing values
            "0"
        );

        final Optional<String> migratedValue = VeTableExtensionMigrator.INSTANCE.tryMigrateValue(
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
