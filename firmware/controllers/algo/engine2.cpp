/*
 * engine2.cpp
 *
 * @date Jan 5, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

// todo: move this code to more proper locations

#include "pch.h"


#include "speed_density.h"
#include "fuel_math.h"
#include "advance_map.h"
#include "aux_valves.h"
#include "closed_loop_fuel.h"
#include "launch_control.h"
#include "injector_model.h"
#include "tunerstudio.h"

#include "rusefi/efistring.h"

#if ! EFI_UNIT_TEST
#include "status_loop.h"
#endif

WarningCodeState::WarningCodeState() {
	clear();
}

void WarningCodeState::clear() {
	warningCounter = 0;
	lastErrorCode = ObdCode::None;
	recentWarnings.clear();
}

void WarningCodeState::addWarningCode(ObdCode code, const char *text) {
	warningCounter++;
	lastErrorCode = code;

	warning_t* existing = recentWarnings.find(code);
	if (!existing) {
		chibios_rt::CriticalSectionLocker csl;

		// Add the code to the list
		existing = recentWarnings.add(warning_t(code));
	}

	if (existing) {
		// Reset the timer on the code to now
		existing->LastTriggered.reset();

		// no pending message? lets try to add this
		if ((m_msgWarning == nullptr) && (text)) {
			strlncpy(m_msg, text, sizeof(m_msg));
			m_msgWarning = existing;
		}
	}

	// Reset the "any warning" timer too
	timeSinceLastWarning.reset();
}

void WarningCodeState::refreshTs() {
	TunerStudioOutputChannels *tsOutputChannels = &engine->outputChannels;
	const int period = maxI(3, engineConfiguration->warningPeriod);

	// TODO: do we neet this sticky warning code?
	tsOutputChannels->warningCounter = engine->engineState.warnings.warningCounter;
	tsOutputChannels->lastErrorCode = static_cast<uint16_t>(engine->engineState.warnings.lastErrorCode);

	// TODO: fix OBD codes "jumping" between positions when one of codes disapears

	size_t i = 0;
	for (size_t j = 0; j < recentWarnings.getCount(); j++) {
		warning_t& warn = recentWarnings.get(j);
		if (warn.Code != ObdCode::None) {
			if (!warn.LastTriggered.hasElapsedSec(period)) {
				if (i < efi::size(tsOutputChannels->recentErrorCode)) {
					tsOutputChannels->recentErrorCode[i] = static_cast<uint16_t>(warn.Code);
					i++;
				}
			} else {
				// warning message is outdated, stop showing to TS
				if (m_msgWarning == &warn) {
					m_msg[0] = 0;
					m_msgWarning = nullptr;
				}
				// TODO:
				// reset warning as it is outdated
			}
		}
	}

	// reset rest
	for ( ; i < efi::size(tsOutputChannels->recentErrorCode); i++) {
		tsOutputChannels->recentErrorCode[i] = 0;
	}
}

bool WarningCodeState::hasWarningMessage() {
	// Do we have any error code to show as text?
	return (m_msgWarning != nullptr);
}

const char* WarningCodeState::getWarningMessage() {
	return m_msg;
}

/**
 * @param forIndicator if we want to retrieving value for TS indicator, this case a minimal period is applued
 */
bool WarningCodeState::isWarningNow() const {
	int period = maxI(3, engineConfiguration->warningPeriod);

	return !timeSinceLastWarning.hasElapsedSec(period);
}

// Check whether a particular warning is active
bool WarningCodeState::isWarningNow(ObdCode code) const {
	warning_t* warn = recentWarnings.find(code);

	// No warning found at all
	if (!warn) {
		return false;
	}

	// If the warning is old, it is not active
	return !warn->LastTriggered.hasElapsedSec(maxI(3, engineConfiguration->warningPeriod));
}

EngineState::EngineState() {
	timeSinceLastTChargeK.reset(getTimeNowNt());
}

void EngineState::updateSparkSkip() {
#if EFI_LAUNCH_CONTROL
		engine->softSparkLimiter.updateTargetSkipRatio(luaSoftSparkSkip, tractionControlSparkSkip);
		engine->hardSparkLimiter.updateTargetSkipRatio(
			luaHardSparkSkip,
			tractionControlSparkSkip,
			/*
			 * We are applying launch controller spark skip ratio only for hard skip limiter (see
			 * https://github.com/rusefi/rusefi/issues/6566#issuecomment-2153149902).
			 */
			engine->launchController.getSparkSkipRatio() + engine->shiftTorqueReductionController.getSparkSkipRatio()
		);
#endif // EFI_LAUNCH_CONTROL
}

