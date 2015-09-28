/**
 * @file    main_trigger_callback.cpp
 * @brief   Main logic is here!
 *
 * See http://rusefi.com/docs/html/
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "main.h"
#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include <nvic.h>
#endif

#if (!EFI_PROD_CODE && !EFI_SIMULATOR) || defined(__DOXYGEN__)

#define chThdSelf() 0
#define getRemainingStack(x) (999999)

#endif

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)

#include "main_trigger_callback.h"
#include "efiGpio.h"
#include "engine_math.h"
#include "trigger_central.h"
#include "rpm_calculator.h"
#include "signal_executor.h"
#include "engine_configuration.h"
#include "interpolation.h"
#include "advance_map.h"
#include "allsensors.h"
#include "cyclic_buffer.h"
#include "histogram.h"
#include "fuel_math.h"
#include "histogram.h"
#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "rfiutil.h"
#endif /* EFI_HISTOGRAMS */
#include "LocalVersionHolder.h"
#include "event_queue.h"
#include "engine.h"
#include "efilib2.h"

EXTERN_ENGINE
;
extern bool hasFirmwareErrorFlag;

static LocalVersionHolder triggerVersion;

extern engine_pins_s enginePins;
static MainTriggerCallback mainTriggerCallbackInstance;

/**
 * That's the list of pending spark firing events
 */
static IgnitionEvent *iHead = NULL;

/**
 * In order to archive higher event precision, we are using a hybrid approach
 * where we are scheduling events based on the closest trigger event with a time offset.
 *
 * This queue is using global trigger event index as 'time'
 */
//static EventQueue triggerEventsQueue;
static cyclic_buffer<int> ignitionErrorDetection;

static Logging *logger;

// todo: figure out if this even helps?
//#if defined __GNUC__
//#define RAM_METHOD_PREFIX __attribute__((section(".ram")))
//#else
//#define RAM_METHOD_PREFIX
//#endif

static void startSimultaniousInjection(Engine *engine) {
	for (int i = 0; i < engine->engineConfiguration->specs.cylindersCount; i++) {
		turnPinHigh(&enginePins.injectors[i]);
	}
}

static void endSimultaniousInjection(Engine *engine) {
	for (int i = 0; i < engine->engineConfiguration->specs.cylindersCount; i++) {
		turnPinLow(&enginePins.injectors[i]);
	}
}

extern WallFuel wallFuel;

static ALWAYS_INLINE void handleFuelInjectionEvent(bool_t limitedFuel, InjectionEvent *event, int rpm DECLARE_ENGINE_PARAMETER_S) {
	/**
	 * todo: this is a bit tricky with batched injection. is it? Does the same
	 * wetting coefficient works the same way for any injection mode, or is something
	 * x2 or /2?
	 */
	floatms_t injectionDuration = wallFuel.adjust(event->injectorIndex, ENGINE(fuelMs) PASS_ENGINE_PARAMETER);

	ENGINE(actualLastInjection) = injectionDuration;
	if (cisnan(injectionDuration)) {
		warning(OBD_PCM_Processor_Fault, "NaN injection pulse");
		return;
	}
	if (injectionDuration < 0) {
		warning(OBD_PCM_Processor_Fault, "Negative injection pulse %f", injectionDuration);
		return;
	}

	if (engine->isCylinderCleanupMode)
		return;

	floatus_t injectionStartDelayUs = ENGINE(rpmCalculator.oneDegreeUs) * event->injectionStart.angleOffset;

	if (event->isSimultanious) {
		if (injectionDuration < 0) {
			firmwareError("duration cannot be negative: %d", injectionDuration);
			return;
		}
		if (cisnan(injectionDuration)) {
			firmwareError("NaN in scheduleOutput", injectionDuration);
			return;
		}
		/**
		 * this is pretty much copy-paste of 'scheduleOutput'
		 * 'scheduleOutput' is currently only used for injection, so maybe it should be
		 * changed into 'scheduleInjection' and unified? todo: think about it.
		 */
		OutputSignal *signal = &event->actuator;
		efiAssertVoid(signal!=NULL, "signal is NULL");
		int index = getRevolutionCounter() % 2;
		scheduling_s * sUp = &signal->signalTimerUp[index];
		scheduling_s * sDown = &signal->signalTimerDown[index];

		if (!limitedFuel) {
			scheduleTask("out up", sUp, (int) injectionStartDelayUs, (schfunc_t) &startSimultaniousInjection, engine);
			scheduleTask("out down", sDown, (int) injectionStartDelayUs + MS2US(injectionDuration), (schfunc_t) &endSimultaniousInjection, engine);
		}

	} else {
		if (!limitedFuel) {
			scheduleOutput(&event->actuator, getTimeNowUs(), injectionStartDelayUs, MS2US(injectionDuration));
		}
	}
}

