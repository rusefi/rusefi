/**
 * @file frequency_sensor.cpp
 */

#include "pch.h"

#include "frequency_sensor.h"

#include "digital_input_exti.h"

// Callback adapter since we can't pass a member function to a callback
static void freqSensorExtiCallback(void* arg) {
	auto inst = reinterpret_cast<FrequencySensor*>(arg);
	inst->onEdge(getTimeNowNt());
}

void FrequencySensor::init(brain_pin_e pin) {
#if EFI_PROD_CODE
	// todo: refactor https://github.com/rusefi/rusefi/issues/2123
	efiExtiEnablePin("flex", pin, 
		PAL_EVENT_MODE_FALLING_EDGE,
		freqSensorExtiCallback, reinterpret_cast<void*>(this));
#endif // EFI_PROD_CODE
}

void FrequencySensor::onEdge(efitick_t nowNt) {
	float frequency = 1 / m_edgeTimer.getElapsedSecondsAndReset(nowNt);

	postRawValue(frequency, nowNt);
}
