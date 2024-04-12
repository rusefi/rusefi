#include "pch.h"

/**
 * See test_start_stop.cpp
 */

#include "start_stop.h"

ButtonDebounce startStopButtonDebounce("start_button");

void initStartStopButton() {
	/* startCrankingDuration is efitimesec_t, so we need to multiply it by 1000 to get milliseconds*/
	startStopButtonDebounce.init((engineConfiguration->startCrankingDuration*1000),
	  engineConfiguration->startStopButtonPin,
	  engineConfiguration->startStopButtonMode,
	  engineConfiguration->startRequestPinInverted);
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
    if (getTimeNowMs() < engineConfiguration->startButtonSuppressOnStartUpMs) {
        // where are odd cases of start button combined with ECU power source button we do not want to crank right on start
        return;
    }

	bool startStopState = startStopButtonDebounce.readPinEvent();

	if (startStopState && !engine->engineState.startStopState) {
		// we are here on transition from 0 to 1
		// TODO: huh? looks like 'stop engine' feature is broken?! we invoke 'toggle' method under "from off to on" condition?!
		onStartStopButtonToggle();
	}
	engine->engineState.startStopState = startStopState;
	engine->engineState.startStopPhysicalState = startStopButtonDebounce.getPhysicalState();

    bool isStarterEngaged = enginePins.starterControl.getLogicValue();

	if (isStarterEngaged) {
	    disengageStarterIfNeeded();
   	}
}
