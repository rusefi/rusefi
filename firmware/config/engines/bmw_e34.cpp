/**
 * @file bmw_e34.cpp
 *
 * set_engine_type 25
 *
 * Coil pairs: 1-6, 2-5, 3-4
 *
 * Injector groups: 1,2,3 è 4,5,6
 *
 * @date Nov 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "global.h"
#include "bmw_e34.h"

EXTERN_ENGINE;

void setBmwE34(DECLARE_ENGINE_PARAMETER_F) {
	board_configuration_s * boardConfiguration = &engineConfiguration->bc;

	engineConfiguration->algorithm = LM_PLAIN_MAF;
	engineConfiguration->injector.flow = 750;

	boardConfiguration->tunerStudioThreadPeriod = 50;
	engineConfiguration->rpmHardLimit = 6000;
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);

	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_60_2;
	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.firingOrder = FO_1_THEN_5_THEN_3_THEN_6_THEN_2_THEN_4;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

	engineConfiguration->ignMathCalculateAtIndex = 15;

	setConstantDwell(engineConfiguration, 3); // a bit shorter dwell
	engineConfiguration->useConstantDwellDuringCranking = true;
	engineConfiguration->ignitionDwellForCrankingMs = 5;

	// todo: check the digital sniffer while simulating
	// set_global_trigger_offset_angle 84
	engineConfiguration->globalTriggerAngleOffset = 84;

	setWholeFuelMap(6 PASS_ENGINE_PARAMETER);
	setWholeTimingTable(10 PASS_ENGINE_PARAMETER);

	board_configuration_s *bc = &engineConfiguration->bc;
	bc->malfunctionIndicatorPin = GPIO_UNASSIGNED;

//	bc->isFastAdcEnabled = true;

	bc->ignitionPinMode = OM_INVERTED;
	bc->ignitionPins[0] = GPIOC_7; // #1
	bc->ignitionPins[1] = GPIO_UNASSIGNED; // #2
	bc->ignitionPins[2] = GPIOE_4; // #3
	bc->ignitionPins[3] = GPIO_UNASSIGNED; // #4
	bc->ignitionPins[4] = GPIOC_9; // #5
	bc->ignitionPins[5] = GPIO_UNASSIGNED; // #6

	bc->injectionPinMode = OM_INVERTED;

	bc->injectionPins[0] = GPIOB_9; // #1
	bc->injectionPins[1] = GPIOB_8; // #2
	/**
	 * this is a relatively dirty performance hack: with unassigned pin we
	 * have less stuff to put into wave chart
	 */
	bc->injectionPins[2] = GPIO_UNASSIGNED; // #3
	bc->injectionPins[3] = GPIO_UNASSIGNED; // #4
	bc->injectionPins[4] = GPIO_UNASSIGNED; // #5
	bc->injectionPins[5] = GPIO_UNASSIGNED; // #6

	bc->triggerErrorPin = GPIOE_3;

	engineConfiguration->clutchUpPin = GPIOD_3;
	engineConfiguration->clutchUpPinMode = PI_PULLUP;

	boardConfiguration->fuelPumpPin = GPIOC_13;

	boardConfiguration->idleValvePin = GPIOB_10;
	boardConfiguration->idleValvePinMode = OM_INVERTED;
	boardConfiguration->idleSolenoidFrequency = 300;
	// set_idle_pwm 50
	boardConfiguration->idleSolenoidPwm = 0.5;

	// turbocharger boost control solenoid: TODO output: GPIOE_6
	// water injection #1 TODO GPIOD_7
	// water injection #2 TODO GPIOE_2


	/**
	 * emulating the 60-0 trigger takes some resources, let's keep it slow by default
	 * rpm 200
	 */
	bc->triggerSimulatorFrequency = 200;

	engineConfiguration->map.sensor.type = MT_MPX4250;

	engineConfiguration->hasCltSensor = false;
	engineConfiguration->hasIatSensor = false;

	/**
	 * This saves a couple of ticks in trigger emulation methods
	 * TODO: add some smart logic to detect unneeded trigger simulation pins?
	 * TODO: but probably not worth it
	 */
	bc->triggerSimulatorPins[1] = GPIO_UNASSIGNED;
	bc->triggerSimulatorPins[2] = GPIO_UNASSIGNED;
}
