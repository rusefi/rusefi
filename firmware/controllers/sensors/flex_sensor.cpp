#include "digital_input_exti.h"
#include "flex_sensor.h"

static void flexExtiCallback(void* arg) {
	auto inst = reinterpret_cast<FlexFuelSensor*>(arg);
	inst->onEdge(getTimeNowNt());
}

void FlexFuelSensor::init(brain_pin_e pin) {
#if EFI_PROD_CODE
	// todo: refactor https://github.com/rusefi/rusefi/issues/2123
	efiExtiEnablePin("flex", pin, 
		PAL_EVENT_MODE_FALLING_EDGE,
		flexExtiCallback, reinterpret_cast<void*>(this));
#endif // EFI_PROD_CODE

	// Update rate is 50-150hz, so this actually filters at 0.5-1.5hz -3db depending on E%, which is ok
	m_filter.configureLowpass(100, 1);
}

FlexFuelSensor::FlexFuelSensor()
	// this long timeout is ok, ethanol content isn't going to change quickly
	: StoredValueSensor(SensorType::FuelEthanolPercent, MS2NT(500))
{
}

void FlexFuelSensor::onEdge(efitick_t nowNt) {
	float frequency = 1 / m_edgeTimer.getElapsedSecondsAndReset(nowNt);

	// Sensor should only report 50-150hz, significantly outside that range indicates a problem
	// it changes to 200hz+ to indicate methanol "contamination"
	if (frequency > 45 && frequency < 155) {
		float flexPct = clampF(0, frequency - 50, 100);

		setValidValue(m_filter.filter(flexPct), nowNt);
	}

	// no explicit invalidation, we can tolerate a somewhat dodgy connection
	// so long as we get a valid interval every now and then
}
