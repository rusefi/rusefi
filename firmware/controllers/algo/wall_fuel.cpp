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

// *** WALL WETTING ADAPTIVE LEARNING - NOVA IMPLEMENTAÇÃO ***
//
// MUDANÇAS PRINCIPAIS:
// 1. Separação clara de janelas temporais para Beta e Tau
// 2. Cálculo independente de erros para cada parâmetro
// 3. Filtros específicos para cada tipo de erro
//
// ESTRATÉGIA DE JANELAS:
// - Beta Window: 0-20% do buffer (mín. 10 amostras)
//   * Captura efeitos imediatos de combustível batendo na parede
//   * Usa média simples para reduzir ruído de alta frequência
//   * Foco em 1-2 ciclos após início do transiente
//
// - Tau Window: 50-100% do buffer  
//   * Captura efeitos prolongados de evaporação
//   * Usa média ponderada (low-pass) para suavizar dados
//   * Foco em 3-5 constantes de tempo tau
//
// BENEFÍCIOS:
// - Ajustes mais precisos e independentes
// - Redução de interferência entre parâmetros
// - Melhor resposta a diferentes tipos de transientes
// - Diagnóstico mais claro dos problemas

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
    int numSamples = endIdx - startIdx;
    
    if (numSamples <= 0) {
        return targetLambda; // Valor padrão se não houver amostras
    }
    
    for (int k = startIdx; k < endIdx; k++) {
        // *** MELHORIA: Peso temporal + peso por qualidade ***
        // Peso temporal: amostras mais recentes têm peso maior
        float pesoTemporal = (float)(k - startIdx + 1) / numSamples;
        
        // Peso por qualidade: valores mais próximos do target têm peso maior
        float desvio = fabsf(lambdaBuffer[k] - targetLambda);
        float pesoQualidade = 1.0f / (1.0f + desvio * 2.0f); // Penaliza mais desvios grandes
        
        // Peso combinado com ênfase na qualidade
        float peso = pesoTemporal * 0.3f + pesoQualidade * 0.7f;
        
        soma += lambdaBuffer[k] * peso;
        somaPesos += peso;
    }
    
    // Verifica se temos amostras suficientes
    if (somaPesos > 0) {
        return soma / somaPesos;
    } else {
        return targetLambda; // Valor padrão se não houver amostras válidas
    }
}

