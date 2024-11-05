//
// Created by kifir on 11/5/24.
//

#pragma once

using IgnitionTable = std::array<std::array<float, IGN_LOAD_COUNT>, IGN_RPM_COUNT>;

class TestPersistentConfiguration {
public:
    static TestPersistentConfiguration& getInstance();

    void setIgnitionTable(const IgnitionTable& ignitions);
private:
    static TestPersistentConfiguration instance;
};
