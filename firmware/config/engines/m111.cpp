/*
 * m111.cpp
 *
 * @date Dec 14, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */


#include "m111.h"

#if HW_PROTEUS
#include "proteus_meta.h"
#endif // HW_PROTEUS

void setM111EngineConfiguration() {
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->specs.displacement = 2.295f;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_MERCEDES);
	strcpy(engineConfiguration->engineCode, "M111");

	engineConfiguration->globalTriggerAngleOffset = 109; // please use a timing light?
}

void setMreM111EngineConfiguration() {
    setM111EngineConfiguration();
}

void setProteusM111EngineConfiguration() {
    setM111EngineConfiguration();
#if HW_PROTEUS
    //engineConfiguration->camInputs[0] = PROTEUS_DIGITAL_2;
#endif // HW_PROTEUS
}