void WallFuelController::adaptiveLearning(float rpm, float map, float lambda, float targetLambda, 
                                        bool isTransient, TransientDirection direction, float clt) {

	// *** NOVA ABORDAGEM: Separação de erros Beta e Tau com janelas específicas ***
	// 
	// Beta (efeito imediato): 
	//   - Janela curta (0-20% do buffer, mín. 10 amostras)
	//   - Captura efeitos imediatos de combustível batendo na parede
	//   - Erro calculado como média simples das primeiras amostras
	//
	// Tau (efeito de evaporação):
	//   - Janela longa (50-100% do buffer)  
	//   - Captura efeitos prolongados de evaporação
	//   - Erro calculado como média ponderada (low-pass) das amostras tardias
	//
	// Esta separação permite ajustes mais precisos e independentes dos parâmetros.

	// *** VERIFICAR CONDIÇÕES DE HABILITAÇÃO ***
	if (!engineConfiguration->complexWallModel) {
		monitoring = false;
		return; // Wall wetting complexo desabilitado
	}
	
	if (!engineConfiguration->wwEnableAdaptiveLearning) {
		monitoring = false;
		return; // Aprendizado adaptativo desabilitado
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

	auto rpmBin = priv::getBin(rpm, config->wwCorrectionRpmBins);
	auto mapBin = priv::getBin(map, config->wwCorrectionMapBins);
	int j = rpmBin.Idx;
	int i = mapBin.Idx;
	
	// Validar índices
	if (i >= WW_CORRECTION_MAP_BINS || j >= WW_CORRECTION_RPM_BINS) {
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
			
			// *** NOVA IMPLEMENTAÇÃO: Separação adequada de janelas Beta vs Tau ***
			// Beta: Janela curta (0-20% do buffer) para capturar efeitos imediatos
			int betaWindow = bufferMaxSize * 0.2f;        // 20% para efeito beta (imediato)
			if (betaWindow < 10) betaWindow = 10;         // Mínimo 10 amostras conforme solicitado
			
			// Tau: Janela longa (50-100% do buffer) para capturar evaporação
			int tauWindowStart = bufferMaxSize * 0.5f;    // 50% início da janela tau
			int tauWindowEnd = bufferMaxSize;             // 100% fim da janela tau
			
			// *** CÁLCULO DE ERROS ESPECÍFICOS PARA BETA E TAU ***
			
			// Beta: Erro imediato (atraso fixo) - média simples das primeiras amostras
			float lambdaImmediate = 0.0f;
			int validBetaSamples = 0;
			for (int k = 0; k < betaWindow && k < bufferIdx; k++) {
				lambdaImmediate += lambdaBuffer[k];
				validBetaSamples++;
			}
			if (validBetaSamples > 0) {
				lambdaImmediate /= validBetaSamples;
			} else {
				lambdaImmediate = targetLambda; // Fallback
			}
			
			// Tau: Erro filtrado (low-pass) - média ponderada das amostras tardias
			float lambdaFiltered = 0.0f;
			float totalWeight = 0.0f;
			int validTauSamples = 0;
			for (int k = tauWindowStart; k < tauWindowEnd && k < bufferIdx; k++) {
				// Aplicar filtro low-pass: amostras mais tardias têm peso maior
				float weight = (float)(k - tauWindowStart + 1) / (tauWindowEnd - tauWindowStart);
				lambdaFiltered += lambdaBuffer[k] * weight;
				totalWeight += weight;
				validTauSamples++;
			}
			if (totalWeight > 0.0f) {
				lambdaFiltered /= totalWeight;
			} else {
				lambdaFiltered = targetLambda; // Fallback
			}
			
			// Calcular erros específicos
			float betaError = lambdaImmediate - targetLambda;     // Erro imediato (beta)
			float tauError = lambdaFiltered - targetLambda;       // Erro prolongado (tau)
			
			// Salvar para diagnóstico (incluindo valores intermediários)
			lastImmediateError = betaError;
			lastProlongedError = tauError;
			
			// Diagnóstico adicional: salvar valores de lambda calculados
			// (podem ser acessados via debugger ou logging se necessário)
			float debugLambdaImmediate = lambdaImmediate;
			float debugLambdaFiltered = lambdaFiltered;
			int debugBetaSamples = validBetaSamples;
			int debugTauSamples = validTauSamples;
			
			// Evitar warnings de variáveis não utilizadas
			(void)debugLambdaImmediate;
			(void)debugLambdaFiltered;
			(void)debugBetaSamples;
			(void)debugTauSamples;
			
			// Parâmetros de aprendizado
			float maxStep = 0.03f;   // Reduzido para 3% para evitar oscilações
			
			// Verificar confiança da célula antes de aplicar correção
			float betaConfidence = getCellConfidence(i, j, true);
			float tauConfidence = getCellConfidence(i, j, false);
			
			// Reduzir taxa de aprendizado para células com baixa confiança
			float betaLearnRate = engineConfiguration->wwBetaLearningRate * (0.3f + 0.7f * betaConfidence / 255.0f);
			float tauLearnRate = engineConfiguration->wwTauLearningRate * (0.3f + 0.7f * tauConfidence / 255.0f);
			
			// *** NOVA LÓGICA: Ajustes baseados nos erros específicos de Beta e Tau ***
			float deltaBeta = 0.0f;
			float deltaTau = 0.0f;
			
			if (direction == TransientDirection::POSITIVE) {
				// ACELERAÇÃO: Esperamos lambda diminuir (ficar rico)
				// Se betaError > 0: lambda imediato está acima do target (pobre) → AUMENTAR beta
				// Se tauError > 0: lambda prolongado está acima do target (pobre) → DIMINUIR tau
				deltaBeta = betaLearnRate * betaError * 1.5f;      // Ajuste agressivo para beta
				deltaTau = -tauLearnRate * tauError * 0.8f;        // Ajuste moderado para tau (sinal negativo)
				
			} else if (direction == TransientDirection::NEGATIVE) {
				// DESACELERAÇÃO: Esperamos lambda aumentar (ficar pobre)
				// Se betaError < 0: lambda imediato está abaixo do target (rico) → DIMINUIR beta
				// Se tauError < 0: lambda prolongado está abaixo do target (rico) → AUMENTAR tau
				deltaBeta = betaLearnRate * betaError * 0.8f;      // Ajuste moderado para beta
				deltaTau = -tauLearnRate * tauError * 1.3f;        // Ajuste agressivo para tau
				
			} else {
				// TRANSIENTE NEUTRO: Ajuste balanceado baseado nos erros específicos
				deltaBeta = betaLearnRate * betaError;
				deltaTau = -tauLearnRate * tauError;  // Manter sinal negativo para tau
			}
			
			// Aplicar limites
			deltaBeta = clampF(-maxStep, deltaBeta, maxStep);
			deltaTau = clampF(-maxStep, deltaTau, maxStep);
			
			// Atualizar tabelas de correção
			float currentBetaValue = config->wwBetaCorrection[i][j] * 0.01f;
			float currentTauValue = config->wwTauCorrection[i][j] * 0.01f;
			
			float newBetaValue = currentBetaValue * (1.0f + deltaBeta);
			float newTauValue = currentTauValue * (1.0f + deltaTau);
			
			newBetaValue = clampF(0.5f, newBetaValue, 2.0f);
			newTauValue = clampF(0.5f, newTauValue, 2.0f);
			
			// Converter de volta para valor inteiro para armazenamento
			config->wwBetaCorrection[i][j] = (uint8_t)(newBetaValue * 100.0f);
			config->wwTauCorrection[i][j] = (uint8_t)(newTauValue * 100.0f);
			
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
			smoothCorrectionTable(config->wwBetaCorrection, i, j, smoothIntensity);
			smoothCorrectionTable(config->wwTauCorrection, i, j, smoothIntensity);
			
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
		
		// *** CORREÇÃO: Usar apenas a tabela de correção geral (tabelas antigas removidas) ***
		tauCorr = interpolate3d(config->wwTauCorrection, config->wwCorrectionMapBins, map, config->wwCorrectionRpmBins, rpm) * 0.01f;
	}
	float tau = tauBase * tauCorr;
	return tau;
}

float WallFuelController::computeTauWithDirection(TransientDirection direction) const {
	// Note: direction parameter reserved for future directional tau adjustments
	(void)direction; // Suppress unused parameter warning
	
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
		
		// Usar a tabela de correção geral
		tauCorr = interpolate3d(config->wwTauCorrection, config->wwCorrectionMapBins, map, config->wwCorrectionRpmBins, rpm) * 0.01f;
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
		
		// Usar a tabela de correção geral
		betaCorr = interpolate3d(config->wwBetaCorrection, config->wwCorrectionMapBins, map, config->wwCorrectionRpmBins, rpm) * 0.01f;
	}
	float beta = betaBase * betaCorr;
	return clampF(0, beta, 1);
}

