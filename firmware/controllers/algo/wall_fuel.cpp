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

// Inicialização das variáveis estáticas
float WallFuelController::lambdaBuffer[400] = {0};
float WallFuelController::rpmBuffer[400] = {0};
float WallFuelController::mapBuffer[400] = {0};
int WallFuelController::bufferIdx = 0;
int WallFuelController::bufferMaxSize = 200;
bool WallFuelController::monitoring = false;

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

	auto rpmBin = priv::getBin(rpm, config->wwRpmBins);
	auto mapBin = priv::getBin(map, config->wwMapBins);
	int j = rpmBin.Idx;
	int i = mapBin.Idx;
	
	if (isTransient) {
		monitoring = true;
		bufferIdx = 0;
		
		// Calcular número de amostras baseado no tau atual
		float tau = computeTau();
		int calculatedSamples = (int)(engineConfiguration->wwSampleMultiplier * tau * rpm / 60.0f);
		bufferMaxSize = minI(engineConfiguration->wwMaxSampleSize, maxI(engineConfiguration->wwMinSampleSize, calculatedSamples));
	}
	
	if (monitoring) {
		if (bufferIdx < bufferMaxSize) {
			lambdaBuffer[bufferIdx] = lambda;
			rpmBuffer[bufferIdx] = rpm;
			mapBuffer[bufferIdx] = map;
			bufferIdx++;
		} else {
			// 1. Dividir as amostras em três fases
			int faseInicial = bufferMaxSize / 5;            // Primeiros 20% das amostras
			int faseFinal = bufferMaxSize - faseInicial;    // Últimos 20% das amostras

			// 2. Calcular médias ponderadas para cada fase
			float mediaInicial = 0, mediaTransiente = 0, mediaFinal = 0;
			float somaPesos = 0;

			// Fase inicial (resposta imediata)
			for (int k = 0; k < faseInicial; k++) {
				float peso = 1.0f;
				mediaInicial += lambdaBuffer[k] * peso;
				somaPesos += peso;
			}
			mediaInicial /= somaPesos;

			// Fase transiente (meio do evento)
			somaPesos = 0;
			for (int k = faseInicial; k < faseFinal; k++) {
				// Peso crescente durante a fase transiente
				float progresso = (float)(k - faseInicial) / (faseFinal - faseInicial);
				float peso = 0.5f + 0.5f * progresso; // Peso aumenta de 0.5 a 1.0
				mediaTransiente += lambdaBuffer[k] * peso;
				somaPesos += peso;
			}
			mediaTransiente /= somaPesos;

			// Fase de estabilização (final)
			somaPesos = 0;
			for (int k = faseFinal; k < bufferMaxSize; k++) {
				float peso = 2.0f; // Maior peso para amostras estabilizadas
				mediaFinal += lambdaBuffer[k] * peso;
				somaPesos += peso;
			}
			mediaFinal /= somaPesos;

			// 3. Calcular erros por fase
			float erroInicial = targetLambda - mediaInicial;
			float erroTransiente = targetLambda - mediaTransiente;
			float erroFinal = targetLambda - mediaFinal;

			// Salvar para diagnóstico
			lastImmediateError = erroInicial;
			lastProlongedError = erroFinal;

			// 4. Aplicar correções com pesos específicos para cada fase
			float learnRate = engineConfiguration->wwLearningRate;
			float maxStep = 0.05f;
			
			float deltaBeta = learnRate * (
				engineConfiguration->wwBetaInitWeight * erroInicial +     
				engineConfiguration->wwBetaTransWeight * erroTransiente +  
				engineConfiguration->wwBetaFinalWeight * erroFinal         
			);

			float deltaTau = -learnRate * (
				engineConfiguration->wwTauInitWeight * erroInicial +     
				engineConfiguration->wwTauTransWeight * erroTransiente +  
				engineConfiguration->wwTauFinalWeight * erroFinal         
			);

			// Aplicar limites e atualizar tabelas de correção
			deltaBeta = clampF(-maxStep, deltaBeta, maxStep);
			deltaTau = clampF(-maxStep, deltaTau, maxStep);
			
			config->betaCorrection[i][j] = config->betaCorrection[i][j] * (1.0f + deltaBeta);
			config->tauCorrection[i][j] = config->tauCorrection[i][j] * (1.0f + deltaTau);
			
			config->betaCorrection[i][j] = clampF(0.05f, config->betaCorrection[i][j], 2.0f);
			config->tauCorrection[i][j] = clampF(0.1f, config->tauCorrection[i][j], 2.0f);
			
			// Suavização após aprendizado
			smoothCorrectionTable(config->betaCorrection, engineConfiguration->wwSmoothIntensity);
			smoothCorrectionTable(config->tauCorrection, engineConfiguration->wwSmoothIntensity);
			
			monitoring = false;
			// Marcar aprendizado pendente
			pendingWwSave = true;
		}
	}
}

void WallFuelController::onIgnitionStateChanged(bool ignitionOn) {
	if (!ignitionOn && pendingWwSave) {
		setNeedToWriteConfiguration();
		pendingWwSave = false;
	}
}

void WallFuelController::smoothCorrectionTable(float table[WW_RPM_BINS][WW_MAP_BINS], float intensity) {
	smoothTable(table, intensity);
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
	// Detectar transiente usando derivada média de TPS/MAP
	constexpr int N = 50; // janela de 50 amostras (~250ms)
	constexpr float callbackPeriod = FAST_CALLBACK_PERIOD_MS / 1000.0f; // em segundos
	constexpr float tpsThreshold = 8.0f; // %/s (ajustável)
	constexpr float mapThreshold = 40.0f; // kPa/s (ajustável)
	static float tpsBuffer[N] = {0};
	static float mapBuffer[N] = {0};
	static int bufIdx = 0;
	static bool bufferFilled = false;
	float tps = Sensor::getOrZero(SensorType::Tps1);
	float map = Sensor::getOrZero(SensorType::Map);
	tpsBuffer[bufIdx] = tps;
	mapBuffer[bufIdx] = map;
	int oldestIdx = (bufIdx + 1) % N;
	float tpsDelta = tps - tpsBuffer[oldestIdx];
	float mapDelta = map - mapBuffer[oldestIdx];
	float tpsDeriv = tpsDelta / (N * callbackPeriod); // %/s
	float mapDeriv = mapDelta / (N * callbackPeriod); // kPa/s
	
	bool isTransient = false;
	if (bufferFilled) {
		bool conditionsMet = fabsf(tpsDeriv) > tpsThreshold || fabsf(mapDeriv) > mapThreshold;
		
		// Só considera transiente se passou tempo suficiente desde o último
		if (conditionsMet) {
			efitimeus_t currentTimeUs = getTimeNowUs();
			efitimeus_t transientTimeoutUs = (efitimeus_t)engineConfiguration->wwTransientTimeoutMs * 1000;
			
			if (currentTimeUs - lastTransientTime > transientTimeoutUs) {
				isTransient = true;
				lastTransientTime = currentTimeUs;
			}
		}
	}
	
	bufIdx = (bufIdx + 1) % N;
	if (bufIdx == 0) bufferFilled = true;
	float lambda = Sensor::getOrZero(SensorType::Lambda1);
	float targetLambda = engine->fuelComputer.targetLambda;
	float clt = Sensor::getOrZero(SensorType::Clt);
	adaptiveLearning(rpm, map, lambda, targetLambda, isTransient, clt);
}

WallFuelController::WallFuelController() {
	// Não é mais necessário inicializar tauCorrection/betaCorrection aqui, pois estão em config
}
