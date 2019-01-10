/**
 * @file	custom_engine.cpp
 *
 *
 * set engine_type 49
 * FRANKENSO_QA_ENGINE
 * See also DEFAULT_ENGINE_TYPE
 * Frankenso QA 12 cylinder engine
 *
 * @date Jan 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */
#ifndef CONFIG_ENGINES_CUSTOM_ENGINE_CPP_
#define CONFIG_ENGINES_CUSTOM_ENGINE_CPP_

#include "custom_engine.h"
#include "honda_accord.h"
#include "allsensors.h"
#include "engine_math.h"

#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "can_hw.h"
#include "scheduler.h"
#endif /* EFI_PROD_CODE */

EXTERN_ENGINE;


#if EFI_PROD_CODE || defined(__DOXYGEN__)
static int periodIndex = 0;

static OutputPin testPin;
scheduling_s scheduling;

static int test557[] = {5, 5, 10, 10, 20, 20, 50, 50, 100, 100, 200, 200, 500, 500, 500, 500};
#define TEST_LEN 16

efitimeus_t testTime;

static void toggleTestAndScheduleNext() {
	testPin.toggle();
	periodIndex = (periodIndex + 1) % TEST_LEN;
	testTime += test557[periodIndex];
	engine->executor.scheduleByTimestamp(&scheduling, testTime, (schfunc_t) &toggleTestAndScheduleNext, NULL);

}

/**
 * https://github.com/rusefi/rusefi/issues/557 common rail / direct injection scheduling control test
 */
void test557init(void) {
	if (engineConfiguration->test557pin == GPIO_UNASSIGNED ||
			engineConfiguration->test557pin == GPIOA_0) {
		return;
	}

	testPin.initPin("test", engineConfiguration->test557pin);
	testPin.setValue(0);
	testTime = getTimeNowUs();
	toggleTestAndScheduleNext();

}
#endif /* EFI_PROD_CODE */

void setFrankenso_01_LCD(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	CONFIGB(HD44780_rs) = GPIOE_7;
	CONFIGB(HD44780_e) = GPIOE_9;
	CONFIGB(HD44780_db4) = GPIOE_11;
	CONFIGB(HD44780_db5) = GPIOE_13;
	CONFIGB(HD44780_db6) = GPIOE_15;
	CONFIGB(HD44780_db7) = GPIOB_10;
}

void disableLCD(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	CONFIGB(HD44780_rs) = GPIO_UNASSIGNED;
	CONFIGB(HD44780_e) = GPIO_UNASSIGNED;
	CONFIGB(HD44780_db4) = GPIO_UNASSIGNED;
	CONFIGB(HD44780_db5) = GPIO_UNASSIGNED;
	CONFIGB(HD44780_db6) = GPIO_UNASSIGNED;
	CONFIGB(HD44780_db7) = GPIO_UNASSIGNED;
}

// todo: should this be renamed to 'setFrankensoConfiguration'?
// todo: should this be part of more default configurations?
void setCustomEngineConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_ONE_PLUS_ONE;

	setFrankenso_01_LCD(PASS_ENGINE_PARAMETER_SIGNATURE);
	commonFrankensoAnalogInputs(PASS_ENGINE_PARAMETER_SIGNATURE);
	setFrankenso0_1_joystick(PASS_ENGINE_PARAMETER_SIGNATURE);

	/**
	 * Frankenso analog #1 PC2 ADC12 CLT
	 * Frankenso analog #2 PC1 ADC11 IAT
	 * Frankenso analog #3 PA0 ADC0 MAP
	 * Frankenso analog #4 PC3 ADC13 WBO / O2
	 * Frankenso analog #5 PA2 ADC2 TPS
	 * Frankenso analog #6 PA1 ADC1
	 * Frankenso analog #7 PA4 ADC4
	 * Frankenso analog #8 PA3 ADC3
	 * Frankenso analog #9 PA7 ADC7
	 * Frankenso analog #10 PA6 ADC6
	 * Frankenso analog #11 PC5 ADC15
	 * Frankenso analog #12 PC4 ADC14 VBatt
	 */
	engineConfiguration->tpsAdcChannel = EFI_ADC_2; // PA2

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0;

	engineConfiguration->clt.adcChannel = EFI_ADC_12;
	engineConfiguration->iat.adcChannel = EFI_ADC_11;
	engineConfiguration->afr.hwChannel = EFI_ADC_13;

	setCommonNTCSensor(&engineConfiguration->clt);
	engineConfiguration->clt.config.bias_resistor = 2700;
	setCommonNTCSensor(&engineConfiguration->iat);
	engineConfiguration->iat.config.bias_resistor = 2700;


	/**
	 * http://rusefi.com/wiki/index.php?title=Manual:Hardware_Frankenso_board
	 */
	// Frankenso low out #1: PE6
	// Frankenso low out #2: PE5
	// Frankenso low out #3: PD7 Main Relay
	// Frankenso low out #4: PC13 Idle valve solenoid
	// Frankenso low out #5: PE3
	// Frankenso low out #6: PE4 fuel pump relay
	// Frankenso low out #7: PE1 (do not use with discovery!)
	// Frankenso low out #8: PE2 injector #2
	// Frankenso low out #9: PB9 injector #1
	// Frankenso low out #10: PE0 (do not use with discovery!)
	// Frankenso low out #11: PB8 injector #3
	// Frankenso low out #12: PB7 injector #4

	CONFIGB(fuelPumpPin) = GPIOE_4;
	CONFIGB(mainRelayPin) = GPIOD_7;
	CONFIGB(idle).solenoidPin = GPIOC_13;

	CONFIGB(fanPin) = GPIOE_5;

	CONFIGB(injectionPins)[0] = GPIOB_9; // #1
	CONFIGB(injectionPins)[1] = GPIOE_2; // #2
	CONFIGB(injectionPins)[2] = GPIOB_8; // #3
	CONFIGB(injectionPins)[3] = GPIOB_7; // #4

	setAlgorithm(LM_SPEED_DENSITY PASS_ENGINE_PARAMETER_SUFFIX);

