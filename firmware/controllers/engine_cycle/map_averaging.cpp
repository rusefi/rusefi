/**
 * @file	map_averaging.cpp
 *
 * In order to have best MAP estimate possible, we real MAP value at a relatively high frequency
 * and average the value within a specified angle position window for each cylinder
 *
 * @date Dec 11, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
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

#include "pch.h"

#include "os_access.h"

#if EFI_MAP_AVERAGING

#include "map_averaging.h"
#include "trigger_central.h"

#if EFI_SENSOR_CHART
#include "sensor_chart.h"
#endif /* EFI_SENSOR_CHART */

#define FAST_MAP_CHART_SKIP_FACTOR 16

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

// allow smoothing up to number of cylinders
#define MAX_MAP_BUFFER_LENGTH (MAX_CYLINDER_COUNT)
// in MAP units, not voltage!
static float averagedMapRunningBuffer[MAX_MAP_BUFFER_LENGTH];
int mapMinBufferLength = 0;
static int averagedMapBufIdx = 0;

/**
 * here we have averaging start and averaging end points for each cylinder
 */
static scheduling_s startTimers[MAX_CYLINDER_COUNT][2];
static scheduling_s endTimers[MAX_CYLINDER_COUNT][2];

/**
 * that's a performance optimization: let's not bother averaging
 * if we are outside of of the window
 */
static bool isAveraging = false;

static void endAveraging(void *arg);

static void startAveraging(scheduling_s *endAveragingScheduling) {
	efiAssertVoid(CUSTOM_ERR_6649, getCurrentRemainingStack() > 128, "lowstck#9");

	{
		// with locking we will have a consistent state
		chibios_rt::CriticalSectionLocker csl;
		mapAdcAccumulator = 0;
		mapMeasurementsCounter = 0;
		isAveraging = true;
	}

	mapAveragingPin.setHigh();

	scheduleByAngle(endAveragingScheduling, getTimeNowNt(), ENGINE(engineState.mapAveragingDuration),
		endAveraging);
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
			float currentPressure = convertMap(voltage).value_or(0);
			scAddData(
					getCrankshaftAngleNt(getTimeNowNt()),
					currentPressure);
		}
	}
#endif /* EFI_SENSOR_CHART */

	{
		// with locking we will have a consistent state
		chibios_rt::CriticalSectionLocker csl;
		mapAdcAccumulator += adcValue;
		mapMeasurementsCounter++;
	}
}
#endif

static void endAveraging(void*) {
#if ! EFI_UNIT_TEST
	chibios_rt::CriticalSectionLocker csl;
#endif
	isAveraging = false;
	// with locking we would have a consistent state
#if HAL_USE_ADC
	if (mapMeasurementsCounter > 0) {
		v_averagedMapValue = adcToVoltsDivided(mapAdcAccumulator / mapMeasurementsCounter);

		SensorResult mapValue = convertMap(v_averagedMapValue);

		// Skip update if conversion invalid
		if (mapValue) {
			averagedMapRunningBuffer[averagedMapBufIdx] = mapValue.Value;
			// increment circular running buffer index
			averagedMapBufIdx = (averagedMapBufIdx + 1) % mapMinBufferLength;
			// find min. value (only works for pressure values, not raw voltages!)
			float minPressure = averagedMapRunningBuffer[0];
			for (int i = 1; i < mapMinBufferLength; i++) {
				if (averagedMapRunningBuffer[i] < minPressure)
					minPressure = averagedMapRunningBuffer[i];
			}

			onMapAveraged(minPressure, getTimeNowNt());
		}
	} else {
		warning(CUSTOM_UNEXPECTED_MAP_VALUE, "No MAP values");
	}
#endif
	mapAveragingPin.setLow();
}

static void applyMapMinBufferLength(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// check range
	mapMinBufferLength = maxI(minI(CONFIG(mapMinBufferLength), MAX_MAP_BUFFER_LENGTH), 1);
	// reset index
	averagedMapBufIdx = 0;
	// fill with maximum values
	for (int i = 0; i < mapMinBufferLength; i++) {
		averagedMapRunningBuffer[i] = FLT_MAX;
	}
}

#if EFI_TUNER_STUDIO
void postMapState(TunerStudioOutputChannels *tsOutputChannels) {
	tsOutputChannels->debugFloatField1 = v_averagedMapValue;
	tsOutputChannels->debugFloatField2 = engine->engineState.mapAveragingDuration;
	tsOutputChannels->debugFloatField3 = Sensor::getOrZero(SensorType::MapFast);
	tsOutputChannels->debugIntField1 = mapMeasurementsCounter;
}
#endif /* EFI_TUNER_STUDIO */