static ALWAYS_INLINE void handleFuel(bool_t limitedFuel, uint32_t eventIndex, int rpm DECLARE_ENGINE_PARAMETER_S) {
	if (!isInjectionEnabled(engine->engineConfiguration))
		return;
	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#3");
	efiAssertVoid(eventIndex < engine->triggerShape.getLength(), "handleFuel/event index");

	/**
	 * Ignition events are defined by addFuelEvents() according to selected
	 * fueling strategy
	 */
	FuelSchedule *fs =
	isCrankingR(rpm) ?
			&ENGINE(engineConfiguration2)->crankingInjectionEvents : &engine->engineConfiguration2->injectionEvents;

	InjectionEventList *source = &fs->injectionEvents;

	if (!fs->hasEvents[eventIndex])
		return;

	engine->tpsAccelEnrichment.onEngineCycleTps(PASS_ENGINE_PARAMETER_F);

	engine->mapAccelEnrichment.onEngineCycle(PASS_ENGINE_PARAMETER_F);
	ENGINE(fuelMs) = getFuelMs(rpm PASS_ENGINE_PARAMETER) * engineConfiguration->globalFuelCorrection;

	for (int i = 0; i < source->size; i++) {
		InjectionEvent *event = &source->elements[i];
		if (event->injectionStart.eventIndex != eventIndex)
			continue;
		handleFuelInjectionEvent(limitedFuel, event, rpm PASS_ENGINE_PARAMETER);
	}
}

static ALWAYS_INLINE void handleSparkEvent(bool_t limitedSpark, uint32_t eventIndex, IgnitionEvent *iEvent,
		int rpm DECLARE_ENGINE_PARAMETER_S) {

	float dwellMs = engine->engineState.sparkDwell;
	if (cisnan(dwellMs) || dwellMs < 0) {
		firmwareError("invalid dwell: %f at %d", dwellMs, rpm);
		return;
	}

	floatus_t chargeDelayUs = engine->rpmCalculator.oneDegreeUs * iEvent->dwellPosition.angleOffset;
	int isIgnitionError = chargeDelayUs < 0;
	ignitionErrorDetection.add(isIgnitionError);
	if (isIgnitionError) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
		scheduleMsg(logger, "Negative spark delay=%f", chargeDelayUs);
#endif
		chargeDelayUs = 0;
		return;
	}

	if (cisnan(dwellMs)) {
		firmwareError("NaN in scheduleOutput", dwellMs);
		return;
	}

	/**
	 * We are alternating two event lists in order to avoid a potential issue around revolution boundary
	 * when an event is scheduled within the next revolution.
	 */
	scheduling_s * sUp = &iEvent->signalTimerUp;
	scheduling_s * sDown = &iEvent->signalTimerDown;

	/**
	 * The start of charge is always within the current trigger event range, so just plain time-based scheduling
	 */
	if (!limitedSpark) {
		/**
		 * Note how we do not check if spark is limited or not while scheduling 'spark down'
		 * This way we make sure that coil dwell started while spark was enabled would fire and not burn
		 * the coil.
		 */
		scheduleTask("spark up", sUp, chargeDelayUs, (schfunc_t) &turnPinHigh, iEvent->output);
	}
	/**
	 * Spark event is often happening during a later trigger event timeframe
	 * TODO: improve precision
	 */

	findTriggerPosition(&iEvent->sparkPosition, iEvent->advance PASS_ENGINE_PARAMETER);

	if (iEvent->sparkPosition.eventIndex == eventIndex) {
		/**
		 * Spark should be fired before the next trigger event - time-based delay is best precision possible
		 */
		float timeTillIgnitionUs = engine->rpmCalculator.oneDegreeUs * iEvent->sparkPosition.angleOffset;

		scheduleTask("spark1 down", sDown, (int) timeTillIgnitionUs, (schfunc_t) &turnPinLow, iEvent->output);
	} else {
		/**
		 * Spark should be scheduled in relation to some future trigger event, this way we get better firing precision
		 */
		bool isPending = assertNotInList<IgnitionEvent>(iHead, iEvent);
		if (isPending)
			return;

		LL_APPEND(iHead, iEvent);
	}
}

