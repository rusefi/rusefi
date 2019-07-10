/**
 * @file	map_averaging.cpp
 *
 * In order to have best MAP estimate possible, we real MAP value at a relatively high frequency
 * and average the value within a specified angle position window for each cylinder
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
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

#include "global.h"
#include "os_access.h"

#include "map.h"

#if EFI_MAP_AVERAGING

#include "map_averaging.h"
#include "trigger_central.h"
#include "adc_inputs.h"
#include "allsensors.h"
#include "engine_configuration.h"
#include "interpolation.h"
#include "signal_executor.h"
#include "engine.h"
#include "engine_math.h"

#if EFI_SENSOR_CHART
#include "sensor_chart.h"
#endif /* EFI_SENSOR_CHART */

#define FAST_MAP_CHART_SKIP_FACTOR 16

static Logging *logger;
/**
 * this instance does not have a real physical pin - it's only used for engine sniffer
 */
static NamedOutputPin mapAveragingPin("map");

/**
 * Running counter of measurements per revolution
 */
static volatile int measurementsPerRevolutionCounter = 0;
/**
 * Number of measurements in previous shaft revolution
 */
static volatile int measurementsPerRevolution = 0;

/**
 * In this lock-free implementation 'readIndex' is always pointing
 * to the consistent copy of accumulator and counter pair
 */
static int readIndex = 0;
static float accumulators[2];
static int counters[2];

/**
 * Running MAP accumulator - sum of all measurements within averaging window
 */
static volatile float mapAdcAccumulator = 0;
/**
 * Running counter of measurements to consider for averaging
 */
static volatile int mapMeasurementsCounter = 0;

/**
 * v_ for Voltage
 */
static float v_averagedMapValue;

// allow a bit more smoothing
#define MAX_MAP_BUFFER_LENGTH (INJECTION_PIN_COUNT * 2)
// in MAP units, not voltage!
static float averagedMapRunningBuffer[MAX_MAP_BUFFER_LENGTH];
int mapMinBufferLength = 0;
static int averagedMapBufIdx = 0;
// we need this 'NO_VALUE_YET' to properly handle transition from engine not running to engine already running
// but prior to first processed result
#define NO_VALUE_YET -100
// this is 'minimal averaged' MAP within avegaging window
static float currentPressure = NO_VALUE_YET;

EXTERN_ENGINE
;

/**
 * here we have averaging start and averaging end points for each cylinder
 */
static scheduling_s startTimer[INJECTION_PIN_COUNT][2];
static scheduling_s endTimer[INJECTION_PIN_COUNT][2];

/**
 * that's a performance optimization: let's not bother averaging
 * if we are outside of of the window
 */
static bool isAveraging = false;

static void startAveraging(void *arg) {
	(void) arg;
	efiAssertVoid(CUSTOM_ERR_6649, getCurrentRemainingStack() > 128, "lowstck#9");
	bool wasLocked = lockAnyContext();
	;
	// with locking we would have a consistent state
	mapAdcAccumulator = 0;
	mapMeasurementsCounter = 0;
	isAveraging = true;
	if (!wasLocked)
		unlockAnyContext();
	;
	mapAveragingPin.setHigh();
}

#if HAL_USE_ADC
/**
 * This method is invoked from ADC callback.
 * @note This method is invoked OFTEN, this method is a potential bottle-next - the implementation should be
 * as fast as possible
 */
void mapAveragingAdcCallback(adcsample_t adcValue) {
	if (!isAveraging && ENGINE(sensorChartMode) != SC_MAP) {
		return;
	}

	/* Calculates the average values from the ADC samples.*/
	measurementsPerRevolutionCounter++;
	efiAssertVoid(CUSTOM_ERR_6650, getCurrentRemainingStack() > 128, "lowstck#9a");

#if EFI_SENSOR_CHART && EFI_ANALOG_SENSORS
	if (ENGINE(sensorChartMode) == SC_MAP) {
		if (measurementsPerRevolutionCounter % FAST_MAP_CHART_SKIP_FACTOR
				== 0) {
			float voltage = adcToVoltsDivided(adcValue);
			float currentPressure = getMapByVoltage(voltage);
			scAddData(
					getCrankshaftAngleNt(getTimeNowNt() PASS_ENGINE_PARAMETER_SUFFIX),
					currentPressure);
		}
	}
#endif /* EFI_SENSOR_CHART */

	/**
	 * Local copy is now safe, but it's an overkill: we only
	 * have one writing thread anyway
	 */
	int readIndexLocal = readIndex;
	int writeIndex = readIndexLocal ^ 1;
	accumulators[writeIndex] = accumulators[readIndexLocal] + adcValue;
	counters[writeIndex] = counters[readIndexLocal] + 1;
	// this would commit the new pair of values
	readIndex = writeIndex;

	// todo: migrate to the lock-free implementation
	bool alreadyLocked = lockAnyContext();
	;
	// with locking we would have a consistent state

	mapAdcAccumulator += adcValue;
	mapMeasurementsCounter++;
	if (!alreadyLocked)
		unlockAnyContext();
	;
}
#endif

