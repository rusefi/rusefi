package com.rusefi.maintenance.migration.ve_table_extension;

import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;

import java.util.Map;

import static com.rusefi.maintenance.migration.ve_table_extension.VeTableExtensionTestTuneMigrationContext.VE_RPM_BINS_FIELD_NAME;
import static com.rusefi.maintenance.migration.ve_table_extension.VeTableExtensionTestTuneMigrationContext.VE_TABLE_FIELD_NAME;
import static org.junit.jupiter.api.Assertions.*;

public class TableAddColumnsMigratorTest {
    @Test
    void checkVeTableMigration() throws JAXBException {
        final TestTuneMigrationContext testContext = VeTableExtensionTestTuneMigrationContext.load();
        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
        final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();
        assertEquals(2, migratedConstants.size());

        final Constant migratedVeTableValue = migratedConstants.get(VE_TABLE_FIELD_NAME);
        assertEquals(VE_TABLE_FIELD_NAME, migratedVeTableValue.getName());
        assertEquals("%", migratedVeTableValue.getUnits());
        assertEquals("\n" +
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
            migratedVeTableValue.getValue()
        );
        assertEquals("1", migratedVeTableValue.getDigits());
        assertEquals("16", migratedVeTableValue.getRows());
        assertEquals("24", migratedVeTableValue.getCols());

        final Constant migratedVeRpmBinsValue = migratedConstants.get(VE_RPM_BINS_FIELD_NAME);
        assertEquals(VE_RPM_BINS_FIELD_NAME, migratedVeRpmBinsValue.getName());
        assertEquals("RPM", migratedVeRpmBinsValue.getUnits());
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
            migratedVeRpmBinsValue.getValue()
        );
        assertEquals("0", migratedVeRpmBinsValue.getDigits());
        assertEquals("24", migratedVeRpmBinsValue.getRows());
        assertEquals("1", migratedVeRpmBinsValue.getCols());
    }
}
