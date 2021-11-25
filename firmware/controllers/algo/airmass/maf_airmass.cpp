#include "pch.h"
#include "maf_airmass.h"
#include "maf.h"

AirmassResult MafAirmass::getAirmass(int rpm) {
	float maf = Sensor::getOrZero(SensorType::Maf);
	return getAirmassImpl(maf, rpm);
}

/**
 * Function block now works to create a standardised load from the cylinder filling as well as tune fuel via VE table. 
 * @return total duration of fuel injection per engine cycle, in milliseconds
 */
AirmassResult MafAirmass::getAirmassImpl(float massAirFlow, int rpm) const {
	// If the engine is stopped, MAF is meaningless
	if (rpm == 0) {
		return {};
	}

	// kg/hr -> g/s
	float gramPerSecond = massAirFlow * 1000 / 3600;

	// 1/min -> 1/s
	float revsPerSecond = rpm / 60.0f;
	float airPerRevolution = gramPerSecond / revsPerSecond;

	// Now we have to divide among cylinders - on a 4 stroke, half of the cylinders happen every rev
	// This math is floating point to work properly on engines with odd cyl count
	float halfCylCount = engineConfiguration->specs.cylindersCount / 2.0f;

	float cylinderAirmass = airPerRevolution / halfCylCount;

	//Create % load for fuel table using relative naturally aspiratedcylinder filling
	float airChargeLoad = 100 * cylinderAirmass / engine->standardAirCharge;
	
	//Correct air mass by VE table
	float correctedAirmass = cylinderAirmass * getVe(rpm, airChargeLoad);

	return {
		correctedAirmass,
		airChargeLoad, // AFR/VE/ignition table Y axis
	};
}
