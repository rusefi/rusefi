package com.rusefi.maintenance;

import com.opensr5.ini.field.IniField;
import com.rusefi.core.Pair;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;
import java.util.List;

import static com.rusefi.maintenance.CalibrationsTestData.*;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class IniFieldsAnalyzerTest {
    private CalibrationsTestData testData;

    private TestCallbacks testCallbacks;
    private List<Pair<IniField, Constant>> valuesToUpdate;

    @BeforeEach
    public void setUp() throws JAXBException {
        testData = CalibrationsTestData.load();
        testCallbacks = new TestCallbacks();
        valuesToUpdate = IniFieldsAnalyzer.findValuesToUpdate(
            testData.getPrevIni(),
            testData.getPrevMsq(),
            testData.getUpdatedIni(),
            testData.getUpdatedMsq(),
            testCallbacks
        );
    }

    @Test
    public void testMax31855spiDevice() {
        checkValueToUpdateExist("max31855spiDevice", "\"Off\"", "\"SPI3\"");
    }

    @Test
    public void testIsEnabledSpi3() {
        checkValueToUpdateExist("is_enabled_spi_3", "\"false\"", "\"true\"");
    }

    @Test
    public void testMax31855Cs1() {
        checkValueToUpdateExist("max31855_cs1", "\"NONE\"", "\"PA15\"");
    }

    @Test
    public void testSpi3misoPin() {
        checkValueToUpdateExist("spi3misoPin", "\"NONE\"", "\"PC11\"");
    }

    @Test
    public void testSpi3sckPin() {
        checkValueToUpdateExist("spi3sckPin", "\"NONE\"", "\"PC10\"");
    }

    @Test
    public void testStepperDcIo1DirectionPin1() {
        checkValueToUpdateExist("stepperDcIo1_directionPin1", "\"NONE\"", "\"PB15\"");
    }

    @Test
    public void testStepperDcIo1ControlPin() {
        checkValueToUpdateExist("stepperDcIo1_controlPin", "\"NONE\"", "\"PC9\"");
    }

    @Test
    public void testStepperDcIo1DisablePin() {
        checkValueToUpdateExist("stepperDcIo1_disablePin", "\"NONE\"", "\"PA10\"");
    }

    @Test
    public void testStepperDcIo2DirectionPin1() {
        checkValueToUpdateExist("stepperDcIo2_directionPin1", "\"NONE\"", "\"PC8\"");
    }

    @Test
    public void testStepperDcIo2ControlPin() {
        checkValueToUpdateExist("stepperDcIo2_controlPin", "\"NONE\"", "\"PC7\"");
    }

    @Test
    public void testStepperDcIo2DisablePin() {
        checkValueToUpdateExist("stepperDcIo2_disablePin", "\"NONE\"", "\"PB14\"");
    }

    @Test
    public void testDebugMode() {
        checkValueToUpdateExist("debugMode", "\"MAP\"", "\"Executor\"");
    }

    @Test
    public void testMc33810maxDwellTimer() {
        checkValueToUpdateExist("mc33810maxDwellTimer", "\"2 mS\"", "\"8 mS\"");
    }

    @Test
    public void testCan2TxPin() {
        checkValueToUpdateExist("can2TxPin", "\"NONE\"", "\"PB13\"");
    }

    @Test
    public void testCan2RxPin() {
        checkValueToUpdateExist("can2RxPin", "\"NONE\"", "\"PB12\"");
    }

    @Test
    public void testIgnitionTable() {
        checkValueToUpdateExist(IGNITION_TABLE_FIELD_NAME, PREV_IGNITION_TABLE_VALUE, UPDATED_IGNITION_TABLE_VALUE);
    }

    @Test
    public void testWarnings() {
        assertEquals(
            "WARNING! Field `enableKnockSpectrogram` cannot be migrated because bit position is updated: `30` -> `26`\r\n" +
            "WARNING! Field `unusedOftenChangesDuringFirmwareUpdate` cannot be updated because its row count is updated: `198` -> `58`\r\n",
            testCallbacks.getContent()
        );
    }

    private void checkValueToUpdateExist(
        final String fieldName,
        final String expectedPrevFieldValue,
        final String expectedUpdatedFieldValue
    ) {
        boolean result = false;
        final Constant prevValue = testData.getPrevValue(fieldName);
        assertEquals(
            expectedPrevFieldValue,
            prevValue.getValue(),
            String.format("Unexpected prev value for `%s` field.", fieldName)
        );
        assertEquals(
            expectedUpdatedFieldValue,
            testData.getUpdatedValue(fieldName).getValue(),
            String.format("Unexpected updated value for `%s` field.", fieldName)
        );
        final IniField expectedField = testData.getUpdatedIni().getIniField(fieldName);
        for (final Pair<IniField, Constant> item: valuesToUpdate) {
            if (item.first.equals(expectedField) && item.second.equals(prevValue)) {
                result = true;
                break;
            }
        }
        assertTrue(result, String.format("Values to update must contain %s -> %s", expectedField, prevValue));
    }
}