static ALWAYS_INLINE void handleSpark(bool_t limitedSpark, uint32_t eventIndex, int rpm,
		IgnitionEventList *list DECLARE_ENGINE_PARAMETER_S) {
	if (!isValidRpm(rpm) || !engineConfiguration->isIgnitionEnabled)
		return; // this might happen for instance in case of a single trigger event after a pause

	/**
	 * Ignition schedule is defined once per revolution
	 * See initializeIgnitionActions()
	 */

	IgnitionEvent *current, *tmp;

	LL_FOREACH_SAFE(iHead, current, tmp)
	{
		if (current->sparkPosition.eventIndex == eventIndex) {
			// time to fire a spark which was scheduled previously
			LL_DELETE(iHead, current);

			scheduling_s * sDown = &current->signalTimerDown;

			float timeTillIgnitionUs = ENGINE(rpmCalculator.oneDegreeUs) * current->sparkPosition.angleOffset;
			scheduleTask("spark 2down", sDown, (int) timeTillIgnitionUs, (schfunc_t) &turnPinLow, current->output);
		}
	}

//	scheduleSimpleMsg(&logger, "eventId spark ", eventIndex);
	for (int i = 0; i < list->size; i++) {
		IgnitionEvent *event = &list->elements[i];
		if (event->dwellPosition.eventIndex != eventIndex)
			continue;
		handleSparkEvent(limitedSpark, eventIndex, event, rpm PASS_ENGINE_PARAMETER);
	}
}

static histogram_s mainLoopHisto;

void showMainHistogram(void) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	printHistogram(logger, &mainLoopHisto);
#endif
}

// todo: the method name is not correct any more - no calc is done here anymore
static ALWAYS_INLINE void ignitionMathCalc(int rpm DECLARE_ENGINE_PARAMETER_S) {
	/**
	 * Within one engine cycle all cylinders are fired with same timing advance.
	 * todo: one day we can control cylinders individually?
	 */
	float dwellMs = ENGINE(engineState.sparkDwell);

	if (cisnan(dwellMs) || dwellMs < 0) {
		firmwareError("invalid dwell: %f at %d", dwellMs, rpm);
		return;
	}
	// todo: eliminate this field
	engine->engineState.advance = -ENGINE(engineState.timingAdvance);
}

#if EFI_PROD_CODE || defined(__DOXYGEN__)
/**
 * this field is used as an Expression in IAR debugger
 */
uint32_t *cyccnt = (uint32_t*)&DWT_CYCCNT;
#endif

