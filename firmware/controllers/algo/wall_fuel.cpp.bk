/*
 * @file wall_fuel.cpp
 *
 * @author Matthew Kennedy
 * 
 * IMPLEMENTAÇÃO WALL WETTING ADAPTATIVO - CORRIGIDA SEGUINDO ARTIGO 2001
 * Baseado rigorosamente em:
 * - SAE 810494 (Aquino) - Modelo físico 
 * - "On-Line Adaptation of Wall-Wetting Model Parameters" (2001) - RLS Eqs. 10-22
 * 
 * PRINCIPAIS CORREÇÕES:
 * 1. RLS baseado em regressor físico real do modelo Aquino
 * 2. Sincronização com atrasos de transporte/combustão
 * 3. Detecção focada em excursões de A/F (lambda error)
 */

#include "pch.h"
#include "wall_fuel.h"
#include "engine_math.h"
#include "efitime.h"
#include "fuel_math.h"
#include <rusefi/interpolation.h>

// *** MODELO FÍSICO AQUINO - SEM ALTERAÇÕES ***

void WallFuel::resetWF() {
	wallFuel = 0;
}

float WallFuel::adjust(float desiredMassGrams) {
	invocationCounter++;
	if (std::isnan(desiredMassGrams)) {
		return desiredMassGrams;
	}

	ScopePerf perf(PE::WallFuelAdjust);

	// Se desabilitado, passar valor direto
	if (!engine->module<WallFuelController>()->getEnable()) {
		return desiredMassGrams;
	}

	float alpha = engine->module<WallFuelController>()->getAlpha();
	float beta = engine->module<WallFuelController>()->getBeta();

	float fuelFilmMass = wallFuel;
	float M_cmd = (desiredMassGrams - (1 - alpha) * fuelFilmMass) / (1 - beta);

	// Não podemos injetar quantidade negativa
	if (M_cmd <= 0) {
		M_cmd = 0;
	}

	// Atualizar massa do filme: sobra da última vez + nova desta vez
	float fuelFilmMassNext = alpha * fuelFilmMass + beta * M_cmd;

	wallFuel = fuelFilmMassNext;
	wallFuelCorrection = M_cmd - desiredMassGrams;
	return M_cmd;
}

float WallFuel::getWallFuel() const {
	return wallFuel;
}

// *** RLS FÍSICO CORRETO - BASEADO NO MODELO AQUINO ***

