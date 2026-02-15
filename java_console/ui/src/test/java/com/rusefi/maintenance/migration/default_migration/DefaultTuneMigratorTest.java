package com.rusefi.maintenance.migration.default_migration;

import static com.rusefi.maintenance.migration.default_migration.CalibrationsTestHelpers.checkField;
import static com.rusefi.maintenance.migration.default_migration.DefaultTestTuneMigrationContext.*;
import static com.rusefi.maintenance.migration.migrators.TableAddColumnsMigrator.VE_RPM_BINS_FIELD_NAME;
import static com.rusefi.maintenance.migration.migrators.TableAddColumnsMigrator.VE_TABLE_FIELD_NAME;
import static java.util.Collections.emptySet;
import static org.junit.jupiter.api.Assertions.*;

import java.util.Optional;

import jakarta.xml.bind.JAXBException;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import com.rusefi.maintenance.CalibrationsHelper;
import com.rusefi.maintenance.CalibrationsInfo;
import com.rusefi.maintenance.TestTuneMigrationContext;
import com.rusefi.maintenance.migration.migrators.ComposedTuneMigrator;
import com.rusefi.tune.xml.Constant;

public class DefaultTuneMigratorTest {
    private TestTuneMigrationContext testContext;

    @BeforeEach
    public void setUp() throws JAXBException {
        testContext = DefaultTestTuneMigrationContext.load();
        ComposedTuneMigrator.INSTANCE.migrateTune(testContext);
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
        checkValueToUpdateExist("is_enabled_spi_3", "\"false\"", "\"yes\"");
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
    	 final Optional<CalibrationsInfo> result = CalibrationsHelper.mergeCalibrations(
                 testContext.getPrevIniFile(),
                 testContext.getPrevTune(),
                 testContext.getUpdatedCalibrationsInfo(),
                 testContext.getCallbacks(),
                 emptySet()
             );

             assertTrue(result.isPresent());

        checkField(testContext, result.get(), "spi3sckPin", "\"NONE\"", "\"PC10\"");
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
    public void testVeRpmBins() {
        checkValueToUpdateExist(VE_RPM_BINS_FIELD_NAME, PREV_VE_RPM_BINS_VALUE, UPDATED_VE_RPM_BINS_VALUE);
    }

    @Test
    public void testVeTable() {
        checkValueToUpdateExist(VE_TABLE_FIELD_NAME, PREV_VE_TABLE_VALUE, UPDATED_VE_TABLE_VALUE);
    }

    @Test
    public void testLambdaLoadBins() {
        checkValueToUpdateExist(
            "lambdaLoadBins",
            "\n" +
                "         31.0\n" +
                "         41.0\n" +
                "         51.0\n" +
                "         61.0\n" +
                "         71.0\n" +
                "         81.0\n" +
                "         91.0\n" +
                "         101.0\n" +
                "         111.0\n" +
                "         121.0\n" +
                "         131.0\n" +
                "         151.0\n" +
                "         171.0\n" +
                "         201.0\n" +
                "         221.0\n" +
                "         251.0\n",
            "\n" +
                "         30.0\n" +
                "         40.0\n" +
                "         50.0\n" +
                "         60.0\n" +
                "         70.0\n" +
                "         80.0\n" +
                "         90.0\n" +
                "         100.0\n" +
                "         110.0\n" +
                "         120.0\n" +
                "         130.0\n" +
                "         150.0\n" +
                "         175.0\n" +
                "         200.0\n" +
                "         225.0\n" +
                "         250.0\n"
        );
    }

    @Test
    public void testThrottlePedalUpVoltage() {
        checkValueToUpdateExist("throttlePedalUpVoltage", "1.7", "0.0");
    }

    @Test
    public void testCltCrankingCorr() {
        checkValueToUpdateExist(
            "cltCrankingCorr",
            "\n" +
                "         1.0\n" +
                "         1.1\n" +
                "         1.2\n" +
                "         1.3\n" +
                "         1.4\n" +
                "         1.5\n" +
                "         1.6\n" +
                "         1.7\n",
            "\n" +
                "         1.0\n" +
                "         1.0\n" +
                "         1.0\n" +
                "         1.0\n" +
                "         1.0\n" +
                "         1.0\n" +
                "         1.0\n" +
                "         1.0\n"
        );
    }

    @Test
    public void testFanOffTemperature() {
        checkValueToUpdateExist("fanOffTemperature", "98.0", "88.0");
    }

    @Test
    public void testContent() {
        assertEquals(
            "WARNING! Type of `map_samplingAngleBins` ini-field is expected to be `UINT16` instead of `FLOAT`\r\n" +
                "WARNING! Type of `map_samplingAngleBins` ini-field is expected to be `UINT16` instead of `FLOAT`\r\n" +
                "We aren't going to restore field `auxSerialRxPin`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `auxSerialSpeed`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `auxSerialTxPin`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `boardUse2stepPullDown`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `boardUseCamPullDown`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `boardUseCamVrPullUp`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `boardUseCrankPullUp`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `boardUseD2PullDown`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `boardUseD3PullDown`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `boardUseD4PullDown`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `boardUseD5PullDown`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `boardUseTachPullUp`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `boardUseTempPullUp`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `boostTpsBins`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `communityCommsLedPid`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `crankingCycleCoef`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `enableInnovateLC2`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `enableKnockSpectrogram`: it looks like its value is just renamed: `\"false\"` -> `\"no\"`\r\n" +
                "We aren't going to restore field `etb1configured`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `etb2configured`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `etbExpAverageLength`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `etbJamIntegratorLimit`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `etbRocExpAverageLength`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `isManualSpinningMode`: it is missed in new .ini file\r\n"+
                "We aren't going to restore field `kickStartCranking`: it looks like its value is just renamed: `\"false\"` -> `\"no\"`\r\n" +
                "We aren't going to restore field `knockBandCustom`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `mapAveragingSchedulingAtIndex`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `noAccelAfterHardLimitPeriodSecs`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `oddFireEngine`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `pauseEtbControl`: it looks like its value is just renamed: `\"false\"` -> `\"no\"`\r\n" +
                "We aren't going to restore field `showHumanReadableWarning`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `skipADC12bitAssert`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `skipBoardCanDash`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `targetVBatt`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `test557pin`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `throttlePedalUpPin`: it is missed in new .ini file\r\n" +
                "We aren't going to restore field `throttlePedalUpPinMode`: it is missed in new .ini file\r\n",
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
        final Constant expectedValueToUpdate = updatedValue != null ?
            updatedValue.cloneWithValue(prevValue.getValue()) :
            prevValue;
        assertEquals(expectedValueToUpdate.getName(), valueToUpdate.getName());
    }
}

