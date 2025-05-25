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

// Note: Buffers são agora membros de instância, não estáticos

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
	// Skip learning if directional corrections not enabled
	if (!engineConfiguration->wwDirectionalCorrections) {
		return;
	}

	#if WW_ENABLE_ROBUST_VALIDATION
	// Validação robusta de dados antes de qualquer processamento (opcional)
	LearningDataQuality quality = validateLearningData(lambda, targetLambda, clt, map, rpm);
	
	if (!isLearningDataValid(quality)) {
		monitoring = false;
		return;
	}
	#else
	// Validação básica simplificada
	if (lambda < MIN_LAMBDA || lambda > MAX_LAMBDA || std::isnan(lambda)) {
		monitoring = false;
		return;
	}
	
	// Check basic conditions
	if (clt < 70.0f || fabsf(lambda - targetLambda) / targetLambda > 0.15f) {
		monitoring = false;
		return;
	}
	#endif

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
		
		// Calcular tamanho ótimo do buffer (simplificado)
		float tau = computeTauWithDirection(direction);
		bufferMaxSize = calculateOptimalBufferSize(tau, rpm);
	}
	
	if (monitoring) {
		if (bufferIdx < bufferMaxSize) {
			// Simplified validation
			bool sampleValid = true;
			#if WW_ENABLE_ROBUST_VALIDATION
			LearningDataQuality sampleQuality = validateLearningData(lambda, targetLambda, clt, map, rpm);
			sampleValid = isLearningDataValid(sampleQuality);
			#else
			sampleValid = (lambda >= MIN_LAMBDA && lambda <= MAX_LAMBDA && !std::isnan(lambda));
			#endif
			
			if (sampleValid) {
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
			
			// 4. Aplicar correções com valores padrão simples
			float learnRate = 0.01f; // Default 1% learning rate
			float maxStep = 0.05f; // Máximo 5% de ajuste por vez
			
			// Verificar confiança da célula antes de aplicar correção
			float betaConfidence = getCellConfidence(i, j, true);
			float tauConfidence = getCellConfidence(i, j, false);
			
			// Reduzir taxa de aprendizado para células com baixa confiança
			float betaLearnRate = learnRate * (0.5f + 0.5f * betaConfidence / 255.0f);
			float tauLearnRate = learnRate * (0.5f + 0.5f * tauConfidence / 255.0f);
			
			// Cálculo da correção baseado na direção do transiente (simplificado)
			float deltaBeta = 0.0f;
			float deltaTau = 0.0f;
			
			// Use simplified weights (equal weighting)
			float avgError = (erroInicial + erroTransiente + erroFinal) / 3.0f;
			
			if (direction == TransientDirection::POSITIVE) {
				// Para aceleração
				deltaBeta = betaLearnRate * avgError;
				deltaTau = -tauLearnRate * avgError;
				
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
				
				#if WW_ENABLE_ROBUST_VALIDATION
				// Atualizar confiança das células (só se habilitado)
				updateCellConfidence(i, j, true, deltaBeta, quality);
				updateCellConfidence(i, j, false, deltaTau, quality);
				#else
				// Simple confidence update
				betaLearningStatus[i][j].confidence = minI(255, betaLearningStatus[i][j].confidence + 10);
				tauLearningStatus[i][j].confidence = minI(255, tauLearningStatus[i][j].confidence + 10);
				betaLearningStatus[i][j].sampleCount = minI(255, betaLearningStatus[i][j].sampleCount + 1);
				tauLearningStatus[i][j].sampleCount = minI(255, tauLearningStatus[i][j].sampleCount + 1);
				#endif
				
				// Optional smoothing (simplified)
				float smoothIntensity = 0.1f; // Default 10% smoothing
				smoothCorrectionTable(config->wwBetaAccel, i, j, smoothIntensity);
				smoothCorrectionTable(config->wwTauAccel, i, j, smoothIntensity);
			} 
			else if (direction == TransientDirection::NEGATIVE) {
				// Para desaceleração - lógica invertida
				deltaBeta = -betaLearnRate * avgError;
				deltaTau = tauLearnRate * avgError;
				
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
				
				#if WW_ENABLE_ROBUST_VALIDATION
				// Atualizar confiança das células (só se habilitado)
				updateCellConfidence(i, j, true, deltaBeta, quality);
				updateCellConfidence(i, j, false, deltaTau, quality);
				#else
				// Simple confidence update
				betaLearningStatus[i][j].confidence = minI(255, betaLearningStatus[i][j].confidence + 10);
				tauLearningStatus[i][j].confidence = minI(255, tauLearningStatus[i][j].confidence + 10);
				betaLearningStatus[i][j].sampleCount = minI(255, betaLearningStatus[i][j].sampleCount + 1);
				tauLearningStatus[i][j].sampleCount = minI(255, tauLearningStatus[i][j].sampleCount + 1);
				#endif
				
				// Optional smoothing (simplified)
				float smoothIntensity = 0.1f; // Default 10% smoothing
				smoothCorrectionTable(config->wwBetaDecel, i, j, smoothIntensity);
				smoothCorrectionTable(config->wwTauDecel, i, j, smoothIntensity);
			}
			
			#if WW_ENABLE_DRIFT_RESET
			// Incrementar contador de ciclos de ajuste
			totalAdjustmentCycles++;
			#endif
			
			monitoring = false;
			pendingWwSave = true;
			
			#if WW_ENABLE_ROBUST_VALIDATION
			// Verificar se é necessário reset de drift (só se habilitado)
			checkAndResetDrift();
			#endif
		}
	}
}

