#include "pch.h"

#include "init.h"
#include "functional_sensor.h"
#include "flex_sensor.h"

#include "digital_input_exti.h"

// 30 sec timeout to avoid issues around cranking
static FlexSensor flexSensor(MS2NT(30000));

#if EFI_PROD_CODE
static Gpio flexPin = Gpio::Unassigned;

static void flexExtiCallback(void*, efitick_t nowNt) {
	flexSensor.callback(nowNt, efiReadPin(flexPin) ^ engineConfiguration->flexSensorInverted);
}
#endif

// https://rusefi.com/forum/viewtopic.php?p=37452#p37452

void initFlexSensor(bool isFirstTime) {
#if EFI_PROD_CODE
	if (efiExtiEnablePin("flex", engineConfiguration->flexSensorPin,
		PAL_EVENT_MODE_BOTH_EDGES, flexExtiCallback, nullptr) < 0) {
		return;
	}
	flexPin = engineConfiguration->flexSensorPin;

	if (isFirstTime) {
		addConsoleAction("flexinfo", []() {
			flexSensor.debug();
		});
	}
#endif // EFI_PROD_CODE

	// If an analog fuel temp sensor is configured, don't use the flex sensor for fuel temp
	flexSensor.Register(!isAdcChannelValid(engineConfiguration->fuelTempSensor.adcChannel));
}

void deInitFlexSensor() {
	flexSensor.unregister();

#if EFI_PROD_CODE
	if (!isBrainPinValid(flexPin)) {
		return;
	}

	efiExtiDisablePin(flexPin);

	flexPin = Gpio::Unassigned;
#endif
}