void PhysicalRLSAdapter::update(float lambdaError, const InjectionConditions& injection, 
								float currentRpm, float currentMap) {
	if (!initialized || !injection.valid) {
		return;
	}
	
	// *** REGRESSOR FÍSICO BASEADO NO MODELO AQUINO ***
	// Da equação: M_cyl = M_cmd * (1-β) + (1-α) * X_prev
	// Se β ou τ(alpha) estão incorretos → erro sistemático em M_cyl → erro em lambda
	//
	// Regressão: lambda_error = θ₁ * ∂M_cyl/∂β + θ₂ * ∂M_cyl/∂τ + noise
	// onde θ₁, θ₂ são os erros nos parâmetros
	
	float M_cmd = injection.injectedMass;  // Massa comandada na injeção
	float X_prev = injection.wallFuelMass; // Massa do filme antes da injeção
	
	// Calcular alpha atual (baseado em tau correto)
	float tau = computeTau(injection.rpm, injection.map, injection.clt);
	float cycleTime = 60.0f / injection.rpm;  // Tempo entre ciclos
	float alpha_actual = expf_taylor(-cycleTime / tau);
	
	// *** SENSIBILIDADES FÍSICAS REAIS ***
	// ∂M_cyl/∂β = -M_cmd (menos combustível vai pro cilindro se β aumenta)
	// ∂M_cyl/∂α = X_prev (mais evaporação se α diminui)
	float sens_beta = -M_cmd;
	float sens_alpha = X_prev; 
	
	// Como adaptamos multiplicadores de τ, não α diretamente:
	// ∂α/∂τ_mult = ∂α/∂τ * ∂τ/∂τ_mult = (∂α/∂τ) * τ_base
	float dadtau = alpha_actual * cycleTime / (tau * tau); // ∂α/∂τ
	float sens_tau_mult = sens_alpha * dadtau * tau;  // ∂M_cyl/∂τ_mult
	
	// Normalizar sensibilidades para melhor condicionamento
	float norm_factor = 1.0f / (fabsf(sens_beta) + fabsf(sens_tau_mult) + 1e-6f);
	sens_beta *= norm_factor;
	sens_tau_mult *= norm_factor;
	
	// Regressor phi = [sens_beta, sens_tau_mult]
	float phi[PARAM_COUNT] = {sens_beta, sens_tau_mult};
	
	// *** ALGORITMO RLS CLÁSSICO (Eqs. 10-22 do artigo) ***
	// Predição: y_pred = phi^T * theta
	float prediction = phi[0] * theta[0] + phi[1] * theta[1];
	
	// Erro de predição
	float predictionError = lambdaError - prediction;
	
	// Vetor K = P * phi / (lambda + phi^T * P * phi)
	float Pphi[PARAM_COUNT];
	for (int i = 0; i < PARAM_COUNT; i++) {
		Pphi[i] = 0;
		for (int j = 0; j < PARAM_COUNT; j++) {
			Pphi[i] += P[i][j] * phi[j];
		}
	}
	
	float denominator = forgettingFactor;
	for (int i = 0; i < PARAM_COUNT; i++) {
		denominator += phi[i] * Pphi[i];
	}
	
	if (denominator < 1e-6f) {
		return; // Evitar divisão por zero
	}
	
	float K[PARAM_COUNT];
	for (int i = 0; i < PARAM_COUNT; i++) {
		K[i] = Pphi[i] / denominator;
	}
	
	// Atualizar parâmetros: theta = theta + K * prediction_error
	for (int i = 0; i < PARAM_COUNT; i++) {
		theta[i] += K[i] * predictionError;
		
		// Aplicar limites físicos aos multiplicadores
		theta[i] = clampF(0.5f, theta[i], 2.0f);
	}
	
	// Atualizar matriz P: P = (P - K * phi^T * P) / lambda
	float temp[PARAM_COUNT][PARAM_COUNT];
	for (int i = 0; i < PARAM_COUNT; i++) {
		for (int j = 0; j < PARAM_COUNT; j++) {
			temp[i][j] = P[i][j];
			for (int k = 0; k < PARAM_COUNT; k++) {
				temp[i][j] -= K[i] * phi[k] * P[k][j];
			}
			temp[i][j] /= forgettingFactor;
		}
	}
	
	// Copiar resultado de volta
	for (int i = 0; i < PARAM_COUNT; i++) {
		for (int j = 0; j < PARAM_COUNT; j++) {
			P[i][j] = temp[i][j];
		}
	}
	
	sampleCount++;
}

// *** SISTEMA DE SINCRONIZAÇÃO COM ATRASOS ***

