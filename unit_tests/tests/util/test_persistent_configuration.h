//
// Created by kifir on 11/5/24.
//

#pragma once

using IgnitionLoadBins = uint16_t[IGN_LOAD_COUNT];
using IgnitionRpmBins = uint16_t[IGN_RPM_COUNT];
using IgnitionTable = std::array<std::array<float, IGN_RPM_COUNT>, IGN_LOAD_COUNT>;
using InjectorStagingTable = std::array<std::array<uint8_t, INJ_STAGING_COUNT>, INJ_STAGING_COUNT>;
using CltFuelCorrCurve = std::array<float, CLT_CURVE_SIZE>;
using FuelLevelBinsCurve = std::array<float, FUEL_LEVEL_TABLE_COUNT>;
using FuelLevelValuesCurve = std::array<uint8_t, FUEL_LEVEL_TABLE_COUNT>;
using IgnTrimLoadBins = uint16_t[IGN_TRIM_SIZE];
using IgnTrimRpmBins = uint16_t[IGN_TRIM_SIZE];


class TestPersistentConfiguration {
public:
    static TestPersistentConfiguration& getInstance();

    void setIgnitionLoadBins(const IgnitionLoadBins& bins);
    void setIgnitionRpmBins(const IgnitionRpmBins& bins);
    void setIgnitionTable(const IgnitionTable& ignitions);
    void setInjectorStagingTable(const InjectorStagingTable& ingectorStaging);
    void setCltFuelCorrCurve(const CltFuelCorrCurve& cltFuelCorr);
	void setFuelLevelBinsCurve(const FuelLevelBinsCurve& fuelLevelBins);
	void setFuelLevelValuesCurve(const FuelLevelValuesCurve& fuelLevelValues);
    void setIgnTrimLoadBins(const IgnTrimLoadBins& bins);
    void setIgnTrimRpmBins(const IgnTrimRpmBins& bins);
private:
    static TestPersistentConfiguration instance;
};
