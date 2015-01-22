/**
 * @file	map_averaging.cpp
 *
 * @date Dec 11, 2013
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

#include "map.h"

#if EFI_MAP_AVERAGING || defined(__DOXYGEN__)

#include "map_averaging.h"
#include "trigger_central.h"
#include "adc_inputs.h"
#include "engine_state.h"
#include "engine_configuration.h"
#include "interpolation.h"
#include "signal_executor.h"
#include "engine.h"

#if EFI_ANALOG_CHART
#include "analog_chart.h"
#endif /* EFI_ANALOG_CHART */

#define FAST_MAP_CHART_SKIP_FACTOR 16

static Logging *logger;

/**
 * Running counter of measurements per revolution
 */
static volatile int perRevolutionCounter = 0;
/**
 * Number of measurements in previous shaft revolution
 */
static volatile int perRevolution = 0;
/**
 * Running MAP accumulator
 * v_ for Voltage
 */
static volatile float v_mapAccumulator = 0;
/**
 * Running counter of measurements to consider for averaging
 */
static volatile int mapMeasurementsCounter = 0;

/**
 * v_ for Voltage
 */
static float v_averagedMapValue;

EXTERN_ENGINE;

static scheduling_s startTimer[2];
static scheduling_s endTimer[2];

static void startAveraging(void *arg) {
	(void) arg;
	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#9");
	bool wasLocked = lockAnyContext();
	;
	// with locking we would have a consistent state
	v_mapAccumulator = 0;
	mapMeasurementsCounter = 0;
	if (!wasLocked)
		chSysUnlockFromIsr()
	;
}

/**
 * This method is invoked from ADC callback.
 * @note This method is invoked OFTEN, this method is a potential bottle-next - the implementation should be
 * as fast as possible
 */
void mapAveragingCallback(adcsample_t value) {
	/* Calculates the average values from the ADC samples.*/
	perRevolutionCounter++;
	efiAssertVoid(getRemainingStack(chThdSelf()) > 128, "lowstck#9a");

	float voltage = adcToVoltsDivided(value);
	float currentPressure = getMapByVoltage(voltage);

#if EFI_ANALOG_CHART
	if (boardConfiguration->analogChartMode == AC_MAP)
		if (perRevolutionCounter % FAST_MAP_CHART_SKIP_FACTOR == 0)
			acAddData(getCrankshaftAngleNt(getTimeNowNt() PASS_ENGINE_PARAMETER), currentPressure);
#endif /* EFI_ANALOG_CHART */

	chSysLockFromIsr()
	;
	// with locking we would have a consistent state

	v_mapAccumulator += voltage;
	mapMeasurementsCounter++;
	chSysUnlockFromIsr()
	;
}

static void endAveraging(void *arg) {
	(void) arg;
	bool wasLocked = lockAnyContext();
	// with locking we would have a consistent state
	v_averagedMapValue = v_mapAccumulator / mapMeasurementsCounter;
	if (!wasLocked)
		chSysUnlockFromIsr()
	;
}

/**
 * Shaft Position callback used to schedule start and end of MAP averaging
 */
static void mapAveragingCallback(trigger_event_e ckpEventType, uint32_t index DECLARE_ENGINE_PARAMETER_S) {
	// this callback is invoked on interrupt thread

	if (index != 0)
		return;

	int rpm = getRpmE(engine);
	if (!isValidRpm(rpm))
		return;

	perRevolution = perRevolutionCounter;
	perRevolutionCounter = 0;

	MAP_sensor_config_s * config = &engineConfiguration->map;

	float startAngle = interpolate2d(rpm, config->samplingAngleBins, config->samplingAngle, MAP_ANGLE_SIZE);
	float windowAngle = interpolate2d(rpm, config->samplingWindowBins, config->samplingWindow, MAP_WINDOW_SIZE);
	if (windowAngle <= 0) {
		firmwareError("map sampling angle should be positive");
		return;
	}

	int structIndex = getRevolutionCounter() % 2;
	// todo: schedule this based on closest trigger event, same as ignition works
	scheduleByAngle(rpm, &startTimer[structIndex], startAngle, startAveraging, NULL);
	scheduleByAngle(rpm, &endTimer[structIndex], startAngle + windowAngle, endAveraging, NULL);
}

static void showMapStats(void) {
	scheduleMsg(logger, "per revolution %d", perRevolution);
}

float getMapVoltage(void) {
	return v_averagedMapValue;
}

/**
 * Because of MAP window averaging, MAP is only available while engine is spinning
 * @return Manifold Absolute Pressure, in kPa
 */
float getMap(void) {
	if (getRpm() == 0)
		return getRawMap(); // maybe return NaN in case of stopped engine?
	return getMapByVoltage(v_averagedMapValue);
}

void initMapAveraging(Logging *sharedLogger, Engine *engine) {
	logger = sharedLogger;

	startTimer[0].name = "map start0";
	startTimer[1].name = "map start1";
	endTimer[0].name = "map end0";
	endTimer[1].name = "map end1";

	addTriggerEventListener(&mapAveragingCallback, "MAP averaging", engine);
	addConsoleAction("faststat", showMapStats);
}

#else

float getMap(void) {
	return getRawMap();
}

#endif /* EFI_MAP_AVERAGING */
