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

static void auxValveTriggerCallback(trigger_event_e ckpSignalType,
		uint32_t index DECLARE_ENGINE_PARAMETER_SUFFIX) {

}

void initAuxValves(Logging *sharedLogger) {
#if EFI_PROD_CODE || EFI_SIMULATOR || defined(__DOXYGEN__)
	if (engineConfiguration->auxValves[0] == GPIO_UNASSIGNED) {
		return;
	}
	addTriggerEventListener(auxValveTriggerCallback, "tach", engine);
#endif /* EFI_PROD_CODE */
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

