#include "engine.h"
#include "digital_input_exti.h"
#include "flex_sensor.h"

EXTERN_ENGINE;

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
		setValidValue(clampF(0, frequency - 50, 100), nowNt);
	}

	// no explicit invalidation, we can tolerate a somewhat dodgy connection
	// so long as we get a valid interval every now and then
}
