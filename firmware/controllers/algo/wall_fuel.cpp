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
const int WallFuelController::WW_BUFFER_MAX;
float WallFuelController::lambdaBuffer[WW_BUFFER_MAX] = {0};
float WallFuelController::rpmBuffer[WW_BUFFER_MAX] = {0};
float WallFuelController::mapBuffer[WW_BUFFER_MAX] = {0};
int WallFuelController::bufferIdx = 0;
int WallFuelController::bufferMaxSize = 200;
bool WallFuelController::monitoring = false;
TransientDirection WallFuelController::currentTransientDirection = TransientDirection::NONE;

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

// Implementação da função auxiliar para calcular média ponderada
float WallFuelController::calculateWeightedAverage(int startIdx, int endIdx, float targetLambda) {
    float soma = 0;
    float somaPesos = 0;
    
    for (int k = startIdx; k < endIdx; k++) {
        // Algoritmo de peso - pode ser ajustado conforme necessário
        float desvio = fabsf(lambdaBuffer[k] - targetLambda);
        float peso = 1.0f / (1.0f + desvio); // Valores mais próximos do target têm peso maior
        
        soma += lambdaBuffer[k] * peso;
        somaPesos += peso;
    }
    
    // Verifica se temos amostras suficientes
    if (somaPesos > 0) {
        return soma / somaPesos;
    } else {
        return targetLambda; // Valor padrão se não houver amostras
    }
}

