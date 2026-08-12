package com.rusefi.maintenance.migration.default_migration;

import com.rusefi.maintenance.TestTuneMigrationContext;

import jakarta.xml.bind.JAXBException;

import static com.rusefi.maintenance.migration.migrators.CltIdleCorrMigrator.*;
import static com.rusefi.maintenance.migration.migrators.TableAddColumnsMigrator.VE_RPM_BINS_FIELD_NAME;
import static com.rusefi.maintenance.migration.migrators.TableAddColumnsMigrator.VE_TABLE_FIELD_NAME;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

public class DefaultTestTuneMigrationContext {
    public static final String VEHICLE_NAME_FIELD_NAME = "vehicleName";
    public static final String IGNITION_TABLE_FIELD_NAME = "ignitionTable";

    static final String PREV_VEHICLE_NAME_VALUE = "Test Vehicle";

    public static final String PREV_IGNITION_TABLE_VALUE = "\n" +
        "         12.1 14.2 18.1 22.0 25.9 29.9 33.8 37.7 41.6 42.9 42.9 42.9 42.9 42.9 42.9 43.0\n" +
        "         12.0 14.0 17.9 21.7 25.6 29.5 33.3 37.2 41.0 42.3 42.3 42.3 42.3 42.3 42.3 42.4\n" +
        "         11.9 13.9 17.7 21.5 25.3 29.1 32.8 36.6 40.4 41.7 41.7 41.7 41.7 41.7 41.7 41.8\n" +
        "         11.7 13.7 17.4 21.2 24.9 28.7 32.4 36.1 39.9 41.1 41.1 41.1 41.1 41.1 41.1 41.2\n" +
        "         11.5 13.5 17.1 20.8 24.4 28.1 31.7 35.3 39.0 40.2 40.2 40.2 40.2 40.2 40.2 40.3\n" +
        "         11.4 13.3 16.9 20.5 24.1 27.7 31.2 34.8 38.4 39.6 39.6 39.6 39.6 39.6 39.6 39.7\n" +
        "         11.3 13.2 16.7 20.2 23.7 27.3 30.8 34.3 37.8 39.0 39.0 39.0 39.0 39.0 39.0 39.1\n" +
        "         11.2 13.0 16.5 19.9 23.4 26.9 30.3 33.9 37.2 38.4 38.4 38.4 38.4 38.4 38.4 38.5\n" +
        "         11.0 12.8 16.2 19.6 23.0 26.5 29.9 33.3 36.7 37.8 37.8 37.8 37.8 37.8 37.8 37.9\n" +
        "         10.8 12.6 15.9 19.2 22.5 25.9 29.2 32.5 35.8 36.9 36.9 36.9 36.9 36.9 36.9 37.0\n" +
        "         10.7 12.4 15.7 18.9 22.2 25.5 28.7 32.0 35.2 36.3 36.3 36.3 36.3 36.3 36.3 36.4\n" +
        "         10.6 12.3 15.5 18.7 21.9 25.1 28.2 31.4 34.6 35.7 35.7 35.7 35.7 35.7 35.7 35.8\n" +
        "         10.4 12.1 15.2 18.4 21.5 24.7 27.8 30.9 34.1 35.1 35.1 35.1 35.1 35.1 35.1 35.2\n" +
        "         10.4 12.1 15.2 18.3 21.5 24.6 27.7 30.8 34.0 35.0 35.0 35.0 35.0 35.0 35.0 35.1\n" +
        "         10.4 12.1 15.2 18.3 21.5 24.6 27.7 30.8 34.0 35.0 35.0 35.0 35.0 35.0 35.0 35.1\n" +
        "         10.4 12.1 15.2 18.3 21.5 24.6 27.7 30.8 34.0 35.0 35.0 35.0 35.0 35.0 35.0 35.1\n";

