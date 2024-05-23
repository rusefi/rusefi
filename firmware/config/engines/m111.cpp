/*
 * m111.cpp
 *
 * @date Dec 14, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_configuration.h"
#include "m111.h"
#include "thermistors.h"
#include "defaults.h"

#if HW_PROTEUS
#include "proteus_meta.h"
#endif // HW_PROTEUS

#if defined(HW_MICRO_RUSEFI)
#include "mre_meta.h"
#endif // HW_MICRO_RUSEFI

void setMercedesM111EngineConfiguration() {
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;
	engineConfiguration->displacement = 2.295f;
	engineConfiguration->cylinderBore = 89.9;
	engineConfiguration->vehicleWeight = 1250;
	engineConfiguration->injector.flow = 196;
	engineConfiguration->enableCanVss = true;
	engineConfiguration->canVssNbcType = W202;
	engineConfiguration->canNbcType = CAN_BUS_W202_C180;

#if defined(HW_HELLEN)
	engineConfiguration->enableSoftwareKnock = true;
#endif

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_MERCEDES);
	strcpy(engineConfiguration->engineCode, "M111");

    engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engineConfiguration->globalTriggerAngleOffset = 109 + 360; // please use a timing light?

    // todo: i wonder if we have less custom curve for same sensor?
    setAtSensor(&engineConfiguration->iat, /*temp low*/0, 7400, /*temp mid*/22, 2180, /*temp high*/ 100, 180);
    // todo: i wonder if these custom IAT and CLT curves are effectively same?
    setAtSensor(&engineConfiguration->clt, /*temp low*/0, 5750, /*temp mid*/30, 1750, /*temp high*/ 50, 750);

	setCustomMap(/*lowValue*/ 20, 0.3, 250, 4.65);

	engineConfiguration->idle.solenoidPin = Gpio::Unassigned;
	engineConfiguration->etb.pFactor = 5.12;
	engineConfiguration->etb.iFactor = 47;
	engineConfiguration->etb.dFactor = 0.088;
	engineConfiguration->etb.offset = 0;

    strcpy(engineConfiguration->gpPwmNote[0], "SC Bypass");
#if defined(HW_MICRO_RUSEFI) && EFI_PROD_CODE
	gppwm_channel *scBypass = &engineConfiguration->gppwm[0];
    scBypass->pin = MRE_GPOUT_3;
#endif // HW_MICRO_RUSEFI

#if defined(HW_MICRO_RUSEFI)
	gppwm_channel *scClutch = &engineConfiguration->gppwm[1];
    scClutch->pin = MRE_LS_2;
    engineConfiguration->vvtPins[0] = MRE_LS_1;
#endif // HW_MICRO_RUSEFI
    strcpy(engineConfiguration->gpPwmNote[1], "SC Clutch");



#if defined(HW_MICRO_RUSEFI)
	engineConfiguration->fuelPumpPin = MRE_GPOUT_1; // more or less MRE default
	engineConfiguration->fanPin = MRE_GPOUT_2; // more or less MRE default
    setPPSInputs(MRE_IN_PPS, MRE_IN_PPS2);
    setTPS1Inputs(MRE_IN_TPS, MRE_IN_TPS2);

    engineConfiguration->lowPressureFuel.hwChannel = MRE_IN_ANALOG_VOLT_4;
    // assumes R30 is populated
    engineConfiguration->boostControlPin = MRE_AV9_REUSE;
#endif // HW_MICRO_RUSEFI
    // note how these numbers are very flipped hyundai154 defaults?
    setTPS1Calibration(891, 69, 98, 926);
    // honda cable position sensor
    setPPSCalibration(0.38, 4.77, 4.64, 2.47);

#if HW_PROTEUS
    engineConfiguration->triggerInputPins[0] = PROTEUS_VR_1;
    engineConfiguration->camInputs[0] = PROTEUS_DIGITAL_2;

    setProteusEtbIO();
#endif // HW_PROTEUS
}
