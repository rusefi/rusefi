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
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "custom_engine.h"
#include "fsio_impl.h"
#include "mre_meta.h"
#include "proteus_meta.h"

#if EFI_ELECTRONIC_THROTTLE_BODY
#include "electronic_throttle.h"
#endif

#if EFI_PROD_CODE
#include "can_hw.h"
#include "scheduler.h"
#endif /* EFI_PROD_CODE */

#if HW_PROTEUS & EFI_PROD_CODE
#include "proteus_meta.h"
#endif

#if EFI_PROD_CODE
static int periodIndex = 0;

static OutputPin testPin;
static scheduling_s testScheduling;

static int test557[] = {5, 5, 10, 10, 20, 20, 50, 50, 100, 100, 200, 200, 500, 500, 500, 500};
#define TEST_LEN 16

efitimeus_t testTime;

static void toggleTestAndScheduleNext(void *) {
	testPin.toggle();
	periodIndex = (periodIndex + 1) % TEST_LEN;
	testTime += test557[periodIndex];
	engine->executor.scheduleByTimestamp("test", &testScheduling, testTime, &toggleTestAndScheduleNext);
}

/**
 * https://github.com/rusefi/rusefi/issues/557 common rail / direct injection scheduling control test
 */
void runSchedulingPrecisionTestIfNeeded(void) {
	if (!isBrainPinValid(engineConfiguration->test557pin)) {
		return;
	}

	testPin.initPin("test", engineConfiguration->test557pin);
	testPin.setValue(0);
	testTime = getTimeNowUs();
	toggleTestAndScheduleNext(/*unused*/ nullptr);
}
#endif /* EFI_PROD_CODE */

void setFrankenso_01_LCD(engine_configuration_s *engineConfiguration) {
	engineConfiguration->HD44780_rs = GPIOE_7;
	engineConfiguration->HD44780_e = GPIOE_9;
	engineConfiguration->HD44780_db4 = GPIOE_11;
	engineConfiguration->HD44780_db5 = GPIOE_13;
	engineConfiguration->HD44780_db6 = GPIOE_15;
	engineConfiguration->HD44780_db7 = GPIOB_10;
}

void disableLCD(engine_configuration_s *engineConfiguration) {
	engineConfiguration->HD44780_rs = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_e = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db4 = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db5 = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db6 = GPIO_UNASSIGNED;
	engineConfiguration->HD44780_db7 = GPIO_UNASSIGNED;
}

// todo: should this be part of more default configurations?
void setFrankensoConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setDefaultFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
	engineConfiguration->trigger.type = TT_ONE_PLUS_ONE;

	setFrankenso_01_LCD(engineConfiguration);
	engineConfiguration->displayMode = DM_HD44780;
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
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_2; // PA2

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0;

	engineConfiguration->clt.adcChannel = EFI_ADC_12;
	engineConfiguration->iat.adcChannel = EFI_ADC_11;
	engineConfiguration->afr.hwChannel = EFI_ADC_13;

	setCommonNTCSensor(&engineConfiguration->clt, 2700);
	setCommonNTCSensor(&engineConfiguration->iat, 2700);


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

	engineConfiguration->fuelPumpPin = GPIOE_4;
	engineConfiguration->mainRelayPin = GPIOD_7;
	engineConfiguration->idle.solenoidPin = GPIOC_13;

	engineConfiguration->fanPin = GPIOE_5;

	engineConfiguration->injectionPins[0] = GPIOB_9; // #1
	engineConfiguration->injectionPins[1] = GPIOE_2; // #2
	engineConfiguration->injectionPins[2] = GPIOB_8; // #3
#ifndef EFI_INJECTOR_PIN3
	engineConfiguration->injectionPins[3] = GPIOB_7; // #4
#else /* EFI_INJECTOR_PIN3 */
	engineConfiguration->injectionPins[3] = EFI_INJECTOR_PIN3; // #4
#endif /* EFI_INJECTOR_PIN3 */

	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

#if EFI_PWM_TESTER
	engineConfiguration->injectionPins[4] = GPIOC_8; // #5
	engineConfiguration->injectionPins[5] = GPIOD_10; // #6
	engineConfiguration->injectionPins[6] = GPIOD_9;
	engineConfiguration->injectionPins[7] = GPIOD_11;
	engineConfiguration->injectionPins[8] = GPIOD_0;
	engineConfiguration->injectionPins[9] = GPIOB_11;
	engineConfiguration->injectionPins[10] = GPIOC_7;
	engineConfiguration->injectionPins[11] = GPIOE_4;

	/**
	 * We want to initialize all outputs for test
	 */
	engineConfiguration->specs.cylindersCount = 12;

	engineConfiguration->displayMode = DM_NONE;
#else /* EFI_PWM_TESTER */
	engineConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[6] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[7] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[8] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[9] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[10] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[11] = GPIO_UNASSIGNED;

	engineConfiguration->ignitionPins[0] = GPIOE_14;
	engineConfiguration->ignitionPins[1] = GPIOC_7;
	engineConfiguration->ignitionPins[2] = GPIOC_9;
	// set_ignition_pin 4 PE10
	engineConfiguration->ignitionPins[3] = GPIOE_10;
