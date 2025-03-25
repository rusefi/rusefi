package com.rusefi.maintenance.migration.default_migration;

import com.rusefi.maintenance.DefaultTuneMigrator;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.tune.xml.Constant;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import javax.xml.bind.JAXBException;

import static com.rusefi.maintenance.migration.default_migration.DefaultTestTuneMigrationContext.*;
import static org.junit.jupiter.api.Assertions.*;

public class DefaultTuneMigratorTest {
    private TestTuneMigrationContext testContext;

    @BeforeEach
    public void setUp() throws JAXBException {
        testContext = DefaultTestTuneMigrationContext.load();
        DefaultTuneMigrator.INSTANCE.migrateTune(testContext);
    }

    @Test
    public void testVehicleName() {
        checkValueToUpdateExist(VEHICLE_NAME_FIELD_NAME, PREV_VEHICLE_NAME_VALUE, null);
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
    public void testContent() {
        assertEquals(
            "We aren't going to restore field `enableKnockSpectrogram`: it looks like its value is just renamed: `\"false\"` -> `\"no\"`\r\n",
            testContext.getTestCallbacks().getContent()
        );
    }

    private void checkValueToUpdateExist(
        final String fieldName,
        final String expectedPrevFieldValue,
        final String expectedUpdatedFieldValue
    ) {
        final Constant prevValue = testContext.getPrevValue(fieldName);
        assertEquals(
            expectedPrevFieldValue,
            prevValue.getValue(),
            String.format("Unexpected prev value for `%s` field.", fieldName)
        );
        final Constant updatedValue = testContext.getUpdatedValue(fieldName);
        if (expectedUpdatedFieldValue != null) {
            assertEquals(
                expectedUpdatedFieldValue,
                updatedValue.getValue(),
                String.format("Unexpected updated value for `%s` field.", fieldName)
            );
        } else {
            assertNull(updatedValue);
        }

        final Constant valueToUpdate = testContext.getMigratedConstants().get(fieldName);
        assertNotNull(valueToUpdate);
        assertEquals(prevValue, valueToUpdate);
    }
}
