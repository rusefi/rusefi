/*
 * @file engine_controller_misc.cpp
 *
 * @date Mar 22, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "os_access.h"

extern ButtonDebounce startStopButtonDebounce;

static uint8_t nextThreadId = 0;
void threadInitHook(void* vtp) {
	// No lock required, this is already under lock
	auto tp = reinterpret_cast<thread_t*>(vtp);
	tp->threadId = ++nextThreadId;
}

#if ENABLE_PERF_TRACE
void irqEnterHook() {
	perfEventBegin(PE::ISR);
}

void irqExitHook() {
	perfEventEnd(PE::ISR);
}

void contextSwitchHook() {
	perfEventInstantGlobal(PE::ContextSwitch);
}

#else
void irqEnterHook() {}
void irqExitHook() {}
void contextSwitchHook() {}
#endif /* ENABLE_PERF_TRACE */

#if EFI_ENABLE_MOCK_ADC
void setMockVoltage(int hwChannel, float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	engine->engineState.mockAdcState.setMockVoltage(hwChannel, voltage PASS_ENGINE_PARAMETER_SUFFIX);
}

void setMockMafVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setMockVoltage(engineConfiguration->mafAdcChannel, voltage PASS_ENGINE_PARAMETER_SUFFIX);
}

void setMockAfrVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setMockVoltage(engineConfiguration->afr.hwChannel, voltage PASS_ENGINE_PARAMETER_SUFFIX);
}

void setMockMapVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setMockVoltage(engineConfiguration->map.sensor.hwChannel, voltage PASS_ENGINE_PARAMETER_SUFFIX);
}
#endif /* EFI_ENABLE_MOCK_ADC */

#if !EFI_UNIT_TEST
/**
 * 64-bit result would not overflow, but that's complex stuff for our 32-bit MCU
 */
efitimeus_t getTimeNowUs(void) {
	ScopePerf perf(PE::GetTimeNowUs);
	return NT2US(getTimeNowNt());
}


static WrapAround62 timeNt;

efitick_t getTimeNowNt() {
	return timeNt.update(getTimeNowLowerNt());
}
#endif /* !EFI_UNIT_TEST */

static void onStartStopButtonToggle(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engine->startStopStateToggleCounter++;

	if (engine->rpmCalculator.isStopped()) {
		bool wasStarterEngaged = enginePins.starterControl.getAndSet(1);
		if (!wasStarterEngaged) {
		    engine->startStopStateLastPushTime = getTimeNowNt();
		    efiPrintf("Let's crank this engine for up to %d seconds via %s!",
		    		CONFIG(startCrankingDuration),
					hwPortname(CONFIG(starterControlPin)));
		}
	} else if (engine->rpmCalculator.isRunning()) {
		efiPrintf("Let's stop this engine!");
		doScheduleStopEngine(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
}


void slowStartStopButtonCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	bool startStopState = startStopButtonDebounce.readPinEvent();

	if (startStopState && !engine->startStopState) {
		// we are here on transition from 0 to 1
		onStartStopButtonToggle(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
	engine->startStopState = startStopState;

	if (engine->startStopStateLastPushTime == 0) {
   		// nothing is going on with startStop button
   		return;
   	}

	// todo: should this be simply FSIO?
	if (engine->rpmCalculator.isRunning()) {
		// turn starter off once engine is running
		bool wasStarterEngaged = enginePins.starterControl.getAndSet(0);
		if (wasStarterEngaged) {
			efiPrintf("Engine runs we can disengage the starter");
			engine->startStopStateLastPushTime = 0;
		}
	}

	if (getTimeNowNt() - engine->startStopStateLastPushTime > NT_PER_SECOND * CONFIG(startCrankingDuration)) {
		bool wasStarterEngaged = enginePins.starterControl.getAndSet(0);
		if (wasStarterEngaged) {
			efiPrintf("Cranking timeout %d seconds", CONFIG(startCrankingDuration));
			engine->startStopStateLastPushTime = 0;
		}
	}
}