#endif /* EFI_PWM_TESTER */

	// todo: 8.2 or 10k?
	engineConfiguration->vbattDividerCoeff = ((float) (10 + 33)) / 10 * 2;

#if EFI_CAN_SUPPORT
	enableFrankensoCan();
#endif /* EFI_CAN_SUPPORT */
}

/**
 * set engine_type 49
 */
void setFrankensoBoardTestConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->triggerSimulatorFrequency = 300;
	engineConfiguration->cranking.rpm = 100;

	engineConfiguration->specs.cylindersCount = 12;
	engineConfiguration->specs.firingOrder = FO_1_7_5_11_3_9_6_12_2_8_4_10;

	// set ignition_mode 1
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	engineConfiguration->injectionPins[0] = GPIOB_7; // injector in default pinout
	engineConfiguration->injectionPins[1] = GPIOB_8; // injector in default pinout
	engineConfiguration->injectionPins[2] = GPIOB_9; // injector in default pinout
	engineConfiguration->injectionPins[3] = GPIOC_13;

	engineConfiguration->injectionPins[4] = GPIOD_3;
	engineConfiguration->injectionPins[5] = GPIOD_5;
	engineConfiguration->injectionPins[6] = GPIOD_7;
	engineConfiguration->injectionPins[7] = GPIOE_2; // injector in default pinout
	engineConfiguration->injectionPins[8] = GPIOE_3;
	engineConfiguration->injectionPins[9] = GPIOE_4;
	engineConfiguration->injectionPins[10] = GPIOE_5;
	engineConfiguration->injectionPins[11] = GPIOE_6;

	engineConfiguration->fuelPumpPin = GPIO_UNASSIGNED;
	engineConfiguration->mainRelayPin = GPIO_UNASSIGNED;
	engineConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	engineConfiguration->fanPin = GPIO_UNASSIGNED;


	engineConfiguration->ignitionPins[0] = GPIOC_9; // coil in default pinout
	engineConfiguration->ignitionPins[1] = GPIOC_7; // coil in default pinout
	engineConfiguration->ignitionPins[2] = GPIOE_10; // coil in default pinout
	engineConfiguration->ignitionPins[3] = GPIOE_8; // Miata VVT tach

	engineConfiguration->ignitionPins[4] = GPIOE_14; // coil in default pinout
	engineConfiguration->ignitionPins[5] = GPIOE_12;
	engineConfiguration->ignitionPins[6] = GPIOD_8;
	engineConfiguration->ignitionPins[7] = GPIOD_9;

	engineConfiguration->ignitionPins[8] = GPIOE_0; // brain board, not discovery
	engineConfiguration->ignitionPins[9] = GPIOE_1; // brain board, not discovery
}


// ETB_BENCH_ENGINE
// set engine_type 58
void setEtbTestConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setDefaultFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
	// VAG test ETB
	// set tps_min 54
	engineConfiguration->tpsMin = 54;
	// by the way this ETB has default position of ADC=74 which is about 4%
	// set tps_max 540
	engineConfiguration->tpsMax = 540;

	// yes, 30K - that's a test configuration
	engineConfiguration->rpmHardLimit = 30000;

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;


	engineConfiguration->ignitionPins[0] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	/**
	 * remember that some H-bridges require 5v control lines, not just 3v logic outputs we have on stm32
	 */
	CONFIG(etbIo[0].directionPin1) = GPIOC_7; // Frankenso high-side in order to get 5v control
	CONFIG(etbIo[0].directionPin2) = GPIOC_9;
	CONFIG(etbIo[0].controlPin) = GPIOE_14;

#if EFI_ELECTRONIC_THROTTLE_BODY
	setBoschVNH2SP30Curve(PASS_CONFIG_PARAMETER_SIGNATURE);
//	setDefaultEtbParameters(PASS_CONFIG_PARAMETER_SIGNATURE);
#endif /* EFI_ELECTRONIC_THROTTLE_BODY */

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_2; // PA2
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_9; // PB1

	engineConfiguration->debugMode = DBG_ELECTRONIC_THROTTLE_PID;

	// turning off other PWMs to simplify debugging
	engineConfiguration->triggerSimulatorFrequency = 0;
	engineConfiguration->stepperEnablePin = GPIO_UNASSIGNED;
	CONFIG(idle).stepperStepPin = GPIO_UNASSIGNED;
	CONFIG(idle).stepperDirectionPin = GPIO_UNASSIGNED;
	engineConfiguration->useStepperIdle = true;

	// no analog dividers - all sensors with 3v supply, naked discovery bench setup
	engineConfiguration->analogInputDividerCoefficient = 1;

	// EFI_ADC_15 = PC5
	engineConfiguration->clt.adcChannel = EFI_ADC_15;
	set10K_4050K(&engineConfiguration->clt, 10000);

	// see also setDefaultEtbBiasCurve
}

