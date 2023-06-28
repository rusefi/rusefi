#pragma once

#include "hal_mfs.h"

#define PERSISTENCE_VERSION 41

#define MFS_CONFIGURATION_RECORD_ID     1

struct TestConfiguration {
    void resetToDefaults() {
        version = PERSISTENCE_VERSION;
        updateCounter = 100;
        rebootCounter = 10;
    }
    bool IsValid() const {
        return version == PERSISTENCE_VERSION;
    }

    int version;
    int updateCounter;
    int rebootCounter;
};

int InitConfiguration();
void pokeConfiguration();