void SynchronizedWallWettingAdapter::onFuelInjection(float rpm, float map, float clt, float injectedMass) {
	// *** CAPTURAR CONDIÇÕES EXATAS DA INJEÇÃO ***
	InjectionConditions injection;
	injection.rpm = rpm;
	injection.map = map;
	injection.clt = clt;
	injection.injectedMass = injectedMass;
	injection.wallFuelMass = getCurrentWallFuelMass();
	injection.timestamp = getTimeNowUs();
	injection.valid = true;
	
	// *** CÁLCULO ROBUSTO DO ATRASO BASEADO NA FÍSICA DO MOTOR ***
	// Modelo físico completo considerando todos os fatores relevantes
	float cycleTime = 60.0f / rpm; // segundos por ciclo
	
	// COMPONENTE 1: Atraso de admissão + compressão (função do RPM)
	// Em baixo RPM: mais tempo para admissão/compressão
	// Em alto RPM: processo mais rápido mas turbulência aumenta
	float admissionDelay = cycleTime * (0.4f + 20.0f / rpm); // 0.4-0.6 ciclos típico
	
	// COMPONENTE 2: Atraso de combustão (função de MAP e RPM)
	// MAP alto = combustão mais rápida (mais pressão)
	// RPM alto = combustão mais rápida (mais turbulência)
	float combustionDelay = cycleTime * (0.15f + 5.0f / sqrtf(map * rpm / 1000.0f));
	
	// COMPONENTE 3: Atraso de escape (função do RPM e geometria)
	// Baseado na velocidade dos gases e volume do escape
	// Fórmula empírica: tempo = volume_escape / velocidade_gases
	float exhaustDelay = engineConfiguration->wwLambdaDelayFactor * (80.0f + 1500.0f / rpm) / 1000.0f; // 80-200ms típico
	
	// COMPONENTE 4: Atraso do sensor lambda (constante)
	float sensorDelay = 0.020f; // 20ms
	
	// MODELO ROBUSTO FINAL: soma ponderada com limites físicos
	float transportDelay = admissionDelay + combustionDelay + exhaustDelay + sensorDelay;
	
	// Aplicar limites físicos realistas para robustez
	transportDelay = clampF(0.050f, transportDelay, 0.500f); // 50ms-500ms
	
	uint32_t delayUs = (uint32_t)(transportDelay * 1e6f);
	
	// Adicionar na fila com timestamp futuro
	injection.observationTime = injection.timestamp + delayUs;
	
	// Adicionar na fila circular (automaticamente gerencia o tamanho)
	injectionQueue.add(injection);
}

void SynchronizedWallWettingAdapter::onLambdaObservation(float lambdaError, float currentRpm, float currentMap) {
	if (!engineConfiguration->wwEnableAdaptiveLearning) {
		return;
	}
	
	// *** VERIFICAR SE ERRO É SIGNIFICATIVO (FOCO EM EXCURSÕES A/F) ***
	if (fabsf(lambdaError) < LAMBDA_ERROR_THRESHOLD) {
		return; // Erro muito pequeno, não vale adaptar
	}
	
	uint32_t currentTime = getTimeNowUs();
	
	// *** BUSCAR INJEÇÃO CORRESPONDENTE (SINCRONIZAÇÃO TEMPORAL) ***
	InjectionConditions* matchingInjection = nullptr;
	
	// Iterar sobre o buffer circular usando índices
	int queueSize = injectionQueue.getCount();
	for (int i = 0; i < queueSize && i < injectionQueue.getSize(); i++) {
		InjectionConditions& inj = injectionQueue.elements[i];
		if (!inj.valid) continue;
		
		// Verificar se chegou a hora desta injeção ser observada
		if (currentTime >= inj.observationTime) {
			// Janela de tempo válida (±100ms para compensar variações)
			uint32_t timeDiff = currentTime - inj.observationTime;
			if (timeDiff < 200000) { // 100ms
				matchingInjection = &inj;
				break;
			}
		}
	}
	
	if (!matchingInjection) {
		return; // Nenhuma injeção correspondente encontrada
	}
	
	// *** APLICAR RLS COM INJEÇÃO SINCRONIZADA ***
	physicalRLS.update(lambdaError, *matchingInjection, currentRpm, currentMap);
	
	// Marcar injeção como processada
	matchingInjection->valid = false;
	
	// *** VERIFICAR SE DEVE APLICAR CORREÇÃO ***
	if (physicalRLS.getSampleCount() >= MIN_SAMPLES_FOR_CORRECTION) {
		if (shouldApplyCorrection(*matchingInjection)) {
			applyPhysicalCorrection(*matchingInjection);
			physicalRLS.reset(); // Reset para nova adaptação
		}
	}
}

