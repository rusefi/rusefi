
#include "engine_configuration.h"
#include "sensor.h"
#include "error_handling.h"

#include "maf_airmass.h"
#include "maf.h"
#include "fuel_math.h"

float MafAirmass::getMaf() const {
	auto maf = Sensor::get(SensorType::Maf);

	if (Sensor::hasSensor(SensorType::Maf2)) {
		auto maf2 = Sensor::get(SensorType::Maf2);

		if (maf && maf2) {
			// Both MAFs work, return the sum
			return maf.Value + maf2.Value;
		} else if (maf) {
			// MAF 1 works, but not MAF 2, so double the value from #1
			return 2 * maf.Value;
		} else if (maf2) {
			// MAF 2 works, but not MAF 1, so double the value from #2
			return 2 * maf2.Value;
		} else {
			// Both MAFs are broken, give up.
			return 0;
		}
	} else {
		return maf.value_or(0);
	}
}

AirmassResult MafAirmass::getAirmass(int rpm, bool postState) {
	float maf = getMaf();

	return getAirmassImpl(maf, rpm, postState);
}

/**
 * Function block now works to create a standardised load from the cylinder filling as well as tune fuel via VE table. 
 * @return total duration of fuel injection per engine cycle, in milliseconds
 */
AirmassResult MafAirmass::getAirmassImpl(float massAirFlow, int rpm, bool postState) const {
	// If the engine is stopped, MAF is meaningless
	if (rpm == 0) {
		return {};
	}

	// kg/hr -> g/s
	float gramPerSecond = massAirFlow * 1000 / 3600;

	// 1/min -> 1/s
	float revsPerSecond = rpm / 60.0f;
	mass_t airPerRevolution = gramPerSecond / revsPerSecond;

	// Now we have to divide among cylinders - on a 4 stroke, half of the cylinders happen every revolution
	// This math is floating point to work properly on engines with odd cylinder count
	float halfCylCount = engineConfiguration->cylindersCount / 2.0f;

	mass_t cylinderAirmass = airPerRevolution / halfCylCount;

	//Create % load for fuel table using relative naturally aspirated cylinder filling
	float airChargeLoad = 100 * cylinderAirmass / getStandardAirCharge();
	
	//Correct air mass by VE table
	mass_t correctedAirmass = cylinderAirmass * getVe(rpm, airChargeLoad, postState);

	return {
		correctedAirmass,
		airChargeLoad, // AFR/VE/ignition table Y axis
	};
}
