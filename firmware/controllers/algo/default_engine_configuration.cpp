//	setRpmBin(config->egrRpmBins, PACK_RPM_MULT);

	// Wall Wetting
	setLinearCurve(config->wwCltBins, -40, 15, CONFIG(wwCltBins));
	setArrayValues(config->wwTauCltValues, 100);
	setArrayValues(config->wwBetaCltValues, 60);

	setLinearCurve(config->wwMapBins, 20, 20, CONFIG(wwMapBins));
	setLinearCurve(config->wwRpmBins, 600, 600, CONFIG(wwRpmBins));
	
	// Initialize wall wetting 3D tables with default values
	for (int rpm = 0; rpm < CONFIG(wwRpmBins); rpm++) {
		for (int load = 0; load < CONFIG(wwMapBins); load++) {
			config->wwTauMapRpmValues[rpm][load] = 100;
			config->wwBetaMapRpmValues[rpm][load] = 60;
			
			// Inicializar novas tabelas direcionais com valor padrão 100 (1.0 após escalonamento)
			config->wwBetaAccel[rpm][load] = 100;
			config->wwBetaDecel[rpm][load] = 100;
			config->wwTauAccel[rpm][load] = 100;
			config->wwTauDecel[rpm][load] = 100;
		}
	}
	
	// Inicializar configurações para correções direcionais
	config->wwBufferSize = 400;
	config->wwMinSampleSize = 100;
	config->wwMaxSampleSize = 400;
	config->wwBufferSize = 400;
	config->wwTransientTimeoutMs = 1000;
	config->wwDirectionalCorrections = false; // Desabilitado por padrão
	config->wwSampleMultiplier = 1.5f;
	
	// Wall Wetting Learning Parameters
	config->wwMinClt = 70;
	config->wwLearningRate = 0.01f; // 1%
	config->wwSmoothIntensity = 0.1f; // 10%
	config->wwIgnitionOffSaveDelay = 10; // 10 segundos
	
	// Phase weight parameters (default values)
	config->wwBetaInitWeight = 0.2f;
	config->wwBetaTransWeight = 0.3f; 
	config->wwBetaFinalWeight = 0.5f;
	config->wwTauInitWeight = 0.2f;
	config->wwTauTransWeight = 0.3f;
	config->wwTauFinalWeight = 0.5f;
	
	// New Phase 1 and 2 parameters
	config->wwDriftResetIntervalMin = 30; // 30 minutes
	config->wwMaxVarianceThreshold = 0.1f; // 10%
	config->wwMaxConsecutiveAdjustments = 10;
	config->wwMinSamplesForConfidence = 5;
	config->wwConfidenceDecayRate = 0.95f; // 95%
	config->wwMinLambdaStability = 0.15f; // 15%
	config->wwMaxCltForLearning = 110.0f;
	config->wwMinMapForLearning = 30; // 30 kPa
	config->wwEnableRobustValidation = true; // Habilitado por padrão
	config->wwEnableDriftReset = true; // Habilitado por padrão
	config->wwDirectionalCorrections = false; // Desabilitado por padrão para compatibilidade

	// Set up default wall wetting parameters based on
	// the H4 calibration
	config->wwaeTau = 0.25;
	config->wwaeBeta = 0.30;

	// LTFT (Long Term Fuel Trim) - parâmetros aprimorados
	config->ltftEmaAlpha = 32; // Valor padrão para suavização EMA
	config->ltftStftRejectThreshold = 10; // 10% de rejeição
	config->ltftRegionalWindow = 4; // 4 células de janela
	config->ltftRegionalIntensity = 50; // 50% de intensidade
	config->ltftHoleThreshold = 10; // 10% de diferença para suavização
	config->ltftSmoothingIntensity = 50; // 50% de suavização
	config->ltftIgnitionOnDelay = 10; // 10 segundos pós-ignição ON
	config->ltftIgnitionOffSaveDelay = 10; // 10 segundos pós-ignição OFF

	// Wall Wetting Adaptativo
	config->wwImmediateErrorThreshold = 0.05f;
	config->wwProlongedErrorThreshold = 0.03f;
	config->wBetaImediato = 0.8f;
	config->wBetaProlongado = 0.2f;
	config->wTauImediato = 0.2f;
	config->wTauProlongado = 0.8f;
// ... existing code ... 