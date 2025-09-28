package com.rusefi.maintenance.migration.table_add_columns_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;

import java.util.Map;

import static com.rusefi.maintenance.migration.TableAddColumnsMigrator.*;
import static com.rusefi.maintenance.migration.table_add_columns_migration.TableAddColumnsMigrationContext.*;
import static org.junit.jupiter.api.Assertions.*;

public class TableAddColumnsMigratorTest {
    @Test
    void checkVeTableMigration() throws JAXBException {
        final TestTuneMigrationContext testContext = TableAddColumnsMigrationContext.load();
        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
        final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();
        assertEquals(4, migratedConstants.size());

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

    @Test
    void checkLambdaTableMigration() throws JAXBException {
        final TestTuneMigrationContext testContext = TableAddColumnsMigrationContext.load();
        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
        final Map<String, Constant> migratedConstants = testContext.getMigratedConstants();
        assertEquals(4, migratedConstants.size());

        final Constant migratedLambdaTableValue = migratedConstants.get(LAMBDA_TABLE_FIELD_NAME);
        assertEquals(LAMBDA_TABLE_FIELD_NAME, migratedLambdaTableValue.getName());
        assertEquals("afr", migratedLambdaTableValue.getUnits());
        assertEquals("\n" +
                "         10.0 10.1 10.2 10.3 10.4 10.5 10.6 10.7 10.8 10.9 11.0 11.1 11.2 11.3 11.4 11.5 11.5 11.5 11.5 11.5 11.5 11.5 11.5 11.5\n" +
                "         11.0 11.1 11.2 11.3 11.4 11.5 11.6 11.7 11.8 11.9 12.0 12.1 12.2 12.3 12.4 12.5 12.5 12.5 12.5 12.5 12.5 12.5 12.5 12.5\n" +
                "         12.0 12.1 12.2 12.3 12.4 12.5 12.6 12.7 12.8 12.9 13.0 13.1 13.2 13.3 13.4 13.5 13.5 13.5 13.5 13.5 13.5 13.5 13.5 13.5\n" +
                "         13.0 13.1 13.2 13.3 13.4 13.5 13.6 13.7 13.8 13.9 14.0 14.1 14.2 14.3 14.4 14.5 14.5 14.5 14.5 14.5 14.5 14.5 14.5 14.5\n" +
                "         14.0 14.1 14.2 14.3 14.4 14.5 14.6 14.7 14.8 14.9 15.0 15.1 15.2 15.3 15.4 15.5 15.5 15.5 15.5 15.5 15.5 15.5 15.5 15.5\n" +
                "         15.0 15.1 15.2 15.3 15.4 15.5 15.6 15.7 15.8 15.9 16.0 16.1 16.2 16.3 16.4 16.5 16.5 16.5 16.5 16.5 16.5 16.5 16.5 16.5\n" +
                "         16.0 16.1 16.2 16.3 16.4 16.5 16.6 16.7 16.8 16.9 17.0 17.1 17.2 17.3 17.4 17.5 17.5 17.5 17.5 17.5 17.5 17.5 17.5 17.5\n" +
                "         17.0 17.1 17.2 17.3 17.4 17.5 17.6 17.7 17.8 17.9 18.0 18.1 18.2 18.3 18.4 18.5 18.5 18.5 18.5 18.5 18.5 18.5 18.5 18.5\n" +
                "         18.0 18.1 18.2 18.3 18.4 18.5 18.6 18.7 18.8 18.9 19.0 19.1 19.2 19.3 19.4 19.5 19.5 19.5 19.5 19.5 19.5 19.5 19.5 19.5\n" +
                "         19.0 19.1 19.2 19.3 19.4 19.5 19.6 19.7 19.8 19.9 20.0 20.1 20.2 20.3 20.4 20.5 20.5 20.5 20.5 20.5 20.5 20.5 20.5 20.5\n" +
                "         20.0 20.1 20.2 20.3 20.4 20.5 20.6 20.7 20.8 20.9 21.0 21.1 21.2 21.3 21.4 21.5 21.5 21.5 21.5 21.5 21.5 21.5 21.5 21.5\n" +
                "         21.0 21.1 21.2 21.3 21.4 21.5 21.6 21.7 21.8 21.9 22.0 22.1 22.2 22.3 22.4 22.5 22.5 22.5 22.5 22.5 22.5 22.5 22.5 22.5\n" +
                "         22.0 22.1 22.2 22.3 22.4 22.5 22.6 22.7 22.8 22.9 23.0 23.1 23.2 23.3 23.4 23.5 23.5 23.5 23.5 23.5 23.5 23.5 23.5 23.5\n" +
                "         23.0 23.1 23.2 23.3 23.4 23.5 23.6 23.7 23.8 23.9 24.0 24.1 24.2 24.3 24.4 24.5 24.5 24.5 24.5 24.5 24.5 24.5 24.5 24.5\n" +
                "         24.0 24.1 24.2 24.3 24.4 24.5 24.6 24.7 24.8 24.9 25.0 25.1 25.2 25.3 25.4 25.5 25.5 25.5 25.5 25.5 25.5 25.5 25.5 25.5\n" +
                "         25.0 25.1 25.2 25.3 25.4 25.5 25.6 25.7 25.8 25.9 26.0 26.1 26.2 26.3 26.4 26.5 26.5 26.5 26.5 26.5 26.5 26.5 26.5 26.5\n",
            migratedLambdaTableValue.getValue()
        );
        assertEquals("1", migratedLambdaTableValue.getDigits());
        assertEquals("16", migratedLambdaTableValue.getRows());
        assertEquals("24", migratedLambdaTableValue.getCols());

        final Constant migratedLambdaRpmBinsValue = migratedConstants.get(LAMBDA_RPM_BINS_FIELD_NAME);
        assertEquals(LAMBDA_RPM_BINS_FIELD_NAME, migratedLambdaRpmBinsValue.getName());
        assertEquals("RPM", migratedLambdaRpmBinsValue.getUnits());
        assertEquals(
            "\n" +
                "         800.0\n" +
                "         1000.0\n" +
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
                "         7250.0\n" +
                "         8500.0\n" +
                "         9000.0\n" +
                "         9500.0\n" +
                "         10000.0\n" +
                "         10500.0\n" +
                "         11000.0\n" +
                "         11500.0\n" +
                "         12000.0\n" +
                "         12500.0\n",
            migratedLambdaRpmBinsValue.getValue()
        );
        assertEquals("0", migratedLambdaRpmBinsValue.getDigits());
        assertEquals("24", migratedLambdaRpmBinsValue.getRows());
        assertEquals("1", migratedLambdaRpmBinsValue.getCols());
    }
}
