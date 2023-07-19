/*
https://rusefi.com/docs/pinouts/hellen/hellen-112-17/
*/

#include "pch.h"
#include "defaults.h"
#include "proteus_meta.h"

// set engine_type 33
void setSlingshot() {
	strcpy(engineConfiguration->engineMake, "Polaris");
    setInline4();
    engineConfiguration->injectionMode = IM_SEQUENTIAL;

    engineConfiguration->ignitionMode = IM_WASTED_SPARK; // just for now
    engineConfiguration->twoWireBatchIgnition = true;

	engineConfiguration->map.sensor.type = MT_CUSTOM;
	// GM TMAP is recommended
	engineConfiguration->map.sensor.lowValue = 20;
   	engineConfiguration->mapLowValueVoltage = 0.3;
  	engineConfiguration->map.sensor.highValue = 250;
   	engineConfiguration->mapHighValueVoltage = 4.65;
#if HW_PROTEUS
	setProteusEtbIO();
#endif // HW_PROTEUS
}
