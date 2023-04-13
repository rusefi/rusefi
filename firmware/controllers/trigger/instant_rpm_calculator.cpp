
#include "pch.h"
#include "instant_rpm_calculator.h"

#if EFI_SENSOR_CHART
#include "sensor_chart.h"
#endif

/**
 * sensorChartMode
 */
#include "engine_state.h"

#if EFI_SHAFT_POSITION_INPUT

InstantRpmCalculator::InstantRpmCalculator() :
			//https://en.cppreference.com/w/cpp/language/zero_initialization
			timeOfLastEvent()
			, instantRpmValue()
	{
}

void InstantRpmCalculator::movePreSynchTimestamps() {
	// here we take timestamps of events which happened prior to synchronization and place them
	// at appropriate locations
	auto triggerSize = getTriggerCentral()->triggerShape.getLength();

	size_t eventsToCopy = minI(spinningEventIndex, triggerSize);

	size_t firstSrc;
	size_t firstDst;

	if (eventsToCopy >= triggerSize) {
		// Only copy one trigger length worth of events, filling the whole buffer
		firstSrc = spinningEventIndex - triggerSize;
		firstDst = 0;
	} else {
		// There is less than one full cycle, copy to the end of the buffer
		firstSrc = 0;
		firstDst = triggerSize - spinningEventIndex;
	}

	memcpy(timeOfLastEvent + firstDst, spinningEvents + firstSrc, eventsToCopy * sizeof(timeOfLastEvent[0]));
}

float InstantRpmCalculator::calculateInstantRpm(
	TriggerWaveform const & triggerShape, TriggerFormDetails *triggerFormDetails,
	uint32_t current_index, efitick_t nowNt) {

	assertIsInBoundsWithResult(current_index, timeOfLastEvent, "calc timeOfLastEvent", 0);

	// Record the time of this event so we can calculate RPM from it later
	timeOfLastEvent[current_index] = nowNt;

	// Determine where we currently are in the revolution
	angle_t currentAngle = triggerFormDetails->eventAngles[current_index];
	efiAssert(ObdCode::OBD_PCM_Processor_Fault, !cisnan(currentAngle), "eventAngles", 0);

	// Hunt for a tooth ~90 degrees ago to compare to the current time
	angle_t previousAngle = currentAngle - engineConfiguration->instantRpmRange;
	fixAngle(previousAngle, "prevAngle", ObdCode::CUSTOM_ERR_TRIGGER_ANGLE_RANGE);
	int prevIndex = triggerShape.findAngleIndex(triggerFormDetails, previousAngle);

	// now let's get precise angle for that event
	angle_t prevIndexAngle = triggerFormDetails->eventAngles[prevIndex];
	efitick_t time90ago = timeOfLastEvent[prevIndex];

	// No previous timestamp, instant RPM isn't ready yet
	if (time90ago == 0) {
		return prevInstantRpmValue;
	}

	// It's OK to truncate from 64b to 32b, ARM with single precision FPU uses an expensive
	// software function to convert 64b int -> float, while 32b int -> float is very cheap hardware conversion
	// The difference is guaranteed to be short (it's 90 degrees of engine rotation!), so it won't overflow.
	uint32_t time = nowNt - time90ago;
	angle_t angleDiff = currentAngle - prevIndexAngle;

	// Wrap the angle in to the correct range (ie, could be -630 when we want +90)
	fixAngle(angleDiff, "angleDiff", ObdCode::CUSTOM_ERR_6561);

	// just for safety, avoid divide-by-0
	if (time == 0) {
		return prevInstantRpmValue;
	}

	float instantRpm = (60000000.0 / 360 * US_TO_NT_MULTIPLIER) * angleDiff / time;
	assertIsInBoundsWithResult(current_index, instantRpmValue, "instantRpmValue", 0);
	instantRpmValue[current_index] = instantRpm;

	// This fixes early RPM instability based on incomplete data
	if (instantRpm < RPM_LOW_THRESHOLD) {
		return prevInstantRpmValue;
	}

	prevInstantRpmValue = instantRpm;

	m_instantRpmRatio = instantRpm / instantRpmValue[prevIndex];

	return instantRpm;
}

void InstantRpmCalculator::setLastEventTimeForInstantRpm(efitick_t nowNt) {
	// here we remember tooth timestamps which happen prior to synchronization
	if (spinningEventIndex >= efi::size(spinningEvents)) {
		// too many events while trying to find synchronization point
		// todo: better implementation would be to shift here or use cyclic buffer so that we keep last
		// 'PRE_SYNC_EVENTS' events
		return;
	}

	spinningEvents[spinningEventIndex] = nowNt;

	// If we are using only rising edges, we never write in to the odd-index slots that
	// would be used by falling edges
	// TODO: don't reach across to trigger central to get this info
	spinningEventIndex += getTriggerCentral()->triggerShape.useOnlyRisingEdges ? 2 : 1;
}

void InstantRpmCalculator::updateInstantRpm(
		uint32_t current_index,
	TriggerWaveform const & triggerShape, TriggerFormDetails *triggerFormDetails,
	uint32_t index, efitick_t nowNt) {

	m_instantRpm = calculateInstantRpm(triggerShape, triggerFormDetails, index,
					   nowNt);

#if EFI_SENSOR_CHART
	if (getEngineState()->sensorChartMode == SC_RPM_ACCEL || getEngineState()->sensorChartMode == SC_DETAILED_RPM) {
		angle_t currentAngle = triggerFormDetails->eventAngles[current_index];
		if (engineConfiguration->sensorChartMode == SC_DETAILED_RPM) {
			scAddData(currentAngle, m_instantRpm);
		} else {
			scAddData(currentAngle, m_instantRpmRatio);
		}
	}
#endif /* EFI_SENSOR_CHART */
}

#endif // EFI_SHAFT_POSITION_INPUT