void refreshMapAveragingPreCalc(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	int rpm = GET_RPM();
	if (isValidRpm(rpm)) {
		MAP_sensor_config_s * c = &engineConfiguration->map;
		angle_t start = interpolate2d(rpm, c->samplingAngleBins, c->samplingAngle);
		efiAssertVoid(CUSTOM_ERR_MAP_START_ASSERT, !cisnan(start), "start");

		angle_t offsetAngle = ENGINE(triggerCentral.triggerFormDetails).eventAngles[CONFIG(mapAveragingSchedulingAtIndex)];
		efiAssertVoid(CUSTOM_ERR_MAP_AVG_OFFSET, !cisnan(offsetAngle), "offsetAngle");

		for (size_t i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
			angle_t cylinderOffset = getEngineCycle(engine->getOperationMode()) * i / engineConfiguration->specs.cylindersCount;
			efiAssertVoid(CUSTOM_ERR_MAP_CYL_OFFSET, !cisnan(cylinderOffset), "cylinderOffset");
			// part of this formula related to specific cylinder offset is never changing - we can
			// move the loop into start-up calculation and not have this loop as part of periodic calculation
			// todo: change the logic as described above in order to reduce periodic CPU usage?
			float cylinderStart = start + cylinderOffset - offsetAngle + tdcPosition();
			fixAngle(cylinderStart, "cylinderStart", CUSTOM_ERR_6562);
			engine->engineState.mapAveragingStart[i] = cylinderStart;
		}
		engine->engineState.mapAveragingDuration = interpolate2d(rpm, c->samplingWindowBins, c->samplingWindow);
	} else {
		for (size_t i = 0; i < engineConfiguration->specs.cylindersCount; i++) {
			engine->engineState.mapAveragingStart[i] = NAN;
		}
		engine->engineState.mapAveragingDuration = NAN;
	}

}

/**
 * Shaft Position callback used to schedule start and end of MAP averaging
 */
void mapAveragingTriggerCallback(
		uint32_t index, efitick_t edgeTimestamp DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_ENGINE_CONTROL
	// this callback is invoked on interrupt thread
	if (index != (uint32_t)CONFIG(mapAveragingSchedulingAtIndex))
		return;

	int rpm = GET_RPM();
	if (!isValidRpm(rpm)) {
		return;
	}

	ScopePerf perf(PE::MapAveragingTriggerCallback);

	if (CONFIG(mapMinBufferLength) != mapMinBufferLength) {
		applyMapMinBufferLength();
	}

	measurementsPerRevolution = measurementsPerRevolutionCounter;
	measurementsPerRevolutionCounter = 0;

	// todo: this could be pre-calculated
	int samplingCount = CONFIG(measureMapOnlyInOneCylinder) ? 1 : engineConfiguration->specs.cylindersCount;

	for (int i = 0; i < samplingCount; i++) {
		angle_t samplingStart = ENGINE(engineState.mapAveragingStart[i]);

		angle_t samplingDuration = ENGINE(engineState.mapAveragingDuration);
		// todo: this assertion could be moved out of trigger handler
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

		// todo: pre-calculate samplingEnd for each cylinder
		fixAngle(samplingEnd, "samplingEnd", CUSTOM_ERR_6563);
		// only if value is already prepared
		int structIndex = getRevolutionCounter() % 2;

		scheduling_s *starTimer = &startTimers[i][structIndex];
		scheduling_s *endTimer = &endTimers[i][structIndex];
		mapAveragingPin.inject();
		starTimer->inject();
		endTimer->inject();

		// at the moment we schedule based on time prediction based on current RPM and angle
		// we are loosing precision in case of changing RPM - the further away is the event the worse is precision
		// todo: schedule this based on closest trigger event, same as ignition works
		scheduleByAngle(starTimer, edgeTimestamp, samplingStart,
				{ startAveraging, endTimer });
	}
#endif
}

static void showMapStats() {
	efiPrintf("per revolution %d", measurementsPerRevolution);
}

void initMapAveraging(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if !EFI_UNIT_TEST
	addConsoleAction("faststat", showMapStats);
#endif /* EFI_UNIT_TEST */

	applyMapMinBufferLength();
}

#endif /* EFI_MAP_AVERAGING */
