//	setRpmBin(config->egrRpmBins, PACK_RPM_MULT);

	// Wall Wetting
	setLinearCurve(config->wwCltBins, -40, 15, CONFIG(wwCltBins));
	setArrayValues(config->wwTauCltValues, 100);
	setArrayValues(config->wwBetaCltValues, 60);

	setLinearCurve(config->wwMapBins, 20, 20, CONFIG(wwMapBins));
	setLinearCurve(config->wwRpmBins, 600, 600, CONFIG(wwRpmBins));
	
	// Initialize wall wetting 3D tables with default values (reduced size: 6x6)
	for (int rpm = 0; rpm < WWAE_RPM_SIZE; rpm++) {
		for (int load = 0; load < WWAE_TABLE_SIZE; load++) {
			config->wwTauMapRpmValues[rpm][load] = 100;
			config->wwBetaMapRpmValues[rpm][load] = 60;
			
			// Inicializar novas tabelas direcionais com valor padrão 100 (1.0 após escalonamento)
			config->wwBetaAccel[rpm][load] = 100;
			config->wwBetaDecel[rpm][load] = 100;
			config->wwTauAccel[rpm][load] = 100;
			config->wwTauDecel[rpm][load] = 100;
		}
	}
	
	// Simplified wall wetting configuration for memory optimization
	config->wwBufferSize = 200; // Reduced from 400
	config->wwTransientTimeoutMs = 1000;
	config->wwDirectionalCorrections = false; // Disabled by default
	config->wwLearningRate = 0.01f; // 1%
	config->wwSmoothIntensity = 0.1f; // 10%
	config->wwEnableAdaptiveLearning = false; // Disabled by default to save memory

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