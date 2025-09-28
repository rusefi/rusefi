package com.rusefi.maintenance.migration.table_add_columns_migration;

import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import javax.xml.bind.JAXBException;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNull;

public class TableAddColumnsMigrationContext {
    public static final String VE_TABLE_FIELD_NAME = "veTable";
    public static final String VE_RPM_BINS_FIELD_NAME = "veRpmBins";

    public static TestTuneMigrationContext load() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/table_add_columns_migration/test_data"
        );
        validateVeTable(result);

        return result;
    }

    private static void validateVeTable(final TestTuneMigrationContext result) {
        final ArrayIniField prevVeTableIniField = (ArrayIniField) result.getPrevIniFile().getIniField(
            VE_TABLE_FIELD_NAME
        );
        assertEquals(FieldType.UINT16, prevVeTableIniField.getType());
        assertEquals(31636, prevVeTableIniField.getOffset());
        assertEquals(16, prevVeTableIniField.getCols());
        assertEquals(16, prevVeTableIniField.getRows());
        assertEquals(0.1, prevVeTableIniField.getMultiplier());
        assertEquals("0", prevVeTableIniField.getMin());
        assertEquals("999", prevVeTableIniField.getMax());
        assertEquals("1", prevVeTableIniField.getDigits());

        final ArrayIniField prevVeRpmBinsIniField = (ArrayIniField) result.getPrevIniFile().getIniField(
            VE_RPM_BINS_FIELD_NAME
        );
        assertEquals(FieldType.UINT16, prevVeRpmBinsIniField.getType());
        assertEquals(32180, prevVeRpmBinsIniField.getOffset());
        assertEquals(1, prevVeRpmBinsIniField.getCols());
        assertEquals(16, prevVeRpmBinsIniField.getRows());
        assertEquals(1, prevVeRpmBinsIniField.getMultiplier());
        assertEquals("0", prevVeRpmBinsIniField.getMin());
        assertEquals("18000", prevVeRpmBinsIniField.getMax());
        assertEquals("0", prevVeRpmBinsIniField.getDigits());

        final Constant prevVeTableValue = result.getPrevTune().getConstantsAsMap().get(VE_TABLE_FIELD_NAME);
        assertEquals(VE_TABLE_FIELD_NAME, prevVeTableValue.getName());
        assertNull(prevVeTableValue.getUnits());
        assertEquals(
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
            prevVeTableValue.getValue()
        );
        assertEquals("1", prevVeTableValue.getDigits());
        assertEquals("16", prevVeTableValue.getRows());
        assertEquals("16", prevVeTableValue.getCols());

        final Constant prevVeRpmBinsValue = result.getPrevTune().getConstantsAsMap().get(VE_RPM_BINS_FIELD_NAME);
        assertEquals(VE_RPM_BINS_FIELD_NAME, prevVeRpmBinsValue.getName());
        assertNull(prevVeRpmBinsValue.getUnits());
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
                "         8500.0\n",
            prevVeRpmBinsValue.getValue()
        );
        assertEquals("0", prevVeRpmBinsValue.getDigits());
        assertEquals("16", prevVeRpmBinsValue.getRows());
        assertEquals("1", prevVeRpmBinsValue.getCols());

        final ArrayIniField updatedVeTableIniField = (ArrayIniField) result.getUpdatedIniFile().getIniField(
            VE_TABLE_FIELD_NAME
        );
        assertEquals(FieldType.UINT16, updatedVeTableIniField.getType());
        assertEquals(31636, updatedVeTableIniField.getOffset());
        assertEquals(24, updatedVeTableIniField.getCols());
        assertEquals(16, updatedVeTableIniField.getRows());
        assertEquals(0.1, updatedVeTableIniField.getMultiplier());
        assertEquals("0", updatedVeTableIniField.getMin());
        assertEquals("999", updatedVeTableIniField.getMax());
        assertEquals("1", updatedVeTableIniField.getDigits());

        final ArrayIniField updatedVeRpmBinsIniField = (ArrayIniField) result.getUpdatedIniFile().getIniField(
            VE_RPM_BINS_FIELD_NAME
        );
        assertEquals(FieldType.UINT16, updatedVeRpmBinsIniField.getType());
        assertEquals(32436, updatedVeRpmBinsIniField.getOffset());
        assertEquals(1, updatedVeRpmBinsIniField.getCols());
        assertEquals(24, updatedVeRpmBinsIniField.getRows());
        assertEquals(1, updatedVeRpmBinsIniField.getMultiplier());
        assertEquals("0", updatedVeRpmBinsIniField.getMin());
        assertEquals("18000", updatedVeRpmBinsIniField.getMax());
        assertEquals("0", updatedVeRpmBinsIniField.getDigits());

        final Constant updatedVeTableValue = result.getUpdatedTune().getConstantsAsMap().get(VE_TABLE_FIELD_NAME);
        assertEquals(VE_TABLE_FIELD_NAME, updatedVeTableValue.getName());
        assertNull(updatedVeTableValue.getUnits());
        assertEquals(
            "\n" +
                "           1.1   2.1   3.1   4.1   5.1   6.1   7.1   8.1   9.1  10.1  11.1  12.1  13.1  14.1  15.1  16.1  17.1  18.1  19.1  20.1  21.1  22.1  23.1  24.1\n" +
                "          25.2  26.2  27.2  28.2  29.2  30.2  31.2  32.2  33.2  34.2  35.2  36.2  37.2  38.2  39.2  40.2  41.2  42.2  43.2  44.2  45.2  46.2  47.2  48.2\n" +
                "          49.3  50.3  51.3  52.3  53.3  54.3  55.3  56.3  57.3  58.3  59.3  60.3  61.3  62.3  63.3  64.3  65.3  66.3  67.3  68.3  69.3  70.3  71.3  72.3\n" +
                "          74.4  75.4  76.4  77.4  78.4  79.4  80.4  81.4  82.4  83.4  84.4  85.4  86.4  87.4  88.4  89.4  90.4  91.4  92.4  93.4  94.4  95.4  96.4  97.4\n" +
                "          98.5  99.5 100.5 101.5 102.5 103.5 104.5 105.5 106.5 107.5 108.5 109.5 110.5 111.5 112.5 113.5 114.5 115.5 116.5 117.5 118.5 119.5 120.5 121.5\n" +
                "         122.6 123.6 124.6 125.6 126.6 127.6 128.6 129.6 130.6 131.6 132.6 133.6 134.6 135.6 136.6 137.6 138.6 139.6 140.6 141.6 142.6 143.6 144.6 145.6\n" +
                "         146.7 147.7 148.7 149.7 150.7 151.7 152.7 153.7 154.7 155.7 156.7 157.7 158.7 159.7 160.7 161.7 162.7 163.7 164.7 165.7 166.7 167.7 168.7 169.7\n" +
                "         170.8 171.8 172.8 173.8 174.8 175.8 176.8 177.8 178.8 179.8 180.8 181.8 182.8 183.8 184.8 185.8 186.8 187.8 188.8 189.8 190.8 191.8 192.8 193.8\n" +
                "         194.9 195.9 196.9 197.9 198.9 199.9 200.9 201.9 202.9 203.9 204.9 205.9 206.9 207.9 208.9 209.9 210.9 211.9 212.9 213.9 214.9 215.9 216.9 217.9\n" +
                "         218.0 219.0 220.0 221.0 222.0 223.0 224.0 225.0 226.0 227.0 228.0 229.0 230.0 231.0 232.0 233.0 234.0 235.0 236.0 237.0 238.0 239.0 240.0 241.0\n" +
                "         242.1 243.1 244.1 245.1 246.1 247.1 248.1 249.1 250.1 251.1 252.1 253.1 254.1 255.1 256.1 257.1 258.1 259.1 260.1 261.1 262.1 263.1 264.1 265.1\n" +
                "         266.2 267.2 268.2 269.2 270.2 271.2 272.2 273.2 274.2 275.2 276.2 277.2 278.2 279.2 280.2 281.2 282.2 283.2 284.2 285.2 286.2 287.2 288.2 289.2\n" +
                "         290.3 291.3 292.3 293.3 294.3 295.3 296.3 297.3 298.3 299.3 300.3 301.3 302.3 303.3 304.3 305.3 306.3 307.3 308.3 309.3 310.3 311.3 312.3 313.3\n" +
                "         314.4 315.4 316.4 317.4 318.4 319.4 320.4 321.4 322.4 323.4 324.4 325.4 326.4 327.4 328.4 329.4 330.4 331.4 332.4 333.4 334.4 335.4 336.4 337.4\n" +
                "         338.5 339.5 340.5 341.5 342.5 343.5 344.5 345.5 346.5 347.5 348.5 349.5 350.5 351.5 352.5 353.5 354.5 355.5 356.5 357.5 358.5 359.5 360.5 361.5\n" +
                "         362.6 363.6 364.6 365.6 366.6 367.6 368.6 369.6 370.6 371.6 372.6 373.6 374.6 375.6 376.6 377.6 378.6 379.6 380.6 381.6 382.6 383.6 384.6 385.6\n",
            updatedVeTableValue.getValue()
        );
        assertEquals("1", updatedVeTableValue.getDigits());
        assertEquals("16", updatedVeTableValue.getRows());
        assertEquals("24", updatedVeTableValue.getCols());

        final Constant updatedVeRpmBinsValue = result.getUpdatedTune().getConstantsAsMap().get(VE_RPM_BINS_FIELD_NAME);
        assertEquals(VE_RPM_BINS_FIELD_NAME, updatedVeRpmBinsValue.getName());
        assertNull(updatedVeRpmBinsValue.getUnits());
        assertEquals(
            "\n" +
                "         1.0\n" +
                "         2.0\n" +
                "         3.0\n" +
                "         4.0\n" +
                "         5.0\n" +
                "         6.0\n" +
                "         7.0\n" +
                "         8.0\n" +
                "         9.0\n" +
                "         10.0\n" +
                "         11.0\n" +
                "         12.0\n" +
                "         13.0\n" +
                "         14.0\n" +
                "         15.0\n" +
                "         16.0\n" +
                "         17.0\n" +
                "         18.0\n" +
                "         19.0\n" +
                "         20.0\n" +
                "         21.0\n" +
                "         22.0\n" +
                "         23.0\n" +
                "         24.0\n",
            updatedVeRpmBinsValue.getValue()
        );
        assertEquals("0", updatedVeRpmBinsValue.getDigits());
        assertEquals("24", updatedVeRpmBinsValue.getRows());
        assertEquals("1", updatedVeRpmBinsValue.getCols());
    }
}