#define MAKE_HUMAN_READABLE_ADVANCE(advance) (advance > getEngineState()->engineCycle / 2 ? advance - getEngineState()->engineCycle : advance)

void EngineState::periodicFastCallback() {
	ScopePerf perf(PE::EngineStatePeriodicFastCallback);

#if EFI_SHAFT_POSITION_INPUT
	if (!engine->slowCallBackWasInvoked) {
		warning(ObdCode::CUSTOM_SLOW_NOT_INVOKED, "Slow not invoked yet");
	}

	efitick_t nowNt = getTimeNowNt();
	bool isCranking = engine->rpmCalculator.isCranking();
	float rpm = Sensor::getOrZero(SensorType::Rpm);

	if (isCranking) {
		crankingTimer.reset(nowNt);
	}

	engine->fuelComputer.running.timeSinceCrankingInSecs = crankingTimer.getElapsedSeconds(nowNt);

#if EFI_AUX_VALVES
	recalculateAuxValveTiming();
#endif //EFI_AUX_VALVES

	engine->ignitionState.updateDwell(rpm, isCranking);

	// todo: move this into slow callback, no reason for IAT corr to be here
	engine->fuelComputer.running.intakeTemperatureCoefficient = getIatFuelCorrection();
	// todo: move this into slow callback, no reason for CLT corr to be here
	engine->fuelComputer.running.coolantTemperatureCoefficient = getCltFuelCorrection();

	engine->module<DfcoController>()->update();
	// should be called before getInjectionMass() and getLimitingTimingRetard()
	getLimpManager()->updateRevLimit(rpm);

	engine->fuelComputer.running.postCrankingFuelCorrection = getPostCrankingFuelCorrection();

	baroCorrection = getBaroCorrection();

	auto tps = Sensor::get(SensorType::Tps1);
	updateTChargeK(rpm, tps.value_or(0));

	float untrimmedInjectionMass = getInjectionMass(rpm) * engine->engineState.lua.fuelMult + engine->engineState.lua.fuelAdd;
	float fuelLoad = getFuelingLoad();

	auto clResult = engine->module<ShortTermFuelTrim>()->getCorrection(rpm, fuelLoad);

#if EFI_LTFT_CONTROL
	engine->module<LongTermFuelTrim>()->learn(clResult, rpm, fuelLoad);
	auto ltftResult = engine->module<LongTermFuelTrim>()->getTrims(rpm, fuelLoad);
#endif

	injectionStage2Fraction = getStage2InjectionFraction(rpm, engine->fuelComputer.afrTableYAxis);
	float stage2InjectionMass = untrimmedInjectionMass * injectionStage2Fraction;
	float stage1InjectionMass = untrimmedInjectionMass - stage2InjectionMass;

	// Store the pre-wall wetting injection duration for scheduling purposes only, not the actual injection duration
	engine->engineState.injectionDuration = engine->module<InjectorModelPrimary>()->getInjectionDuration(stage1InjectionMass);
	engine->engineState.injectionDurationStage2 =
		engineConfiguration->enableStagedInjection
		? engine->module<InjectorModelSecondary>()->getInjectionDuration(stage2InjectionMass)
		: 0;

	injectionOffset = getInjectionOffset(rpm, fuelLoad);
	engine->lambdaMonitor.update(rpm, fuelLoad);

#if EFI_LAUNCH_CONTROL
	engine->launchController.update();
	engine->shiftTorqueReductionController.update();
#endif //EFI_LAUNCH_CONTROL

	float l_ignitionLoad = getIgnitionLoad();
	engine->ignitionState.updateAdvanceCorrections(l_ignitionLoad);
	float baseAdvance = engine->ignitionState.getWrappedAdvance(rpm, l_ignitionLoad);
	float corrections = engineConfiguration->timingMode == TM_DYNAMIC ?
			// Pull any extra timing for knock retard
			- engine->module<KnockController>()->getKnockRetard()
			// Degrees of timing REMOVED from actual timing during soft RPM limit window
			- getLimpManager()->getLimitingTimingRetard() :
			0;
	float correctedIgnitionAdvance = baseAdvance + corrections;
	// these fields are scaled_channel so let's only use for observability, with a local variables holding value while it matters locally
	engine->ignitionState.baseIgnitionAdvance = MAKE_HUMAN_READABLE_ADVANCE(baseAdvance);
	engine->ignitionState.correctedIgnitionAdvance = MAKE_HUMAN_READABLE_ADVANCE(correctedIgnitionAdvance);

	// compute per-bank fueling
	for (size_t bankIndex = 0; bankIndex < FT_BANK_COUNT; bankIndex++) {
		engine->engineState.stftCorrection[bankIndex] = clResult.banks[bankIndex];
	}

	// Now apply that to per-cylinder fueling and timing
	for (size_t cylinderIndex = 0; cylinderIndex < engineConfiguration->cylindersCount; cylinderIndex++) {
		uint8_t bankIndex = engineConfiguration->cylinderBankSelect[cylinderIndex];
    efiAssertVoid(ObdCode::CUSTOM_OBD_BAD_BANK_INDEX, bankIndex < FT_BANK_COUNT, "bankIndex");
		/* TODO: add LTFT trims when ready */
		auto bankTrim = clResult.banks[bankIndex] *
#if EFI_LTFT_CONTROL
			ltftResult.banks[bankIndex] *
#endif
			1.0;
		auto cylinderTrim = getCylinderFuelTrim(cylinderIndex, rpm, fuelLoad);
		auto knockTrim = engine->module<KnockController>()->getFuelTrimMultiplier();

		// Apply both per-bank and per-cylinder trims
		engine->engineState.injectionMass[cylinderIndex] = untrimmedInjectionMass * bankTrim * cylinderTrim * knockTrim;

		angle_t cylinderIgnitionAdvance = correctedIgnitionAdvance
									+ getCylinderIgnitionTrim(cylinderIndex, rpm, l_ignitionLoad)
									// spark hardware latency correction, for implementation details see:
									// https://github.com/rusefi/rusefi/issues/6832:
									+ engine->ignitionState.getSparkHardwareLatencyCorrection();
		wrapAngle(cylinderIgnitionAdvance, "EngineState::periodicFastCallback", ObdCode::CUSTOM_ERR_ADCANCE_CALC_ANGLE);
		// todo: is it OK to apply cylinder trim with FIXED timing?
		timingAdvance[cylinderIndex] = cylinderIgnitionAdvance;
	}

	shouldUpdateInjectionTiming = getInjectorDutyCycle(rpm) < 90;

	engine->ignitionState.trailingSparkAngle = engine->ignitionState.getTrailingSparkAngle(rpm, l_ignitionLoad);

	multispark.count = getMultiSparkCount(rpm);

#if EFI_ANTILAG_SYSTEM
	engine->antilagController.update();
#endif //EFI_ANTILAG_SYSTEM
#endif // EFI_SHAFT_POSITION_INPUT
}

