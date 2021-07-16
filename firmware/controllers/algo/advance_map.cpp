/**
 * @file	advance_map.cpp
 *
 * @date Mar 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "global.h"
#include "engine_configuration.h"
#include "engine.h"
#include "advance_map.h"
#include "interpolation.h"
#include "engine_math.h"
#include "sensor.h"
#include "idle_thread.h"
#include "allsensors.h"
#include "launch_control.h"
#include "tunerstudio_outputs.h"

#if EFI_ENGINE_CONTROL

static ign_Map3D_t advanceMap;
static ign_Map3D_t iatAdvanceCorrectionMap;

// todo: reset this between cranking attempts?! #2735
int minCrankingRpm = 0;

/**
 * @return ignition timing angle advance before TDC
 */
static angle_t getRunningAdvance(int rpm, float engineLoad DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (CONFIG(timingMode) == TM_FIXED) {
		return engineConfiguration->fixedTiming;
	}

	if (cisnan(engineLoad)) {
		warning(CUSTOM_NAN_ENGINE_LOAD, "NaN engine load");
		return NAN;
	}

	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(engineLoad), "invalid el", NAN);

	float advanceAngle = advanceMap.getValue((float) rpm, engineLoad);

	// get advance from the separate table for Idle
	if (CONFIG(useSeparateAdvanceForIdle) && isIdling()) {
		float idleAdvance = interpolate2d(rpm, config->idleAdvanceBins, config->idleAdvance);

		auto [valid, tps] = Sensor::get(SensorType::DriverThrottleIntent);
		if (valid) {
			// interpolate between idle table and normal (running) table using TPS threshold
			advanceAngle = interpolateClamped(0.0f, idleAdvance, CONFIG(idlePidDeactivationTpsThreshold), advanceAngle, tps);
		}
	}

#if EFI_LAUNCH_CONTROL
	if (engine->isLaunchCondition && CONFIG(enableLaunchRetard)) {
        if (CONFIG(launchSmoothRetard)) {
       	    float launchAngle = CONFIG(launchTimingRetard);
	        int launchAdvanceRpmRange = CONFIG(launchTimingRpmRange);
	        int launchRpm = CONFIG(launchRpm);
			 // interpolate timing from rpm at launch triggered to full retard at launch launchRpm + launchTimingRpmRange
			return interpolateClamped(launchRpm, advanceAngle, (launchRpm + launchAdvanceRpmRange), launchAngle, rpm);
		} else {
			return engineConfiguration->launchTimingRetard;
        }
	}
#endif /* EFI_LAUNCH_CONTROL */

	return advanceAngle;
}

angle_t getAdvanceCorrections(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	float iatCorrection;

	const auto [iatValid, iat] = Sensor::get(SensorType::Iat);

	if (!iatValid) {
		iatCorrection = 0;
	} else {
		iatCorrection = iatAdvanceCorrectionMap.getValue(rpm, iat);
	}

	float pidTimingCorrection = getIdleTimingAdjustment(rpm);

	if (engineConfiguration->debugMode == DBG_IGNITION_TIMING) {
#if EFI_TUNER_STUDIO
		tsOutputChannels.debugFloatField1 = iatCorrection;
		tsOutputChannels.debugFloatField2 = engine->engineState.cltTimingCorrection;
		tsOutputChannels.debugFloatField3 = engine->fsioState.fsioTimingAdjustment;
		tsOutputChannels.debugFloatField4 = pidTimingCorrection;
		tsOutputChannels.debugIntField1 = engine->engineState.multispark.count;
#endif /* EFI_TUNER_STUDIO */
	}

	return iatCorrection
		+ engine->fsioState.fsioTimingAdjustment
		+ engine->engineState.cltTimingCorrection
		+ pidTimingCorrection
		// todo: uncomment once we get usable knock   - engine->knockCount
		;
}

/**
 * @return ignition timing angle advance before TDC for Cranking
 */
static angle_t getCrankingAdvance(int rpm, float engineLoad DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// get advance from the separate table for Cranking
	if (CONFIG(useSeparateAdvanceForCranking)) {
		return interpolate2d(rpm, CONFIG(crankingAdvanceBins), CONFIG(crankingAdvance));
	}

	// Interpolate the cranking timing angle to the earlier running angle for faster engine start
	angle_t crankingToRunningTransitionAngle = getRunningAdvance(CONFIG(cranking.rpm), engineLoad PASS_ENGINE_PARAMETER_SUFFIX);
	// interpolate not from zero, but starting from min. possible rpm detected
	if (rpm < minCrankingRpm || minCrankingRpm == 0)
		minCrankingRpm = rpm;
	return interpolateClamped(minCrankingRpm, CONFIG(crankingTimingAngle), CONFIG(cranking.rpm), crankingToRunningTransitionAngle, rpm);
}


angle_t getAdvance(int rpm, float engineLoad DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	if (cisnan(engineLoad)) {
		return 0; // any error should already be reported
	}

	angle_t angle;

	bool isCranking = ENGINE(rpmCalculator).isCranking();
	if (isCranking) {
		angle = getCrankingAdvance(rpm, engineLoad PASS_ENGINE_PARAMETER_SUFFIX);
		assertAngleRange(angle, "crAngle", CUSTOM_ERR_6680);
		efiAssert(CUSTOM_ERR_ASSERT, !cisnan(angle), "cr_AngleN", 0);
	} else {
		angle = getRunningAdvance(rpm, engineLoad PASS_ENGINE_PARAMETER_SUFFIX);

		if (cisnan(angle)) {
			warning(CUSTOM_ERR_6610, "NaN angle from table");
			return 0;
		}
	}

	// Allow correction only if set to dynamic
	// AND we're either not cranking OR allowed to correct in cranking
	bool allowCorrections = CONFIG(timingMode) == TM_DYNAMIC
		&& (!isCranking || CONFIG(useAdvanceCorrectionsForCranking));

	if (allowCorrections) {
		angle_t correction = getAdvanceCorrections(rpm PASS_ENGINE_PARAMETER_SUFFIX);
		if (!cisnan(correction)) { // correction could be NaN during settings update
			angle += correction;
		}
	}

	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(angle), "_AngleN5", 0);
	fixAngle(angle, "getAdvance", CUSTOM_ERR_ADCANCE_CALC_ANGLE);
	return angle;
