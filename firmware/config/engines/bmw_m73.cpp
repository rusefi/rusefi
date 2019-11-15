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
 * ECU pin 32: IN          ORG VR crankshaft sensor
 * ECU pin 40: OUT BRN/BLK     injector #3
 * ECU pin 41: OUT BRN/WHT     injector #1
 * ECU pin 45: GND             crankshaft shield
 * ECU pin 46: IN  BLK     BLU VR crankshaft sensor
 *
 *
 * Plug #4 40 pin
 * ECU pin 6:  IN              start signal from ignition key
 * ECU pin 17: OUT BLK         engine speed output for gauge cluster
 * ECU pin 26: IN  GRN/BLK RED +12 hot in start & run
 * ECU pin 40: OUT YEL/BRN BRN starter enable
 *
 *
 * set engine_type 40
 *
 *
 * @date Nov 1, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "engine_template.h"
#include "custom_engine.h"

EXTERN_CONFIG;

void setEngineBMW_M73(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->specs.cylindersCount = 12;
	engineConfiguration->specs.displacement = 5.4;
	engineConfiguration->specs.firingOrder = FO_1_7_5_11_3_9_6_12_2_8_4_10;

	boardConfiguration->triggerInputPins[0] = GPIOA_5;
	boardConfiguration->triggerInputPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->camInputs[0] = GPIOC_6;
	engineConfiguration->vvtMode = VVT_FIRST_HALF;

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_60_2_VW;


	// 13641435991 injector
	engineConfiguration->injector.flow = 180; // cc/min, who knows if this number is real - no good source of info
}
