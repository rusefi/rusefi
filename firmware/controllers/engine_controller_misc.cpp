/*
 * @file engine_controller_misc.cpp
 *
 * @date Mar 22, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_controller.h"
#include "perf_trace.h"
#include "counter64.h"
#include "settings.h"

EXTERN_ENGINE;

extern LoggingWithStorage sharedLogger;

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

void setMockVBattVoltage(float voltage DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setMockVoltage(engineConfiguration->vbattAdcChannel, voltage PASS_ENGINE_PARAMETER_SUFFIX);
}

#endif /* EFI_ENABLE_MOCK_ADC */

#if EFI_PROD_CODE
static Overflow64Counter halTime;

/**
 * 64-bit result would not overflow, but that's complex stuff for our 32-bit MCU
 */
//todo: macro to save method invocation
efitimeus_t getTimeNowUs(void) {
	ScopePerf perf(PE::GetTimeNowUs);
	return getTimeNowNt() / (CORE_CLOCK / 1000000);
}

//todo: macro to save method invocation
efitick_t getTimeNowNt(void) {
#if EFI_PROD_CODE
    /* Entering a reentrant critical zone.*/
    syssts_t sts = chSysGetStatusAndLockX();
	efitime_t localH = halTime.state.highBits;
	uint32_t localLow = halTime.state.lowBits;

	uint32_t value = getTimeNowLowerNt();

	if (value < localLow) {
		// new value less than previous value means there was an overflow in that 32 bit counter
		localH += 0x100000000LL;
	}

	efitime_t result = localH + value;

    /* Leaving the critical zone.*/
    chSysRestoreStatusX(sts);
	return result;
#else /* EFI_PROD_CODE */
// todo: why is this implementation not used?
	/**
	 * this method is lock-free and thread-safe, that's because the 'update' method
	 * is atomic with a critical zone requirement.
	 *
	 * http://stackoverflow.com/questions/5162673/how-to-read-two-32bit-counters-as-a-64bit-integer-without-race-condition
	 */
	efitime_t localH;
	efitime_t localH2;
	uint32_t localLow;
	int counter = 0;
	do {
		localH = halTime.state.highBits;
		localLow = halTime.state.lowBits;
		localH2 = halTime.state.highBits;
#if EFI_PROD_CODE
		if (counter++ == 10000)
			chDbgPanic("lock-free frozen");
#endif /* EFI_PROD_CODE */
	} while (localH != localH2);
	/**
	 * We need to take current counter after making a local 64 bit snapshot
	 */
	uint32_t value = getTimeNowLowerNt();

	if (value < localLow) {
		// new value less than previous value means there was an overflow in that 32 bit counter
		localH += 0x100000000LL;
	}

	return localH + value;
#endif /* EFI_PROD_CODE */

}

void touchTimeCounter() {
	/**
	 * We need to push current value into the 64 bit counter often enough so that we do not miss an overflow
	 */
    /* Entering a reentrant critical zone.*/
    syssts_t sts = chSysGetStatusAndLockX();
	updateAndSet(&halTime.state, getTimeNowLowerNt());
    /* Leaving the critical zone.*/
    chSysRestoreStatusX(sts);
}

static void onStartStopButtonToggle(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engine->startStopStateToggleCounter++;

	if (engine->rpmCalculator.isStopped(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		engine->startStopStateLastPushTime = getTimeNowNt();

		bool wasStarterEngaged = enginePins.starterControl.getAndSet(1);
		if (!wasStarterEngaged) {
			scheduleMsg(&sharedLogger, "Let's crank this engine for up to %dseconds!", CONFIG(startCrankingDuration));
		}
	} else if (engine->rpmCalculator.isRunning(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		scheduleMsg(&sharedLogger, "Let's stop this engine!");
		scheduleStopEngine();
	}
}

static bool isFirstStartStopCallback = true;

void slowStartStopButtonCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (CONFIG(startStopButtonPin) != GPIO_UNASSIGNED) {
#if EFI_PROD_CODE
		bool startStopState = efiReadPin(CONFIG(startStopButtonPin));

		if (isFirstStartStopCallback) {
			// we just remember initial value on first callback and do not react to it
			isFirstStartStopCallback = false;
		} else if (startStopState && !engine->startStopState) {
			// we are here on transition from 0 to 1
			onStartStopButtonToggle(PASS_ENGINE_PARAMETER_SIGNATURE);
		}
		engine->startStopState = startStopState;
#endif /* EFI_PROD_CODE */
	}

	if (engine->startStopStateLastPushTime == 0) {
		// nothing is going on with startStop button
		return;
	}


	// todo: should this be simply FSIO?
	if (engine->rpmCalculator.isRunning(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		// turn starter off once engine is running
		bool wasStarterEngaged = enginePins.starterControl.getAndSet(0);
		if (wasStarterEngaged) {
			scheduleMsg(&sharedLogger, "Engine runs we can disengage the starter");
		}
		engine->startStopStateLastPushTime = 0;
	}

	if (getTimeNowNt() - engine->startStopStateLastPushTime > NT_PER_SECOND * CONFIG(startCrankingDuration)) {
		bool wasStarterEngaged = enginePins.starterControl.getAndSet(0);
		if (wasStarterEngaged) {
			scheduleMsg(&sharedLogger, "Cranking timeout %dseconds", CONFIG(startCrankingDuration));
		}
		engine->startStopStateLastPushTime = 0;
	}
}


#endif /* EFI_PROD_CODE */
