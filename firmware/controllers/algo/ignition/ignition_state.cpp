/**
 * @file ignition_state.cpp
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

#include "idle_thread.h"
#include "launch_control.h"
#include "gppwm_channel.h"

#if EFI_ENGINE_CONTROL

static Map3D<TRACTION_CONTROL_ETB_DROP_SLIP_SIZE, TRACTION_CONTROL_ETB_DROP_SPEED_SIZE, int8_t, uint16_t, uint8_t> tcTimingDropTable{"tct"};
static Map3D<TRACTION_CONTROL_ETB_DROP_SLIP_SIZE, TRACTION_CONTROL_ETB_DROP_SPEED_SIZE, int8_t, uint16_t, uint8_t> tcSparkSkipTable{"tcs"};

#if EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT

/**
 * @return ignition timing angle advance before TDC
 */
angle_t getRunningAdvance(float rpm, float engineLoad) {
	if (std::isnan(engineLoad)) {
		warning(ObdCode::CUSTOM_NAN_ENGINE_LOAD, "NaN engine load");
		return NAN;
	}

	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, !std::isnan(engineLoad), "invalid el", NAN);

	// compute base ignition angle from main table
	float advanceAngle = IgnitionState::getInterpolatedIgnitionAngle(rpm, engineLoad);

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
		engine->outputChannels.ignBlendYAxis[i] = result.TableYAxis;

		advanceAngle += result.Value;
	}

	// get advance from the separate table for Idle
#if EFI_IDLE_CONTROL
	if (engineConfiguration->useSeparateAdvanceForIdle &&
		(engine->module<IdleController>()->isIdlingOrTaper() || engine->module<IdleController>()->isCoastingAdvance())) {
		float idleAdvance = interpolate2d(rpm, config->idleAdvanceBins, config->idleAdvance);

		auto tps = Sensor::get(SensorType::DriverThrottleIntent);
		if (tps) {
			// interpolate between idle table and normal (running) table using TPS threshold
			// 0 TPS -> idle table
			// 1/2 threshold -> idle table
			// idle threshold -> normal table
			float idleThreshold = engineConfiguration->idlePidDeactivationTpsThreshold;
			advanceAngle = interpolateClamped(idleThreshold / 2, idleAdvance, idleThreshold, advanceAngle, tps.Value);
		}
	}
#endif

#if EFI_IDLE_CONTROL
	// reset ignition table dot, see #8198
	if(engineConfiguration->useSeparateAdvanceForIdle && engine->module<IdleController>()->isIdlingOrTaper()){
		engine->ignitionState.rpmForIgnitionIdleTableDot = rpm;
		engine->ignitionState.rpmForIgnitionTableDot = -1;
		engine->ignitionState.loadForIgnitionTableDot = -1;
	} else {
		engine->ignitionState.rpmForIgnitionIdleTableDot = -1;
		engine->ignitionState.rpmForIgnitionTableDot = rpm;
		engine->ignitionState.loadForIgnitionTableDot = engineLoad;
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
	if (engineConfiguration->torqueReductionEnabled
		&& engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied
	) {
		return engine->shiftTorqueReductionController.getTorqueReductionIgnitionRetard();
	}
    if (engineConfiguration->nitrousControlEnabled && engine->module<NitrousController>()->isNitrousCondition) {
        advanceAngle -= engineConfiguration->nitrousIgnitionRetard;
    }
#endif /* EFI_LAUNCH_CONTROL */

#ifdef MODULE_VVL_CONTROLLER
	advanceAngle += engine->module<VvlController>().unmock().getTimingModifier();
#endif /* MODULE_VVL_CONTROLLER */

	return advanceAngle;
}

angle_t getCltTimingCorrection(float engineLoad) {
	const auto clt = Sensor::get(SensorType::Clt);

	if (!clt)
		return 0; // this error should be already reported somewhere else, let's just handle it

	return interpolate3d(
			config->ignitionCltCorrTable,
			config->ignitionCltCorrLoadBins, engineLoad,
			config->ignitionCltCorrTempBins, clt.Value
	);
}

void IgnitionState::updateAdvanceCorrections(float engineLoad) {
	cltTimingCorrection = getCltTimingCorrection(engineLoad);
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

	engine->ignitionState.dfcoTimingRetard = engine->module<DfcoController>()->getTimingRetard();

#if EFI_TUNER_STUDIO
	engine->outputChannels.multiSparkCounter = engine->engineState.multispark.count;
#endif /* EFI_TUNER_STUDIO */

	return engine->ignitionState.timingIatCorrection
		+ engine->ignitionState.cltTimingCorrection
		+ engine->ignitionState.timingPidCorrection
		- engine->ignitionState.dfcoTimingRetard;
}

/**
 * @return ignition timing angle advance before TDC for Cranking
 */
