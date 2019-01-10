/**
 * @file	rover_v8.cpp
 *
 * V8, firing order 18436572
 *
 * This config overrides some values of the default configuration which is set by setDefaultConfiguration() method
 *
 * set engine_type 10
 *
 * ROVER_V8 = 10
 *
 * @date Jun 27, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "rover_v8.h"
#include "engine_math.h"
#include "allsensors.h"

EXTERN_ENGINE;

void setFrankenstein_01_LCD(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	CONFIGB(HD44780_rs) = GPIOE_9;
	CONFIGB(HD44780_e) = GPIOE_11;
	CONFIGB(HD44780_db4) = GPIOE_13;
	CONFIGB(HD44780_db5) = GPIOE_15;
	CONFIGB(HD44780_db6) = GPIOB_11;
	CONFIGB(HD44780_db7) = GPIOB_13;
}

void setRoverv8(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	// set trigger_type 9
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_36_1;

	CONFIGB(is_enabled_spi_2) = false;
	CONFIGB(isHip9011Enabled) = false;
	setFrankenstein_01_LCD(PASS_ENGINE_PARAMETER_SIGNATURE);

	engineConfiguration->specs.displacement = 3.528;
	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_4_3_6_5_7_2;

	// set_rpm_hard_limit 4000
	engineConfiguration->rpmHardLimit = 4000; // yes, 4k. let's play it safe for now
	// set_cranking_rpm 350
	engineConfiguration->cranking.rpm = 350;

	// set_whole_fuel_map 3
	setWholeFuelMap(3 PASS_ENGINE_PARAMETER_SUFFIX);

	// set cranking_injection_mode 0
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	// set ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	CONFIGB(ignitionPinMode) = OM_INVERTED;

    // set_ignition_channels
    CONFIGB(ignitionPins)[0] = GPIOE_8; // Frankenstein: low side - out #x (?)
    CONFIGB(ignitionPins)[7] = GPIOE_10; // Frankenstein: low side - out #x (?)
    CONFIGB(ignitionPins)[3] = GPIOE_12; // Frankenstein: low side - out #x (?)
    CONFIGB(ignitionPins)[2] = GPIOE_14; // Frankenstein: low side - out #x (?)

	// Frankenstein: low side - out #1: PC14
	// Frankenstein: low side - out #2: PC15
	// Frankenstein: low side - out #3: PE6
	// Frankenstein: low side - out #4: PC13
	// Frankenstein: low side - out #5: PE4
	// Frankenstein: low side - out #6: PE5
	// Frankenstein: low side - out #7: PE2
	// Frankenstein: low side - out #8: PE3
	// Frankenstein: low side - out #9: PE0
	// Frankenstein: low side - out #10: PE1
	// Frankenstein: low side - out #11: PB8
	// Frankenstein: low side - out #12: PB9

	CONFIGB(injectionPins)[0] = GPIOC_14; // Frankenstein: low side - out #1
	CONFIGB(injectionPins)[1] = GPIOC_15; // Frankenstein: low side - out #2
	CONFIGB(injectionPins)[2] = GPIOE_6; // Frankenstein: low side - out #3
	CONFIGB(injectionPins)[3] = GPIOC_13; // Frankenstein: low side - out #4
    CONFIGB(injectionPins)[4] = GPIOE_4; // Frankenstein: low side - out #5
    CONFIGB(injectionPins)[5] = GPIOE_5; // Frankenstein: low side - out #6
    CONFIGB(injectionPins)[6] = GPIOE_2; // Frankenstein: low side - out #7
    CONFIGB(injectionPins)[7] = GPIOE_3; // Frankenstein: low side - out #8

// not valid ICU pin	CONFIGB(vehicleSpeedSensorInputPin) = GPIOC_2;

	//GPIOE_0 AND GPIOE_1 are bad pins since they conflict with accelerometer
	//no malfunction indicator pin needed, since we use CAN_BUS_MAZDA_RX8
	//CONFIGB(fuelPumpPin) = GPIOE_0; // Frankenstein: low side - out #9
	//CONFIGB(malfunctionIndicatorPin) = GPIOE_1; // Frankenstein: low side - out #10
	CONFIGB(fuelPumpPin) = GPIOB_8; // Frankenstein: low side - out #11
	CONFIGB(fuelPumpPinMode) = OM_DEFAULT;
	CONFIGB(mainRelayPin)  = GPIOB_9; // Frankenstein: low side - out #12

    CONFIGB(triggerInputPins)[0] = GPIOC_6; // 2G YEL/BLU
    CONFIGB(triggerInputPins)[1] = GPIOA_5; // 2E White CKP

    setCommonNTCSensor(&engineConfiguration->clt);
    engineConfiguration->clt.config.bias_resistor = 2700;
    setCommonNTCSensor(&engineConfiguration->iat);
    engineConfiguration->iat.config.bias_resistor = 2700;

    engineConfiguration->tpsAdcChannel = EFI_ADC_3; //Frankenstein: inp2
    engineConfiguration->vbattAdcChannel = EFI_ADC_0; //Frankenstein: inp5
    engineConfiguration->clt.adcChannel = EFI_ADC_11;
    engineConfiguration->iat.adcChannel = EFI_ADC_13;

    setCommonNTCSensor(&engineConfiguration->clt);
    engineConfiguration->clt.config.bias_resistor = 2700;
    setCommonNTCSensor(&engineConfiguration->iat);
    engineConfiguration->iat.config.bias_resistor = 2700;

    /**
     * TPS 0% 0.9v
     * TPS 100% 2.34v
     */
    engineConfiguration->tpsMin = convertVoltageTo10bitADC(1.250);
    engineConfiguration->tpsMax = convertVoltageTo10bitADC(4.538);

    // Stepper logic:
    CONFIGB(idle).stepperDirectionPin = GPIOB_10;
    CONFIGB(idle).stepperStepPin = GPIOB_15;
    engineConfiguration->stepperEnablePin = GPIOB_14;
    engineConfiguration->idleStepperReactionTime = 10;
    engineConfiguration->idleStepperTotalSteps = 150;

    CONFIGB(useStepperIdle) = false;

	// set injection_pin_mode 0
	CONFIGB(injectionPinMode) = OM_DEFAULT;

	CONFIGB(canTxPin) = GPIOB_6;
	CONFIGB(canRxPin) = GPIOB_12;
	engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canReadEnabled = false;
	engineConfiguration->canNbcType = CAN_BUS_MAZDA_RX8;


    setAlgorithm(LM_SPEED_DENSITY PASS_ENGINE_PARAMETER_SUFFIX);

    // todo: make this official Frankenstein joystick?
    CONFIGB(joystickCenterPin) = GPIOD_8;
    CONFIGB(joystickAPin) = GPIOD_10;
    CONFIGB(joystickBPin) = GPIO_UNASSIGNED;
    CONFIGB(joystickCPin) = GPIO_UNASSIGNED;
    CONFIGB(joystickDPin) = GPIOD_11;

}
