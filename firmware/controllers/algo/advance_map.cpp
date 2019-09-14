/**
 * @file	advance_map.cpp
 *
 * @date Mar 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2019
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
#include "tps.h"
#include "idle_thread.h"

EXTERN_ENGINE
;

#if EFI_TUNER_STUDIO
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */

static ign_Map3D_t advanceMap("advance");
static ign_tps_Map3D_t advanceTpsMap("advanceTps");
static ign_Map3D_t iatAdvanceCorrectionMap("iat corr");

// Init PID later (make it compatible with unit-tests)
static Pid idleTimingPid;
static bool shouldResetTimingPid = false;

static int minCrankingRpm = 0;

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
static const float iatTimingRpmBins[IGN_LOAD_COUNT] = {880,	1260,	1640,	2020,	2400,	2780,	3000,	3380,	3760,	4140,	4520,	5000,	5700,	6500,	7200,	8000};

//880	1260	1640	2020	2400	2780	3000	3380	3760	4140	4520	5000	5700	6500	7200	8000
static const ignition_table_t defaultIatTiming = {
		{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2},
		{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2},
		{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2},
		{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2},
		{3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 3.5, 2, 2, 2, 2, 2},
		{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2},
		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{ 0, 0, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9, -0.9},
		{ -3.3, -3.4, -4.9, -4.9, -4.9, -4.9, -4.4, -4.4, -4.4, -4.4, -4.4, -0.9, -0.9, -0.9, -0.9, -0.9},
		{ -4.4, -4.9, -5.9, -5.9, -5.9, -5.9, -4.9, -4.9, -4.9, -4.9, -4.9, -2.4, -2.4, -2.4, -2.4, -2.4},
		{ -4.4, -4.9, -5.9, -5.9, -5.9, -5.9, -4.9, -4.9, -4.9, -4.9, -4.9, -2.9, -2.9, -2.9, -2.9, -2.9},
		{-4.4, -4.9, -5.9, -5.9, -5.9, -5.9, -4.9, -4.9, -4.9, -4.9, -4.9, -3.9, -3.9, -3.9, -3.9, -3.9},
		{-4.4, -4.9, -5.9, -5.9, -5.9, -5.9, -4.9, -4.9, -4.9, -4.9, -4.9, -3.9, -3.9, -3.9, -3.9, -3.9},
		{-4.4, -4.9, -5.9, -5.9, -5.9, -5.9, -4.9, -4.9, -4.9, -4.9, -4.9, -3.9, -3.9, -3.9, -3.9, -3.9},
};

#endif /* IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT */

bool isStep1Condition(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	return  CONFIGB(enabledStep1Limiter) && rpm >= engineConfiguration->step1rpm;
}

/**
 * @return ignition timing angle advance before TDC
 */
static angle_t getRunningAdvance(int rpm, float engineLoad DECLARE_ENGINE_PARAMETER_SUFFIX) {
	if (CONFIG(timingMode) == TM_FIXED)
		return engineConfiguration->fixedTiming;

	engine->m.beforeAdvance = getTimeNowLowerNt();
	if (cisnan(engineLoad)) {
		warning(CUSTOM_NAN_ENGINE_LOAD, "NaN engine load");
		return NAN;
	}
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(engineLoad), "invalid el", NAN);
	engine->m.beforeZeroTest = getTimeNowLowerNt();
	engine->m.zeroTestTime = getTimeNowLowerNt() - engine->m.beforeZeroTest;

	if (isStep1Condition(rpm PASS_ENGINE_PARAMETER_SUFFIX)) {
		return engineConfiguration->step1timing;
	}
	
	float advanceAngle;
	if (CONFIG(useTPSAdvanceTable)) {
		float tps = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
		advanceAngle = advanceTpsMap.getValue((float) rpm, tps);
	} else {
		advanceAngle = advanceMap.getValue((float) rpm, engineLoad);
	}
	
	// get advance from the separate table for Idle
	if (CONFIG(useSeparateAdvanceForIdle)) {
		float idleAdvance = interpolate2d("idleAdvance", rpm, config->idleAdvanceBins, config->idleAdvance);
		// interpolate between idle table and normal (running) table using TPS threshold
		float tps = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
		advanceAngle = interpolateClamped(0.0f, idleAdvance, CONFIGB(idlePidDeactivationTpsThreshold), advanceAngle, tps);
	}

	engine->m.advanceLookupTime = getTimeNowLowerNt() - engine->m.beforeAdvance;
	return advanceAngle;
}

