//
// Created by FDSoftware on 02/05/25.
//

#pragma once

#include "pch.h"
#include "engine_configuration.h"

static void setTestFuelCrankingTable(float fuelMass) {
    setTable(config->crankingCycleBaseFuel, fuelMass);
    for (int cltIndex = 0;cltIndex<CRANKING_CYCLE_CLT_SIZE;cltIndex++) {
        // kludge: we have a few unit tests which depend on these magic numbers
        // misc.testFuelMap / FuelMath.getCycleFuelMassTest / cranking.testFasterEngineSpinningUp / engine.testPlainCrankingWithoutAdvancedFeatures / OddFireRunningMode.hd
        config->crankingCycleBaseFuel[cltIndex][0] = fuelMass * 2;
        config->crankingCycleBaseFuel[cltIndex][/*x - cycles*/1] = fuelMass * 1.3f;
    }
}
