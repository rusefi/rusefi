/*
 * @file engine_controller_misc.cpp
 *
 * @date Mar 22, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_controller.h"
#include "perf_trace.h"
#include "os_access.h"
#include "settings.h"

EXTERN_ENGINE;

extern LoggingWithStorage sharedLogger;
extern ButtonDebounce startStopButtonDebounce;

#if ! EFI_PROD_CODE
extern bool mockPinStates[(1 << sizeof(brain_pin_e))];
#endif

#if ENABLE_PERF_TRACE
static uint8_t nextThreadId = 0;
void threadInitHook(void* vtp) {
	// No lock required, this is already under lock
	auto tp = reinterpret_cast<thread_t*>(vtp);
	tp->threadId = ++nextThreadId;
}

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
void threadInitHook(void*) {}
void irqEnterHook() {}
void irqExitHook() {}
void contextSwitchHook() {}
#endif /* ENABLE_PERF_TRACE */

#if EFI_ENABLE_MOCK_ADC
void setMockVoltage(int hwChannel, float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	engine->engineState.mockAdcState.setMockVoltage(hwChannel, voltage PASS_ENGINE_PARAMETER_SUFFIX);
}

void setMockCltVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setMockVoltage(engineConfiguration->clt.adcChannel, voltage PASS_ENGINE_PARAMETER_SUFFIX);
}

void setMockIatVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setMockVoltage(engineConfiguration->iat.adcChannel, voltage PASS_ENGINE_PARAMETER_SUFFIX);
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

void setMockVBattVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setMockVoltage(engineConfiguration->vbattAdcChannel, voltage PASS_ENGINE_PARAMETER_SUFFIX);
}

void setMockState(brain_pin_e pin, bool state DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if ! EFI_PROD_CODE
	mockPinStates[static_cast<int>(pin)] = state;
#endif
}

#endif /* EFI_ENABLE_MOCK_ADC */

#if EFI_PROD_CODE
/**
 * 64-bit result would not overflow, but that's complex stuff for our 32-bit MCU
 */
//todo: macro to save method invocation
efitimeus_t getTimeNowUs(void) {
	ScopePerf perf(PE::GetTimeNowUs);
	return getTimeNowNt() / (CORE_CLOCK / 1000000);
}

volatile uint32_t lastLowerNt = 0;
volatile uint32_t upperTimeNt = 0;

efitick_t getTimeNowNt(void) {
	chibios_rt::CriticalSectionLocker csl;

	uint32_t stamp = getTimeNowLowerNt();

	// Lower 32 bits of the timer has wrapped - time to step upper bits
	if (stamp < lastLowerNt) {
		upperTimeNt++;
	}

	lastLowerNt = stamp;

	return ((int64_t)upperTimeNt << 32) | stamp;
}

static void onStartStopButtonToggle(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engine->startStopStateToggleCounter++;

	if (engine->rpmCalculator.isStopped(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		bool wasStarterEngaged = enginePins.starterControl.getAndSet(1);
		if (!wasStarterEngaged) {
			scheduleMsg(&sharedLogger, "Let's crank this engine for up to %dseconds!", CONFIG(startCrankingDuration));
		}
	} else if (engine->rpmCalculator.isRunning()) {
		scheduleMsg(&sharedLogger, "Let's stop this engine!");
		scheduleStopEngine();
	}
}

void slowStartStopButtonCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_PROD_CODE
	bool startStopState = startStopButtonDebounce.readPinEvent();

	if (startStopState && !engine->startStopState) {
		// we are here on transition from 0 to 1
		onStartStopButtonToggle(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
	engine->startStopState = startStopState;
#endif /* EFI_PROD_CODE */

	// todo: should this be simply FSIO?
	if (engine->rpmCalculator.isRunning()) {
		// turn starter off once engine is running
		bool wasStarterEngaged = enginePins.starterControl.getAndSet(0);
		if (wasStarterEngaged) {
			scheduleMsg(&sharedLogger, "Engine runs we can disengage the starter");
		}
	}
}


#endif /* EFI_PROD_CODE */
