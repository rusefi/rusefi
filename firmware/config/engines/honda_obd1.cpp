#include "pch.h"

void setHondaObd1() {
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_HONDA);

	engineConfiguration->map.sensor.type = MT_DENSO183;
	engineConfiguration->ignitionMode = IM_ONE_COIL;

	// Keihin 06164-P0A-A00
	engineConfiguration->injector.flow = 248;
}
