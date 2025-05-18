/*
 * @file wall_fuel.cpp
 *
 * @author Matthew Kennedy
 */

#include "pch.h"
#include "wall_fuel.h"
#include "engine_math.h"
#include "efitime.h"
#include <rusefi/interpolation.h>

void WallFuel::resetWF() {
	wallFuel = 0;
}

float WallFuel::adjust(float desiredMassGrams) {
	invocationCounter++;
	if (std::isnan(desiredMassGrams)) {
		return desiredMassGrams;
	}

	ScopePerf perf(PE::WallFuelAdjust);

	/*
		this math is based on
				SAE 810494 by C. F. Aquino
				SAE 1999-01-0553 by Peter J Maloney

		M_cmd = commanded fuel mass (output of this function)
		desiredMassGrams = desired fuel mass (input to this function)
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
	*/

	// If disabled, pass value through
	if (!engine->module<WallFuelController>()->getEnable()) {
		return desiredMassGrams;
	}

	float alpha = engine->module<WallFuelController>()->getAlpha();
	float beta = engine->module<WallFuelController>()->getBeta();

	float fuelFilmMass = wallFuel;
	float M_cmd = (desiredMassGrams - (1 - alpha) * fuelFilmMass) / (1 - beta);

	// We can't inject a negative amount of fuel
	// If this goes below zero we will be over-fueling slightly,
	// but that's ok.
	if (M_cmd <= 0) {
		M_cmd = 0;
	}

	// remainder on walls from last time + new from this time
	float fuelFilmMassNext = alpha * fuelFilmMass + beta * M_cmd;

	wallFuel = fuelFilmMassNext;
	wallFuelCorrection = M_cmd - desiredMassGrams;
	return M_cmd;
}

float WallFuel::getWallFuel() const {
	return wallFuel;
}

void WallFuelController::adaptiveLearning(float rpm, float map, float lambda, float targetLambda, bool isTransient, float clt) {
	if (clt < engineConfiguration->wwMinClt) return;
	static bool monitoring = false;
	static float lambdaBuffer[200]; // 2s @ 100Hz
	static int bufferIdx = 0;
	static uint32_t transStartTime = 0;
	auto rpmBin = priv::getBin(rpm, config->wwRpmBins);
	auto mapBin = priv::getBin(map, config->wwMapBins);
	int i = rpmBin.Idx;
	int j = mapBin.Idx;
	if (isTransient) {
		monitoring = true;
		bufferIdx = 0;
		transStartTime = getTimeNowS();
	}
	if (monitoring) {
		if (bufferIdx < 200) {
			lambdaBuffer[bufferIdx++] = lambda;
		} else {
			// Calcular erro imediato e prolongado
			float erroImediato = 0, erroProlongado = 0;
			int nImediato = 50, nProlongado = 150;
			for (int k = 0; k < nImediato; k++) erroImediato += lambdaBuffer[k];
			for (int k = nImediato; k < nImediato + nProlongado; k++) erroProlongado += lambdaBuffer[k];
			erroImediato = erroImediato / nImediato - targetLambda;
			erroProlongado = erroProlongado / nProlongado - targetLambda;
			lastImmediateError = erroImediato;
			lastProlongedError = erroProlongado;
			float learnRate = engineConfiguration->wwLearningRate;
			float maxStep = 0.05f;
			// Pesos configuráveis
			float wBetaImediato = engineConfiguration->wBetaImediato;
			float wBetaProlongado = engineConfiguration->wBetaProlongado;
			float wTauImediato = engineConfiguration->wTauImediato;
			float wTauProlongado = engineConfiguration->wTauProlongado;
			// Ajuste simultâneo
			float deltaBeta = learnRate * (wBetaImediato * erroImediato + wBetaProlongado * erroProlongado);
			float deltaTau  = learnRate * (wTauImediato  * erroImediato + wTauProlongado  * erroProlongado);
			deltaBeta = clampF(-maxStep, deltaBeta, maxStep);
			deltaTau  = clampF(-maxStep, deltaTau,  maxStep);
			config->betaCorrection[i][j] = config->betaCorrection[i][j] * (1.0f + deltaBeta);
			config->tauCorrection[i][j]  = config->tauCorrection[i][j]  * (1.0f + deltaTau);
			config->betaCorrection[i][j] = clampF(0.05f, config->betaCorrection[i][j], 2.0f);
			config->tauCorrection[i][j]  = clampF(0.1f,  config->tauCorrection[i][j],  2.0f);
			// Suavização
			// smoothCorrectionTable(config->betaCorrection, engineConfiguration->wwSmoothIntensity);
			// smoothCorrectionTable(config->tauCorrection,  engineConfiguration->wwSmoothIntensity);
			monitoring = false;
		}
	}
	// Lógica de salvamento: só salva após ignição OFF + delay
	static bool ignitionOffSaveDelayActive = false;
	static uint32_t ignitionOffTimestamp = 0;
	static bool pendingSave = false;
	bool ignitionState = engine->module<IgnitionController>()->secondsSinceIgnVoltage() > 1.0f;
	uint32_t now = getTimeNowS();
	if (!ignitionState) {
		if (!ignitionOffSaveDelayActive) {
			ignitionOffTimestamp = now;
			ignitionOffSaveDelayActive = true;
			pendingSave = true;
		}
		if (ignitionOffSaveDelayActive && (now - ignitionOffTimestamp) >= engineConfiguration->wwIgnitionOffSaveDelay) {
			if (pendingSave) {
				setNeedToWriteConfiguration();
				pendingSave = false;
			}
			ignitionOffSaveDelayActive = false;
		}
	} else {
		ignitionOffSaveDelayActive = false;
		pendingSave = false;
	}
}

