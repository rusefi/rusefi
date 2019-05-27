/*
 * engine2.cpp
 *
 * @date Jan 5, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

// todo: move this code to more proper locations

#include "engine.h"
#include "thermistors.h"
#include "speed_density.h"
#include "allsensors.h"
#include "fuel_math.h"
#include "engine_math.h"
#include "advance_map.h"
#include "aux_valves.h"
#if EFI_PROD_CODE
#include "svnversion.h"
#endif

#if ! EFI_UNIT_TEST
#include "status_loop.h"
#endif

extern fuel_Map3D_t veMap;
extern afr_Map3D_t afrMap;

EXTERN_ENGINE
;

// this does not look exactly right
extern LoggingWithStorage engineLogger;

#if EFI_TUNER_STUDIO
extern TunerStudioOutputChannels tsOutputChannels;
#endif /* EFI_TUNER_STUDIO */

WarningCodeState::WarningCodeState() {
	clear();
}

void WarningCodeState::clear() {
	warningCounter = 0;
	lastErrorCode = 0;
	timeOfPreviousWarning = -10;
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
bool WarningCodeState::isWarningNow(efitimesec_t now, bool forIndicator DECLARE_ENGINE_PARAMETER_SUFFIX) const {
	int period = forIndicator ? maxI(3, engineConfiguration->warningPeriod) : engineConfiguration->warningPeriod;
	return absI(now - timeOfPreviousWarning) < period;
}

MockAdcState::MockAdcState() {
	memset(hasMockAdc, 0, sizeof(hasMockAdc));
}

#if EFI_ENABLE_MOCK_ADC || EFI_SIMULATOR
void MockAdcState::setMockVoltage(int hwChannel, float voltage) {
	scheduleMsg(&engineLogger, "fake voltage: channel %d value %.2f", hwChannel, voltage);

	fakeAdcValues[hwChannel] = voltsToAdc(voltage);
	hasMockAdc[hwChannel] = true;
}
#endif /* EFI_ENABLE_MOCK_ADC */

FuelConsumptionState::FuelConsumptionState() {
	accumulatedSecondPrevNt = accumulatedMinutePrevNt = getTimeNowNt();
}

void FuelConsumptionState::addData(float durationMs) {
	if (durationMs > 0.0f) {
		perSecondAccumulator += durationMs;
		perMinuteAccumulator += durationMs;
	}
}

void FuelConsumptionState::update(efitick_t nowNt DECLARE_ENGINE_PARAMETER_SUFFIX) {
	efitick_t deltaNt = nowNt - accumulatedSecondPrevNt;
	if (deltaNt >= US2NT(US_PER_SECOND_LL)) {
		perSecondConsumption = getFuelRate(perSecondAccumulator, deltaNt PASS_ENGINE_PARAMETER_SUFFIX);
		perSecondAccumulator = 0;
		accumulatedSecondPrevNt = nowNt;
	}

	deltaNt = nowNt - accumulatedMinutePrevNt;
	if (deltaNt >= US2NT(US_PER_SECOND_LL * 60)) {
		perMinuteConsumption = getFuelRate(perMinuteAccumulator, deltaNt PASS_ENGINE_PARAMETER_SUFFIX);
		perMinuteAccumulator = 0;
		accumulatedMinutePrevNt = nowNt;
	}
}

TransmissionState::TransmissionState() {
}

EngineState::EngineState() {
	timeSinceLastTChargeK = getTimeNowNt();
}

void EngineState::updateSlowSensors(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engine->sensors.iat = getIntakeAirTemperature(PASS_ENGINE_PARAMETER_SIGNATURE);
	engine->sensors.clt = getCoolantTemperature(PASS_ENGINE_PARAMETER_SIGNATURE);

	// todo: reduce code duplication with 'getCoolantTemperature'
	if (engineConfiguration->auxTempSensor1.adcChannel != EFI_ADC_NONE) {
		engine->sensors.auxTemp1 = getTemperatureC(&engineConfiguration->auxTempSensor1,
				&engine->engineState.auxTemp1Curve,
				false);
	}
	if (engineConfiguration->auxTempSensor2.adcChannel != EFI_ADC_NONE) {
		engine->sensors.auxTemp2 = getTemperatureC(&engineConfiguration->auxTempSensor2,
				&engine->engineState.auxTemp2Curve,
				false);
	}

#if EFI_UNIT_TEST
	if (!cisnan(engine->sensors.mockClt)) {
		engine->sensors.clt = engine->sensors.mockClt;
	}
#endif
	engine->sensors.oilPressure = getOilPressure(PASS_ENGINE_PARAMETER_SIGNATURE);

	warmupTargetAfr = interpolate2d("warm", engine->sensors.clt, engineConfiguration->warmupTargetAfrBins,
			engineConfiguration->warmupTargetAfr, WARMUP_TARGET_AFR_SIZE);
}

void EngineState::periodicFastCallback(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
#if EFI_ENGINE_CONTROL
	if (!engine->slowCallBackWasInvoked) {
		warning(CUSTOM_ERR_6696, "Slow not invoked yet");
	}
	efitick_t nowNt = getTimeNowNt();
	if (ENGINE(rpmCalculator).isCranking(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		crankingTime = nowNt;
		timeSinceCranking = 0.0f;
	} else {
		timeSinceCranking = nowNt - crankingTime;
	}
	updateAuxValves(PASS_ENGINE_PARAMETER_SIGNATURE);

	int rpm = ENGINE(rpmCalculator).getRpm(PASS_ENGINE_PARAMETER_SIGNATURE);
	sparkDwell = getSparkDwell(rpm PASS_ENGINE_PARAMETER_SUFFIX);
	dwellAngle = cisnan(rpm) ? NAN :  sparkDwell / getOneDegreeTimeMs(rpm);
	if (hasAfrSensor(PASS_ENGINE_PARAMETER_SIGNATURE)) {
		engine->sensors.currentAfr = getAfr(PASS_ENGINE_PARAMETER_SIGNATURE);
	}

	// todo: move this into slow callback, no reason for IAT corr to be here
	iatFuelCorrection = getIatFuelCorrection(engine->sensors.iat PASS_ENGINE_PARAMETER_SUFFIX);
	// todo: move this into slow callback, no reason for CLT corr to be here
	if (CONFIGB(useWarmupPidAfr) && engine->sensors.clt < engineConfiguration->warmupAfrThreshold) {
		if (rpm < 200) {
			cltFuelCorrection = 1;
			warmupAfrPid.reset();
		} else {
			cltFuelCorrection = warmupAfrPid.getOutput(warmupTargetAfr, engine->sensors.currentAfr, MS2SEC(FAST_CALLBACK_PERIOD_MS));
		}
		if (engineConfiguration->debugMode == DBG_WARMUP_ENRICH) {
#if EFI_TUNER_STUDIO
			tsOutputChannels.debugFloatField1 = warmupTargetAfr;
			warmupAfrPid.postState(&tsOutputChannels);
#endif /* EFI_TUNER_STUDIO */
		}
	} else {
		cltFuelCorrection = getCltFuelCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);
	}

	// update fuel consumption states
	fuelConsumption.update(nowNt PASS_ENGINE_PARAMETER_SUFFIX);

	// Fuel cut-off isn't just 0 or 1, it can be tapered
	fuelCutoffCorrection = getFuelCutOffCorrection(nowNt, rpm PASS_ENGINE_PARAMETER_SUFFIX);

	// post-cranking fuel enrichment.
	// for compatibility reasons, apply only if the factor is greater than zero (0.01 margin used)
	if (engineConfiguration->postCrankingFactor > 0.01f) {
		// convert to microsecs and then to seconds
		float timeSinceCrankingInSecs = NT2US(timeSinceCranking) / 1000000.0f;
		// use interpolation for correction taper
		postCrankingFuelCorrection = interpolateClamped(0.0f, engineConfiguration->postCrankingFactor,
			engineConfiguration->postCrankingDurationSec, 1.0f, timeSinceCrankingInSecs);
	} else {
		postCrankingFuelCorrection = 1.0f;
	}

	cltTimingCorrection = getCltTimingCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);

	engineNoiseHipLevel = interpolate2d("knock", rpm, engineConfiguration->knockNoiseRpmBins,
					engineConfiguration->knockNoise, ENGINE_NOISE_CURVE_SIZE);

	baroCorrection = getBaroCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);

	injectionOffset = getInjectionOffset(rpm PASS_ENGINE_PARAMETER_SUFFIX);
	float engineLoad = getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE);
	timingAdvance = getAdvance(rpm, engineLoad PASS_ENGINE_PARAMETER_SUFFIX);

	if (engineConfiguration->fuelAlgorithm == LM_SPEED_DENSITY) {
		float tps = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE);
		updateTChargeK(rpm, tps PASS_ENGINE_PARAMETER_SUFFIX);
		float map = getMap(PASS_ENGINE_PARAMETER_SIGNATURE);

		/**
		 * *0.01 because of https://sourceforge.net/p/rusefi/tickets/153/
		 */
		if (CONFIGB(useTPSBasedVeTable)) {
			// todo: should we have 'veTpsMap' fuel_Map3D_t variable here?
			currentRawVE = interpolate3d<float>(tps, CONFIG(ignitionTpsBins), IGN_TPS_COUNT, rpm, config->veRpmBins, FUEL_RPM_COUNT, veMap.pointers);
		} else {
			currentRawVE = veMap.getValue(rpm, map);
		}
		// get VE from the separate table for Idle
		if (CONFIG(useSeparateVeForIdle)) {
			float idleVe = interpolate2d("idleVe", rpm, config->idleVeBins, config->idleVe, IDLE_VE_CURVE_SIZE);
			// interpolate between idle table and normal (running) table using TPS threshold
			currentRawVE = interpolateClamped(0.0f, idleVe, CONFIGB(idlePidDeactivationTpsThreshold), currentRawVE, tps);
		}
		currentBaroCorrectedVE = baroCorrection * currentRawVE * PERCENT_DIV;
		targetAFR = afrMap.getValue(rpm, map);
	} else {
		baseTableFuel = getBaseTableFuel(rpm, engineLoad);
	}
