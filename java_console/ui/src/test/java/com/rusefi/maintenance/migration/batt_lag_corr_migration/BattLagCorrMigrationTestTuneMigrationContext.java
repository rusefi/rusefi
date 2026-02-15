package com.rusefi.maintenance.migration.batt_lag_corr_migration;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import jakarta.xml.bind.JAXBException;

import java.util.Map;

import static com.rusefi.maintenance.migration.migrators.BattLagCorrExtensionMigrator.*;
import static org.junit.jupiter.api.Assertions.*;

public class BattLagCorrMigrationTestTuneMigrationContext {
    public static String INJECTOR_BATT_LAG_CORR_BINS_TEST_VALUE = "\n" +
        "         6.0\n" +
        "         8.0\n" +
        "         10.0\n" +
        "         11.0\n" +
        "         12.0\n" +
        "         13.0\n" +
        "         14.0\n" +
        "         15.0\n" +
        "      ";

    public static TestTuneMigrationContext load() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/batt_lag_corr_migration/test_data"
        );
        {
            final IniFileModel prevIniFile = result.getPrevIniFile();
            {
                final ArrayIniField prevInjectorBattLagCorrBinsIniField = (ArrayIniField) prevIniFile.getIniField(
                    INJECTOR_BATT_LAG_CORR_BINS_FIELD_NAME
                );
                assertEquals(FieldType.FLOAT, prevInjectorBattLagCorrBinsIniField.getType());
                assertEquals(56, prevInjectorBattLagCorrBinsIniField.getOffset());
                assertEquals(1, prevInjectorBattLagCorrBinsIniField.getCols());
                assertEquals(8, prevInjectorBattLagCorrBinsIniField.getRows());
                assertEquals(1, prevInjectorBattLagCorrBinsIniField.getMultiplier());
                assertEquals("0", prevInjectorBattLagCorrBinsIniField.getMin());
                assertEquals("20", prevInjectorBattLagCorrBinsIniField.getMax());
                assertEquals("2", prevInjectorBattLagCorrBinsIniField.getDigits());
            }
            {
                final ArrayIniField prevInjectorBattLagCorrIniField = (ArrayIniField) prevIniFile.getIniField(
                    INJECTOR_BATT_LAG_CORR_FIELD_NAME
                );
                assertEquals(FieldType.FLOAT, prevInjectorBattLagCorrIniField.getType());
                assertEquals(88, prevInjectorBattLagCorrIniField.getOffset());
                assertEquals(1, prevInjectorBattLagCorrIniField.getCols());
                assertEquals(8, prevInjectorBattLagCorrIniField.getRows());
                assertEquals(1.0, prevInjectorBattLagCorrIniField.getMultiplier());
                assertEquals("0", prevInjectorBattLagCorrIniField.getMin());
                assertEquals("50", prevInjectorBattLagCorrIniField.getMax());
                assertEquals("2", prevInjectorBattLagCorrIniField.getDigits());
            }

            assertFalse(prevIniFile.findIniField(INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME).isPresent());
            assertFalse(prevIniFile.findIniField(INJECTOR_BATT_LAG_CORR_PRESS_BINS_FIELD_NAME).isPresent());
            assertFalse(prevIniFile.findIniField(INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME).isPresent());
        }
        {
            final Map<String, Constant> prevTune = result.getPrevTune().getConstantsAsMap();
            {
                final Constant prevInjectorBattLagCorrBinsValue = prevTune.get(INJECTOR_BATT_LAG_CORR_BINS_FIELD_NAME);
                assertEquals(INJECTOR_BATT_LAG_CORR_BINS_FIELD_NAME, prevInjectorBattLagCorrBinsValue.getName());
                assertEquals("volts", prevInjectorBattLagCorrBinsValue.getUnits());
                assertEquals(
                    INJECTOR_BATT_LAG_CORR_BINS_TEST_VALUE,
                    prevInjectorBattLagCorrBinsValue.getValue()
                );
                assertEquals("2", prevInjectorBattLagCorrBinsValue.getDigits());
                assertEquals("8", prevInjectorBattLagCorrBinsValue.getRows());
                assertEquals("1", prevInjectorBattLagCorrBinsValue.getCols());
            }
            {
                final Constant prevInjectorBattLagCorrValue = prevTune.get(INJECTOR_BATT_LAG_CORR_FIELD_NAME);
                assertEquals(INJECTOR_BATT_LAG_CORR_FIELD_NAME, prevInjectorBattLagCorrValue.getName());
                assertEquals("ms", prevInjectorBattLagCorrValue.getUnits());
                assertEquals(
                    "\n" +
                        "         3.371\n" +
                        "         1.974\n" +
                        "         1.383\n" +
                        "         1.194\n" +
                        "         1.04\n" +
                        "         0.914\n" +
                        "         0.797\n" +
                        "         0.726\n" +
                        "      ",
                    prevInjectorBattLagCorrValue.getValue()
                );
                assertEquals("2", prevInjectorBattLagCorrValue.getDigits());
                assertEquals("8", prevInjectorBattLagCorrValue.getRows());
                assertEquals("1", prevInjectorBattLagCorrValue.getCols());
            }

            assertFalse(prevTune.containsKey(INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME));
            assertFalse(prevTune.containsKey(INJECTOR_BATT_LAG_CORR_PRESS_BINS_FIELD_NAME));
            assertFalse(prevTune.containsKey(INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME));
        }
        {
            final IniFileModel updatedIniFile = result.getUpdatedIniFile();
            {
                final ArrayIniField updatedInjectorBattLagCorrBattBinsIniField = (ArrayIniField) updatedIniFile.getIniField(
                    INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME
                );
                assertEquals(FieldType.INT16, updatedInjectorBattLagCorrBattBinsIniField.getType());
                assertEquals(52, updatedInjectorBattLagCorrBattBinsIniField.getOffset());
                assertEquals(1, updatedInjectorBattLagCorrBattBinsIniField.getCols());
                assertEquals(8, updatedInjectorBattLagCorrBattBinsIniField.getRows());
                assertEquals(0.01, updatedInjectorBattLagCorrBattBinsIniField.getMultiplier());
                assertEquals("0", updatedInjectorBattLagCorrBattBinsIniField.getMin());
                assertEquals("20", updatedInjectorBattLagCorrBattBinsIniField.getMax());
                assertEquals("2", updatedInjectorBattLagCorrBattBinsIniField.getDigits());
            }
            {
                final ArrayIniField updatedInjectorBattLagPressBattBinsIniField = (ArrayIniField) updatedIniFile.getIniField(
                    INJECTOR_BATT_LAG_CORR_PRESS_BINS_FIELD_NAME
                );
                assertEquals(FieldType.INT, updatedInjectorBattLagPressBattBinsIniField.getType());
                assertEquals(68, updatedInjectorBattLagPressBattBinsIniField.getOffset());
                assertEquals(1, updatedInjectorBattLagPressBattBinsIniField.getCols());
                assertEquals(2, updatedInjectorBattLagPressBattBinsIniField.getRows());
                assertEquals(0.1, updatedInjectorBattLagPressBattBinsIniField.getMultiplier());
                assertEquals("0", updatedInjectorBattLagPressBattBinsIniField.getMin());
                assertEquals("30000", updatedInjectorBattLagPressBattBinsIniField.getMax());
                assertEquals("2", updatedInjectorBattLagPressBattBinsIniField.getDigits());
            }
            {
                final ArrayIniField updatedInjectorBattLagCorrTableIniField = (ArrayIniField) updatedIniFile.getIniField(
                    INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME
                );
                assertEquals(FieldType.INT16, updatedInjectorBattLagCorrTableIniField.getType());
                assertEquals(76, updatedInjectorBattLagCorrTableIniField.getOffset());
                assertEquals(8, updatedInjectorBattLagCorrTableIniField.getCols());
                assertEquals(2, updatedInjectorBattLagCorrTableIniField.getRows());
                assertEquals(0.01, updatedInjectorBattLagCorrTableIniField.getMultiplier());
                assertEquals("0", updatedInjectorBattLagCorrTableIniField.getMin());
                assertEquals("50", updatedInjectorBattLagCorrTableIniField.getMax());
                assertEquals("2", updatedInjectorBattLagCorrTableIniField.getDigits());
            }
            assertFalse(updatedIniFile.findIniField(INJECTOR_BATT_LAG_CORR_BINS_FIELD_NAME).isPresent());
            assertFalse(updatedIniFile.findIniField(INJECTOR_BATT_LAG_CORR_FIELD_NAME).isPresent());
        }
        {
            final Map<String, Constant> updatedTune = result.getUpdatedTune().getConstantsAsMap();
            {
                final Constant updatedInjectorBattLagCorrBattBinsValue = updatedTune.get(
                    INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME
                );
                assertEquals(
                    INJECTOR_BATT_LAG_CORR_BATT_BINS_FIELD_NAME,
                    updatedInjectorBattLagCorrBattBinsValue.getName()
                );
                assertEquals("volts", updatedInjectorBattLagCorrBattBinsValue.getUnits());
                assertEquals(
                    "\n" +
                        "         6.1\n" +
                        "         8.1\n" +
                        "         10.1\n" +
                        "         11.1\n" +
                        "         12.1\n" +
                        "         13.1\n" +
                        "         14.1\n" +
                        "         15.1\n" +
                        "      ",
                    updatedInjectorBattLagCorrBattBinsValue.getValue()
                );
                assertEquals("2", updatedInjectorBattLagCorrBattBinsValue.getDigits());
                assertEquals("8", updatedInjectorBattLagCorrBattBinsValue.getRows());
                assertEquals("1", updatedInjectorBattLagCorrBattBinsValue.getCols());
            }
            {
                final Constant updatedInjectorBattLagCorrPressBinsValue = updatedTune.get(
                    INJECTOR_BATT_LAG_CORR_PRESS_BINS_FIELD_NAME
                );
                assertEquals(
                    INJECTOR_BATT_LAG_CORR_PRESS_BINS_FIELD_NAME,
                    updatedInjectorBattLagCorrPressBinsValue.getName()
                );
                assertEquals("kPa", updatedInjectorBattLagCorrPressBinsValue.getUnits());
                assertEquals(
                    "\n" +
                        "         206.8\n" +
                        "         413.7\n" +
                        "      ",
                    updatedInjectorBattLagCorrPressBinsValue.getValue()
                );
                assertEquals("2", updatedInjectorBattLagCorrPressBinsValue.getDigits());
                assertEquals("2", updatedInjectorBattLagCorrPressBinsValue.getRows());
                assertEquals("1", updatedInjectorBattLagCorrPressBinsValue.getCols());
            }
            {
                final Constant updatedInjectorBattLagCorrTableValue = updatedTune.get(
                    INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME
                );
                assertEquals(INJECTOR_BATT_LAG_CORR_TABLE_FIELD_NAME, updatedInjectorBattLagCorrTableValue.getName());
                assertEquals("ms", updatedInjectorBattLagCorrTableValue.getUnits());
                assertEquals(
                    "\n" +
                        "         4.24 2.48 1.74 1.5 1.31 1.15 0.96 0.91\n" +
                        "         3.08 1.64 1.15 1.19 0.99 0.76 0.64 0.6\n" +
                        "      ",
                    updatedInjectorBattLagCorrTableValue.getValue()
                );
                assertEquals("2", updatedInjectorBattLagCorrTableValue.getDigits());
                assertEquals("2", updatedInjectorBattLagCorrTableValue.getRows());
                assertEquals("8", updatedInjectorBattLagCorrTableValue.getCols());
            }

            assertFalse(updatedTune.containsKey(INJECTOR_BATT_LAG_CORR_BINS_FIELD_NAME));
            assertFalse(updatedTune.containsKey(INJECTOR_BATT_LAG_CORR_FIELD_NAME));
        }

        return result;
    }
}

