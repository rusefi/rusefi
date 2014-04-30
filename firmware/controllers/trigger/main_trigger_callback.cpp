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
#include "main_trigger_callback.h"
#include "ec2.h"

extern "C" {
//#include "settings.h"
#include "trigger_central.h"
#include "rpm_calculator.h"
#include "signal_executor.h"
#include "eficonsole.h"
#include "engine_math.h"
//#include "injector_central.h"
//#include "ignition_central.h"
#include "engine_configuration.h"
#include "interpolation.h"
#include "advance_map.h"
#include "allsensors.h"
#include "cyclic_buffer.h"
#include "histogram.h"
#include "fuel_math.h"
#include "histogram.h"
#include "rfiutil.h"
#include "LocalVersionHolder.h"

static LocalVersionHolder localVersion;

int isInjectionEnabled(void);

}

// todo: move this to engine_configuration2_s for now

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;

static cyclic_buffer ignitionErrorDetection;

static Logging logger;

/**
 * this field is accessed only from shaft sensor event handler.
 * This is not a method variable just to save us from stack overflow.
 */
static ActuatorEventList events;

static void handleFuelInjectionEvent(ActuatorEvent *event, int rpm) {
	float fuelMs = getFuelMs(rpm) * engineConfiguration->globalFuelCorrection;
	if (fuelMs < 0) {
		scheduleMsg(&logger, "ERROR: negative injectionPeriod %f", fuelMs);
		return;
	}

	float delay = getOneDegreeTimeMs(rpm) * event->angleOffset;

//	if (isCranking())
//		scheduleMsg(&logger, "crankingFuel=%f for CLT=%fC", fuelMs, getCoolantTemperature());

	scheduleOutput(event->actuator, delay, fuelMs);
}

static void handleFuel(int eventIndex) {
	if (!isInjectionEnabled())
		return;
	chDbgCheck(eventIndex < engineConfiguration2->triggerShape.shaftPositionEventCount, "event index");

	/**
	 * Ignition events are defined by addFuelEvents() according to selected
	 * fueling strategy
	 */
	ActuatorEventList *source =
			isCranking() ?
					&engineConfiguration2->engineEventConfiguration.crankingInjectionEvents :
					&engineConfiguration2->engineEventConfiguration.injectionEvents;
	findEvents(eventIndex, source, &events);

	if (events.size == 0)
		return;

//	scheduleSimpleMsg(&logger, "eventId size=", events.size);

	int rpm = getRpm();

	for (int i = 0; i < events.size; i++) {
		ActuatorEvent *event = &events.events[i];
		handleFuelInjectionEvent(event, rpm);
	}
}

static void handleSparkEvent(ActuatorEvent *event, int rpm) {
	efiAssert(rpm != 0, "non-zero RPM expected here");

//	float advance = getAdvance(rpm, getEngineLoad());

//	float sparkAdvanceMs = getOneDegreeTimeMs(rpm) * advance;

	float dwellMs = getSparkDwellMs(rpm);
	if (dwellMs < 0)
		firmwareError("invalid dwell: %f at %d", dwellMs, rpm);

	float sparkDelay = getOneDegreeTimeMs(rpm) * event->angleOffset;
	int isIgnitionError = sparkDelay < 0;
	ignitionErrorDetection.add(isIgnitionError);
	if (isIgnitionError) {
		scheduleMsg(&logger, "Negative spark delay=%f", sparkDelay);
		sparkDelay = 0;
		//return;
	}

	scheduleOutput(event->actuator, sparkDelay, dwellMs);
}

static void handleSpark(int eventIndex) {
	int rpm = getRpm();

	/**
	 * Ignition schedule is defined once per revolution
	 * See initializeIgnitionActions()
	 */
	findEvents(eventIndex, &engineConfiguration2->engineEventConfiguration.ignitionEvents, &events);
	if (events.size == 0)
		return;

//	scheduleSimpleMsg(&logger, "eventId spark ", eventIndex);

	for (int i = 0; i < events.size; i++) {
		ActuatorEvent *event = &events.events[i];
		handleSparkEvent(event, rpm);
	}
}

static histogram_s mainLoopHisto;

void showMainHistogram(void) {
	printHistogram(&logger, &mainLoopHisto);
}

/**
 * This is the main entry point into the primary shaft signal handler signal. Both injection and ignition are controlled from this method.
 */
static void onShaftSignal(ShaftEvents ckpSignalType, int eventIndex) {
	chDbgCheck(eventIndex < engineConfiguration2->triggerShape.shaftPositionEventCount, "event index");

	int rpm = getRpm();
	if (rpm == 0) {
		// this happens while we just start cranking
		// todo: check for 'trigger->is_synchnonized?'
		return;
	}
	if (rpm == NOISY_RPM) {
		scheduleMsg(&logger, "noisy trigger");
		return;
	}
	if (rpm > engineConfiguration->rpmHardLimit) {
		warning(OBD_PCM_Processor_Fault, "skipping stroke due to rpm=%d", rpm);
		return;
	}

	int beforeCallback = hal_lld_get_counter_value();
	if (eventIndex == 0) {
		if (localVersion.isOld())
			prepareOutputSignals(engineConfiguration, engineConfiguration2);

		/**
		 * TODO: warning. there is a bit of a hack here, todo: improve.
		 * currently output signals/times signalTimerUp from the previous revolutions could be
		 * still used because they have crossed the revolution boundary
		 * but we are already repurposing the output signals, but everything works because we
		 * are not affecting that space in memory. todo: use two instances of 'ignitionSignals'
		 */

		float dwellMs = getSparkDwellMs(rpm);
		float advance = getAdvance(rpm, getEngineLoad());

		float dwellAngle = dwellMs / getOneDegreeTimeMs(rpm);

		initializeIgnitionActions(advance - dwellAngle, engineConfiguration, engineConfiguration2);
	}

	handleFuel(eventIndex);
	handleSpark(eventIndex);
	int diff = hal_lld_get_counter_value() - beforeCallback;
	if (diff > 0)
		hsAdd(&mainLoopHisto, diff);
}

static void showTriggerHistogram(void) {
	printAllCallbacksHistogram();
	showMainHistogram();
}

static void showMainInfo(void) {
	int rpm = getRpm();
	float el = getEngineLoad();
	scheduleMsg(&logger, "rpm %d engine_load %f", rpm, el);
	scheduleMsg(&logger, "fuel %fms timing %f", getFuelMs(rpm), getAdvance(rpm, el));
}

void initMainEventListener() {
	addConsoleAction("performanceinfo", showTriggerHistogram);
	addConsoleAction("maininfo", showMainInfo);

	initLogging(&logger, "main event handler");
	printMsg(&logger, "initMainLoop: %d", currentTimeMillis());
	initHistogram(&mainLoopHisto, "main callback");

	if (!isInjectionEnabled())
		printMsg(&logger, "!!!!!!!!!!!!!!!!!!! injection disabled");

	addTriggerEventListener(&onShaftSignal, "main loop");
}

int isIgnitionTimingError(void) {
	return ignitionErrorDetection.sum(6) > 4;
}