void WallFuelController::adaptiveLearning(float rpm, float map, float lambda, float targetLambda, 
                                        bool isTransient, TransientDirection direction, float clt) {
	// Validação robusta de dados antes de qualquer processamento
	LearningDataQuality quality = validateLearningData(lambda, targetLambda, clt, map, rpm);
	
	if (!isLearningDataValid(quality)) {
		// Dados não são adequados para aprendizado
		monitoring = false;
		return;
	}
	
	// Verificar se correções direcionais estão habilitadas
	if (!engineConfiguration->wwDirectionalCorrections) {
		return;
	}

	auto rpmBin = priv::getBin(rpm, config->wwRpmBins);
	auto mapBin = priv::getBin(map, config->wwMapBins);
	int j = rpmBin.Idx;
	int i = mapBin.Idx;
	
	// Validar índices
	if (i >= WW_MAP_BINS || j >= WW_RPM_BINS) {
		return;
	}
	
	lastTransientDirection = direction;
	
	if (isTransient) {
		monitoring = true;
		bufferIdx = 0;
		
		// Calcular tamanho ótimo do buffer baseado em tau e RPM
		float tau = computeTauWithDirection(direction);
		bufferMaxSize = calculateOptimalBufferSize(tau, rpm);
	}
	
	if (monitoring) {
		if (bufferIdx < bufferMaxSize) {
			// Validar novamente cada amostra antes de armazenar
			LearningDataQuality sampleQuality = validateLearningData(lambda, targetLambda, clt, map, rpm);
			if (isLearningDataValid(sampleQuality)) {
				lambdaBuffer[bufferIdx] = lambda;
				rpmBuffer[bufferIdx] = rpm;
				mapBuffer[bufferIdx] = map;
				bufferIdx++;
			}
		} else {
			// Buffer cheio - processar aprendizado
			
			// 1. Dividir as amostras em três fases
			int faseInicial = bufferMaxSize / 5;      // Primeiros 20% das amostras
			int faseFinal = bufferMaxSize - faseInicial; // Últimos 20% das amostras
			
			// 2. Calcular médias ponderadas para cada fase
			float mediaInicial = calculateWeightedAverage(0, faseInicial, targetLambda);
			float mediaTransiente = calculateWeightedAverage(faseInicial, faseFinal, targetLambda);
			float mediaFinal = calculateWeightedAverage(faseFinal, bufferMaxSize, targetLambda);
			
			// 3. Calcular erros por fase
			float erroInicial = targetLambda - mediaInicial;
			float erroTransiente = targetLambda - mediaTransiente;
			float erroFinal = targetLambda - mediaFinal;
			
			// Salvar para diagnóstico
			lastImmediateError = erroInicial;
			lastProlongedError = erroFinal;
			
			// 4. Aplicar correções com pesos específicos para cada fase
			float learnRate = engineConfiguration->wwLearningRate;
			if (learnRate <= 0.0f) learnRate = 0.01f; // Valor padrão 1%
			
			float maxStep = 0.05f; // Máximo 5% de ajuste por vez
			
			// Verificar confiança da célula antes de aplicar correção
			float betaConfidence = betaLearningStatus[i][j].confidence;
			float tauConfidence = tauLearningStatus[i][j].confidence;
			
			// Reduzir taxa de aprendizado para células com baixa confiança
			float betaLearnRate = learnRate * (0.5f + 0.5f * betaConfidence);
			float tauLearnRate = learnRate * (0.5f + 0.5f * tauConfidence);
			
			// Cálculo da correção baseado na direção do transiente
			float deltaBeta = 0.0f;
			float deltaTau = 0.0f;
			
			if (direction == TransientDirection::POSITIVE) {
				// Para aceleração
				deltaBeta = betaLearnRate * (
					engineConfiguration->wwBetaInitWeight * erroInicial +
					engineConfiguration->wwBetaTransWeight * erroTransiente +
					engineConfiguration->wwBetaFinalWeight * erroFinal
				);
				
				deltaTau = -tauLearnRate * (
					engineConfiguration->wwTauInitWeight * erroInicial +
					engineConfiguration->wwTauTransWeight * erroTransiente +
					engineConfiguration->wwTauFinalWeight * erroFinal
				);
				
				// Aplicar limites e atualizar tabelas de correção para aceleração
				deltaBeta = clampF(-maxStep, deltaBeta, maxStep);
				deltaTau = clampF(-maxStep, deltaTau, maxStep);
				
				// Converter para a escala autoscale (0-255)
				float currentBetaValue = config->wwBetaAccel[i][j] * 0.01f;
				float currentTauValue = config->wwTauAccel[i][j] * 0.01f;
				
				float newBetaValue = currentBetaValue * (1.0f + deltaBeta);
				float newTauValue = currentTauValue * (1.0f + deltaTau);
				
				newBetaValue = clampF(0.05f, newBetaValue, 2.5f);
				newTauValue = clampF(0.05f, newTauValue, 2.5f);
				
				// Converter de volta para valor inteiro para armazenamento
				config->wwBetaAccel[i][j] = (uint8_t)(newBetaValue * 100.0f);
				config->wwTauAccel[i][j] = (uint8_t)(newTauValue * 100.0f);
				
				// Atualizar confiança das células
				updateCellConfidence(i, j, true, deltaBeta, quality);
				updateCellConfidence(i, j, false, deltaTau, quality);
				
				// Suavização após aprendizado
				float smoothIntensity = engineConfiguration->wwSmoothIntensity;
				if (smoothIntensity > 0.0f) {
					smoothCorrectionTable(config->wwBetaAccel, i, j, smoothIntensity);
					smoothCorrectionTable(config->wwTauAccel, i, j, smoothIntensity);
				}
			} 
			else if (direction == TransientDirection::NEGATIVE) {
				// Para desaceleração - lógica invertida
				deltaBeta = -betaLearnRate * (  // Nota: sinal invertido para desaceleração
					engineConfiguration->wwBetaInitWeight * erroInicial +
					engineConfiguration->wwBetaTransWeight * erroTransiente +
					engineConfiguration->wwBetaFinalWeight * erroFinal
				);
				
				deltaTau = tauLearnRate * (  // Nota: sinal invertido para desaceleração
					engineConfiguration->wwTauInitWeight * erroInicial +
					engineConfiguration->wwTauTransWeight * erroTransiente +
					engineConfiguration->wwTauFinalWeight * erroFinal
				);
				
				// Aplicar limites e atualizar tabelas de correção para desaceleração
				deltaBeta = clampF(-maxStep, deltaBeta, maxStep);
				deltaTau = clampF(-maxStep, deltaTau, maxStep);
				
				// Converter para a escala autoscale (0-255)
				float currentBetaValue = config->wwBetaDecel[i][j] * 0.01f;
				float currentTauValue = config->wwTauDecel[i][j] * 0.01f;
				
				float newBetaValue = currentBetaValue * (1.0f + deltaBeta);
				float newTauValue = currentTauValue * (1.0f + deltaTau);
				
				newBetaValue = clampF(0.05f, newBetaValue, 2.5f);
				newTauValue = clampF(0.05f, newTauValue, 2.5f);
				
				// Converter de volta para valor inteiro para armazenamento
				config->wwBetaDecel[i][j] = (uint8_t)(newBetaValue * 100.0f);
				config->wwTauDecel[i][j] = (uint8_t)(newTauValue * 100.0f);
				
				// Atualizar confiança das células
				updateCellConfidence(i, j, true, deltaBeta, quality);
				updateCellConfidence(i, j, false, deltaTau, quality);
				
				// Suavização após aprendizado
				float smoothIntensity = engineConfiguration->wwSmoothIntensity;
				if (smoothIntensity > 0.0f) {
					smoothCorrectionTable(config->wwBetaDecel, i, j, smoothIntensity);
					smoothCorrectionTable(config->wwTauDecel, i, j, smoothIntensity);
				}
			}
			
			// Incrementar contador de ciclos de ajuste
			totalAdjustmentCycles++;
			
			monitoring = false;
			pendingWwSave = true;
			
			// Verificar se é necessário reset de drift
			checkAndResetDrift();
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
		
		// Usamos a média das tabelas de aceleração e desaceleração
		float tauCorrAccel = interpolate3d(config->wwTauAccel, config->wwMapBins, map, config->wwRpmBins, rpm) * 0.01f;
		float tauCorrDecel = interpolate3d(config->wwTauDecel, config->wwMapBins, map, config->wwRpmBins, rpm) * 0.01f;
		tauCorr = (tauCorrAccel + tauCorrDecel) / 2.0f;
	}
	float tau = tauBase * tauCorr;
	return tau;
}

float WallFuelController::computeTauWithDirection(TransientDirection direction) const {
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
		
		// Usar a tabela de correção apropriada com base na direção
		if (direction == TransientDirection::POSITIVE) {
			tauCorr = interpolate3d(config->wwTauAccel, config->wwMapBins, map, config->wwRpmBins, rpm) * 0.01f;
		} else if (direction == TransientDirection::NEGATIVE) {
			tauCorr = interpolate3d(config->wwTauDecel, config->wwMapBins, map, config->wwRpmBins, rpm) * 0.01f;
		} else {
			// Se não há transiente claro, usar média das duas tabelas
			float tauCorrAccel = interpolate3d(config->wwTauAccel, config->wwMapBins, map, config->wwRpmBins, rpm) * 0.01f;
			float tauCorrDecel = interpolate3d(config->wwTauDecel, config->wwMapBins, map, config->wwRpmBins, rpm) * 0.01f;
			tauCorr = (tauCorrAccel + tauCorrDecel) / 2.0f;
		}
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
		
		// Usamos a média das tabelas de aceleração e desaceleração
		float betaCorrAccel = interpolate3d(config->wwBetaAccel, config->wwMapBins, map, config->wwRpmBins, rpm) * 0.01f;
		float betaCorrDecel = interpolate3d(config->wwBetaDecel, config->wwMapBins, map, config->wwRpmBins, rpm) * 0.01f;
		betaCorr = (betaCorrAccel + betaCorrDecel) / 2.0f;
	}
	float beta = betaBase * betaCorr;
	return clampF(0, beta, 1);
}

