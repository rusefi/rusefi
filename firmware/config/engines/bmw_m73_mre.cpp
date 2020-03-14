/*
 * @file bmw_m73_mre.cpp
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
 * ECU pin 4:  GND BRN/ORG BLK
 * ECU pin 6:  GND BRN     BLK
 * ECU pin 7:  IN  RED     RED +12v hot at all times
 * ECU pin 8:  IN  RED/BLU RED +12v from ECU relay
 *
 * Plug #2 24 pin
 *
 *
 *
 *
 * Plug #3 52 pin
 * ECU pin 32: IN          WHT VR positive crankshaft sensor
 * ECU pin 40: OUT BRN/BLK GRN injector #3
 * ECU pin 41: OUT BRN/WHT BLU injector #1
 * ECU pin 46: IN  BLK     BLU VR negative crankshaft sensor
 *
 *
 *
 * Plug #5 9 pin
 * ECU pic 3:  OUT BLK     ORG coil signal
 *
 * BMW_M73_MRE
 * set engine_type 104
 *
 */

#include "bmw_m73.h"

EXTERN_CONFIG;

void setEngineBMW_M73_microRusEfi(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	// 13641435991 injector
	engineConfiguration->injector.flow = 180; // cc/min, who knows if this number is real - no good source of info


	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.displacement = 5.4 / 2;
	engineConfiguration->specs.firingOrder = FO_1_5_3_6_2_4;

	engineConfiguration->ignitionMode = IM_ONE_COIL;

	engineConfiguration->injectionMode = IM_BATCH;


}