#if EFI_UNIT_TEST
void setIssue898(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_MAZDA_MIATA_NA;
}
#endif /* EFI_UNIT_TEST */

// TLE8888_BENCH_ENGINE
// todo: remove this? this was used to play with "secret" red boards prior to MRE reality
// set engine_type 59
void setTle8888TestConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_7_2_6_5_4_3;
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;

#if defined(STM32_HAS_GPIOG) && STM32_HAS_GPIOG
	engineConfiguration->ignitionPins[0] = GPIOG_3;
	engineConfiguration->ignitionPins[1] = GPIOG_4;
	engineConfiguration->ignitionPins[2] = GPIOG_5;
	engineConfiguration->ignitionPins[3] = GPIOG_6;
	engineConfiguration->ignitionPins[4] = GPIOG_7;
	engineConfiguration->ignitionPins[5] = GPIOG_8;
#endif /* STM32_HAS_GPIOG */
	engineConfiguration->ignitionPins[6] = GPIOC_6;
	engineConfiguration->ignitionPins[7] = GPIOC_7;

#if (BOARD_TLE8888_COUNT > 0)
	engineConfiguration->tle8888spiDevice = SPI_DEVICE_1;
	engineConfiguration->tle8888_cs = GPIOD_5;

	// PB3 is nicely both SWO and SPI1 SCK so logic analyzer could be used on SWO header
	engineConfiguration->spi1mosiPin = GPIOB_5;
	engineConfiguration->spi1misoPin = GPIOB_4;
	engineConfiguration->spi1sckPin = GPIOB_3; // please note that this pin is also SWO/SWD - Single Wire debug Output
	engineConfiguration->is_enabled_spi_1 = true;
	engineConfiguration->debugMode = DBG_TLE8888;

	engineConfiguration->fuelPumpPin = TLE8888_PIN_20;
#endif /* BOARD_TLE8888_COUNT */

	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_9; // PB1 // just any non-empty value for now
	// ETB #1 top one - closer to 121 connector
	// DIS PF12
	// EN  PF13
	// IN1 PF15
	// IN2 PF14
	// SF  PF11
#if defined(STM32_HAS_GPIOF) && STM32_HAS_GPIOF
#if EFI_FSIO
	setFsio(14, GPIOF_13, "1" PASS_CONFIG_PARAMETER_SUFFIX);
#endif /* EFI_FSIO */
	CONFIG(etbIo[0].directionPin1) = GPIOF_15;
	CONFIG(etbIo[0].directionPin2) = GPIOF_14;
	CONFIG(etbIo[0].disablePin) = GPIOF_12;
#endif /* STM32_HAS_GPIOF */
	CONFIG(etb_use_two_wires) = true;
	engineConfiguration->isHip9011Enabled = false;

	// ETB #2
	// DIS PE5
	// EN  PE6
	// IN1 PE2
	// IN2 PE4
	// SF  PE3
#if EFI_FSIO
	setFsio(15, GPIOE_6, "1" PASS_CONFIG_PARAMETER_SUFFIX);
#endif
	CONFIG(etbIo[0].directionPin1) = GPIOE_2;
	CONFIG(etbIo[0].directionPin2) = GPIOE_4;
	CONFIG(etbIo[0].disablePin) = GPIOE_5;


	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3; // PA3
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_7; // PA7

//	engineConfiguration->etb.pFactor = 1.07;
//	engineConfiguration->etb.iFactor = 	0.18;
//	engineConfiguration->etb.dFactor = 0.24;
//	engineConfiguration->etb.offset = 80;

	engineConfiguration->etb.pFactor = 22;
	engineConfiguration->etb.iFactor = 	0;
	engineConfiguration->etb.dFactor = 0;
	engineConfiguration->etb.offset = 0;

	engineConfiguration->etb_iTermMin = -300;
	engineConfiguration->etb_iTermMax = 300;

	// VAG test ETB, no divider on red board - direct 3v TPS sensor
	// set tps_min 332
	engineConfiguration->tpsMin = 332;
	// by the way this ETB has default position of ADC=74 which is about 4%
	// set tps_max 540
	engineConfiguration->tpsMax = 799;
}

/**
 * This configuration is used for MRE board Quality Assurance validation
 * todo: inline
 */