static void endAveraging(void *arg) {
	(void) arg;
	bool wasLocked = lockAnyContext();
	isAveraging = false;
	// with locking we would have a consistent state
#if HAL_USE_ADC
	if (mapMeasurementsCounter > 0) {
		v_averagedMapValue = adcToVoltsDivided(mapAdcAccumulator / mapMeasurementsCounter);
		// todo: move out of locked context?
		averagedMapRunningBuffer[averagedMapBufIdx] = getMapByVoltage(v_averagedMapValue);
		// increment circular running buffer index
		averagedMapBufIdx = (averagedMapBufIdx + 1) % mapMinBufferLength;
		// find min. value (only works for pressure values, not raw voltages!)
		float minPressure = averagedMapRunningBuffer[0];
		for (int i = 1; i < mapMinBufferLength; i++) {
			if (averagedMapRunningBuffer[i] < minPressure)
				minPressure = averagedMapRunningBuffer[i];
		}
		currentPressure = minPressure;
	} else {
		warning(CUSTOM_UNEXPECTED_MAP_VALUE, "No MAP values");
	}
#endif
	if (!wasLocked)
		unlockAnyContext();
	;
	mapAveragingPin.setLow();
}

static void applyMapMinBufferLength() {
	// check range
	mapMinBufferLength = maxI(minI(CONFIGB(mapMinBufferLength), MAX_MAP_BUFFER_LENGTH), 1);
	// reset index
	averagedMapBufIdx = 0;
	// fill with maximum values
	for (int i = 0; i < mapMinBufferLength; i++) {
		averagedMapRunningBuffer[i] = FLT_MAX;
	}
}

void postMapState(TunerStudioOutputChannels *tsOutputChannels) {
#if EFI_TUNER_STUDIO
	tsOutputChannels->debugFloatField1 = v_averagedMapValue;
	tsOutputChannels->debugFloatField2 = engine->engineState.mapAveragingDuration;
	tsOutputChannels->debugFloatField3 = currentPressure;
	tsOutputChannels->debugIntField1 = mapMeasurementsCounter;
#endif /* EFI_TUNER_STUDIO */
}

void refreshMapAveragingPreCalc(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	int rpm = GET_RPM_VALUE;
	if (isValidRpm(rpm)) {
		MAP_sensor_config_s * c = &engineConfiguration->map;
		angle_t start = interpolate2d("mapa", rpm, c->samplingAngleBins, c->samplingAngle);
		efiAssertVoid(CUSTOM_ERR_6690, !cisnan(start), "start");

		angle_t offsetAngle = TRIGGER_SHAPE(eventAngles[CONFIG(mapAveragingSchedulingAtIndex)]);
		efiAssertVoid(CUSTOM_ERR_MAP_AVG_OFFSET, !cisnan(offsetAngle), "offsetAngle");

		for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
			angle_t cylinderOffset = getEngineCycle(engineConfiguration->operationMode) * i / engineConfiguration->specs.cylindersCount;
			efiAssertVoid(CUSTOM_ERR_6692, !cisnan(cylinderOffset), "cylinderOffset");
			float cylinderStart = start + cylinderOffset - offsetAngle + tdcPosition();
			fixAngle(cylinderStart, "cylinderStart", CUSTOM_ERR_6562);
			engine->engineState.mapAveragingStart[i] = cylinderStart;
		}
		engine->engineState.mapAveragingDuration = interpolate2d("samp", rpm, c->samplingWindowBins, c->samplingWindow);
	} else {
		for (int i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
			engine->engineState.mapAveragingStart[i] = NAN;
		}
		engine->engineState.mapAveragingDuration = NAN;
	}

}