    public static final String UPDATED_IGNITION_TABLE_VALUE = "\n" +
        "         12.2 14.2 18.1 22.0 25.9 29.9 33.8 37.7 41.6 42.9 42.9 42.9 42.9 42.9 42.9 42.9\n" +
        "         12.1 14.0 17.9 21.7 25.6 29.5 33.3 37.2 41.0 42.3 42.3 42.3 42.3 42.3 42.3 42.3\n" +
        "         12.0 13.9 17.7 21.5 25.3 29.1 32.8 36.6 40.4 41.7 41.7 41.7 41.7 41.7 41.7 41.7\n" +
        "         11.8 13.7 17.4 21.2 24.9 28.7 32.4 36.1 39.9 41.1 41.1 41.1 41.1 41.1 41.1 41.1\n" +
        "         11.6 13.5 17.1 20.8 24.4 28.1 31.7 35.3 39.0 40.2 40.2 40.2 40.2 40.2 40.2 40.2\n" +
        "         11.5 13.3 16.9 20.5 24.1 27.7 31.2 34.8 38.4 39.6 39.6 39.6 39.6 39.6 39.6 39.6\n" +
        "         11.4 13.2 16.7 20.2 23.7 27.3 30.8 34.3 37.8 39.0 39.0 39.0 39.0 39.0 39.0 39.0\n" +
        "         11.3 13.0 16.5 19.9 23.4 26.9 30.3 33.8 37.2 38.4 38.4 38.4 38.4 38.4 38.4 38.4\n" +
        "         11.1 12.8 16.2 19.6 23.0 26.5 29.9 33.3 36.7 37.8 37.8 37.8 37.8 37.8 37.8 37.8\n" +
        "         10.9 12.6 15.9 19.2 22.5 25.9 29.2 32.5 35.8 36.9 36.9 36.9 36.9 36.9 36.9 36.9\n" +
        "         10.8 12.4 15.7 18.9 22.2 25.5 28.7 32.0 35.2 36.3 36.3 36.3 36.3 36.3 36.3 36.3\n" +
        "         10.7 12.3 15.5 18.7 21.9 25.1 28.2 31.4 34.6 35.7 35.7 35.7 35.7 35.7 35.7 35.7\n" +
        "         10.5 12.1 15.2 18.4 21.5 24.7 27.8 30.9 34.1 35.1 35.1 35.1 35.1 35.1 35.1 35.1\n" +
        "         10.5 12.1 15.2 18.3 21.5 24.6 27.7 30.8 34.0 35.0 35.0 35.0 35.0 35.0 35.0 35.0\n" +
        "         10.5 12.1 15.2 18.3 21.5 24.6 27.7 30.8 34.0 35.0 35.0 35.0 35.0 35.0 35.0 35.0\n" +
        "         10.5 12.1 15.2 18.3 21.5 24.6 27.7 30.8 34.0 35.0 35.0 35.0 35.0 35.0 35.0 35.0\n";

    public static final String ENABLE_KNOCK_SPECTROGRAM_FIELD_NAME = "enableKnockSpectrogram";

    public static final String PREV_ENABLE_KNOCK_SPECTROGRAM_VALUE = "\"false\"";

    public static final String UPDATED_ENABLE_KNOCK_SPECTROGRAM_VALUE = "\"no\"";

    public static final String PREV_MAN_IDLE_POSITION_VALUE = "30.0";

    public static final String UPDATED_MAN_IDLE_POSITION_VALUE = "50.0";

    public static final String PREV_CLT_IDLE_CORR_BINS_VALUE = "\n" +
        "         -41.0\n" +
        "         -31.0\n" +
        "         -21.0\n" +
        "         -11.0\n" +
        "         1.0\n" +
        "         11.0\n" +
        "         21.0\n" +
        "         31.0\n" +
        "         41.0\n" +
        "         51.0\n" +
        "         61.0\n" +
        "         71.0\n" +
        "         81.0\n" +
        "         91.0\n" +
        "         101.0\n" +
        "         111.0\n";

