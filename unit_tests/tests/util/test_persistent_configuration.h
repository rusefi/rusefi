//
// Created by kifir on 11/5/24.
//

#pragma once

using IgnitionTable = std::array<std::array<float, IGN_LOAD_COUNT>, IGN_RPM_COUNT>;
using InjectorStagingTable = std::array<std::array<uint8_t, INJ_STAGING_COUNT>, INJ_STAGING_COUNT>;

class TestPersistentConfiguration {
public:
    static TestPersistentConfiguration& getInstance();

    void setIgnitionTable(const IgnitionTable& ignitions);
    void setInjectorStagingTable(const InjectorStagingTable& ingectorStaging);
private:
    static TestPersistentConfiguration instance;
};
