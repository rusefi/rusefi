#include "pch.h"

void setHondaObd1() {
	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_HONDA);

	engineConfiguration->trigger.type = trigger_type_e::TT_12_TOOTH_CRANK; // 12 crank same as 24 cam
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;

	engineConfiguration->map.sensor.type = MT_DENSO183;
	engineConfiguration->ignitionMode = IM_ONE_COIL;

	// Keihin 06164-P0A-A00
	engineConfiguration->injector.flow = 248;
}
