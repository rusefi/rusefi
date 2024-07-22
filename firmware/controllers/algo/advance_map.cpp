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

#include "pch.h"

#include "advance_map.h"
#include "idle_thread.h"
#include "launch_control.h"
#include "gppwm_channel.h"

#if EFI_ENGINE_CONTROL

// TODO: wow move this into engineState at least for context not to leak from test to test!
// todo: reset this between cranking attempts?! #2735
int minCrankingRpm = 0;

static Map3D<TRACTION_CONTROL_ETB_DROP_SIZE, TRACTION_CONTROL_ETB_DROP_SIZE, int8_t, uint16_t, uint8_t> tcTimingDropTable{"tct"};
static Map3D<TRACTION_CONTROL_ETB_DROP_SIZE, TRACTION_CONTROL_ETB_DROP_SIZE, int8_t, uint16_t, uint8_t> tcSparkSkipTable{"tcs"};

#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT

/**
 * @return ignition timing angle advance before TDC
 */
angle_t getRunningAdvance(int rpm, float engineLoad) {
	if (engineConfiguration->timingMode == TM_FIXED) {
		return engineConfiguration->fixedTiming;
	}

	if (std::isnan(engineLoad)) {
		warning(ObdCode::CUSTOM_NAN_ENGINE_LOAD, "NaN engine load");
		return NAN;
	}

	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, !std::isnan(engineLoad), "invalid el", NAN);

	// compute base ignition angle from main table
	float advanceAngle = interpolate3d(
		config->ignitionTable,
		config->ignitionLoadBins, engineLoad,
		config->ignitionRpmBins, rpm
	);

  float vehicleSpeed = Sensor::getOrZero(SensorType::VehicleSpeed);
  float wheelSlip = Sensor::getOrZero(SensorType::WheelSlipRatio);
  engine->ignitionState.tractionAdvanceDrop = tcTimingDropTable.getValue(wheelSlip, vehicleSpeed);
  engine->engineState.tractionControlSparkSkip = tcSparkSkipTable.getValue(wheelSlip, vehicleSpeed);
  engine->engineState.updateSparkSkip();

  advanceAngle += engine->ignitionState.tractionAdvanceDrop;

#if EFI_ANTILAG_SYSTEM
	if (engine->antilagController.isAntilagCondition) {
		float throttleIntent = Sensor::getOrZero(SensorType::DriverThrottleIntent);
		engine->antilagController.timingALSCorrection = interpolate3d(
			config->ALSTimingRetardTable,
			config->alsIgnRetardLoadBins, throttleIntent,
			config->alsIgnRetardrpmBins, rpm
		);
		advanceAngle += engine->antilagController.timingALSCorrection;
	}
#endif /* EFI_ANTILAG_SYSTEM */

	// Add any adjustments if configured
	for (size_t i = 0; i < efi::size(config->ignBlends); i++) {
		auto result = calculateBlend(config->ignBlends[i], rpm, engineLoad);

		engine->outputChannels.ignBlendParameter[i] = result.BlendParameter;
		engine->outputChannels.ignBlendBias[i] = result.Bias;
		engine->outputChannels.ignBlendOutput[i] = result.Value;

		advanceAngle += result.Value;
	}

	// get advance from the separate table for Idle
#if EFI_IDLE_CONTROL
	if (engineConfiguration->useSeparateAdvanceForIdle &&
		engine->module<IdleController>()->isIdlingOrTaper()) {
		float idleAdvance = interpolate2d(rpm, config->idleAdvanceBins, config->idleAdvance);

		auto tps = Sensor::get(SensorType::DriverThrottleIntent);
		if (tps) {
			// interpolate between idle table and normal (running) table using TPS threshold
			advanceAngle = interpolateClamped(0.0f, idleAdvance, engineConfiguration->idlePidDeactivationTpsThreshold, advanceAngle, tps.Value);
		}
	}
#endif

#if EFI_LAUNCH_CONTROL
	if (engineConfiguration->launchControlEnabled && engineConfiguration->enableLaunchRetard) {
		const float launchAngle = engineConfiguration->launchTimingRetard;
		if (engine->launchController.isPreLaunchCondition) {
			const int launchRpm = engineConfiguration->launchRpm;
			const int smoothRetardStartRpm = (launchRpm - engineConfiguration->launchRpmWindow);
			const int smoothRetardEndRpm = (launchRpm - engineConfiguration->launchCorrectionsEndRpm);
			if (smoothRetardStartRpm <= rpm) {
				if (engineConfiguration->launchSmoothRetard && (rpm <= smoothRetardEndRpm)) {
					// https://github.com/rusefi/rusefi/issues/5611#issuecomment-2130431696
					return interpolateClamped(smoothRetardStartRpm, advanceAngle, smoothRetardEndRpm, launchAngle, rpm);
				} else {
					return launchAngle;
				}
			}
		} else if (engine->launchController.isLaunchCondition) {
			return launchAngle;
		}
	}
#endif /* EFI_LAUNCH_CONTROL */

	return advanceAngle;
}

