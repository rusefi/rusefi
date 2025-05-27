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

// *** WALL WETTING ADAPTIVE LEARNING - IMPLEMENTAÇÃO SIMPLIFICADA ***
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



void WallFuelController::adaptiveLearning(float rpm, float map, float lambda, float targetLambda, 
                                        bool isTransient, TransientDirection direction, float clt) {

	// *** VERIFICAR CONDIÇÕES DE HABILITAÇÃO ***
	if (!engineConfiguration->complexWallModel) {
		monitoring = false;
		return; // Wall wetting complexo desabilitado
	}
	
	if (!engineConfiguration->wwEnableAdaptiveLearning) {
		monitoring = false;
		return; // Aprendizado adaptativo desabilitado
	}

	if (clt > engineConfiguration->wwMaxCoolantTemp || clt < engineConfiguration->wwMinCoolantTemp) {
		monitoring = false;
		return;
	}
	
	if (map < engineConfiguration->wwMinMapForLearning) {
		monitoring = false;
		return;
	}

	// *** CORREÇÃO FUNDAMENTAL: CÉLULAS SEPARADAS PARA BETA E TAU ***
	// Célula atual (condições finais - para TAU)
	auto rpmBin = priv::getBin(rpm, config->wwCorrectionRpmBins);
	auto mapBin = priv::getBin(map, config->wwCorrectionMapBins);
	int j_final = rpmBin.Idx;
	int i_final = mapBin.Idx;
	
	// Validar índices finais
	if (i_final >= WW_CORRECTION_MAP_BINS || j_final >= WW_CORRECTION_RPM_BINS) {
		return;
	}
	
	lastTransientDirection = direction;
	
	if (isTransient) {		
		// *** ARMAZENAR CONDIÇÕES INICIAIS DO TRANSIENTE ***
		// Usar valores do buffer para estimar condições antes do transiente
		if (m_transientBuffer.hasEnoughSamples()) {
			// Pegar valores de algumas amostras atrás (antes do transiente)
			float initialMap = m_transientBuffer.getMapSample(5);  // 5 amostras atrás (~25ms)
			float initialRpm = rpm; // RPM muda mais lentamente, usar atual
			
			// Calcular índices das condições iniciais (para BETA)
			auto initialRpmBin = priv::getBin(initialRpm, config->wwCorrectionRpmBins);
			auto initialMapBin = priv::getBin(initialMap, config->wwCorrectionMapBins);
			
			// Armazenar para uso posterior
			transientInitialMapIdx = initialMapBin.Idx;
			transientInitialRpmIdx = initialRpmBin.Idx;
			
			// Validar índices iniciais
			if (transientInitialMapIdx >= WW_CORRECTION_MAP_BINS || transientInitialRpmIdx >= WW_CORRECTION_RPM_BINS) {
				// Se índices inválidos, usar os finais como fallback
				transientInitialMapIdx = i_final;
				transientInitialRpmIdx = j_final;
			}
		} else {
			// Fallback: usar condições atuais se não temos histórico
			transientInitialMapIdx = i_final;
			transientInitialRpmIdx = j_final;
		}
		
		// Calcular tamanho ótimo do buffer
		float tau = computeTau();
		bufferMaxSize = calculateOptimalBufferSize(tau, rpm);
		
		// Sempre reiniciar o monitoramento para novos transientes
		monitoring = true;
		bufferIdx = 0;
		m_monitoringTimeoutTimer.reset(); // Reset timeout timer
	}
	
	// *** TIMEOUT DO MONITORING PARA EVITAR TRAVAMENTO ***
	if (monitoring && m_monitoringTimeoutTimer.hasElapsedMs(2000)) { // 2 segundos timeout
		monitoring = false; // Forçar saída do monitoring se demorar muito
		globalMonitoring = false; // Também desligar monitoramento global
		bufferIdx = 0;
	}
	
	if (monitoring) {
		if (bufferIdx < bufferMaxSize) {
			// Validação simplificada
			bool sampleValid = (lambda >= 0.5f && lambda <= 1.5f);
			
			if (sampleValid) {
				lambdaBuffer[bufferIdx] = lambda;
				rpmBuffer[bufferIdx] = rpm;
				mapBuffer[bufferIdx] = map;
				bufferIdx++;
			}
		} else {
			// Buffer cheio - processar aprendizado
			
			// *** CORREÇÃO: JANELAS BASEADAS NA FÍSICA EM VEZ DE PERCENTUAIS ARBITRÁRIOS ***
			float cycleTimeSeconds = 60.0f / rpm;  // Tempo por ciclo (4-stroke)
			float callbackPeriodSeconds = 0.005f;  // ~5ms por callback
			int samplesPerCycle = (int)(cycleTimeSeconds / callbackPeriodSeconds);
			
			// Beta window: 2 ciclos para efeitos imediatos de impacto na parede
			int betaWindow = samplesPerCycle * 2;
			if (betaWindow < 10) betaWindow = 10;         // Mínimo absoluto
			if (betaWindow > bufferIdx) betaWindow = bufferIdx; // Não exceder buffer
			
			// Tau window: baseado na constante de tempo tau
			float tau = computeTau();
			int tauCycles = (int)(3.0f * tau / cycleTimeSeconds);  // 3x tau para capturar evaporação
			int tauWindowStart = samplesPerCycle * 2;  // Começar após efeitos beta
			int tauWindowEnd = tauWindowStart + (tauCycles * samplesPerCycle);
			if (tauWindowEnd > bufferIdx) tauWindowEnd = bufferIdx;
			if (tauWindowStart >= tauWindowEnd) {
				// Fallback se janela tau inválida
				tauWindowStart = bufferIdx / 2;
				tauWindowEnd = bufferIdx;
			}
			
			// *** CÁLCULO DE ERROS ESPECÍFICOS PARA BETA E TAU ***
			
			// Beta: Erro imediato - média simples das primeiras amostras
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
			
			// Tau: Erro filtrado - média ponderada das amostras tardias
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
			
			// Salvar para diagnóstico
			lastImmediateError = betaError;
			lastProlongedError = tauError;
			
			// *** CORREÇÃO CONCEITUAL CRÍTICA: LÓGICA DE AJUSTE INVERTIDA ***
			float maxStep = 0.03f;   // Reduzido para evitar oscilações
			
			// Usar taxas de aprendizado diretas da configuração
			float betaLearnRate = engineConfiguration->wwBetaLearningRate;
			float tauLearnRate = engineConfiguration->wwTauLearningRate;
			
			// *** CORREÇÃO FÍSICA: Ajustes baseados na física correta do wall wetting ***
			float deltaBeta = 0.0f;
			float deltaTau = 0.0f;
			
			// *** LÓGICA CORRIGIDA - FÍSICA CORRETA ***
			// Princípio: Se lambda está alto (pobre), precisamos REDUZIR o combustível que vai para a parede
			//           Se lambda está baixo (rico), precisamos AUMENTAR o combustível que vai para a parede
			//
			// IMPORTANTE: A correção deve ser OPOSTA ao erro para compensá-lo
			
			if (direction == TransientDirection::POSITIVE) {
				// ACELERAÇÃO: Esperamos lambda diminuir (ficar rico)
				// Se betaError > 0: lambda imediato está alto (pobre) → DIMINUIR beta (menos combustível na parede)
				// Se tauError > 0: lambda prolongado está alto (pobre) → DIMINUIR tau (evaporação mais rápida)
				deltaBeta = -betaLearnRate * betaError;     // *** SINAL NEGATIVO - CORREÇÃO CRÍTICA ***
				deltaTau = -tauLearnRate * tauError;        // *** SINAL NEGATIVO - CORREÇÃO CRÍTICA ***
				
			} else if (direction == TransientDirection::NEGATIVE) {
				// DESACELERAÇÃO: Esperamos lambda aumentar (ficar pobre)
				// Se betaError < 0: lambda imediato está baixo (rico) → AUMENTAR beta (mais combustível na parede)
				// Se tauError < 0: lambda prolongado está baixo (rico) → AUMENTAR tau (evaporação mais lenta)
				deltaBeta = -betaLearnRate * betaError;     // *** SINAL NEGATIVO - CORREÇÃO CRÍTICA ***
				deltaTau = -tauLearnRate * tauError;        // *** SINAL NEGATIVO - CORREÇÃO CRÍTICA ***
				
			} else {
				// TRANSIENTE NEUTRO: Ajuste balanceado
				deltaBeta = -betaLearnRate * betaError * 0.5f;  // *** SINAL NEGATIVO ***
				deltaTau = -tauLearnRate * tauError * 0.5f;     // *** SINAL NEGATIVO ***
			}
			
			// Aplicar limites
			deltaBeta = clampF(-maxStep, deltaBeta, maxStep);
			deltaTau = clampF(-maxStep, deltaTau, maxStep);
			
			// *** CORREÇÃO FUNDAMENTAL: APLICAR CORREÇÕES NAS CÉLULAS CORRETAS ***
			
			// BETA: Aplicar na célula das condições INICIAIS (onde o combustível foi injetado)
			int i_beta = transientInitialMapIdx;
			int j_beta = transientInitialRpmIdx;
			
			// TAU: Aplicar na célula das condições FINAIS (onde a evaporação acontece)
			int i_tau = i_final;
			int j_tau = j_final;
			
			// Ler valores atuais das células corretas
			float currentBetaValue = config->wwBetaCorrection[i_beta][j_beta];
			float currentTauValue = config->wwTauCorrection[i_tau][j_tau];
			
			// *** VALIDAÇÃO DE CONVERGÊNCIA CORRIGIDA PARA EVITAR OSCILAÇÕES ***
			SimpleLearningStatus& betaStatus = betaLearningStatus[i_beta][j_beta];
			SimpleLearningStatus& tauStatus = tauLearningStatus[i_tau][j_tau];
			
			// *** CORREÇÃO: Detecção de oscilação baseada em histórico real ***
			bool betaOscillating = false;
			bool tauOscillating = false;
			
			// Detectar oscilação real: apenas se o erro ainda é grande após várias tentativas
			// Removida a condição de parada após 10 iterações para permitir aprendizado contínuo
			if (betaStatus.sampleCount > 5) {
				// Oscilação = erro persistentemente grande (sem melhoria)
				bool largeError = fabsf(betaError) > 0.05f;  // Erro significativo
				// Remover a condição "manyAttempts" para permitir aprendizado contínuo
				betaOscillating = largeError && (betaStatus.confidence < 50); // Baixa confiança indica oscilação
			}
			
			if (tauStatus.sampleCount > 5) {
				bool largeError = fabsf(tauError) > 0.05f;
				// Remover a condição "manyAttempts" para permitir aprendizado contínuo
				tauOscillating = largeError && (tauStatus.confidence < 50); // Baixa confiança indica oscilação
			}
			
			// Reduzir taxa de aprendizado se detectar oscilação real
			if (betaOscillating) {
				deltaBeta *= 0.3f;  // Redução mais agressiva para oscilações reais
			}
			if (tauOscillating) {
				deltaTau *= 0.3f;   // Redução mais agressiva para oscilações reais
			}
			
			// *** CORREÇÃO: Ajuste direto em vez de multiplicação exponencial ***
			// Agora os deltas são aplicados diretamente aos multiplicadores
			float newBetaValue = currentBetaValue + deltaBeta;
			float newTauValue = currentTauValue + deltaTau;
			
			// Aplicar limites físicos para multiplicadores
			newBetaValue = clampF(0.5f, newBetaValue, 2.0f);  // Faixa razoável para multiplicador
			newTauValue = clampF(0.5f, newTauValue, 2.0f);    // Faixa razoável para multiplicador
			
			// *** VALIDAÇÃO: Só aplicar se a mudança for significativa ***
			bool significantBetaChange = fabsf(deltaBeta) > 0.001f;
			bool significantTauChange = fabsf(deltaTau) > 0.001f;
			
			if (significantBetaChange) {
				config->wwBetaCorrection[i_beta][j_beta] = newBetaValue;
				betaStatus.sampleCount++;
				
				// *** CORREÇÃO: Confiança baseada na qualidade do erro ***
				float errorQuality = 1.0f / (1.0f + fabsf(betaError) * 10.0f);  // 0-1, melhor com erro menor
				float confidenceChange = errorQuality * 20.0f - 5.0f;  // +15 se perfeito, -5 se erro grande
				betaStatus.confidence = (uint8_t)clampF(0, betaStatus.confidence + confidenceChange, 255);
			}
			
			if (significantTauChange) {
				config->wwTauCorrection[i_tau][j_tau] = newTauValue;
				tauStatus.sampleCount++;
				
				// *** CORREÇÃO: Confiança baseada na qualidade do erro ***
				float errorQuality = 1.0f / (1.0f + fabsf(tauError) * 10.0f);
				float confidenceChange = errorQuality * 20.0f - 5.0f;
				tauStatus.confidence = (uint8_t)clampF(0, tauStatus.confidence + confidenceChange, 255);
			}
			
			// *** CORREÇÃO: Critério de convergência mais realista ***
			// Convergido = erro pequeno + confiança razoável + estabilidade
			bool betaSmallError = fabsf(betaError) < 0.08f;  // Mais realista (8% lambda)
			bool betaStable = betaStatus.sampleCount >= 3 && betaStatus.confidence > 100;
			betaStatus.isConverged = betaSmallError && betaStable && !betaOscillating;
			
			bool tauSmallError = fabsf(tauError) < 0.08f;
			bool tauStable = tauStatus.sampleCount >= 3 && tauStatus.confidence > 100;
			tauStatus.isConverged = tauSmallError && tauStable && !tauOscillating;
			
			// Suavização opcional (simplificada) - aplicar nas células corretas
			float smoothIntensity = 0.05f; // Reduzido para ser mais conservador
			smoothCorrectionTable(config->wwBetaCorrection, i_beta, j_beta, smoothIntensity);
			smoothCorrectionTable(config->wwTauCorrection, i_tau, j_tau, smoothIntensity);
			
			monitoring = false;
			globalMonitoring = false; // *** DESLIGAR MONITORAMENTO GLOBAL ***
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
		tauCorr = interpolate3d(config->wwTauCorrection, config->wwCorrectionMapBins, map, config->wwCorrectionRpmBins, rpm);
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
		betaCorr = interpolate3d(config->wwBetaCorrection, config->wwCorrectionMapBins, map, config->wwCorrectionRpmBins, rpm);
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
	
	// *** DETECÇÃO DE TRANSIENTE COM BUFFER CIRCULAR ***
	TransientInfo transient = detectTransientWithBuffer(tps, map);
	bool isTransient = transient.isValid;
	
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
	
	// Calcular tau e beta
	float tau, beta;
	tau = computeTau();
	beta = computeBeta();
	
	if (tau < 0.01f || beta < 0.01f) {
		m_enable = false;
		return;
	}
	
	// *** CORREÇÃO CRÍTICA: FÓRMULA CORRETA DO ALPHA ***
	// Alpha = exp(-dt/tau) onde dt é o tempo entre ciclos de combustão
	// Para motor 4-tempos: dt = 120/rpm segundos por ciclo de combustão
	float cycleTimeSeconds = 120.0f / rpm;  // Tempo entre ciclos de combustão (4-stroke)
	float alpha = expf_taylor(-cycleTimeSeconds / tau);
	
	// Validação física: alpha deve estar entre 0 e 1
	alpha = clampF(0.0f, alpha, 1.0f);
	
	// Constraint física: beta não pode ser maior que alpha
	// (não pode depositar mais combustível do que permanece na parede)
	if (beta > alpha) {
		beta = alpha;
	}
	
	m_alpha = alpha;
	m_beta = beta;
	m_enable = true;
	
	// *** CORREÇÃO: Processar aprendizado durante TODO o período de monitoramento ***
	float lambdaValue = Sensor::getOrZero(SensorType::Lambda1);
	float targetLambda = engine->fuelComputer.targetLambda;
	float clt = Sensor::getOrZero(SensorType::Clt);
	
	// *** GERENCIAMENTO DO ESTADO GLOBAL DE MONITORAMENTO ***
	if (isTransient) {
		// *** CORREÇÃO: Só iniciar novo monitoramento se não estivermos invalidando ***
		// A invalidação já foi tratada dentro de adaptiveLearning()
		// Aqui só iniciamos se globalMonitoring estiver false (não há invalidação)
		if (!globalMonitoring) {
			// Novo transiente detectado - iniciar monitoramento global
			globalMonitoring = true;
			monitoringDirection = m_currentTransient.direction;
			lastTransientDirection = m_currentTransient.direction;
		} else {
			monitoring = false;
			globalMonitoring = false;
			bufferIdx = 0;  // Reset buffer
		}
	}
	
	// Chamar adaptiveLearning sempre que estivermos em monitoramento global
	if (globalMonitoring) {
		adaptiveLearning(rpm, map, lambdaValue, targetLambda, isTransient, monitoringDirection, clt);
	}
}

WallFuelController::WallFuelController() : 
	bufferIdx(0), bufferMaxSize(200), monitoring(false), pendingWwSave(false),
	currentTransientDirection(TransientDirection::NONE), lastTransientDirection(TransientDirection::NONE),
	globalMonitoring(false), monitoringDirection(TransientDirection::NONE),
	lastImmediateError(0.0f), lastProlongedError(0.0f),
	transientInitialMapIdx(0), transientInitialRpmIdx(0) {
	
	// Inicializar timers
	m_transientCooldownTimer.reset();
	m_monitoringTimeoutTimer.reset();
	
	// Inicializar buffers
	for (int i = 0; i < WW_BUFFER_MAX; i++) {
		lambdaBuffer[i] = 1.0f;  // Valor neutro em vez de zero
		rpmBuffer[i] = 0.0f;
		mapBuffer[i] = 0.0f;
	}
	
	// Inicializar status simplificado de aprendizado de todas as células
	for (int i = 0; i < WW_CORRECTION_MAP_BINS; i++) {
		for (int j = 0; j < WW_CORRECTION_RPM_BINS; j++) {
			betaLearningStatus[i][j] = SimpleLearningStatus();
			tauLearningStatus[i][j] = SimpleLearningStatus();
		}
	}
}

// *** NOVA IMPLEMENTAÇÃO: DETECÇÃO DE TRANSIENTE COM BUFFER CIRCULAR ***
TransientInfo WallFuelController::detectTransientWithBuffer(float tps, float map) {
	TransientInfo result;
	
	// Adicionar amostra atual ao buffer circular
	m_transientBuffer.addSample(tps, map);
	
	// Verificar se temos amostras suficientes
	if (!m_transientBuffer.hasEnoughSamples()) {
		return result; // Retorna transiente inválido se não temos dados suficientes
	}
	
	// Verificar cooldown para evitar detecções muito frequentes
	if (!m_transientCooldownTimer.hasElapsedMs(50)) { // 50ms cooldown mínimo
		return result;
	}
	
	// *** CORREÇÃO: Calcular derivadas com conversão correta para unidades por segundo ***
	// Callback é chamado a cada ~5ms, então 1 amostra = 5ms
	const float CALLBACK_PERIOD_MS = 5.0f;
	const float MS_TO_SECONDS = 1000.0f;
	
	// Calcular derivadas em unidades corretas (%/s e kPa/s)
	float tpsDerivativeShort = m_transientBuffer.calculateTpsDerivative(5) * (MS_TO_SECONDS / (5 * CALLBACK_PERIOD_MS));   // %/s
	float mapDerivativeShort = m_transientBuffer.calculateMapDerivative(5) * (MS_TO_SECONDS / (5 * CALLBACK_PERIOD_MS));   // kPa/s
	float tpsDerivativeMedium = m_transientBuffer.calculateTpsDerivative(10) * (MS_TO_SECONDS / (10 * CALLBACK_PERIOD_MS)); // %/s
	float mapDerivativeMedium = m_transientBuffer.calculateMapDerivative(10) * (MS_TO_SECONDS / (10 * CALLBACK_PERIOD_MS)); // kPa/s
	float tpsDerivativeLong = m_transientBuffer.calculateTpsDerivative(20) * (MS_TO_SECONDS / (20 * CALLBACK_PERIOD_MS));   // %/s
	float mapDerivativeLong = m_transientBuffer.calculateMapDerivative(20) * (MS_TO_SECONDS / (20 * CALLBACK_PERIOD_MS));   // kPa/s
	
	// Thresholds da configuração (já em unidades corretas)
	float tpsThreshold = engineConfiguration->wwTpsThreshold;  // %/s
	float mapThreshold = engineConfiguration->wwMapThreshold;  // kPa/s
	
	// *** LÓGICA DE DETECÇÃO MELHORADA ***
	// Usar múltiplas janelas para detectar transientes de diferentes velocidades
	
	// Transiente rápido (janela curta)
	bool fastPositive = (tpsDerivativeShort > tpsThreshold * 2.0f) || (mapDerivativeShort > mapThreshold * 2.0f);
	bool fastNegative = (tpsDerivativeShort < -tpsThreshold * 2.0f) || (mapDerivativeShort < -mapThreshold * 2.0f);
	
	// Transiente médio (janela média)
	bool mediumPositive = (tpsDerivativeMedium > tpsThreshold) || (mapDerivativeMedium > mapThreshold);
	bool mediumNegative = (tpsDerivativeMedium < -tpsThreshold) || (mapDerivativeMedium < -mapThreshold);
	
	// Transiente lento (janela longa) - mais sensível
	bool slowPositive = (tpsDerivativeLong > tpsThreshold * 0.5f) || (mapDerivativeLong > mapThreshold * 0.5f);
	bool slowNegative = (tpsDerivativeLong < -tpsThreshold * 0.5f) || (mapDerivativeLong < -mapThreshold * 0.5f);
	
	// Combinar detecções - qualquer janela pode detectar um transiente
	bool isPositive = fastPositive || mediumPositive || slowPositive;
	bool isNegative = fastNegative || mediumNegative || slowNegative;
	
	// Evitar detecção simultânea de positivo e negativo
	if (isPositive && isNegative) {
		// Se ambos são detectados, usar a janela média como desempate
		if (fabsf(tpsDerivativeMedium) > fabsf(mapDerivativeMedium)) {
			isPositive = tpsDerivativeMedium > 0;
			isNegative = tpsDerivativeMedium < 0;
		} else {
			isPositive = mapDerivativeMedium > 0;
			isNegative = mapDerivativeMedium < 0;
		}
	}
	
	if (isPositive) {
		result.direction = TransientDirection::POSITIVE;
		result.isValid = true;
		result.tpsRate = tpsDerivativeMedium; // Usar janela média para reportar
		result.mapRate = mapDerivativeMedium;
		m_transientCooldownTimer.reset(); // Reset cooldown
	} else if (isNegative) {
		result.direction = TransientDirection::NEGATIVE;
		result.isValid = true;
		result.tpsRate = tpsDerivativeMedium; // Usar janela média para reportar
		result.mapRate = mapDerivativeMedium;
		m_transientCooldownTimer.reset(); // Reset cooldown
	}
	
	return result;
}

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

// *** CORREÇÃO: Implementação correta da função de suavização para scaled_channel ***
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
    
    // *** CORREÇÃO: Trabalhar com valores float para scaled_channel ***
    float centerValue = table[centerI][centerJ]; // scaled_channel converte automaticamente
    
    // Aplicar suavização para células adjacentes
    for (int di = -1; di <= 1; di++) {
        for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue; // Skip center cell
            
            int ni = centerI + di;
            int nj = centerJ + dj;
            
            // Verificar limites
            if (ni >= 0 && ni < WW_CORRECTION_MAP_BINS && nj >= 0 && nj < WW_CORRECTION_RPM_BINS) {
                float neighborValue = table[ni][nj]; // scaled_channel converte automaticamente
                
                // Aplicar suavização ponderada baseada na distância
                float weight = intensity;
                if (di != 0 && dj != 0) {
                    weight *= 0.7f; // Diagonal neighbors have less weight
                }
                
                // Interpolação linear entre valor atual e valor central
                float newValue = neighborValue * (1.0f - weight) + centerValue * weight;
                
                // *** CORREÇÃO: Aplicar limites físicos antes de atribuir ***
                newValue = clampF(0.5f, newValue, 2.0f);
                
                table[ni][nj] = newValue; // scaled_channel converte automaticamente para uint8_t
            }
        }
    }
}