/**
 * Shaft Position callback used to schedule start and end of MAP averaging
 */
static void mapAveragingTriggerCallback(trigger_event_e ckpEventType,
		uint32_t index DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_ENGINE_CONTROL
	// this callback is invoked on interrupt thread
	UNUSED(ckpEventType);
	if (index != CONFIG(mapAveragingSchedulingAtIndex))
		return;

	engine->m.beforeMapAveragingCb = getTimeNowLowerNt();
	int rpm = GET_RPM_VALUE;
	if (!isValidRpm(rpm)) {
		return;
	}

	if (CONFIGB(mapMinBufferLength) != mapMinBufferLength) {
		applyMapMinBufferLength();
	}

	measurementsPerRevolution = measurementsPerRevolutionCounter;
	measurementsPerRevolutionCounter = 0;

	int samplingCount = CONFIGB(measureMapOnlyInOneCylinder) ? 1 : engineConfiguration->specs.cylindersCount;

	for (int i = 0; i < samplingCount; i++) {
		angle_t samplingStart = ENGINE(engineState.mapAveragingStart[i]);

		angle_t samplingDuration = ENGINE(engineState.mapAveragingDuration);
		assertAngleRange(samplingDuration, "samplingDuration", CUSTOM_ERR_6563);
		if (samplingDuration <= 0) {
			warning(CUSTOM_MAP_ANGLE_PARAM, "map sampling angle should be positive");
			return;
		}

		angle_t samplingEnd = samplingStart + samplingDuration;

		if (cisnan(samplingEnd)) {
			// todo: when would this happen?
			warning(CUSTOM_ERR_6549, "no map angles");
			return;
		}


		fixAngle(samplingEnd, "samplingEnd", CUSTOM_ERR_6563);
		// only if value is already prepared
		int structIndex = engine->rpmCalculator.getRevolutionCounter() % 2;
		// todo: schedule this based on closest trigger event, same as ignition works
		scheduleByAngle(rpm, &startTimer[i][structIndex], samplingStart,
				startAveraging, NULL, &engine->rpmCalculator);
		scheduleByAngle(rpm, &endTimer[i][structIndex], samplingEnd,
				endAveraging, NULL, &engine->rpmCalculator);
		engine->m.mapAveragingCbTime = getTimeNowLowerNt()
				- engine->m.beforeMapAveragingCb;
	}
#endif
}

static void showMapStats(void) {
	scheduleMsg(logger, "per revolution %d", measurementsPerRevolution);
}

#if EFI_PROD_CODE

/**
 * Because of MAP window averaging, MAP is only available while engine is spinning
 * @return Manifold Absolute Pressure, in kPa
 */
float getMap(void) {
	if (engineConfiguration->hasFrequencyReportingMapSensor) {
		return getRawMap();
	}

#if EFI_ANALOG_SENSORS
	if (!isValidRpm(GET_RPM_VALUE) || currentPressure == NO_VALUE_YET)
		return validateMap(getRawMap()); // maybe return NaN in case of stopped engine?
	return validateMap(currentPressure);
#else
	return 100;
#endif
}
#endif /* EFI_PROD_CODE */

void initMapAveraging(Logging *sharedLogger, Engine *engine) {
	logger = sharedLogger;

//	startTimer[0].name = "map start0";
//	startTimer[1].name = "map start1";
//	endTimer[0].name = "map end0";
//	endTimer[1].name = "map end1";

#if EFI_SHAFT_POSITION_INPUT
	addTriggerEventListener(&mapAveragingTriggerCallback, "MAP averaging", engine);
#endif /* EFI_SHAFT_POSITION_INPUT */
	addConsoleAction("faststat", showMapStats);
	applyMapMinBufferLength();
}

#else

#if EFI_PROD_CODE

float getMap(void) {
#if EFI_ANALOG_SENSORS
	return getRawMap();
#else
	return NAN;
#endif /* EFI_ANALOG_SENSORS */
}
#endif /* EFI_PROD_CODE */

#endif /* EFI_MAP_AVERAGING */