float WallFuelController::computeBetaWithDirection(TransientDirection direction) const {
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
		
		// Usar a tabela de correção apropriada com base na direção
		if (direction == TransientDirection::POSITIVE) {
			betaCorr = interpolate3d(config->wwBetaAccel, config->wwMapBins, map, config->wwRpmBins, rpm) * 0.01f;
		} else if (direction == TransientDirection::NEGATIVE) {
			betaCorr = interpolate3d(config->wwBetaDecel, config->wwMapBins, map, config->wwRpmBins, rpm) * 0.01f;
		} else {
			// Se não há transiente claro, usar média das duas tabelas
			float betaCorrAccel = interpolate3d(config->wwBetaAccel, config->wwMapBins, map, config->wwRpmBins, rpm) * 0.01f;
			float betaCorrDecel = interpolate3d(config->wwBetaDecel, config->wwMapBins, map, config->wwRpmBins, rpm) * 0.01f;
			betaCorr = (betaCorrAccel + betaCorrDecel) / 2.0f;
		}
	}
	
	float beta = betaBase * betaCorr;
	return clampF(0, beta, 1);
}

void WallFuelController::onFastCallback() {
	if (engine->rpmCalculator.isCranking()) {
		m_enable = false;
		return;
	}
	
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	if (rpm < 100) {
		m_enable = false;
		return;
	}
	
	// Calcular tau e beta considerando a direção do transiente apenas se habilitado
	TransientDirection direction = TransientDirection::NONE;
	
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
		// Determinar a direção do transiente baseado nas derivadas
		bool isPositiveTransient = tpsDeriv > tpsThreshold || mapDeriv > mapThreshold;
		bool isNegativeTransient = tpsDeriv < -tpsThreshold || mapDeriv < -mapThreshold;
		
		if (isPositiveTransient) {
			direction = TransientDirection::POSITIVE;
		} else if (isNegativeTransient) {
			direction = TransientDirection::NEGATIVE;
		}
		
		bool conditionsMet = isPositiveTransient || isNegativeTransient;
		
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
				currentTransientDirection = direction;
			}
		}
	}
	
	bufIdx = (bufIdx + 1) % N;
	if (bufIdx == 0) bufferFilled = true;
	
	// Calcular tau e beta - usando direção se configurado
	float tau, beta;
	if (engineConfiguration->wwDirectionalCorrections) {
		tau = computeTauWithDirection(direction);
		beta = computeBetaWithDirection(direction);
	} else {
		tau = computeTau();
		beta = computeBeta();
	}
	
	if (tau < 0.01f || beta < 0.01f) {
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
	
	// Processar o aprendizado se tivermos um transiente e uso de correções direcionais
	// Usar validação robusta de sensores
	auto lambdaSensor = Sensor::get(SensorType::Lambda1);
	float targetLambda = engine->fuelComputer.targetLambda;
	float clt = Sensor::getOrZero(SensorType::Clt);
	
	if (isTransient && engineConfiguration->wwDirectionalCorrections && lambdaSensor.Valid) {
		adaptiveLearning(rpm, map, lambdaSensor.Value, targetLambda, isTransient, direction, clt);
	}
}

