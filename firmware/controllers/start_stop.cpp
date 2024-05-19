#include "pch.h"

/**
 * See test_start_stop.cpp
 */

#include "start_stop.h"
#include "ignition_controller.h"

void initStartStopButton() {
	/* startCrankingDuration is efitimesec_t, so we need to multiply it by 1000 to get milliseconds*/
	engine->startStopState.startStopButtonDebounce.init((engineConfiguration->startCrankingDuration*1000),
	  engineConfiguration->startStopButtonPin,
	  engineConfiguration->startStopButtonMode,
	  engineConfiguration->startRequestPinInverted);
}

static void onStartStopButtonToggle() {
	engine->engineState.startStopStateToggleCounter++;

	if (engine->rpmCalculator.isStopped()) {
		bool wasStarterEngaged = enginePins.starterControl.getAndSet(1);
		if (!wasStarterEngaged) {
		    engine->startStopState.startStopStateLastPush.reset();
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
    	if (engine->startStopState.startStopStateLastPush.hasElapsedSec(engineConfiguration->startCrankingDuration)) {
    		bool wasStarterEngaged = enginePins.starterControl.getAndSet(0);
    		if (wasStarterEngaged) {
    			efiPrintf("Cranking timeout %d seconds", engineConfiguration->startCrankingDuration);
    		}
    	}
    }
}

PUBLIC_API_WEAK bool isCrankingSuppressed() {
  return false;
}

void slowStartStopButtonCallback() {
  if (!isIgnVoltage()) {
    // nothing to crank if we are powered only via USB
    engine->startStopState.timeSinceIgnitionPower.reset();
    return;
  } else if (engine->startStopState.isFirstTime) {
    // initialize when first time with proper power
    engine->startStopState.timeSinceIgnitionPower.reset();
    engine->startStopState.isFirstTime = false;
  }

    if (engine->startStopState.timeSinceIgnitionPower.getElapsedUs() < MS2US(engineConfiguration->startButtonSuppressOnStartUpMs)) {
        // where are odd cases of start button combined with ECU power source button we do not want to crank right on start
        return;
    }

    if (engineConfiguration->requireFootOnBrakeToCrank && !engine->brakePedalSwitchedState) {
      return;
    }

    if (isCrankingSuppressed()) {
      return;
    }

	bool startStopState = engine->startStopState.startStopButtonDebounce.readPinEvent();

	if (startStopState && !engine->engineState.startStopState) {
		// we are here on transition from 0 to 1
		// TODO: huh? looks like 'stop engine' feature is broken?! we invoke 'toggle' method under "from off to on" condition?!
		onStartStopButtonToggle();
	}
	// todo: we shall extract start_stop.txt from engine_state.txt
	engine->engineState.startStopState = startStopState;
	engine->engineState.startStopPhysicalState = engine->startStopState.startStopButtonDebounce.getPhysicalState();

    bool isStarterEngaged = enginePins.starterControl.getLogicValue();

	if (isStarterEngaged) {
	    disengageStarterIfNeeded();
   	}
}
