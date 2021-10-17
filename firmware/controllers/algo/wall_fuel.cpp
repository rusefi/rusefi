/*
 * @file wall_fuel.cpp
 *
 * @author Matthew Kennedy
 */

#include "pch.h"
#include "wall_fuel.h"

void WallFuel::resetWF() {
	wallFuel = 0;
}

floatms_t WallFuel::adjust(floatms_t desiredFuel DECLARE_ENGINE_PARAMETER_SUFFIX) {
	invocationCounter++;
	if (cisnan(desiredFuel)) {
		return desiredFuel;
	}
	// disable this correction for cranking
	if (ENGINE(rpmCalculator).isCranking()) {
		return desiredFuel;
	}

	ScopePerf perf(PE::WallFuelAdjust);

	/*
		this math is based on
				SAE 810494 by C. F. Aquino
				SAE 1999-01-0553 by Peter J Maloney

		M_cmd = commanded fuel mass (output of this function)
		desiredFuel = desired fuel mass (input to this function)
		fuelFilmMass = fuel film mass (how much is currently on the wall)

		First we compute how much fuel to command, by accounting for
		a) how much fuel will evaporate from the walls, entering the air
		b) how much fuel from the injector will hit the walls, being deposited

		Next, we compute how much fuel will be deposited on the walls.  The net
		effect of these two steps is computed (some leaves walls, some is deposited)
		and stored back in fuelFilmMass.

		alpha describes the amount of fuel that REMAINS on the wall per cycle.
		It is computed as a function of the evaporation time constant (tau) and
		the time the fuel spent on the wall this cycle, (recriprocal RPM).

		beta describes the amount of fuel that hits the wall.

		TODO: these parameters, tau and beta vary with various engine parameters,
		most notably manifold pressure (as a proxy for air speed), and coolant
		temperature (as a proxy for the intake valve and runner temperature).

		TAU: decreases with increasing temperature.
		     decreases with decreasing manifold pressure.

		BETA: decreases with increasing temperature.
		     decreases with decreasing manifold pressure.
	*/

	// if tau is really small, we get div/0.
	// you probably meant to disable wwae.
	float tau = CONFIG(wwaeTau);
	if (tau < 0.01f) {
		return desiredFuel;
	}

	// Ignore really slow RPM
	int rpm = GET_RPM();
	if (rpm < 100) {
		return desiredFuel;
	}

	float alpha = expf_taylor(-120 / (rpm * tau));
	float beta = CONFIG(wwaeBeta);

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_KNOCK) {
		tsOutputChannels.debugFloatField1 = alpha;
		tsOutputChannels.debugFloatField2 = beta;
	}
#endif // EFI_TUNER_STUDIO

	// If beta is larger than alpha, the system is underdamped.
	// For reasonable values {tau, beta}, this should only be possible
	// at extremely low engine speeds (<300rpm ish)
	// Clamp beta to less than alpha.
	if (beta > alpha) {
		beta = alpha;
	}

	float fuelFilmMass = wallFuel;
	float M_cmd = (desiredFuel - (1 - alpha) * fuelFilmMass) / (1 - beta);

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_KNOCK) {
		tsOutputChannels.debugFloatField3 = fuelFilmMass;
		tsOutputChannels.debugFloatField4 = M_cmd;
	}
#endif // EFI_TUNER_STUDIO

	// We can't inject a negative amount of fuel
	// If this goes below zero we will be over-fueling slightly,
	// but that's ok.
	if (M_cmd <= 0) {
		M_cmd = 0;
	}

	// remainder on walls from last time + new from this time
	float fuelFilmMassNext = alpha * fuelFilmMass + beta * M_cmd;

#if EFI_TUNER_STUDIO
	if (engineConfiguration->debugMode == DBG_KNOCK) {
		tsOutputChannels.debugFloatField5 = fuelFilmMassNext;
	}
#endif // EFI_TUNER_STUDIO

	wallFuel = fuelFilmMassNext;
	wallFuelCorrection = M_cmd - desiredFuel;
	return M_cmd;
}

floatms_t WallFuel::getWallFuel() const {
	return wallFuel;
}