#if EFI_ENGINE_CONTROL
void EngineState::updateTChargeK(float rpm, float tps) {
	float newTCharge = engine->fuelComputer.getTCharge(rpm, tps);
	if (!std::isnan(newTCharge)) {
		// control the rate of change or just fill with the initial value
		efitick_t nowNt = getTimeNowNt();
		float secsPassed = timeSinceLastTChargeK.getElapsedSeconds(nowNt);
		sd.tCharge = (sd.tChargeK == 0) ? newTCharge : limitRateOfChange(newTCharge, sd.tCharge, engineConfiguration->tChargeAirIncrLimit, engineConfiguration->tChargeAirDecrLimit, secsPassed);
		sd.tChargeK = convertCelsiusToKelvin(sd.tCharge);
		timeSinceLastTChargeK.reset(nowNt);
	}
}
#endif

void TriggerConfiguration::update() {
	VerboseTriggerSynchDetails = isVerboseTriggerSynchDetails();
	TriggerType = getType();
}

trigger_config_s PrimaryTriggerConfiguration::getType() const {
	return engineConfiguration->trigger;
}

bool PrimaryTriggerConfiguration::isVerboseTriggerSynchDetails() const {
	return engineConfiguration->verboseTriggerSynchDetails;
}

trigger_config_s VvtTriggerConfiguration::getType() const {
	// Convert from VVT type to trigger_config_s
	return { getVvtTriggerType(engineConfiguration->vvtMode[index]), 0, 0 };
}

bool VvtTriggerConfiguration::isVerboseTriggerSynchDetails() const {
	return engineConfiguration->verboseVVTDecoding;
}

bool isLockedFromUser() {
	int lock = engineConfiguration->tuneHidingKey;
	bool isLocked = lock > 0;
	if (isLocked) {
		criticalError("Tune is password protected. Please use console to unlock tune.");
	}
	return isLocked;
}

void unlockEcu(int password) {
	if (password != engineConfiguration->tuneHidingKey) {
		efiPrintf("Nope rebooting...");
#if EFI_PROD_CODE
		scheduleReboot();
#endif // EFI_PROD_CODE
	} else {
		efiPrintf("Unlocked! Burning...");
		engineConfiguration->tuneHidingKey = 0;
		requestBurn();
	}
}
