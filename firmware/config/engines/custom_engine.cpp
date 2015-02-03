/**
 * @file	custom_engine.cpp
 *
 * @date Jan 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef CONFIG_ENGINES_CUSTOM_ENGINE_CPP_
#define CONFIG_ENGINES_CUSTOM_ENGINE_CPP_

#include "custom_engine.h"
#include "honda_accord.h"

void setCustomEngineConfiguration(engine_configuration_s *engineConfiguration) {
	engineConfiguration->trigger.type = TT_ONE_PLUS_ONE;

	board_configuration_s * boardConfiguration = &engineConfiguration->bc;
	setFrankenso_01_LCD(boardConfiguration);
	setFrankenso0_1_joystick(engineConfiguration);

	/**
	 * Frankenso analog #1 PC2 ADC12
	 * Frankenso analog #2 PC1 ADC11
	 * Frankenso analog #3
	 * Frankenso analog #4 PC3 ADC13
	 * Frankenso analog #5
	 * Frankenso analog #6
	 * Frankenso analog #7 PA4 ADC4
	 * Frankenso analog #8
	 * Frankenso analog #9
	 * Frankenso analog #10 PA6 ADC6
	 * Frankenso analog #11 PC5 ADC15
	 * Frankenso analog #12 VBatt
	 */


	/**
	 * http://rusefi.com/wiki/index.php?title=Manual:Hardware_Frankenso_board
	 */
	// Frankenso low out #1: PE6
	// Frankenso low out #2: PE5
	// Frankenso low out #3: PD7
	// Frankenso low out #4: PC13 Idle valve solenoid
	// Frankenso low out #5: PE3
	// Frankenso low out #6: PE4
	// Frankenso low out #7: PE1 (do not use with discovery!)
	// Frankenso low out #8: PE2 injector #2
	// Frankenso low out #9: PB9 injector #1
	// Frankenso low out #10: PE0 (do not use with discovery!)
	// Frankenso low out #11: PB8 injector #3
	// Frankenso low out #12: PB7 injector #4

	boardConfiguration->idleValvePin = GPIOC_13;

	boardConfiguration->fanPin = GPIOE_5;

	boardConfiguration->injectionPins[0] = GPIOB_9; // #1
	boardConfiguration->injectionPins[1] = GPIOE_2; // #2
	boardConfiguration->injectionPins[2] = GPIOB_8; // #3
	boardConfiguration->injectionPins[3] = GPIOB_7; // #4

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
	engineConfiguration->cylindersCount = 12;

	engineConfiguration->displayMode = DM_NONE;
#else
	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[6] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[7] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[8] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[9] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[10] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[11] = GPIO_UNASSIGNED;

	boardConfiguration->ignitionPins[0] = GPIOC_7;
	boardConfiguration->ignitionPins[1] = GPIOE_4; // todo: update this value
	boardConfiguration->ignitionPins[2] = GPIOE_0; // todo: update this value
	boardConfiguration->ignitionPins[3] = GPIOE_1; // todo: update this value
#endif

}

#endif /* CONFIG_ENGINES_CUSTOM_ENGINE_CPP_ */
