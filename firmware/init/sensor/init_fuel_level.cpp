#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "functional_sensor.h"
#include "table_func.h"

static FunctionalSensor fuelSensor(SensorType::FuelLevel, /* timeout = */ MS2NT(500));

#if !EFI_UNIT_TEST
// extract the type of the elements in the bin/value arrays
using BinType = std::remove_extent_t<decltype(CONFIG(fuelLevelBins))>;
using ValueType = std::remove_extent_t<decltype(CONFIG(fuelLevelValues))>;

static TableFunc
	<BinType, ValueType, FUEL_LEVEL_TABLE_COUNT,
		// Bins are stored in millivolts
		efi::ratio<1, PACK_MULT_VOLTAGE>,
		// Values are stored in percent
		efi::ratio<1>>
			fuelCurve(CONFIG(fuelLevelBins), CONFIG(fuelLevelValues));

void initFuelLevel(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	adc_channel_e channel = CONFIG(fuelLevelSensor);

	if (!isAdcChannelValid(channel)) {
		return;
	}

	fuelSensor.setFunction(fuelCurve);

	// Filtering with such a small bandwidth helps prevent noisy data from fuel tank slosh
	AdcSubscription::SubscribeSensor(fuelSensor, channel, /*lowpassCutoff =*/ 0.05f);
	fuelSensor.Register();
}
#endif // ! EFI_UNIT_TEST
