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
		}
	}

	// Set up default wall wetting parameters based on
	// the H4 calibration
	config->wwaeTau = 0.25;
	config->wwaeBeta = 0.30;
// ... existing code ... 