void WallFuelController::smoothCorrectionTable(float table[WW_RPM_BINS][WW_MAP_BINS], float intensity) {
	float temp[WW_RPM_BINS][WW_MAP_BINS];
	for (int i = 0; i < WW_RPM_BINS; i++) {
		for (int j = 0; j < WW_MAP_BINS; j++) {
			float sum = table[i][j];
			int count = 1;
			// Vizinhos imediatos
			if (i > 0)   { sum += table[i-1][j]; count++; }
			if (i < WW_RPM_BINS-1) { sum += table[i+1][j]; count++; }
			if (j > 0)   { sum += table[i][j-1]; count++; }
			if (j < WW_MAP_BINS-1) { sum += table[i][j+1]; count++; }
			float avg = sum / count;
			temp[i][j] = table[i][j] * (1.0f - intensity) + avg * intensity;
		}
	}
	for (int i = 0; i < WW_RPM_BINS; i++)
		for (int j = 0; j < WW_MAP_BINS; j++)
			table[i][j] = temp[i][j];
}

float WallFuelController::computeTau() const {
	if (!engineConfiguration->complexWallModel) {
		return engineConfiguration->wwaeTau;
	}
	float clt = Sensor::get(SensorType::Clt).value_or(90);
	float tauClt = interpolate2d(clt, config->wwCltBins, config->wwTauCltValues);
	float tauBase = tauClt;
	float tauCorr = 1.0f;
	float map = Sensor::get(SensorType::Map).value_or(60);
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	if (Sensor::hasSensor(SensorType::Map)) {
		tauBase *= interpolate3d(config->wwTauMapRpmValues, config->wwMapBins, map, config->wwRpmBins, rpm);
		tauCorr = interpolate3d(config->tauCorrection, config->wwMapBins, map, config->wwRpmBins, rpm);
	}
	float tau = tauBase * tauCorr;
	return tau;
}

float WallFuelController::computeBeta() const {
	if (!engineConfiguration->complexWallModel) {
		return engineConfiguration->wwaeBeta;
	}
	float clt = Sensor::get(SensorType::Clt).value_or(90);
	float betaClt = interpolate2d(clt, config->wwCltBins, config->wwBetaCltValues);
	float betaBase = betaClt;
	float betaCorr = 1.0f;
	float map = Sensor::get(SensorType::Map).value_or(60);
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	if (Sensor::hasSensor(SensorType::Map)) {
		betaBase *= interpolate3d(config->wwBetaMapRpmValues, config->wwMapBins, map, config->wwRpmBins, rpm);
		betaCorr = interpolate3d(config->betaCorrection, config->wwMapBins, map, config->wwRpmBins, rpm);
	}
	float beta = betaBase * betaCorr;
	return clampF(0, beta, 1);
}

void WallFuelController::onFastCallback() {
	if (engine->rpmCalculator.isCranking()) {
		m_enable = false;
		return;
	}
	float tau = computeTau();
	float beta = computeBeta();
	if (tau < 0.01f || beta < 0.01f) {
		m_enable = false;
		return;
	}
	auto rpm = Sensor::getOrZero(SensorType::Rpm);
	if (rpm < 100) {
		m_enable = false;
		return;
	}
	float alpha = expf_taylor(-120 / (rpm * tau));
	if (beta > alpha) {
		beta = alpha;
	}
	m_alpha = alpha;
	m_beta = beta;
	m_enable = true;
	// --- INTEGRAÇÃO DO APRENDIZADO ---
	// Detectar transiente e chamar adaptiveLearning
	static float lastTps = 0, lastMap = 0;
	float tps = Sensor::getOrZero(SensorType::Tps1);
	float map = Sensor::getOrZero(SensorType::Map);
	float dTps = tps - lastTps;
	float dMap = map - lastMap;
	bool isTransient = fabsf(dTps) > 5.0f || fabsf(dMap) > 10.0f;
	float lambda = Sensor::getOrZero(SensorType::Lambda1);
	float targetLambda = engine->fuelComputer.targetLambda;
	float clt = Sensor::getOrZero(SensorType::Clt);
	adaptiveLearning(rpm, map, lambda, targetLambda, isTransient, clt);
	lastTps = tps;
	lastMap = map;
}

WallFuelController::WallFuelController() {
	// Não é mais necessário inicializar tauCorrection/betaCorrection aqui, pois estão em config
}