bool SynchronizedWallWettingAdapter::shouldApplyCorrection(const InjectionConditions& injection) {
	// *** CRITÉRIOS MENOS RESTRITIVOS PARA PERMITIR MAIS CORREÇÕES ***
	
	// 1. Erro deve ser consistente (reduzido de 3% para 1.5%)
	float avgError = physicalRLS.getAverageError();
	if (fabsf(avgError) < 0.015f) { // 1.5% threshold (era 3%)
		return false;
	}
	
	// 2. Condições devem ser estáveis durante coleta
	if (!areConditionsStable(injection)) {
		return false;
	}
	
	// *** 3. VALIDAÇÃO DE TRANSIENTE MAIS PERMISSIVA ***
	// Verificar se houve variação significativa de pressão no coletor
	float mapVariation = 0.0f;
	float minMap = injection.map;
	float maxMap = injection.map;
	
	// Calcular variação de MAP nas últimas injeções
	int queueSize = injectionQueue.getCount();
	for (int i = 0; i < queueSize && i < injectionQueue.getSize(); i++) {
		const InjectionConditions& inj = injectionQueue.elements[i];
		if (inj.valid) {
			minMap = minF(minMap, inj.map);
			maxMap = maxF(maxMap, inj.map);
		}
	}
	mapVariation = maxMap - minMap;
	
	// Threshold reduzido: mínimo 2 kPa de variação (era 5 kPa)
	const float MAP_VARIATION_THRESHOLD = 2.0f; // kPa (mais permissivo)
	if (mapVariation < MAP_VARIATION_THRESHOLD) {
		return false; // Não é transiente real
	}
	
	// DEBUG: Log para verificar critérios
	efiPrintf("WW: shouldApply avgErr=%.3f mapVar=%.1f samples=%d", 
			 avgError, mapVariation, physicalRLS.getSampleCount());
	
	return true;
}

void SynchronizedWallWettingAdapter::applyPhysicalCorrection(const InjectionConditions& injection) {
	// *** APLICAR NA CÉLULA FISICAMENTE CORRETA ***
	auto rpmBin = priv::getBin(injection.rpm, config->wwCorrectionRpmBins);
	auto mapBin = priv::getBin(injection.map, config->wwCorrectionMapBins);
	
	if (rpmBin.Idx >= WW_CORRECTION_RPM_BINS || mapBin.Idx >= WW_CORRECTION_MAP_BINS) {
		return;
	}
	
	// BETA: Aplicar na célula da injeção (condições físicas do impacto)
	float betaCorrection = physicalRLS.getBetaCorrection();
	// float oldBeta = config->wwBetaCorrection[mapBin.Idx][rpmBin.Idx];
	config->wwBetaCorrection[mapBin.Idx][rpmBin.Idx] = config->wwBetaCorrection[mapBin.Idx][rpmBin.Idx] * betaCorrection;
	config->wwBetaCorrection[mapBin.Idx][rpmBin.Idx] = 
		clampF(0.5f, config->wwBetaCorrection[mapBin.Idx][rpmBin.Idx], 2.0f);
	
	// TAU: Aplicar na célula da injeção (simplificação - na prática poderia usar célula térmica atual)
	float tauCorrection = physicalRLS.getTauCorrection();
	// float oldTau = config->wwTauCorrection[mapBin.Idx][rpmBin.Idx];
	config->wwTauCorrection[mapBin.Idx][rpmBin.Idx] = config->wwTauCorrection[mapBin.Idx][rpmBin.Idx] * tauCorrection;
	config->wwTauCorrection[mapBin.Idx][rpmBin.Idx] = 
		clampF(0.5f, config->wwTauCorrection[mapBin.Idx][rpmBin.Idx], 2.0f);
	
	// *** APLICAR SUAVIZAÇÃO NAS CÉLULAS ADJACENTES ***
	applySmoothingCorrection(mapBin.Idx, rpmBin.Idx, betaCorrection, tauCorrection);
}