float WallFuelController::computeBetaWithDirection(TransientDirection direction) const {
	// Note: direction parameter reserved for future directional beta adjustments
	(void)direction; // Suppress unused parameter warning
	
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
		
		// Usar a tabela de correção geral
		betaCorr = interpolate3d(config->wwBetaCorrection, config->wwCorrectionMapBins, map, config->wwCorrectionRpmBins, rpm) * 0.01f;
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
	
	float tps = Sensor::getOrZero(SensorType::Tps1);
	float map = Sensor::getOrZero(SensorType::Map);
	
	// Enhanced transient detection
	TransientInfo transient = detectTransientEnhanced(tps, map, rpm);
	bool isTransient = transient.isValid && isTransientValid(transient);
	
	// Update current transient info
	if (isTransient) {
		m_currentTransient = transient;
		currentTransientDirection = transient.direction;
	} else {
		// Reset if no valid transient
		if (m_currentTransient.direction != TransientDirection::NONE) {
			m_currentTransient = TransientInfo(); // Reset to default
		}
	}
	
	// Calcular tau e beta - usando direção se configurado
	float tau, beta;
	tau = computeTauWithDirection(m_currentTransient.direction);
	beta = computeBetaWithDirection(m_currentTransient.direction);
	
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
	
	if (isTransient && lambdaSensor.Valid) {
		adaptiveLearning(rpm, map, lambdaSensor.Value, targetLambda, isTransient, m_currentTransient.direction, clt);
	}
}