WallFuelController::WallFuelController() : 
	bufferIdx(0), bufferMaxSize(200), monitoring(false), pendingWwSave(false),
	currentTransientDirection(TransientDirection::NONE), lastTransientDirection(TransientDirection::NONE),
	lastImmediateError(0.0f), lastProlongedError(0.0f), totalAdjustmentCycles(0) {
	
	// Inicializar buffers
	for (int i = 0; i < WW_BUFFER_MAX; i++) {
		lambdaBuffer[i] = 1.0f;  // Valor neutro em vez de zero
		rpmBuffer[i] = 0.0f;
		mapBuffer[i] = 0.0f;
	}
	
	// Inicializar status de aprendizado de todas as células
	for (int i = 0; i < WW_MAP_BINS; i++) {
		for (int j = 0; j < WW_RPM_BINS; j++) {
			betaLearningStatus[i][j] = CellLearningStatus();
			tauLearningStatus[i][j] = CellLearningStatus();
		}
	}
	
	lastResetTimer.reset();
}

// Nova função para validação robusta de dados de aprendizado
LearningDataQuality WallFuelController::validateLearningData(float lambda, float targetLambda, float clt, float map, float rpm) {
    LearningDataQuality quality;
    
    // Validação de lambda
    quality.lambdaValid = (lambda >= MIN_LAMBDA && lambda <= MAX_LAMBDA && !std::isnan(lambda));
    
         // Validação de estabilidade de lambda (usando parâmetro configurável)
     float lambdaDeviation = fabsf(lambda - targetLambda) / targetLambda;
     float minStability = engineConfiguration->wwMinLambdaStability;
     if (minStability <= 0.0f) minStability = 0.15f; // Default 15%
     quality.conditionsStable = (lambdaDeviation <= minStability);
     
     // Validação de temperatura (usando parâmetros configuráveis)
     float minClt = engineConfiguration->wwMinClt;
     float maxClt = engineConfiguration->wwMaxCltForLearning;
     if (minClt <= 0.0f) minClt = 70.0f; // Default
     if (maxClt <= 0.0f) maxClt = 110.0f; // Default
     quality.tempAppropriate = (clt >= minClt && clt <= maxClt);
     
     // Validação de carga (MAP) (usando parâmetro configurável)
     float minMap = engineConfiguration->wwMinMapForLearning;
     if (minMap <= 0.0f) minMap = 30.0f; // Default
     quality.loadAppropriate = (map >= minMap);
    
    // Cálculo de score de qualidade
    int validConditions = 0;
    if (quality.lambdaValid) validConditions++;
    if (quality.conditionsStable) validConditions++;
    if (quality.tempAppropriate) validConditions++;
    if (quality.loadAppropriate) validConditions++;
    
    quality.qualityScore = (float)validConditions / 4.0f;
    
    // Bonus para condições muito estáveis
    if (quality.conditionsStable && lambdaDeviation < 0.05f) {
        quality.qualityScore = minF(1.0f, quality.qualityScore + 0.1f);
    }
    
    return quality;
}

