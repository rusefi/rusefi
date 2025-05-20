#include "pch.h"
#include "closed_loop_idle.h"
#include "engine_math.h"
#include "efitime.h"
#include "tunerstudio.h"
#include "event_queue.h"
#include "efi_gpio.h"
#include "engine.h"

LongTermIdleTrim::LongTermIdleTrim() {
    // Inicializar com valores padrão, não carrega da flash no construtor
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            ltitTableHelper[i][j] = 100.0f; // Valor padrão, será substituído quando loadLtitFromConfig for chamado
        }
    }
    acTrim = 0;
    fan1Trim = 0;
    fan2Trim = 0;
    emaError = 0;
    ltitTableInitialized = false;
}

bool LongTermIdleTrim::hasValidData() const {
    // Verifica se a tabela tem pelo menos alguns valores significativos
    // Consideramos válida se ao menos algumas células não forem zeros
    int nonZeroCount = 0;
    
    for (int i = 0; i < 16 && nonZeroCount < 3; i++) {
        for (int j = 0; j < 16 && nonZeroCount < 3; j++) {
            if (config->ltitTable[i][j] > 1.0f) { // Consideramos qualquer valor acima de 1 como não-zero significativo
                nonZeroCount++;
            }
        }
    }
    
    // Consideramos válido se há pelo menos 3 células com valores não-zero
    return nonZeroCount >= 3;
}

void LongTermIdleTrim::loadLtitFromConfig() {
    if (hasValidData()) {
        // Se a tabela de configuração tem dados válidos, copia para a tabela helper
        copyTable(ltitTableHelper, config->ltitTable, 1);
        acTrim = (float)config->ltitAcTrim / 10.0f;
        fan1Trim = (float)config->ltitFan1Trim / 10.0f;
        fan2Trim = (float)config->ltitFan2Trim / 10.0f;
        ltitTableInitialized = true;
    } 
}

float LongTermIdleTrim::getLtitFactor(float rpm, float clt) const {
    // Interpolação 3D:
    return interpolate3d(ltitTableHelper, config->cltIdleCorrBins, clt, config->rpmIdleCorrBins, rpm) * 0.01f;
}
float LongTermIdleTrim::getLtitAcTrim() const { return acTrim; }
float LongTermIdleTrim::getLtitFan1Trim() const { return fan1Trim; }
float LongTermIdleTrim::getLtitFan2Trim() const { return fan2Trim; }

void LongTermIdleTrim::update(float rpm, float clt, bool acActive, bool fan1Active, bool fan2Active, float idleIntegral) {
    if (!engineConfiguration->ltitEnabled)
        return;
    
    // Tenta carregar dados da flash periodicamente até conseguir
    if (!ltitTableInitialized) {
        loadLtitFromConfig();
    }
        
    (void)acActive; (void)fan1Active; (void)fan2Active;

    if(!ltitTableInitialized){
        return;
    }

    // Verificar se ignição está ligada há tempo suficiente
    if (!m_updateTimer.hasElapsedSec(engineConfiguration->ltitIgnitionOnDelay)) {
        // Reseta o timer de estabilidade se não passamos do delay de ignição
        m_stableIdleTimer.reset();
        isStableIdle = false;
        return;
    }

    // Verificar se estamos dentro da faixa de RPM considerada lenta e estável
    float rpmDelta = fabsf(rpm - engineConfiguration->targetIdleRpm);
    bool isIdleRpm = rpmDelta < engineConfiguration->ltitStableRpmThreshold;

    // Verificar estabilidade da lenta
    if (!isIdleRpm) {
        // Resetar timer de estabilidade se estiver fora da faixa de RPM
        m_stableIdleTimer.reset();
        isStableIdle = false;
        return;
    }

    // Se estamos na faixa de RPM, verificar se já está estável pelo tempo mínimo
    if (!isStableIdle && m_stableIdleTimer.hasElapsedSec(engineConfiguration->ltitStableTime)) {
        isStableIdle = true;
    }

    // Somente aplicar correções após estabilidade da lenta
    if (!isStableIdle) {
        return;
    }

    // Verificar se passou o tempo mínimo entre atualizações (padrão 1 segundo)
    float updateIntervalSeconds = 1.0f;
    if (!m_updateTimer.hasElapsedSec(updateIntervalSeconds)) {
        return;
    }
    m_updateTimer.reset();

    // Encontrar índices dos bins
    int i = 0, j = 0;
    for (int idx = 0; idx < 16; idx++) {
        if (clt < config->cltIdleCorrBins[idx]) { i = idx > 0 ? idx - 1 : 0; break; }
    }
    for (int idx = 0; idx < 16; idx++) {
        if (rpm < config->rpmIdleCorrBins[idx]) { j = idx > 0 ? idx - 1 : 0; break; }
    }
    // Correção multiplicativa baseada no termo integral do PID
    float correction = idleIntegral * (float)engineConfiguration->ltitCorrectionRate * 0.01f;
    ltitTableHelper[i][j] *= (1.0f + correction);
    // Clamping configurável
    float clampMin = engineConfiguration->ltitClampMin > 0 ? engineConfiguration->ltitClampMin : 50.0f;
    float clampMax = engineConfiguration->ltitClampMax > 0 ? engineConfiguration->ltitClampMax : 150.0f;
    if (ltitTableHelper[i][j] > clampMax) ltitTableHelper[i][j] = clampMax;
    if (ltitTableHelper[i][j] < clampMin) ltitTableHelper[i][j] = clampMin;
    // Remover trims AC/Fan com timers
    //acTrim, fan1Trim, fan2Trim podem ser ajustados diretamente se necessário
    // Após atualização da tabela, marcar aprendizado pendente
    updatedLtit = true;
    smoothLtitTable(engineConfiguration->ltitSmoothingIntensity);
}

void LongTermIdleTrim::onIgnitionStateChanged(bool ignitionOn) {
    m_ignitionState = ignitionOn;
    
    if (ignitionOn) {
        // Reset timers quando ignição liga
        m_updateTimer.reset();
        m_stableIdleTimer.reset();
        isStableIdle = false;
    } else if (updatedLtit) {
        // Salvar na memória persistente após delay de ignição OFF
        float saveDelaySeconds = engineConfiguration->ltitIgnitionOffSaveDelay;
        if (saveDelaySeconds <= 0) {
            // Se não configurado, usar valor padrão de 5 segundos
            saveDelaySeconds = 5.0f;
        }
        
        // Reset do timer de ignição OFF para controlar o delay de salvamento
        m_ignitionOffTimer.reset();
        
        // Na implementação atual salvamos imediatamente
        // O ideal seria verificar o timer no callback periódico, mas não temos acesso
        copyTable(config->ltitTable, ltitTableHelper);
        config->ltitAcTrim = (int16_t)(acTrim * 10.0f);
        config->ltitFan1Trim = (int16_t)(fan1Trim * 10.0f);
        config->ltitFan2Trim = (int16_t)(fan2Trim * 10.0f);
        setNeedToWriteConfiguration();
        
        updatedLtit = false;
    }
}

// Suavização regional da tabela LTIT
void LongTermIdleTrim::smoothLtitTable(float intensity) {
    //smoothTable<float, 16, 16>(ltitTableHelper, intensity / 100.0f);
    return;
} 