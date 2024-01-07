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



#if EFI_MAP_AVERAGING

#include "map_averaging.h"
#include "trigger_central.h"

#if EFI_SENSOR_CHART
#include "sensor_chart.h"
#endif /* EFI_SENSOR_CHART */

/**
 * this instance does not have a real physical pin - it's only used for engine sniffer
 *
 * todo: we can kind of add real physical pin just for a very narrow case of troubleshooting but only if we ever need it :)
 */
static NamedOutputPin mapAveragingPin("map");

// allow smoothing up to number of cylinders
#define MAX_MAP_BUFFER_LENGTH (MAX_CYLINDER_COUNT)
// in MAP units, not voltage!
static float averagedMapRunningBuffer[MAX_MAP_BUFFER_LENGTH];
int mapMinBufferLength = 0;
static int averagedMapBufIdx = 0;

/**
 * here we have averaging start and averaging end points for each cylinder
 */
static CCM_OPTIONAL scheduling_s startTimers[MAX_CYLINDER_COUNT][2];
static CCM_OPTIONAL scheduling_s endTimers[MAX_CYLINDER_COUNT][2];

static void endAveraging(MapAverager* arg);

static size_t currentMapAverager = 0;

static void startAveraging(scheduling_s *endAveragingScheduling) {
	efiAssertVoid(ObdCode::CUSTOM_ERR_6649, getCurrentRemainingStack() > 128, "lowstck#9");

	// TODO: set currentMapAverager based on cylinder bank
	auto& averager = getMapAvg(currentMapAverager);
	averager.start();

	mapAveragingPin.setHigh();

	scheduleByAngle(endAveragingScheduling, getTimeNowNt(), engine->engineState.mapAveragingDuration,
		{ endAveraging, &averager });
}

void MapAverager::start() {
	chibios_rt::CriticalSectionLocker csl;

	m_counter = 0;
	m_sum = 0;
	m_isAveraging = true;
}

SensorResult MapAverager::submit(float volts) {
	auto result = m_function ? m_function->convert(volts) : unexpected;

	if (m_isAveraging && result) {
		chibios_rt::CriticalSectionLocker csl;

		m_counter++;
		m_sum += result.Value;
	}

	return result;
}

void MapAverager::stop() {
	chibios_rt::CriticalSectionLocker csl;

	m_isAveraging = false;

	if (m_counter > 0) {
		float averageMap = m_sum / m_counter;
		m_lastCounter = m_counter;

		// TODO: this should be per-sensor, not one for all MAP sensors
		averagedMapRunningBuffer[averagedMapBufIdx] = averageMap;
		// increment circular running buffer index
		averagedMapBufIdx = (averagedMapBufIdx + 1) % mapMinBufferLength;
		// find min. value (only works for pressure values, not raw voltages!)
		float minPressure = averagedMapRunningBuffer[0];
		for (int i = 1; i < mapMinBufferLength; i++) {
			if (averagedMapRunningBuffer[i] < minPressure)
				minPressure = averagedMapRunningBuffer[i];
		}

		setValidValue(minPressure, getTimeNowNt());
	} else {
#if EFI_PROD_CODE
		warning(ObdCode::CUSTOM_UNEXPECTED_MAP_VALUE, "No MAP values");
#endif
	}
}

#if HAL_USE_ADC

/**
 * This method is invoked from ADC callback.
 * @note This method is invoked OFTEN, this method is a potential bottleneck - the implementation should be
 * as fast as possible
 */
void mapAveragingAdcCallback(float instantVoltage) {
	efiAssertVoid(ObdCode::CUSTOM_ERR_6650, getCurrentRemainingStack() > 128, "lowstck#9a");

	SensorResult mapResult = getMapAvg(currentMapAverager).submit(instantVoltage);

	if (!mapResult) {
		// hopefully this warning is not too much CPU consumption for fast ADC callback
		warning(ObdCode::CUSTOM_INSTANT_MAP_DECODING, "Invalid MAP at %f", instantVoltage);
	}

	float instantMap = mapResult.value_or(0);
#if EFI_TUNER_STUDIO
	engine->outputChannels.instantMAPValue = instantMap;
#endif // EFI_TUNER_STUDIO
}
#endif

static void endAveraging(MapAverager* arg) {
	arg->stop();

	mapAveragingPin.setLow();
}

static void applyMapMinBufferLength() {
	// check range
	mapMinBufferLength = maxI(minI(engineConfiguration->mapMinBufferLength, MAX_MAP_BUFFER_LENGTH), 1);
	// reset index
	averagedMapBufIdx = 0;
	// fill with maximum values
	for (int i = 0; i < mapMinBufferLength; i++) {
		averagedMapRunningBuffer[i] = FLT_MAX;
	}
}