void WallFuelController::onIgnitionStateChanged(bool ignitionOn) {
	m_ignitionState = ignitionOn;
	
	if (ignitionOn) {
		// Reset timer when ignition turns on
		m_ignitionOffTimer.reset();
	} else if (pendingWwSave) {
		// Use default delay of 5 seconds after ignition off
		float saveDelaySeconds = 5.0f; // Default 5 seconds
		
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
			// Use default timeout of 1 second
			float transientTimeoutMs = 1000.0f; // Default 1 second
			
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

// Template instantiation will be done implicitly

WallFuelController::WallFuelController() : 
	bufferIdx(0), bufferMaxSize(200), monitoring(false), pendingWwSave(false),
	currentTransientDirection(TransientDirection::NONE), lastTransientDirection(TransientDirection::NONE),
	lastImmediateError(0.0f), lastProlongedError(0.0f) {
	
	// Inicializar buffers
	for (int i = 0; i < WW_BUFFER_MAX; i++) {
		lambdaBuffer[i] = 1.0f;  // Valor neutro em vez de zero
		rpmBuffer[i] = 0.0f;
		mapBuffer[i] = 0.0f;
	}
	
	// Inicializar status simplificado de aprendizado de todas as células
	for (int i = 0; i < WW_MAP_BINS; i++) {
		for (int j = 0; j < WW_RPM_BINS; j++) {
			betaLearningStatus[i][j] = SimpleLearningStatus();
			tauLearningStatus[i][j] = SimpleLearningStatus();
		}
	}
	
	#if WW_ENABLE_DRIFT_RESET
	totalAdjustmentCycles = 0;
	lastResetTimer.reset();
	#endif
}

#if WW_ENABLE_ROBUST_VALIDATION
// Função para validação robusta de dados de aprendizado (opcional)
LearningDataQuality WallFuelController::validateLearningData(float lambda, float targetLambda, float clt, float map, float /* rpm */) {
    LearningDataQuality quality;
    
    // Validação de lambda
    quality.lambdaValid = (lambda >= MIN_LAMBDA && lambda <= MAX_LAMBDA && !std::isnan(lambda));
    
    // Validação de estabilidade de lambda (15% padrão fixo)
    float lambdaDeviation = fabsf(lambda - targetLambda) / targetLambda;
    quality.conditionsStable = (lambdaDeviation <= 0.15f);
    
    // Validação de temperatura (70-110°C fixo)
    quality.tempAppropriate = (clt >= 70.0f && clt <= 110.0f);
    
    // Validação de carga (MAP >= 30 kPa fixo)
    quality.loadAppropriate = (map >= 30.0f);
    
    // Cálculo de score de qualidade
    int validConditions = 0;
    if (quality.lambdaValid) validConditions++;
    if (quality.conditionsStable) validConditions++;
    if (quality.tempAppropriate) validConditions++;
    if (quality.loadAppropriate) validConditions++;
    
    quality.qualityScore = (float)validConditions / 4.0f;
    
    return quality;
}

bool WallFuelController::isLearningDataValid(const LearningDataQuality& quality) {
    return quality.lambdaValid && quality.conditionsStable && 
           quality.tempAppropriate && quality.loadAppropriate &&
           quality.qualityScore >= 0.8f;
}

void WallFuelController::updateCellConfidence(int i, int j, bool isBeta, float adjustment, const LearningDataQuality& quality) {
    SimpleLearningStatus& status = isBeta ? betaLearningStatus[i][j] : tauLearningStatus[i][j];
    
    // Incrementar contador de amostras (limitado a 255)
    if (status.sampleCount < 255) {
        status.sampleCount++;
    }
    
    // Atualizar confiança baseada na qualidade dos dados (0-255 scale)
    float qualityFactor = quality.qualityScore;
    uint8_t newConfidence = (uint8_t)(qualityFactor * 255);
    
    if (status.sampleCount >= 5) {
        // Média móvel simples para confiança
        status.confidence = (status.confidence * 3 + newConfidence) / 4;
    } else {
        status.confidence = newConfidence * status.sampleCount / 5;
    }
    
    // Determinar se célula convergiu (confiança > 80% e suficientes amostras)
    status.isConverged = (status.confidence > 204 && status.sampleCount >= 5); // 204 = 80% of 255
}

void WallFuelController::checkAndResetDrift() {
    #if WW_ENABLE_DRIFT_RESET
    // Reset simplificado a cada 30 minutos
    if (!lastResetTimer.hasElapsedMs(30 * 60 * 1000)) {
        return;
    }
    
    // Reset simples de células com baixa confiança
    for (int i = 0; i < WW_MAP_BINS; i++) {
        for (int j = 0; j < WW_RPM_BINS; j++) {
            if (betaLearningStatus[i][j].confidence < 128) { // < 50%
                betaLearningStatus[i][j] = SimpleLearningStatus();
            }
            if (tauLearningStatus[i][j].confidence < 128) { // < 50%
                tauLearningStatus[i][j] = SimpleLearningStatus();
            }
        }
    }
    
    lastResetTimer.reset();
    #endif
}

void WallFuelController::resetCellsWithHighVariance() {
    // Versão simplificada - apenas reset de células não convergidas
    for (int i = 0; i < WW_MAP_BINS; i++) {
        for (int j = 0; j < WW_RPM_BINS; j++) {
            if (!betaLearningStatus[i][j].isConverged) {
                betaLearningStatus[i][j] = SimpleLearningStatus();
            }
            if (!tauLearningStatus[i][j].isConverged) {
                tauLearningStatus[i][j] = SimpleLearningStatus();
            }
        }
    }
}
#endif // WW_ENABLE_ROBUST_VALIDATION

// Nova função para calcular tamanho ótimo do buffer (simplificada)
int WallFuelController::calculateOptimalBufferSize(float tau, float rpm) {
    if (rpm < 100.0f || tau <= 0.0f) {
        return 50; // Valor mínimo fixo
    }
    
    // Cálculo simplificado baseado em tau e RPM
    float cycleTimeSeconds = 60.0f / rpm;
    float timeConstantCycles = (tau * 2.0f) / cycleTimeSeconds;
    int optimalSamples = (int)(timeConstantCycles * 1.5f);
    
    // Limites fixos para simplicidade
    return clampF(50, optimalSamples, WW_BUFFER_MAX);
}

// Funções de diagnóstico
float WallFuelController::getCellConfidence(int i, int j, bool isBeta) const {
	if (i >= WW_MAP_BINS || j >= WW_RPM_BINS) return 0.0f;
	// Convert uint8_t confidence (0-255) back to float (0.0-1.0)
	uint8_t conf = isBeta ? betaLearningStatus[i][j].confidence : tauLearningStatus[i][j].confidence;
	return (float)conf / 255.0f;
}

int WallFuelController::getCellSampleCount(int i, int j, bool isBeta) const {
	if (i >= WW_MAP_BINS || j >= WW_RPM_BINS) return 0;
	return isBeta ? betaLearningStatus[i][j].sampleCount : tauLearningStatus[i][j].sampleCount;
}
