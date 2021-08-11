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
#endif /* EFI_ENABLE_MOCK_ADC */

#if EFI_PROD_CODE
/**
 * 64-bit result would not overflow, but that's complex stuff for our 32-bit MCU
 */
efitimeus_t getTimeNowUs(void) {
	ScopePerf perf(PE::GetTimeNowUs);
	return NT2US(getTimeNowNt());
}

volatile uint32_t lastLowerNt = 0;
volatile uint32_t upperTimeNt = 0;

efitick_t getTimeNowNt() {
	chibios_rt::CriticalSectionLocker csl;

	uint32_t stamp = getTimeNowLowerNt();

	// Lower 32 bits of the timer has wrapped - time to step upper bits
	if (stamp < lastLowerNt) {
		upperTimeNt++;
	}

	lastLowerNt = stamp;

	return ((int64_t)upperTimeNt << 32) | stamp;
}

/*	//Alternative lock free implementation (probably actually slower!)
	// this method is lock-free and thread-safe, that's because the 'update' method
	// is atomic with a critical zone requirement.
	//
	// http://stackoverflow.com/questions/5162673/how-to-read-two-32bit-counters-as-a-64bit-integer-without-race-condition

etitick_t getTimeNowNt() {
	efitime_t localH;
	efitime_t localH2;
	uint32_t localLow;
	int counter = 0;
	do {
		localH = halTime.state.highBits;
		localLow = halTime.state.lowBits;
		localH2 = halTime.state.highBits;
		if (counter++ == 10000)
			chDbgPanic("lock-free frozen");
	} while (localH != localH2);

	// We need to take current counter after making a local 64 bit snapshot
	uint32_t value = getTimeNowLowerNt();

	if (value < localLow) {
		// new value less than previous value means there was an overflow in that 32 bit counter
		localH += 0x100000000LL;
	}

	return localH + value;
}
*/

#endif /* EFI_PROD_CODE */

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
