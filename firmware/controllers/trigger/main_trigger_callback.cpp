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

static void handleFuelInjectionEvent(MainTriggerCallback *mainTriggerCallback, ActuatorEvent *event, int rpm) {
	float fuelMs = getFuelMs(rpm, mainTriggerCallback->engine)
			* mainTriggerCallback->engineConfiguration->globalFuelCorrection;
	if (cisnan(fuelMs)) {
		warning(OBD_PCM_Processor_Fault, "NaN injection pulse");
		return;
	}
	if (fuelMs < 0) {
		warning(OBD_PCM_Processor_Fault, "Negative injection pulse %f", fuelMs);
		return;
	}

	if (mainTriggerCallback->engine->isCylinderCleanupMode)
		return;

	float delay = getOneDegreeTimeMs(rpm) * event->position.angleOffset;

//	if (isCranking())
//		scheduleMsg(&logger, "crankingFuel=%f for CLT=%fC", fuelMs, getCoolantTemperature());

	scheduleOutput(event->actuator, delay, fuelMs);
}

static void handleFuel(Engine *engine, MainTriggerCallback *mainTriggerCallback, uint32_t eventIndex, int rpm) {
	if (!isInjectionEnabled(mainTriggerCallback->engineConfiguration))
		return;
	efiAssertVoid(getRemainingStack(chThdSelf()) > 16, "stack#3");
	efiAssertVoid(eventIndex < mainTriggerCallback->engineConfiguration2->triggerShape.getLength(), "event index");

	engine_configuration_s *engineConfiguration = engine->engineConfiguration;
	/**
	 * Ignition events are defined by addFuelEvents() according to selected
	 * fueling strategy
	 */
	ActuatorEventList *source =
	isCrankingR(rpm) ?
			&mainTriggerCallback->engineConfiguration2->engineEventConfiguration.crankingInjectionEvents :
			&mainTriggerCallback->engineConfiguration2->engineEventConfiguration.injectionEvents;

	for (int i = 0; i < source->size; i++) {
		ActuatorEvent *event = &source->events[i];
		if (event->position.eventIndex != eventIndex)
			continue;
		handleFuelInjectionEvent(mainTriggerCallback, event, rpm);
	}
}

