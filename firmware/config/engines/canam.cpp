
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
    engineConfiguration->camInputs[0] = PROTEUS_DIGITAL_6;
   	engineConfiguration->vehicleSpeedSensorInputPin = PROTEUS_DIGITAL_5;
	engineConfiguration->auxAnalogInputs[0] = PROTEUS_IN_ANALOG_VOLT_4;
	setPPSCalibration(0.25, 0.5, 1.5, 3);
	// todo: matches Hyundai TODO extract method?
	setTPS1Calibration(98, 926, 891, 69);
	setProteusEtbIO();
	engineConfiguration->starterControlPin = Gpio::PROTEUS_LS_14;
	engineConfiguration->startStopButtonPin = PROTEUS_IN_AV_6_DIGITAL;

	engineConfiguration->boostControlPin = Gpio::PROTEUS_LS_16;

	gppwm_channel *icFanPwm = &engineConfiguration->gppwm[0];
	icFanPwm->pin = Gpio::PROTEUS_LS_15;

	gppwm_channel *accRelayPwm = &engineConfiguration->gppwm[1];
	accRelayPwm->pin = Gpio::PROTEUS_LS_4;
#endif // HW_PROTEUS

	strcpy(engineConfiguration->gpPwmNote[0], "IC Fan");
	strcpy(engineConfiguration->gpPwmNote[1], "Acc Relay");


#if HW_PROTEUS
    #include "canam_2021.lua"
#endif // HW_PROTEUS

}