#if EFI_PWM_TESTER
	CONFIGB(injectionPins)[4] = GPIOC_8; // #5
	CONFIGB(injectionPins)[5] = GPIOD_10; // #6
	CONFIGB(injectionPins)[6] = GPIOD_9;
	CONFIGB(injectionPins)[7] = GPIOD_11;
	CONFIGB(injectionPins)[8] = GPIOD_0;
	CONFIGB(injectionPins)[9] = GPIOB_11;
	CONFIGB(injectionPins)[10] = GPIOC_7;
	CONFIGB(injectionPins)[11] = GPIOE_4;

	/**
	 * We want to initialize all outputs for test
	 */
	engineConfiguration->specs.cylindersCount = 12;

	engineConfiguration->displayMode = DM_NONE;
#else /* EFI_PWM_TESTER */
	CONFIGB(injectionPins)[4] = GPIO_UNASSIGNED;
	CONFIGB(injectionPins)[5] = GPIO_UNASSIGNED;
	CONFIGB(injectionPins)[6] = GPIO_UNASSIGNED;
	CONFIGB(injectionPins)[7] = GPIO_UNASSIGNED;
	CONFIGB(injectionPins)[8] = GPIO_UNASSIGNED;
	CONFIGB(injectionPins)[9] = GPIO_UNASSIGNED;
	CONFIGB(injectionPins)[10] = GPIO_UNASSIGNED;
	CONFIGB(injectionPins)[11] = GPIO_UNASSIGNED;

	CONFIGB(ignitionPins)[0] = GPIOE_14;
	CONFIGB(ignitionPins)[1] = GPIOC_7;
	CONFIGB(ignitionPins)[2] = GPIOC_9;
	// set_ignition_pin 4 PE10
	CONFIGB(ignitionPins)[3] = GPIOE_10;
#endif /* EFI_PWM_TESTER */

	// todo: 8.2 or 10k?
	engineConfiguration->vbattDividerCoeff = ((float) (10 + 33)) / 10 * 2;

#if EFI_CAN_SUPPORT || defined(__DOXYGEN__)
	enableFrankensoCan();
#endif /* EFI_CAN_SUPPORT */
}

void setFrankensoBoardTestConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);

	engineConfiguration->directSelfStimulation = true; // this engine type is used for board validation

	CONFIGB(triggerSimulatorFrequency) = 300;
	engineConfiguration->cranking.rpm = 100;

	engineConfiguration->specs.cylindersCount = 12;
	engineConfiguration->specs.firingOrder = FO_1_7_5_11_3_9_6_12_2_8_4_10;

	// set ignition_mode 1
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	CONFIGB(injectionPins)[0] = GPIOB_7; // injector in default pinout
	CONFIGB(injectionPins)[1] = GPIOB_8; // injector in default pinout
	CONFIGB(injectionPins)[2] = GPIOB_9; // injector in default pinout
	CONFIGB(injectionPins)[3] = GPIOC_13;

	CONFIGB(injectionPins)[4] = GPIOD_3;
	CONFIGB(injectionPins)[5] = GPIOD_5;
	CONFIGB(injectionPins)[6] = GPIOD_7;
	CONFIGB(injectionPins)[7] = GPIOE_2; // injector in default pinout
	CONFIGB(injectionPins)[8] = GPIOE_3;
	CONFIGB(injectionPins)[9] = GPIOE_4;
	CONFIGB(injectionPins)[10] = GPIOE_5;
	CONFIGB(injectionPins)[11] = GPIOE_6;

	CONFIGB(fuelPumpPin) = GPIO_UNASSIGNED;
	CONFIGB(mainRelayPin) = GPIO_UNASSIGNED;
	CONFIGB(idle).solenoidPin = GPIO_UNASSIGNED;
	CONFIGB(fanPin) = GPIO_UNASSIGNED;


	CONFIGB(ignitionPins)[0] = GPIOC_9; // coil in default pinout
	CONFIGB(ignitionPins)[1] = GPIOC_7; // coil in default pinout
	CONFIGB(ignitionPins)[2] = GPIOE_10; // coil in default pinout
	CONFIGB(ignitionPins)[3] = GPIOE_8; // Miata VVT tach

	CONFIGB(ignitionPins)[4] = GPIOE_14; // coil in default pinout
	CONFIGB(ignitionPins)[5] = GPIOE_12;
	CONFIGB(ignitionPins)[6] = GPIOD_8;
	CONFIGB(ignitionPins)[7] = GPIOD_9;

	CONFIGB(ignitionPins)[8] = GPIOE_0; // brain board, not discovery
	CONFIGB(ignitionPins)[9] = GPIOE_1; // brain board, not discovery

}


#endif /* CONFIG_ENGINES_CUSTOM_ENGINE_CPP_ */