// Nova função para verificar se dados são válidos para aprendizado
bool WallFuelController::isLearningDataValid(const LearningDataQuality& quality) {
    return quality.lambdaValid && quality.conditionsStable && 
           quality.tempAppropriate && quality.loadAppropriate &&
           quality.qualityScore >= 0.8f;
}

// Nova função para atualizar confiança da célula
void WallFuelController::updateCellConfidence(int i, int j, bool isBeta, float adjustment, const LearningDataQuality& quality) {
    CellLearningStatus& status = isBeta ? betaLearningStatus[i][j] : tauLearningStatus[i][j];
    
    // Incrementar contador de amostras
    status.sampleCount++;
    
         // Atualizar variância (média móvel simples das últimas diferenças)
     float adjustmentMagnitude = fabsf(adjustment);
     if (status.sampleCount == 1) {
         status.variance = adjustmentMagnitude;
     } else {
         // Média móvel exponencial da variância
         status.variance = status.variance * 0.8f + adjustmentMagnitude * 0.2f;
     }
     
     // Usar parâmetros configuráveis
     float maxVarianceThreshold = engineConfiguration->wwMaxVarianceThreshold;
     if (maxVarianceThreshold <= 0.0f) maxVarianceThreshold = 0.1f; // Default 10%
     
     int minSamplesForConfidence = engineConfiguration->wwMinSamplesForConfidence;
     if (minSamplesForConfidence <= 0) minSamplesForConfidence = 5; // Default
     
     // Atualizar confiança baseada na qualidade dos dados e consistência
     float qualityFactor = quality.qualityScore;
     float consistencyFactor = 1.0f - minF(1.0f, status.variance / maxVarianceThreshold);
     
     float newConfidence = (qualityFactor + consistencyFactor) / 2.0f;
     
     if (status.sampleCount >= minSamplesForConfidence) {
         status.confidence = status.confidence * 0.9f + newConfidence * 0.1f;
     } else {
         status.confidence = newConfidence * ((float)status.sampleCount / minSamplesForConfidence);
     }
     
     // Determinar se célula convergiu
     status.isConverged = (status.confidence > 0.8f && status.variance < maxVarianceThreshold/2.0f && 
                          status.sampleCount >= minSamplesForConfidence);
    
    // Tracking de ajustes consecutivos
    if (adjustmentMagnitude > 0.01f) {
        status.consecutiveAdjustments++;
    } else {
        status.consecutiveAdjustments = 0;
    }
    
    status.lastAdjustment = adjustment;
}