#else
	return 0;
#endif
}

size_t getMultiSparkCount(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// Compute multispark (if enabled)
	if (CONFIG(multisparkEnable)
		&& rpm <= CONFIG(multisparkMaxRpm)
		&& CONFIG(multisparkMaxExtraSparkCount) > 0) {
		// For zero RPM, disable multispark.  We don't yet know the engine speed, so multispark may not be safe.
		if (rpm == 0) {
			return 0;
		}

		floatus_t multiDelay = CONFIG(multisparkSparkDuration);
		floatus_t multiDwell = CONFIG(multisparkDwell);

        // dwell times are below 10 seconds here so we use 32 bit type for performance reasons
		ENGINE(engineState.multispark.delay) = (uint32_t)USF2NT(multiDelay);
		ENGINE(engineState.multispark.dwell) = (uint32_t)USF2NT(multiDwell);

		constexpr float usPerDegreeAt1Rpm = 60e6 / 360;
		floatus_t usPerDegree = usPerDegreeAt1Rpm / rpm;

		// How long is there for sparks? The user configured an angle, convert to time.
		floatus_t additionalSparksUs = usPerDegree * CONFIG(multisparkMaxSparkingAngle);
		// How long does one spark take?
		floatus_t oneSparkTime = multiDelay + multiDwell;

		// How many sparks can we fit in the alloted time?
		float sparksFitInTime = additionalSparksUs / oneSparkTime;

		// Take the floor (convert to uint8_t) - we want to undershoot, not overshoot
		uint32_t floored = sparksFitInTime;

		// Allow no more than the maximum number of extra sparks
		return minI(floored, CONFIG(multisparkMaxExtraSparkCount));
	} else {
		return 0;
	}
}

void initTimingMap(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// We init both tables in RAM because here we're at a very early stage, with no config settings loaded.
	advanceMap.init(config->ignitionTable, config->ignitionLoadBins,
			config->ignitionRpmBins);
	iatAdvanceCorrectionMap.init(config->ignitionIatCorrTable, config->ignitionIatCorrLoadBins,
			config->ignitionIatCorrRpmBins);
}

/**
 * @param octane gas octane number
 * @param bore in mm
 */
float getTopAdvanceForBore(chamber_style_e style, int octane, double compression, double bore) {
    int octaneCorrection;
    if ( octane <= 90) {
        octaneCorrection = -2;
    } else if (octane < 94) {
        octaneCorrection = -1;
    } else {
        octaneCorrection = 0;
    }

    int compressionCorrection;
    if (compression <= 9) {
        compressionCorrection = 2;
    } else if (compression <= 10) {
        compressionCorrection = 1;
    } else if (compression <= 11) {
        compressionCorrection = 0;
    } else {
        // compression ratio above 11
        compressionCorrection = -2;
    }
    int base;
    if (style == CS_OPEN) {
    	base = 33;
    } else if (style == CS_CLOSED) {
    	base = 28;
    } else {
    	// CS_SWIRL_TUMBLE
    	base = 22;
    }

    float boreCorrection = (bore - 4 * 25.4) / 25.4 * 6;
    float result = base + octaneCorrection + compressionCorrection + boreCorrection;
    return ((int)(result * 10)) / 10.0;
}

float getAdvanceForRpm(int rpm, float advanceMax) {
        if (rpm >= 3000)
            return advanceMax;
        if (rpm < 600)
            return 10;
       return interpolateMsg("advance", 600, 10, 3000, advanceMax, rpm);
}

#define round10(x) efiRound(x, 0.1)

float getInitialAdvance(int rpm, float map, float advanceMax) {
	map = minF(map, 100);
	float advance = getAdvanceForRpm(rpm, advanceMax);

	if (rpm >= 3000)
		return round10(advance + 0.1 * (100 - map));
	return round10(advance + 0.1 * (100 - map) * rpm / 3000);
}

/**
 * this method builds a good-enough base timing advance map bases on a number of heuristics
 */
void buildTimingMap(float advanceMax DECLARE_CONFIG_PARAMETER_SUFFIX) {
	if (engineConfiguration->fuelAlgorithm != LM_SPEED_DENSITY) {
		warning(CUSTOM_WRONG_ALGORITHM, "wrong algorithm for MAP-based timing");
		return;
	}
	/**
	 * good enough (but do not trust us!) default timing map in case of MAP-based engine load
	 */
	for (int loadIndex = 0; loadIndex < IGN_LOAD_COUNT; loadIndex++) {
		float load = config->ignitionLoadBins[loadIndex];
		for (int rpmIndex = 0;rpmIndex<IGN_RPM_COUNT;rpmIndex++) {
			float rpm = config->ignitionRpmBins[rpmIndex];
			config->ignitionTable[loadIndex][rpmIndex] = getInitialAdvance(rpm, load, advanceMax);
		}
	}
}

#endif // EFI_ENGINE_CONTROL