void SynchronizedWallWettingAdapter::applySmoothingCorrection(int centerMapIdx, int centerRpmIdx, 
															 float betaCorrection, float tauCorrection) {
	// *** FUNÇÃO DE SUAVIZAÇÃO PARA EVITAR BURACOS NO MAPA ***
	// Aplica correções graduais nas células adjacentes com fatores de diminuição
	// baseados na distância física da célula central
	
	// Fatores de suavização baseados na distância
	// Distância 1: 60% da correção
	// Distância 2: 30% da correção  
	// Distância 3+: 10% da correção
	const float SMOOTH_FACTOR_DIST1 = 0.60f;  // Células imediatamente adjacentes
	const float SMOOTH_FACTOR_DIST2 = 0.30f;  // Células a distância 2
	const float SMOOTH_FACTOR_DIST3 = 0.10f;  // Células a distância 3+
	
	// Raio máximo de suavização (2 células em cada direção)
	const int MAX_SMOOTH_RADIUS = 2;
	
	// Iterar sobre todas as células dentro do raio de suavização
	for (int mapOffset = -MAX_SMOOTH_RADIUS; mapOffset <= MAX_SMOOTH_RADIUS; mapOffset++) {
		for (int rpmOffset = -MAX_SMOOTH_RADIUS; rpmOffset <= MAX_SMOOTH_RADIUS; rpmOffset++) {
			// Pular a célula central (já foi corrigida)
			if (mapOffset == 0 && rpmOffset == 0) {
				continue;
			}
			
			// Calcular índices da célula adjacente
			int adjMapIdx = centerMapIdx + mapOffset;
			int adjRpmIdx = centerRpmIdx + rpmOffset;
			
			// Verificar limites da tabela
			if (adjMapIdx < 0 || adjMapIdx >= WW_CORRECTION_MAP_BINS ||
				adjRpmIdx < 0 || adjRpmIdx >= WW_CORRECTION_RPM_BINS) {
				continue;
			}
			
			// Calcular distância Manhattan (mais apropriada para tabelas discretas)
			int distance = abs(mapOffset) + abs(rpmOffset);
			
			// Determinar fator de suavização baseado na distância
			float smoothFactor;
			if (distance == 1) {
				smoothFactor = SMOOTH_FACTOR_DIST1;
			} else if (distance == 2) {
				smoothFactor = SMOOTH_FACTOR_DIST2;
			} else {
				smoothFactor = SMOOTH_FACTOR_DIST3;
			}
			
			// *** APLICAR CORREÇÃO SUAVIZADA PARA BETA ***
			// Calcular correção suavizada: 1.0 + smoothFactor * (correction - 1.0)
			// Isso garante que a correção seja gradualmente reduzida com a distância
			float smoothedBetaCorrection = 1.0f + smoothFactor * (betaCorrection - 1.0f);
			
			// Aplicar correção suavizada
			config->wwBetaCorrection[adjMapIdx][adjRpmIdx] = config->wwBetaCorrection[adjMapIdx][adjRpmIdx] * smoothedBetaCorrection;
			config->wwBetaCorrection[adjMapIdx][adjRpmIdx] = 
				clampF(0.5f, config->wwBetaCorrection[adjMapIdx][adjRpmIdx], 2.0f);
			
			// *** APLICAR CORREÇÃO SUAVIZADA PARA TAU ***
			float smoothedTauCorrection = 1.0f + smoothFactor * (tauCorrection - 1.0f);
			
			// Aplicar correção suavizada
			config->wwTauCorrection[adjMapIdx][adjRpmIdx] = config->wwTauCorrection[adjMapIdx][adjRpmIdx] * smoothedTauCorrection;
			config->wwTauCorrection[adjMapIdx][adjRpmIdx] = 
				clampF(0.5f, config->wwTauCorrection[adjMapIdx][adjRpmIdx], 2.0f);
		}
	}
}

// *** CONTROLADOR PRINCIPAL - MODIFICAÇÕES MÍNIMAS ***

WallFuelController::WallFuelController() : 
	m_enable(false), m_alpha(0), m_beta(0), ignitionState(false) {
}