// Nova função para calcular tamanho ótimo do buffer
int WallFuelController::calculateOptimalBufferSize(float tau, float rpm) {
    if (rpm < 100.0f || tau <= 0.0f) {
        return engineConfiguration->wwMinSampleSize;
    }
    
    // Tempo de ciclo em segundos
    float cycleTimeSeconds = 60.0f / rpm;
    
    // Número de ciclos para cobrir 2x tau (tempo para 86% de estabilização)
    float timeConstantCycles = (tau * 2.0f) / cycleTimeSeconds;
    
    // Aplicar multiplicador configurável
    float multiplier = engineConfiguration->wwSampleMultiplier;
    if (multiplier <= 0.0f) multiplier = 1.5f; // Valor padrão
    
    int optimalSamples = (int)(timeConstantCycles * multiplier);
    
    // Aplicar limites configuráveis
    int minSamples = engineConfiguration->wwMinSampleSize;
    int maxSamples = engineConfiguration->wwBufferSize;
    
    if (minSamples <= 0) minSamples = 100;
    if (maxSamples <= 0) maxSamples = 400;
    
    return clampI(minSamples, optimalSamples, maxSamples);
}

 // Nova função para reset de drift
 void WallFuelController::checkAndResetDrift() {
     // Verificar se o reset de drift está habilitado
     if (!engineConfiguration->wwEnableDriftReset) {
         return;
     }
     
     // Usar intervalo configurável
     int driftResetInterval = engineConfiguration->wwDriftResetIntervalMin;
     if (driftResetInterval <= 0) driftResetInterval = 30; // Default 30 minutes
     
     // Verificar se é hora de fazer reset de drift
     if (!lastResetTimer.hasElapsedMin(driftResetInterval)) {
         return;
     }
    
    int cellsWithHighVariance = 0;
    int cellsWithExcessiveAdjustments = 0;
    
         // Usar parâmetros configuráveis
     float maxVarianceThreshold = engineConfiguration->wwMaxVarianceThreshold;
     if (maxVarianceThreshold <= 0.0f) maxVarianceThreshold = 0.1f; // Default 10%
     
     int maxConsecutiveAdjustments = engineConfiguration->wwMaxConsecutiveAdjustments;
     if (maxConsecutiveAdjustments <= 0) maxConsecutiveAdjustments = 10; // Default
     
     // Contar células problemáticas
     for (int i = 0; i < WW_MAP_BINS; i++) {
         for (int j = 0; j < WW_RPM_BINS; j++) {
             if (betaLearningStatus[i][j].variance > maxVarianceThreshold) {
                 cellsWithHighVariance++;
             }
             if (betaLearningStatus[i][j].consecutiveAdjustments > maxConsecutiveAdjustments) {
                 cellsWithExcessiveAdjustments++;
             }
             if (tauLearningStatus[i][j].variance > maxVarianceThreshold) {
                 cellsWithHighVariance++;
             }
             if (tauLearningStatus[i][j].consecutiveAdjustments > maxConsecutiveAdjustments) {
                 cellsWithExcessiveAdjustments++;
             }
         }
     }
    
    // Se mais de 25% das células têm problemas, fazer reset seletivo
    int totalCells = WW_MAP_BINS * WW_RPM_BINS * 2; // beta + tau
    if ((cellsWithHighVariance + cellsWithExcessiveAdjustments) > (totalCells / 4)) {
        resetCellsWithHighVariance();
    }
    
    lastResetTimer.reset();
}

 // Nova função para reset seletivo de células problemáticas
 void WallFuelController::resetCellsWithHighVariance() {
     // Usar parâmetros configuráveis
     float maxVarianceThreshold = engineConfiguration->wwMaxVarianceThreshold;
     if (maxVarianceThreshold <= 0.0f) maxVarianceThreshold = 0.1f; // Default 10%
     
     int maxConsecutiveAdjustments = engineConfiguration->wwMaxConsecutiveAdjustments;
     if (maxConsecutiveAdjustments <= 0) maxConsecutiveAdjustments = 10; // Default
     
     for (int i = 0; i < WW_MAP_BINS; i++) {
         for (int j = 0; j < WW_RPM_BINS; j++) {
             // Reset beta cells
             if (betaLearningStatus[i][j].variance > maxVarianceThreshold ||
                 betaLearningStatus[i][j].consecutiveAdjustments > maxConsecutiveAdjustments) {
                
                // Reset para valor padrão (100 = 1.0x)
                config->wwBetaAccel[i][j] = 100;
                config->wwBetaDecel[i][j] = 100;
                
                // Reset status
                betaLearningStatus[i][j] = CellLearningStatus();
            }
            
            // Reset tau cells
            if (tauLearningStatus[i][j].variance > maxVarianceThreshold ||
                tauLearningStatus[i][j].consecutiveAdjustments > maxConsecutiveAdjustments) {
                
                // Reset para valor padrão (100 = 1.0x)
                config->wwTauAccel[i][j] = 100;
                config->wwTauDecel[i][j] = 100;
                
                // Reset status
                tauLearningStatus[i][j] = CellLearningStatus();
            }
        }
    }
    
    // Marcar para salvar configuração
    pendingWwSave = true;
}

// Implementação da suavização de tabela
void WallFuelController::smoothCorrectionTable(uint8_t table[WW_RPM_BINS][WW_MAP_BINS], int centerI, int centerJ, float intensity) {
    smoothCorrectionTableTemplate(table, centerI, centerJ, intensity);
}

// Funções de diagnóstico
float WallFuelController::getCellConfidence(int i, int j, bool isBeta) const {
    if (i >= WW_MAP_BINS || j >= WW_RPM_BINS) return 0.0f;
    return isBeta ? betaLearningStatus[i][j].confidence : tauLearningStatus[i][j].confidence;
}

int WallFuelController::getCellSampleCount(int i, int j, bool isBeta) const {
    if (i >= WW_MAP_BINS || j >= WW_RPM_BINS) return 0;
    return isBeta ? betaLearningStatus[i][j].sampleCount : tauLearningStatus[i][j].sampleCount;
}

float WallFuelController::getCellVariance(int i, int j, bool isBeta) const {
    if (i >= WW_MAP_BINS || j >= WW_RPM_BINS) return 0.0f;
    return isBeta ? betaLearningStatus[i][j].variance : tauLearningStatus[i][j].variance;
}
