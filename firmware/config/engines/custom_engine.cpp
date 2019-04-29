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
#include "allsensors.h"
#include "engine_math.h"
#include "fsio_impl.h"

#if EFI_PROD_CODE
#include "can_hw.h"
#include "scheduler.h"
#include "electronic_throttle.h"
#endif /* EFI_PROD_CODE */

EXTERN_ENGINE;


#if EFI_PROD_CODE
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
void runSchedulingPrecisionTestIfNeeded(void) {
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

void setFrankenso_01_LCD(board_configuration_s *boardConfiguration) {
	boardConfiguration->HD44780_rs = GPIOE_7;
	boardConfiguration->HD44780_e = GPIOE_9;
	boardConfiguration->HD44780_db4 = GPIOE_11;
	boardConfiguration->HD44780_db5 = GPIOE_13;
	boardConfiguration->HD44780_db6 = GPIOE_15;
	boardConfiguration->HD44780_db7 = GPIOB_10;
}

void disableLCD(board_configuration_s *boardConfiguration) {
	boardConfiguration->HD44780_rs = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_e = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db4 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db5 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db6 = GPIO_UNASSIGNED;
	boardConfiguration->HD44780_db7 = GPIO_UNASSIGNED;
}

// todo: should this be renamed to 'setFrankensoConfiguration'?
// todo: should this be part of more default configurations?
void setCustomEngineConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_ONE_PLUS_ONE;

	setFrankenso_01_LCD(boardConfiguration);
	commonFrankensoAnalogInputs(engineConfiguration);
	setFrankenso0_1_joystick(engineConfiguration);

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

	boardConfiguration->fuelPumpPin = GPIOE_4;
	boardConfiguration->mainRelayPin = GPIOD_7;
	boardConfiguration->idle.solenoidPin = GPIOC_13;

	boardConfiguration->fanPin = GPIOE_5;

	boardConfiguration->injectionPins[0] = GPIOB_9; // #1
	boardConfiguration->injectionPins[1] = GPIOE_2; // #2
	boardConfiguration->injectionPins[2] = GPIOB_8; // #3
#ifndef EFI_INJECTOR_PIN3
	boardConfiguration->injectionPins[3] = GPIOB_7; // #4
#else /* EFI_INJECTOR_PIN3 */
	boardConfiguration->injectionPins[3] = EFI_INJECTOR_PIN3; // #4
#endif /* EFI_INJECTOR_PIN3 */

	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

#if EFI_PWM_TESTER
	boardConfiguration->injectionPins[4] = GPIOC_8; // #5
	boardConfiguration->injectionPins[5] = GPIOD_10; // #6
	boardConfiguration->injectionPins[6] = GPIOD_9;
	boardConfiguration->injectionPins[7] = GPIOD_11;
	boardConfiguration->injectionPins[8] = GPIOD_0;
	boardConfiguration->injectionPins[9] = GPIOB_11;
	boardConfiguration->injectionPins[10] = GPIOC_7;
	boardConfiguration->injectionPins[11] = GPIOE_4;

	/**
	 * We want to initialize all outputs for test
	 */
	engineConfiguration->specs.cylindersCount = 12;

	engineConfiguration->displayMode = DM_NONE;
#else /* EFI_PWM_TESTER */
	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[6] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[7] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[8] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[9] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[10] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[11] = GPIO_UNASSIGNED;

	boardConfiguration->ignitionPins[0] = GPIOE_14;
	boardConfiguration->ignitionPins[1] = GPIOC_7;
	boardConfiguration->ignitionPins[2] = GPIOC_9;
	// set_ignition_pin 4 PE10
	boardConfiguration->ignitionPins[3] = GPIOE_10;
#endif /* EFI_PWM_TESTER */

	// todo: 8.2 or 10k?
	engineConfiguration->vbattDividerCoeff = ((float) (10 + 33)) / 10 * 2;

#if EFI_CAN_SUPPORT
	enableFrankensoCan();
#endif /* EFI_CAN_SUPPORT */
}

void setFrankensoBoardTestConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_SIGNATURE);

	engineConfiguration->directSelfStimulation = true; // this engine type is used for board validation

	boardConfiguration->triggerSimulatorFrequency = 300;
	engineConfiguration->cranking.rpm = 100;

	engineConfiguration->specs.cylindersCount = 12;
	engineConfiguration->specs.firingOrder = FO_1_7_5_11_3_9_6_12_2_8_4_10;

	// set ignition_mode 1
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	boardConfiguration->injectionPins[0] = GPIOB_7; // injector in default pinout
	boardConfiguration->injectionPins[1] = GPIOB_8; // injector in default pinout
	boardConfiguration->injectionPins[2] = GPIOB_9; // injector in default pinout
	boardConfiguration->injectionPins[3] = GPIOC_13;

	boardConfiguration->injectionPins[4] = GPIOD_3;
	boardConfiguration->injectionPins[5] = GPIOD_5;
	boardConfiguration->injectionPins[6] = GPIOD_7;
	boardConfiguration->injectionPins[7] = GPIOE_2; // injector in default pinout
	boardConfiguration->injectionPins[8] = GPIOE_3;
	boardConfiguration->injectionPins[9] = GPIOE_4;
	boardConfiguration->injectionPins[10] = GPIOE_5;
	boardConfiguration->injectionPins[11] = GPIOE_6;

	boardConfiguration->fuelPumpPin = GPIO_UNASSIGNED;
	boardConfiguration->mainRelayPin = GPIO_UNASSIGNED;
	boardConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	boardConfiguration->fanPin = GPIO_UNASSIGNED;


	boardConfiguration->ignitionPins[0] = GPIOC_9; // coil in default pinout
	boardConfiguration->ignitionPins[1] = GPIOC_7; // coil in default pinout
	boardConfiguration->ignitionPins[2] = GPIOE_10; // coil in default pinout
	boardConfiguration->ignitionPins[3] = GPIOE_8; // Miata VVT tach

	boardConfiguration->ignitionPins[4] = GPIOE_14; // coil in default pinout
	boardConfiguration->ignitionPins[5] = GPIOE_12;
	boardConfiguration->ignitionPins[6] = GPIOD_8;
	boardConfiguration->ignitionPins[7] = GPIOD_9;

	boardConfiguration->ignitionPins[8] = GPIOE_0; // brain board, not discovery
	boardConfiguration->ignitionPins[9] = GPIOE_1; // brain board, not discovery

}

// ETB_BENCH_ENGINE
// set engine_type 58
void setEtbTestConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setDefaultBasePins(PASS_CONFIG_PARAMETER_SIGNATURE);
	// TODO: HOW?! IS THIS HELPING USB SERIAL?!
	setDefaultSerialParameters(PASS_CONFIG_PARAMETER_SIGNATURE);
	// VAG test ETB
	// set tps_min 54
	engineConfiguration->tpsMin = 54;
	// by the way this ETB has default position of ADC=74 which is about 4%
	// set tps_max 540
	engineConfiguration->tpsMax = 540;

	boardConfiguration->ignitionPins[0] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	/**
	 * remember that some H-bridges require 5v control lines, not just 3v logic outputs we have on stm32
	 */
	CONFIGB(etb1.directionPin1) = GPIOC_7;
	CONFIGB(etb1.directionPin2) = GPIOC_9;
	CONFIGB(etb1.controlPin1) = GPIOE_14;

#if EFI_ELECTRONIC_THROTTLE_BODY
	setDefaultEtbParameters(PASS_ENGINE_PARAMETER_SIGNATURE);
	// values are above 100% since we have feedforward part of the total summation
	engineConfiguration->etb.minValue = -200;
	engineConfiguration->etb.maxValue = 200;
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

	engineConfiguration->tpsAdcChannel = EFI_ADC_2; // PA2
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_9; // PB1

	engineConfiguration->debugMode = DBG_ELECTRONIC_THROTTLE_PID;

	// turning off other PWMs to simplify debugging
	engineConfiguration->bc.triggerSimulatorFrequency = 0;
	engineConfiguration->stepperEnablePin = GPIO_UNASSIGNED;
	CONFIGB(idle).stepperStepPin = GPIO_UNASSIGNED;
	CONFIGB(idle).stepperDirectionPin = GPIO_UNASSIGNED;
	boardConfiguration->useStepperIdle = true;

	engineConfiguration->clt.adcChannel = EFI_ADC_15;
	set10K_4050K(&engineConfiguration->clt);

	// see also setDefaultEtbBiasCurve
}

