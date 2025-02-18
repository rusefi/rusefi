package com.rusefi.maintenance;

import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;

import java.util.Map;
import java.util.Optional;

import static com.rusefi.maintenance.CalibrationsTestData.*;
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

public class CalibrationsHelperTest {
    CalibrationsTestData testData;

    TestCallbacks testCallbacks;

    CalibrationsInfo mergedCalibrations;

    @BeforeEach
    public void setUp() throws JAXBException {
        testData = CalibrationsTestData.load();
        testCallbacks = new TestCallbacks();
        final Optional<CalibrationsInfo> result = CalibrationsHelper.mergeCalibrations(
            testData.getPrevCalibrationsInfo(),
            testData.getUpdatedCalibrationsInfo(),
            testCallbacks
        );
        assertTrue(result.isPresent());
        mergedCalibrations = result.get();
    }

    @Test
    public void testMax31855spiDevice() {
        checkField("max31855spiDevice", "\"Off\"", "\"SPI3\"");
    }

    @Test
    public void testIsEnabledSpi3() {
        checkField("is_enabled_spi_3", "\"false\"", "\"true\"");
    }

    @Test
    public void testMax31855Cs1() {
        checkField("max31855_cs1", "\"NONE\"", "\"PA15\"");
    }

    @Test
    public void testSpi3misoPin() {
        checkField("spi3misoPin", "\"NONE\"", "\"PC11\"");
    }

    @Test
    public void testSpi3sckPin() {
        checkField("spi3sckPin", "\"NONE\"", "\"PC10\"");
    }

    @Test
    public void testStepperDcIo1DirectionPin1() {
        checkField("stepperDcIo1_directionPin1", "\"NONE\"", "\"PB15\"");
    }

    @Test
    public void testStepperDcIo1ControlPin() {
        checkField("stepperDcIo1_controlPin", "\"NONE\"", "\"PC9\"");
    }

    @Test
    public void testStepperDcIo1DisablePin() {
        checkField("stepperDcIo1_disablePin", "\"NONE\"", "\"PA10\"");
    }

    @Test
    public void testStepperDcIo2DirectionPin1() {
        checkField("stepperDcIo2_directionPin1", "\"NONE\"", "\"PC8\"");
    }

    @Test
    public void testStepperDcIo2ControlPin() {
        checkField("stepperDcIo2_controlPin", "\"NONE\"", "\"PC7\"");
    }

    @Test
    public void testStepperDcIo2DisablePin() {
        checkField("stepperDcIo2_disablePin", "\"NONE\"", "\"PB14\"");
    }

    @Test
    public void testDebugMode() {
        checkField("debugMode", "\"MAP\"", "\"Executor\"");
    }

    @Test
    public void testMc33810maxDwellTimer() {
        checkField("mc33810maxDwellTimer", "\"2 mS\"", "\"8 mS\"");
    }

    @Test
    public void testCan2TxPin() {
        checkField("can2TxPin", "\"NONE\"", "\"PB13\"");
    }

    @Test
    public void testCan2RxPin() {
        checkField("can2RxPin", "\"NONE\"", "\"PB12\"");
    }

    @Test
    public void testAuxTempSensor1BiasResistor() {
        checkField("auxTempSensor1_bias_resistor", "0.0", "4700.0");
    }

    @Test
    public void testAuxTempSensor2BiasResistor() {
        checkField("auxTempSensor2_bias_resistor", "0.0", "4700.0");
    }

    @Test
    public void testIgnitionTable() {
        checkField(IGNITION_TABLE_FIELD_NAME, PREV_IGNITION_TABLE_VALUE, UPDATED_IGNITION_TABLE_VALUE);
    }

    private void checkField(final String fieldName, final String expectedPrevValue, final String expectedUpdatedValue) {
        assertEquals(
            expectedPrevValue,
            testData.getPrevValue(fieldName).getValue(),
            String.format("Unexpected prev `%s` field value", fieldName)
        );
        assertEquals(
            expectedUpdatedValue,
            testData.getUpdatedValue(fieldName).getValue(),
            String.format("Unexpected updated `%s` field value", fieldName)
        );
        final Map<String, Constant> mergedConstants = mergedCalibrations.generateMsq().getConstantsAsMap();
        assertEquals(
            expectedPrevValue,
            mergedConstants.get(fieldName).getValue(),
            String.format("Unexpected merged `%s` field value", fieldName)
        );
    }
}
