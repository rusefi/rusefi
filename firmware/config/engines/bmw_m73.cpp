/*
 * @file bmw_m73.cpp
 *
 *
 * https://github.com/rusefi/rusefi_documentation/wiki/BMW_e38_750
 *
 * https://rusefi.com/wiki/index.php?title=Hardware:OEM_connectors#134_pin
 *
 * 1/2 plugs black
 * 2/2 plugs grey
 *
 *
 * ********* |    | OEM | rusEfi | function
 *
 * Plug #1 9 pin
 * ECU pin 4:  GND BRN/ORG
 * ECU pin 6:  GND BRN
 * ECU pin 7:  IN  RED         +12v hot at all times
 * ECU pin 8:  IN  RED/BLU     +12v from ECU relay
 *
 * Plug #2 24 pin
 * ECU pin 3:  CAN xxx/xxx     CAN low
 * ECU pin 4:  CAN xxx/xxx     CAN high
 * ECU pin 23: OUT BRN/BLK BLK ECU relay control, low-side
 *
 * Plug #3 52 pin
 * ECU pin 2:  OUT             injector #4
 * ECU pin 6:  GND             ECU
 * ECU pin 15: OUT             injector #2
 * ECU pin 20: IN          WHT hall effect camshaft sensor signal
 * ECU pin 21: GND BRN     BLK CLT sensor
 * ECU pin 22: IN  RED/BRN GRN CLT sensor
 * ECU pin 27: OUT             injector #6
 * ECU pin 28: OUT             injector #5
 * ECU pin 32: IN          ORG VR positive crankshaft sensor - only 2x 5k per channel, R111 not installed, W1002 not installed
 * ECU pin 40: OUT BRN/BLK     injector #3
 * ECU pin 41: OUT BRN/WHT     injector #1
 * ECU pin 45: GND             crankshaft shield
 * ECU pin 46: IN  BLK     BLU VR negative crankshaft sensor
 *
 *
 * Plug #4 40 pin
 * ECU pin 6:  IN              start signal from ignition key
 * ECU pin 17: OUT BLK         engine speed output for gauge cluster
 * ECU pin 26: IN  GRN/BLK RED +12 hot in start & run
 * ECU pin 40: OUT YEL/BRN BRN starter enable
 *
 *
 * Plug #5 9 pin
 * ECU pic 3:  OUT BLK         coil signal, low-side
 * ECU pic 5:  GND BRN         ground
 * ECU pic 6:  OUT BLK         coil signal, low-side
 * ECU pic 9:  OUT BLK         coil signal, low-side
 *
 * Frankenso
 * set engine_type 40
 * Manhattan
 * set engine_type 24
 *
 * @date Nov 1, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine_template.h"
#include "custom_engine.h"

EXTERN_CONFIG;

static void m73engine(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	// 13641435991 injector
	engineConfiguration->injector.flow = 180; // cc/min, who knows if this number is real - no good source of info

	engineConfiguration->specs.cylindersCount = 12;
	engineConfiguration->specs.displacement = 5.4;
	engineConfiguration->specs.firingOrder = FO_1_7_5_11_3_9_6_12_2_8_4_10;

	engineConfiguration->vvtMode = VVT_FIRST_HALF;

	engineConfiguration->globalTriggerAngleOffset = 90;
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_60_2_VW;

	engineConfiguration->ignitionMode = IM_TWO_COILS;
}


// BMW_M73_F
void setEngineBMW_M73_Frankenso(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
	m73engine(PASS_CONFIG_PARAMETER_SIGNATURE);

	boardConfiguration->triggerInputPins[0] = GPIOA_5;
	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->camInputs[0] = GPIOC_6;

	CONFIGB(idle).solenoidPin = GPIO_UNASSIGNED;
	CONFIGB(mainRelayPin) = GPIO_UNASSIGNED;
	CONFIGB(fanPin) = GPIO_UNASSIGNED;
	CONFIGB(fuelPumpPin) = GPIO_UNASSIGNED;


	boardConfiguration->ignitionPins[ID2INDEX(1)] = GPIOE_14; // Frankenso high side - pin 1G
	boardConfiguration->ignitionPins[ID2INDEX(2)] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[ID2INDEX(3)] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[ID2INDEX(4)] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[ID2INDEX(7)] = GPIOC_7; // Frankenso high side - pin 1H


	boardConfiguration->injectionPins[0] = GPIOB_8;
	boardConfiguration->injectionPins[1] = GPIOB_7;
	boardConfiguration->injectionPins[2] = GPIOB_9;
	boardConfiguration->injectionPins[3] = GPIOD_5;
	boardConfiguration->injectionPins[4] = GPIOD_3;
	boardConfiguration->injectionPins[5] = GPIOE_2;

	boardConfiguration->injectionPins[6] = GPIOE_3;
	boardConfiguration->injectionPins[7] = GPIOE_4;
	boardConfiguration->injectionPins[8] = GPIOE_5;
	boardConfiguration->injectionPins[9] = GPIOE_6;
	boardConfiguration->injectionPins[10] = GPIOC_13;
	boardConfiguration->injectionPins[11] = GPIOD_7;
}

// BMW_M73_M
void setEngineBMW_M73_Manhattan(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	m73engine(PASS_CONFIG_PARAMETER_SIGNATURE);

	/**
Nucleo boards - first step is to confirm that I can blink via each pin
going clockwise from top-right corner

GPIOA_10 USD ID
GPIOA_11 USD DM
GPIOA_12 USD DP

E_4: running

Good GPIO:
GPIOC_9 ETB#1
GPIOC_8 ETB#1
GPIOB_8
GPIOB_9
GPIOC_5
GPIOA_7
GPIOA_6
	 */


	CONFIGB(fsioOutputPins)[7] = GPIO_UNASSIGNED;
	boardConfiguration->fuelPumpPin = GPIO_UNASSIGNED;
	boardConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	boardConfiguration->fanPin = GPIO_UNASSIGNED;


	// For example TLE7209 - two control wires:
	// PWM on both wires - one to open, another to close

	// PWM pin
	boardConfiguration->etb1.controlPin1 = GPIO_UNASSIGNED;
	// DIR pin
	boardConfiguration->etb1.directionPin1 = GPIOC_9;
	boardConfiguration->etb1.directionPin2 = GPIOC_8;
	CONFIG(etb1_use_two_wires) = true;


	boardConfiguration->injectionPins[0] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[2] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;

	boardConfiguration->injectionPins[6] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[7] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[8] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[9] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[10] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[11] = GPIO_UNASSIGNED;


}