void WallFuelController::onFastCallback() {
	// Verificações básicas - SEM ALTERAÇÕES
	if (engine->rpmCalculator.isCranking()) {
		m_enable = false;
		return;
	}
	
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	if (rpm < 100) {
		m_enable = false;
		return;
	}
	
	// Cálculo físico - SEM ALTERAÇÕES
	float tau = computeTau();
	float beta = computeBeta();
	
	if (tau < 0.01f || beta < 0.01f) {
		m_enable = false;
		return;
	}
	
	// Fórmula correta do alpha - SEM ALTERAÇÕES
	float cycleTimeSeconds = 60.0f / rpm;
	float alpha = expf_taylor(-cycleTimeSeconds / tau);
	
	alpha = clampF(0.0f, alpha, 1.0f);
	
	if (beta > alpha) {
		beta = alpha;
	}
	
	m_alpha = alpha;
	m_beta = beta;
	m_enable = true;
	
	// *** SISTEMA ADAPTATIVO CORRIGIDO ***
	if (engineConfiguration->wwEnableAdaptiveLearning) {
		float map = Sensor::getOrZero(SensorType::Map);
		float clt = Sensor::getOrZero(SensorType::Clt);
		
		// *** VERIFICAÇÃO DE TEMPERATURA MÍNIMA RESTAURADA ***
		if (clt < engineConfiguration->wwMinCoolantTemp) {
			// Não executar adaptação em temperaturas muito baixas
			return;
		}
		
		// *** CORREÇÃO CRÍTICA: SEPARAR LÓGICA DE INJEÇÃO E OBSERVAÇÃO ***
		// Processar observações lambda (sempre que há dados válidos)
		float lambda = Sensor::getOrZero(SensorType::Lambda1);
		float targetLambda = engine->fuelComputer.targetLambda;
		
		// Verificar se temos dados válidos de lambda
		if (lambda > 0.5f && lambda < 1.5f && targetLambda > 0.5f && targetLambda < 1.5f) {
			float lambdaError = lambda - targetLambda;
			
			// DEBUG: Log para verificar se está funcionando
			if (fabsf(lambdaError) > 0.01f) { // Log apenas erros > 1%
				efiPrintf("WW: lambda=%.3f target=%.3f error=%.3f samples=%d", 
						 lambda, targetLambda, lambdaError, synchronizedAdapter.getSampleCount());
			}
			
			synchronizedAdapter.onLambdaObservation(lambdaError, rpm, map);
		}
		
		// *** NOTA: onFuelInjection será chamado separadamente quando houver injeção real ***
		// Esta função deve ser integrada no sistema de injeção, não aqui no onFastCallback
	}
}

float WallFuelController::computeTau() const {
	if (!engineConfiguration->complexWallModel) {
		return engineConfiguration->wwaeTau;
	}
	
	float clt = Sensor::get(SensorType::Clt).value_or(90);
	float tauClt = interpolate2d(clt, config->wwCltBins, config->wwTauCltValues);
	float tauBase = tauClt;
	
	float map = Sensor::get(SensorType::Map).value_or(60);
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	
	if (Sensor::hasSensor(SensorType::Map)) {
		tauBase *= interpolate3d(config->wwTauMapRpmValues, config->wwMapBins, map, config->wwRpmBins, rpm);
		
		// Aplicar correção da tabela de adaptação
		float tauCorr = interpolate3d(config->wwTauCorrection, config->wwCorrectionMapBins, map, config->wwCorrectionRpmBins, rpm);
		tauBase *= tauCorr;
	}
	
	return tauBase;
}

float WallFuelController::computeBeta() const {
	if (!engineConfiguration->complexWallModel) {
		return engineConfiguration->wwaeBeta;
	}
	
	float clt = Sensor::get(SensorType::Clt).value_or(90);
	float betaClt = interpolate2d(clt, config->wwCltBins, config->wwBetaCltValues);
	float betaBase = betaClt;
	
	float map = Sensor::get(SensorType::Map).value_or(60);
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	
	if (Sensor::hasSensor(SensorType::Map)) {
		betaBase *= interpolate3d(config->wwBetaMapRpmValues, config->wwMapBins, map, config->wwRpmBins, rpm);
		
		// Aplicar correção da tabela de adaptação
		float betaCorr = interpolate3d(config->wwBetaCorrection, config->wwCorrectionMapBins, map, config->wwCorrectionRpmBins, rpm);
		betaBase *= betaCorr;
	}
	
	return clampF(0, betaBase, 1);
}