angle_t getCrankingAdvance(float rpm, float engineLoad) {
	// get advance from the separate table for Cranking
	if (engineConfiguration->useSeparateAdvanceForCranking) {
		return interpolate2d(rpm, config->crankingAdvanceBins, config->crankingAdvance);
	}

	// Interpolate the cranking timing angle to the earlier running angle for faster engine start
	angle_t crankingToRunningTransitionAngle = getRunningAdvance(engineConfiguration->cranking.rpm, engineLoad);
	// interpolate not from zero, but starting from min. possible rpm detected
	return interpolateClamped(engine->rpmCalculator.getMinCrankingRpm(), engineConfiguration->crankingTimingAngle, engineConfiguration->cranking.rpm, crankingToRunningTransitionAngle, rpm);
}
#endif // EFI_ENGINE_CONTROL && EFI_SHAFT_POSITION_INPUT

angle_t IgnitionState::getAdvance(float rpm, float engineLoad) {
	if (std::isnan(engineLoad)) {
		return 0; // any error should already be reported
	}
  if (engineConfiguration->timingMode == TM_FIXED) {
		// fixed timing is the simple: cranking/running does not matter, no corrections!
		return engineConfiguration->fixedTiming;
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

	// Allow if we're either not cranking OR allowed to correct in cranking
	bool allowCorrections = !isCranking || engineConfiguration->useAdvanceCorrectionsForCranking;

	if (allowCorrections) {
		angle_t correction = getAdvanceCorrections(engineLoad);
		if (!std::isnan(correction)) { // correction could be NaN during settings update
			angle += correction;
		}
	}

	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, !std::isnan(angle), "_AngleN5", 0);
	return angle;
}

angle_t IgnitionState::getWrappedAdvance(const float rpm, const float engineLoad) {
    angle_t angle = getAdvance(rpm, engineLoad) * luaTimingMult + luaTimingAdd;
    wrapAngle(angle, "getWrappedAdvance", ObdCode::CUSTOM_ERR_ADCANCE_CALC_ANGLE);
    return angle;
}

PUBLIC_API_WEAK_SOMETHING_WEIRD
angle_t getCylinderIgnitionTrim(size_t cylinderNumber, float rpm, float ignitionLoad) {
	return IgnitionState::getInterpolatedIgnitionTrim(cylinderNumber, rpm, ignitionLoad);
}

size_t getMultiSparkCount(float rpm) {
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

/**
 * @return Spark dwell time, in milliseconds. 0 if tables are not ready.
 */
floatms_t IgnitionState::getSparkDwell(float rpm, bool isCranking) {
	float dwellMs;
	if (isCranking) {
		dwellMs = engineConfiguration->ignitionDwellForCrankingMs;
	} else {
		efiAssert(ObdCode::CUSTOM_ERR_ASSERT, !std::isnan(rpm), "invalid rpm", NAN);

		baseDwell = interpolate2d(rpm, config->sparkDwellRpmBins, config->sparkDwellValues);
		dwellVoltageCorrection = interpolate2d(
				Sensor::getOrZero(SensorType::BatteryVoltage),
				config->dwellVoltageCorrVoltBins,
				config->dwellVoltageCorrValues
		);

		// for compat (table full of zeroes)
		if (dwellVoltageCorrection < 0.1f) {
			dwellVoltageCorrection = 1;
		}

		dwellMs = baseDwell * dwellVoltageCorrection;
	}

	if (std::isnan(dwellMs) || dwellMs <= 0) {
		// this could happen during engine configuration reset
		warning(ObdCode::CUSTOM_ERR_DWELL_DURATION, "invalid dwell: %.2f at rpm=%.0f", dwellMs, rpm);
		return 0;
	}
	return dwellMs;
}

void IgnitionState::updateDwell(float rpm, bool isCranking) {
	sparkDwell = getSparkDwell(rpm, isCranking);
	dwellDurationAngle = std::isnan(rpm) ? NAN : getDwell() / getOneDegreeTimeMs(rpm);
}

floatms_t IgnitionState::getDwell() const {
	return sparkDwell;
}

  angle_t IgnitionState::getTrailingSparkAngle(const float rpm, const float engineLoad){
	if (std::isnan(engineLoad)) {
		// default value from: https://github.com/rusefi/rusefi/commit/86683afca22ed1a8af8fd5ac9231442e2124646e#diff-6e80cdd8c55add68105618ad9e8954170a47f59814201dadd2b888509d6b2e39R176
		return 10;
	}
	return interpolate3d(
			config->trailingSparkTable,
			config->trailingSparkLoadBins, engineLoad,
			config->trailingSparkRpmBins, rpm
	);
}

angle_t IgnitionState::getSparkHardwareLatencyCorrection(){
	// time => degree
	angle_t correction = engineConfiguration->sparkHardwareLatencyCorrection / engine->rpmCalculator.oneDegreeUs;

	if (!std::isnan(correction)) {
		return correction;
	}
	return 0;
}

angle_t IgnitionState::getInterpolatedIgnitionAngle(const float rpm, const float ignitionLoad) {
	return interpolate3d(
		config->ignitionTable,
		config->ignitionLoadBins, ignitionLoad,
		config->ignitionRpmBins, rpm
	);
}

angle_t IgnitionState::getInterpolatedIgnitionTrim(
	const size_t cylinderNumber,
	const float rpm,
	const float ignitionLoad
) {
	return interpolate3d(
		config->ignTrims[cylinderNumber].table,
		config->ignTrimLoadBins, ignitionLoad,
		config->ignTrimRpmBins, rpm
	);
}

#endif // EFI_ENGINE_CONTROL
