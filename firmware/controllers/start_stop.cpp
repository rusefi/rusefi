#include "pch.h"

#include "start_stop.h"

static ButtonDebounce startStopButtonDebounce("start_button");

void initStartStopButton() {
	/* startCrankingDuration is efitimesec_t, so we need to multiply it by 1000 to get milliseconds*/
	startStopButtonDebounce.init((engineConfiguration->startCrankingDuration*1000), engineConfiguration->startStopButtonPin, engineConfiguration->startStopButtonMode);
}

static void onStartStopButtonToggle() {
	engine->engineState.startStopStateToggleCounter++;

	if (engine->rpmCalculator.isStopped()) {
		bool wasStarterEngaged = enginePins.starterControl.getAndSet(1);
		if (!wasStarterEngaged) {
		    engine->startStopStateLastPush.reset();
		    efiPrintf("Let's crank this engine for up to %d seconds via %s!",
		    		engineConfiguration->startCrankingDuration,
					hwPortname(engineConfiguration->starterControlPin));
		}
	} else if (engine->rpmCalculator.isRunning()) {
		efiPrintf("Let's stop this engine!");
		doScheduleStopEngine();
	}
}

static void disengageStarterIfNeeded() {
	if (engine->rpmCalculator.isRunning()) {
		// turn starter off once engine is now running!
		bool wasStarterEngaged = enginePins.starterControl.getAndSet(0);
		if (wasStarterEngaged) {
			efiPrintf("Engine runs we can disengage the starter");
		}
	} else {
    	if (engine->startStopStateLastPush.hasElapsedSec(engineConfiguration->startCrankingDuration)) {
    		bool wasStarterEngaged = enginePins.starterControl.getAndSet(0);
    		if (wasStarterEngaged) {
    			efiPrintf("Cranking timeout %d seconds", engineConfiguration->startCrankingDuration);
    		}
    	}
    }
}

void slowStartStopButtonCallback() {
	bool startStopState = startStopButtonDebounce.readPinEvent();

	if (startStopState && !engine->engineState.startStopState) {
		// we are here on transition from 0 to 1
		// TODO: huh? looks like 'stop engine' feature is broken?! we invoke 'toggle' method under "from off to on" condition?!
		onStartStopButtonToggle();
	}
	engine->engineState.startStopState = startStopState;

    bool isStarterEngaged = enginePins.starterControl.getLogicValue();

	if (isStarterEngaged) {
	    disengageStarterIfNeeded();
   	}
}