    public static final String UPDATED_CLT_IDLE_CORR_BINS_VALUE = "\n" +
        "         -40.0\n" +
        "         -30.0\n" +
        "         -20.0\n" +
        "         -10.0\n" +
        "         0.0\n" +
        "         10.0\n" +
        "         20.0\n" +
        "         30.0\n" +
        "         40.0\n" +
        "         50.0\n" +
        "         60.0\n" +
        "         70.0\n" +
        "         80.0\n" +
        "         90.0\n" +
        "         100.0\n" +
        "         110.0\n";


    public static final String PREV_CLT_IDLE_CORR_VALUE = "\n" +
        "         1.6\n" +
        "         1.6\n" +
        "         1.433333\n" +
        "         1.433333\n" +
        "         1.433333\n" +
        "         1.433333\n" +
        "         1.433333\n" +
        "         1.433333\n" +
        "         1.433333\n" +
        "         1.333333\n" +
        "         1.266667\n" +
        "         1.2\n" +
        "         1.1\n" +
        "         1.1\n" +
        "         1.1\n" +
        "         1.1\n";

    public static final String UPDATED_CLT_IDLE_CORR_VALUE = "\n" +
        "         1.5\n" +
        "         1.5\n" +
        "         1.333333\n" +
        "         1.333333\n" +
        "         1.333333\n" +
        "         1.333333\n" +
        "         1.333333\n" +
        "         1.333333\n" +
        "         1.333333\n" +
        "         1.233333\n" +
        "         1.166667\n" +
        "         1.1\n" +
        "         1.0\n" +
        "         1.0\n" +
        "         1.0\n" +
        "         1.0\n";

    public static final String PREV_VE_RPM_BINS_VALUE = "\n" +
        "         650.0\n" +
        "         800.0\n" +
        "         1100.0\n" +
        "         1400.0\n" +
        "         1700.0\n" +
        "         2000.0\n" +
        "         2300.0\n" +
        "         2600.0\n" +
        "         2900.0\n" +
        "         3200.0\n" +
        "         3500.0\n" +
        "         3800.0\n" +
        "         4100.0\n" +
        "         4400.0\n" +
        "         4700.0\n" +
        "         7000.0\n";

    public static final String UPDATED_VE_RPM_BINS_VALUE = "\n" +
        "         651.0\n" +
        "         801.0\n" +
        "         1101.0\n" +
        "         1401.0\n" +
        "         1701.0\n" +
        "         2001.0\n" +
        "         2301.0\n" +
        "         2601.0\n" +
        "         2901.0\n" +
        "         3201.0\n" +
        "         3501.0\n" +
        "         3801.0\n" +
        "         4101.0\n" +
        "         4401.0\n" +
        "         4701.0\n" +
        "         7001.0\n";


