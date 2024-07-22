/**
 * @file	speed_density.cpp
 *
 * See http://rusefi.com/wiki/index.php?title=Manual:Software:Fuel_Control#Speed_Density for details
 *
 * @date May 29, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include <rusefi/interpolation.h>
#include "engine_configuration.h"
#include "error_handling.h"
#include "fuel_computer.h"
#include "speed_density.h"
#include "fuel_math.h"
#include "sensor.h"
#include "efi_interpolation.h"
#include "table_helper.h"
#include "engine_math.h"

#if defined(HAS_OS_ACCESS)
#error "Unexpected OS ACCESS HERE"
#endif

#define rpmMin 500
#define rpmMax 8000

fuel_Map3D_t veMap;

#define tpMin 0
#define tpMax 100

float IFuelComputer::getTChargeCoefficient(int rpm, float tps) {
	// First, do TPS mode since it doesn't need any of the airflow math.
	if (engineConfiguration->tChargeMode == TCHARGE_MODE_RPM_TPS) {
		float minRpmKcurrentTPS = interpolateMsg("minRpm", tpMin,
				engineConfiguration->tChargeMinRpmMinTps, tpMax,
				engineConfiguration->tChargeMinRpmMaxTps, tps);
		float maxRpmKcurrentTPS = interpolateMsg("maxRpm", tpMin,
				engineConfiguration->tChargeMaxRpmMinTps, tpMax,
				engineConfiguration->tChargeMaxRpmMaxTps, tps);

		return interpolateMsg("Kcurr", rpmMin, minRpmKcurrentTPS, rpmMax, maxRpmKcurrentTPS, rpm);
	}

	constexpr floatms_t gramsPerMsToKgPerHour = (3600.0f * 1000.0f) / 1000.0f;
	// We're actually using an 'old' airMass calculated for the previous cycle, but it's ok, we're not having any self-excitaton issues
	floatms_t airMassForEngine = sdAirMassInOneCylinder * engineConfiguration->cylindersCount;
	// airMass is in grams per 1 cycle for 1 cyl. Convert it to airFlow in kg/h for the engine.
	// And if the engine is stopped (0 rpm), then airFlow is also zero (avoiding NaN division)
	floatms_t airFlow = (rpm == 0) ? 0 : airMassForEngine * gramsPerMsToKgPerHour / getEngineCycleDuration(rpm);

	if (engineConfiguration->tChargeMode == TCHARGE_MODE_AIR_INTERP) {
		// just interpolate between user-specified min and max coefs, based on the max airFlow value
		return interpolateClamped(
			0.0, engineConfiguration->tChargeAirCoefMin,
			engineConfiguration->tChargeAirFlowMax, engineConfiguration->tChargeAirCoefMax,
			airFlow
		);
	} else if (engineConfiguration->tChargeMode == TCHARGE_MODE_AIR_INTERP_TABLE) {
		return interpolate2d(
			airFlow,
			engineConfiguration->tchargeBins,
			engineConfiguration->tchargeValues
		);
	} else {
		firmwareError(ObdCode::OBD_PCM_Processor_Fault, "Unexpected tChargeMode: %d", engineConfiguration->tChargeMode);
		return 0;
	}
}

//  http://rusefi.com/math/t_charge.html
/***panel:Charge Temperature*/
temperature_t IFuelComputer::getTCharge(int rpm, float tps) {
	const auto clt = Sensor::get(SensorType::Clt);
	const auto iat = Sensor::get(SensorType::Iat);

	float airTemp;

	// Without either valid, return 0C.  It's wrong, but it'll pretend to be nice and dense, so at least you won't go lean.
	if (!iat && !clt) {
		return 0;
	} else if (!clt && iat) {
		// Intake temperature will almost always be colder (richer) than CLT - use that
		return iat.Value;
	} else if (!iat && clt) {
		// Without valid intake temperature, assume intake temp is 0C, and interpolate anyway
		airTemp = 0;
	} else {
		// All is well - use real air temp
		airTemp = iat.Value;
	}

	float coolantTemp = clt.Value;

	sdTcharge_coff = getTChargeCoefficient(rpm, tps);

	if (std::isnan(sdTcharge_coff)) {
		warning(ObdCode::CUSTOM_ERR_T2_CHARGE, "t2-getTCharge NaN");
		return coolantTemp;
	}

	// Interpolate between CLT and IAT:
	// 0.0 coefficient -> use CLT (full heat transfer)
	// 1.0 coefficient -> use IAT (no heat transfer)
	float Tcharge = interpolateClamped(0.0f, coolantTemp, 1.0f, airTemp, sdTcharge_coff);

	if (std::isnan(Tcharge)) {
		// we can probably end up here while resetting engine state - interpolation would fail
		warning(ObdCode::CUSTOM_ERR_TCHARGE_NOT_READY, "getTCharge NaN");
		return coolantTemp;
	}

	return Tcharge;
}

void initSpeedDensity() {
	veMap.init(config->veTable, config->veLoadBins, config->veRpmBins);
}
