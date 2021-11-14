/*
 * @file bmw_m73_mre.cpp
 *
 * https://github.com/rusefi/rusefi/wiki/BMW_e38_750
 *
 * https://rusefi.com/wiki/index.php?title=Hardware:OEM_connectors#134_pin
 * https://github.com/rusefi/rusefi/wiki/HOWTO_electronic_throttle_body
 * Ignition module https://rusefi.com/forum/viewtopic.php?f=4&t=286
 * https://github.com/rusefi/rusefi/wiki/Hardware_microRusEfi_wiring
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
 * ECU pin 23: OUT BRN/BLK BLK ECU relay control, low-side
 *
 *
 *
 *
 * Plug #3 52 pin
 * ECU pin 2:  OUT         ORG injector #4
 * ECU pin 6:  GND             ECU
 * ECU pin 15: OUT         ORG injector #2
 * ECU pin 21: GND BRN     BLK CLT sensor (only on first ECU)
 * ECU pin 22: IN  RED/BRN BLU CLT sensor (only on first ECU)
 * ECU pin 27: OUT         GRN injector #6
 * ECU pin 28: OUT         BLU injector #5
 * ECU pin 32: IN          WHT VR positive crankshaft sensor
 * ECU pin 34: IN          IAT sensor (only on second ECU)
 * ECU pin 40: OUT BRN/BLK GRN injector #3
 * ECU pin 41: OUT BRN/WHT BLU injector #1
 * ECU pin 46: IN  BLK     BLU VR negative crankshaft sensor
 * ECU pin 47: GND BRN     IAT sensor (only on second ECU)
 *
 * Plug #4 40 pin
 * ECU pin 6:  IN          ORG start signal from ignition key. Custom wiring: pulled-up thermistor wire on MRE
 * ECU pin 26: IN  GRN/BLK RED +12v hot in start & run
 * ECU pin 40: OUT YEL/BRN GRN starter enable
 *
 *
 * Plug #5 9 pin
 * ECU pin 3:  OUT BLK     ORG coil signal
 * ECU pin 5:  GND BRN         ground
 * ECU pin 6:  OUT BLK     ORG coil signal
 * ECU pin 9:  OUT BLK     ORG coil signal
 *
 * BMW_M73_MRE
 * set engine_type 104
 * BMW_M73_MRE_SLAVE
 * set engine_type 105
 *
 */

#include "pch.h"

#include "bmw_m73.h"
#include "fsio_impl.h"

void setEngineBMW_M73_microRusEfi(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	// 13641435991 injector
	engineConfiguration->injector.flow = 180; // cc/min, who knows if this number is real - no good source of info

	CONFIG(isFasterEngineSpinUpEnabled) = true;

	strcpy(CONFIG(vehicleName), "microRusEFIx2");

	engineConfiguration->globalTriggerAngleOffset = 90;
	engineConfiguration->specs.cylindersCount = 6;
	engineConfiguration->specs.displacement = 5.4 / 2;
	engineConfiguration->specs.firingOrder = FO_1_5_3_6_2_4;

	engineConfiguration->ignitionMode = IM_ONE_COIL;

	engineConfiguration->injectionMode = IM_BATCH;

	// enable ETB
	// set_rpn_expression 8 "0"
	// TODO LUA setFsio(7, GPIOC_8, "0" PASS_CONFIG_PARAMETER_SUFFIX);


	CONFIG(debugMode) = DBG_ELECTRONIC_THROTTLE_PID;
	engineConfiguration->etb.pFactor = 2.00;
	engineConfiguration->etb.iFactor = 0.35;

	// set debug_mode 37
	// 22 - AN Temp 4, orange wire
	CONFIG(startStopButtonPin) = GPIOA_3;

#if (BOARD_TLE8888_COUNT > 0)
	// "43 - GP Out 4"
	CONFIG(starterControlPin) = TLE8888_PIN_24;
#endif /* BOARD_TLE8888_COUNT */


	engineConfiguration->canNbcType = CAN_BUS_NBC_NONE;
	// set_analog_input_pin pps PA7
	// EFI_ADC_7: "31 - AN volt 3" - PA7
	CONFIG(throttlePedalPositionAdcChannel) = EFI_ADC_7;

	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canWriteEnabled = true;
	CONFIG(enableVerboseCanTx) = true;


	// do I have VR wires flipped?
	engineConfiguration->trigger.type = TT_60_2_VW;

	// this large engine seems to crank at around only 150 RPM? And happily idle at 400RPM?
	engineConfiguration->cranking.rpm = 280;

	CONFIG(crankingTimingAngle) = 30;

	// I am too lazy to add MAP sensor
	engineConfiguration->fuelAlgorithm = LM_ALPHA_N;

	// set cranking_fuel 15
	engineConfiguration->cranking.baseFuel = 15;

	//set tps_min 891
	CONFIG(tpsMin) = 891;
	//set tps_max 177
	CONFIG(tpsMax) = 177;

}
