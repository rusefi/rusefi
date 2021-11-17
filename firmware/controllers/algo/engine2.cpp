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

#if EFI_PROD_CODE
#include "svnversion.h"
#endif

#if ! EFI_UNIT_TEST
#include "status_loop.h"
#endif

WarningCodeState::WarningCodeState() {
	clear();
}

void WarningCodeState::clear() {
	warningCounter = 0;
	lastErrorCode = 0;
	timeOfPreviousWarning = DEEP_IN_THE_PAST_SECONDS;
	recentWarnings.clear();
}

void WarningCodeState::addWarningCode(obd_code_e code) {
	warningCounter++;
	lastErrorCode = code;
	if (!recentWarnings.contains(code)) {
		recentWarnings.add((int)code);
	}
}

/**
 * @param forIndicator if we want to retrieving value for TS indicator, this case a minimal period is applued
 */
bool WarningCodeState::isWarningNow(efitimesec_t now, bool forIndicator) const {
	int period = forIndicator ? maxI(3, engineConfiguration->warningPeriod) : engineConfiguration->warningPeriod;
	return absI(now - timeOfPreviousWarning) < period;
}

MockAdcState::MockAdcState() {
	memset(hasMockAdc, 0, sizeof(hasMockAdc));
}

#if EFI_ENABLE_MOCK_ADC
void MockAdcState::setMockVoltage(int hwChannel, float voltage) {
	efiAssertVoid(OBD_PCM_Processor_Fault, hwChannel >= 0 && hwChannel < MOCK_ADC_SIZE, "hwChannel out of bounds");
	efiPrintf("fake voltage: channel %d value %.2f", hwChannel, voltage);

	fakeAdcValues[hwChannel] = voltsToAdc(voltage);
	hasMockAdc[hwChannel] = true;
}
#endif /* EFI_ENABLE_MOCK_ADC */

void FuelConsumptionState::consumeFuel(float grams, efitick_t nowNt) {
	m_consumedGrams += grams;

	float elapsedSecond = m_timer.getElapsedSecondsAndReset(nowNt);

	// If it's been a long time since last injection, ignore this pulse
	if (elapsedSecond > 0.2f) {
		m_rate = 0;
	} else {
		m_rate = grams / elapsedSecond;
	}
}

float FuelConsumptionState::getConsumedGrams() const {
	return m_consumedGrams;
}

float FuelConsumptionState::getConsumptionGramPerSecond() const {
	return m_rate;
}

EngineState::EngineState() {
	timeSinceLastTChargeK = getTimeNowNt();
}

void EngineState::updateSlowSensors() {
}

void EngineState::periodicFastCallback() {
	ScopePerf perf(PE::EngineStatePeriodicFastCallback);

#if EFI_ENGINE_CONTROL
	if (!engine->slowCallBackWasInvoked) {
		warning(CUSTOM_SLOW_NOT_INVOKED, "Slow not invoked yet");
	}
	efitick_t nowNt = getTimeNowNt();
	if (engine->rpmCalculator.isCranking()) {
		crankingTime = nowNt;
		timeSinceCranking = 0.0f;
	} else {
		timeSinceCranking = nowNt - crankingTime;
	}
	recalculateAuxValveTiming();

	int rpm = engine->rpmCalculator.getRpm();
	sparkDwell = getSparkDwell(rpm);
	dwellAngle = cisnan(rpm) ? NAN :  sparkDwell / getOneDegreeTimeMs(rpm);

	// todo: move this into slow callback, no reason for IAT corr to be here
	running.intakeTemperatureCoefficient = getIatFuelCorrection();
	// todo: move this into slow callback, no reason for CLT corr to be here
	running.coolantTemperatureCoefficient = getCltFuelCorrection();

	// Fuel cut-off isn't just 0 or 1, it can be tapered
	fuelCutoffCorrection = getFuelCutOffCorrection(nowNt, rpm);

	// post-cranking fuel enrichment.
	// for compatibility reasons, apply only if the factor is greater than unity (only allow adding fuel)
	if (engineConfiguration->postCrankingFactor > 1.0f) {
		// convert to microsecs and then to seconds
		running.timeSinceCrankingInSecs = NT2US(timeSinceCranking) / US_PER_SECOND_F;
		// use interpolation for correction taper
		running.postCrankingFuelCorrection = interpolateClamped(0.0f, engineConfiguration->postCrankingFactor,
			engineConfiguration->postCrankingDurationSec, 1.0f, running.timeSinceCrankingInSecs);
	} else {
		running.postCrankingFuelCorrection = 1.0f;
	}

	cltTimingCorrection = getCltTimingCorrection();

	knockThreshold = interpolate2d(rpm, engineConfiguration->knockNoiseRpmBins,
					engineConfiguration->knockNoise);

	baroCorrection = getBaroCorrection();

	auto tps = Sensor::get(SensorType::Tps1);
	updateTChargeK(rpm, tps.value_or(0));

	float injectionMass = getInjectionMass(rpm);
	auto clResult = fuelClosedLoopCorrection();

	// compute per-bank fueling
	for (size_t i = 0; i < STFT_BANK_COUNT; i++) {
		float corr = clResult.banks[i];
		engine->injectionMass[i] = injectionMass * corr;
		engine->stftCorrection[i] = corr;
	}

	// Store the pre-wall wetting injection duration for scheduling purposes only, not the actual injection duration
	engine->injectionDuration = engine->engineModules.get<InjectorModel>().getInjectionDuration(injectionMass);

	float fuelLoad = getFuelingLoad();
	injectionOffset = getInjectionOffset(rpm, fuelLoad);

	float ignitionLoad = getIgnitionLoad();
	timingAdvance = getAdvance(rpm, ignitionLoad) * luaAdjustments.ignitionTimingMult + luaAdjustments.ignitionTimingAdd;

	// TODO: calculate me from a table!
	trailingSparkAngle = engineConfiguration->trailingSparkAngle;

	multispark.count = getMultiSparkCount(rpm);

#if EFI_LAUNCH_CONTROL
	engine->launchController.update();
#endif //EFI_LAUNCH_CONTROL

	engine->limpManager.updateState(rpm, nowNt);

#endif // EFI_ENGINE_CONTROL
}