#if EFI_TUNER_STUDIO
void postMapState(TunerStudioOutputChannels *tsOutputChannels) {
	tsOutputChannels->debugFloatField2 = engine->engineState.mapAveragingDuration;
}
#endif /* EFI_TUNER_STUDIO */

void refreshMapAveragingPreCalc() {
	int rpm = Sensor::getOrZero(SensorType::Rpm);
	if (isValidRpm(rpm)) {
		MAP_sensor_config_s * c = &engineConfiguration->map;
		angle_t start = interpolate2d(rpm, c->samplingAngleBins, c->samplingAngle);
		efiAssertVoid(ObdCode::CUSTOM_ERR_MAP_START_ASSERT, !cisnan(start), "start");

		angle_t offsetAngle = engine->triggerCentral.triggerFormDetails.eventAngles[engineConfiguration->mapAveragingSchedulingAtIndex];
		efiAssertVoid(ObdCode::CUSTOM_ERR_MAP_AVG_OFFSET, !cisnan(offsetAngle), "offsetAngle");

		for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
			angle_t cylinderOffset = getEngineCycle(getEngineRotationState()->getOperationMode()) * i / engineConfiguration->cylindersCount;
			efiAssertVoid(ObdCode::CUSTOM_ERR_MAP_CYL_OFFSET, !cisnan(cylinderOffset), "cylinderOffset");
			// part of this formula related to specific cylinder offset is never changing - we can
			// move the loop into start-up calculation and not have this loop as part of periodic calculation
			// todo: change the logic as described above in order to reduce periodic CPU usage?
			float cylinderStart = start + cylinderOffset - offsetAngle + tdcPosition();
			wrapAngle(cylinderStart, "cylinderStart", ObdCode::CUSTOM_ERR_6562);
			engine->engineState.mapAveragingStart[i] = cylinderStart;
		}
		engine->engineState.mapAveragingDuration = interpolate2d(rpm, c->samplingWindowBins, c->samplingWindow);
	} else {
		for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
			engine->engineState.mapAveragingStart[i] = NAN;
		}
		engine->engineState.mapAveragingDuration = NAN;
	}

}

/**
 * Shaft Position callback used to schedule start and end of MAP averaging
 */
void mapAveragingTriggerCallback(
		uint32_t index, efitick_t edgeTimestamp) {
#if EFI_ENGINE_CONTROL
	// this callback is invoked on interrupt thread
	if (index != (uint32_t)engineConfiguration->mapAveragingSchedulingAtIndex)
		return;

	int rpm = Sensor::getOrZero(SensorType::Rpm);
	if (!isValidRpm(rpm)) {
		return;
	}

	ScopePerf perf(PE::MapAveragingTriggerCallback);

	if (engineConfiguration->mapMinBufferLength != mapMinBufferLength) {
		applyMapMinBufferLength();
	}

	// todo: this could be pre-calculated
	int samplingCount = engineConfiguration->measureMapOnlyInOneCylinder ? 1 : engineConfiguration->cylindersCount;

	for (int i = 0; i < samplingCount; i++) {
		angle_t samplingStart = engine->engineState.mapAveragingStart[i];

		angle_t samplingDuration = engine->engineState.mapAveragingDuration;
		// todo: this assertion could be moved out of trigger handler
		assertAngleRange(samplingDuration, "samplingDuration", ObdCode::CUSTOM_ERR_6563);
		if (samplingDuration <= 0) {
			warning(ObdCode::CUSTOM_MAP_ANGLE_PARAM, "map sampling angle should be positive");
			return;
		}

		angle_t samplingEnd = samplingStart + samplingDuration;

		if (cisnan(samplingEnd)) {
			// todo: when would this happen?
			warning(ObdCode::CUSTOM_ERR_6549, "no map angles");
			return;
		}

		// todo: pre-calculate samplingEnd for each cylinder
		wrapAngle(samplingEnd, "samplingEnd", ObdCode::CUSTOM_ERR_6563);
		// only if value is already prepared
		int structIndex = getRevolutionCounter() % 2;

		scheduling_s *startTimer = &startTimers[i][structIndex];
		scheduling_s *endTimer = &endTimers[i][structIndex];

		// at the moment we schedule based on time prediction based on current RPM and angle
		// we are loosing precision in case of changing RPM - the further away is the event the worse is precision
		// todo: schedule this based on closest trigger event, same as ignition works
		scheduleByAngle(startTimer, edgeTimestamp, samplingStart,
				{ startAveraging, endTimer });
	}
#endif
}

void initMapAveraging() {
	applyMapMinBufferLength();
}

#endif /* EFI_MAP_AVERAGING */