static void mreBoardOldTest(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
#if (BOARD_TLE8888_COUNT > 0)
	engineConfiguration->debugMode = DBG_TLE8888;

	engineConfiguration->triggerSimulatorFrequency = HW_CHECK_RPM;
	// set cranking_rpm 500
	engineConfiguration->cranking.rpm = 100;
	// set cranking_dwell 200
	engineConfiguration->ignitionDwellForCrankingMs = 200;
	// set cranking_fuel 300
	engineConfiguration->cranking.baseFuel = 190;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;

	// EFI_ADC_1: "23 - AN temp 2"
	// test harness: Red/Green, 2K PD. expected 2.0v
	// iat in microrusefi/board_configuration.cpp

	// EFI_ADC_2: "24 - AN temp 3"
	// test harness: Blue/White, 2K PD. expected 2.0v


	// EFI_ADC_10: "27 - AN volt 1"
	// test harness: Blue/Red, 3.84K PD / 5.3 PU. expected 1.6v
	engineConfiguration->mafAdcChannel = EFI_ADC_10;

	// EFI_ADC_14: "32 - AN volt 6"
	// test harness: Red/White 3.6K PD / 5.2 PU. expected 1.6v
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_14;


	// EFI_ADC_4: "28 - AN volt 10"
	// test harness: Red/Yellow
	engineConfiguration->afr.hwChannel = EFI_ADC_4;


	// EFI_ADC_7: "31 - AN volt 3"
	// test harness: White/Red
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_7;


	//engineConfiguration->baroSensor.hwChannel
	//engineConfiguration->oilPressure.hwChannel
	//engineConfiguration->fuelLevelSensor

	// TPS tps1_1AdcChannel EFI_ADC_13

	engineConfiguration->specs.cylindersCount = 10;
	engineConfiguration->specs.firingOrder = FO_1_10_9_4_3_6_5_8_7_2;

	// red LED #1
	engineConfiguration->ignitionPins[1 - 1] = GPIOD_4;
	engineConfiguration->ignitionPins[10 - 1] = GPIOD_3;
	engineConfiguration->ignitionPins[9 - 1] = GPIOD_6;
	engineConfiguration->ignitionPins[4 - 1] = GPIOD_7;
	engineConfiguration->ignitionPins[3 - 1] = GPIOD_1;
	engineConfiguration->ignitionPins[6 - 1] = GPIOD_2;
	engineConfiguration->ignitionPins[1] =  GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[4] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[6] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[7] = GPIO_UNASSIGNED;


	engineConfiguration->fuelPumpPin = GPIO_UNASSIGNED;
	engineConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	engineConfiguration->fanPin = GPIO_UNASSIGNED;

	// fuel pump is useful to test power on/off scenario
//	engineConfiguration->fuelPumpPin = TLE8888_PIN_22;


	// LED #1
	// TLE8888_PIN_22: "34 - GP Out 2"
	engineConfiguration->injectionPins[1 - 1] = TLE8888_PIN_22;

	// LED #2
	// TLE8888_PIN_23: "33 - GP Out 3"
	engineConfiguration->injectionPins[10 - 1] = TLE8888_PIN_23;

	// TLE8888_PIN_1: LED #3 - INJ#2
	engineConfiguration->injectionPins[9 - 1] = TLE8888_PIN_1;

	// TLE8888_PIN_2: LED #4 - INJ#1
	engineConfiguration->injectionPins[4 - 1] = TLE8888_PIN_2;

	// TLE8888_PIN_3: LED #5 - INJ#3
	engineConfiguration->injectionPins[3 - 1] = TLE8888_PIN_3;

	// TLE8888_PIN_4: LED #6 - INJ#4
	engineConfiguration->injectionPins[6 - 1] = TLE8888_PIN_4;

	// LED #7
	// TLE8888_PIN_24: "43 - GP Out 4"
	engineConfiguration->injectionPins[5 - 1] = TLE8888_PIN_24;

	// LED #8
	// TLE8888 half bridges (pushpull, lowside, or high-low)  IN12
	// TLE8888_PIN_21: "35 - GP Out 1"
	engineConfiguration->injectionPins[8 - 1] = TLE8888_PIN_21;

	// LED #9
	// TLE8888 high current low side: IN10
	// TLE8888_PIN_6: "7 - Lowside 1"
	engineConfiguration->injectionPins[7 - 1] = TLE8888_PIN_6;

	// LED #10
	// TLE8888 high current low side: VVT2 IN9 / OUT5
	// TLE8888_PIN_5: "3 - Lowside 2"
	engineConfiguration->injectionPins[2 - 1] = TLE8888_PIN_5;
#endif /* BOARD_TLE8888_COUNT */
}

#if HW_PROTEUS
/**
 * PROTEUS_QC_TEST_BOARD
 * set engine_type 42
 */
