#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "function_pointer_sensor.h"
#include "live_data.h"

#if EFI_CAN_SUPPORT
#include "AemXSeriesLambda.h"
#else
#include "wideband_state_generated.h"
#endif


namespace {
	FunctionPointerSensor lambdaSensor1(SensorType::Lambda1,
	                                    []() {
		                                    return Ego::getAfr(SensorType::Lambda1) / STOICH_RATIO;
	                                    });

	FunctionPointerSensor lambdaSensor2(SensorType::Lambda2,
	                                    []() {
		                                    return Ego::getAfr(SensorType::Lambda2) / STOICH_RATIO;
	                                    });

#if EFI_CAN_SUPPORT
	AemXSeriesWideband canWidebandController1(0, SensorType::Lambda1);
	AemXSeriesWideband canWidebandController2(1, SensorType::Lambda2);

	void initCanLambda() {
		if (!engineConfiguration->canWriteEnabled || !engineConfiguration->canReadEnabled) {
			criticalError("CAN read and write are required to use CAN wideband.");
			return;
		}

		registerCanSensor(canWidebandController1);
		registerCanSensor(canWidebandController2);
	}
#endif

	void initAnalogLambda() {
#if EFI_UNIT_TEST
	constexpr bool isUnitTest = true;
#else
		constexpr bool isUnitTest = false;
#endif

		if (isAdcChannelValid(engineConfiguration->afr.hwChannel) || isUnitTest) {
			lambdaSensor1.Register();
		}

		if (isAdcChannelValid(engineConfiguration->afr.hwChannel2) || isUnitTest) {
			lambdaSensor2.Register();
		}

		Ego::registerStoredValueSensors();
	}
}

template <>
const wideband_state_s* getLiveData(size_t idx) {
#if EFI_CAN_SUPPORT
	switch (idx) {
		case 0:
			aem1.refreshState();
			return &aem1;
		case 1:
			aem2.refreshState();
			return &aem2;
		default:
			return nullptr;
	}
#else
	return nullptr;
#endif
}

void initLambda() {
#if EFI_CAN_SUPPORT
	if (engineConfiguration->enableAemXSeries) {
		initCanLambda();
		return;
	}
#endif

	// CAN bus option is handled above, let's handle analog inputs conditionally to give Lua sensors a chance
	initAnalogLambda();
}