angle_t getAdvanceCorrections(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX) {
	float iatCorrection;
	if (cisnan(engine->sensors.iat)) {
		iatCorrection = 0;
	} else {
		iatCorrection = iatAdvanceCorrectionMap.getValue((float) rpm, engine->sensors.iat);
	}
	// PID Ignition Advance angle correction
	float pidTimingCorrection = 0.0f;
	if (CONFIGB(useIdleTimingPidControl)) {
		int targetRpm = getTargetRpmForIdleCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);
		int rpmDelta = absI(rpm - targetRpm);
		float tps = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
		if (tps >= CONFIGB(idlePidDeactivationTpsThreshold)) {
			// we are not in the idle mode anymore, so the 'reset' flag will help us when we return to the idle.
			shouldResetTimingPid = true;
		} 
		else if (rpmDelta > CONFIG(idleTimingPidDeadZone) && rpmDelta < CONFIG(idleTimingPidWorkZone) + CONFIG(idlePidFalloffDeltaRpm)) {
			// We're now in the idle mode, and RPM is inside the Timing-PID regulator work zone!
			// So if we need to reset the PID, let's do it now
			if (shouldResetTimingPid) {
				idleTimingPid.reset();
				shouldResetTimingPid = false;
			}
			// get PID value (this is not an actual Advance Angle, but just a additive correction!)
			percent_t timingRawCorr = idleTimingPid.getOutput(targetRpm, rpm,
					/* is this the right dTime? this period is not exactly the period at which this code is invoked*/engineConfiguration->idleTimingPid.periodMs);
			// tps idle-running falloff
			pidTimingCorrection = interpolateClamped(0.0f, timingRawCorr, CONFIGB(idlePidDeactivationTpsThreshold), 0.0f, tps);
			// rpm falloff
			pidTimingCorrection = interpolateClamped(0.0f, pidTimingCorrection, CONFIG(idlePidFalloffDeltaRpm), 0.0f, rpmDelta - CONFIG(idleTimingPidWorkZone));
		} else {
			shouldResetTimingPid = true;
		}
	} else {
		shouldResetTimingPid = true;
	}

	if (engineConfiguration->debugMode == DBG_IGNITION_TIMING) {
#if EFI_TUNER_STUDIO
		tsOutputChannels.debugFloatField1 = iatCorrection;
		tsOutputChannels.debugFloatField2 = engine->engineState.cltTimingCorrection;
		tsOutputChannels.debugFloatField3 = engine->fsioState.fsioTimingAdjustment;
		tsOutputChannels.debugFloatField4 = pidTimingCorrection;
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
		return interpolate2d("crankingAdvance", rpm, CONFIG(crankingAdvanceBins), CONFIG(crankingAdvance));
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
	if (ENGINE(rpmCalculator).isCranking(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		angle = getCrankingAdvance(rpm, engineLoad PASS_ENGINE_PARAMETER_SUFFIX);
		assertAngleRange(angle, "crAngle", CUSTOM_ERR_6680);
		efiAssert(CUSTOM_ERR_ASSERT, !cisnan(angle), "cr_AngleN", 0);
		if (CONFIG(useAdvanceCorrectionsForCranking)) {
			angle_t correction = getAdvanceCorrections(rpm PASS_ENGINE_PARAMETER_SUFFIX);
			if (!cisnan(correction)) { // correction could be NaN during settings update
				angle += correction;
			}
		}
		efiAssert(CUSTOM_ERR_ASSERT, !cisnan(angle), "cr_AngleN2", 0);
	} else {
		angle = getRunningAdvance(rpm, engineLoad PASS_ENGINE_PARAMETER_SUFFIX);
		if (cisnan(angle)) {
			warning(CUSTOM_ERR_6610, "NaN angle from table");
			return 0;
		}
		angle_t correction = getAdvanceCorrections(rpm PASS_ENGINE_PARAMETER_SUFFIX);
		if (!cisnan(correction)) { // correction could be NaN during settings update
			angle += correction;
		}
		efiAssert(CUSTOM_ERR_ASSERT, !cisnan(angle), "AngleN3", 0);
	}
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(angle), "_AngleN4", 0);
	angle -= engineConfiguration->ignitionOffset;
	efiAssert(CUSTOM_ERR_ASSERT, !cisnan(angle), "_AngleN5", 0);
	fixAngle(angle, "getAdvance", CUSTOM_ERR_ADCANCE_CALC_ANGLE);
	return angle;
#else
	return 0;
#endif
}

void setDefaultIatTimingCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	setLinearCurve(config->ignitionIatCorrLoadBins, IGN_LOAD_COUNT, /*from*/CLT_CURVE_RANGE_FROM, 110, 1);
#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
	memcpy(config->ignitionIatCorrRpmBins, iatTimingRpmBins, sizeof(iatTimingRpmBins));
	copyTimingTable(defaultIatTiming, config->ignitionIatCorrTable);
#else
	setLinearCurve(config->ignitionIatCorrLoadBins, IGN_RPM_COUNT, /*from*/0, 6000, 1);
#endif /* IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT */
}

void initTimingMap(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// We init both tables in RAM because here we're at a very early stage, with no config settings loaded.
	advanceMap.init(config->ignitionTable, config->ignitionLoadBins,
			config->ignitionRpmBins);
	advanceTpsMap.init(CONFIG(ignitionTpsTable), CONFIG(ignitionTpsBins),
			config->ignitionRpmBins);
	iatAdvanceCorrectionMap.init(config->ignitionIatCorrTable, config->ignitionIatCorrLoadBins,
			config->ignitionIatCorrRpmBins);
	// init timing PID
	idleTimingPid = Pid(&CONFIG(idleTimingPid));
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
	if (engineConfiguration->fuelAlgorithm != LM_SPEED_DENSITY &&
			engineConfiguration->fuelAlgorithm != LM_MAP) {
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