void proteusBoardTest(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->specs.cylindersCount = 12;
	engineConfiguration->specs.firingOrder = FO_1_2_3_4_5_6_7_8_9_10_11_12;
	engineConfiguration->triggerSimulatorFrequency = 600;

	engineConfiguration->cranking.rpm = 100;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;

	engineConfiguration->mainRelayPin = GPIO_UNASSIGNED;
	CONFIG(fanPin) = GPIO_UNASSIGNED;
	CONFIG(fuelPumpPin) = GPIO_UNASSIGNED;

#if EFI_PROD_CODE
	engineConfiguration->injectionPins[0] = PROTEUS_LS_1;
	engineConfiguration->injectionPins[1] = PROTEUS_LS_2;
	engineConfiguration->injectionPins[2] = PROTEUS_LS_3;
	engineConfiguration->injectionPins[3] = PROTEUS_LS_4;
	engineConfiguration->injectionPins[4] = GPIOG_12;// "Lowside 5"
	engineConfiguration->injectionPins[5] = GPIOG_13;// "Lowside 6"
	engineConfiguration->injectionPins[6] = GPIOB_5;//  "Lowside 9"
	engineConfiguration->injectionPins[7] = GPIOB_4;//  "Lowside 8"
	engineConfiguration->injectionPins[8] = GPIOB_7;//  "Lowside 11"
	engineConfiguration->injectionPins[9] = PROTEUS_LS_10;
	engineConfiguration->injectionPins[10] = GPIOB_8;//  "Lowside 12"
	engineConfiguration->injectionPins[11] = GPIOB_9;//  "Lowside 13"    # pin 10/black35




	engineConfiguration->ignitionPins[0] = PROTEUS_IGN_1;
	engineConfiguration->ignitionPins[1] = PROTEUS_IGN_2;
	engineConfiguration->ignitionPins[2] = PROTEUS_IGN_4;
	engineConfiguration->ignitionPins[3] = PROTEUS_IGN_5;
	engineConfiguration->ignitionPins[4] = PROTEUS_IGN_6;
	engineConfiguration->ignitionPins[5] = PROTEUS_IGN_7;

	engineConfiguration->ignitionPins[6] = GPIOD_15;// "Highside 3"    # pin 13/black35
	engineConfiguration->ignitionPins[7] = PROTEUS_IGN_3;
	engineConfiguration->ignitionPins[8] = PROTEUS_IGN_9;
	engineConfiguration->ignitionPins[9] = PROTEUS_IGN_8;
	engineConfiguration->ignitionPins[10] = PROTEUS_IGN_1;
	engineConfiguration->ignitionPins[11] = PROTEUS_IGN_12;

	engineConfiguration->fsioOutputPins[0] = GPIOE_2;//  "Lowside 16"    # pin 23/black35
	engineConfiguration->fsioOutputPins[1] = GPIOG_14;// "Lowside 7"
	engineConfiguration->fsioOutputPins[2] = GPIOE_0;//  "Lowside 14"    # pin 11/black35
	engineConfiguration->fsioOutputPins[3] = GPIOE_1;//  "Lowside 15"    # pin 12/black35

	engineConfiguration->fsioOutputPins[4] = GPIOG_3;//  "Ign 11"
	engineConfiguration->fsioOutputPins[5] = GPIOA_8;//  "Highside 2"    # pin 1/black35
	engineConfiguration->fsioOutputPins[6] = GPIOD_14;// "Highside 4"    # pin 14/black35
	engineConfiguration->fsioOutputPins[7] = GPIOG_4;//  "Ign 10"

#endif // EFI_PROD_CODE

	setProteusHitachiEtbDefaults(PASS_CONFIG_PARAMETER_SIGNATURE);
}
#endif // HW_PROTEUS

