#pragma once

#include "hal_mfs.h"

#define PERSISTENCE_VERSION 40

#define MFS_CONFIGURATION_RECORD_ID     1

struct TestConfiguration {
    void resetToDefaults() {
        version = PERSISTENCE_VERSION;
        updateCounter = 20;
    }
    bool IsValid() const {
        return version == PERSISTENCE_VERSION;
    }

    int version;
    int updateCounter;
};

int InitConfiguration();
void pokeConfiguration();