    public static final String PREV_VE_TABLE_VALUE = "\n" +
        "         49.3 49.3 49.4 49.6 50.2 51.4 52.6 53.8 54.4 54.6 54.4 53.7 52.8 51.8 50.9 50.0\n" +
        "         49.6 50.5 51.5 54.1 57.5 60.7 62.9 64.4 65.0 65.0 64.5 63.5 62.3 61.1 60.0 58.8\n" +
        "         50.3 52.4 54.3 58.5 62.7 66.1 68.2 69.7 70.2 70.1 69.5 68.4 67.1 65.8 64.5 63.3\n" +
        "         51.4 55.2 58.2 63.5 68.0 71.4 73.5 74.9 75.3 75.2 74.5 73.3 71.8 70.4 69.0 67.7\n" +
        "         54.3 60.9 64.9 70.7 75.1 78.3 80.3 81.7 82.1 81.9 81.1 79.7 78.1 76.5 75.0 73.5\n" +
        "         56.4 64.4 68.7 74.5 78.7 81.8 83.8 85.1 85.5 85.3 84.4 83.0 81.3 79.6 78.0 76.4\n" +
        "         60.7 70.2 74.5 79.9 83.8 86.8 88.9 90.2 90.6 90.3 89.4 87.8 86.0 84.2 82.5 80.8\n" +
        "         65.8 76.0 80.0 85.0 88.8 91.8 93.9 95.2 95.6 95.3 94.3 92.6 90.7 88.8 87.0 85.2\n" +
        "         69.4 79.1 82.9 87.8 91.7 94.8 97.0 98.3 98.8 98.4 97.4 95.6 93.5 91.5 89.6 87.7\n" +
        "         73.0 82.2 85.9 90.7 94.6 97.8 100.0 101.5 101.9 101.6 100.4 98.5 96.4 94.2 92.2 90.2\n" +
        "         76.6 85.3 88.8 93.5 97.4 100.7 103.1 104.6 105.1 104.7 103.5 101.5 99.2 96.9 94.8 92.6\n" +
        "         80.3 88.5 91.8 96.4 100.3 103.7 106.2 107.8 108.2 107.8 106.5 104.4 102.0 99.7 97.4 95.1\n" +
        "         83.9 91.6 94.7 99.2 103.2 106.7 109.2 110.9 111.4 110.9 109.6 107.4 104.8 102.4 99.9 97.6\n" +
        "         87.5 94.7 97.6 102.0 106.1 109.7 112.3 114.0 114.5 114.1 112.6 110.3 107.7 105.1 102.5 100.1\n" +
        "         91.1 97.8 100.6 104.9 108.9 112.6 115.3 117.2 117.7 117.2 115.7 113.3 110.5 107.8 105.1 102.5\n" +
        "         94.7 100.9 103.5 107.7 111.8 115.6 118.4 120.3 120.8 120.3 118.7 116.2 113.3 110.5 107.7 105.0\n";

    public static final String UPDATED_VE_TABLE_VALUE = "\n" +
        "         49.2 49.3 49.4 49.6 50.2 51.4 52.6 53.8 54.4 54.6 54.4 53.7 52.8 51.8 50.9 50.0\n" +
        "         49.6 50.4 51.5 54.1 57.5 60.7 62.9 64.4 65.0 65.0 64.5 63.5 62.3 61.1 60.0 58.8\n" +
        "         50.3 52.4 54.2 58.5 62.7 66.1 68.2 69.7 70.2 70.1 69.5 68.4 67.1 65.8 64.5 63.3\n" +
        "         51.4 55.2 58.2 63.4 68.0 71.4 73.5 74.9 75.3 75.2 74.5 73.3 71.8 70.4 69.0 67.7\n" +
        "         54.3 60.9 64.9 70.7 75.0 78.3 80.3 81.7 82.1 81.9 81.1 79.7 78.1 76.5 75.0 73.5\n" +
        "         56.4 64.4 68.7 74.5 78.7 81.7 83.8 85.1 85.5 85.3 84.4 83.0 81.3 79.6 78.0 76.4\n" +
        "         60.7 70.2 74.5 79.9 83.8 86.8 88.8 90.2 90.6 90.3 89.4 87.8 86.0 84.2 82.5 80.8\n" +
        "         65.8 76.0 80.0 85.0 88.8 91.8 93.9 95.1 95.6 95.3 94.3 92.6 90.7 88.8 87.0 85.2\n" +
        "         69.4 79.1 82.9 87.8 91.7 94.8 97.0 98.3 98.7 98.4 97.4 95.6 93.5 91.5 89.6 87.7\n" +
        "         73.0 82.2 85.9 90.7 94.6 97.8 100.0 101.5 101.8 101.6 100.4 98.5 96.4 94.2 92.2 90.2\n" +
        "         76.6 85.3 88.8 93.5 97.4 100.7 103.1 104.6 105.1 104.6 103.5 101.5 99.2 96.9 94.8 92.6\n" +
        "         80.3 88.5 91.8 96.4 100.3 103.7 106.2 107.8 108.2 107.8 106.4 104.4 102.0 99.7 97.4 95.1\n" +
        "         83.9 91.6 94.7 99.2 103.2 106.7 109.2 110.9 111.4 110.9 109.6 107.3 104.8 102.4 99.9 97.6\n" +
        "         87.5 94.7 97.6 102.0 106.1 109.7 112.3 114.0 114.5 114.1 112.6 110.3 107.6 105.1 102.5 100.1\n" +
        "         91.1 97.8 100.6 104.9 108.9 112.6 115.3 117.2 117.7 117.2 115.7 113.3 110.5 107.7 105.1 102.5\n" +
        "         94.7 100.9 103.5 107.7 111.8 115.6 118.4 120.3 120.8 120.3 118.7 116.2 113.3 110.5 107.6 105.0\n";