// *** MÉTODO onIgnitionStateChanged RESTAURADO ***
void WallFuelController::onIgnitionStateChanged(bool ignitionOn) {
	ignitionState = ignitionOn;
	
	if (ignitionOn) {
		// Reset timer quando ignição liga
		ignitionOffTimer.reset();
		
		// Resetar adaptador para novo ciclo
		synchronizedAdapter.reset();
	} else {
		// Quando ignição desliga, resetar timer para contar tempo
		ignitionOffTimer.reset();

		// *** SALVAMENTO AUTOMÁTICO RESTAURADO ***
		setNeedToWriteConfiguration();
    }
}

// *** NOVA FUNÇÃO PARA INTEGRAÇÃO COM SISTEMA DE INJEÇÃO ***
void WallFuelController::onActualFuelInjection(float injectedMass, int cylinderIndex) {
	if (!engineConfiguration->wwEnableAdaptiveLearning || !m_enable) {
		return;
	}
	
	float rpm = Sensor::getOrZero(SensorType::Rpm);
	float map = Sensor::getOrZero(SensorType::Map);
	float clt = Sensor::getOrZero(SensorType::Clt);
	
	// Verificar condições mínimas
	if (rpm < 100 || clt < engineConfiguration->wwMinCoolantTemp) {
		return;
	}
	
	// DEBUG: Log injeções para verificar se está sendo chamado
	efiPrintf("WW: Injection cyl=%d mass=%.3f rpm=%.0f map=%.1f", 
			 cylinderIndex, injectedMass, rpm, map);
	
	synchronizedAdapter.onFuelInjection(rpm, map, clt, injectedMass);
}

// *** IMPLEMENTAÇÕES DAS FUNÇÕES AUXILIARES NECESSÁRIAS ***

float PhysicalRLSAdapter::computeTau(float rpm, float map, float clt) const {
	// Simplificação para acesso às tabelas globais - em implementação real seria injetado
	if (!engineConfiguration->complexWallModel) {
		return engineConfiguration->wwaeTau;
	}
	
	float tauClt = interpolate2d(clt, config->wwCltBins, config->wwTauCltValues);
	float tauBase = tauClt;
	
	if (Sensor::hasSensor(SensorType::Map)) {
		tauBase *= interpolate3d(config->wwTauMapRpmValues, config->wwMapBins, map, config->wwRpmBins, rpm);
	}
	
	return tauBase;
}

float SynchronizedWallWettingAdapter::getCurrentWallFuelMass() const {
	// *** CORREÇÃO: USAR A ESTRUTURA CORRETA DO ENGINE ***
	// engine->wallFuel[i] NÃO EXISTE!
	// O correto é: engine->injectionEvents.elements[i].getWallFuel()
	
	float totalWallFuel = 0.0f;
	int activeCylinders = 0;
	
	// Iterar por todos os cilindros configurados
	for (int i = 0; i < engineConfiguration->cylindersCount; i++) {
		float cylinderWallFuel = engine->injectionEvents.elements[i].getWallFuel().getWallFuel();
		if (cylinderWallFuel > 0.001f) { // Considerar apenas cilindros com combustível significativo
			totalWallFuel += cylinderWallFuel;
			activeCylinders++;
		}
	}
	
	// Retornar média ou valor do primeiro cilindro como fallback
	if (activeCylinders > 0) {
		return totalWallFuel / activeCylinders; // Média dos cilindros ativos
	} else {
		return engine->injectionEvents.elements[0].getWallFuel().getWallFuel(); // Fallback para cilindro 0
	}
}

bool SynchronizedWallWettingAdapter::areConditionsStable(const InjectionConditions& injection) const {
	// Verificação simplificada de estabilidade
	// Em implementação mais robusta poderia verificar variância das condições
	return injection.valid; // Por enquanto, consideramos sempre estável se válido
}

float WallFuelController::getLastInjectedMass() const {
	// Implementação simplificada - retornar estimativa baseada no último pulso
	// Em implementação real seria capturado do sistema de injeção
	float baseFuel = engine->fuelComputer.running.baseFuel;
	float correction = engine->fuelComputer.totalFuelCorrection;
	return baseFuel * correction * getNumberOfInjections(engineConfiguration->injectionMode) / 1000.0f; // Convert mg to g
}