// TLE8888_BENCH_ENGINE
// set engine_type 59
void setTle8888TestConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setDefaultBasePins(PASS_CONFIG_PARAMETER_SIGNATURE);
	// TODO: HOW?! IS THIS HELPING USB SERIAL?!
	setDefaultSerialParameters(PASS_CONFIG_PARAMETER_SIGNATURE);
	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_7_2_6_5_4_3;
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;

	engineConfiguration->tle8888_cs = GPIOD_5;
	engineConfiguration->directSelfStimulation = true;

	boardConfiguration->ignitionPins[0] = GPIOG_3;
	boardConfiguration->ignitionPins[1] = GPIOG_4;
	boardConfiguration->ignitionPins[2] = GPIOG_5;
	boardConfiguration->ignitionPins[3] = GPIOG_6;
	boardConfiguration->ignitionPins[4] = GPIOG_7;
	boardConfiguration->ignitionPins[5] = GPIOG_8;
	boardConfiguration->ignitionPins[6] = GPIOC_6;
	boardConfiguration->ignitionPins[7] = GPIOC_7;

	engineConfiguration->tle8888spiDevice = SPI_DEVICE_1;

	// PB3 is nicely both SWO and SPI1 SCK so logic analyzer could be used on SWO header
	boardConfiguration->is_enabled_spi_1 = true;
	engineConfiguration->debugMode = DBG_TLE8888;

	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_9; // PB1 // just any non-empty value for now
	// ETB #1 top one - closer to 121 connector
	// DIS PF12
	// EN  PF13
	// IN1 PF15
	// IN2 PF14
	// SF  PF11
#if EFI_FSIO
	setFsio(12, GPIOF_12, "0" PASS_ENGINE_PARAMETER_SUFFIX);
	setFsio(14, GPIOF_13, "1" PASS_ENGINE_PARAMETER_SUFFIX);
#endif
	CONFIG(etb1_use_two_wires) = true;
	CONFIGB(etb1.directionPin1) = GPIOF_15;
	CONFIGB(etb1.directionPin2) = GPIOF_14;
	boardConfiguration->isHip9011Enabled = false;

	// ETB #2
	// DIS PE5
	// EN  PE6
	// IN1 PE2
	// IN2 PE4
	// SF  PE3
#if EFI_FSIO
	setFsio(13, GPIOE_5, "0" PASS_ENGINE_PARAMETER_SUFFIX);
	setFsio(15, GPIOE_6, "1" PASS_ENGINE_PARAMETER_SUFFIX);
#endif
	CONFIG(etb2_use_two_wires) = true;
	CONFIG(etb2.directionPin1) = GPIOE_2;
	CONFIG(etb2.directionPin2) = GPIOE_4;

	boardConfiguration->fuelPumpPin = TLE8888_PIN_20;

	engineConfiguration->tpsAdcChannel = EFI_ADC_3; // PA3
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_7; // PA7

	engineConfiguration->etb.pFactor = 1.07;
	engineConfiguration->etb.iFactor = 	0.18;
	engineConfiguration->etb.dFactor = 0.24;
	engineConfiguration->etb.offset = 80;
#if EFI_PROD_CODE
	engineConfiguration->etb.periodMs = (1000 / DEFAULT_ETB_LOOP_FREQUENCY);
	engineConfiguration->etbFreq = DEFAULT_ETB_PWM_FREQUENCY;
#endif
	engineConfiguration->etb_iTermMin = -300;
	engineConfiguration->etb_iTermMax = 300;

	// VAG test ETB, no divider on red board - direct 3v TPS sensor
	// set tps_min 332
	engineConfiguration->tpsMin = 332;
	// by the way this ETB has default position of ADC=74 which is about 4%
	// set tps_max 540
	engineConfiguration->tpsMax = 799;
}

#endif /* CONFIG_ENGINES_CUSTOM_ENGINE_CPP_ */
