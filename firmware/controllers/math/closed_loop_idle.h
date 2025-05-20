#pragma once
#include "engine_module.h"
#include "rusefi_types.h"
#include "efi_pid.h"
#include "sensor.h"
#include "flash_main.h"
#include "table_helper.h"
#include <rusefi/timer.h>

class LongTermIdleTrim {
public:
    LongTermIdleTrim();
    float getLtitFactor(float rpm, float clt) const;
    float getLtitAcTrim() const;
    float getLtitFan1Trim() const;
    float getLtitFan2Trim() const;
    void update(float rpm, float clt, bool acActive, bool fan1Active, bool fan2Active, float idleIntegral);
    float ltitTableHelper[16][16];
    float acTrim, fan1Trim, fan2Trim;
    void smoothLtitTable(float intensity);
    void onIgnitionStateChanged(bool ignitionOn);
    bool updatedLtit = false;
    
    // Nova função para carregar os dados da flash para a tabela helper
    void loadLtitFromConfig();
    // Verifica se a tabela tem dados válidos (não-zeros)
    bool hasValidData() const;
    
private:
    bool ltitTableInitialized = false;
    float emaError = 0;
    // Timers para controle de tempo
    Timer m_updateTimer;
    Timer m_stableIdleTimer;
    Timer m_ignitionOffTimer;
    bool isStableIdle = false;
    bool m_ignitionState = false;
}; 