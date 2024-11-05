//
// Created by kifir on 11/5/24.
//

#include "pch.h"

#include "test_persistent_configuration.h"

TestPersistentConfiguration& TestPersistentConfiguration::getInstance() {
    return instance;
}

void TestPersistentConfiguration::setIgnitionTable(const IgnitionTable& ignitions) {
    for (int i = 0; i < IGN_LOAD_COUNT; i++) {
        for (int j = 0; j < IGN_RPM_COUNT; j++) {
            config->ignitionTable[i][j] = ignitions[i][j];
        }
    }
}

TestPersistentConfiguration TestPersistentConfiguration::instance;