// Template instantiation will be done implicitly

WallFuelController::WallFuelController() : 
	m_filterBufferIdx(0), m_filterBufferFilled(false),
	bufferIdx(0), bufferMaxSize(200), monitoring(false), pendingWwSave(false),
	currentTransientDirection(TransientDirection::NONE), lastTransientDirection(TransientDirection::NONE),
	lastImmediateError(0.0f), lastProlongedError(0.0f) {
	
	// Inicializar buffers
	for (int i = 0; i < WW_BUFFER_MAX; i++) {
		lambdaBuffer[i] = 1.0f;  // Valor neutro em vez de zero
		rpmBuffer[i] = 0.0f;
		mapBuffer[i] = 0.0f;
	}
	
	// Initialize transient filter buffer
	for (int i = 0; i < 10; i++) {
		m_transientFilterBuffer[i] = 0.0f;
	}
	
	// Inicializar status simplificado de aprendizado de todas as células
	for (int i = 0; i < WW_CORRECTION_MAP_BINS; i++) {
		for (int j = 0; j < WW_CORRECTION_RPM_BINS; j++) {
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
    
    // Get configurable validation parameters with fallback defaults
    float minClt = engineConfiguration->wwMinCoolantTemp > 0 ? engineConfiguration->wwMinCoolantTemp : 70.0f;
    float maxClt = engineConfiguration->wwMaxCoolantTemp > 0 ? engineConfiguration->wwMaxCoolantTemp : 110.0f;
    float minMap = engineConfiguration->wwMinMapForLearning > 0 ? engineConfiguration->wwMinMapForLearning : 30.0f;
    float minQuality = engineConfiguration->wwMinQualityScore > 0 ? engineConfiguration->wwMinQualityScore : 0.6f;
    
    // Validação de lambda
    quality.lambdaValid = (lambda >= minLambda && lambda <= maxLambda && !std::isnan(lambda));
    
    // Validação de estabilidade de lambda (configurável)
    float lambdaDeviation = fabsf(lambda - targetLambda) / targetLambda;
    quality.conditionsStable = (lambdaDeviation <= maxDeviation);
    
    // Validação de temperatura (configurável)
    quality.tempAppropriate = (clt >= minClt && clt <= maxClt);
    
    // Validação de carga (configurável)
    quality.loadAppropriate = (map >= minMap);
    
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

	return 1;
    
}

void WallFuelController::updateCellConfidence(int i, int j, bool isBeta, float adjustment, const LearningDataQuality& quality) { 
    // Determinar se célula convergiu (configurável)
    status.isConverged = 1;
}

void WallFuelController::checkAndResetDrift() {
    #if WW_ENABLE_DRIFT_RESET
    // Reset simplificado a cada 30 minutos
    if (!lastResetTimer.hasElapsedMs(30 * 60 * 1000)) {
        return;
    }
    
    // Reset simples de células com baixa confiança
    for (int i = 0; i < WW_CORRECTION_MAP_BINS; i++) {
        for (int j = 0; j < WW_CORRECTION_RPM_BINS; j++) {
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
    for (int i = 0; i < WW_CORRECTION_MAP_BINS; i++) {
        for (int j = 0; j < WW_CORRECTION_RPM_BINS; j++) {
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

// Função para calcular tamanho ótimo do buffer baseado na nova estratégia de janelas
int WallFuelController::calculateOptimalBufferSize(float tau, float rpm) {
    if (rpm < 100.0f || tau <= 0.0f) {
        return 80; // Valor mínimo aumentado para capturar melhor a dinâmica
    }
    
    // *** CÁLCULO BASEADO NA NOVA ESTRATÉGIA DE JANELAS ***
    float cycleTimeSeconds = 60.0f / rpm;
    
    // Nova estratégia:
    // - Beta window: 20% do buffer (mín. 10 amostras) para efeitos imediatos
    // - Tau window: 50-100% do buffer para efeitos de evaporação
    // - Buffer total deve acomodar ambas as janelas adequadamente
    
    float timeConstantCycles = tau / cycleTimeSeconds;
    
    // Calcular necessidades:
    // - Beta: mínimo 10 amostras (fixo)
    // - Tau: 3-4 constantes de tempo tau para capturar evaporação completa
    int minBetaSamples = 10;  // Mínimo conforme especificação
    int tauCycles = (int)(timeConstantCycles * 4.0f); // 4x tau para capturar evaporação
    
    // Buffer total deve ser grande o suficiente para:
    // - Beta window (20%) >= 10 amostras
    // - Tau window (50% do buffer) >= tauCycles
    int minBufferForBeta = minBetaSamples * 5;  // 20% = minBetaSamples, então total = 5x
    int minBufferForTau = tauCycles * 2;        // 50% = tauCycles, então total = 2x
    
    int optimalSamples = maxI(minBufferForBeta, minBufferForTau);
    
    // Garantir limites razoáveis
    optimalSamples = clampF(80, optimalSamples, WW_BUFFER_MAX);
    
    return optimalSamples;
}

// Implementação da função de suavização de tabelas de correção
void WallFuelController::smoothCorrectionTable(scaled_channel<uint8_t, 100, 1> table[WW_CORRECTION_MAP_BINS][WW_CORRECTION_RPM_BINS], int centerI, int centerJ, float intensity) {
    // Aplicar suavização simples nas células adjacentes
    // intensity: 0.0 = sem suavização, 1.0 = suavização máxima
    // centerI = MAP index, centerJ = RPM index
    
    if (centerI < 0 || centerI >= WW_CORRECTION_MAP_BINS || centerJ < 0 || centerJ >= WW_CORRECTION_RPM_BINS) {
        return; // Índices inválidos
    }
    
    if (intensity <= 0.0f) {
        return; // Sem suavização
    }
    
    intensity = clampF(0.0f, intensity, 1.0f);
    
    uint8_t centerValue = table[centerI][centerJ]; // Corrected: [i][j] = [MAP][RPM]
    
    // Aplicar suavização para células adjacentes
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue; // Skip center cell
            
            int ni = centerI + di;
            int nj = centerJ + dj;
            
            // Verificar limites
            if (ni >= 0 && ni < WW_CORRECTION_MAP_BINS && nj >= 0 && nj < WW_CORRECTION_RPM_BINS) {
                uint8_t neighborValue = table[ni][nj]; // Corrected: [MAP][RPM]
                
                // Aplicar suavização ponderada baseada na distância
                float weight = intensity;
                if (di != 0 && dj != 0) {
                    weight *= 0.7f; // Diagonal neighbors have less weight
                }
                
                // Interpolação linear entre valor atual e valor central
                uint8_t newValue = (uint8_t)(neighborValue * (1.0f - weight) + centerValue * weight);
                table[ni][nj] = newValue; // Corrected: [MAP][RPM]
            }
        }
    }
}

// Funções de diagnóstico
float WallFuelController::getCellConfidence(int i, int j, bool isBeta) const {
	if (i >= WW_CORRECTION_MAP_BINS || j >= WW_CORRECTION_RPM_BINS) return 0.0f;
	// Convert uint8_t confidence (0-255) back to float (0.0-1.0)
	uint8_t conf = isBeta ? betaLearningStatus[i][j].confidence : tauLearningStatus[i][j].confidence;
	return clampF(0.3f, (float)conf / 255.0f, 1.0f);
}

int WallFuelController::getCellSampleCount(int i, int j, bool isBeta) const {
	if (i >= WW_CORRECTION_MAP_BINS || j >= WW_CORRECTION_RPM_BINS) return 0;
	return isBeta ? betaLearningStatus[i][j].sampleCount : tauLearningStatus[i][j].sampleCount;
}

// Enhanced Transient Detection Implementation
TransientInfo WallFuelController::detectTransientEnhanced(float tps, float map, float rpm) {
	// Note: rpm parameter reserved for future RPM-dependent transient detection
	(void)rpm; // Suppress unused parameter warning
	
	TransientInfo result;
	
	// Get configurable parameters with fallback defaults
	float tpsThreshold = engineConfiguration->wwTpsThreshold > 0 ? engineConfiguration->wwTpsThreshold : 3.0f;
	float mapThreshold = engineConfiguration->wwMapThreshold > 0 ? engineConfiguration->wwMapThreshold : 15.0f;
	uint16_t windowMs = engineConfiguration->wwTransientDetectionWindowMs > 0 ? engineConfiguration->wwTransientDetectionWindowMs : 200;
	
	// Calculate window size in samples
	constexpr float callbackPeriod = FAST_CALLBACK_PERIOD_MS; // in ms
	int windowSamples = (int)(windowMs / callbackPeriod);
	windowSamples = clampF(10, windowSamples, 100); // Reasonable limits
	
	// Static buffers for rate calculation
	static float tpsDetectionBuffer[100] = {0};
	static float mapDetectionBuffer[100] = {0};
	static int detectionBufIdx = 0;
	static bool detectionBufferFilled = false;
	
	// Store current values
	tpsDetectionBuffer[detectionBufIdx] = tps;
	mapDetectionBuffer[detectionBufIdx] = map;
	
	// Calculate rates if buffer has enough data
	if (detectionBufferFilled || detectionBufIdx >= windowSamples) {
		int oldestIdx = (detectionBufIdx - windowSamples + 100) % 100;
		float tpsDelta = tps - tpsDetectionBuffer[oldestIdx];
		float mapDelta = map - mapDetectionBuffer[oldestIdx];
		float timeWindow = windowSamples * callbackPeriod / 1000.0f; // Convert to seconds
		
		result.tpsRate = tpsDelta / timeWindow; // %/s
		result.mapRate = mapDelta / timeWindow; // kPa/s
		
		// Determine direction and intensity
		bool isPositive = result.tpsRate > tpsThreshold || result.mapRate > mapThreshold;
		bool isNegative = result.tpsRate < -tpsThreshold || result.mapRate < -mapThreshold;
		
		if (isPositive) {
			result.direction = TransientDirection::POSITIVE;
			result.intensity = classifyTransientIntensity(result.tpsRate, result.mapRate);
			result.isValid = true;
		} else if (isNegative) {
			result.direction = TransientDirection::NEGATIVE;
			result.intensity = classifyTransientIntensity(-result.tpsRate, -result.mapRate);
			result.isValid = true;
		}
		
		// Apply filtering if enabled
		if (engineConfiguration->wwEnableTransientFiltering && result.isValid) {
			float combinedRate = sqrtf(result.tpsRate * result.tpsRate + result.mapRate * result.mapRate);
			result.isValid = applyTransientFiltering(combinedRate);
		}
	}
	
	// Update buffer index
	detectionBufIdx = (detectionBufIdx + 1) % 100;
	if (detectionBufIdx == 0) detectionBufferFilled = true;
	
	return result;
}

bool WallFuelController::isTransientValid(const TransientInfo& transient) {
	if (!transient.isValid) return false;
	
	// Check timeout between transients
	uint16_t timeoutMs = engineConfiguration->wwTransientTimeoutMs > 0 ? engineConfiguration->wwTransientTimeoutMs : 500;
	if (!m_transientTimer.hasElapsedMs(timeoutMs)) {
		return false;
	}
	
	// Check minimum duration if configured
	float minDuration = engineConfiguration->wwMinTransientDuration > 0 ? engineConfiguration->wwMinTransientDuration : 0;
	if (minDuration > 0) {
		updateTransientDuration();
		if (m_currentTransient.duration < minDuration) {
			return false;
		}
	}
	
	// Reset timer for next transient
	m_transientTimer.reset();
	return true;
}

TransientIntensity WallFuelController::classifyTransientIntensity(float tpsRate, float mapRate) {
	// Get configurable thresholds with fallback defaults
	float lightTps = engineConfiguration->wwTpsThresholdLight > 0 ? engineConfiguration->wwTpsThresholdLight : 2.0f;
	float lightMap = engineConfiguration->wwMapThresholdLight > 0 ? engineConfiguration->wwMapThresholdLight : 8.0f;
	float heavyTps = engineConfiguration->wwTpsThresholdHeavy > 0 ? engineConfiguration->wwTpsThresholdHeavy : 15.0f;
	float heavyMap = engineConfiguration->wwMapThresholdHeavy > 0 ? engineConfiguration->wwMapThresholdHeavy : 60.0f;
	
	// Use absolute values for classification
	float absTpsRate = fabsf(tpsRate);
	float absMapRate = fabsf(mapRate);
	
	// Heavy transient: either TPS or MAP exceeds heavy threshold
	if (absTpsRate > heavyTps || absMapRate > heavyMap) {
		return TransientIntensity::HEAVY;
	}
	
	// Light transient: both TPS and MAP are below normal but above light threshold
	if (absTpsRate <= lightTps && absMapRate <= lightMap) {
		return TransientIntensity::LIGHT;
	}
	
	// Normal transient: everything else
	return TransientIntensity::NORMAL;
}

bool WallFuelController::applyTransientFiltering(float rate) {
	if (!engineConfiguration->wwEnableTransientFiltering) {
		return true; // No filtering
	}
	
	uint8_t filterSamples = engineConfiguration->wwTransientFilterSamples > 0 ? 
							engineConfiguration->wwTransientFilterSamples : 3;
	filterSamples = clampF(1, filterSamples, 10);
	
	// Store rate in filter buffer
	m_transientFilterBuffer[m_filterBufferIdx] = rate;
	m_filterBufferIdx = (m_filterBufferIdx + 1) % 10;
	
	if (m_filterBufferIdx == 0) {
		m_filterBufferFilled = true;
	}
	
	// Need enough samples for filtering
	if (!m_filterBufferFilled && m_filterBufferIdx < filterSamples) {
		return false;
	}
	
	// Check if enough recent samples exceed threshold
	int validSamples = 0;
	int samplesToCheck = m_filterBufferFilled ? filterSamples : m_filterBufferIdx;
	
	for (int i = 0; i < samplesToCheck; i++) {
		int idx = (m_filterBufferIdx - 1 - i + 10) % 10;
		if (m_transientFilterBuffer[idx] > 0) { // Any positive rate
			validSamples++;
		}
	}
	
	// Require at least 60% of samples to be valid
	return (float)validSamples / samplesToCheck >= 0.6f;
}

void WallFuelController::updateTransientDuration() {
	if (m_currentTransient.direction != TransientDirection::NONE) {
		if (!m_transientDurationTimer.hasElapsedMs(0)) {
			// Timer was just started
			m_transientDurationTimer.reset();
			m_currentTransient.duration = 0;
		} else {
			// Update duration (convert from microseconds to milliseconds)
			m_currentTransient.duration = m_transientDurationTimer.getElapsedUs() / 1000.0f;
		}
	} else {
		// No active transient, reset timer
		m_transientDurationTimer.reset();
	}
}