static ALWAYS_INLINE void scheduleIgnitionAndFuelEvents(int rpm, int revolutionIndex DECLARE_ENGINE_PARAMETER_S) {
	engine->m.beforeFuelCalc = GET_TIMESTAMP();
	ENGINE(fuelMs) = getFuelMs(rpm PASS_ENGINE_PARAMETER) * engineConfiguration->globalFuelCorrection;
	engine->m.fuelCalcTime = GET_TIMESTAMP() - engine->m.beforeFuelCalc;

	engine->m.beforeIgnitionSch = GET_TIMESTAMP();
	/**
	 * TODO: warning. there is a bit of a hack here, todo: improve.
	 * currently output signals/times signalTimerUp from the previous revolutions could be
	 * still used because they have crossed the revolution boundary
	 * but we are already re-purposing the output signals, but everything works because we
	 * are not affecting that space in memory. todo: use two instances of 'ignitionSignals'
	 */
	float maxAllowedDwellAngle = (int) (engineConfiguration->engineCycle / 2); // the cast is about making Coverity happy

	if (engineConfiguration->ignitionMode == IM_ONE_COIL) {
		maxAllowedDwellAngle = engineConfiguration->engineCycle / engineConfiguration->specs.cylindersCount / 1.1;
	}

	if (engine->engineState.dwellAngle == 0) {
		warning(OBD_PCM_Processor_Fault, "dwell is zero?");
	}
	if (engine->engineState.dwellAngle > maxAllowedDwellAngle) {
		warning(OBD_PCM_Processor_Fault, "dwell angle too long: %f", engine->engineState.dwellAngle);
	}

	// todo: add some check for dwell overflow? like 4 times 6 ms while engine cycle is less then that

	IgnitionEventList *list = &engine->engineConfiguration2->ignitionEvents[revolutionIndex];

	if (cisnan(engine->engineState.advance)) {
		// error should already be reported
		list->reset(); // reset is needed to clear previous ignition schedule
		return;
	}
	initializeIgnitionActions(engine->engineState.advance, engine->engineState.dwellAngle,
			list PASS_ENGINE_PARAMETER);
	engine->m.ignitionSchTime = GET_TIMESTAMP() - engine->m.beforeIgnitionSch;

	engine->m.beforeInjectonSch = GET_TIMESTAMP();

	if (isCrankingR(rpm)) {
		ENGINE(engineConfiguration2)->crankingInjectionEvents.addFuelEvents(
				engineConfiguration->crankingInjectionMode PASS_ENGINE_PARAMETER);
	} else {
		ENGINE(engineConfiguration2)->injectionEvents.addFuelEvents(
				engineConfiguration->injectionMode PASS_ENGINE_PARAMETER);
	}
	engine->m.injectonSchTime = GET_TIMESTAMP() - engine->m.beforeInjectonSch;

}

/**
 * This is the main trigger event handler.
 * Both injection and ignition are controlled from this method.
 */
