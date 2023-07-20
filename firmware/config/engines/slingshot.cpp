/*
https://rusefi.com/docs/pinouts/hellen/hellen-112-17/
*/

#include "pch.h"
#include "defaults.h"
#include "proteus_meta.h"

// set engine_type 33
// ME17_9_MISC
void setSlingshot() {
	strcpy(engineConfiguration->engineMake, "Polaris");
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_4_3_2; // yes WOW that's not the most popular firing order!!!
    engineConfiguration->injectionMode = IM_SEQUENTIAL;

    engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL_36_1;
    engineConfiguration->vvtMode[0] = VVT_BOSCH_QUICK_START;

    engineConfiguration->ignitionMode = IM_WASTED_SPARK; // just for now
    engineConfiguration->twoWireBatchIgnition = true;
    engineConfiguration->globalTriggerAngleOffset = 110;

	//engineConfiguration->map.sensor.type = MT_GM_1_BAR;
	engineConfiguration->map.sensor.type = MT_CUSTOM;
	// GM TMAP is recommended
	engineConfiguration->map.sensor.lowValue = 20;
   	engineConfiguration->mapLowValueVoltage = 0.3;
  	engineConfiguration->map.sensor.highValue = 250;
   	engineConfiguration->mapHighValueVoltage = 4.65;
#if HW_PROTEUS
	setProteusEtbIO();
	engineConfiguration->invertCamVVTSignal = true;
#endif // HW_PROTEUS
}
