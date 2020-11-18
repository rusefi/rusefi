#include "start_stop.h"
#include "engine.h"

EXTERN_ENGINE;

ButtonDebounce startStopButtonDebounce("start");

void initStartStopButton(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	/* startCrankingDuration is efitimesec_t, so we need to multiply it by 1000 to get milliseconds*/
	startStopButtonDebounce.init((CONFIG(startCrankingDuration)*1000), CONFIG(startStopButtonPin), CONFIG(startStopButtonMode));
}
