/*
 * aux_valves.cpp
 *
 * @date Nov 25, 2017
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "aux_valves.h"
#include "allsensors.h"
#include "trigger_central.h"

EXTERN_ENGINE
;

static scheduling_s turnOnEvent[AUX_DIGITAL_VALVE_COUNT][2];
static scheduling_s turnOffEvent[AUX_DIGITAL_VALVE_COUNT][2];

static void turnOn(void *arg) {

}
static void auxValveTriggerCallback(trigger_event_e ckpSignalType,
		uint32_t index DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_PROD_CODE || EFI_SIMULATOR || defined(__DOXYGEN__)
	if (index != 2) {
		return;
	}
	int rpm = ENGINE(rpmCalculator.rpmValue);

	scheduleByAngle(rpm, &turnOnEvent[0][0], engine->engineState.auxValveStart, (schfunc_t)&turnOn, NULL, &engine->rpmCalculator);
#endif /* EFI_PROD_CODE || EFI_SIMULATOR */
}

void initAuxValves(Logging *sharedLogger) {
#if EFI_PROD_CODE || EFI_SIMULATOR || defined(__DOXYGEN__)
	if (engineConfiguration->auxValves[0] == GPIO_UNASSIGNED) {
		return;
	}
	addTriggerEventListener(auxValveTriggerCallback, "tach", engine);
#endif /* EFI_PROD_CODE || EFI_SIMULATOR */
}

void updateAuxValves(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (engineConfiguration->auxValves[0] == GPIO_UNASSIGNED) {
		return;
	}

	float x = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
	engine->engineState.auxValveStart = interpolate2d("aux", x, engineConfiguration->fsioCurve1Bins,
			engineConfiguration->fsioCurve1, FSIO_CURVE_16);

	engine->engineState.auxValveEnd = interpolate2d("aux", x, engineConfiguration->fsioCurve2Bins,
			engineConfiguration->fsioCurve2, FSIO_CURVE_16);
}

