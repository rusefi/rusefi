//
// Created by kifir on 11/5/24.
//

#pragma once

using IgnitionTable = std::array<std::array<float, IGN_RPM_COUNT>, IGN_LOAD_COUNT>;
using InjectorStagingTable = std::array<std::array<uint8_t, INJ_STAGING_COUNT>, INJ_STAGING_COUNT>;
using CltFuelCorrCurve = std::array<float, CLT_CURVE_SIZE>;
using FuelLevelBinsCurve = std::array<float, FUEL_LEVEL_TABLE_COUNT>;
using FuelLevelValuesCurve = std::array<uint8_t, FUEL_LEVEL_TABLE_COUNT>;

class TestPersistentConfiguration {
public:
    static TestPersistentConfiguration& getInstance();

    void setIgnitionTable(const IgnitionTable& ignitions);
    void setInjectorStagingTable(const InjectorStagingTable& ingectorStaging);
    void setCltFuelCorrCurve(const CltFuelCorrCurve& cltFuelCorr);
	void setFuelLevelBinsCurve(const FuelLevelBinsCurve& fuelLevelBins);
	void setFuelLevelValuesCurve(const FuelLevelValuesCurve& fuelLevelValues);
private:
    static TestPersistentConfiguration instance;
};