void mreBCM(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	for (int i = 0; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->ignitionPins[i] = GPIO_UNASSIGNED;
		engineConfiguration->injectionPins[i] = GPIO_UNASSIGNED;
	}
	engineConfiguration->fanPin = GPIO_UNASSIGNED;
	engineConfiguration->consumeObdSensors = true;


	engineConfiguration->scriptSetting[0] = 1500;
	// simple warning light as default configuration
	// set_fsio_expression 1 "rpm > fsio_setting(1)"
	setFsio(0, GPIO_UNASSIGNED, RPM_ABOVE_USER_SETTING_1 PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->scriptSetting[2] = 1500;
	setFsio(2, GPIO_UNASSIGNED, RPM_BELOW_USER_SETTING_3 PASS_CONFIG_PARAMETER_SUFFIX);


#if (BOARD_TLE8888_COUNT > 0)
	engineConfiguration->fsioOutputPins[0] = TLE8888_PIN_1; // "37 - Injector 1"
	engineConfiguration->fsioOutputPins[1] = TLE8888_PIN_2; // "38 - Injector 2"
	engineConfiguration->fsioOutputPins[2] = TLE8888_PIN_3; // "41 - Injector 3"
	engineConfiguration->fsioOutputPins[3] = TLE8888_PIN_4; // "42 - Injector 4"
// 	engineConfiguration->fsioOutputPins[4] = TLE8888_PIN_5;
// 	engineConfiguration->fsioOutputPins[5] = TLE8888_PIN_6;
//	engineConfiguration->fsioOutputPins[6] = TLE8888_PIN_21;
	engineConfiguration->fsioOutputPins[7] = TLE8888_PIN_22; // "34 - GP Out 2"
	engineConfiguration->fsioOutputPins[8] = TLE8888_PIN_23; // "33 - GP Out 3"
	engineConfiguration->fsioOutputPins[9] = TLE8888_PIN_24; // "43 - GP Out 4"
#endif /* BOARD_TLE8888_COUNT */

}

void mreSecondaryCan(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_NONE;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;
	engineConfiguration->clt.adcChannel = EFI_ADC_NONE;
	engineConfiguration->iat.adcChannel = EFI_ADC_NONE;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_NONE;


	engineConfiguration->auxAnalogInputs[0] = MRE_IN_TPS;
	engineConfiguration->auxAnalogInputs[1] = MRE_IN_MAP;
	engineConfiguration->auxAnalogInputs[2] = MRE_IN_CLT;
	engineConfiguration->auxAnalogInputs[3] = MRE_IN_IAT;
	// engineConfiguration->auxAnalogInputs[0] =


	// EFI_ADC_14: "32 - AN volt 6"
//	engineConfiguration->afr.hwChannel = EFI_ADC_14;


	strncpy(config->luaScript, R"(
txPayload = {}
function onTick()
  auxV = getAuxAnalog(0)
  print('Hello analog ' .. auxV )
  -- first byte: integer part, would be autoboxed to int
  txPayload[1] = auxV
  -- second byte: fractional part, would be autoboxed to int, overflow would be ignored
  txPayload[2] = auxV * 256;
  auxV = getAuxAnalog(1)
  print('Hello analog ' .. auxV )
  txPayload[3] = auxV
  txPayload[4] = auxV * 256;
  auxV = getAuxAnalog(2)
  print('Hello analog ' .. auxV )
  txPayload[5] = auxV
  txPayload[6] = auxV * 256;
  txCan(1, 0x600, 1, txPayload)
end
)", efi::size(config->luaScript));

}

/**
 * MRE_BOARD_NEW_TEST
 * set engine_type 31
 */
void mreBoardNewTest(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	mreBoardOldTest(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->specs.cylindersCount = 12;
	engineConfiguration->specs.firingOrder = FO_1_2_3_4_5_6_7_8_9_10_11_12;

#if (BOARD_TLE8888_COUNT > 0)
	engineConfiguration->ignitionPins[1 - 1] = GPIOD_6;
	engineConfiguration->ignitionPins[2 - 1] = GPIOD_7;
	engineConfiguration->ignitionPins[3 - 1] = GPIOD_1;
	engineConfiguration->ignitionPins[4 - 1] = GPIOD_2;
	engineConfiguration->ignitionPins[5 - 1] = GPIOD_3;
	engineConfiguration->ignitionPins[6 - 1] = GPIOD_4;

	engineConfiguration->ignitionPins[7 - 1] = TLE8888_PIN_11;
	engineConfiguration->ignitionPins[8 - 1] = TLE8888_PIN_12;

	// LED #8
	// TLE8888 half bridges (pushpull, lowside, or high-low)  IN12
	// TLE8888_PIN_21: "35 - GP Out 1"
	engineConfiguration->ignitionPins[9 - 1] = TLE8888_PIN_21;

	// LED #1
	// TLE8888_PIN_22: "34 - GP Out 2"
	engineConfiguration->ignitionPins[10- 1] = TLE8888_PIN_22;

	// LED #2
	// TLE8888_PIN_23: "33 - GP Out 3"
	engineConfiguration->ignitionPins[11 - 1] = TLE8888_PIN_23;

	// LED #7
	// TLE8888_PIN_24: "43 - GP Out 4"
	engineConfiguration->ignitionPins[12 - 1] = TLE8888_PIN_24;

	engineConfiguration->afr.hwChannel = EFI_ADC_6;
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;

	// TLE8888 high current low side: IN10
	// TLE8888_PIN_6:  "7 - Lowside 1"
	engineConfiguration->injectionPins[1 - 1] = TLE8888_PIN_6;

	// TLE8888 high current low side: VVT2 IN9 / OUT5
	// TLE8888_PIN_5: "3 - Lowside 2"
	engineConfiguration->injectionPins[2 - 1] = TLE8888_PIN_5;

	// TLE8888_PIN_4: INJ#4
	engineConfiguration->injectionPins[3 - 1] = TLE8888_PIN_4;
	// TLE8888_PIN_3: INJ#3
	engineConfiguration->injectionPins[4 - 1] = TLE8888_PIN_3;
	// TLE8888_PIN_2: INJ#2
	engineConfiguration->injectionPins[5 - 1] = TLE8888_PIN_2;
	// TLE8888_PIN_1: LED #3 - INJ#1
	engineConfiguration->injectionPins[6 - 1] = TLE8888_PIN_1;


	engineConfiguration->injectionPins[7 - 1] = GPIOA_4; // AV10
	engineConfiguration->injectionPins[8  - 1] = GPIOB_1; // AV9
	engineConfiguration->injectionPins[9  - 1] = GPIOB_0; // AV8
	engineConfiguration->injectionPins[10 - 1] = GPIOC_4; // AV6

	engineConfiguration->injectionPins[11- 1] = TLE8888_PIN_13;

	engineConfiguration->injectionPins[12- 1] = TLE8888_PIN_10;
#endif /* BOARD_TLE8888_COUNT */

}

void setBoschHDEV_5_injectors(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	// This is the configuration for bosch HDEV 5 injectors
	// all times in microseconds/us
	CONFIG(mc33_hvolt) = 65;
	CONFIG(mc33_i_boost) = 13000;
	CONFIG(mc33_i_peak) = 9400;
	CONFIG(mc33_i_hold) = 3700;
	CONFIG(mc33_t_max_boost) = 470;
	CONFIG(mc33_t_peak_off) = 10;
	CONFIG(mc33_t_peak_tot) = 700;
	CONFIG(mc33_t_bypass) = 15;
	CONFIG(mc33_t_hold_off) = 60;
	CONFIG(mc33_t_hold_tot) = 10000;
}

/**
 * set engine_type 108
 */
void setVrThresholdTest(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_HONDA_1_24;

	setHellenDefaultVrThresholds(PASS_CONFIG_PARAMETER_SIGNATURE);
	engineConfiguration->vrThreshold[0].pin = GPIOB_4;

	engineConfiguration->triggerInputPins[0] = GPIOA_5;
	engineConfiguration->triggerInputPins[1] = GPIOC_6;
}

/**
 * set engine_type 107
 */
void setRotary(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->specs.cylindersCount = 2;
	engineConfiguration->specs.firingOrder = FO_1_2;

	engineConfiguration->trigger.type = TT_36_2_2_2;
	setOperationMode(engineConfiguration, TWO_STROKE);

	strcpy(CONFIG(engineMake), ENGINE_MAKE_MAZDA);
	strcpy(CONFIG(engineCode), "13B");
	strcpy(CONFIG(vehicleName), "test");

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->injectionPins[2] = GPIO_UNASSIGNED; // injector in default pinout
	engineConfiguration->injectionPins[3] = GPIO_UNASSIGNED;

	engineConfiguration->enableTrailingSparks = true;
	engineConfiguration->trailingCoilPins[0] = GPIOC_9;
	engineConfiguration->trailingCoilPins[1] = GPIOE_10;
}

/**
 * set engine_type 103
 */
void setTest33816EngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {

	// grey
	// default spi3mosiPin PB5
	// white
	// default spi3misoPin PB4
	// violet
	// default spi3sckPin  PB3


	CONFIG(triggerSimulatorPins[0]) = GPIO_UNASSIGNED;
	CONFIG(triggerSimulatorPins[1]) = GPIO_UNASSIGNED;
	CONFIG(triggerSimulatorPins[2]) = GPIO_UNASSIGNED;

	engineConfiguration->injectionPins[0] = GPIOB_9; // #1
	engineConfiguration->injectionPins[1] = GPIOE_2; // #2
	engineConfiguration->injectionPins[2] = GPIOB_8; // #3
	engineConfiguration->injectionPins[3] = GPIOB_7; // #4


	// blue
	CONFIG(mc33816_cs) = GPIOD_7;
	// green
	CONFIG(mc33816_rstb) = GPIOD_4;
	// yellow
	CONFIG(mc33816_driven) = GPIOD_6;

	CONFIG(mc33816_flag0) = GPIOD_3;

	// enable_spi 3
	CONFIG(is_enabled_spi_3) = true;
	// Wire up spi3
	engineConfiguration->spi3mosiPin = GPIOB_5;
	engineConfiguration->spi3misoPin = GPIOB_4;
	engineConfiguration->spi3sckPin = GPIOB_3;

	CONFIG(isSdCardEnabled) = false;

	CONFIG(mc33816spiDevice) = SPI_DEVICE_3;
	setBoschHDEV_5_injectors(PASS_CONFIG_PARAMETER_SIGNATURE);
}

void setHellen72etb(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	CONFIG(etbIo[0].directionPin1) = GPIOC_6;
	CONFIG(etbIo[0].directionPin2) = GPIOC_7;
	engineConfiguration->etb_use_two_wires = true;
}

void setHellenDefaultVrThresholds(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	for (int i = 0;i<VR_THRESHOLD_COUNT;i++) {
		setLinearCurve(engineConfiguration->vrThreshold[i].rpmBins, 600 / RPM_1_BYTE_PACKING_MULT, 7000 / RPM_1_BYTE_PACKING_MULT, 100 / RPM_1_BYTE_PACKING_MULT);
		setLinearCurve(engineConfiguration->vrThreshold[i].values, PACK_PERCENT_BYTE_MULT * 0.6, PACK_PERCENT_BYTE_MULT * 1.2, PACK_PERCENT_BYTE_MULT * 0.1);
	}
}

void proteusHarley(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->luaOutputPins[0] = PROTEUS_LS_16;
#if HW_PROTEUS
	strncpy(config->luaScript, R"(
startPwm(0, 100, 0)
function onTick()
	rpm = getSensor("RPM")
-- handle nil RPM, todo: change firmware to avoid nil RPM
	rpm = (rpm == nil and 0 or rpm)
    print('Rpm ' .. rpm)
	print('getTimeSinceTriggerEventMs ' .. getTimeSinceTriggerEventMs())

	enableCompressionReleaseSolenoid = getTimeSinceTriggerEventMs() < 5000 and rpm < 300
	setPwmDuty(0, enableCompressionReleaseSolenoid and 100 or 0)
end
)", efi::size(config->luaScript));
#endif
}

void proteusLuaDemo(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
#if HW_PROTEUS
	engineConfiguration->tpsMin = 889;
	engineConfiguration->tpsMax = 67;

	engineConfiguration->tps1SecondaryMin = 105;
	engineConfiguration->tps1SecondaryMax = 933;

	strcpy(engineConfiguration->scriptCurveName[2 - 1], "rateofchange");

	strcpy(engineConfiguration->scriptCurveName[3 - 1], "bias");

	/**
	 * for this demo I use ETB just a sample object to control with PID. No reasonable person should consider actually using
	 * Lua for actual intake ETB control while driving around the racing track - hard-coded ETB control is way smarter!
	 */
	static const float defaultBiasBins[] = {
		0, 1, 2, 4, 7, 98, 99, 100
	};
	static const float defaultBiasValues[] = {
		-20, -18, -17, 0, 20, 21, 22, 25
	};

	engineConfiguration->luaOutputPins[0] = GPIOD_12;
	engineConfiguration->luaOutputPins[1] = GPIOD_10;
	engineConfiguration->luaOutputPins[2] = GPIOD_11;

	setLinearCurve(CONFIG(scriptCurve2Bins), 0, 8000, 1);
	setLinearCurve(CONFIG(scriptCurve2), 0, 100, 1);

	copyArray(CONFIG(scriptCurve3Bins), defaultBiasBins);
	copyArray(CONFIG(scriptCurve3), defaultBiasValues);

	engineConfiguration->auxAnalogInputs[0] = PROTEUS_IN_ANALOG_VOLT_10;
	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;


	// ETB direction #1 PD10
	engineConfiguration->etbIo[0].directionPin1 = GPIO_UNASSIGNED;
	// ETB control PD12
	engineConfiguration->etbIo[0].controlPin = GPIO_UNASSIGNED;
	// ETB disable PD11
	engineConfiguration->etbIo[0].disablePin = GPIO_UNASSIGNED;

/**
controlIndex = 0
directionIndex = 1

  print('pid output ' .. output)
  print('')



  local duty = (bias + output) / 100

--  isPositive = duty > 0;
--  pwmValue = isPositive and duty or -duty
--  setPwmDuty(controlIndex, pwmValue)

--  dirValue = isPositive and 1 or 0;
--  setPwmDuty(directionIndex, dirValue)

--  print('pwm ' .. pwmValue .. ' dir ' .. dirValue)

 *
 */

	auto script = R"(

startPwm(0, 800, 0.1)
-- direction
startPwm(1, 80, 1.0)
-- disable
startPwm(2, 80, 0.0)

pid = Pid.new(2, 0, 0, -100, 100)

biasCurveIndex = findCurveIndex("bias")

canRxAdd(0x600)

voltageFromCan = nil
canRxAdd(0x600)

function onCanRx(bus, id, dlc, data)
  print('got CAN id=' .. id .. ' dlc='  .. dlc)
  voltageFromCan =   data[2] / 256.0 + data[1]
end

function onTick()
  local targetVoltage = getAuxAnalog(0)
  
--  local target = interpolate(1, 0, 3.5, 100, targetVoltage)
  local target = interpolate(1, 0, 3.5, 100, voltageFromCan)
-- clamp 0 to 100
  target = math.max(0, target)
  target = math.min(100, target)

  print('Decoded target: ' .. target)

  local tps = getSensor("TPS1")
  tps = (tps == nil and 'invalid TPS' or tps)
  print('Tps ' .. tps)

  local output = pid:get(target, tps)

  local bias = curve(biasCurveIndex, target)
  print('bias ' .. bias)

  local duty = (bias + output) / 100
  isPositive = duty > 0;
  pwmValue = isPositive and duty or -duty
  setPwmDuty(0, pwmValue)

  dirValue = isPositive and 1 or 0;
  setPwmDuty(1, dirValue)

  print('pwm ' .. pwmValue .. ' dir ' .. dirValue)
  print('')
end
				)";
	strncpy(config->luaScript, script, efi::size(config->luaScript));
#endif
}

#if HW_HELLEN
void setHellen144LedPins(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
#ifdef EFI_COMMUNICATION_PIN
	engineConfiguration->communicationLedPin = EFI_COMMUNICATION_PIN;
#else
	engineConfiguration->communicationLedPin = GPIOE_7;
#endif /* EFI_COMMUNICATION_PIN */
	engineConfiguration->runningLedPin = GPIOG_1;
	engineConfiguration->warningLedPin = GPIOE_8;
}

void setHellen176LedPins(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
#ifdef EFI_COMMUNICATION_PIN
	engineConfiguration->communicationLedPin = EFI_COMMUNICATION_PIN;
#else
	engineConfiguration->communicationLedPin = GPIOH_10;
#endif /* EFI_COMMUNICATION_PIN */
	engineConfiguration->runningLedPin = GPIOH_9;  // green
	engineConfiguration->warningLedPin = GPIOH_11; // yellow
}
#endif //HW_HELLEN