angle_t getAdvanceCorrections(float engineLoad) {
	auto iat = Sensor::get(SensorType::Iat);

	if (!iat) {
		engine->ignitionState.timingIatCorrection = 0;
	} else {
		engine->ignitionState.timingIatCorrection = interpolate3d(
			config->ignitionIatCorrTable,
			config->ignitionIatCorrLoadBins, engineLoad,
			config->ignitionIatCorrTempBins, iat.Value
		);
	}

#if EFI_IDLE_CONTROL
	float instantRpm = engine->triggerCentral.instantRpm.getInstantRpm();

	engine->ignitionState.timingPidCorrection = engine->module<IdleController>()->getIdleTimingAdjustment(instantRpm);
#endif // EFI_IDLE_CONTROL

#if EFI_TUNER_STUDIO
	engine->outputChannels.multiSparkCounter = engine->engineState.multispark.count;
#endif /* EFI_TUNER_STUDIO */

	return engine->ignitionState.timingIatCorrection
		+ engine->ignitionState.cltTimingCorrection
		+ engine->ignitionState.timingPidCorrection;
}

/**
 * @return ignition timing angle advance before TDC for Cranking
 */
angle_t getCrankingAdvance(int rpm, float engineLoad) {
	// get advance from the separate table for Cranking
	if (engineConfiguration->useSeparateAdvanceForCranking) {
		return interpolate2d(rpm, config->crankingAdvanceBins, config->crankingAdvance);
	}

	// Interpolate the cranking timing angle to the earlier running angle for faster engine start
	angle_t crankingToRunningTransitionAngle = getRunningAdvance(engineConfiguration->cranking.rpm, engineLoad);
	// interpolate not from zero, but starting from min. possible rpm detected
	if (rpm < minCrankingRpm || minCrankingRpm == 0)
		minCrankingRpm = rpm;
	return interpolateClamped(minCrankingRpm, engineConfiguration->crankingTimingAngle, engineConfiguration->cranking.rpm, crankingToRunningTransitionAngle, rpm);
}
#endif // EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT

angle_t getAdvance(int rpm, float engineLoad) {
#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT
	if (std::isnan(engineLoad)) {
		return 0; // any error should already be reported
	}

	angle_t angle;

	bool isCranking = engine->rpmCalculator.isCranking();
	if (isCranking) {
		angle = getCrankingAdvance(rpm, engineLoad);
		assertAngleRange(angle, "crAngle", ObdCode::CUSTOM_ERR_ANGLE_CR);
		efiAssert(ObdCode::CUSTOM_ERR_ASSERT, !std::isnan(angle), "cr_AngleN", 0);
	} else {
		angle = getRunningAdvance(rpm, engineLoad);

		if (std::isnan(angle)) {
			warning(ObdCode::CUSTOM_ERR_6610, "NaN angle from table");
			return 0;
		}
	}

	// Allow correction only if set to dynamic
	// AND we're either not cranking OR allowed to correct in cranking
	bool allowCorrections = engineConfiguration->timingMode == TM_DYNAMIC
		&& (!isCranking || engineConfiguration->useAdvanceCorrectionsForCranking);

	if (allowCorrections) {
		angle_t correction = getAdvanceCorrections(engineLoad);
		if (!std::isnan(correction)) { // correction could be NaN during settings update
			angle += correction;
		}
	}

	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, !std::isnan(angle), "_AngleN5", 0);
	wrapAngle(angle, "getAdvance", ObdCode::CUSTOM_ERR_ADCANCE_CALC_ANGLE);
	return angle;
#else
	return 0;
#endif
}

angle_t getCylinderIgnitionTrim(size_t cylinderNumber, int rpm, float ignitionLoad) {
	return interpolate3d(
		config->ignTrims[cylinderNumber].table,
		config->ignTrimLoadBins, ignitionLoad,
		config->ignTrimRpmBins, rpm
	);
}

size_t getMultiSparkCount(int rpm) {
	// Compute multispark (if enabled)
	if (engineConfiguration->multisparkEnable
		&& rpm <= engineConfiguration->multisparkMaxRpm
		&& engineConfiguration->multisparkMaxExtraSparkCount > 0) {
		// For zero RPM, disable multispark.  We don't yet know the engine speed, so multispark may not be safe.
		if (rpm == 0) {
			return 0;
		}

		floatus_t multiDelay = 1000.0f * engineConfiguration->multisparkSparkDuration;
		floatus_t multiDwell = 1000.0f * engineConfiguration->multisparkDwell;

		// dwell times are below 10 seconds here so we use 32 bit type for performance reasons
		engine->engineState.multispark.delay = (uint32_t)USF2NT(multiDelay);
		engine->engineState.multispark.dwell = (uint32_t)USF2NT(multiDwell);

		constexpr float usPerDegreeAt1Rpm = 60e6 / 360;
		floatus_t usPerDegree = usPerDegreeAt1Rpm / rpm;

		// How long is there for sparks? The user configured an angle, convert to time.
		floatus_t additionalSparksUs = usPerDegree * engineConfiguration->multisparkMaxSparkingAngle;
		// How long does one spark take?
		floatus_t oneSparkTime = multiDelay + multiDwell;

		// How many sparks can we fit in the alloted time?
		float sparksFitInTime = additionalSparksUs / oneSparkTime;

		// Take the floor (convert to uint8_t) - we want to undershoot, not overshoot
		uint32_t floored = sparksFitInTime;

		// Allow no more than the maximum number of extra sparks
		return minI(floored, engineConfiguration->multisparkMaxExtraSparkCount);
	} else {
		return 0;
	}
}

void initIgnitionAdvanceControl() {
	tcTimingDropTable.initTable(engineConfiguration->tractionControlTimingDrop, engineConfiguration->tractionControlSlipBins, engineConfiguration->tractionControlSpeedBins);
	tcSparkSkipTable.initTable(engineConfiguration->tractionControlIgnitionSkip, engineConfiguration->tractionControlSlipBins, engineConfiguration->tractionControlSpeedBins);
}

#endif // EFI_ENGINE_CONTROL