    public static TestTuneMigrationContext load() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/default_migration/test_data"
        );
        assertEquals(
            PREV_VEHICLE_NAME_VALUE,
            result.getPrevValue(VEHICLE_NAME_FIELD_NAME).getValue()
        );
        assertNull(result.getUpdatedValue(VEHICLE_NAME_FIELD_NAME));
        assertEquals(
            PREV_IGNITION_TABLE_VALUE,
            result.getPrevValue(IGNITION_TABLE_FIELD_NAME).getValue()
        );
        assertEquals(
            UPDATED_IGNITION_TABLE_VALUE,
            result.getUpdatedValue(IGNITION_TABLE_FIELD_NAME).getValue()
        );
        assertEquals(
            PREV_ENABLE_KNOCK_SPECTROGRAM_VALUE,
            result.getPrevValue(ENABLE_KNOCK_SPECTROGRAM_FIELD_NAME).getValue()
        );
        assertEquals(
            UPDATED_ENABLE_KNOCK_SPECTROGRAM_VALUE,
            result.getUpdatedValue(ENABLE_KNOCK_SPECTROGRAM_FIELD_NAME).getValue()
        );
        assertEquals(
            PREV_MAN_IDLE_POSITION_VALUE,
            result.getPrevValue(MAN_IDLE_POSITION_FIELD_NAME).getValue()
        );
        assertEquals(
            UPDATED_MAN_IDLE_POSITION_VALUE,
            result.getUpdatedValue(MAN_IDLE_POSITION_FIELD_NAME).getValue()
        );
        assertEquals(
            PREV_CLT_IDLE_CORR_BINS_VALUE,
            result.getPrevValue(CLT_IDLE_CORR_BINS_FIELD_NAME).getValue()
        );
        assertEquals(
            UPDATED_CLT_IDLE_CORR_BINS_VALUE,
            result.getUpdatedValue(CLT_IDLE_CORR_BINS_FIELD_NAME).getValue()
        );
        assertEquals(
            PREV_CLT_IDLE_CORR_VALUE,
            result.getPrevValue(CLT_IDLE_CORR_FIELD_NAME).getValue()
        );
        assertEquals(
            UPDATED_CLT_IDLE_CORR_VALUE,
            result.getUpdatedValue(CLT_IDLE_CORR_FIELD_NAME).getValue()
        );
        assertEquals(
            PREV_VE_RPM_BINS_VALUE,
            result.getPrevValue(VE_RPM_BINS_FIELD_NAME).getValue()
        );
        assertEquals(
            UPDATED_VE_RPM_BINS_VALUE,
            result.getUpdatedValue(VE_RPM_BINS_FIELD_NAME).getValue()
        );
        assertEquals(
            PREV_VE_TABLE_VALUE,
            result.getPrevValue(VE_TABLE_FIELD_NAME).getValue()
        );
        assertEquals(
            UPDATED_VE_TABLE_VALUE,
            result.getUpdatedValue(VE_TABLE_FIELD_NAME).getValue()
        );
        return result;
    }
}

