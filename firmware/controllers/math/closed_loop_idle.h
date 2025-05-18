#pragma once
#include "engine_module.h"
#include "rusefi_types.h"
#include "efi_pid.h"
#include "sensor.h"
#include "flash_main.h"
#include "table_helper.h"
class LongTermIdleTrim {
public:
    LongTermIdleTrim(engine_configuration_s* config);
    float getLtitFactor(float rpm, float clt) const;
    float getLtitAcTrim() const;
    float getLtitFan1Trim() const;
    float getLtitFan2Trim() const;
    void update(float rpm, float clt, bool acActive, bool fan1Active, bool fan2Active);
    float ltitTableHelper[16][16];
    float acTrim, fan1Trim, fan2Trim;
private:
    engine_configuration_s* config;
    float emaError = 0;
    uint32_t lastUpdateTime = 0;
    // ... outros campos internos ...
}; 