static void handleSparkEvent(MainTriggerCallback *mainTriggerCallback, uint32_t eventIndex, IgnitionEvent *iEvent,
		int rpm) {
	engine_configuration_s *engineConfiguration = mainTriggerCallback->engineConfiguration;
	engine_configuration2_s *engineConfiguration2 = mainTriggerCallback->engineConfiguration2;

	float dwellMs = getSparkDwellMsT(engineConfiguration, rpm);
	if (cisnan(dwellMs) || dwellMs < 0) {
		firmwareError("invalid dwell: %f at %d", dwellMs, rpm);
		return;
	}

	float sparkDelay = getOneDegreeTimeMs(rpm) * iEvent->dwellPosition.angleOffset;
	int isIgnitionError = sparkDelay < 0;
	ignitionErrorDetection.add(isIgnitionError);
	if (isIgnitionError) {
#if EFI_PROD_CODE
		scheduleMsg(&logger, "Negative spark delay=%f", sparkDelay);
#endif
		sparkDelay = 0;
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
	scheduleTask("spark up", sUp, (int) MS2US(sparkDelay), (schfunc_t) &turnPinHigh, (void *) iEvent->io_pin);
	/**
	 * Spark event is often happening during a later trigger event timeframe
	 * TODO: improve precision
	 */

	findTriggerPosition(engineConfiguration, &engineConfiguration2->triggerShape, &iEvent->sparkPosition,
			iEvent->advance);

	if (iEvent->sparkPosition.eventIndex == eventIndex) {
		/**
		 * Spark should be fired before the next trigger event - time-based delay is best precision possible
		 */
		float timeTillIgnitionUs = getOneDegreeTimeUs(rpm) * iEvent->sparkPosition.angleOffset;

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

static void handleSpark(MainTriggerCallback *mainTriggerCallback, uint32_t eventIndex, int rpm,
		IgnitionEventList *list) {
	if (!isValidRpm(rpm) || !mainTriggerCallback->engineConfiguration->isIgnitionEnabled)
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

			float timeTillIgnitionUs = getOneDegreeTimeUs(rpm) * current->sparkPosition.angleOffset;
			scheduleTask("spark 2down", sDown, (int) timeTillIgnitionUs, (schfunc_t) &turnPinLow,
					(void*) current->io_pin);
		}
	}

//	scheduleSimpleMsg(&logger, "eventId spark ", eventIndex);
	for (int i = 0; i < list->size; i++) {
		IgnitionEvent *event = &list->events[i];
		if (event->dwellPosition.eventIndex != eventIndex)
			continue;
		handleSparkEvent(mainTriggerCallback, eventIndex, event, rpm);
	}
}

static histogram_s mainLoopHisto;

void showMainHistogram(void) {
#if EFI_PROD_CODE
	printHistogram(&logger, &mainLoopHisto);
#endif
}

// todo: eliminate this 'extern'
extern Engine engine;

/**
 * This is the main trigger event handler.
 * Both injection and ignition are controlled from this method.
 */
void onTriggerEvent(trigger_event_e ckpSignalType, uint32_t eventIndex, MainTriggerCallback *mainTriggerCallback) {
	(void) ckpSignalType;
	efiAssertVoid(eventIndex < 2 * mainTriggerCallback->engineConfiguration2->triggerShape.shaftPositionEventCount,
			"event index");
	efiAssertVoid(getRemainingStack(chThdSelf()) > 16, "stack#3");

// todo	int rpm = getRpmE(mainTriggerCallback->engine);
	int rpm = getRpmE(&engine);
	if (rpm == 0) {
		// this happens while we just start cranking
		// todo: check for 'trigger->is_synchnonized?'
		return;
	}
	if (rpm == NOISY_RPM) {
		warning(OBD_Camshaft_Position_Sensor_Circuit_Range_Performance, "noisy trigger");
		return;
	}
	if (rpm > mainTriggerCallback->engineConfiguration->rpmHardLimit) {
		warning(OBD_PCM_Processor_Fault, "skipping stroke due to rpm=%d", rpm);
		return;
	}

#if EFI_HISTOGRAMS && EFI_PROD_CODE
	int beforeCallback = hal_lld_get_counter_value();
#endif

	int revolutionIndex = getRevolutionCounter() % 2;

	if (eventIndex == 0) {
		if (localVersion.isOld())
			prepareOutputSignals(mainTriggerCallback->engine);

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
		float dwellMs = getSparkDwellMsT(mainTriggerCallback->engineConfiguration, rpm);
		if (cisnan(dwellMs) || dwellMs < 0) {
			firmwareError("invalid dwell: %f at %d", dwellMs, rpm);
			return;
		}
		float advance = getAdvance(rpm, getEngineLoadT(mainTriggerCallback->engine));
		if (cisnan(advance)) {
			// error should already be reported
			return;
		}

		float dwellAngle = dwellMs / getOneDegreeTimeMs(rpm);

		initializeIgnitionActions(advance, dwellAngle, mainTriggerCallback->engineConfiguration,
				mainTriggerCallback->engineConfiguration2,
				&mainTriggerCallback->engineConfiguration2->engineEventConfiguration.ignitionEvents[revolutionIndex]);
	}

	triggerEventsQueue.executeAll(getCrankEventCounter());

//todo	handleFuel(mainTriggerCallback->engine, mainTriggerCallback, eventIndex, rpm);
	handleFuel(&engine, mainTriggerCallback, eventIndex, rpm);
	handleSpark(mainTriggerCallback, eventIndex, rpm,
			&mainTriggerCallback->engineConfiguration2->engineEventConfiguration.ignitionEvents[revolutionIndex]);
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
	this->engineConfiguration = engine->engineConfiguration;
	efiAssertVoid(engineConfiguration!=NULL, "engineConfiguration NULL");
	this->engineConfiguration2 = engineConfiguration2;
}

static void showMainInfo(void) {
	int rpm = getRpm();
	float el = getEngineLoadT(mainTriggerCallbackInstance.engine);
#if EFI_PROD_CODE
	scheduleMsg(&logger, "rpm %d engine_load %f", rpm, el);
	scheduleMsg(&logger, "fuel %fms timing %f", getFuelMs(rpm, mainTriggerCallbackInstance.engine),
			getAdvance(rpm, el));
#endif
}

void initMainEventListener(Engine *engine, engine_configuration2_s *engineConfiguration2) {
	efiAssertVoid(engine!=NULL, "null engine");

	mainTriggerCallbackInstance.init(engine, engineConfiguration2);

#if EFI_PROD_CODE || defined(__DOXYGEN__)
	addConsoleAction("performanceinfo", showTriggerHistogram);
	addConsoleAction("maininfo", showMainInfo);

	initLogging(&logger, "main event handler");
	printMsg(&logger, "initMainLoop: %d", currentTimeMillis());
	if (!isInjectionEnabled(mainTriggerCallbackInstance.engineConfiguration))
		printMsg(&logger, "!!!!!!!!!!!!!!!!!!! injection disabled");
#endif

#if EFI_HISTOGRAMS
	initHistogram(&mainLoopHisto, "main callback");
#endif /* EFI_HISTOGRAMS */

	addTriggerEventListener((ShaftPositionListener) &onTriggerEvent, "main loop", &mainTriggerCallbackInstance);
}

int isIgnitionTimingError(void) {
	return ignitionErrorDetection.sum(6) > 4;
}

#endif /* EFI_ENGINE_CONTROL */
