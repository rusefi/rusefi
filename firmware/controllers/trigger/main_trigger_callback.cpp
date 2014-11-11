/**
 * @file    main_trigger_callback.cpp
 * @brief   Main logic is here!
 *
 * See http://rusefi.com/docs/html/
 *
 * @date Feb 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
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

#if !EFI_PROD_CODE && !EFI_SIMULATOR

#define chThdSelf() 0
#define getRemainingStack(x) (999999)

#endif

#if EFI_ENGINE_CONTROL || defined(__DOXYGEN__)

#include "main_trigger_callback.h"
#include "ec2.h"

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
#if EFI_PROD_CODE
#include "rfiutil.h"
#endif /* EFI_HISTOGRAMS */
#include "LocalVersionHolder.h"
#include "event_queue.h"
#include "engine.h"

EXTERN_ENGINE
;

static LocalVersionHolder localVersion;

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
static EventQueue triggerEventsQueue;

static cyclic_buffer ignitionErrorDetection;

static Logging logger;

static INLINE void handleFuelInjectionEvent(ActuatorEvent *event, int rpm DECLATE_ENGINE_PARAMETER) {
	/**
	 * todo: we do not really need to calculate fuel for each individual cylinder
	 */
	float fuelMs = getFuelMs(rpm PASS_ENGINE_PARAMETER) * engineConfiguration->globalFuelCorrection;
	if (cisnan(fuelMs)) {
		warning(OBD_PCM_Processor_Fault, "NaN injection pulse");
		return;
	}
	if (fuelMs < 0) {
		warning(OBD_PCM_Processor_Fault, "Negative injection pulse %f", fuelMs);
		return;
	}

	if (engine->isCylinderCleanupMode)
		return;

	float delay = getOneDegreeTimeMs(rpm) * event->position.angleOffset;

//	if (isCranking())
//		scheduleMsg(&logger, "crankingFuel=%f for CLT=%fC", fuelMs, getCoolantTemperature());

	scheduleOutput(event->actuator, delay, fuelMs);
}

static INLINE void handleFuel(uint32_t eventIndex, int rpm DECLATE_ENGINE_PARAMETER) {
	if (!isInjectionEnabled(engine->engineConfiguration))
		return;
	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#3");
	efiAssertVoid(eventIndex < engine->engineConfiguration2->triggerShape.getLength(), "event index");

	/**
	 * Ignition events are defined by addFuelEvents() according to selected
	 * fueling strategy
	 */
	FuelSchedule *fs =
	isCrankingR(rpm) ?
			&engine->engineConfiguration2->crankingInjectionEvents : &engine->engineConfiguration2->injectionEvents;

	ActuatorEventList *source = &fs->events;

	/**
	 * This is a performance optimization for https://sourceforge.net/p/rusefi/tickets/64/
	 * TODO: better data structure? better algorithm?
	 */
	if (!fs->hasEvents[eventIndex])
		return;

	for (int i = 0; i < source->size; i++) {
		ActuatorEvent *event = &source->events[i];
		if (event->position.eventIndex != eventIndex)
			continue;
		handleFuelInjectionEvent(event, rpm PASS_ENGINE_PARAMETER);
	}
}