#endif
}

void EngineState::updateTChargeK(int rpm, float tps DECLARE_ENGINE_PARAMETER_SUFFIX) {
#if EFI_ENGINE_CONTROL
	float coolantC = ENGINE(sensors.clt);
	float intakeC = ENGINE(sensors.iat);
	float newTCharge = getTCharge(rpm, tps, coolantC, intakeC PASS_ENGINE_PARAMETER_SUFFIX);
	// convert to microsecs and then to seconds
	efitick_t curTime = getTimeNowNt();
	float secsPassed = (float)NT2US(curTime - timeSinceLastTChargeK) / 1000000.0f;
	if (!cisnan(newTCharge)) {
		// control the rate of change or just fill with the initial value
		tCharge = (tChargeK == 0) ? newTCharge : limitRateOfChange(newTCharge, tCharge, CONFIG(tChargeAirIncrLimit), CONFIG(tChargeAirDecrLimit), secsPassed);
		tChargeK = convertCelsiusToKelvin(tCharge);
		timeSinceLastTChargeK = curTime;
	}
#endif
}

SensorsState::SensorsState() {
}

int MockAdcState::getMockAdcValue(int hwChannel) const {
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
			scheduleMsg(&engineLogger, "let's squirt prime pulse %.2f", pumpsCounter);
			pumpsCounter = 0;
		} else {
			scheduleMsg(&engineLogger, "setPumpsCounter %d", pumpsCounter);
		}
	}
}

void StartupFuelPumping::update(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (GET_RPM() == 0) {
		bool isTpsAbove50 = getTPS(PASS_ENGINE_PARAMETER_SIGNATURE) >= 50;

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

void printCurrentState(Logging *logging, int seconds, const char *name) {
	logging->appendPrintf("%s%s%d@%s %s %d%s", RUS_EFI_VERSION_TAG, DELIMETER,
			getRusEfiVersion(), VCS_VERSION,
			name,
			seconds,
			DELIMETER);
}

