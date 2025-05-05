//
// Created by kifir on 11/5/24.
//

#include "pch.h"

#include "test_persistent_configuration.h"

TestPersistentConfiguration& TestPersistentConfiguration::getInstance() {
    return instance;
}

void TestPersistentConfiguration::setIgnitionLoadBins(const IgnitionLoadBins& bins) {
	copyArray(config->ignitionLoadBins, bins);
}

void TestPersistentConfiguration::setIgnitionRpmBins(const IgnitionRpmBins& bins) {
    copyArray(config->ignitionRpmBins, bins);
}

void TestPersistentConfiguration::setIgnitionTable(const IgnitionTable& ignitions) {
    for (int i = 0; i < IGN_LOAD_COUNT; i++) {
        for (int j = 0; j < IGN_RPM_COUNT; j++) {
            config->ignitionTable[i][j] = ignitions[i][j];
        }
    }
}

void TestPersistentConfiguration::setInjectorStagingTable(const InjectorStagingTable& injectorStaging) {
    for (int i = 0; i < INJ_STAGING_COUNT; i++) {
        for (int j = 0; j < INJ_STAGING_COUNT; j++) {
            config->injectorStagingTable[i][j] = injectorStaging[i][j];
        }
    }
}

void TestPersistentConfiguration::setCltFuelCorrCurve(const CltFuelCorrCurve& cltFuelCorr) {
    std::copy(std::begin(cltFuelCorr), std::end(cltFuelCorr), std::begin(config->cltFuelCorr));
}

void TestPersistentConfiguration::setFuelLevelBinsCurve(const FuelLevelBinsCurve& fuelLevelBins) {
	for (size_t i = 0; i < fuelLevelBins.size(); i++) {
		config->fuelLevelBins[i] = fuelLevelBins[i];
	}
}

void TestPersistentConfiguration::setFuelLevelValuesCurve(const FuelLevelValuesCurve& fuelLevelValues) {
	std::copy(std::begin(fuelLevelValues), std::end(fuelLevelValues), std::begin(config->fuelLevelValues));
}

void TestPersistentConfiguration::setIgnTrimLoadBins(const IgnTrimLoadBins& bins) {
	copyArray(config->ignTrimLoadBins, bins);
}

void TestPersistentConfiguration::setIgnTrimRpmBins(const IgnTrimRpmBins& bins) {
    copyArray(config->ignTrimRpmBins, bins);
}

TestPersistentConfiguration TestPersistentConfiguration::instance;