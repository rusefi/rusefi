#include "pch.h"

#include "start_stop.h"

ButtonDebounce startStopButtonDebounce("start_button");

void initStartStopButton() {
	/* startCrankingDuration is efitimesec_t, so we need to multiply it by 1000 to get milliseconds*/
	startStopButtonDebounce.init((engineConfiguration->startCrankingDuration*1000), engineConfiguration->startStopButtonPin, engineConfiguration->startStopButtonMode);
}