void mainTriggerCallback(trigger_event_e ckpSignalType, uint32_t eventIndex DECLARE_ENGINE_PARAMETER_S) {
	engine->m.beforeMainTrigger = GET_TIMESTAMP();
	if (hasFirmwareError()) {
		/**
		 * In case on a major error we should not process any more events.
		 * TODO: add 'pin shutdown' invocation somewhere - coils might be still open here!
		 */
		return;
	}

	(void) ckpSignalType;
	efiAssertVoid(eventIndex < 2 * engine->triggerShape.getSize(), "trigger/event index");
	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#2");

	int rpm = getRpmE(engine);
	if (rpm == 0) {
		// this happens while we just start cranking
		// todo: check for 'trigger->is_synchnonized?'
		// TODO: add 'pin shutdown' invocation somewhere - coils might be still open here!
		return;
	}
	if (rpm == NOISY_RPM) {
		warning(OBD_Camshaft_Position_Sensor_Circuit_Range_Performance, "noisy trigger");
		// TODO: add 'pin shutdown' invocation somewhere - coils might be still open here!
		return;
	}
	bool_t limitedSpark = rpm > engineConfiguration->rpmHardLimit;
	bool_t limitedFuel = rpm > engineConfiguration->rpmHardLimit;
	if (limitedSpark || limitedFuel) {
		warning(OBD_PCM_Processor_Fault, "skipping stroke due to rpm=%d", rpm);
	}

#if (EFI_HISTOGRAMS && EFI_PROD_CODE) || defined(__DOXYGEN__)
	int beforeCallback = hal_lld_get_counter_value();
#endif

	int revolutionIndex = engine->rpmCalculator.getRevolutionCounter() % 2;

	if (eventIndex == 0) {
		if (triggerVersion.isOld())
			prepareOutputSignals(PASS_ENGINE_PARAMETER_F);
	}

	if (engineConfiguration->useOnlyFrontForTrigger && engineConfiguration->ignMathCalculateAtIndex % 2 != 0) {
		firmwareError("invalid ignMathCalculateAtIndex %d", engineConfiguration->ignMathCalculateAtIndex);
	}

	if (eventIndex == engineConfiguration->ignMathCalculateAtIndex) {
		if (engineConfiguration->externalKnockSenseAdc != EFI_ADC_NONE) {
			float externalKnockValue = getVoltageDivided("knock", engineConfiguration->externalKnockSenseAdc);
			engine->knockLogic(externalKnockValue);
		}

		engine->m.beforeIgnitionMath = GET_TIMESTAMP();
		ignitionMathCalc(rpm PASS_ENGINE_PARAMETER);
		engine->m.ignitionMathTime = GET_TIMESTAMP() - engine->m.beforeIgnitionMath;
	}

	if (eventIndex == 0) {
		scheduleIgnitionAndFuelEvents(rpm, revolutionIndex PASS_ENGINE_PARAMETER);
	}

//	triggerEventsQueue.executeAll(getCrankEventCounter());

	/**
	 * For fuel we schedule start of injection based on trigger angle, and then inject for
	 * specified duration of time
	 */
	handleFuel(limitedFuel, eventIndex, rpm PASS_ENGINE_PARAMETER);
	/**
	 * For spark we schedule both start of coil charge and actual spark based on trigger angle
	 */
	handleSpark(limitedSpark, eventIndex, rpm, &engine->engineConfiguration2->ignitionEvents[revolutionIndex] PASS_ENGINE_PARAMETER);
#if (EFI_HISTOGRAMS && EFI_PROD_CODE) || defined(__DOXYGEN__)
	int diff = hal_lld_get_counter_value() - beforeCallback;
	if (diff > 0)
	hsAdd(&mainLoopHisto, diff);
#endif /* EFI_HISTOGRAMS */

	if (eventIndex == 0) {
		engine->m.mainTriggerCallbackTime = GET_TIMESTAMP() - engine->m.beforeMainTrigger;
	}
}

#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)
#include "engine_sniffer.h"
#endif

static void showTriggerHistogram(void) {
	printAllCallbacksHistogram();
	showMainHistogram();
#if EFI_ENGINE_SNIFFER || defined(__DOXYGEN__)
	showWaveChartHistogram();
#endif
}

void MainTriggerCallback::init(Engine *engine) {
	efiAssertVoid(engine!=NULL, "engine NULL");
	this->engine = engine;
}

static void showMainInfo(Engine *engine) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	int rpm = engine->rpmCalculator.rpm(PASS_ENGINE_PARAMETER_F);
	float el = getEngineLoadT(PASS_ENGINE_PARAMETER_F);
	scheduleMsg(logger, "rpm %d engine_load %f", rpm, el);
	scheduleMsg(logger, "fuel %fms timing %f", getFuelMs(rpm PASS_ENGINE_PARAMETER),
			engine->engineState.timingAdvance);
#endif
}

void initMainEventListener(Logging *sharedLogger, Engine *engine) {
	logger = sharedLogger;
	efiAssertVoid(engine!=NULL, "null engine");

	mainTriggerCallbackInstance.init(engine);

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	addConsoleAction("performanceinfo", showTriggerHistogram);
	addConsoleActionP("maininfo", (VoidPtr) showMainInfo, engine);

	printMsg(logger, "initMainLoop: %d", currentTimeMillis());
	if (!isInjectionEnabled(mainTriggerCallbackInstance.engine->engineConfiguration))
		printMsg(logger, "!!!!!!!!!!!!!!!!!!! injection disabled");
#endif

#if EFI_HISTOGRAMS || defined(__DOXYGEN__)
	initHistogram(&mainLoopHisto, "main callback");
#endif /* EFI_HISTOGRAMS */

	addTriggerEventListener(mainTriggerCallback, "main loop", engine);
}

int isIgnitionTimingError(void) {
	return ignitionErrorDetection.sum(6) > 4;
}

#endif /* EFI_ENGINE_CONTROL */
