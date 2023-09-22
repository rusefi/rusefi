
#include "pch.h"
#include "defaults.h"
#include "proteus_meta.h"
#include "canam.h"
#include "lua_lib.h"

// set engine_type 54
// https://www.youtube.com/watch?v=j8DOFp02QDY
void setMaverickX3() {
    strcpy(engineConfiguration->engineMake, "Rotax");
    strcpy(engineConfiguration->engineCode, "900 ACE");

    engineConfiguration->cylindersCount = 3;
    engineConfiguration->firingOrder = FO_1_2_3;
    engineConfiguration->displacement = 0.9;
    engineConfiguration->injectionMode = IM_SEQUENTIAL;

#if HW_PROTEUS
	setPPSCalibration(0.25, 0.5, 1.5, 3);
	// todo: matches Hyundai TODO extract method?
	setTPS1Calibration(98, 926, 891, 69);
	setProteusEtbIO();
	engineConfiguration->starterControlPin = Gpio::PROTEUS_LS_14;
	engineConfiguration->startStopButtonPin = PROTEUS_IN_AV_6_DIGITAL;

#endif // HW_PROTEUS

#if HW_PROTEUS
    #include "canam_2021.lua"
#endif // HW_PROTEUS

}