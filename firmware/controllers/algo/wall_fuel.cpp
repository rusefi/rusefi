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

			// 2. Encontrar as 10 amostras mais extremas (mais ricas/pobres) para fase inicial
			float extremeInitialSamples[10];
			int numExtremeSamples = minI(10, faseInicial); // Garantir que não ultrapassamos o tamanho da fase
			
			// Copiar as primeiras amostras para o array de extremos
			for (int k = 0; k < numExtremeSamples; k++) {
				extremeInitialSamples[k] = lambdaBuffer[k];
			}
			
			// Ordenar o array (pelo desvio do target, não pelo valor absoluto)
			for (int k = 0; k < numExtremeSamples; k++) {
				for (int l = k + 1; l < numExtremeSamples; l++) {
					float dev1 = fabsf(extremeInitialSamples[k] - targetLambda);
					float dev2 = fabsf(extremeInitialSamples[l] - targetLambda);
					if (dev2 > dev1) {
						// Trocar os valores
						float temp = extremeInitialSamples[k];
						extremeInitialSamples[k] = extremeInitialSamples[l];
						extremeInitialSamples[l] = temp;
					}
				}
			}
			
			// Calcular a média das amostras extremas para fase inicial
			float mediaInicial = 0;
			for (int k = 0; k < numExtremeSamples; k++) {
				mediaInicial += extremeInitialSamples[k];
			}
			mediaInicial /= numExtremeSamples;
			
			// Fase transiente (meio do evento) - cálculo ponderado
			float mediaTransiente = 0;
			float somaPesos = 0;
			for (int k = faseInicial; k < faseFinal; k++) {
				// Peso crescente durante a fase transiente
				float progresso = (float)(k - faseInicial) / (faseFinal - faseInicial);
				float peso = 0.5f + 0.5f * progresso; // Peso aumenta de 0.5 a 1.0
				mediaTransiente += lambdaBuffer[k] * peso;
				somaPesos += peso;
			}
			// Verificar se temos amostras suficientes nesta fase
			if (somaPesos > 0) {
				mediaTransiente /= somaPesos;
			} else {
				mediaTransiente = targetLambda; // Valor padrão se não houver amostras
			}

			// Fase de estabilização (final) - cálculo ponderado
			float mediaFinal = 0;
			somaPesos = 0;
			for (int k = faseFinal; k < bufferMaxSize; k++) {
				float peso = 2.0f; // Maior peso para amostras estabilizadas
				mediaFinal += lambdaBuffer[k] * peso;
				somaPesos += peso;
			}
			// Verificar se temos amostras suficientes nesta fase
			if (somaPesos > 0) {
				mediaFinal /= somaPesos;
			} else {
				mediaFinal = targetLambda; // Valor padrão se não houver amostras
			}

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
			smoothCorrectionTable(config->betaCorrection, i, j, engineConfiguration->wwSmoothIntensity);
			smoothCorrectionTable(config->tauCorrection, i, j, engineConfiguration->wwSmoothIntensity);
			
			monitoring = false;
			// Marcar aprendizado pendente
			pendingWwSave = true;
		}
	}
}

void WallFuelController::onIgnitionStateChanged(bool ignitionOn) {
	m_ignitionState = ignitionOn;
	
	if (ignitionOn) {
		// Reset timer when ignition turns on
		m_ignitionOffTimer.reset();
	} else if (pendingWwSave) {
		// Verificar se passou o tempo de delay após desligar ignição
		float saveDelaySeconds = engineConfiguration->wwIgnitionOffSaveDelay;
		if (saveDelaySeconds <= 0) {
			saveDelaySeconds = 5.0f; // Valor padrão 5 segundos
		}
		
		// Reset timer para contar o tempo desde que a ignição desligou
		m_ignitionOffTimer.reset();
		
		// Na implementação atual, marcamos para salvar imediatamente
		// mas o salvamento real acontecerá no onFastCallback após o delay
		// Isso permite que possíveis outras configurações sejam salvas juntas
		setNeedToWriteConfiguration();
		pendingWwSave = false;
	}
}

void WallFuelController::smoothCorrectionTable(float table[WW_RPM_BINS][WW_MAP_BINS], int centerI, int centerJ, float intensity) {
	// Retorno antecipado se intensidade for baixa demais
	if (intensity <= 0.01f) {
		return;
	}
	
	// Valor da célula central que foi ajustada
	float centerValue = table[centerI][centerJ];
	
	// Fator para determinar o quão forte cada célula vizinha será influenciada
	// 0.0 = sem influência, 1.0 = influência completa (igual ao valor central)
	float maxInfluence = clampF(0.0f, intensity, 0.5f);
	
	// Para cada célula na vizinhança 3x3 (células adjacentes)
	for (int di = -1; di <= 1; di++) {
		for (int dj = -1; dj <= 1; dj++) {
			// Pular a célula central
			if (di == 0 && dj == 0) {
				continue;
			}
			
			// Calcular índices da célula vizinha
			int ni = centerI + di;
			int nj = centerJ + dj;
			
			// Verificar limites da tabela
			if (ni < 0 || ni >= WW_MAP_BINS || nj < 0 || nj >= WW_RPM_BINS) {
				continue;
			}
			
			// Calcular distância (0.0-1.0 normalizada)
			float distance = sqrtf(di*di + dj*dj);
			if (distance > 1.41f) distance = 1.41f; // Normalizar distância diagonal
			distance /= 1.41f; // Normalizar para 0.0-1.0
			
			// Quanto mais próximo, maior a influência
			float influence = maxInfluence * (1.0f - distance);
			
			// Calcular novo valor como uma média ponderada
			float currentValue = table[ni][nj];
			float newValue = currentValue * (1.0f - influence) + centerValue * influence;
			
			// Atualizar o valor com limites
			table[ni][nj] = clampF(0.05f, newValue, 2.0f);
		}
	}
	
	return;
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
	static float localMapBuffer[N] = {0};
	static int bufIdx = 0;
	static bool bufferFilled = false;
	float tps = Sensor::getOrZero(SensorType::Tps1);
	float map = Sensor::getOrZero(SensorType::Map);
	tpsBuffer[bufIdx] = tps;
	localMapBuffer[bufIdx] = map;
	int oldestIdx = (bufIdx + 1) % N;
	float tpsDelta = tps - tpsBuffer[oldestIdx];
	float mapDelta = map - localMapBuffer[oldestIdx];
	float tpsDeriv = tpsDelta / (N * callbackPeriod); // %/s
	float mapDeriv = mapDelta / (N * callbackPeriod); // kPa/s
	
	bool isTransient = false;
	if (bufferFilled) {
		bool conditionsMet = fabsf(tpsDeriv) > tpsThreshold || fabsf(mapDeriv) > mapThreshold;
		
		// Só considera transiente se passou tempo suficiente desde o último
		if (conditionsMet) {
			// Verificar tempo desde último transiente usando o Timer
			float transientTimeoutMs = engineConfiguration->wwTransientTimeoutMs;
			if (transientTimeoutMs <= 0) {
				transientTimeoutMs = 1000.0f; // Valor padrão 1 segundo
			}
			
			if (m_transientTimer.hasElapsedMs(transientTimeoutMs)) {
				isTransient = true;
				m_transientTimer.reset();
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
