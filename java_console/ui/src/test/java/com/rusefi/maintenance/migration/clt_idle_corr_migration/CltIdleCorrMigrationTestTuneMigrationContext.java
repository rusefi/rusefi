package com.rusefi.maintenance.migration.clt_idle_corr_migration;

import com.opensr5.ini.IniFileModel;
import com.opensr5.ini.field.ArrayIniField;
import com.opensr5.ini.field.ScalarIniField;
import com.rusefi.config.FieldType;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.tune.xml.Constant;

import jakarta.xml.bind.JAXBException;

import java.util.Map;

import static com.rusefi.maintenance.migration.migrators.CltIdleCorrMigrator.*;
import static org.junit.jupiter.api.Assertions.*;

public class CltIdleCorrMigrationTestTuneMigrationContext {
    public static final String TEST_PREV_CLT_IDLE_CORR_BINS_VALUE = "\n" +
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
        "         110.0\n" +
        "      ";
    public static TestTuneMigrationContext load() throws JAXBException {
        final TestTuneMigrationContext result = TestTuneMigrationContext.load(
            "src/test/java/com/rusefi/maintenance/migration/clt_idle_corr_migration/test_data"
        );
        {
            final IniFileModel prevIniFile = result.getPrevIniFile();
            {
                final ScalarIniField prevManIdlePositionIniField = (ScalarIniField) prevIniFile.getIniField(
                    MAN_IDLE_POSITION_FIELD_NAME
                );
                assertEquals("%", prevManIdlePositionIniField.getUnits());
                assertEquals(FieldType.FLOAT, prevManIdlePositionIniField.getType());
                assertEquals(44, prevManIdlePositionIniField.getOffset());
                assertEquals("0", prevManIdlePositionIniField.getDigits());
            }
            {
                final ArrayIniField prevCltIdleCorrBinsIniField = (ArrayIniField) prevIniFile.getIniField(
                    CLT_IDLE_CORR_BINS_FIELD_NAME
                );
                assertEquals(FieldType.FLOAT, prevCltIdleCorrBinsIniField.getType());
                assertEquals(14064, prevCltIdleCorrBinsIniField.getOffset());
                assertEquals(1, prevCltIdleCorrBinsIniField.getCols());
                assertEquals(16, prevCltIdleCorrBinsIniField.getRows());
                assertEquals(1, prevCltIdleCorrBinsIniField.getMultiplier());
                assertEquals("-100", prevCltIdleCorrBinsIniField.getMin());
                assertEquals("250", prevCltIdleCorrBinsIniField.getMax());
                assertEquals("2", prevCltIdleCorrBinsIniField.getDigits());
            }
            {
                final Map<String, Constant> prevTune = result.getPrevTune().getConstantsAsMap();
                {
                    final Constant prevManIdlePositionValue = prevTune.get(MAN_IDLE_POSITION_FIELD_NAME);
                    assertEquals(MAN_IDLE_POSITION_FIELD_NAME, prevManIdlePositionValue.getName());
                    assertEquals("%", prevManIdlePositionValue.getUnits());
                    assertEquals(
                        "50.0",
                        prevManIdlePositionValue.getValue()
                    );
                    assertEquals("0", prevManIdlePositionValue.getDigits());
                    assertNull(prevManIdlePositionValue.getRows());
                    assertNull(prevManIdlePositionValue.getCols());
                }
                {
                    final Constant prevCltIdleCorrBinsValue = prevTune.get(CLT_IDLE_CORR_BINS_FIELD_NAME);
                    assertEquals(CLT_IDLE_CORR_BINS_FIELD_NAME, prevCltIdleCorrBinsValue.getName());
                    assertEquals("C", prevCltIdleCorrBinsValue.getUnits());
                    assertEquals(TEST_PREV_CLT_IDLE_CORR_BINS_VALUE, prevCltIdleCorrBinsValue.getValue());
                    assertEquals("2", prevCltIdleCorrBinsValue.getDigits());
                    assertEquals("16", prevCltIdleCorrBinsValue.getRows());
                    assertEquals("1", prevCltIdleCorrBinsValue.getCols());
                }
                {
                    final Constant prevCltIdleCorrValue = prevTune.get(CLT_IDLE_CORR_FIELD_NAME);
                    assertEquals(CLT_IDLE_CORR_FIELD_NAME, prevCltIdleCorrValue.getName());
                    assertEquals("ratio", prevCltIdleCorrValue.getUnits());
                    assertEquals(
                        "\n" +
                            "         1.5\n" +
                            "         1.5\n" +
                            "         1.3333334\n" +
                            "         1.3333334\n" +
                            "         1.3333334\n" +
                            "         1.3333334\n" +
                            "         1.3333334\n" +
                            "         1.3333334\n" +
                            "         1.3333334\n" +
                            "         1.2333333\n" +
                            "         1.1666666\n" +
                            "         1.1\n" +
                            "         1.0\n" +
                            "         1.0\n" +
                            "         1.0\n" +
                            "         1.0\n" +
                            "      ",
                        prevCltIdleCorrValue.getValue()
                    );
                    assertEquals("2", prevCltIdleCorrValue.getDigits());
                    assertEquals("16", prevCltIdleCorrValue.getRows());
                    assertEquals("1", prevCltIdleCorrValue.getCols());
                }
            }
            {
                final ArrayIniField prevCltIdleCorrIniField = (ArrayIniField) prevIniFile.findIniField(
                    CLT_IDLE_CORR_FIELD_NAME
                ).get();
                assertEquals(FieldType.FLOAT, prevCltIdleCorrIniField.getType());
                assertEquals(14128, prevCltIdleCorrIniField.getOffset());
                assertEquals(1, prevCltIdleCorrIniField.getCols());
                assertEquals(16, prevCltIdleCorrIniField.getRows());
                assertEquals(1.0, prevCltIdleCorrIniField.getMultiplier());
                assertEquals("0", prevCltIdleCorrIniField.getMin());
                assertEquals("10", prevCltIdleCorrIniField.getMax());
                assertEquals("2", prevCltIdleCorrIniField.getDigits());
            }
        }
        {
            final IniFileModel updatedIniFile = result.getUpdatedIniFile();
            assertFalse(updatedIniFile.findIniField(MAN_IDLE_POSITION_FIELD_NAME).isPresent());
            {
                final ArrayIniField updatedCltIdleCorrBinsIniField = (ArrayIniField) updatedIniFile.findIniField(
                    CLT_IDLE_CORR_BINS_FIELD_NAME
                ).get();
                assertEquals(FieldType.FLOAT, updatedCltIdleCorrBinsIniField.getType());
                assertEquals(14048, updatedCltIdleCorrBinsIniField.getOffset());
                assertEquals(1, updatedCltIdleCorrBinsIniField.getCols());
                assertEquals(16, updatedCltIdleCorrBinsIniField.getRows());
                assertEquals(1, updatedCltIdleCorrBinsIniField.getMultiplier());
                assertEquals("-100", updatedCltIdleCorrBinsIniField.getMin());
                assertEquals("250", updatedCltIdleCorrBinsIniField.getMax());
                assertEquals("2", updatedCltIdleCorrBinsIniField.getDigits());
            }
            {
                final ArrayIniField prevCltIdleCorrIniField = (ArrayIniField) updatedIniFile.findIniField(
                    CLT_IDLE_CORR_FIELD_NAME
                ).get();
                assertEquals(FieldType.FLOAT, prevCltIdleCorrIniField.getType());
                assertEquals(14112, prevCltIdleCorrIniField.getOffset());
                assertEquals(1, prevCltIdleCorrIniField.getCols());
                assertEquals(16, prevCltIdleCorrIniField.getRows());
                assertEquals(1.0, prevCltIdleCorrIniField.getMultiplier());
                assertEquals("0", prevCltIdleCorrIniField.getMin());
                assertEquals("100", prevCltIdleCorrIniField.getMax());
                assertEquals("2", prevCltIdleCorrIniField.getDigits());
            }
        }
        {
            final Map<String, Constant> updatedTune = result.getUpdatedTune().getConstantsAsMap();
            assertNull(updatedTune.get(MAN_IDLE_POSITION_FIELD_NAME));
            {
                final Constant updatedCltIdleCorrBinsValue = updatedTune.get(CLT_IDLE_CORR_BINS_FIELD_NAME);
                assertNotNull(updatedCltIdleCorrBinsValue);
                assertEquals(CLT_IDLE_CORR_BINS_FIELD_NAME, updatedCltIdleCorrBinsValue.getName());
                assertEquals("C", updatedCltIdleCorrBinsValue.getUnits());
                assertEquals(
                    "\n" +
                        "         -40.1\n" +
                        "         -30.1\n" +
                        "         -20.1\n" +
                        "         -10.1\n" +
                        "         0.1\n" +
                        "         10.1\n" +
                        "         20.1\n" +
                        "         30.1\n" +
                        "         40.1\n" +
                        "         50.1\n" +
                        "         60.1\n" +
                        "         70.1\n" +
                        "         80.1\n" +
                        "         90.1\n" +
                        "         100.1\n" +
                        "         110.1\n" +
                        "      ",
                    updatedCltIdleCorrBinsValue.getValue()
                );
                assertEquals("2", updatedCltIdleCorrBinsValue.getDigits());
                assertEquals("16", updatedCltIdleCorrBinsValue.getRows());
                assertEquals("1", updatedCltIdleCorrBinsValue.getCols());
            }
            {
                final Constant updatedCltIdleCorrValue = updatedTune.get(CLT_IDLE_CORR_FIELD_NAME);
                assertEquals(CLT_IDLE_CORR_FIELD_NAME, updatedCltIdleCorrValue.getName());
                assertEquals("%", updatedCltIdleCorrValue.getUnits());
                assertEquals(
                    "\n" +
                        "         75.1\n" +
                        "         75.1\n" +
                        "         70.1\n" +
                        "         70.1\n" +
                        "         70.1\n" +
                        "         65.1\n" +
                        "         65.1\n" +
                        "         65.1\n" +
                        "         60.1\n" +
                        "         60.1\n" +
                        "         60.1\n" +
                        "         55.1\n" +
                        "         55.1\n" +
                        "         55.1\n" +
                        "         50.1\n" +
                        "         50.1\n" +
                        "      ",
                    updatedCltIdleCorrValue.getValue()
                );
                assertEquals("2", updatedCltIdleCorrValue.getDigits());
                assertEquals("16", updatedCltIdleCorrValue.getRows());
                assertEquals("1", updatedCltIdleCorrValue.getCols());
            }
        }
        return result;
    }
}

