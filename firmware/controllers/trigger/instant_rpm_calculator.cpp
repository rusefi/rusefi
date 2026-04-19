
#include "pch.h"
#include "instant_rpm_calculator.h"

/**
 * sensorChartMode
 */
#include "engine_state.h"

#if EFI_UNIT_TEST
extern bool printTriggerDebug;
#endif

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

	// It's OK to truncate from 64b to 32b, ARM with single precision FPU uses an expensive
	// software function to convert 64b int -> float, while 32b int -> float is very cheap hardware conversion
	// The difference is guaranteed to be short (it's 90 degrees of engine rotation!), so it won't overflow.
	uint32_t nowNt32 = nowNt;

	assertIsInBoundsWithResult(current_index, timeOfLastEvent, "calc timeOfLastEvent", 0);

	// Save previous timestamp before overwriting - needed for single-tooth triggers
	// where prevIndex == current_index (see below)
	uint32_t previousTimeAtIndex = timeOfLastEvent[current_index];

	// Record the time of this event so we can calculate RPM from it later
	timeOfLastEvent[current_index] = nowNt32;

	// Determine where we currently are in the revolution
	angle_t currentAngle = triggerFormDetails->eventAngles[current_index];
	efiAssert(ObdCode::OBD_PCM_Processor_Fault, !std::isnan(currentAngle), "eventAngles", 0);

	// Hunt for a tooth ~90 degrees ago to compare to the current time
	angle_t previousAngle = currentAngle - 90;
	wrapAngle(previousAngle, "prevAngle", ObdCode::CUSTOM_ERR_TRIGGER_ANGLE_RANGE);
	int prevIndex = triggerShape.findAngleIndex(triggerFormDetails, previousAngle);

	// now let's get precise angle for that event
	angle_t prevIndexAngle = triggerFormDetails->eventAngles[prevIndex];
	auto time90ago = timeOfLastEvent[prevIndex];
	angle_t angleDiff = currentAngle - prevIndexAngle;

	// Wrap the angle in to the correct range (ie, could be -630 when we want +90)
	wrapAngle(angleDiff, "angleDiff", ObdCode::CUSTOM_ERR_6561);

	// For single-tooth triggers, all event angles map to the same value, so
	// findAngleIndex returns current_index. This causes two problems:
	// 1) time90ago was just overwritten with nowNt32, yielding time=0
	// 2) angleDiff is 0 since both angles are identical
	// Fix: use the saved previous timestamp and the full engine cycle as angle delta,
	// effectively measuring RPM from one revolution to the next.
	if (prevIndex == (int)current_index) {
		time90ago = previousTimeAtIndex;
		angleDiff = getEngineState()->engineCycle;
	}

	// No previous timestamp, instant RPM isn't ready yet
	if (time90ago == 0) {
		return prevInstantRpmValue;
	}

	uint32_t time = nowNt32 - time90ago;

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

	uint32_t nowNt32 = nowNt;
	spinningEvents[spinningEventIndex] = nowNt32;

	// If we are using only rising edges, we never write in to the odd-index slots that
	// would be used by falling edges
	// TODO: don't reach across to trigger central to get this info
	spinningEventIndex += getTriggerCentral()->triggerShape.useOnlyRisingEdges ? 2 : 1;
}

void InstantRpmCalculator::updateInstantRpm(
		uint32_t current_index,
		TriggerWaveform const & triggerShape, TriggerFormDetails *triggerFormDetails,
		uint32_t index, efitick_t nowNt) {
	UNUSED(current_index);

	m_instantRpm = calculateInstantRpm(triggerShape, triggerFormDetails, index,
					   nowNt);
#if EFI_UNIT_TEST
	if (printTriggerDebug) {
		printf("instantRpm = %f\n", m_instantRpm);
	}
#endif

}

#endif // EFI_SHAFT_POSITION_INPUT
