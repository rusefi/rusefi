/*
 * @file bmw_m73.cpp
 *
 *
 * https://github.com/rusefi/rusefi_documentation/wiki/BMW_e38_750
 *
 * https://rusefi.com/wiki/index.php?title=Hardware:OEM_connectors#134_pin
 *
 *
 * 40 pin section:
 * ECU pin 6:  IN start signal from ignition key
 * ECU pin 17: OUT BLK engine speed output for gauge cluster
 * ECU pin 26: IN GRN/BLK +12 hot in start & run
 *
 * 52 pin section:
 * ECU pin 2:  OUT injector #4
 * ECU pin 6:  GND ECU
 * ECU pin 15: OUT injector #2
 * ECU pin 20: IN  hall effect camshaft sensor signal
 * ECU pin 21: GND CLT sensor
 * ECU pin 22: IN  CLT sensor
 * ECU pin 27: OUT injector #6
 * ECU pin 28: OUT injector #5
 * ECU pin 32: IN  VR crankshaft sensor
 * ECU pin 40: OUT injector #3
 * ECU pin 41: OUT injector #1
 * ECU pin 45: GND crankshaft shield
 * ECU pin 46: IN  VR crankshaft sensor
 *
 *
 * 9 pin section:
 * ECU pin 4: GND
 * ECU pin 6: GND
 * ECU pin 7: IN  RED     +12v hot at all times
 * ECU pin 8: IN  RED/BLU
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
}
