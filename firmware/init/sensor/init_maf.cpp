#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "functional_sensor.h"
#include "table_func.h"

static FunctionalSensor maf(SensorType::Maf, /* timeout = */ MS2NT(50));

#if !EFI_UNIT_TEST
// extract the type of the elements in the bin/value arrays
using BinType = std::remove_extent_t<decltype(config->mafDecodingBins)>;
using ValueType = std::remove_extent_t<decltype(config->mafDecoding)>;

// This function converts volts -> kg/h
static TableFunc
	<BinType, ValueType, MAF_DECODING_COUNT>
	mafCurve(config->mafDecodingBins, config->mafDecoding);

void initMaf(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	adc_channel_e channel = CONFIG(mafAdcChannel);

	if (!isAdcChannelValid(channel)) {
		return;
	}

	maf.setFunction(mafCurve);

	AdcSubscription::SubscribeSensor(maf, channel, /*lowpassCutoff =*/ 50);
	maf.Register();
}
#endif // ! EFI_UNIT_TEST