static INLINE void handleSparkEvent(uint32_t eventIndex, IgnitionEvent *iEvent, int rpm DECLATE_ENGINE_PARAMETER) {
	engine_configuration2_s *engineConfiguration2 = engine->engineConfiguration2;

	float dwellMs = getSparkDwellMsT(rpm PASS_ENGINE_PARAMETER);
	if (cisnan(dwellMs) || dwellMs < 0) {
		firmwareError("invalid dwell: %f at %d", dwellMs, rpm);
		return;
	}

	float sparkDelayUs = engine->rpmCalculator.oneDegreeUs * iEvent->dwellPosition.angleOffset;
	int isIgnitionError = sparkDelayUs < 0;
	ignitionErrorDetection.add(isIgnitionError);
	if (isIgnitionError) {
#if EFI_PROD_CODE
		scheduleMsg(&logger, "Negative spark delay=%f", sparkDelayUs);
#endif
		sparkDelayUs = 0;
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
	scheduleTask("spark up", sUp, sparkDelayUs, (schfunc_t) &turnPinHigh, (void *) iEvent->io_pin);
	/**
	 * Spark event is often happening during a later trigger event timeframe
	 * TODO: improve precision
	 */

	findTriggerPosition(&engineConfiguration2->triggerShape, &iEvent->sparkPosition,
			iEvent->advance PASS_ENGINE_PARAMETER);

	if (iEvent->sparkPosition.eventIndex == eventIndex) {
		/**
		 * Spark should be fired before the next trigger event - time-based delay is best precision possible
		 */
		float timeTillIgnitionUs = engine->rpmCalculator.oneDegreeUs * iEvent->sparkPosition.angleOffset;

		scheduleTask("spark 1down", sDown, (int) timeTillIgnitionUs, (schfunc_t) &turnPinLow, (void*) iEvent->io_pin);
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

static INLINE void handleSpark(uint32_t eventIndex, int rpm, IgnitionEventList *list DECLATE_ENGINE_PARAMETER) {
	if (!isValidRpm(rpm) || !engine->engineConfiguration->isIgnitionEnabled)
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

			float timeTillIgnitionUs = engine->rpmCalculator.oneDegreeUs * current->sparkPosition.angleOffset;
			scheduleTask("spark 2down", sDown, (int) timeTillIgnitionUs, (schfunc_t) &turnPinLow,
					(void*) current->io_pin);
		}
	}

//	scheduleSimpleMsg(&logger, "eventId spark ", eventIndex);
	for (int i = 0; i < list->size; i++) {
		IgnitionEvent *event = &list->events[i];
		if (event->dwellPosition.eventIndex != eventIndex)
			continue;
		handleSparkEvent(eventIndex, event, rpm PASS_ENGINE_PARAMETER);
	}
}

static histogram_s mainLoopHisto;

void showMainHistogram(void) {
#if EFI_PROD_CODE
	printHistogram(&logger, &mainLoopHisto);
#endif
}

/**
 * This is the main trigger event handler.
 * Both injection and ignition are controlled from this method.
 */
void mainTriggerCallback(trigger_event_e ckpSignalType, uint32_t eventIndex, Engine *engine) {
	if (hasFirmwareError()) {
		/**
		 * In case on a major error we should not process any more events.
		 * TODO: add 'pin shutdown' invocation somewhere
		 */
		return;
	}

	(void) ckpSignalType;
	efiAssertVoid(eventIndex < 2 * engine->engineConfiguration2->triggerShape.shaftPositionEventCount, "event index");
	efiAssertVoid(getRemainingStack(chThdSelf()) > 256, "lowstck#2");

	// todo: remove these local variables soon?
	engine_configuration_s *engineConfiguration = engine->engineConfiguration;

	int rpm = getRpmE(engine);
	if (rpm == 0) {
		// this happens while we just start cranking
		// todo: check for 'trigger->is_synchnonized?'
		return;
	}
	if (rpm == NOISY_RPM) {
		warning(OBD_Camshaft_Position_Sensor_Circuit_Range_Performance, "noisy trigger");
		return;
	}
	if (rpm > engine->engineConfiguration->rpmHardLimit) {
		warning(OBD_PCM_Processor_Fault, "skipping stroke due to rpm=%d", rpm);
		return;
	}

#if EFI_HISTOGRAMS && EFI_PROD_CODE
	int beforeCallback = hal_lld_get_counter_value();
#endif

	int revolutionIndex = engine->rpmCalculator.getRevolutionCounter() % 2;

	if (eventIndex == 0) {
		if (localVersion.isOld())
			prepareOutputSignals(engine);

		/**
		 * TODO: warning. there is a bit of a hack here, todo: improve.
		 * currently output signals/times signalTimerUp from the previous revolutions could be
		 * still used because they have crossed the revolution boundary
		 * but we are already repurposing the output signals, but everything works because we
		 * are not affecting that space in memory. todo: use two instances of 'ignitionSignals'
		 */

		/**
		 * Within one engine cycle all cylinders are fired with same timing advance.
		 * todo: one day we can control cylinders individually
		 */
		float dwellMs = getSparkDwellMsT(rpm PASS_ENGINE_PARAMETER);
		if (cisnan(dwellMs) || dwellMs < 0) {
			firmwareError("invalid dwell: %f at %d", dwellMs, rpm);
			return;
		}
		float advance = getAdvance(rpm, getEngineLoadT(engine) PASS_ENGINE_PARAMETER);

		if (cisnan(advance)) {
			// error should already be reported
			return;
		}

		float dwellAngle = dwellMs / getOneDegreeTimeMs(rpm);

		initializeIgnitionActions(fixAngle(engineConfiguration, -advance), dwellAngle, engine->engineConfiguration2,
				&engine->engineConfiguration2->ignitionEvents[revolutionIndex] PASS_ENGINE_PARAMETER);
	}

	triggerEventsQueue.executeAll(getCrankEventCounter());

	handleFuel(eventIndex, rpm PASS_ENGINE_PARAMETER);
	handleSpark(eventIndex, rpm, &engine->engineConfiguration2->ignitionEvents[revolutionIndex] PASS_ENGINE_PARAMETER);
#if EFI_HISTOGRAMS && EFI_PROD_CODE
	int diff = hal_lld_get_counter_value() - beforeCallback;
	if (diff > 0)
	hsAdd(&mainLoopHisto, diff);
#endif /* EFI_HISTOGRAMS */
}

#include "wave_chart.h"

static void showTriggerHistogram(void) {
	printAllCallbacksHistogram();
	showMainHistogram();
#if EFI_PROD_CODE
	showWaveChartHistogram();
#endif
}

void MainTriggerCallback::init(Engine *engine, engine_configuration2_s *engineConfiguration2) {
	efiAssertVoid(engine!=NULL, "engine NULL");
	this->engine = engine;
}

static void showMainInfo(Engine *engine) {
	int rpm = engine->rpmCalculator.rpm();
	float el = getEngineLoadT(mainTriggerCallbackInstance.engine);
#if EFI_PROD_CODE
	scheduleMsg(&logger, "rpm %d engine_load %f", rpm, el);
	scheduleMsg(&logger, "fuel %fms timing %f", getFuelMs(rpm PASS_ENGINE_PARAMETER),
			getAdvance(rpm, el PASS_ENGINE_PARAMETER));
#endif
}

void initMainEventListener(Engine *engine, engine_configuration2_s *engineConfiguration2) {
	efiAssertVoid(engine!=NULL, "null engine");

	mainTriggerCallbackInstance.init(engine, engineConfiguration2);

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	addConsoleAction("performanceinfo", showTriggerHistogram);
	addConsoleActionP("maininfo", (VoidPtr) showMainInfo, engine);

	initLogging(&logger, "main event handler");
	printMsg(&logger, "initMainLoop: %d", currentTimeMillis());
	if (!isInjectionEnabled(mainTriggerCallbackInstance.engine->engineConfiguration))
		printMsg(&logger, "!!!!!!!!!!!!!!!!!!! injection disabled");
#endif

#if EFI_HISTOGRAMS
	initHistogram(&mainLoopHisto, "main callback");
#endif /* EFI_HISTOGRAMS */

	addTriggerEventListener(mainTriggerCallback, "main loop", engine);
}

int isIgnitionTimingError(void) {
	return ignitionErrorDetection.sum(6) > 4;
}

#endif /* EFI_ENGINE_CONTROL */
