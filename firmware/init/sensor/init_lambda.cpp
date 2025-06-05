#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "function_pointer_sensor.h"
#include "live_data.h"
#include "CanWideband.h"

namespace {
	struct GetAfrWrapper {
		float getLambda() {
			return Ego::getAfr(SensorType::Lambda1) / STOICH_RATIO;
		};

		float getLambda2() {
			return Ego::getAfr(SensorType::Lambda2) / STOICH_RATIO;
		}
	};

	GetAfrWrapper afrWrapper;

	FunctionPointerSensor lambdaSensor(SensorType::Lambda1,
	                                   []() {
		                                   return afrWrapper.getLambda();
	                                   });

	FunctionPointerSensor lambdaSensor2(SensorType::Lambda2,
	                                    []() {
		                                    return afrWrapper.getLambda2();
	                                    });

#if EFI_CAN_SUPPORT
	CanWideband canWideband1(0, SensorType::Lambda1);
	CanWideband canWideband2(1, SensorType::Lambda2);
#endif
}

template <>
const wideband_state_s* getLiveData(size_t idx) {
#if EFI_CAN_SUPPORT
	switch (idx) {
	case 0:
		canWideband1.refreshState();
		return &canWideband1;
	case 1:
		canWideband2.refreshState();
		return &canWideband2;
	default:
		return nullptr;
	}
#else
	return nullptr;
#endif
}

void initLambda() {
#if EFI_CAN_SUPPORT
	if (engineConfiguration->enableCanWideband) {
		if (!engineConfiguration->canWriteEnabled || !engineConfiguration->canReadEnabled) {
			criticalError("CAN read and write are required to use CAN wideband.");
			return;
		}

		registerCanSensor(canWideband1);
		registerCanSensor(canWideband2);

		return;
	}
#endif

#if EFI_UNIT_TEST
    constexpr bool isUnitTest = true;
#else
	constexpr bool isUnitTest = false;
#endif

	// CANbus option is handled above, let's handle analog inputs conditionally to give Lua sensors a chance
	if (isAdcChannelValid(engineConfiguration->afr.hwChannel) || isUnitTest) {
		lambdaSensor.Register();
	}

	if (isAdcChannelValid(engineConfiguration->afr.hwChannel2) || isUnitTest) {
		lambdaSensor2.Register();
	}

	Ego::registerStoredValueSensors();
}
