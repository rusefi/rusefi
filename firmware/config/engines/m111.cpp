/*
 * m111.cpp
 *
 * @date Dec 14, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */


#include "m111.h"
#include "thermistors.h"
#include "defaults.h"

#if HW_PROTEUS
#include "proteus_meta.h"
#endif // HW_PROTEUS

#include "mre_meta.h"

void setM111EngineConfiguration() {
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->specs.displacement = 2.295f;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_MERCEDES);
	strcpy(engineConfiguration->engineCode, "M111");

    engineConfiguration->vvtMode[0] = VVT_FIRST_HALF;
	engineConfiguration->globalTriggerAngleOffset = 109; // please use a timing light?

    setAtSensor(&engineConfiguration->iat, /*temp low*/0, 7400, /*temp mid*/22, 2180, /*temp high*/ 100, 180);

	engineConfiguration->map.sensor.type = MT_CUSTOM;
	// GM TMAP is recommended
	engineConfiguration->map.sensor.lowValue = 20;
   	engineConfiguration->mapLowValueVoltage = 0.3;
  	engineConfiguration->map.sensor.highValue = 250;
   	engineConfiguration->mapHighValueVoltage = 4.65;

	engineConfiguration->etb.pFactor = 5.12;
	engineConfiguration->etb.iFactor = 47;
	engineConfiguration->etb.dFactor = 0.088;
	engineConfiguration->etb.offset = 0;
}

void setMreM111EngineConfiguration() {
    setM111EngineConfiguration();

    setPPSInputs(MRE_IN_PPS, MRE_IN_PPS2);
    setTPS1Inputs(MRE_IN_TPS, MRE_IN_TPS2);
    // note how these numbers are very flipped hyundai154 defaults?
    setTPS1Calibration(891, 69, 98, 926);
    // honda cable position sensor
    setPPSCalibration(0.38, 4.77, 4.64, 2.47);
}

void setProteusM111EngineConfiguration() {
    setM111EngineConfiguration();
#if HW_PROTEUS
    engineConfiguration->triggerInputPins[0] = PROTEUS_VR_1;
    engineConfiguration->camInputs[0] = PROTEUS_DIGITAL_2;

    engineConfiguration->tps1_2AdcChannel = PROTEUS_IN_TPS1_2;
    setPPSInputs(PROTEUS_IN_PPS, PROTEUS_IN_PPS2);
#endif // HW_PROTEUS
}