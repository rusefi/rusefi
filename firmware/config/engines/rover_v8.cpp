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
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "main.h"
#include "rover_v8.h"
#include "engine_math.h"
#include "allsensors.h"

void setFrankenstein_01_LCD(board_configuration_s *boardConfiguration) {
	boardConfiguration->HD44780_rs = GPIOE_9;
	boardConfiguration->HD44780_e = GPIOE_11;
	boardConfiguration->HD44780_db4 = GPIOE_13;
	boardConfiguration->HD44780_db5 = GPIOE_15;
	boardConfiguration->HD44780_db6 = GPIOB_11;
	boardConfiguration->HD44780_db7 = GPIOB_13;
}

EXTERN_ENGINE;

void setRoverv8(DECLARE_ENGINE_PARAMETER_F) {
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_36_1;

	engineConfiguration->specs.displacement = 3.528;
	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_4_3_6_5_7_2;

	// set_rpm_hard_limit 4000
	engineConfiguration->rpmHardLimit = 4000; // yes, 4k. let's play it safe for now
	// set_cranking_rpm 350
	engineConfiguration->cranking.rpm = 350;

	// set_whole_fuel_map 3
	setWholeFuelMap(3 PASS_ENGINE_PARAMETER);

	// set cranking_injection_mode 0
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	// set ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

    // set_ignition_channels
    boardConfiguration->ignitionPins[0] = GPIOE_8; // Frankenstein: low side - out #8
    boardConfiguration->ignitionPins[1] = GPIOE_10; // Frankenstein: low side - out #8
    boardConfiguration->ignitionPins[2] = GPIOE_12; // Frankenstein: low side - out #8
    boardConfiguration->ignitionPins[3] = GPIOE_14; // Frankenstein: low side - out #8

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

	boardConfiguration->injectionPins[0] = GPIOC_14; // Frankenstein: low side - out #1
	boardConfiguration->injectionPins[1] = GPIOC_15; // Frankenstein: low side - out #2
	boardConfiguration->injectionPins[2] = GPIOE_6; // Frankenstein: low side - out #3
	boardConfiguration->injectionPins[3] = GPIOC_13; // Frankenstein: low side - out #4
    boardConfiguration->injectionPins[4] = GPIOE_4; // Frankenstein: low side - out #5
    boardConfiguration->injectionPins[5] = GPIOE_5; // Frankenstein: low side - out #6
    boardConfiguration->injectionPins[6] = GPIOE_2; // Frankenstein: low side - out #7
    boardConfiguration->injectionPins[7] = GPIOE_3; // Frankenstein: low side - out #8

    boardConfiguration->fuelPumpPin = GPIOE_0; // Frankenstein: low side - out #9
    boardConfiguration->malfunctionIndicatorPin = GPIOE_1; // Frankenstein: low side - out #10
	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;

    boardConfiguration->triggerInputPins[0] = GPIOC_6; // 2G YEL/BLU
    boardConfiguration->triggerInputPins[1] = GPIOA_5; // 2E White CKP

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

    /* Stepper logic:
    boardConfiguration->idle.stepperDirectionPin = GPIOE_10;
    boardConfiguration->idle.stepperStepPin = GPIOE_12;
    engineConfiguration->stepperEnablePin = GPIOE_14;
    engineConfiguration->idleStepperReactionTime = 10;
    engineConfiguration->idleStepperTotalSteps = 150;
    */
    boardConfiguration->useStepperIdle = true;

	// set injection_pin_mode 0
	boardConfiguration->injectionPinMode = OM_DEFAULT;

	boardConfiguration->canTxPin = GPIOB_6;
	boardConfiguration->canRxPin = GPIOB_12;
	engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canReadEnabled = false;
	engineConfiguration->canNbcType = CAN_BUS_MAZDA_RX8;


    setAlgorithm(LM_SPEED_DENSITY PASS_ENGINE_PARAMETER);

    // todo: make this official Frankenstein joystick?
    boardConfiguration->joystickCenterPin = GPIOD_8;
    boardConfiguration->joystickAPin = GPIOD_10;
    boardConfiguration->joystickBPin = GPIO_UNASSIGNED;
    boardConfiguration->joystickCPin = GPIO_UNASSIGNED;
    boardConfiguration->joystickDPin = GPIOD_11;

}