void EngineState::updateTChargeK(int rpm, float tps) {
#if EFI_ENGINE_CONTROL
	float newTCharge = getTCharge(rpm, tps);
	// convert to microsecs and then to seconds
	efitick_t curTime = getTimeNowNt();
	float secsPassed = (float)NT2US(curTime - timeSinceLastTChargeK) / US_PER_SECOND_F;
	if (!cisnan(newTCharge)) {
		// control the rate of change or just fill with the initial value
		sd.tCharge = (sd.tChargeK == 0) ? newTCharge : limitRateOfChange(newTCharge, sd.tCharge, engineConfiguration->tChargeAirIncrLimit, engineConfiguration->tChargeAirDecrLimit, secsPassed);
		sd.tChargeK = convertCelsiusToKelvin(sd.tCharge);
		timeSinceLastTChargeK = curTime;
	}
#endif
}

SensorsState::SensorsState() {
}

int MockAdcState::getMockAdcValue(int hwChannel) const {
	efiAssert(OBD_PCM_Processor_Fault, hwChannel >= 0 && hwChannel < MOCK_ADC_SIZE, "hwChannel out of bounds", -1);
	return fakeAdcValues[hwChannel];
}

StartupFuelPumping::StartupFuelPumping() {
	isTpsAbove50 = false;
	pumpsCounter = 0;
}

void StartupFuelPumping::setPumpsCounter(int newValue) {
	if (pumpsCounter != newValue) {
		pumpsCounter = newValue;

		if (pumpsCounter == PUMPS_TO_PRIME) {
			efiPrintf("let's squirt prime pulse %.2f", pumpsCounter);
			pumpsCounter = 0;
		} else {
			efiPrintf("setPumpsCounter %d", pumpsCounter);
		}
	}
}

void StartupFuelPumping::update() {
	if (GET_RPM() == 0) {
		bool isTpsAbove50 = Sensor::getOrZero(SensorType::DriverThrottleIntent) >= 50;

		if (this->isTpsAbove50 != isTpsAbove50) {
			setPumpsCounter(pumpsCounter + 1);
		}

	} else {
		/**
		 * Engine is not stopped - not priming pumping mode
		 */
		setPumpsCounter(0);
		isTpsAbove50 = false;
	}
}

#if EFI_SIMULATOR
#define VCS_VERSION "123"
#endif

void printCurrentState(Logging *logging, int seconds, const char *engineTypeName, const char *firmwareBuildId) {
	// VersionChecker in rusEFI console is parsing these version string, please follow the expected format
	logging->appendPrintf("%s%s%d@%s %s %s %d%s", PROTOCOL_VERSION_TAG, DELIMETER,
			getRusEfiVersion(), VCS_VERSION,
			firmwareBuildId,
			engineTypeName,
			seconds,
			DELIMETER);
}

void TriggerConfiguration::update() {
	UseOnlyRisingEdgeForTrigger = isUseOnlyRisingEdgeForTrigger();
	VerboseTriggerSynchDetails = isVerboseTriggerSynchDetails();
	TriggerType = getType();
}

bool PrimaryTriggerConfiguration::isUseOnlyRisingEdgeForTrigger() const {
	return engineConfiguration->useOnlyRisingEdgeForTrigger;
}

trigger_type_e PrimaryTriggerConfiguration::getType() const {
	return engineConfiguration->trigger.type;
}

bool PrimaryTriggerConfiguration::isVerboseTriggerSynchDetails() const {
	return engineConfiguration->verboseTriggerSynchDetails;
}

bool VvtTriggerConfiguration::isUseOnlyRisingEdgeForTrigger() const {
	return engineConfiguration->vvtCamSensorUseRise;
}

trigger_type_e VvtTriggerConfiguration::getType() const {
	return engine->triggerCentral.vvtTriggerType[index];
}

bool VvtTriggerConfiguration::isVerboseTriggerSynchDetails() const {
	return engineConfiguration->verboseVVTDecoding;
}
