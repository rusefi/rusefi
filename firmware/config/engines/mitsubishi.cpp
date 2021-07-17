/**
 * @file	mitsubishi.cpp
 *
 * MITSU_4G93 16
 *
 * set engine_type 16
 *
 * @date Aug 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "fsio_impl.h"
#include "mitsubishi.h"
#include "allsensors.h"

void setMitsubishiConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setDefaultFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->engineType = MITSU_4G93;

	engineConfiguration->trigger.type = TT_MITSUBISHI; // same trigger as 4G63?

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.displacement = 1.800;

	// set ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	// set global_trigger_offset_angle 671
	engineConfiguration->globalTriggerAngleOffset = 671;

	// set_cranking_rpm 550
	engineConfiguration->cranking.rpm = 550;
	// set cranking_timing_angle 715
	engineConfiguration->crankingTimingAngle = -715;

//	/**
//	 * 29150 Ohm @ 40C
//	 * 10160 Ohm @ 70C
//	 * 1270 Ohm @ 150C
//	 */
//	setThermistorConfiguration(&engineConfiguration->clt, 40, 29150, 70, 10160, 150, 1270);

	setCommonNTCSensor(&engineConfiguration->clt, 2700);

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

	engineConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
	engineConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
	engineConfiguration->injectionPins[2] = GPIOE_3; // Frankenstein: low side - out #8
	engineConfiguration->injectionPins[3] = GPIOE_5; // Frankenstein: low side - out #6


	// Frankenstein: high side #1: PE8
	// Frankenstein: high side #2: PE10

	engineConfiguration->ignitionPins[0] = GPIOE_8; // Frankenstein: high side #1
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIOE_10; // // Frankenstein: high side #2
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;

	engineConfiguration->HD44780width = 20;
	engineConfiguration->HD44780height = 4;

	setEgoSensor(ES_Innovate_MTX_L PASS_CONFIG_PARAMETER_SUFFIX);
#if EFI_FSIO
	setFsioExt(0, GPIOC_9,  RPM_ABOVE_6000_SOLENOID_80_DUTY, 200 PASS_CONFIG_PARAMETER_SUFFIX);
	setFsioExt(1, GPIOC_7,  RPM_ABOVE_2000_SOLENOID_50_DUTY, 200 PASS_CONFIG_PARAMETER_SUFFIX);
#endif /* EFI_FSIO */
}


